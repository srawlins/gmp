#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

/*
 * Document-class: GMP::Q
 *
 * GMP Multiple Precision Rational Number.
 *
 * Instances of this class can store variables of the type mpq_t. This class
 * also contains many methods that act as the functions for mpq_t variables,
 * as well as a few methods that attempt to make this library more Ruby-ish.
 *
 * The following list is just a simple checklist for me, really. A better
 * reference should be found in the rdocs.
 *
 *   Ruby method  C Extension function  GMP function
 *   to_d         r_gmpq_to_d           mpq_get_d
 *   to_s         r_gmpq_to_s           ?
 *   add          r_gmpq_add            mpq_add
 *   sub          r_gmpq_sub            mpq_sub
 *   mul          r_gmpq_mul            mpq_mul
 *   div          r_gmpq_div            mpq_div
 */

/**********************************************************************
 *    Macros                                                          *
 **********************************************************************/

#define DEFUN_RAT_CMP(name,CMP_OP)                             \
static VALUE r_gmpq_cmp_##name(VALUE self, VALUE arg)          \
{                                                              \
  MP_RAT *self_val;                                            \
  mpq_get_struct(self,self_val);                               \
  return (mpq_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}

#define DEFUN_RAT2INT(fname,mpz_fname)                            \
static VALUE r_gmpq_##fname(VALUE self)                           \
{                                                                 \
  MP_RAT *self_val;                                               \
  MP_INT *res_val;                                                \
  VALUE res;                                                      \
                                                                  \
  mpq_get_struct(self, self_val);                                 \
  mpz_make_struct_init(res, res_val)                              \
  mpz_fname(res_val, mpq_numref(self_val), mpq_denref(self_val)); \
  return res;                                                     \
}


/**********************************************************************
 *    Initializing Rationals                                          *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::Q.new(arg)
 *
 * Creates a new GMP::Q rational, with arg as its value, converting where
 * necessary.
 */
VALUE r_gmpqsg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_RAT *res_val;
  VALUE res;

  (void)klass;

  if (argc > 2)
    rb_raise(rb_eArgError, "wrong # of arguments(%d for 0, 1 or 2)", argc);

  mpq_make_struct (res, res_val);
  mpq_init (res_val);
  rb_obj_call_init(res, argc, argv);
 
  return res;
}

/*
 * call-seq:
 *   GMP::Q(arg)
 *
 * A convenience method for +GMP::Q.new(arg)+.
 */
VALUE r_gmpmod_q(int argc, VALUE *argv, VALUE module)
{
  (void)module;
  return r_gmpqsg_new(argc, argv, cGMP_Q);
}

/*
 * call-seq:
 *   rat1.swap rat2
 *
 * Efficiently swaps the contents of +rat1+ with +rat2+.
 */
VALUE r_gmpq_swap(VALUE self, VALUE arg)
{
  MP_RAT *self_val, *arg_val;

  if (!GMPQ_P(arg)) {
    rb_raise(rb_eTypeError, "Can't swap GMP::Q with object of other class");
  }

  mpq_get_struct(self, self_val);
  mpq_get_struct(arg, arg_val);
  mpq_swap(self_val,arg_val);

  return Qnil;
}


/**********************************************************************
 *    Rational Conversions                                            *
 **********************************************************************/

/*
 * call-seq:
 *   rational.to_d
 *
 * Returns +rational+ as an Float if +rational+ fits in a Float.
 *
 * Otherwise returns the least significant part of +rational+, with the same
 * sign as +rational+.
 *
 * If the exponent from the conversion is too big or too small to fit a double
 * then the result is system dependent. For too big an infinity is returned
 * when available. For too small 0.0 is normally returned. Hardware overflow,
 * underflow and denorm traps may or may not occur. 
 */
VALUE r_gmpq_to_d(VALUE self)
{
  MP_RAT *self_val;
  mpq_get_struct(self, self_val);

  return rb_float_new(mpq_get_d(self_val));
}

/*
 * Document-method: to_s
 *
 * call-seq:
 *   rational.to_s
 *
 * Returns the decimal representation of +rational+, as a Ruby string.
 */
