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
 *
 * The following list is just a simple checklist for me, really. A better
 * reference should be found in the rdocs.
 *
 *   Ruby method    C Extension function    GMP function
 *   to_d           r_gmpf_to_d             mpf_get_d
 *   to_s           r_gmpf_to_s             mpf_get_s
 *   +              r_gmpf_add              mpf_add
 *   -              r_gmpf_sub              mpf_sub
 *   *              r_gmpf_mul              mpf_mul
 *   /              r_gmpf_div              mpf_div
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

  if (argc > 4)
    rb_raise(rb_eArgError, "wrong # of arguments(%d for 0, 1 2, 3, or 4)", argc);

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
#ifdef MPFR
    mpfr_init (self_val);
    mpfr_set_si (self_val, 0, __gmp_default_rounding_mode);
#else
    r_mpf_init (self_val);
    mpf_set_si (self_val, 0);
#endif
    return Qnil;
  }

  arg = argv[0];

  //argc >= 2 ==> argv[0] is value, argv[1] is prec
  if (argc >= 2) {
    if (FIXNUM_P(argv[1])) {
      if (FIX2INT(argv[1]) >= 0)
        prec = FIX2INT(argv[1]);
      else {
        r_mpf_init (self_val);
        rb_raise(rb_eRangeError, "prec must be non-negative");
      }
    } else {
      r_mpf_init (self_val);
      rb_raise(rb_eTypeError, "prec must be a Fixnum");
    }
  } else if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    prec = mpf_get_prec (arg_val_f);
  }
#ifdef MPFR
  int base = 10;
  if (prec == 0)
    mpfr_init (self_val);
  else
    mpfr_init2 (self_val, prec);
  
  if (STRING_P(argv[0])) {
    if (argc >= 3) {
      if (FIXNUM_P(argv[2])) {
        if (FIX2INT(argv[2]) >= 2 && FIX2INT(argv[2]) <= 36)
          base = FIX2INT(argv[2]);
        else
          rb_raise(rb_eRangeError, "base must be between 2 and 36");
      }
      else {
        rb_raise(rb_eTypeError, "base must be a Fixnum");
      }
    }
    if (argc == 4) {
      // FIGURE IT OUT. ACCEPT A ROUNDING MODE!
    }
    
    mpf_set_value2 (self_val, arg, base);
    return Qnil;
  }

#else
  if (prec == 0)
    r_mpf_init (self_val);
  else
    r_mpf_init2 (self_val, prec);
#endif

  if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    mpf_set(self_val, arg_val_f);
  } else {
    mpf_set_value(self_val, arg);
  }

  return Qnil;
}

/* don't pass GMP::F here, it should be handled separately */
void mpf_set_value(MP_FLOAT *self_val, VALUE arg)
{
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  int result;

#ifdef MPFR
  if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    r_mpf_set_q(self_val, arg_val_q);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    r_mpf_set_z(self_val, arg_val_z);
  } else if (FLOAT_P(arg)) {
    mpfr_set_d(self_val, NUM2DBL(arg), __gmp_default_rounding_mode);
  } else if (FIXNUM_P(arg)) {
    mpf_set_si(self_val, FIX2NUM(arg));
  } else if (STRING_P(arg)) {
    result = mpfr_set_str(self_val, StringValuePtr(arg), 10, __gmp_default_rounding_mode);
    if (result == -1) {
      rb_raise(rb_eRuntimeError, "Badly formatted string");
    }
  } else if (BIGNUM_P(arg)) {
#if 1 /* GMP3 code */
    mpz_temp_from_bignum(arg_val_z, arg);
    r_mpf_set_z(self_val, arg_val_z);
    mpz_temp_free(arg_val_z);
#endif
#else
  if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    r_mpf_set_q(self_val, arg_val_q);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    r_mpf_set_z(self_val, arg_val_z);
  } else if (FLOAT_P(arg)) {
    r_mpf_set_d(self_val, NUM2DBL(arg));
  } else if (FIXNUM_P(arg)) {
    mpf_set_si(self_val, FIX2NUM(arg));
  } else if (STRING_P(arg)) {
    result = r_mpf_set_str(self_val, StringValuePtr(arg), 10);
    if (result == -1) {
      rb_raise(rb_eRuntimeError, "Badly formatted string");
    }
  } else if (BIGNUM_P(arg)) {
#if 1 /* GMP3 code */
    mpz_temp_from_bignum(arg_val_z, arg);
    r_mpf_set_z(self_val, arg_val_z);
    mpz_temp_free(arg_val_z);
#endif
#endif
  } else {
    rb_raise(rb_eTypeError, "Don't know how to convert %s into GMP::F", rb_class2name(rb_class_of(arg)));
  }
}

