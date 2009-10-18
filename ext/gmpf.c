#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

/*
 * Document-class: GMP::F
 *
 * GMP Multiple Precision floating point numbers.
 *
 * Instances of this class can store variables of the type mpf_t. This class
 * also contains many methods that act as the functions for mpf_t variables,
 * as well as a few methods that attempt to make this library more Ruby-ish.
 */

/**********************************************************************
 *    Macros                                                          *
 **********************************************************************/

#define DEFUN_FLOAT2FLOAT(fname,mpf_fname)                    \
static VALUE r_gmpf_##fname(VALUE self)                       \
{                                                             \
  MP_FLOAT *self_val, *res_val;                               \
  VALUE res;                                                  \
  mpf_get_struct(self, self_val);                             \
  mpf_make_struct_init(res, res_val, mpf_get_prec(self_val)); \
  mpf_fname(res_val, self_val);                               \
  return res;                                                 \
}                                                             \
                                                              \
static VALUE r_gmpf_##fname##_self(VALUE self)                \
{                                                             \
  MP_FLOAT *self_val;                                         \
  mpf_get_struct(self, self_val);                             \
  mpf_fname(self_val, self_val);                              \
  return Qnil;                                                \
}

#define DEFUN_FLOAT_CMP(name,CMP_OP)                           \
static VALUE r_gmpf_cmp_##name(VALUE self, VALUE arg)          \
{                                                              \
  MP_FLOAT *self_val;                                          \
  mpf_get_struct(self,self_val);                               \
  return (mpf_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}


/**********************************************************************
 *    Initializing, Assigning Floats                                  *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::R.new(arg)
 *
 * Creates a new GMP::R float, with arg as its value, converting where
 * necessary.
 */
VALUE r_gmpfsg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_FLOAT *res_val;
  VALUE res;

  (void)klass;

  if (argc > 2)
    rb_raise(rb_eArgError, "wrong # of arguments(%d for 0, 1 or 2)", argc);

  mpf_make_struct (res, res_val);
  rb_obj_call_init(res, argc, argv);

  return res;
}

VALUE r_gmpf_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_FLOAT *self_val, *arg_val_f;
  unsigned long prec = 0;
  VALUE arg;

  mpf_get_struct (self, self_val);

  if (argc==0) {
    mpf_init(self_val);
    mpf_set_si(self_val, 0);
    return Qnil;
  }

  arg = argv[0];

  if (argc == 2) {
    if (FIXNUM_P(argv[1])) {
      if (FIX2INT(argv[1]) >= 0)
        prec = FIX2INT(argv[1]);
      else
        rb_raise(rb_eRangeError, "prec must be non-negative");
    } else {
      rb_raise(rb_eTypeError, "prec must be FixNum");
    }
  } else if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    prec = mpf_get_prec (arg_val_f);
  }
  if (prec == 0)
    mpf_init (self_val);
  else
    mpf_init2 (self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    mpf_set(self_val, arg_val_f);
  } else {
    mpf_set_value(self_val, arg);
  }

  return Qnil;
}

/*
 * call-seq:
 *   GMP::F(arg)
 *
 * A convenience method for +GMP::F.new(arg)+.
 */
VALUE r_gmpmod_f(int argc, VALUE *argv, VALUE module)
{
  (void)module;
  return r_gmpfsg_new(argc, argv, cGMP_F);
}


/**********************************************************************
 *    Converting Floats                                               *
 **********************************************************************/

VALUE r_gmpf_to_d(VALUE self)
{
  MP_FLOAT *self_val;
  mpf_get_struct(self, self_val);

  return rb_float_new(mpf_get_d(self_val));
}

/*
 * Document-method: to_s
 *
 * call-seq:
 *   float.to_s
 *
 * Returns the decimal representation of +float+, as a string.
 */
VALUE r_gmpf_to_s(VALUE self)
{
  MP_FLOAT *self_val;
  char *str, *str2;
  VALUE res;
  mp_exp_t exponent;

  mpf_get_struct(self, self_val);

  str = mpf_get_str(NULL, &exponent, 10, 0, self_val);
  if ((strcmp(str,  "NaN") == 0) ||
      (strcmp(str,  "Inf") == 0) ||
      (strcmp(str, "-Inf") == 0))
  {
    res = rb_str_new2(str);
  }
  else
  {
    if (str[0] == '-')
      __gmp_asprintf(&str2, "-0.%se%+ld", str+1, exponent);
    else
      __gmp_asprintf(&str2, "0.%se%+ld", str, exponent);
    res = rb_str_new2(str2);
    free(str2);
  }
  free(str);
  return res;
}


/**********************************************************************
 *    Float Arithmetic                                                *
 **********************************************************************/

/*
 * call-seq:
 *   float + other
 *
 * Returns the sum of +float+ and +other+. +other+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 * * Float
 */