VALUE r_gmpq_to_s(VALUE self)
{
  MP_RAT *self_val;
  MP_INT *self_val_num, *self_val_den;
  char *str;
  VALUE res;
  int sizeinbase;
  int offset;

  Data_Get_Struct(self, MP_RAT, self_val);

  if (mpz_cmp_ui(mpq_denref(self_val), 1) == 0) {
    str = mpz_get_str(NULL, 10, mpq_numref (self_val));
    res = rb_str_new2(str);
    free (str);
    return res;
  }

  self_val_num = mpq_numref(self_val);
  self_val_den = mpq_denref(self_val);

  sizeinbase = mpz_sizeinbase (self_val_num, 10) + mpz_sizeinbase (self_val_den, 10) + 3;
  str = malloc (sizeinbase);

  mpz_get_str (str, 10, self_val_num);
  offset = strlen (str);
  str[offset] = '/';
  mpz_get_str (str + offset + 1, 10, self_val_den);
  res = rb_str_new2(str);
  free (str);

  return res;
}


/**********************************************************************
 *    Rational Arithmetic                                             *
 **********************************************************************/

/*
 * call-seq:
 *   rat1 + rat2
 *
 * Adds +rat1+ to +rat2+. +rat2+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpq_add(VALUE self, VALUE arg)
{
  MP_RAT *self_val, *arg_val_q, *res_val;
  MP_INT *arg_val_z, *res_val_num;
  VALUE res;

  mpq_get_struct(self, self_val);
  mpq_make_struct_init(res, res_val);

  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpq_add(res_val, self_val, arg_val_q);
  } else if (GMPZ_P(arg)) {
    res_val_num = mpq_numref(res_val);
    mpz_set(mpq_denref(res_val), mpq_denref(self_val));
    mpz_get_struct(arg, arg_val_z);
    mpz_mul(res_val_num, mpq_denref(self_val), arg_val_z);
    mpz_add(res_val_num, res_val_num, mpq_numref(self_val));
  } else if (FIXNUM_P(arg)) {
    res_val_num = mpq_numref(res_val);
    mpz_set(mpq_denref(res_val), mpq_denref(self_val));
    mpz_mul_si(res_val_num, mpq_denref(self_val), FIX2INT(arg));
    mpz_add(res_val_num, res_val_num, mpq_numref(self_val));
  } else if (GMPF_P(arg)) {
    return r_gmpf_add(arg,self);
  } else if (BIGNUM_P(arg)) {
    res_val_num = mpq_numref(res_val);
    mpz_set(mpq_denref(res_val), mpq_denref(self_val));
    mpz_set_bignum(res_val_num, arg);
    mpz_mul(res_val_num, res_val_num, mpq_denref(self_val));
    mpz_add(res_val_num, res_val_num, mpq_numref(self_val));
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   rat1 - rat2
 *
 * Subtracts +rat2+ from +rat1+. +rat2+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpq_sub(VALUE self, VALUE arg)
{
  MP_RAT *self_val, *arg_val_q, *res_val;
  MP_INT *arg_val_z, *res_val_num;
  MP_FLOAT *arg_val_f, *res_val_f;
  VALUE res;
  unsigned int prec;

  mpq_get_struct(self, self_val);
  mpq_make_struct_init(res, res_val);

  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpq_sub (res_val, self_val, arg_val_q);
  } else if (GMPZ_P(arg)) {
    res_val_num = mpq_numref(res_val);
    mpz_set (mpq_denref(res_val), mpq_denref(self_val));
    mpz_get_struct(arg, arg_val_z);
    mpz_mul (res_val_num, mpq_denref(self_val), arg_val_z);
    mpz_neg (res_val_num, res_val_num);
    mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
  } else if (FIXNUM_P(arg)) {
    res_val_num = mpq_numref(res_val);
    mpz_set (mpq_denref(res_val), mpq_denref(self_val));
    mpz_mul_si (res_val_num, mpq_denref(self_val), -FIX2INT(arg));
    mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
  } else if (GMPF_P(arg)) {
    mpf_get_struct_prec (arg, arg_val_f, prec);
    mpf_make_struct_init(res, res_val_f, prec);
    mpf_set_q (res_val_f, self_val);
    mpf_sub (res_val_f, res_val_f, arg_val_f);
  } else if (BIGNUM_P(arg)) {
    res_val_num = mpq_numref(res_val);
    mpz_set (mpq_denref(res_val), mpq_denref(self_val));
    mpz_set_bignum (res_val_num, arg);
    mpz_mul (res_val_num, res_val_num, mpq_denref(self_val));
    mpz_neg (res_val_num, res_val_num);
    mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
  } else {
    typeerror (ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   rat1 * rat2
 *
 * Multiplies +rat1+ with +rat2+. +rat2+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpq_mul(VALUE self, VALUE arg)
{
  MP_RAT *self_val, *arg_val_q, *res_val;
  MP_INT *arg_val_z, *tmp_z;
  VALUE res;
#if GMP >= 4
  unsigned long tmp_ui;
#endif

  mpq_get_struct(self, self_val);
  mpq_make_struct_init(res, res_val);

  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpq_mul(res_val, self_val, arg_val_q);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val_z);
    mpz_temp_init(tmp_z);
    mpz_gcd(tmp_z, mpq_denref(self_val), arg_val_z);
    mpz_divexact(mpq_denref(res_val), mpq_denref(self_val), tmp_z);
    mpz_divexact(mpq_numref(res_val), arg_val_z, tmp_z);
    mpz_mul(mpq_numref(res_val), mpq_numref(res_val), mpq_numref(self_val));
    mpz_temp_free(tmp_z);
  } else if (FIXNUM_P(arg)) {
#if GMP >= 4
    if (FIX2INT(arg) > 0) {
      tmp_ui = mpz_gcd_ui(0, mpq_denref(self_val), FIX2INT(arg));
    } else if (FIX2INT(arg) < 0) {
      tmp_ui = mpz_gcd_ui(0, mpq_denref(self_val), -FIX2INT(arg));
    } else {
      mpz_set_ui(mpq_numref(res_val), 0);
      mpz_set_ui(mpq_denref(res_val), 1);
      return res;
    }
    mpz_divexact_ui(mpq_denref(res_val), mpq_denref(self_val), tmp_ui);
    mpz_mul_ui(mpq_numref(res_val), mpq_numref(self_val), FIX2INT(arg)/tmp_ui);
#else
    mpz_set(mpq_denref(res_val), mpq_denref(self_val));
    mpz_mul_si(mpq_numref(res_val), mpq_numref(self_val), FIX2INT(arg));
    mpq_canonicalize(res_val);
#endif
  } else if (GMPF_P(arg)) {
    return r_gmpf_mul(arg, self);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_alloc(tmp_z);
    mpz_set_bignum(tmp_z, arg);
    mpz_gcd(mpq_denref(res_val), mpq_denref(self_val), tmp_z);
    mpz_divexact(mpq_numref(res_val), tmp_z, mpq_denref(res_val));
    mpz_divexact(mpq_denref(res_val), mpq_denref(self_val), mpq_denref(res_val));
    mpz_mul(mpq_numref(res_val), mpq_numref(res_val), mpq_numref(self_val));
    mpz_temp_free(tmp_z);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   rat1 / rat2
 *
 * Divides +rat1+ by +rat2+. +rat2+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpq_div(VALUE self, VALUE arg)
{
  MP_RAT *self_val, *arg_val_q, *res_val;
  MP_INT *arg_val_z, *tmp_z;
  MP_FLOAT *arg_val_f, *res_val_f;
  VALUE res;
  unsigned long tmp_ui, prec;

  mpq_get_struct(self, self_val);
  mpq_make_struct_init(res, res_val);

  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    if (mpz_sgn(mpq_numref(arg_val_q)) == 0)
      rb_raise(rb_eZeroDivError, "divided by 0");
    mpq_div(res_val, self_val, arg_val_q);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val_z);
    mpz_temp_init(tmp_z);
    mpz_gcd(tmp_z, mpq_numref(self_val), arg_val_z);
    mpz_divexact(mpq_numref(res_val), mpq_numref(self_val), tmp_z);
    mpz_divexact(mpq_denref(res_val), arg_val_z, tmp_z);
    mpz_mul(mpq_denref(res_val), mpq_denref(res_val), mpq_denref(self_val));
    mpz_temp_free(tmp_z);
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) == 0)
      rb_raise(rb_eZeroDivError, "divided by 0");
    if (FIX2INT(arg) > 0) {
      tmp_ui = mpz_gcd_ui(0, mpq_numref(self_val), FIX2INT(arg));
    } else {
      tmp_ui = mpz_gcd_ui(0, mpq_numref(self_val), -FIX2INT(arg));
    }
    mpz_divexact_ui(mpq_numref(res_val), mpq_numref(self_val), tmp_ui);
    mpz_mul_ui(mpq_denref(res_val), mpq_denref(self_val), FIX2INT(arg)/tmp_ui);
  } else if (GMPF_P(arg)) {
    mpf_get_struct_prec(arg, arg_val_f, prec);
    mpf_make_struct_init(res, res_val_f, prec);
    mpf_set_q(res_val_f, self_val);
    mpf_div(res_val_f, res_val_f, arg_val_f);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_alloc(tmp_z);
    mpz_set_bignum(tmp_z, arg);
    mpz_gcd(mpq_numref(res_val), mpq_numref(self_val), tmp_z);
    mpz_divexact(mpq_denref(res_val), tmp_z, mpq_numref(res_val));
    mpz_divexact(mpq_numref(res_val), mpq_numref(self_val), mpq_numref(res_val));
    mpz_mul(mpq_denref(res_val), mpq_denref(res_val), mpq_denref(self_val));
    mpz_temp_free(tmp_z);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

DEFUN_RAT2INT(floor,mpz_fdiv_q)
DEFUN_RAT2INT(trunc,mpz_tdiv_q)
DEFUN_RAT2INT(ceil,mpz_cdiv_q)

/*
 * Document-method: neg
 *
 * call-seq:
 *   -rational
 *   rational.neg
 *
 * From the GMP Manual:
 * 
 * Returns -+rational+.
 */