#ifdef MPFR
void mpf_set_value2(MP_FLOAT *self_val, VALUE arg, int base)
{
  int result;

  result = mpfr_set_str(self_val, StringValuePtr(arg), base, __gmp_default_rounding_mode);
  
  if (result == -1) {
    rb_raise(rb_eRuntimeError, "Badly formatted string");
  }
}
#endif

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

#ifdef MPFR
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
  
  //mpfr_sprintf(str, "%Rf", self_val);
  //res = rb_str_new2(str);
  
  str = mpfr_get_str(NULL, &exponent, 10, 0, self_val, __gmp_default_rounding_mode);
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
    mpfr_free_str(str2);
  }
  
  mpfr_free_str(str);
  return res;
}
#else
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
#endif


/**********************************************************************
 *    Float Arithmetic                                                *
 **********************************************************************/

#ifndef MPFR
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
  mpfr_prec_t prec;

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
    mpf_set_si (res_val, FIX2NUM(arg));
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
#else
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
#define DEFUN_F_ZQXFBD2F(fname)                                 \
VALUE r_gmpfr_##fname(int argc, VALUE *argv, VALUE self)        \
{                                                               \
  MP_FLOAT *self_val, *res_val, *arg_val_f;                     \
  MP_RAT *arg_val_q;                                            \
  MP_INT *arg_val_z;                                            \
  VALUE arg, res, res_prec, rnd_mode;                           \
  mpfr_prec_t prec, res_prec_val;                               \
  mp_rnd_t rnd_mode_val;                                        \
                                                                \
  rb_scan_args (argc, argv, "12", &arg, &rnd_mode, &res_prec);  \
                                                                \
  mpf_get_struct_prec (self, self_val, prec);                   \
  if (NIL_P (rnd_mode)) { rnd_mode_val = __gmp_default_rounding_mode; }    \
  else { rnd_mode_val = r_get_rounding_mode(rnd_mode); }        \
  if (NIL_P (res_prec)) { res_prec_val = prec; }                \
  else { res_prec_val = FIX2INT (res_prec); }                   \
  mpf_make_struct_init (res, res_val, res_prec_val);            \
                                                                \
  if (GMPF_P (arg)) {                                           \
    mpf_get_struct (arg, arg_val_f);                            \
    prec_max (prec, arg_val_f);                                 \
    mpf_make_struct_init (res, res_val, prec);                  \
    mpfr_##fname (res_val, self_val, arg_val_f, rnd_mode_val);  \
  } else if (GMPQ_P (arg)) {                                    \
    mpq_get_struct (arg, arg_val_q);                            \
    mpf_make_struct_init (res, res_val, prec);                  \
    mpfr_##fname##_q (res_val, self_val, arg_val_q, rnd_mode_val);  \
  } else if (GMPZ_P (arg)) {                                    \
    mpz_get_struct (arg, arg_val_z);                            \
    mpf_make_struct_init (res, res_val, prec);                  \
    mpfr_##fname##_z (res_val, self_val, arg_val_z, rnd_mode_val);  \
  } else if (FLOAT_P (arg)) {                                   \
    mpf_make_struct_init (res, res_val, prec);                  \
    mpfr_##fname##_d (res_val, self_val, NUM2DBL (arg), rnd_mode_val);  \
  } else if (FIXNUM_P (arg)) {                                  \
    mpf_make_struct_init(res, res_val, prec);                   \
    mpfr_##fname##_si (res_val, self_val, FIX2NUM (arg), rnd_mode_val);  \
  } else if (BIGNUM_P (arg)) {                                  \
    mpz_temp_from_bignum(arg_val_z, arg);                       \
    mpf_make_struct_init(res, res_val, prec);                   \
    mpfr_##fname##_z (res_val, self_val, arg_val_z, rnd_mode_val);  \
    mpz_temp_free (arg_val_z);                                  \
  } else {                                                      \
    typeerror(ZQFXBD);                                          \
  }                                                             \
                                                                \
  return res;                                                   \
}

