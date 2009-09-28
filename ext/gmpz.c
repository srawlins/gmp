#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

/*
 * Document-class: GMP::Z
 *
 * GMP Multiple Precision Integer.
 *
 * Instances of this class can store variables of the type mpz_t. This class
 * also contains many methods that act as the functions for mpz_t variables,
 * as well as a few methods that attempt to make this library more Ruby-ish.
 *
 * The following list is just a simple checklist for me, really. A better
 * reference should be found in the rdocs.
 *
 *   Ruby method  C Extension function  GMP function
 *   +            r_gmpz_add            mpz_add
 *   add!         r_gmpz_add_self       mpz_add
 *   -            r_gmpz_sub            mpz_sub
 *   sub!         r_gmpz_sub_self       mpz_sub
 *   *            r_gmpz_mul            mpz_mul
 *   /            r_gmpz_div            ...
 *   tdiv         r_gmpz_tdiv           mpz_tdiv_q
 *   tmod         r_gmpz_tmod           mpz_tdiv_r
 *   fdiv         r_gmpz_fdiv           mpz_fdiv_q
 *   fmod         r_gmpz_fmod           mpz_fdiv_r
 *   cdiv         r_gmpz_cdiv           mpz_cdiv_q
 *   cmod         r_gmpz_cmod           mpz_cdiv_r
 *   abs          r_gmpz_abs            mpz_abs
 *   abs!         r_gmpz_abs_self       mpz_abs
 *   -@           r_gmpz_neg            mpz_neg
 *   neg          r_gmpz_neg            mpz_neg
 *   neg!         r_gmpz_neg_self       mpz_neg
 *   com          r_gmpz_com            mpz_com
 *   com!         r_gmpz_com_self       mpz_com
 *   []=          r_gmpz_setbit         mpz_setbit
 *   []           r_gmpz_getbit         mpz_tstbit
 *   scan0        r_gmpz_scan0          mpz_scan0
 *   scan1        r_gmpz_scan1          mpz_scan1
 *   even?        r_gmpz_is_even        mpz_even
 *   odd?         r_gmpz_is_odd         mpz_odd
 *   ...
 */

/*
 * The goal is to organize this file in the same order that the GMP Manual
 * is organized.
 */

/**********************************************************************
 *    Macros                                                          *
 **********************************************************************/

#define DEFUN_INT2INT(fname,mpz_fname)         \
static VALUE r_gmpz_##fname(VALUE self)        \
{                                              \
  MP_INT *self_val, *res_val;                  \
  VALUE res;                                   \
  mpz_get_struct(self, self_val);              \
  mpz_make_struct_init(res, res_val);          \
  mpz_fname(res_val, self_val);                \
  return res;                                  \
}                                              \
                                               \
static VALUE r_gmpz_##fname##_self(VALUE self) \
{                                              \
  MP_INT *self_val;                            \
  mpz_get_struct(self, self_val);              \
  mpz_fname(self_val, self_val);               \
  return Qnil;                                 \
}

#define DEFUN_INT_F_UL(fname,mpz_fname,argname)          \
static VALUE r_gmpz_##fname(VALUE self, VALUE exp)       \
{                                                        \
  MP_INT *self_val, *res_val;                            \
  VALUE res;                                             \
  unsigned long exp_val;                                 \
                                                         \
  if (FIXNUM_P(exp)) {                                   \
    if (FIX2INT(exp) < 0)                                \
      rb_raise(rb_eRangeError, argname " out of range"); \
    exp_val = FIX2INT(exp);                              \
  } else if (GMPZ_P(exp)) {                              \
    mpz_get_struct(exp, res_val);                        \
    if (!mpz_fits_ulong_p(res_val))                      \
      rb_raise(rb_eRangeError, argname " out of range"); \
    exp_val = mpz_get_ui(res_val);                       \
    if (exp_val == 0)                                    \
      rb_raise(rb_eRangeError, argname " out of range"); \
  } else {                                               \
    typeerror_as(ZX, argname);                           \
  }                                                      \
                                                         \
  mpz_make_struct_init(res, res_val);                    \
  mpz_get_struct(self, self_val);                        \
  mpz_fname(res_val, self_val, exp_val);                 \
                                                         \
  return res;                                            \
}