VALUE r_gmpq_neg(VALUE self)
{
  MP_RAT *self_val, *res_val;
  VALUE res;
  mpq_get_struct(self, self_val);
  mpq_make_struct_init(res, res_val);
  mpq_neg (res_val, self_val);
  return res;
}

/*
 * Document-method: neg!
 *
 * call-seq:
 *   rational.neg!
 *
 * From the GMP Manual:
 * 
 * Sets +rational+ to -+rational+.
 */
VALUE r_gmpq_neg_self(VALUE self)
{
  MP_RAT *self_val;
  mpq_get_struct(self, self_val);
  mpz_neg (mpq_numref(self_val), mpq_numref(self_val));
  return Qnil;
}

/*
 * Document-method: abs
 *
 * call-seq:
 *   rational.abs
 *
 * From the GMP Manual:
 * 
 * Returns the absolute value of +rational+.
 */
VALUE r_gmpq_abs(VALUE self)
{
  MP_RAT *self_val, *res_val;
  VALUE res;
  mpq_get_struct(self, self_val);
  mpq_make_struct_init(res, res_val);
  mpz_abs (mpq_numref(res_val), mpq_numref(self_val));
  mpz_set (mpq_denref(res_val), mpq_denref(self_val));

  return res;
}

/*
 * Document-method: abs!
 *
 * call-seq:
 *   rational.abs!
 *
 * From the GMP Manual:
 * 
 * Sets +rational+ to its absolute value.
 */