DEFUN_F_ZQXFBD2F(add)
DEFUN_F_ZQXFBD2F(mul)
#endif

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
  mpfr_prec_t prec;

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
    mpf_set_si(res_val, FIX2NUM(arg));
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
  //unsigned long prec;
  mpfr_prec_t prec;

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
    mpf_set_si(res_val, FIX2NUM(arg));
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
 *   float ** integer
 *
 * Returns +float+ raised to the +integer+ power. +integer+ must be
 * * Fixnum or Bignum
 * * non-negative
 */
VALUE r_gmpf_pow(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val;
  VALUE res;
  
  //unsigned long prec;
  mpfr_prec_t prec;

  mpf_get_struct_prec (self, self_val, prec);

  if (FIXNUM_P(arg)) {
    if (FIX2NUM(arg) >= 0) {
      mpf_make_struct_init(res, res_val, prec);
      mpf_pow_ui(res_val, self_val, FIX2NUM(arg));
    } else {
      rb_raise(rb_eRangeError, "power must be non-negative");
    }
  } else {
    typeerror(X);
  }

  return res;
}

#ifdef MPFR
/*
 * call-seq:
 *   float ** other
 *
 * Returns +float+ raised to the +other+ power. +other+ must be an instance of
 * * Fixnum
 * * Bignum
 * * Float
 * * GMP::Z
 * * GMP::F
 */