#define DEFUN_INT_CMP(name,CMP_OP)                             \
static VALUE r_gmpz_cmp_##name(VALUE self, VALUE arg)          \
{                                                              \
  MP_INT *self_val;                                            \
  mpz_get_struct(self,self_val);                               \
  return (mpz_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}

#define DEFUN_INT_DIV(fname,gmp_fname)                \
static VALUE r_gmpz_##fname(VALUE self, VALUE arg)    \
{                                                     \
  MP_INT *self_val, *arg_val, *res_val;               \
  VALUE res;                                          \
  int arg_val_i;                                      \
                                                      \
  mpz_get_struct(self, self_val);                     \
  mpz_make_struct_init(res, res_val);                 \
                                                      \
  if (GMPZ_P(arg)) {                                  \
    mpz_get_struct(arg,arg_val);                      \
    if (mpz_cmp_ui(arg_val, 0) == 0)                  \
      rb_raise (rb_eZeroDivError, "divided by 0");    \
    gmp_fname (res_val, self_val, arg_val);           \
  } else if (FIXNUM_P(arg)) {                         \
    arg_val_i = FIX2INT(arg);                         \
    if (arg_val_i > 0) {                              \
      gmp_fname##_ui (res_val, self_val, arg_val_i);  \
    } else if (arg_val_i == 0) {                      \
      rb_raise (rb_eZeroDivError, "divided by 0");    \
    } else {                                          \
      mpz_neg (res_val, self_val);                    \
      gmp_fname##_ui (res_val, self_val, -arg_val_i); \
    }                                                 \
  } else if (BIGNUM_P(arg)) {                         \
    mpz_set_bignum (res_val, arg);                    \
    if (mpz_cmp_ui(res_val, 0) == 0)                  \
      rb_raise (rb_eZeroDivError, "divided by 0");    \
    gmp_fname (res_val, self_val, res_val);           \
  } else {                                            \
    typeerror(ZXB);                                   \
  }                                                   \
  return res;                                         \
}

#define DEFUN_INT_LOGIC(fname, mpz_fname)          \
static VALUE r_gmpz_##fname(VALUE self, VALUE arg) \
{                                                  \
  MP_INT *self_val, *arg_val, *res_val;            \
  VALUE res;                                       \
                                                   \
  mpz_get_struct(self, self_val);                  \
                                                   \
  mpz_make_struct(res, res_val);                   \
  if (GMPZ_P(arg)) {                               \
    mpz_get_struct(arg,arg_val);                   \
    mpz_init(res_val);                             \
    mpz_fname(res_val, self_val, arg_val);         \
  } else if (FIXNUM_P(arg)) {                      \
    mpz_init_set_si(res_val, FIX2INT(arg));        \
    mpz_fname(res_val, self_val, res_val);         \
  } else if (BIGNUM_P(arg)) {                      \
    mpz_init(res_val);                             \
    mpz_set_bignum(res_val, arg);                  \
    mpz_fname(res_val, self_val, res_val);         \
  } else  {                                        \
    typeerror(ZXB);                                \
  }                                                \
  return res;                                      \
}

#define DEFUN_INT_SINGLETON_UI(fname,mpz_fname)           \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE arg)     \
{                                                         \
  MP_INT *arg_val_z, *res_val;                            \
  unsigned long arg_val_ul;                               \
  VALUE res;                                              \
                                                          \
  (void)klass;                                            \
                                                          \
  if (FIXNUM_P(arg)) {                                    \
    arg_val_ul = FIX2INT (arg);                           \
  } else if (GMPZ_P(arg)) {                               \
    mpz_get_struct(arg, arg_val_z);                       \
    if (!mpz_fits_ulong_p (arg_val_z))                    \
      rb_raise(rb_eRangeError, "argument out of range");  \
    arg_val_ul = mpz_get_ui(arg_val_z);                   \
    if (arg_val_ul == 0)                                  \
      rb_raise(rb_eRangeError, "argument out of range");  \
  } else {                                                \
    typeerror_as(ZX, "argument");                         \
  }                                                       \
  mpz_make_struct_init(res, res_val);                     \
  mpz_fname(res_val, arg_val_ul);                         \
  return res;                                             \
}

/**********************************************************************
 *    Initializing, Assigning Integers                                *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::Z.new(arg)
 *
 * Creates a new GMP::Z integer, with arg as its value, converting where
 * necessary.
 */
VALUE r_gmpzsg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_INT *res_val;
  VALUE res;

  (void)klass;

  if (argc > 1)
    rb_raise(rb_eArgError, "wrong # of arguments (%d for 0 or 1)", argc);

  mpz_make_struct(res, res_val);
  mpz_init(res_val);

  rb_obj_call_init(res, argc, argv);

  return res;
}

/*
 * call-seq:
 *   a = b
 *
 * Sets the value of +a+ to the value of +b+. +b+ may be one of the following
 * classes:
 *
 * * GMP::Z
 * * Fixnum
 * * String
 * * Bignum
 */
void mpz_set_value(MP_INT *target, VALUE source)
{
  MP_INT *source_val;

  if (GMPZ_P(source)) {
    mpz_get_struct(source, source_val);
    mpz_set(target, source_val);
  } else if (FIXNUM_P(source)) {
    mpz_set_si(target, NUM2INT(source));
  } else if (STRING_P(source)) {
    mpz_set_str(target, STR2CSTR(source), 0);
  } else if (BIGNUM_P(source)) {
    mpz_set_bignum(target, source);
  } else {
    rb_raise(rb_eTypeError, "Don't know how to convert %s into GMP_Z", rb_class2name(rb_class_of(source)));
  }
}

/*
 * call-seq:
 *   GMP::Z(arg)
 *
 * A convenience method for +GMP::Z.new(arg)+.
 */
VALUE r_gmpmod_z(int argc, VALUE *argv, VALUE module)
{
  (void)module;
  return r_gmpzsg_new(argc, argv, cGMP_Z);
}

/*
 * call-seq:
 *   int1.swap int2
 *
 * Efficiently swaps the contents of +int1+ with +int2+.
 */
VALUE r_gmpz_swap(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;
  if (!GMPZ_P(arg)) {
    rb_raise(rb_eTypeError, "Can't swap GMP::Z with object of other class");
  }
  mpz_get_struct(self, self_val);
  mpz_get_struct(arg, arg_val);
  mpz_swap(self_val,arg_val);
  return Qnil;
}


/**********************************************************************
 *    Converting Integers                                             *
 **********************************************************************/

/*
 * call-seq:
 *   integer.to_i
 *
 * Returns +integer+ as an Fixnum if +integer+ fits in a Fixnum.
 *
 * Otherwise returns the least significant part of +integer+, with the same
 * sign as +integer+.
 *
 * If +integer+ is too big to fit in a Fixnum, the returned result is probably
 * not very useful. To find out if the value will fit, use the function
 * mpz_fits_slong_p (<b>Unimplemented</b>).
 */
VALUE r_gmpz_to_i(VALUE self)
{
  MP_INT *self_val;
  char *str;
  VALUE res;

  mpz_get_struct(self, self_val);
  if (mpz_fits_slong_p(self_val))
    return rb_int2inum(mpz_get_si(self_val));
  str = mpz_get_str(NULL, 0, self_val);
  res = rb_cstr2inum(str, 10);
  free(str);
  return res;
}

/*
 * call-seq:
 *   integer.to_d
 *
 * Returns +integer+ as an Float if +integer+ fits in a Float.
 *
 * Otherwise returns the least significant part of +integer+, with the same
 * sign as +integer+.
 *
 * If +integer+ is too big to fit in a Float, the returned result is probably
 * not very useful. To find out if the value will fit, use the function
 * mpz_fits_slong_p (<b>Unimplemented</b>).
 */
VALUE r_gmpz_to_d(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);

  return rb_float_new(mpz_get_d(self_val));
}


/**********************************************************************
 *    Integer Arithmetic                                              *
 **********************************************************************/