VALUE r_gmpq_abs_self(VALUE self)
{
  MP_RAT *self_val;
  mpq_get_struct(self, self_val);
  mpz_abs (mpq_numref(self_val), mpq_numref(self_val));
  return Qnil;
}

VALUE r_gmpq_inv(VALUE self)
{
  MP_RAT *self_val, *res_val;
  VALUE res;

  mpq_get_struct(self, self_val);
  if (mpq_sgn(self_val) == 0)
    rb_raise (rb_eZeroDivError, "divided by 0");
  mpq_make_struct_init(res, res_val);
  mpq_inv (res_val, self_val);

  return res;
}

VALUE r_gmpq_inv_self(VALUE self)
{
  MP_RAT *self_val;
  mpq_get_struct(self, self_val);
  if (mpq_sgn(self_val) == 0)
    rb_raise (rb_eZeroDivError, "divided by 0");
  mpq_inv (self_val, self_val);
  return Qnil;
}


/**********************************************************************
 *    Comparing Rationals                                             *
 **********************************************************************/

int mpq_cmp_value(MP_RAT *OP, VALUE arg)
{
  MP_INT *arg_val_z, *tmp_z;
  MP_RAT *arg_val_q;
  int res;

  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    return mpq_cmp(OP,arg_val_q);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    mpz_temp_alloc(tmp_z);
    mpz_init(tmp_z);
    mpz_mul(tmp_z, mpq_denref(OP), arg_val_z);
    res = mpz_cmp(mpq_numref(OP),tmp_z);
    mpz_temp_free(tmp_z);
    return res;
  } else if (FIXNUM_P(arg)) {
    mpz_temp_alloc(tmp_z);
    mpz_init(tmp_z);
    mpz_mul_si(tmp_z, mpq_denref(OP), FIX2INT(arg));
    res = mpz_cmp(mpq_numref(OP), tmp_z);
    mpz_temp_free(tmp_z);
    return res;
  } else if (GMPF_P(arg)) {
    not_yet;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(tmp_z, arg);
    mpz_mul(tmp_z, tmp_z, mpq_denref(OP));
    res = mpz_cmp(mpq_numref(OP), tmp_z);
    mpz_temp_free(tmp_z);
    return res;
  } else {
    typeerror(ZQFXB);
  }
}