/*VALUE r_gmpfr_pow(int argc, VALUE *argv, VALUE self)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  VALUE arg, rnd_mode, res_prec;
  unsigned long arg_val, prec, res_prec_value;
  mp_rnd_t rnd_mode_value;
  MP_INT *arg_val_z;
  VALUE res;
  
  rb_scan_args (argc, argv, "12", &arg, &rnd_mode, &res_prec);

  mpf_get_struct_prec (self, self_val, prec);
  
  if (NIL_P (rnd_mode)) { rnd_mode_value = __gmp_default_rounding_mode; }
  else { rnd_mode_value = r_get_rounding_mode(rnd_mode); }
  if (NIL_P (res_prec)) { res_prec_value = prec; }
  else { res_prec_value = FIX2INT (res_prec); }
  mpf_make_struct_init (res, res_val, res_prec_value);

  if (FIXNUM_P(arg)) {
    mpfr_pow_ui(res_val, self_val, FIX2NUM(arg), rnd_mode_value);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    mpfr_pow_z (res_val, self_val, arg_val_z, rnd_mode_value);
    mpz_temp_free(arg_val_z);
  } else if (FLOAT_P(arg)) {
    r_mpf_set_d (res_val, NUM2DBL(arg));
    mpfr_pow (res_val, self_val, res_val, rnd_mode_value);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct (arg, arg_val_z);
    mpfr_pow_z (res_val, self_val, arg_val_z, rnd_mode_value);
  } else if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    mpfr_pow (res_val, self_val, arg_val_f, rnd_mode_value);
  } else {
    typeerror(ZFXBD);
  }

  return res;
}*/
#endif

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
  mpfr_prec_t prec;

  mpf_get_struct_prec (self, self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct(arg, arg_val_f);
    prec_max(prec, arg_val_f);
    mpf_make_struct_init(res, res_val, prec);
    mpf_div(res_val, self_val, arg_val_f);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_q(res_val, arg_val_q);
    mpf_div(res_val, self_val, res_val);
  } else if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z(res_val, arg_val_z);
    mpf_div(res_val, self_val, res_val);
  } else if (FLOAT_P(arg)) {
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_d(res_val, NUM2DBL(arg));
    mpf_div(res_val, self_val, res_val);
  } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_si(res_val, FIX2NUM(arg));
    mpf_div(res_val, self_val, res_val);
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    mpf_make_struct_init(res, res_val, prec);
    mpf_set_z(res_val, arg_val_z);
    mpf_div(res_val, self_val, res_val);	
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
    return r_mpf_cmp (self_val, arg_val);
  } else {
    mpf_temp_init(arg_val, mpf_get_prec (self_val));
    mpf_set_value (arg_val, arg);
    result = r_mpf_cmp (self_val, arg_val);
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
  mpf_get_struct (self, self_val);
  res = mpf_cmp_value (self_val, arg);
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


#ifdef MPFR

#define MPFR_SINGLE_FUNCTION(name)                             \
VALUE r_gmpfr_##name(int argc, VALUE *argv, VALUE self)        \
{                                                              \
  MP_FLOAT *self_val, *res_val;                                \
  VALUE rnd_mode, res_prec;                                    \
  mpfr_prec_t prec, res_prec_value;                            \
  mp_rnd_t rnd_mode_val;                                       \
  VALUE res;                                                   \
                                                               \
  rb_scan_args (argc, argv, "02", &rnd_mode, &res_prec);       \
                                                               \
  mpf_get_struct_prec (self, self_val, prec);                  \
  if (NIL_P (rnd_mode)) { rnd_mode_val = __gmp_default_rounding_mode; }    \
  else { rnd_mode_val = r_get_rounding_mode(rnd_mode); }       \
  if (NIL_P (res_prec)) { res_prec_value = prec; }             \
  else { res_prec_value = FIX2INT (res_prec); }                \
  mpf_make_struct_init (res, res_val, res_prec_value);         \
  mpfr_##name (res_val, self_val, rnd_mode_val);               \
                                                               \
  return res;                                                  \
}

#define MPFR_SINGLE_1ARG_FUNCTION(name)                            \
VALUE r_gmpfr_##name(int argc, VALUE *argv, VALUE self)            \
{                                                                  \
  MP_FLOAT *self_val, *res_val;                                    \
  VALUE arg1, res_prec;                                            \
  mpfr_prec_t prec, arg1_val, res_prec_value;                      \
  VALUE res;                                                       \
                                                                   \
  rb_scan_args (argc, argv, "11", &arg1, &res_prec);               \
                                                                   \
  mpf_get_struct_prec (self, self_val, prec);                      \
  if (!FIXNUM_P (arg1)) { typeerror(ZXB); }                        \
  arg1_val = FIX2LONG (arg1);                                      \
  if (NIL_P (res_prec)) { res_prec_value = prec; }                 \
  else { res_prec_value = FIX2INT (res_prec); }                    \
  mpf_make_struct_init (res, res_val, res_prec_value);             \
  mpfr_##name (res_val, arg1_val, self_val, __gmp_default_rounding_mode);    \
                                                                   \
  return res;                                                      \
}

#define MPFR_SINGLE_BOOLEAN_FUNCTION(name)  \
static VALUE r_gmpfr_##name(VALUE self)     \
{                                           \
  MP_FLOAT *self_val;                       \
                                            \
  mpf_get_struct(self, self_val);           \
  if (mpfr_##name (self_val))               \
    return Qtrue;                           \
  else                                      \
    return Qfalse;                          \
}


#define MPFR_CONST_FUNCTION(name)                            \
VALUE r_gmpfrsg_##name(int argc, VALUE *argv, VALUE self)    \
{                                                            \
  (void)self;                                                \
  MP_FLOAT *res_val;                                         \
  VALUE res;                                                 \
  VALUE rnd_mode, prec;                                      \
  mp_rnd_t rnd_mode_val;                                     \
  mpfr_prec_t prec_val;                                      \
                                                             \
  rb_scan_args (argc, argv, "02", &rnd_mode, &prec);         \
                                                             \
  if (NIL_P (rnd_mode)) { rnd_mode_val = __gmp_default_rounding_mode; }    \
  else { rnd_mode_val = r_get_rounding_mode(rnd_mode); }     \
  if (NIL_P (prec)) { prec_val = mpfr_get_default_prec(); }  \
  else { prec_val = FIX2INT (prec); }                        \
  mpf_make_struct_init (res, res_val, prec_val);             \
  mpfr_##name (res_val, rnd_mode_val);                       \
                                                             \
  return res;                                                \
}

MPFR_SINGLE_FUNCTION(sqrt)
MPFR_SINGLE_FUNCTION(rec_sqrt)
MPFR_SINGLE_FUNCTION(cbrt)

MPFR_SINGLE_FUNCTION(log)
MPFR_SINGLE_FUNCTION(log2)
MPFR_SINGLE_FUNCTION(log10)
MPFR_SINGLE_FUNCTION(exp)
MPFR_SINGLE_FUNCTION(exp2)
MPFR_SINGLE_FUNCTION(exp10)
MPFR_SINGLE_FUNCTION(cos)
MPFR_SINGLE_FUNCTION(sin)
MPFR_SINGLE_FUNCTION(tan)
MPFR_SINGLE_FUNCTION(sec)
MPFR_SINGLE_FUNCTION(csc)
MPFR_SINGLE_FUNCTION(cot)

MPFR_SINGLE_FUNCTION(acos)
MPFR_SINGLE_FUNCTION(asin)
MPFR_SINGLE_FUNCTION(atan)

MPFR_SINGLE_FUNCTION(cosh)
MPFR_SINGLE_FUNCTION(sinh)
MPFR_SINGLE_FUNCTION(tanh)

MPFR_SINGLE_FUNCTION(sech)
MPFR_SINGLE_FUNCTION(csch)
MPFR_SINGLE_FUNCTION(coth)
MPFR_SINGLE_FUNCTION(acosh)
MPFR_SINGLE_FUNCTION(asinh)
MPFR_SINGLE_FUNCTION(atanh)

MPFR_SINGLE_FUNCTION(log1p)
MPFR_SINGLE_FUNCTION(expm1)
MPFR_SINGLE_FUNCTION(eint)
MPFR_SINGLE_FUNCTION(li2)
MPFR_SINGLE_FUNCTION(gamma)
MPFR_SINGLE_FUNCTION(lngamma)
/*MPFR_SINGLE_FUNCTION(lgamma)*/
#if MPFR_VERSION_MAJOR > 2
MPFR_SINGLE_FUNCTION(digamma)
#endif
MPFR_SINGLE_FUNCTION(zeta)
MPFR_SINGLE_FUNCTION(erf)
MPFR_SINGLE_FUNCTION(erfc)
MPFR_SINGLE_FUNCTION(j0)
MPFR_SINGLE_FUNCTION(j1)
MPFR_SINGLE_1ARG_FUNCTION(jn)
MPFR_SINGLE_FUNCTION(y0)
MPFR_SINGLE_FUNCTION(y1)
MPFR_SINGLE_1ARG_FUNCTION(yn)

MPFR_CONST_FUNCTION(const_log2)
MPFR_CONST_FUNCTION(const_pi)
MPFR_CONST_FUNCTION(const_euler)
MPFR_CONST_FUNCTION(const_catalan)

MPFR_SINGLE_BOOLEAN_FUNCTION(nan_p)
MPFR_SINGLE_BOOLEAN_FUNCTION(inf_p)
static VALUE r_gmpfr_fin_p(VALUE self)
{
  if (r_gmpfr_inf_p(self)) {
    return Qfalse;
  }
  else {
    return Qtrue;
  }
}
MPFR_SINGLE_BOOLEAN_FUNCTION(number_p)
MPFR_SINGLE_BOOLEAN_FUNCTION(zero_p)
#if MPFR_VERSION_MAJOR > 2
MPFR_SINGLE_BOOLEAN_FUNCTION(regular_p)
#endif

static VALUE r_gmpfr_pow(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  mpfr_prec_t prec;
  VALUE res;

  mpf_get_struct_prec(self, self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct(arg, arg_val_f);
    prec_max(prec, arg_val_f);
    mpf_make_struct_init(res, res_val, prec);
    mpfr_pow(res_val, self_val, arg_val_f, __gmp_default_rounding_mode);
  } else {
    mpf_make_struct_init(res, res_val, prec);

    if (GMPZ_P(arg)) {
      mpz_get_struct(arg, arg_val_z);
      mpf_set_z(res_val, arg_val_z);
      mpfr_pow(res_val, self_val, res_val, __gmp_default_rounding_mode);
    } else if (GMPQ_P(arg)) {
      mpq_get_struct(arg, arg_val_q);
      mpf_set_q(res_val, arg_val_q);
      mpfr_pow(res_val, self_val, res_val, __gmp_default_rounding_mode);
    } else if (FLOAT_P(arg)) {
      mpf_set_d(res_val, NUM2DBL(arg));
      mpfr_pow(res_val, self_val, res_val, __gmp_default_rounding_mode);
    } else if (FIXNUM_P(arg)) {
      mpfr_pow_si(res_val, self_val, FIX2INT(arg), __gmp_default_rounding_mode);
    } else if (BIGNUM_P(arg)) {
      mpz_temp_from_bignum(arg_val_z, arg);
      mpf_set_z(res_val, arg_val_z);
      mpz_temp_free(arg_val_z);
      mpfr_pow(res_val, self_val, res_val, __gmp_default_rounding_mode);
    } else {
      typeerror(ZQFXBD);
    }
  }

  return res;
}

#endif


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
  mpf_get_struct (self, self_val);
  return INT2FIX (mpf_sgn (self_val));
}

VALUE r_gmpf_get_prec(VALUE self)
{
  MP_FLOAT *self_val;
  mpf_get_struct (self, self_val);
  return INT2NUM (mpf_get_prec (self_val));
}

VALUE r_gmpf_set_prec(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val;
  if (FIXNUM_P(arg)) {
    mpf_get_struct (self, self_val);
    mpf_set_prec (self_val, FIX2NUM (arg));
    return Qnil;
  } else {
    typeerror(X);
  }
}

VALUE r_gmpf_set_prec_raw(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val;
  if (FIXNUM_P(arg)) {
    mpf_get_struct (self, self_val);
    mpf_set_prec_raw (self_val, FIX2NUM (arg));
    return Qnil;
  } else {
    typeerror(X);
  }
}


void init_gmpf()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "F", r_gmpmod_f, -1);
  
  cGMP_F = rb_define_class_under (mGMP, "F", rb_cNumeric);
  
  
  // Initializing, Assigning Floats
  rb_define_singleton_method(cGMP_F, "new", r_gmpfsg_new, -1);
  rb_define_method(cGMP_F, "initialize", r_gmpf_initialize, -1);
  rb_define_method(cGMP_F, "prec", r_gmpf_get_prec, 0);
  rb_define_method(cGMP_F, "prec=", r_gmpf_set_prec, 1);
  rb_define_method(cGMP_F, "prec_raw=", r_gmpf_set_prec_raw, 1);
  
  // Converting Floats
  rb_define_method(cGMP_F, "to_s", r_gmpf_to_s, 0);
  rb_define_method(cGMP_F, "to_d",  r_gmpf_to_d, 0);
  rb_define_alias(cGMP_F, "to_f", "to_d");
  
  // Float Arithmetic
  rb_define_method(cGMP_F, "-", r_gmpf_sub, 1);
  rb_define_method(cGMP_F, "/", r_gmpf_div, 1);