/*
 * call-seq:
 *   +(other)
 *
 * Adds this GMP::Z to other. Other can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_add(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    mpz_make_struct_init(res, res_val);
    mpz_add(res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    if (FIX2INT(arg) > 0)
      mpz_add_ui(res_val, self_val, FIX2INT(arg));
    else
      mpz_sub_ui(res_val, self_val, -FIX2INT(arg));
  } else if (GMPQ_P(arg)) {
    return r_gmpq_add(arg, self);
  } else if (GMPF_P(arg)) {
    return r_gmpf_add(arg, self);
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_init(res_val);
    mpz_set_bignum (res_val, arg);
    mpz_add(res_val, res_val, self_val);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   add!(other)
 *
 * Adds this GMP::Z to other, and sets this GMP::Z's value to the result. Other
 * can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_add_self(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    mpz_add(self_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) > 0)
      mpz_add_ui(self_val, self_val, FIX2INT(arg));
    else
      mpz_sub_ui(self_val, self_val, -FIX2INT(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    mpz_add(self_val, self_val, arg_val);
    mpz_temp_free(arg_val);
  } else {
    typeerror(ZXB);
  }
  return Qnil;
}

/*
 * call-seq:
 *   -(other)
 *
 * Subtracts other from this GMP::Z. Other can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_sub(VALUE self, VALUE arg)
{
  MP_RAT *res_val_q, *arg_val_q;
  MP_INT *self_val, *arg_val, *res_val;
  MP_FLOAT *arg_val_f, *res_val_f;
  VALUE res;
  unsigned long prec;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_get_struct(arg,arg_val);
    mpz_sub (res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    if (FIX2INT(arg) > 0)
      mpz_sub_ui (res_val, self_val, FIX2INT(arg));
    else
      mpz_add_ui (res_val, self_val, -FIX2INT(arg));
  } else if (GMPQ_P(arg)) {
    mpq_make_struct_init(res, res_val_q);
    mpq_get_struct(arg,arg_val_q);
    mpz_set (mpq_denref(res_val_q), mpq_denref(arg_val_q));
    mpz_mul (mpq_numref(res_val_q), mpq_denref(arg_val_q), self_val);
    mpz_sub (mpq_numref(res_val_q), mpq_numref(res_val_q), mpq_numref(arg_val_q));
  } else if (GMPF_P(arg)) {
    mpf_get_struct_prec (arg, arg_val_f, prec);
    mpf_make_struct_init(res, res_val_f, prec);
    mpf_set_z (res_val_f, self_val);
    mpf_sub (res_val_f, res_val_f, arg_val_f);
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_set_bignum (res_val, arg);
    mpz_sub (res_val, self_val, res_val);
  } else {
    typeerror (ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   sub!(other)
 *
 * Subtracts other from this GMP::Z, and sets this GMP::Z's value to the
 * result. Other can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_sub_self(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val);
    mpz_sub (self_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) > 0)
      mpz_sub_ui (self_val, self_val, FIX2INT(arg));
    else
      mpz_add_ui (self_val, self_val, -FIX2INT(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    mpz_sub (self_val, self_val, arg_val);
    mpz_temp_free (arg_val);
  } else {
    typeerror (ZXB);
  }
  return Qnil;
}

/*
 * call-seq:
 *   *(other)
 *
 * Multiplies this GMP::Z with other. Other can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_mul(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_get_struct(arg,arg_val);
    mpz_mul(res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_mul_si(res_val, self_val, FIX2INT(arg));
  } else if (GMPQ_P(arg)) {
    return r_gmpq_mul(arg, self);
  } else if (GMPF_P(arg)) {
    return r_gmpf_mul(arg, self);
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_set_bignum(res_val, arg);
    mpz_mul(res_val, res_val, self_val);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * Document-method: neg
 *
 * call-seq:
 *   integer.neg
 *   -integer
 *
 * From the GMP Manual:
 * 
 * Returns -+integer+.
 */
/*
 * Document-method: neg!
 *
 * call-seq:
 *   integer.neg!
 *
 * From the GMP Manual:
 * 
 * Sets +integer+ to -+integer+.
 */
DEFUN_INT2INT(neg, mpz_neg)
/*
 * Document-method: abs
 *
 * call-seq:
 *   integer.abs
 *
 * From the GMP Manual:
 * 
 * Returns the absolute value of +integer+.
 */
/*
 * Document-method: abs!
 *
 * call-seq:
 *   integer.abs!
 *
 * From the GMP Manual:
 * 
 * Sets +integer+ to its absolute value.
 */
DEFUN_INT2INT(abs, mpz_abs)


/**********************************************************************
 *    Integer Division                                                *
 **********************************************************************/

/**********************************************************************
 *    Integer Exponentiation                                          *
 **********************************************************************/

/**********************************************************************
 *    Integer Roots                                                   *
 **********************************************************************/

DEFUN_INT_F_UL(root,mpz_root,"root number")

/*
 * Document-method: sqrt
 *
 * call-seq:
 *   integer.sqrt
 *
 * From the GMP Manual:
 * 
 * Returns the truncated integer part of the square root of +integer+.
 */
/*
 * Document-method: sqrt!
 *
 * call-seq:
 *   integer.sqrt!
 *
 * From the GMP Manual:
 * 
 * Sets +integer+ to the truncated integer part of its square root.
 */
DEFUN_INT2INT(sqrt, mpz_sqrt)

/*
 * Document-method: sqrtrem
 *
 * call-seq:
 *   integer.sqrtrem #=> sqrt, rem
 *
 * From the GMP Manual:
 * 
 * Returns the truncated integer part of the square root of +integer+ as
 * +sqrt+ and the remainder <tt>integer - sqrt * sqrt</tt> as +rem+, which will
 * be zero if +integer+ is a perfect square.
 */
static VALUE r_gmpz_sqrtrem(VALUE self)
{
  MP_INT *self_val, *sqrt_val, *rem_val;
  VALUE sqrt, rem;

  mpz_get_struct(self, self_val);
  mpz_make_struct_init(sqrt, sqrt_val);
  mpz_make_struct_init(rem, rem_val);
  mpz_sqrtrem(sqrt_val, rem_val, self_val);
  return rb_assoc_new(sqrt, rem);
}


/**********************************************************************
 *    Number Theoretic Functions                                      *
 **********************************************************************/

/*
 * Document-method: remove
 *
 * call-seq:
 *   integer.remove(factor)
 *
 * From the GMP Manual:
 * 
 * Remove all occurrences of the factor +factor+ from +integer+. The return
 * value is how many such occurrences were removed.
 */
VALUE r_gmpz_remove(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;
  int free_arg_val = 0;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    if (mpz_sgn(arg_val) != 1)
      rb_raise(rb_eRangeError, "argument must be positive");
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) <= 0)
      rb_raise(rb_eRangeError, "argument must be positive");
    mpz_temp_alloc(arg_val);
    mpz_init_set_ui(arg_val, FIX2INT(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    if (mpz_sgn(arg_val) != 1) {
      mpz_temp_free (arg_val);
      rb_raise(rb_eRangeError, "argument must be positive");
    }
  } else {
    typeerror(ZXB);
  }

  mpz_make_struct_init (res, res_val);
  mpz_remove (res_val, self_val, arg_val);

  if (free_arg_val)
    mpz_temp_free(arg_val);

  return res;
}