VALUE r_gmpf_add(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  VALUE res;
  unsigned long prec;

  mpf_get_struct_prec (self, self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    prec_max(prec, arg_val_f);
    mpf_make_struct_init(res, res_val, prec);
    mpf_add(res_val, self_val, arg_val_f);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct (arg, arg_val_q);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_q (res_val, arg_val_q);
    mpf_add (res_val, res_val, self_val);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct (arg, arg_val_z);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z (res_val, arg_val_z);
    mpf_add (res_val, res_val, self_val);
  } else if (FLOAT_P(arg)) {
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_d (res_val, NUM2DBL(arg));
    mpf_add (res_val, res_val, self_val);
  } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_si (res_val, FIX2INT(arg));
    mpf_add (res_val, res_val, self_val);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z (res_val, arg_val_z);
    mpf_add (res_val, res_val, self_val);
    mpz_temp_free(arg_val_z);
  } else {
    typeerror(ZQFXBD);
  }

  return res;
}

/*
 * call-seq:
 *   float1 - float2
 *
 * Subtracts +float2+ from +float1+. +float2+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 * * Float
 */
VALUE r_gmpf_sub(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  VALUE res;
  unsigned long prec;

  mpf_get_struct_prec (self, self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct(arg, arg_val_f);
    prec_max(prec, arg_val_f);
    mpf_make_struct_init(res, res_val, prec);
    mpf_sub(res_val, self_val, arg_val_f);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_q(res_val, arg_val_q);
    mpf_sub(res_val, self_val, res_val);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z(res_val, arg_val_z);
    mpf_sub(res_val, self_val, res_val);
  } else if (FLOAT_P(arg)) {
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_d(res_val, NUM2DBL(arg));
    mpf_sub(res_val, self_val, res_val);
  } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_si(res_val, FIX2INT(arg));
    mpf_sub(res_val, self_val, res_val);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z(res_val, arg_val_z);
    mpf_sub(res_val, self_val, res_val);
    mpz_temp_free(arg_val_z);
  } else {
    typeerror(ZQFXBD);
  }

  return res;
}

/*
 * call-seq:
 *   float * other
 *
 * Returns the product of +float+ and +other+. +other+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 * * Float
 */
VALUE r_gmpf_mul(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  VALUE res;
  unsigned long prec;

  mpf_get_struct_prec (self, self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct(arg, arg_val_f);
    prec_max(prec, arg_val_f);
    mpf_make_struct_init(res, res_val, prec);
    mpf_mul(res_val, self_val, arg_val_f);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_q(res_val, arg_val_q);
    mpf_mul(res_val, self_val, res_val);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z(res_val, arg_val_z);
    mpf_mul(res_val, self_val, res_val);
  } else if (FLOAT_P(arg)) {
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_d(res_val, NUM2DBL(arg));
    mpf_mul(res_val, self_val, res_val);
  } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_si(res_val, FIX2INT(arg));
    mpf_mul(res_val, self_val, res_val);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z(res_val, arg_val_z);
    mpf_mul(res_val, res_val, self_val);
    mpz_temp_free(arg_val_z);
  } else {
    typeerror(ZQFXBD);
  }

  return res;
}

/*
 * call-seq:
 *   float1 / float2
 *
 * Divides +float1+ by +float2+. +float2+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 * * Float
 */
VALUE r_gmpf_div(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  VALUE res;
  unsigned long prec;

  mpf_get_struct_prec (self, self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    prec_max(prec, arg_val_f);
    mpf_make_struct_init(res, res_val, prec);
    mpf_div(res_val, self_val, arg_val_f);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct (arg, arg_val_q);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_q (res_val, arg_val_q);
    mpf_div (res_val, self_val, res_val);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct (arg, arg_val_z);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z (res_val, arg_val_z);
    mpf_div (res_val, self_val, res_val);
  } else if (FLOAT_P(arg)) {
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_d (res_val, NUM2DBL(arg));
    mpf_div (res_val, self_val, res_val);
  } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_si (res_val, FIX2INT(arg));
    mpf_div (res_val, self_val, res_val);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z (res_val, arg_val_z);
    mpf_div (res_val, self_val, res_val);	
    mpz_temp_free(arg_val_z);
  } else {
    typeerror(ZQFXBD);
  }

  return res;
}

/*
 * Document-method: neg
 *
 * call-seq:
 *   float.neg
 *   -float
 *
 * From the GMP Manual:
 * 
 * Returns -+float+.
 */
/*
 * Document-method: neg!
 *
 * call-seq:
 *   float.neg!
 *
 * Sets +float+ to -+float+.
 */
DEFUN_FLOAT2FLOAT(neg,mpf_neg)
/*
 * Document-method: abs
 *
 * call-seq:
 *   float.abs
 *
 * From the GMP Manual:
 * 
 * Returns the absolute value of +float+.
 */
/*
 * Document-method: abs!
 *
 * call-seq:
 *   float.abs!
 *
 * Sets +float+ to the absolute value of +float+.
 */
DEFUN_FLOAT2FLOAT(abs,mpf_abs)


/**********************************************************************
 *    Float Comparison                                                *
 **********************************************************************/