#ifndef MPFR
  rb_define_method(cGMP_F, "+", r_gmpf_add, 1);
  rb_define_method(cGMP_F, "*", r_gmpf_mul, 1);
  rb_define_method(cGMP_F, "**", r_gmpf_pow, 1);
  rb_define_alias(cGMP_F, "pow", "**");
#else
  rb_define_method(cGMP_F, "+", r_gmpfr_add, -1);
  rb_define_method(cGMP_F, "*", r_gmpfr_mul, -1);
  rb_define_method(cGMP_F, "**", r_gmpfr_pow, 1);
#endif
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
  rb_define_method(cGMP_F, "sgn", r_gmpf_sgn, 0);
  
  // Miscellaneous Functions
  rb_define_method(cGMP_F, "ceil",  r_gmpf_ceil, 0);
  rb_define_method(cGMP_F, "ceil!",  r_gmpf_ceil_self, 0);
  rb_define_method(cGMP_F, "floor",  r_gmpf_floor, 0);
  rb_define_method(cGMP_F, "floor!",  r_gmpf_floor_self, 0);rb_define_method(cGMP_F, "trunc",  r_gmpf_trunc, 0);
  rb_define_method(cGMP_F, "trunc!",  r_gmpf_trunc_self, 0);
  
  
#ifdef MPFR
  /* To implement; new in MPFR 3.0.0:
   *
   * mpfr_buildopt_tls_p
   * mpfr_buildopt_decimal_p
   * mpfr_set_zero
   * mpfr_ai
   * mpfr_set_flt
   * mpfr_get_flt
   * mpfr_urandom
   * mpfr_set_z_2exp
   */
  // Basic Arithmetic Functions
  rb_define_method(cGMP_F, "sqrt",     r_gmpfr_sqrt,     -1);
  rb_define_method(cGMP_F, "rec_sqrt", r_gmpfr_rec_sqrt, -1);
  rb_define_method(cGMP_F, "cbrt",     r_gmpfr_cbrt,     -1);
  // "root", r_gmpfr_root
  // "neg", r_gmpfr_neg
  // "abs", r_gmpfr_abs
  // "dim", r_gmpfr_dim
  // "mul_2", r_gmpfr_mul_2
  // "div_2", r_gmpfr_div_2
  
  //rb_define_method(cGMP_F, "**", r_gmpfr_pow, 1);
  
  // Comparison Functions
  rb_define_method(cGMP_F, "nan?", r_gmpfr_nan_p, 0);
  rb_define_method(cGMP_F, "infinite?", r_gmpfr_inf_p, 0);
  rb_define_method(cGMP_F, "finite?", r_gmpfr_fin_p, 0);
  rb_define_method(cGMP_F, "number?", r_gmpfr_number_p, 0);
  rb_define_method(cGMP_F, "zero?", r_gmpfr_zero_p, 0);