/*
 * Document-method: GMP::Z.fac
 *
 * call-seq:
 *   GMP::Z.fac(n)
 *
 * From the GMP Manual:
 * 
 * Returns <tt>n!</tt>, the factorial of +n+.
 *
 * Examples:
 *
 * GMP::Z.fac(0)  #=>  1
 * GMP::Z.fac(1)  #=>  1
 * GMP::Z.fac(2)  #=>  2
 * GMP::Z.fac(3)  #=>  6
 * GMP::Z.fac(4)  #=> 24
 */
DEFUN_INT_SINGLETON_UI(fac,mpz_fac_ui)
/*
 * Document-method: GMP::Z.fib
 *
 * call-seq:
 *   GMP::Z.fib(n)
 *
 * From the GMP Manual:
 * 
 * Returns <tt>F[n]</tt>, the +n+th Fibonacci number.
 *
 * Examples:
 *
 * GMP::Z.fib(1)  #=>  1
 * GMP::Z.fib(2)  #=>  1
 * GMP::Z.fib(3)  #=>  2
 * GMP::Z.fac(4)  #=>  3
 * GMP::Z.fac(5)  #=>  5
 * GMP::Z.fac(6)  #=>  8
 * GMP::Z.fac(7)  #=> 13
 */
DEFUN_INT_SINGLETON_UI(fib,mpz_fib_ui)


/**********************************************************************
 *    Integer Comparisons                                             *
 **********************************************************************/

VALUE r_gmpz_eq(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val_z;
  MP_RAT *arg_val_q;

  mpz_get_struct (self, self_val);
  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    return (mpz_cmp (self_val, arg_val_z)==0) ? Qtrue : Qfalse;
  } else if (FIXNUM_P(arg)) {
    return (mpz_cmp_si (self_val, FIX2INT(arg))==0) ? Qtrue : Qfalse;
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    if (mpz_cmp_ui(mpq_denref(arg_val_q), 1)==0)
      return Qfalse;
    return (mpz_cmp (self_val, mpq_numref(arg_val_q))==0) ? Qtrue : Qfalse;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    if (mpz_cmp (self_val, arg_val_z)==0) {
      mpz_temp_free(arg_val_z);
      return Qtrue;
    } else {
      mpz_temp_free(arg_val_z);
      return Qfalse;
    }
  } else {
    return Qfalse;
  }
}

VALUE r_gmpz_cmpabs(VALUE self, VALUE arg)
{
  MP_INT *arg_val_z, *self_val;
  MP_RAT *arg_val_q;
  int res;

  mpz_get_struct(self, self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val_z);
    return INT2FIX(mpz_cmpabs(self_val, arg_val_z));
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) >= 0)
      return INT2FIX(mpz_cmpabs_ui(self_val, FIX2INT(arg)));
    else
      return INT2FIX(mpz_cmpabs_ui(self_val, -FIX2INT(arg)));
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpz_temp_alloc(arg_val_z);
    mpz_init(arg_val_z);
    mpz_mul(arg_val_z, self_val, mpq_denref(arg_val_q));
    res = mpz_cmpabs(arg_val_z, mpq_numref(arg_val_q));
    mpz_temp_free(arg_val_z);
    return INT2FIX(res);
  } else if (GMPF_P(arg)) {
    not_yet;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    res = mpz_cmpabs(self_val, arg_val_z);
    mpz_temp_free(arg_val_z);
    return INT2FIX(res);
  } else {
    typeerror(ZQFXB);
  }
}


/**********************************************************************
 *    Integer Logic and Bit Fiddling                                  *
 **********************************************************************/

/*
 * Document-method: &
 *
 * call-seq:
 *   integer & other
 *
 * From the GMP Manual:
 * 
 * Returns +integer+ bitwise-and +other+.
 */
DEFUN_INT_LOGIC(and, mpz_and)
/*
 * Document-method: |
 *
 * call-seq:
 *   integer | other
 *
 * From the GMP Manual:
 * 
 * Returns +integer+ bitwise inclusive-or +other+.
 */
DEFUN_INT_LOGIC(or, mpz_ior)
/*
 * Document-method: ^
 *
 * call-seq:
 *   integer ^ other
 *
 * From the GMP Manual:
 * 
 * Returns +integer+ bitwise exclusive-or +other+.
 */
DEFUN_INT_LOGIC(xor, mpz_xor)

/*
 * call-seq:
 *   integer.scan0(starting_bit)
 *
 * From the GMP Manual:
 * 
 * Scan integer, starting from bit starting_bit, towards more significant bits,
 * until the first 0 bit is found. Return the index of the found bit.
 *
 * If the bit at starting_bit is already what's sought, then starting_bit is
 * returned.
 * 
 * If there's no bit found, then INT2FIX(ULONG_MAX) is returned. This will
 * happen in scan0 past the end of a negative number.
 */
VALUE r_gmpz_scan0(VALUE self, VALUE bitnr)
{
  MP_INT *self_val;
  int bitnr_val;
  mpz_get_struct(self, self_val);
  if (FIXNUM_P(bitnr)) {
    bitnr_val = FIX2INT (bitnr);
  } else {
    typeerror_as(X, "index");
  }
  return INT2FIX(mpz_scan0(self_val, bitnr_val));
}

/*
 * call-seq:
 *   integer.scan1(starting_bit)
 *
 * From the GMP Manual:
 * 
 * Scan integer, starting from bit starting_bit, towards more significant bits,
 * until the first 1 bit is found. Return the index of the found bit.
 *
 * If the bit at starting_bit is already what's sought, then starting_bit is
 * returned.
 *
 * If there's no bit found, then INT2FIX(ULONG_MAX) is returned. This will
 * happen in mpz_scan1 past the end of a nonnegative number.
 */
VALUE r_gmpz_scan1(VALUE self, VALUE bitnr)
{
  MP_INT *self_val;
  int bitnr_val;

  mpz_get_struct(self, self_val);

  if (FIXNUM_P(bitnr)) {
    bitnr_val = FIX2INT (bitnr);
  } else {
    typeerror_as(X, "index");
  }

  return INT2FIX(mpz_scan1(self_val, bitnr_val));
}