VALUE r_gmpq_eq(VALUE self, VALUE arg)
{
  MP_RAT *self_val, *arg_val_q;
  MP_INT *arg_val_z;

  mpq_get_struct(self,self_val);
  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    return mpq_equal(self_val,arg_val_q)?Qtrue:Qfalse;
  } else if (GMPZ_P(arg)) {
    if (mpz_cmp_ui(mpq_denref(self_val), 1) != 0)
      return Qfalse;
    mpz_get_struct (arg, arg_val_z);
    return (mpz_cmp(mpq_numref(self_val),arg_val_z)==0)?Qtrue:Qfalse;
  } else if (FIXNUM_P(arg)) {
    if (mpz_cmp_ui(mpq_denref(self_val), 1) != 0)
      return Qfalse;
    return (mpz_cmp_ui(mpq_numref(self_val),FIX2INT(arg))==0)?Qtrue:Qfalse;
  } else if (BIGNUM_P(arg)) {
    if (mpz_cmp_ui(mpq_denref(self_val), 1) != 0)
      return Qfalse;
    mpz_temp_from_bignum(arg_val_z, arg);
    if (mpz_cmp (mpq_numref(self_val),arg_val_z) == 0) {
      mpz_temp_free (arg_val_z);
      return Qtrue;
    } else {
      mpz_temp_free (arg_val_z);
      return Qfalse;
    }
  } else {
    return Qfalse;
  }
}

VALUE r_gmpq_cmp(VALUE self, VALUE arg)
{
  MP_RAT *self_val;
  int res;
  mpq_get_struct (self,self_val);
  res = mpq_cmp_value(self_val, arg);
  if (res > 0)
    return INT2FIX(1);
  else if (res == 0)
    return INT2FIX(0);
  else
    return INT2FIX(-1);
}

DEFUN_RAT_CMP(lt,<)
DEFUN_RAT_CMP(le,<=)
DEFUN_RAT_CMP(gt,>)
DEFUN_RAT_CMP(ge,>=)

static VALUE r_gmpq_cmpabs(VALUE self, VALUE arg)
{
  MP_RAT *arg_val_q, *self_val;
  MP_INT *arg_val_z, *tmp_z;
  int res;
  int sgnt;

  mpq_get_struct(self, self_val);

  if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    sgnt = 3*mpz_sgn(mpq_numref(self_val)) + mpz_sgn(mpq_numref(arg_val_q));
    switch (sgnt)
    {
    default:
    case 0:
      return INT2FIX(0);
    case 1:
    case -1:
      return INT2FIX(-1);
    case 2:
      tmp_z = mpq_numref(arg_val_q);
      mpz_neg (tmp_z, tmp_z);
      res = mpq_cmp (self_val, arg_val_q);
      mpz_neg (tmp_z, tmp_z);
      return res;
    case -2:
      tmp_z = mpq_numref(arg_val_q);
      mpz_neg (tmp_z, tmp_z);
      res = mpq_cmp (self_val, arg_val_q);
      mpz_neg (tmp_z, tmp_z);
      return res;
    case 3:
    case -3:
      return INT2FIX(1);
    case 4:
    case -4:
      return INT2FIX(mpq_cmp (self_val,arg_val_q));
    }
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    mpz_temp_alloc (tmp_z);
    mpz_init (tmp_z);
    mpz_mul (tmp_z, mpq_denref(self_val), arg_val_z);
    res = mpz_cmpabs (mpq_numref(self_val),tmp_z);
    mpz_temp_free (tmp_z);
    return res;
  } else if (FIXNUM_P(arg)) {
    mpz_temp_alloc (tmp_z);
    mpz_init (tmp_z);
    mpz_mul_si (tmp_z, mpq_denref(self_val), FIX2INT(arg));
    res = mpz_cmpabs (mpq_numref(self_val), tmp_z);
    mpz_temp_free (tmp_z);
    return res;
  } else if (GMPF_P(arg)) {
    not_yet;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum (tmp_z, arg);
    mpz_mul (tmp_z, tmp_z, mpq_denref(self_val));
    res = mpz_cmpabs (mpq_numref(self_val), tmp_z);
    mpz_temp_free (tmp_z);
    return res;
  } else {
    typeerror (ZQFXB);
  }
}