#if MPFR_VERSION_MAJOR > 2
  rb_define_method(cGMP_F, "regular?", r_gmpfr_regular_p, 0);
#endif
  //"sgn", r_gmpfr_sgn
  //"lessgreater", r_gmpfr_lessgreater_p
  //"unordered", r_gmpfr_unordered_p
  
  // Special Functions
  rb_define_method(cGMP_F, "log",   r_gmpfr_log,   -1);
  rb_define_method(cGMP_F, "log2",  r_gmpfr_log2,  -1);
  rb_define_method(cGMP_F, "log10", r_gmpfr_log10, -1);
  rb_define_method(cGMP_F, "exp",   r_gmpfr_exp,   -1);
  rb_define_method(cGMP_F, "exp2",  r_gmpfr_exp2,  -1);
  rb_define_method(cGMP_F, "exp10", r_gmpfr_exp10, -1);
  rb_define_method(cGMP_F, "cos",   r_gmpfr_cos,   -1);
  rb_define_method(cGMP_F, "sin",   r_gmpfr_sin,   -1);
  rb_define_method(cGMP_F, "tan",   r_gmpfr_tan,   -1);
  // "sin_cos", r_gmpfr_sin_cos
  rb_define_method(cGMP_F, "sec",   r_gmpfr_sec,   -1);
  rb_define_method(cGMP_F, "csc",   r_gmpfr_csc,   -1);
  rb_define_method(cGMP_F, "cot",   r_gmpfr_cot,   -1);
  
  rb_define_method(cGMP_F, "acos",  r_gmpfr_acos,  -1);
  rb_define_method(cGMP_F, "asin",  r_gmpfr_asin,  -1);
  rb_define_method(cGMP_F, "atan",  r_gmpfr_atan,  -1);
  // "atan2", r_gmpfr_atan2
  
  rb_define_method(cGMP_F, "cosh",  r_gmpfr_cosh,  -1);
  rb_define_method(cGMP_F, "sinh",  r_gmpfr_sinh,  -1);
  rb_define_method(cGMP_F, "tanh",  r_gmpfr_tanh,  -1);

  // "sinh_cosh", r_gmpfr_sinh_cosh
  
  rb_define_method(cGMP_F, "sech",  r_gmpfr_sech,  -1);
  rb_define_method(cGMP_F, "csch",  r_gmpfr_csch,  -1);
  rb_define_method(cGMP_F, "coth",  r_gmpfr_coth,  -1);
  rb_define_method(cGMP_F, "acosh", r_gmpfr_acosh, -1);
  rb_define_method(cGMP_F, "asinh", r_gmpfr_asinh, -1);
  rb_define_method(cGMP_F, "atanh", r_gmpfr_atanh, -1);
  
  // "fac", r_gmpfr_fac

  rb_define_method(cGMP_F, "log1p",   r_gmpfr_log1p,   -1);
  rb_define_method(cGMP_F, "expm1",   r_gmpfr_expm1,   -1);
  rb_define_method(cGMP_F, "eint",    r_gmpfr_eint,    -1);
  rb_define_method(cGMP_F, "li2",     r_gmpfr_li2,     -1);
  rb_define_method(cGMP_F, "gamma",   r_gmpfr_gamma,   -1);
  rb_define_method(cGMP_F, "lngamma", r_gmpfr_lngamma, -1);
  /*rb_define_method(cGMP_F, "lgamma",  r_gmpfr_lgamma,   -1);*/