/**********************************************************************
 *    Integer Random Numbers                                          *
 **********************************************************************/

/**********************************************************************
 *    Miscellaneous Integer Functions                                 *
 **********************************************************************/

/**********************************************************************
 *    Integer Special Functions                                       *
 **********************************************************************/


/**********************************************************************
 *    _unsorted_                                                      *
 **********************************************************************/

/*
 * call-seq:
 *   /(other)
 *
 * Divides this GMP::Z by other. Other can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_div(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val_z, *tmp_z;
  MP_RAT *arg_val_q, *res_val_q;
  MP_FLOAT *arg_val_f, *res_val_f;
  VALUE res;
  unsigned int prec;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    if (mpz_cmp_ui(arg_val_z, 0) == 0)
      rb_raise(rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init(res, res_val_q);
    mpq_set_num(res_val_q, self_val);
    mpq_set_den(res_val_q, arg_val_z);
    mpq_canonicalize (res_val_q);
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) == 0)
      rb_raise(rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init(res, res_val_q);
    mpq_set_num(res_val_q, self_val);
    mpz_set_ui(mpq_denref(res_val_q), FIX2INT(arg));
    mpq_canonicalize (res_val_q);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    if (mpz_cmp_ui(mpq_numref(arg_val_q), 0) == 0)
      rb_raise(rb_eZeroDivError, "divided by 0");
    mpz_temp_init(tmp_z);
    mpq_make_struct_init(res, res_val_q);
    mpz_gcd(tmp_z, mpq_numref(arg_val_q), self_val);
    mpz_divexact(mpq_numref(res_val_q), self_val, tmp_z);
    mpz_divexact(mpq_denref(res_val_q), mpq_numref(arg_val_q), tmp_z);
    mpz_mul(mpq_numref(res_val_q), mpq_numref(res_val_q), mpq_denref(arg_val_q));
    mpz_temp_free(tmp_z);
  } else if (GMPF_P(arg)) {
    mpf_get_struct_prec(arg, arg_val_f, prec);
    mpf_make_struct_init(res, res_val_f, prec);
    mpf_set_z(res_val_f, self_val);
    mpf_div(res_val_f, res_val_f, arg_val_f);
  } else if (BIGNUM_P(arg)) {
    mpq_make_struct_init(res, res_val_q);
    mpz_set_bignum(mpq_denref(res_val_q), arg);
    if (mpz_cmp_ui(mpq_denref(res_val_q), 0) == 0)
      rb_raise(rb_eZeroDivError, "divided by 0");
    mpq_set_num(res_val_q, self_val);
    mpq_canonicalize(res_val_q);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * Document-method: tdiv
 *
 * call-seq:
 *   n.tdiv d
 *
 * From the GMP Manual:
 * 
 * Divides +n+ by +d+, forming a quotient +q+. tdiv rounds +q+ towards zero.
 * The +t+ stands for "truncate".
 *
 * +q+ will satisfy <tt>n=q*d+r</tt>, and +r+ will satisfy
 * <tt>0<=abs(r)<abs(d)</tt>.
 *
 * This function calculates only the quotient.
 */
DEFUN_INT_DIV(tdiv, mpz_tdiv_q)
/*
 * Document-method: tmod
 *
 * call-seq:
 *   n.tmod d
 *
 * From the GMP Manual:
 * 
 * Divides +n+ by +d+, forming a remainder +r+. +r+ will have the same sign as
 * +n+. The +t+ stands for “truncate”. 
 *
 * +r+ will satisfy <tt>n=q*d+r</tt>, and +r+ will satisfy
 * <tt>0<=abs(r)<abs(d)</tt>.
 *
 * This function calculates only the remainder.
 *
 * The remainder can be negative, so the return value is the absolute value of
 * the remainder.
 */
DEFUN_INT_DIV(tmod, mpz_tdiv_r)
/*
 * Document-method: fdiv
 *
 * call-seq:
 *   n.fdiv d
 *
 * From the GMP Manual:
 * 
 * Divide n by d, forming a quotient q. fdiv rounds q down towards -infinity.
 * The f stands for “floor”.
 *
 * q will satisfy n=q*d+r.
 *
 * This function calculates only the quotient.
 */
DEFUN_INT_DIV(fdiv, mpz_fdiv_q)
/*
 * Document-method: fmod
 *
 * call-seq:
 *   n.fmod d
 *
 * From the GMP Manual:
 * 
 * Divides n by d, forming a remainder r. r will have the same sign as d. The f
 * stands for “floor”. 
 *
 * r will satisfy n=q*d+r, and r will satisfy 0<=abs(r)<abs(d).
 *
 * This function calculates only the remainder.
 *
 * The remainder can be negative, so the return value is the absolute value of
 * the remainder.
 */
DEFUN_INT_DIV(fmod, mpz_fdiv_r)
DEFUN_INT_DIV(cdiv, mpz_cdiv_q)
DEFUN_INT_DIV(cmod, mpz_cdiv_r)

/*
 * Document-method: com
 *
 * call-seq:
 *   integer.com
 *
 * From the GMP Manual:
 * 
 * Returns the one's complement of +integer+.
 */
/*
 * Document-method: com!
 *
 * call-seq:
 *   integer.com!
 *
 * From the GMP Manual:
 * 
 * Sets +integer+ to its one's complement.
 */
DEFUN_INT2INT(com, mpz_com)
/*
 * Document-method: nextprime
 *
 * call-seq:
 *   integer.nextprime
 *
 * From the GMP Manual:
 * 
 * Returns the next prime greater than +integer+.
 *
 * This function uses a probabilistic algorithm to identify primes. For
 * practical purposes it's adequate, the chance of a composite passing will be
 * extremely small.
 */
/*
 * Document-method: nextprime!
 *
 * call-seq:
 *   integer.nextprime!
 *
 * From the GMP Manual:
 * 
 * Sets +integer+ to the next prime greater than +integer+.
 *
 * This function uses a probabilistic algorithm to identify primes. For
 * practical purposes it's adequate, the chance of a composite passing will be
 * extremely small.
 */
DEFUN_INT2INT(nextprime, mpz_nextprime)