/*
 * call-seq:
 *   rational.sgn
 *
 * From the GMP Manual:
 * 
 * Returns +1 if +rational+ > 0, 0 if +rational+ == 0, and -1 if +rational+ < 0.
 */
VALUE r_gmpq_sgn(VALUE self)
{
  MP_RAT *self_val;
  mpq_get_struct(self, self_val);
  return INT2FIX(mpq_sgn(self_val));
}

/**********************************************************************
 *    Applying Integer Functions                                      *
 **********************************************************************/

VALUE r_gmpq_num(VALUE self)
{
  MP_RAT *self_val;
  MP_INT *res_val;
  VALUE res;
  mpq_get_struct(self,self_val);
  mpz_make_struct(res, res_val);
  mpz_init_set (res_val, mpq_numref (self_val));
  return res;
}

VALUE r_gmpq_den(VALUE self)
{
  MP_RAT *self_val;
  MP_INT *res_val;
  VALUE res;
  mpq_get_struct(self,self_val);
  mpz_make_struct(res, res_val);
  mpz_init_set (res_val, mpq_denref (self_val));
  return res;
}


void init_gmpq()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "Q", r_gmpmod_q, -1);

  cGMP_Q = rb_define_class_under (mGMP, "Q", rb_cNumeric);

  // Initializing Rationals
  rb_define_singleton_method(cGMP_Q, "new", r_gmpqsg_new, -1);
  rb_define_method(cGMP_Q, "swap",  r_gmpq_swap, 1);
  
  // Rational Conversions
  rb_define_method(cGMP_Q, "to_d", r_gmpq_to_d, 0);
  rb_define_alias(cGMP_Q, "to_f", "to_d");
  rb_define_method(cGMP_Q, "to_s", r_gmpq_to_s, 0);
  
  // Rational Arithmetic
  rb_define_method(cGMP_Q, "+", r_gmpq_add, 1);
  rb_define_method(cGMP_Q, "-", r_gmpq_sub, 1);
  rb_define_method(cGMP_Q, "*", r_gmpq_mul, 1);
  rb_define_method(cGMP_Q, "/", r_gmpq_div, 1);
  rb_define_method(cGMP_Q, "-@", r_gmpq_neg, 0);
  rb_define_method(cGMP_Q, "neg!", r_gmpq_neg_self, 0);
  rb_define_method(cGMP_Q, "inv", r_gmpq_inv, 0);
  rb_define_method(cGMP_Q, "inv!", r_gmpq_inv_self, 0);
  rb_define_method(cGMP_Q, "abs", r_gmpq_abs, 0);
  rb_define_method(cGMP_Q, "abs!", r_gmpq_abs_self, 0);

  // Comparing Rationals
  rb_define_method(cGMP_Q, "<=>", r_gmpq_cmp, 1);
  rb_define_method(cGMP_Q, ">",   r_gmpq_cmp_gt, 1);
  rb_define_method(cGMP_Q, ">=",  r_gmpq_cmp_ge, 1);
  rb_define_method(cGMP_Q, "<",   r_gmpq_cmp_lt, 1);
  rb_define_method(cGMP_Q, "<=",  r_gmpq_cmp_le, 1);
  rb_define_method(cGMP_Q, "==",  r_gmpq_eq, 1);
  rb_define_method(cGMP_Q, "sgn", r_gmpq_sgn, 0);
  rb_define_method(cGMP_Q, "cmpabs",  r_gmpq_cmpabs, 1);
  
  // _unsorted_
  rb_define_method(cGMP_Q, "floor",  r_gmpq_floor, 0);
  rb_define_method(cGMP_Q, "ceil",  r_gmpq_ceil, 0);
  rb_define_method(cGMP_Q, "trunc",  r_gmpq_trunc, 0);
}