int mpf_cmp_value(MP_FLOAT *self_val, VALUE arg)
{
  MP_FLOAT *arg_val;
  int result;

  if (GMPF_P(arg)) {
    mpf_get_struct(arg,arg_val);
    return mpf_cmp (self_val, arg_val);
  } else {
    mpf_temp_init(arg_val, mpf_get_prec (self_val));
    mpf_set_value (arg_val, arg);
    result = mpf_cmp (self_val, arg_val);
    mpf_temp_free(arg_val);
    return result;
  }
}

/* what does really "equal" mean ? it's not obvious */
VALUE r_gmpf_eq(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val;
  mpf_get_struct (self,self_val);
  return (mpf_cmp_value(self_val, arg) == 0) ? Qtrue : Qfalse;
}

VALUE r_gmpf_cmp(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val;
  int res;
  mpf_get_struct(self,self_val);
  res = mpf_cmp_value(self_val, arg);
  if (res > 0)
    return INT2FIX(1);
  else if (res == 0)
    return INT2FIX(0);
  else
    return INT2FIX(-1);
}

DEFUN_FLOAT_CMP(lt,<)
DEFUN_FLOAT_CMP(le,<=)
DEFUN_FLOAT_CMP(gt,>)
DEFUN_FLOAT_CMP(ge,>=)


/**********************************************************************
 *    _unsorted_                                                      *
 **********************************************************************/

DEFUN_FLOAT2FLOAT(floor,mpf_floor)
DEFUN_FLOAT2FLOAT(trunc,mpf_trunc)
DEFUN_FLOAT2FLOAT(ceil,mpf_ceil)

/*
 * call-seq:
 *   float.sgn
 *
 * From the GMP Manual:
 * 
 * Returns +1 if +float+ > 0, 0 if +float+ == 0, and -1 if +float+ < 0.
 */
VALUE r_gmpf_sgn(VALUE self)
{
  MP_FLOAT *self_val;
  mpf_get_struct(self, self_val);
  return INT2FIX(mpf_sgn(self_val));
}

VALUE r_gmpf_get_prec(VALUE self)
{
  MP_FLOAT *self_val;
  mpf_get_struct(self, self_val);
  return INT2NUM(mpf_get_prec(self_val));
}


void init_gmpf()
{
  mGMP = rb_define_module("GMP");
  // rb_define_module_function(mGMP, "Z", r_gmpmod_z, -1);
  // rb_define_module_function(mGMP, "Q", r_gmpmod_q, -1);
  rb_define_module_function(mGMP, "F", r_gmpmod_f, -1);
  
  cGMP_F = rb_define_class_under (mGMP, "F", rb_cNumeric);
  
  
  // Initializing, Assigning Floats
  rb_define_singleton_method(cGMP_F, "new", r_gmpfsg_new, -1);
  rb_define_method(cGMP_F, "initialize", r_gmpf_initialize, -1);
  
  // Converting Floats
  rb_define_method(cGMP_F, "to_s", r_gmpf_to_s, 0);
  rb_define_method(cGMP_F, "to_d",  r_gmpf_to_d, 0);
  rb_define_alias(cGMP_F, "to_f", "to_d");
  
  // Float Arithmetic
  rb_define_method(cGMP_F, "+", r_gmpf_add, 1);
  rb_define_method(cGMP_F, "-", r_gmpf_sub, 1);
  rb_define_method(cGMP_F, "*", r_gmpf_mul, 1);
  rb_define_method(cGMP_F, "/", r_gmpf_div, 1);
  rb_define_method(cGMP_F, "-@", r_gmpf_neg, 0);
  rb_define_method(cGMP_F, "neg!", r_gmpf_neg_self, 0);
  rb_define_method(cGMP_F, "abs", r_gmpf_abs, 0);
  rb_define_method(cGMP_F, "abs!", r_gmpf_abs_self, 0);
  
  // Float Comparison
  rb_define_method(cGMP_F, "<=>", r_gmpf_cmp, 1);
  rb_define_method(cGMP_F, ">",   r_gmpf_cmp_gt, 1);
  rb_define_method(cGMP_F, ">=",  r_gmpf_cmp_ge, 1);
  rb_define_method(cGMP_F, "<",   r_gmpf_cmp_lt, 1);
  rb_define_method(cGMP_F, "<=",  r_gmpf_cmp_le, 1);
  rb_define_method(cGMP_F, "==",  r_gmpf_eq, 1);
  
  // _unsorted_
  rb_define_method(cGMP_F, "floor",  r_gmpf_floor, 0);
  rb_define_method(cGMP_F, "floor!",  r_gmpf_floor_self, 0);
  rb_define_method(cGMP_F, "ceil",  r_gmpf_ceil, 0);
  rb_define_method(cGMP_F, "ceil!",  r_gmpf_ceil_self, 0);
  rb_define_method(cGMP_F, "trunc",  r_gmpf_trunc, 0);
  rb_define_method(cGMP_F, "trunc!",  r_gmpf_trunc_self, 0);
  rb_define_method(cGMP_F, "sgn", r_gmpf_sgn, 0);
  rb_define_method(cGMP_F, "prec", r_gmpf_get_prec, 0);
}