/*
 * call-seq: probab_prime?([reps])
 *
 * Determine whether +n+ is prime. Returns 2 if +n+ is definitely prime,
 * returns 1 if +n+ is probably prime (without being certain), or return 0 if
 * +n+ is definitely composite. 
 *
 * This function does some trial divisions, then some Miller-Rabin
 * probabilistic primality tests. +reps+ controls how many such tests are done,
 * 5 to 10 is a reasonable number, more will reduce the chances of a composite
 * being returned as “probably prime”.
 *
 * Miller-Rabin and similar tests can be more properly called compositeness
 * tests. Numbers which fail are known to be composite but those which pass
 * might be prime or might be composite. Only a few composites pass, hence
 * those which pass are considered probably prime. 
 */
VALUE r_gmpz_is_probab_prime(int argc, VALUE* argv, VALUE self)
{
  MP_INT *self_val;
  int reps_val;
  VALUE reps;
  mpz_get_struct(self, self_val);
  rb_scan_args(argc, argv, "01", &reps);
  if(NIL_P(reps)){
    reps = INT2FIX(5);
  }
  if (FIXNUM_P(reps)) {
    reps_val = FIX2INT (reps);
  } else {
    typeerror_as(X, "reps");
  }
  return INT2FIX(mpz_probab_prime_p(self_val, reps_val));
}

/*
 * Document-method: popcount
 *
 * call-seq:
 *   integer.popcount
 *
 * From the GMP Manual:
 * 
 * If <tt>integer>=0</tt>, return the population count of <tt>integer</tt>,
 * which is the number of 1 bits in the binary representation. If
 * <tt>op<0</tt>, the number of 1s is infinite, and the return value is
 * INT2FIX(ULONG_MAX), the largest possible unsigned long.
 */
VALUE r_gmpz_popcount(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);
  return INT2FIX(mpz_popcount(self_val));
}

/*
 * Document-method: jacobi
 *
 * call-seq:
 *   a.jacobi
 *
 * <b>90% sure this is incorrectly implemented. Todo.</b>
 *
 * From the GMP Manual:
 * 
 * Calculate the Jacobi symbol <tt>(a/b)</tt>. This is defined only for +b+
 * odd.
 */
VALUE r_gmpz_jacobi(VALUE self)
{
  MP_INT *self_val, *res_val;
  VALUE res;
  mpz_get_struct(self, self_val);
  if (mpz_sgn(self_val) != 1)
    rb_raise(rb_eRangeError, "you can take jacobi symbol only of positive value");
  if (mpz_even_p(self_val))
    rb_raise(rb_eRangeError, "you can't take jacobi symbol of even value");
  mpz_make_struct_init(res, res_val);
  mpz_jacobi(res_val, self_val);
  return res;
}

/*
 * Document-method: legendre
 *
 * call-seq:
 *   a.legendre
 *
 * <b>90% sure this is incorrectly implemented. Todo.</b>
 *
 * From the GMP Manual:
 * 
 * Calculate the Legendre symbol <tt>(a/p)</tt>. This is defined only for +p+
 * an odd positive prime, and for such +p+ it's identical to the Jacobi symbol. 
 */
VALUE r_gmpz_legendre(VALUE self)
{
  MP_INT *self_val, *res_val;
  VALUE res;
  mpz_get_struct(self, self_val);
  if (mpz_sgn(self_val) != 1)
    rb_raise(rb_eRangeError, "you can take legendre symbol only of positive value");
  if (mpz_even_p(self_val))
    rb_raise(rb_eRangeError, "you can't take legendre symbol of even value");
    mpz_make_struct_init(res, res_val);
  mpz_legendre(res_val, self_val);
  return res;
}

/*
 * call-seq:
 *   integer[index] = x &rarr; nil
 *
 * Sets the bit at index to x.
 */
VALUE r_gmpz_setbit(VALUE self, VALUE bitnr, VALUE set_to)
{
  MP_INT *self_val;
  int bitnr_val;

  mpz_get_struct(self, self_val);

  if (FIXNUM_P(bitnr)) {
    bitnr_val = FIX2INT (bitnr);
  } else {
    typeerror_as(X, "index");
  }
  if (RTEST(set_to)) {
    mpz_setbit(self_val, bitnr_val);
  } else {
    mpz_clrbit(self_val, bitnr_val);
  }
  return Qnil;
}

/*
 * call-seq:
 *   integer[index] &rarr; boolean
 *
 * Gets the bit at index, returned as either true or false.
 */
VALUE r_gmpz_getbit(VALUE self, VALUE bitnr)
{
  MP_INT *self_val;
  int bitnr_val;
  mpz_get_struct(self, self_val);
  if (FIXNUM_P(bitnr)) {
    bitnr_val = FIX2INT (bitnr);
  } else {
    typeerror_as(X, "index");
  }
  return mpz_tstbit(self_val, bitnr_val)?Qtrue:Qfalse;
}

/*
 * Document-method: even?
 *
 * call-seq:
 *   integer.even?
 *
 * From the GMP Manual:
 * 
 * Determines whether integer is even. Returns true or false.
 */
DEFUN_INT_COND_P(is_even,mpz_even_p)
/*
 * Document-method: odd?
 *
 * call-seq:
 *   integer.odd?
 *
 * From the GMP Manual:
 * 
 * Determines whether integer is odd. Returns true or false.
 */
DEFUN_INT_COND_P(is_odd,mpz_odd_p)
/*
 * Document-method: square?
 *
 * call-seq:
 *   integer.square?
 *
 * From the GMP Manual:
 * 
 * Returns true if integer is a perfect square, i.e., if the square root of
 * integer is an integer. Under this definition both 0 and 1 are considered to
 * be perfect squares.
 */
DEFUN_INT_COND_P(is_square,mpz_perfect_square_p)
/*
 * Document-method: power?
 *
 * call-seq:
 *   integer.square?
 *
 * From the GMP Manual:
 * 
 * Returns true if integer is a perfect power, i.e., if there exist integers a
 * and b, with b>1, such that integer equals a raised to the power b.
 *
 * Under this definition both 0 and 1 are considered to be perfect powers.
 * Negative values of integers are accepted, but of course can only be odd
 * perfect powers.
 */
DEFUN_INT_COND_P(is_power,mpz_perfect_power_p)

/*
 * call-seq:
 *   integer.sgn
 *
 * From the GMP Manual:
 * 
 * Returns +1 if +integer+ > 0, 0 if +integer+ == 0, and -1 if +integer+ < 0.
 */
VALUE r_gmpz_sgn(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);
  return INT2FIX(mpz_sgn(self_val));
}