#if MPFR_VERSION_MAJOR > 2
  rb_define_method(cGMP_F, "digamma", r_gmpfr_digamma, -1);
#endif
  rb_define_method(cGMP_F, "zeta", r_gmpfr_zeta,       -1);
  rb_define_method(cGMP_F, "erf", r_gmpfr_erf,         -1);
  rb_define_method(cGMP_F, "erfc", r_gmpfr_erfc,       -1);
  rb_define_method(cGMP_F, "j0", r_gmpfr_j0,           -1);
  rb_define_method(cGMP_F, "j1", r_gmpfr_j1,           -1);
  rb_define_method(cGMP_F, "jn", r_gmpfr_jn,           -1);
  rb_define_method(cGMP_F, "y0", r_gmpfr_y0,           -1);
  rb_define_method(cGMP_F, "y1", r_gmpfr_y1,           -1);
  rb_define_method(cGMP_F, "yn", r_gmpfr_yn,           -1);

  // "fma", r_gmpfr_fma
  // "fms", r_gmpfr_fms
  // "agm", r_gmpfr_agm
  // "hypot", r_gmpfr_hypot
  // "ai", r_gmpfr_ai  !! 3.0.0
  
  rb_define_singleton_method(cGMP_F, "const_log2",    r_gmpfrsg_const_log2,    -1);
  rb_define_singleton_method(cGMP_F, "const_pi",      r_gmpfrsg_const_pi,      -1);
  rb_define_singleton_method(cGMP_F, "const_euler",   r_gmpfrsg_const_euler,   -1);
  rb_define_singleton_method(cGMP_F, "const_catalan", r_gmpfrsg_const_catalan, -1);

  // Integer and Remainder Related Functions
  // "integer?", r_gmpfr_integer_p
#endif /* MPFR */
  
  // _unsorted_
}