/*
 * call-seq:
 *   integer.powmod(exp, mod)
 *
 * From the GMP Manual:
 * 
 * Returns +integer+ raised to +exp+ modulo +mod+.
 *
 * Negative +exp+ is supported if an inverse <tt>integer^-1</tt> mod
 * <tt>mod</tt> exists. If an inverse doesn't exist then a divide by zero is
 * raised.
 */
VALUE r_gmpz_powm(VALUE self, VALUE exp, VALUE mod)
{
  MP_INT *self_val, *res_val, *mod_val, *exp_val;
  VALUE res;
  int free_mod_val = 0;

  if (GMPZ_P(mod)) {
    mpz_get_struct(mod, mod_val);
    if (mpz_sgn(mod_val) <= 0) {
      rb_raise (rb_eRangeError, "modulus must be positive");
    }
  } else if (FIXNUM_P(mod)) {
  if (FIX2INT(mod) <= 0) {
    rb_raise (rb_eRangeError, "modulus must be positive");
  }
  mpz_temp_alloc (mod_val);
  mpz_init_set_ui(mod_val, FIX2INT(mod));
  free_mod_val = 1;
  } else if (BIGNUM_P(mod)) {
    mpz_temp_from_bignum (mod_val, mod);
    if (mpz_sgn(mod_val) <= 0) {
      mpz_temp_free(mod_val);
      rb_raise (rb_eRangeError, "modulus must be positive");
    }
    free_mod_val = 1;
  } else {
    typeerror_as (ZXB, "modulus");
  }
  mpz_make_struct_init(res, res_val);
  mpz_get_struct(self, self_val);

  if (GMPZ_P(exp)) {
    mpz_get_struct(exp, exp_val);
    if (mpz_sgn(mod_val) < 0) {
      rb_raise (rb_eRangeError, "exponent must be nonnegative");
    }
    mpz_powm (res_val, self_val, exp_val, mod_val);
  } else if (FIXNUM_P(exp)) {
    if (FIX2INT(exp) < 0)
    {
      if (free_mod_val)
        mpz_temp_free(mod_val);
      rb_raise (rb_eRangeError, "exponent must be nonnegative");
    }
    mpz_powm_ui (res_val, self_val, FIX2INT(exp), mod_val);
  } else if (BIGNUM_P(exp)) {
    mpz_temp_from_bignum (exp_val, exp);
    mpz_powm (res_val, self_val, exp_val, mod_val);
    mpz_temp_free (exp_val);
  } else {
    if (free_mod_val)
      mpz_temp_free(mod_val);
    typeerror_as (ZXB, "exponent");
  }
  if (free_mod_val)
    mpz_temp_free(mod_val);
  return res;
}

/*
 * Document-method: **
 *
 * call-seq:
 *   integer ** exp
 *
 * From the GMP Manual:
 * 
 * Returns +integer+ raised to +exp+. The case 0^0 yields 1.
 */
DEFUN_INT_F_UL(pow,mpz_pow_ui,"exponent")
/*
 * Document-method: <<
 *
 * call-seq:
 *   integer << n
 *
 * From the GMP Manual:
 * 
 * Returns +integer+ times 2 raised to +n+. This operation can also be defined
 * as a left shift by +n+ bits.
 */
DEFUN_INT_F_UL(shl,mpz_mul_2exp,"shift size")
DEFUN_INT_F_UL(fshr,mpz_fdiv_q_2exp,"shift size")
DEFUN_INT_F_UL(tshr,mpz_tdiv_q_2exp,"shift size")
DEFUN_INT_F_UL(fshrm,mpz_fdiv_r_2exp,"mark size")
DEFUN_INT_F_UL(tshrm,mpz_tdiv_r_2exp,"mark size")

int mpz_cmp_value(MP_INT *OP, VALUE arg)
{
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  int res;

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val_z);
    return mpz_cmp(OP,arg_val_z);
  } else if (FIXNUM_P(arg)) {
    return mpz_cmp_si(OP, FIX2INT(arg));
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpz_temp_alloc(arg_val_z);
    mpz_init(arg_val_z);
    mpz_mul(arg_val_z, OP, mpq_denref(arg_val_q));
    res = mpz_cmp(arg_val_z, mpq_numref(arg_val_q));
    mpz_temp_free(arg_val_z);
    return res;
  } else if (GMPF_P(arg)) {
    not_yet;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    res = mpz_cmp(OP, arg_val_z);
    mpz_temp_free(arg_val_z);
    return res;
  } else {
    typeerror_as(ZQFXB, "exponent");
  }
}

VALUE r_gmpz_cmp(VALUE self, VALUE arg)
{
  MP_INT *self_val;
  int res;
  mpz_get_struct(self,self_val);
  res = mpz_cmp_value(self_val, arg);
  if (res > 0)
    return INT2FIX(1);
  else if (res == 0)
    return INT2FIX(0);
  else
    return INT2FIX(-1);
}

/*
 * Document-method: <
 *
 * call-seq:
 *   int1 < int2
 * 
 * Returns whether +int1+ is strictly less than +int2+.
 */
DEFUN_INT_CMP(lt,<)
/*
 * Document-method: <=
 *
 * call-seq:
 *   int1 <= int2
 * 
 * Returns whether +int1+ is less than or equal to +int2+.
 */
DEFUN_INT_CMP(le,<=)
/*
 * Document-method: >
 *
 * call-seq:
 *   int1 > int2
 * 
 * Returns whether +int1+ is strictly greater than +int2+.
 */
DEFUN_INT_CMP(gt,>)
/*
 * Document-method: >=
 *
 * call-seq:
 *   int1 >= int2
 *
 * Returns whether +int1+ is greater than or equal to +int2+.
 */
DEFUN_INT_CMP(ge,>=)

VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp)
{
  MP_INT *res_val;
  VALUE res;

  if (FIXNUM_P(base) && FIXNUM_P(exp))
  {
    if (FIX2INT(base) < 0)
      rb_raise (rb_eRangeError, "base must not be negative");
    if (FIX2INT(exp) < 0) 
      rb_raise (rb_eRangeError, "exponent must not be negative");
    mpz_make_struct_init (res, res_val);
    mpz_ui_pow_ui (res_val, base, exp);
    return res;
  }
  return r_gmpz_pow (r_gmpzsg_new(1, &base, klass), exp);
}

/*
 * Document-method: to_s
 *
 * call-seq:
 *   integer.to_s
 *
 * Returns the decimal representation of +integer+, as a string.
 */
VALUE r_gmpz_to_s(VALUE self)
{
  MP_INT *self_val;
  char *str;
  VALUE res;

  Data_Get_Struct(self, MP_INT, self_val);
  str = mpz_get_str(NULL, 10, self_val);
  res = rb_str_new2(str);
  free (str);

  return res;
}

void init_gmpz()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "Z", r_gmpmod_z, -1);

  cGMP_Z = rb_define_class_under(mGMP, "Z", rb_cInteger);

  // Initializing, Assigning Integers
  rb_define_singleton_method(cGMP_Z, "new", r_gmpzsg_new, -1);
  rb_define_method(cGMP_Z, "swap",  r_gmpz_swap, 1);

  // Converting Integers
  rb_define_method(cGMP_Z, "to_i",  r_gmpz_to_i, 0);
  rb_define_method(cGMP_Z, "to_d",  r_gmpz_to_d, 0);
  
  // Integer Arithmetic
  rb_define_method(cGMP_Z, "+", r_gmpz_add, 1);
  rb_define_method(cGMP_Z, "add!", r_gmpz_add_self, 1);
  rb_define_method(cGMP_Z, "-", r_gmpz_sub, 1);  
  rb_define_method(cGMP_Z, "sub!", r_gmpz_sub_self, 1);
  rb_define_method(cGMP_Z, "*", r_gmpz_mul, 1);
  rb_define_method(cGMP_Z, "neg", r_gmpz_neg, 0);
  rb_define_method(cGMP_Z, "neg!", r_gmpz_neg_self, 0);
  
  // Integer Division
  rb_define_method(cGMP_Z, "/", r_gmpz_div, 1);
  rb_define_method(cGMP_Z, "tdiv", r_gmpz_tdiv, 1);
  rb_define_method(cGMP_Z, "tmod", r_gmpz_tmod, 1);
  rb_define_method(cGMP_Z, "fdiv", r_gmpz_fdiv, 1);
  rb_define_method(cGMP_Z, "fmod", r_gmpz_fmod, 1);
  rb_define_method(cGMP_Z, "cdiv", r_gmpz_cdiv, 1);
  rb_define_method(cGMP_Z, "cmod", r_gmpz_cmod, 1);

  // Integer Roots
  rb_define_method(cGMP_Z, "root",  r_gmpz_root, 1);
  
  // Number Theoretic Functions
  rb_define_method(cGMP_Z, "remove",  r_gmpz_remove, 1);
  rb_define_singleton_method(cGMP_Z, "fac", r_gmpzsg_fac, 1);
  rb_define_singleton_method(cGMP_Z, "fib", r_gmpzsg_fib, 1);
  
  // Integer Comparisons
  rb_define_method(cGMP_Z, "<=>", r_gmpz_cmp, 1);
  rb_define_method(cGMP_Z, ">",   r_gmpz_cmp_gt, 1);
  rb_define_method(cGMP_Z, ">=",  r_gmpz_cmp_ge, 1);
  rb_define_method(cGMP_Z, "<",   r_gmpz_cmp_lt, 1);
  rb_define_method(cGMP_Z, "<=",  r_gmpz_cmp_le, 1);
  rb_define_method(cGMP_Z, "cmpabs",  r_gmpz_cmpabs, 1);
  
  // _unsorted_
  rb_define_method(cGMP_Z, "-@", r_gmpz_neg, 0);
  rb_define_method(cGMP_Z, "abs", r_gmpz_abs, 0);
  rb_define_method(cGMP_Z, "abs!", r_gmpz_abs_self, 0);
  rb_define_method(cGMP_Z, "com", r_gmpz_com, 0);
  rb_define_method(cGMP_Z, "com!", r_gmpz_com_self, 0);
  rb_define_method(cGMP_Z, "&", r_gmpz_and, 1);
  rb_define_method(cGMP_Z, "|", r_gmpz_or, 1);
  rb_define_method(cGMP_Z, "^", r_gmpz_xor, 1);
  rb_define_method(cGMP_Z, "[]=", r_gmpz_setbit, 2);
  rb_define_method(cGMP_Z, "[]", r_gmpz_getbit, 1);
  rb_define_method(cGMP_Z, "scan0", r_gmpz_scan0, 1);
  rb_define_method(cGMP_Z, "scan1", r_gmpz_scan1, 1);
  rb_define_method(cGMP_Z, "even?", r_gmpz_is_even, 0);
  rb_define_method(cGMP_Z, "odd?", r_gmpz_is_odd, 0);
  rb_define_method(cGMP_Z, "sgn", r_gmpz_sgn, 0);
  
  rb_define_method(cGMP_Z, "**", r_gmpz_pow, 1);
  rb_define_method(cGMP_Z, "powmod", r_gmpz_powm, 2);
  
  rb_define_method(cGMP_Z, "==",  r_gmpz_eq, 1);
  rb_define_method(cGMP_Z, ">>",  r_gmpz_fshr, 1);
  rb_define_method(cGMP_Z, "<<",  r_gmpz_shl, 1);
  rb_define_method(cGMP_Z, "tshr",  r_gmpz_tshr, 1);
  rb_define_method(cGMP_Z, "lastbits_sgn",  r_gmpz_tshrm, 1);
  rb_define_method(cGMP_Z, "lastbits_pos",  r_gmpz_fshrm, 1);
  rb_define_method(cGMP_Z, "square?",  r_gmpz_is_square, 0);
  rb_define_method(cGMP_Z, "power?",  r_gmpz_is_power, 0);
  rb_define_method(cGMP_Z, "sqrt",  r_gmpz_sqrt, 0);
  rb_define_method(cGMP_Z, "sqrt!",  r_gmpz_sqrt_self, 0);
  rb_define_method(cGMP_Z, "sqrtrem",  r_gmpz_sqrtrem, 0);
  rb_define_method(cGMP_Z, "jacobi",  r_gmpz_jacobi, 0);
  rb_define_method(cGMP_Z, "legendre",  r_gmpz_legendre, 0);
  rb_define_method(cGMP_Z, "probab_prime?",  r_gmpz_is_probab_prime, -1);
  rb_define_method(cGMP_Z, "nextprime",  r_gmpz_nextprime, 0);
  rb_define_method(cGMP_Z, "nextprime!",  r_gmpz_nextprime_self, 0);
  rb_define_method(cGMP_Z, "popcount",  r_gmpz_popcount, 0);

}
