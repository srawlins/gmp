#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

/*
 * Document-class: GMP::F
 *
 * GMP Multiple Precision floating point numbers.
 *
 * Instances of this class can store variables of the type `mpf_t`. This class
 * also contains many methods that act as the functions for `mpf_t` variables,
 * as well as a few methods that attempt to make this library more Ruby-ish.
 */

/**********************************************************************
 *    Macros                                                          *
 **********************************************************************/

#define DEFUN_FLOAT2FLOAT(fname,mpf_fname)                  \
static VALUE r_gmpf_##fname(VALUE self_val)                 \
{                                                           \
  MP_FLOAT *self, *res;                                     \
  VALUE res_val;                                            \
  mpf_get_struct (self_val, self);                          \
  mpf_make_struct_init (res_val, res, mpf_get_prec(self));  \
  mpf_fname (res, self);                                    \
  return res_val;                                           \
}                                                           \
                                                            \
static VALUE r_gmpf_##fname##_self(VALUE self)              \
{                                                           \
  MP_FLOAT *self_val;                                       \
  mpf_get_struct (self, self_val);                          \
  mpf_fname (self_val, self_val);                           \
  return Qnil;                                              \
}

#define DEFUN_FLOAT_CMP(name,CMP_OP)                                 \
static VALUE r_gmpf_cmp_##name(VALUE self_val, VALUE arg_val)        \
{                                                                    \
  MP_FLOAT *self;                                                    \
  mpf_get_struct (self_val, self);                                   \
  return (mpf_cmp_value (self, arg_val) CMP_OP 0) ? Qtrue : Qfalse;  \
}


/**********************************************************************
 *    Initializing, Assigning Floats                                  *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::F.new(value)
 *   GMP::F.new(value, precision)
 *   GMP::F.new(value, precision, rounding_mode) (MPFR only)
 *   GMP::F.new(string_value, precision, base)
 *
 * Creates a new GMP::F floating-point number, with _value_ as its value,
 * converting where necessary.  _value_ must be an instance of one of the
 * following classes:
 *
 * * Fixnum
 * * Bignum
 * * GMP::Z
 * * Float
 * * GMP::Q
 * * GMP::F
 * * String
 *
 * @example
 *   GMP::F.new(5)                    #=> 5
 *   GMP::F(3**41)                    #=> 0.36472996377170788e+20
 *   GMP::F(3**41, 32)                #=> 0.36472996375e+20
 *   GMP::F(3**41, 32, GMP::GMP_RNDU) #=> 0.36472996384e+20
 *   GMP::F.new("20")                 #=> 20
 *   GMP::F.new("0x20")               #=> 32
 *   GMP::F("111", 16)                #=> 111
 *   GMP::F("111", 16, 2)             #=> 7
 *   GMP::F("111", 16, 16)            #=> 273
 */
VALUE r_gmpfsg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_FLOAT *res_val;
  VALUE res;
  (void)klass;

  if (argc > 4)
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0, 1, 2, 3, or 4)", argc);

  mpf_make_struct (res, res_val);
  rb_obj_call_init(res, argc, argv);

  return res;
}

VALUE r_gmpf_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_FLOAT *self_val, *arg_val_f;
  unsigned long prec = 0;
  VALUE arg;
#ifdef MPFR
  mp_rnd_t rnd_mode_val;
#endif
  int base = 10;

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

  /* argc >= 2 ==> argv[0] is value, argv[1] is prec */
  if (argc >= 2) {
    if (FIXNUM_P(argv[1])) {
      if (FIX2INT(argv[1]) >= 0)
        prec = FIX2INT(argv[1]);
      else {
        r_mpf_init (self_val);
        rb_raise(rb_eRangeError, "precision must be non-negative");
      }
    } else {
      r_mpf_init (self_val);
      rb_raise(rb_eTypeError, "precision must be a Fixnum");
    }
  } else if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    prec = mpf_get_prec (arg_val_f);
  }
#ifdef MPFR
  rnd_mode_val = __gmp_default_rounding_mode;

  if (prec == 0)
    mpfr_init (self_val);
  else
    mpfr_init2 (self_val, prec);

  if (STRING_P (argv[0])) {
    if (argc >= 3) {
      if (! FIXNUM_P (argv[2]))
        rb_raise(rb_eTypeError, "base must be a Fixnum");

      if (FIX2INT (argv[2]) >= 2 && FIX2INT (argv[2]) <= 36)
        base = FIX2INT (argv[2]);
      else
        rb_raise (rb_eRangeError, "base must be between 2 and 36");

      if (argc == 4)
        rnd_mode_val = r_get_rounding_mode (argv[3]);
      else
        rnd_mode_val = __gmp_default_rounding_mode;
    }


    mpf_set_value2 (self_val, arg, base, rnd_mode_val);
    return Qnil;
  } else {  /* not STRING_P(argv[0]) */
    if (argc == 3)
      rnd_mode_val = r_get_rounding_mode (argv[2]);
  }

  if (GMPF_P (arg)) {
    mpf_get_struct (arg, arg_val_f);
    mpfr_set (self_val, arg_val_f, rnd_mode_val);
  } else {
    mpfr_set_value (self_val, arg, rnd_mode_val);
  }

#else  /* not MPFR */
  (void)base;

  if (prec == 0)
    r_mpf_init (self_val);
  else
    r_mpf_init2 (self_val, prec);

  if (GMPF_P(arg)) {
    mpf_get_struct (arg, arg_val_f);
    mpf_set (self_val, arg_val_f);
  } else {
    mpf_set_value (self_val, arg);
  }
#endif /* MPFR */

  return Qnil;
}

#ifndef MPFR
/* don't pass GMP::F here, it should be handled separately */
void mpf_set_value(MP_FLOAT *self_val, VALUE arg)
{
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  int result;

  if (GMPQ_P (arg)) {
    mpq_get_struct (arg, arg_val_q);
    r_mpf_set_q (self_val, arg_val_q);
  } else if (GMPZ_P (arg)) {
    mpz_get_struct (arg, arg_val_z);
    r_mpf_set_z (self_val, arg_val_z);
  } else if (FLOAT_P (arg)) {
    r_mpf_set_d (self_val, NUM2DBL (arg));
  } else if (FIXNUM_P (arg)) {
    mpf_set_si (self_val, FIX2NUM (arg));
  } else if (STRING_P (arg)) {
    result = r_mpf_set_str (self_val, StringValuePtr (arg), 10);
    if (result == -1) {
      rb_raise (rb_eRuntimeError, "Badly formatted string");
    }
  } else if (BIGNUM_P (arg)) {
#if 1 /* GMP3 code */
    mpz_temp_from_bignum (arg_val_z, arg);
    r_mpf_set_z (self_val, arg_val_z);
    mpz_temp_free (arg_val_z);
#endif /* GMP3 code */
  } else {
    rb_raise (rb_eTypeError, "Don't know how to convert %s into GMP::F", rb_class2name (rb_class_of (arg)));
  }
}

#else /* MPFR */
/* don't pass GMP::F here, it should be handled separately */
void mpfr_set_value(MP_FLOAT *self_val, VALUE arg, mp_rnd_t rnd_mode_val)
{
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  int result;

  if (GMPQ_P (arg)) {
    mpq_get_struct (arg, arg_val_q);
    r_mpfr_set_q (self_val, arg_val_q, rnd_mode_val);
  } else if (GMPZ_P (arg)) {
    mpz_get_struct (arg, arg_val_z);
    r_mpfr_set_z (self_val, arg_val_z, rnd_mode_val);
  } else if (FLOAT_P (arg)) {
    mpfr_set_d (self_val, NUM2DBL (arg), rnd_mode_val);
  } else if (TYPE (arg) == T_FIXNUM) {
    mpfr_set_si (self_val, FIX2NUM (arg), rnd_mode_val);
  } else if (STRING_P(arg)) {
    result = mpfr_set_str (self_val, StringValuePtr (arg), 10, rnd_mode_val);
    if (result == -1) {
      rb_raise(rb_eRuntimeError, "Badly formatted string");
    }
  } else if (BIGNUM_P (arg)) {
#if 1 /* GMP3 code */
    mpz_temp_from_bignum (arg_val_z, arg);
    r_mpfr_set_z (self_val, arg_val_z, rnd_mode_val);
    mpz_temp_free (arg_val_z);
#endif /* GMP3 code */
  } else {
    rb_raise (rb_eTypeError, "Don't know how to convert %s into GMP::F", rb_class2name (rb_class_of (arg)));
  }
}

void mpf_set_value2(MP_FLOAT *self_val, VALUE arg, int base, mp_rnd_t rnd_mode_val)
{
  int result;

  result = mpfr_set_str (self_val, StringValuePtr (arg), base, rnd_mode_val);

  if (result == -1) {
    rb_raise (rb_eRuntimeError, "Badly formatted string");
  }
}

#if MPFR_VERSION_MAJOR > 2
VALUE r_gmpfsg_new_2exp(int argc, VALUE *argv, VALUE klass)
{
  MP_FLOAT *res;
  MP_INT *arg_z;
  VALUE res_val, arg_val, exp_val, prec_val, rnd_mode_val;
  mp_rnd_t rnd_mode;
  (void)klass;

  rb_scan_args (argc, argv, "22", &arg_val, &exp_val, &prec_val, &rnd_mode_val);
  mpf_make_struct (res_val, res);

  if (!FIXNUM_P (exp_val)) {
    mpfr_init (res);
    rb_raise(rb_eTypeError, "exp must be a Fixnum");
  }

  if (NIL_P (prec_val))
    mpfr_init (res);
  else if (FIXNUM_P (prec_val)) {
    if (FIX2INT (prec_val) >= 0) {
      mpfr_init2 (res, FIX2INT (prec_val));
    } else {
      mpfr_init (res);
      rb_raise(rb_eRangeError, "precision must be non-negative");
    }
  } else {
    mpfr_init (res);
    rb_raise(rb_eTypeError, "precision must be a Fixnum");
  }

  if (NIL_P (rnd_mode_val))
    rnd_mode = __gmp_default_rounding_mode;
  else
    rnd_mode = r_get_rounding_mode (rnd_mode_val);

  if (GMPZ_P (arg_val)) {
    mpz_get_struct (arg_val, arg_z);
    mpfr_set_z_2exp (res, arg_z, FIX2NUM (exp_val), rnd_mode);
  } else if (FIXNUM_P (arg_val)) {
    mpfr_set_si_2exp (res, FIX2NUM (arg_val), FIX2NUM (exp_val), rnd_mode);
  } else if (BIGNUM_P (arg_val)) {
#if 1 /* GMP3 code */
    mpz_temp_from_bignum (arg_z, arg_val);
    mpfr_set_z_2exp (res, arg_z, FIX2NUM (exp_val), rnd_mode);
    mpz_temp_free (arg_z);
#endif /* GMP3 code */
  } else {
    rb_raise (rb_eTypeError, "Don't know how to convert %s into GMP::F", rb_class2name (rb_class_of (arg_val)));
    typeerror_as (ZXB, "value");
  }

  return res_val;
}
#endif /* MPFR_VERSION_MAJOR > 2 */

/*
 * Document-method: nan
 * call-seq:
 *   GMP::F.nan
 *
 * `NaN`, an instance of GMP::F
 *
 * @since 0.6.47
 */
VALUE r_gmpfsg_nan(VALUE klass)
{
  MP_FLOAT *res;
  VALUE res_val;
  (void)klass;

  mpf_make_struct_init (res_val, res, mpfr_get_default_prec());
  mpfr_set_nan (res);

  return res_val;
}

/*
 * Document-method: inf
 * call-seq:
 *   GMP::F.inf
 *   GMP::F.inf(sign)
 *
 * `Inf` (positive infinity), an instance of GMP::F, or `-Inf` (negative
 * infinity), if a negative Fixnum _sign_ is passed
 *
 * @since 0.6.47
 */
VALUE r_gmpfsg_inf(int argc, VALUE *argv, VALUE klass)
{
  MP_FLOAT *res;
  VALUE sign_val, res_val;
  int sign = 0;
  (void)klass;

  rb_scan_args (argc, argv, "01", &sign_val);

  if (NIL_P (sign_val))         { sign = 1; }
  else if (FIXNUM_P (sign_val)) { sign = FIX2INT (sign_val); }
  else                          { typeerror_as (X, "sign"); }
  mpf_make_struct_init (res_val, res, mpfr_get_default_prec());
  mpfr_set_inf (res, sign);

  return res_val;
}

#if MPFR_VERSION_MAJOR > 2
/*
 * Document-method: zero
 * call-seq:
 *   GMP::F.zero
 *   GMP::F.zero(sign)
 *
 * zero or negative zero, an instance of GMP::F, depending on _sign_, a Fixnum
 *
 * @since 0.6.47
 */
VALUE r_gmpfsg_zero(int argc, VALUE *argv, VALUE klass)
{
  MP_FLOAT *res;
  VALUE sign_val, res_val;
  int sign = 0;
  (void)klass;

  rb_scan_args (argc, argv, "01", &sign_val);

  if (NIL_P (sign_val))         { sign = 1; }
  else if (FIXNUM_P (sign_val)) { sign = FIX2INT (sign_val); }
  else                          { typeerror_as (X, "sign"); }
  mpf_make_struct_init (res_val, res, mpfr_get_default_prec());
  mpfr_set_zero (res, sign);

  return res_val;
}
#endif  /* MPFR_VERSION_MAJOR > 2 */
#endif  /* MPFR */

/*
 * call-seq:
 *   GMP::F(arg)
 *
 * A convenience method for GMP::F.new(arg).
 */
VALUE r_gmpmod_f(int argc, VALUE *argv, VALUE module)
{
  (void)module;
  return r_gmpfsg_new(argc, argv, cGMP_F);
}


/**********************************************************************
 *    Converting Floats                                               *
 **********************************************************************/

/*
 * Document-method: to_d
 * call-seq:
 *   x.to_d
 *
 * Returns _x_ as a Float.
 */
VALUE r_gmpf_to_d(VALUE self)
{
  MP_FLOAT *self_val;
  mpf_get_struct(self, self_val);

  return rb_float_new(mpf_get_d(self_val));
}

/*
 * Document-method: to_s
 * call-seq:
 *   x.to_s(base = 10)
 *
 * Returns a representation of _x_, as a String. By default, the String will be
 * the decimal representation. Any valid GMP base can be passed.
 */
VALUE r_gmpf_to_s(int argc, VALUE *argv, VALUE self_val)
{
  MP_FLOAT *self;
  char *str, *str2;
  VALUE res_val;
  mp_exp_t exponent;
  VALUE base_val;
  int base = 10;

  mpf_get_struct (self_val, self);

  /* TODO: accept a second optional argument, n_digits */
  rb_scan_args (argc, argv, "01", &base_val);
  if (NIL_P (base_val)) { base = 10; }                /* default value */
  else { base = get_base (base_val); }

#ifndef MPFR
  str = mpf_get_str (NULL, &exponent, base, 0, self);
#else
  str = mpfr_get_str (NULL, &exponent, base, 0, self, __gmp_default_rounding_mode);
#endif
  if ((strcmp (str,  "NaN") == 0) ||
      (strcmp (str,  "Inf") == 0) ||
      (strcmp (str, "-Inf") == 0))
  {
    res_val = rb_str_new2 (str);
  }
  else
  {
    if (str[0] == '-')
      __gmp_asprintf (&str2, "-0.%se%+ld", str+1, exponent);
    else
      __gmp_asprintf (&str2, "0.%se%+ld", str, exponent);

    res_val = rb_str_new2 (str2);
#ifndef MPFR
    free (str2);
#else
    mpfr_free_str (str2);
#endif
  }

#ifndef MPFR
  free (str);
#else
  mpfr_free_str (str);
#endif
  return res_val;
}


/**********************************************************************
 *    Float Arithmetic                                                *
 **********************************************************************/

#ifndef MPFR
/*
 * Document-method: +
 * call-seq:
 *   x + y
 *
 * Returns the sum of _x_ and _y_. _y_ must be an instance of:
 *
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
  } else if (FIXNUM_P(arg)) { /* TODO: _ui with sign control instead */
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
 * Document-method: +
 * call-seq:
 *   x + y
 *
 * Returns the sum of _x_ and _y_. _y_ must be an instance of:
 *
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
 * Document-method: -
 * call-seq:
 *   x - y
 *
 * Subtracts _y_ from _x_. _y_ must be an instance of:
 *
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
  VALUE res = 0;
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
  } else if (FIXNUM_P(arg)) { /* TODO: _ui with sign control instead ? */
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
 * Document-method: *
 * call-seq:
 *   x * y
 *
 * Returns the product of _x_ and _y_. _y_ can be one of:
 *
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
  VALUE res = 0;
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
  } else if (FIXNUM_P(arg)) { /* _ui with sign control instead ? */
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
 * Document-method: **
 * call-seq:
 *   x ** y
 *
 * Returns _x_ raised to the _y_ power. _y_ must be
 *
 * * an instance of Fixnum or Bignum
 * * non-negative
 */
VALUE r_gmpf_pow(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val;
  VALUE res = 0;

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

/*
 * Document-method: /
 * call-seq:
 *   x / y
 *
 * Divides _x_ by _y_. _y_ can be
 *
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
  VALUE res = 0;
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
  } else if (FIXNUM_P(arg)) { /* TODO: _ui with sign control instead */
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

#ifdef MPFR
/*
 * Document-method: **
 * call-seq:
 *   float ** other
 *
 * Returns _x_ raised to the _y_ power. _y_ must be an instance of:
 *
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
 * Document-method: neg
 * call-seq:
 *   x.neg
 *   -x
 *
 * Returns -_x_.
 */
/*
 * Document-method: neg!
 * call-seq:
 *   x.neg!
 *
 * Sets _x_ to -_x_.
 */
DEFUN_FLOAT2FLOAT(neg,mpf_neg)

/*
 * Document-method: abs
 * call-seq:
 *   x.abs
 *
 * Returns the absolute value of _x_.
 */
/*
 * Document-method: abs!
 * call-seq:
 *   x.abs!
 *
 * Sets _x_ to the absolute value of _x_.
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
#ifndef MPFR
    mpf_set_value (arg_val, arg);
#else
    mpfr_set_value (arg_val, arg, __gmp_default_rounding_mode);
#endif
    result = r_mpf_cmp (self_val, arg_val);
    mpf_temp_free(arg_val);
    return result;
  }
}

/*
 * what does really "equal" mean ? it's not obvious
 * Is this a note that I, srawlins, put in here? It is not obvious to me...
 */
VALUE r_gmpf_eq(VALUE self_val, VALUE arg_val)
{
  MP_FLOAT *self;
  mpf_get_struct (self_val, self);
  return (mpf_cmp_value (self, arg_val) == 0) ? Qtrue : Qfalse;
}

VALUE r_gmpf_cmp(VALUE self_val, VALUE arg_val)
{
  MP_FLOAT *self;
  int res;
  mpf_get_struct (self_val, self);
  res = mpf_cmp_value (self, arg_val);
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

/*
 * Document-method: sgn
 * call-seq:
 *   x.sgn
 *
 * Returns +1 if _x_ > 0, 0 if _x_ == 0, and -1 if _x_ < 0.
 */
VALUE r_gmpf_sgn(VALUE self_val)
{
  MP_FLOAT *self;
  mpf_get_struct (self_val, self);
  return INT2FIX (mpf_sgn (self));
}

#ifdef MPFR

/*
 * Document-method: lessgreater?
 * call-seq:
 *   x.lessgreater?(y)
 *
 * Return true if _x_ < _y_ or _x_ > _y_, false otherwise
 *
 * @since 0.6.47
 */
VALUE r_gmpfr_lessgreater_p(VALUE self_val, VALUE arg_val)
{
  MP_FLOAT *self, *arg;

  if (!GMPF_P (arg_val))
    typeerror_as (F, "arg");

  mpf_get_struct (self_val, self);
  mpf_get_struct (arg_val, arg);
  return (mpfr_lessgreater_p (self, arg) != 0) ? Qtrue : Qfalse;
}

/*
 * Document-method: unordered?
 * call-seq:
 *   x.unordered?(y)
 *
 * Return true if _x_ or _y_ is a `NaN`, false otherwise
 *
 * @since 0.6.47
 */
VALUE r_gmpfr_unordered_p(VALUE self_val, VALUE arg_val)
{
  MP_FLOAT *self, *arg;

  if (!GMPF_P (arg_val))
    typeerror_as (F, "arg");

  mpf_get_struct (self_val, self);
  mpf_get_struct (arg_val, arg);
  return (mpfr_unordered_p (self, arg) != 0) ? Qtrue : Qfalse;
}

#endif  /* MPFR */


/**********************************************************************
 *    Miscellaneous Float Functions                                   *
 **********************************************************************/

DEFUN_FLOAT2FLOAT(floor,mpf_floor)
DEFUN_FLOAT2FLOAT(trunc,mpf_trunc)
DEFUN_FLOAT2FLOAT(ceil,mpf_ceil)


#ifdef MPFR

#define MPFR_SINGLE_FUNCTION(name)                                       \
VALUE r_gmpfr_##name(int argc, VALUE *argv, VALUE self_val)              \
{                                                                        \
  MP_FLOAT *self, *res;                                                  \
  VALUE rnd_mode_val, res_prec_val, res_val;                             \
  mpfr_prec_t prec, res_prec;                                            \
  mp_rnd_t rnd_mode;                                                     \
                                                                         \
  mpf_get_struct_prec (self_val, self, prec);                            \
                                                                         \
  rb_scan_args (argc, argv, "02", &rnd_mode_val, &res_prec_val);         \
  if (NIL_P (rnd_mode_val)) { rnd_mode = __gmp_default_rounding_mode; }  \
  else { rnd_mode = r_get_rounding_mode(rnd_mode_val); }                 \
  if (NIL_P (res_prec_val)) { res_prec = prec; }                         \
  else if (FIXNUM_P (res_prec_val)) { res_prec = FIX2INT (res_prec_val); } \
  else { typeerror_as (X, "precision"); }                                \
  mpf_make_struct_init (res_val, res, res_prec);                         \
  mpfr_##name (res, self, rnd_mode);                                     \
                                                                         \
  return res_val;                                                        \
}

#define MPFR_DOUBLE_FUNCTION(name)                                   \
VALUE r_gmpfr_##name(int argc, VALUE *argv, VALUE self)              \
{                                                                    \
  MP_FLOAT *self_val, *sin_val, *cos_val;                            \
  VALUE rnd_mode, sin_prec, cos_prec, sinn, coss;                    \
  mpfr_prec_t prec, sin_prec_val, cos_prec_val;                      \
  mp_rnd_t rnd_mode_val;                                             \
                                                                     \
  mpf_get_struct_prec (self, self_val, prec);                        \
                                                                     \
  rb_scan_args (argc, argv, "03", &rnd_mode, &sin_prec, &cos_prec);  \
  if (NIL_P (rnd_mode)) { rnd_mode_val = __gmp_default_rounding_mode; }  \
  else { rnd_mode_val = r_get_rounding_mode(rnd_mode); }             \
  if (NIL_P (sin_prec)) { sin_prec_val = prec; }                     \
  /* TODO test type */                                               \
  else { sin_prec_val = FIX2INT (sin_prec); }                        \
  if (NIL_P (cos_prec)) { cos_prec_val = sin_prec_val; }             \
  /* TODO test type */                                               \
  else { cos_prec_val = FIX2INT (cos_prec); }                        \
  mpf_make_struct_init (sinn, sin_val, sin_prec_val);                \
  mpf_make_struct_init (coss, cos_val, cos_prec_val);                \
  mpfr_##name (sin_val, cos_val, self_val, rnd_mode_val);            \
                                                                     \
  return rb_ary_new3(2, sinn, coss);                                 \
}

#define MPFR_SINGLE_LONG_FUNCTION(name)                            \
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
  mpfr_##name (res_val, arg1_val, self_val, __gmp_default_rounding_mode);  \
                                                                   \
  return res;                                                      \
}

#define MPFR_SINGLE_MPF_FUNCTION(name)                                     \
VALUE r_gmpfr_##name(int argc, VALUE *argv, VALUE self)                    \
{                                                                          \
  MP_FLOAT *self_val, *arg1_val, *res_val;                                 \
  VALUE arg1, rnd_mode, res_prec;                                          \
  mpfr_prec_t prec, arg1_prec, res_prec_value;                             \
  VALUE res;                                                               \
  mp_rnd_t rnd_mode_val;                                                   \
                                                                           \
  rb_scan_args (argc, argv, "12", &arg1, &rnd_mode, &res_prec);            \
                                                                           \
  mpf_get_struct_prec (self, self_val, prec);                              \
  if (GMPF_P (arg1)) {                                                     \
    mpf_get_struct_prec (arg1, arg1_val, arg1_prec);                       \
  } else if (FLOAT_P (arg1)) {                                             \
    mpf_temp_init(arg1_val, mpf_get_prec (self_val));                      \
    mpfr_set_d (arg1_val, NUM2DBL (arg1), __gmp_default_rounding_mode);    \
  } else {                                                                 \
    typeerror (FD);                                                        \
  }                                                                        \
  if (NIL_P (rnd_mode)) { rnd_mode_val = __gmp_default_rounding_mode; }    \
  else { rnd_mode_val = r_get_rounding_mode(rnd_mode); }                   \
  if (NIL_P (res_prec)) { res_prec_value = prec; }                         \
  /* TODO test type */                                                     \
  else { res_prec_value = FIX2INT (res_prec); }                            \
  mpf_make_struct_init (res, res_val, res_prec_value);                     \
  mpfr_##name (res_val, self_val, arg1_val, rnd_mode_val);                 \
  if (FLOAT_P (arg1)) {                                                    \
    mpf_temp_free(arg1_val);                                               \
  }                                                                        \
                                                                           \
  return res;                                                              \
}

#define MPFR_SINGLE_BOOLEAN_FUNCTION(name)  \
static VALUE r_gmpfr_##name(VALUE self)     \
{                                           \
  MP_FLOAT *self_val;                       \
                                            \
  mpf_get_struct (self, self_val);          \
  if (mpfr_##name (self_val))               \
    return Qtrue;                           \
  else                                      \
    return Qfalse;                          \
}


#define MPFR_CONST_FUNCTION(name)                            \
VALUE r_gmpfrsg_##name(int argc, VALUE *argv, VALUE self)    \
{                                                            \
  MP_FLOAT *res_val;                                         \
  VALUE res;                                                 \
  VALUE rnd_mode, prec;                                      \
  mp_rnd_t rnd_mode_val;                                     \
  mpfr_prec_t prec_val = 0;                                  \
  (void)self;                                                \
                                                             \
  rb_scan_args (argc, argv, "02", &rnd_mode, &prec);         \
                                                             \
  if (NIL_P (rnd_mode)) { rnd_mode_val = __gmp_default_rounding_mode; }  \
  else { rnd_mode_val = r_get_rounding_mode (rnd_mode); }    \
  if (NIL_P (prec)) { prec_val = mpfr_get_default_prec(); }  \
  else if (FIXNUM_P (prec)) { prec_val = FIX2INT (prec); }   \
  else { typeerror_as (X, "precision"); }                    \
  mpf_make_struct_init (res, res_val, prec_val);             \
  mpfr_##name (res_val, rnd_mode_val);                       \
                                                             \
  return res;                                                \
}

/*
 * Document-method: frexp
 * call-seq:
 *   exp, y = x.frexp(rnd_mode = nil, prec = nil)
 *
 * Set _exp_ and _y_ such that 0.5 <= _abs(y)_ < 1 and _y_ times 2 raised to
 * _exp_ equals _x_ rounded to _prec_, or the precision of _x_, using the given
 * rounding mode. If _x_ is zero, then _y_ is set to a zero of the same sign
 * and _exp_ is set to 0. If _x_ is `NaN` or an infinity, then _y_ is set to
 * the same value and _exp_ is undefined.
 *
 * @since 0.6.47
 */
VALUE r_gmpfr_frexp(int argc, VALUE *argv, VALUE self_val)
{
  MP_FLOAT *self, *res;
  VALUE rnd_mode_val, res_prec_val, exp_val, res_val;
  mpfr_prec_t prec, res_prec;
  mp_rnd_t rnd_mode;
  mpfr_exp_t exp;

  mpf_get_struct_prec (self_val, self, prec);

  rb_scan_args (argc, argv, "02", &rnd_mode_val, &res_prec_val);
  if (NIL_P (rnd_mode_val)) { rnd_mode = __gmp_default_rounding_mode; }
  else { rnd_mode = r_get_rounding_mode (rnd_mode_val); }
  if (NIL_P (res_prec_val)) { res_prec = prec; }
  else { res_prec = FIX2INT (res_prec_val); }
  mpf_make_struct_init (res_val, res, res_prec);
  mpfr_frexp (&exp, res, self, rnd_mode);
  exp_val = INT2FIX (exp);

  return rb_assoc_new(exp_val, res_val);
}

/*
 * Document-method: sqrt
 * call-seq:
 *   x.sqrt
 *   x.sqrt(rounding_mode)
 *   x.sqrt(rounding_mode, precision)
 *
 * Calculate the square root of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(sqrt)

/*
 * Document-method: rec_sqrt
 * call-seq:
 *   x.rec_sqrt
 *   x.rec_sqrt(rounding_mode)
 *   x.rec_sqrt(rounding_mode, precision)
 *
 * Calculate the reciprocal square root of _x_, rounding according to
 * `rounding_mode`. The resultant GMP::F float has the same precision that _x_
 * has, if `precision` was not passed in.
 */
MPFR_SINGLE_FUNCTION(rec_sqrt)

/*
 * Document-method: cbrt
 * call-seq:
 *   x.cbrt
 *   x.cbrt(rounding_mode)
 *   x.cbrt(rounding_mode, precision)
 *
 * Calculate the cubic root of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(cbrt)

/*
 * Document-method: log
 * call-seq:
 *   x.log
 *   x.log(rounding_mode)
 *   x.log(rounding_mode, precision)
 *
 * Calculate the natural logarithm of _x_, rounding according to
 * `rounding_mode`. The resultant GMP::F float has the same precision that _x_
 * has, if `precision` was not passed in.
 */
MPFR_SINGLE_FUNCTION(log)

/*
 * Document-method: log2
 * call-seq:
 *   x.log2
 *   x.log2(rounding_mode)
 *   x.log2(rounding_mode, precision)
 *
 * Calculate the logarithm base 2 of _x_, rounding according to
 * `rounding_mode`. The resultant GMP::F float has the same precision that _x_
 * has, if `precision` was not passed in.
 */
MPFR_SINGLE_FUNCTION(log2)

/*
 * Document-method: log10
 * call-seq:
 *   x.log10
 *   x.log10(rounding_mode)
 *   x.log10(rounding_mode, precision)
 *
 * Calculate the logarithm base 10 of _x_, rounding according to
 * `rounding_mode`. The resultant GMP::F float has the same precision that _x_
 * has, if `precision` was not passed in.
 */
MPFR_SINGLE_FUNCTION(log10)

/*
 * Document-method: exp
 * call-seq:
 *   x.exp
 *   x.exp(rounding_mode)
 *   x.exp(rounding_mode, precision)
 *
 * Calculate the exponential of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(exp)

/*
 * Document-method: exp2
 * call-seq:
 *   x.exp2
 *   x.exp2(rounding_mode)
 *   x.exp2(rounding_mode, precision)
 *
 * Calculate the 2 power of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(exp2)

/*
 * Document-method: exp10
 * call-seq:
 *   x.exp10
 *   x.exp10(rounding_mode)
 *   x.exp10(rounding_mode, precision)
 *
 * Calculate the 10 power of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(exp10)

/*
 * Document-method: cos
 * call-seq:
 *   x.cos
 *   x.cos(rounding_mode)
 *   x.cos(rounding_mode, precision)
 *
 * Calculate the cosine of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(cos)

/*
 * Document-method: sin
 * call-seq:
 *   x.sin
 *   x.sin(rounding_mode)
 *   x.sin(rounding_mode, precision)
 *
 * Calculate the sine of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(sin)

/*
 * Document-method: tan
 * call-seq:
 *   x.tan
 *   x.tan(rounding_mode)
 *   x.tan(rounding_mode, precision)
 *
 * Calculate the tangent of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(tan)

/*
 * Document-method: sin_cos
 * call-seq:
 *   x.sin_cos
 *   x.sin_cos(rounding_mode)
 *   x.sin_cos(rounding_mode, precision)
 *
 * Simultaneously calculate the sine and cosine of _x_, rounding according to
 * `rounding_mode`, returning both numbers. The resultant GMP::F floats have
 * the same precision that _x_ has, if `precision` was not passed in.
 */
MPFR_DOUBLE_FUNCTION(sin_cos)

/*
 * Document-method: secant
 * call-seq:
 *   x.secant
 *   x.secant(rounding_mode)
 *   x.secant(rounding_mode, precision)
 *
 * Calculate the secant of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(sec)

/*
 * Document-method: csc
 * call-seq:
 *   x.csc
 *   x.csc(rounding_mode)
 *   x.csc(rounding_mode, precision)
 *
 * Calculate the cosecant of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(csc)

/*
 * Document-method: cot
 * call-seq:
 *   x.cot
 *   x.cot(rounding_mode)
 *   x.cot(rounding_mode, precision)
 *
 * Calculate the cotangent of _x_, rounding according to `rounding_mode`. The
 * resultant GMP::F float has the same precision that _x_ has, if `precision`
 * was not passed in.
 */
MPFR_SINGLE_FUNCTION(cot)
MPFR_SINGLE_FUNCTION(acos)
MPFR_SINGLE_FUNCTION(asin)
MPFR_SINGLE_FUNCTION(atan)
MPFR_SINGLE_MPF_FUNCTION(atan2)
MPFR_SINGLE_FUNCTION(cosh)
MPFR_SINGLE_FUNCTION(sinh)
MPFR_SINGLE_FUNCTION(tanh)
MPFR_DOUBLE_FUNCTION(sinh_cosh)
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
MPFR_SINGLE_LONG_FUNCTION(jn)
MPFR_SINGLE_FUNCTION(y0)
MPFR_SINGLE_FUNCTION(y1)
MPFR_SINGLE_LONG_FUNCTION(yn)

MPFR_SINGLE_MPF_FUNCTION(agm)
MPFR_SINGLE_MPF_FUNCTION(hypot)

/*
 * call-seq:
 *   GMP::F.fac(n)
 *   GMP::F.fac(n, rounding_mode)
 *   GMP::F.fac(n, rounding_mode, precision)
 *
 * Creates a new GMP::F float, equal to the factorial of n, which must be a
 * Fixnum. Optionally pass a rounding mode, and precision for the resultant
 * GMP::F.
 *
 * @since 0.6.47
 */
VALUE r_gmpfrsg_fac(int argc, VALUE *argv, VALUE self_val)
{
  MP_FLOAT *res;
  VALUE arg_val, res_val;
  VALUE rnd_mode_val, prec_val;
  mp_rnd_t rnd_mode;
  mpfr_prec_t prec;
  unsigned long int arg = 0;
  (void)self_val;

  rb_scan_args (argc, argv, "12", &arg_val, &rnd_mode_val, &prec_val);

  if (FIXNUM_P (arg_val)) { arg = FIX2INT (arg_val); }
  else { rb_raise (rb_eTypeError, "operand must be a Fixnum"); }
  if (NIL_P (rnd_mode_val)) { rnd_mode = __gmp_default_rounding_mode; }
  else { rnd_mode = r_get_rounding_mode (rnd_mode_val); }
  if (NIL_P (prec_val)) { prec = mpfr_get_default_prec(); }
  /* TODO check type */
  else { prec = FIX2INT (prec_val); }
  mpf_make_struct_init (res_val, res, prec);
  mpfr_fac_ui (res, arg, rnd_mode);

  return res_val;
}

//VALUE r_gmpfrsg_sprintf(int argc, VALUE *argv, VALUE self)
  //rb_scan_args (argc, argv, "1*", &format, &list);
VALUE r_gmpfrsg_sprintf2(VALUE klass, VALUE format, VALUE arg) {
  VALUE res;
  char *buffer;
  char *format_str;
  MP_INT *arg_val_z;
  MP_FLOAT *arg_val_f;
  (void)klass;
  format_str = StringValuePtr (format);
  if (GMPZ_P (arg)) {
    mpz_get_struct (arg, arg_val_z);
    mpfr_asprintf (&buffer, format_str, arg_val_z);
  } else if (GMPF_P (arg)) {
    mpf_get_struct (arg, arg_val_f);
    mpfr_asprintf (&buffer, format_str, arg_val_f);
  }

  res = rb_str_new2 (buffer);
  free (buffer);
  return res;
}

MPFR_CONST_FUNCTION(const_log2)
MPFR_CONST_FUNCTION(const_pi)
MPFR_CONST_FUNCTION(const_euler)
MPFR_CONST_FUNCTION(const_catalan)

MPFR_SINGLE_BOOLEAN_FUNCTION(integer_p)

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

static VALUE r_gmpfr_pow(VALUE self_val, VALUE arg_val)
{
  MP_FLOAT *self, *res, *arg_f;
  MP_RAT *arg_q;
  MP_INT *arg_z;
  mpfr_prec_t prec;
  VALUE res_val;

  mpf_get_struct_prec (self_val, self, prec);

  if (GMPF_P (arg_val)) {
    mpf_get_struct (arg_val, arg_f);
    prec_max (prec, arg_f);
    mpf_make_struct_init (res_val, res, prec);
    mpfr_pow (res, self, arg_f, __gmp_default_rounding_mode);
  } else {
    mpf_make_struct_init (res_val, res, prec);

    if (GMPZ_P (arg_val)) {
      mpz_get_struct (arg_val, arg_z);
      mpfr_pow_z (res, self, arg_z, __gmp_default_rounding_mode);
    } else if (GMPQ_P (arg_val)) {
      mpq_get_struct (arg_val, arg_q);
      mpf_set_q (res, arg_q);
      mpfr_pow (res, self, res, __gmp_default_rounding_mode);
    } else if (FLOAT_P (arg_val)) {
      mpf_set_d (res, NUM2DBL (arg_val));
      mpfr_pow (res, self, res, __gmp_default_rounding_mode);
    } else if (FIXNUM_P (arg_val)) {
      if (FIX2NUM (arg_val) >= 0)
        mpfr_pow_ui (res, self, FIX2NUM (arg_val), __gmp_default_rounding_mode);
      else
        mpfr_pow_si (res, self, FIX2NUM (arg_val), __gmp_default_rounding_mode);
    } else if (BIGNUM_P (arg_val)) {
      mpz_temp_from_bignum (arg_z, arg_val);
      mpf_set_z (res, arg_z);
      mpz_temp_free (arg_z);
      mpfr_pow (res, self, res, __gmp_default_rounding_mode);
    } else {
      typeerror (ZQFXBD);
    }
  }

  return res_val;
}

/**********************************************************************
 *    Rounding Related Functions                                      *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::F.default_rounding_mode
 *
 * Get the default rounding mode.
 */
VALUE r_gmpfsg_get_default_rounding_mode(VALUE klass)
{
  const char *rounding_string_val;
  (void)klass;
  rounding_string_val = mpfr_print_rnd_mode (mpfr_get_default_rounding_mode ());
  if ( rounding_string_val == NULL ) {
    return Qnil;
  }
  else {
    return rb_const_get (mGMP, rb_intern (rounding_string_val));
  }
}

/*
 * call-seq:
 *   GMP::F.default_rounding_mode=(rnd)
 *
 * Set the default rounding mode to _rnd_. The default rounding mode is to
 * nearest initially.
 */
VALUE r_gmpfsg_set_default_rounding_mode(VALUE klass, VALUE arg)
{
  VALUE mode = 0;
  (void)klass;

  if (GMPRND_P (arg)) {
    mode = rb_funcall (arg, rb_intern("mode"), 0);
    if (FIX2INT (mode) < 0 || FIX2INT (mode) > 3) {
      rb_raise (rb_eRangeError, "rounding mode must be one of the rounding mode constants.");
    }
  } else {
    rb_raise (rb_eTypeError, "rounding mode must be one of the rounding mode constants.");
  }

  switch (FIX2INT(mode)) {
    case 0:
      mpfr_set_default_rounding_mode (GMP_RNDN); break;
    case 1:
      mpfr_set_default_rounding_mode (GMP_RNDZ); break;
    case 2:
      mpfr_set_default_rounding_mode (GMP_RNDU); break;
    case 3:
      mpfr_set_default_rounding_mode (GMP_RNDD); break;
#if MPFR_VERSION_MAJOR>2
    case 4:
      mpfr_set_default_rounding_mode (MPFR_RNDA); break;
#endif
  }

  return Qnil;
}

VALUE r_gmpf_can_round(VALUE self, VALUE err, VALUE rnd1, VALUE rnd2, VALUE prec)
{
  MP_FLOAT *self_val;
  mp_exp_t err_val = 0;
  mpfr_rnd_t rnd1_val, rnd2_val;
  mpfr_prec_t prec_val;

  mpf_get_struct (self, self_val);
  if (FIXNUM_P (err)) { err_val = FIX2INT (err); }
  else { typeerror_as (X, "err"); }
  rnd1_val = r_get_rounding_mode (rnd1);
  rnd2_val = r_get_rounding_mode (rnd2);
  prec_val = FIX2INT (prec);

  if (mpfr_can_round (self_val, err_val, rnd1_val, rnd2_val, prec_val))
    return Qtrue;
  else
    return Qfalse;
}


/**********************************************************************
 *    _unsorted_                                                      *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::F.mpfr_buildopt_tls_p
 *
 * Return a non-zero value if MPFR was compiled as thread safe using
 * compiler-level Thread Local Storage (that is, MPFR was built with the
 * --enable-thread-safe configure option, see INSTALL file), return zero
 *  otherwise.
 */
VALUE r_gmpfsg_mpfr_buildopt_tls_p(VALUE klass)
{
  (void)klass;
  return INT2FIX (mpfr_buildopt_tls_p());
}

/*
 * call-seq:
 *   GMP::F.mpfr_buildopt_decimal_p
 *
 * Return a non-zero value if MPFR was compiled with decimal float support
 * (that is, MPFR was built with the --enable-decimal-float configure option),
 * return zero otherwise.
 */
VALUE r_gmpfsg_mpfr_buildopt_decimal_p(VALUE klass)
{
  (void)klass;
  return INT2FIX (mpfr_buildopt_decimal_p());
}

#endif  /* MPFR */


/**********************************************************************
 *    _unsorted_                                                      *
 **********************************************************************/

/*
 * Document-method: prec
 * call-seq:
 *   x.prec
 *
 * Return the precision of _x_, i.e. the number of bits used to store its
 * significand.
 */
VALUE r_gmpf_get_prec(VALUE self)
{
  MP_FLOAT *self_val;
  mpf_get_struct (self, self_val);
  return INT2NUM (mpf_get_prec (self_val));
}

/*
 * Document-method: prec=
 * call-seq:
 *   x.prec=(p)
 *
 * Reset the precision of _x_ to be exactly _p_ bits, and set its value to
 * NaN. The previous value stored in _x_ is lost.
 * The precision prec can be any integer between
 * `MPFR_PREC_MIN` and `MPFR_PREC_MAX`.
 */
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

  return Qnil;  /* should never get here */
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

  return Qnil;  /* should never get here */
}

#ifdef MPFR

#define DEFUN_FR_CLASS_M(fname,mpfr_fname)      \
static VALUE r_gmpfrsg_get_##fname(VALUE klass) \
{                                               \
  (void)klass;                                  \
  return INT2NUM (mpfr_fname ());               \
}

/*
 * Document-method: GMP::F.emin
 * call-seq:
 *   GMP::F.emin
 *
 * Return the (current) smallest exponent allowed for a
 * floating-point variable. The smallest positive value of a floating-point
 * variable is one half times 2 raised to the smallest exponent.
 *
 * @since 0.7.19
 */
DEFUN_FR_CLASS_M(emin,mpfr_get_emin)

/*
 * Document-method: GMP::F.emax
 * call-seq:
 *   GMP::F.emax
 *
 * Return the (current) largest exponent allowed for a floating-point variable.
 * The largest floating-point value has the form (1 - epsilon) times 2 raised
 * to the largest exponent, where epsilon depends on the precision of the
 * considered variable.
 *
 * @since 0.7.19
 */
DEFUN_FR_CLASS_M(emax,mpfr_get_emax)

/*
 * Document-method: GMP::F.emin_min
 * call-seq:
 *   GMP::F.emin_min
 *
 * Return the minimum exponent allowed for GMP::F.emin=()
 *
 * @since 0.7.19
 */
DEFUN_FR_CLASS_M(emin_min,mpfr_get_emin_min)

/*
 * Document-method: GMP::F.emin_max
 * call-seq:
 *   GMP::F.emin_max
 *
 * Return the maximum exponent allowed for GMP::F.emin=()
 *
 * @since 0.7.19
 */
DEFUN_FR_CLASS_M(emin_max,mpfr_get_emin_max)

/*
 * Document-method: GMP::F.emax_min
 * call-seq:
 *   GMP::F.emax_min
 *
 * Return the minimum exponent allowed for GMP::F.emax=()
 *
 * @since 0.7.19
 */
DEFUN_FR_CLASS_M(emax_min,mpfr_get_emax_min)

/*
 * Document-method: GMP::F.emax_max
 * call-seq:
 *   GMP::F.emax_max
 *
 * Return the maximum exponent allowed for GMP::F.emax=()
 *
 * @since 0.7.19
 */
DEFUN_FR_CLASS_M(emax_max,mpfr_get_emax_max)

/*
 * Document-method: GMP::F.emin=
 * call-seq:
 *   GMP::F.emin=(exp)
 *
 * Set the smallest exponent allowed for a floating-point variable.
 *
 * @since 0.7.19
 */
VALUE r_gmpfrsg_set_emin(VALUE klass, VALUE arg_val)
{
  (void)klass;

  if (! FIXNUM_P (arg_val))
    typeerror_as (X, "exp");

  mpfr_set_emin (FIX2NUM (arg_val));
  /* TODO: figure out a way to generate this RangeError:
    if (success != 0)
      rb_raise(rb_eRangeError, "exp must be in-range");*/

  return Qnil;
}

/*
 * Document-method: GMP::F.emax=
 * call-seq:
 *   GMP::F.emax=(exp)
 *
 * Set the largest exponent allowed for a floating-point variable.
 *
 * @since 0.7.19
 */
VALUE r_gmpfrsg_set_emax(VALUE klass, VALUE arg_val)
{
  (void)klass;

  if (! FIXNUM_P (arg_val))
    typeerror_as (X, "exp");

  mpfr_set_emax (FIX2NUM (arg_val));
  /* TODO: figure out a way to generate this RangeError:
    if (success != 0)
      rb_raise(rb_eRangeError, "exp must be in-range");*/

  return Qnil;
}

#endif  /* MPFR */


void init_gmpf()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "F", r_gmpmod_f, -1);

  cGMP_F = rb_define_class_under (mGMP, "F", rb_cNumeric);


  // Initializing, Assigning Floats
  rb_define_singleton_method(cGMP_F, "new", r_gmpfsg_new, -1);
  rb_define_method(cGMP_F, "initialize", r_gmpf_initialize, -1);
#ifdef MPFR
  rb_define_singleton_method(cGMP_F, "new_2exp", r_gmpfsg_new_2exp, -1);
  rb_define_singleton_method(cGMP_F, "nan", r_gmpfsg_nan, 0);
  rb_define_singleton_method(cGMP_F, "inf", r_gmpfsg_inf, -1);
#if MPFR_VERSION_MAJOR>2
  rb_define_singleton_method(cGMP_F, "zero", r_gmpfsg_zero, -1);
#endif  /* MPFR_VERSION_MAJOR>2 */
#endif  /* MPFR */
  rb_define_method(cGMP_F, "prec", r_gmpf_get_prec, 0);
  rb_define_method(cGMP_F, "prec=", r_gmpf_set_prec, 1);
  rb_define_method(cGMP_F, "prec_raw=", r_gmpf_set_prec_raw, 1);

#ifdef MPFR
  rb_define_singleton_method(cGMP_F, "emin", r_gmpfrsg_get_emin, 0);
  rb_define_singleton_method(cGMP_F, "emax", r_gmpfrsg_get_emax, 0);
  rb_define_singleton_method(cGMP_F, "emin=", r_gmpfrsg_set_emin, 1);
  rb_define_singleton_method(cGMP_F, "emax=", r_gmpfrsg_set_emax, 1);
  rb_define_singleton_method(cGMP_F, "emin_min", r_gmpfrsg_get_emin_min, 0);
  rb_define_singleton_method(cGMP_F, "emin_max", r_gmpfrsg_get_emin_max, 0);
  rb_define_singleton_method(cGMP_F, "emax_min", r_gmpfrsg_get_emax_min, 0);
  rb_define_singleton_method(cGMP_F, "emax_max", r_gmpfrsg_get_emax_max, 0);
#endif  /* MPFR */

  // Converting Floats
  rb_define_method(cGMP_F, "to_s", r_gmpf_to_s, -1);
  rb_define_alias(cGMP_F, "inspect", "to_s");
  rb_define_method(cGMP_F, "to_d",  r_gmpf_to_d, 0);
  rb_define_alias(cGMP_F, "to_f", "to_d");

  // Float Arithmetic
  rb_define_method(cGMP_F, "-", r_gmpf_sub, 1);
  rb_define_method(cGMP_F, "/", r_gmpf_div, 1);
  rb_define_alias(cGMP_F, "divide", "/");
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
  rb_define_method(cGMP_F, "floor!",  r_gmpf_floor_self, 0);
  rb_define_method(cGMP_F, "trunc",  r_gmpf_trunc, 0);
  rb_define_method(cGMP_F, "trunc!",  r_gmpf_trunc_self, 0);


#ifdef MPFR
  /* TODO: new in MPFR 3.0.0:
   *
   * mpfr_ai
   */

  /* TODO: new in MPFR 3.1.0:
   *
   * mpfr_buildopt_gmpinternals_p
   * mpfr_buildopt_tune_case
   * mpfr_frexp
   * mpfr_grandom
   * mpfr_z_sub
   */
#if MPFR_VERSION_MAJOR >= 3 && MPFR_VERSION_MINOR >= 1
  rb_define_method(cGMP_F, "frexp",    r_gmpfr_frexp,    -1);
#endif  /* MPFR_VERSION >= 3.1 */

  // Basic Arithmetic Functions
  rb_define_method(cGMP_F, "sqrt",     r_gmpfr_sqrt,     -1);
  rb_define_method(cGMP_F, "rec_sqrt", r_gmpfr_rec_sqrt, -1);
  rb_define_method(cGMP_F, "cbrt",     r_gmpfr_cbrt,     -1);
  // TODO "root", r_gmpfr_root
  // TODO "dim", r_gmpfr_dim
  // TODO "mul_2", r_gmpfr_mul_2
  // TODO "div_2", r_gmpfr_div_2

  // Comparison Functions
  rb_define_method(cGMP_F, "nan?", r_gmpfr_nan_p, 0);
  rb_define_method(cGMP_F, "infinite?", r_gmpfr_inf_p, 0);
  rb_define_method(cGMP_F, "finite?", r_gmpfr_fin_p, 0);
  rb_define_method(cGMP_F, "number?", r_gmpfr_number_p, 0);
  rb_define_method(cGMP_F, "zero?", r_gmpfr_zero_p, 0);
#if MPFR_VERSION_MAJOR > 2
  rb_define_method(cGMP_F, "regular?", r_gmpfr_regular_p, 0);
#endif
  rb_define_method(cGMP_F, "lessgreater?", r_gmpfr_lessgreater_p, 1);
  rb_define_method(cGMP_F, "unordered?",   r_gmpfr_unordered_p, 1);

  // Special Functions
  rb_define_method(cGMP_F, "log",       r_gmpfr_log,     -1);
  rb_define_method(cGMP_F, "log2",      r_gmpfr_log2,    -1);
  rb_define_method(cGMP_F, "log10",     r_gmpfr_log10,   -1);
  rb_define_method(cGMP_F, "exp",       r_gmpfr_exp,     -1);
  rb_define_method(cGMP_F, "exp2",      r_gmpfr_exp2,    -1);
  rb_define_method(cGMP_F, "exp10",     r_gmpfr_exp10,   -1);
  rb_define_method(cGMP_F, "cos",       r_gmpfr_cos,     -1);
  rb_define_method(cGMP_F, "sin",       r_gmpfr_sin,     -1);
  rb_define_method(cGMP_F, "tan",       r_gmpfr_tan,     -1);
  rb_define_method(cGMP_F, "sin_cos",   r_gmpfr_sin_cos, -1);
  rb_define_method(cGMP_F, "sec",       r_gmpfr_sec,     -1);
  rb_define_method(cGMP_F, "csc",       r_gmpfr_csc,     -1);
  rb_define_method(cGMP_F, "cot",       r_gmpfr_cot,     -1);
  rb_define_method(cGMP_F, "acos",      r_gmpfr_acos,    -1);
  rb_define_method(cGMP_F, "asin",      r_gmpfr_asin,    -1);
  rb_define_method(cGMP_F, "atan",      r_gmpfr_atan,    -1);
  rb_define_method(cGMP_F, "atan2",     r_gmpfr_atan2,   -1);
  rb_define_method(cGMP_F, "cosh",      r_gmpfr_cosh,      -1);
  rb_define_method(cGMP_F, "sinh",      r_gmpfr_sinh,      -1);
  rb_define_method(cGMP_F, "tanh",      r_gmpfr_tanh,      -1);
  rb_define_method(cGMP_F, "sinh_cosh", r_gmpfr_sinh_cosh, -1);
  rb_define_method(cGMP_F, "sech",      r_gmpfr_sech,      -1);
  rb_define_method(cGMP_F, "csch",      r_gmpfr_csch,      -1);
  rb_define_method(cGMP_F, "coth",      r_gmpfr_coth,      -1);
  rb_define_method(cGMP_F, "acosh",     r_gmpfr_acosh,     -1);
  rb_define_method(cGMP_F, "asinh",     r_gmpfr_asinh,     -1);
  rb_define_method(cGMP_F, "atanh",     r_gmpfr_atanh,     -1);

  rb_define_singleton_method(cGMP_F, "fac", r_gmpfrsg_fac, -1);

  rb_define_method(cGMP_F, "log1p",   r_gmpfr_log1p,   -1);
  rb_define_method(cGMP_F, "expm1",   r_gmpfr_expm1,   -1);
  rb_define_method(cGMP_F, "eint",    r_gmpfr_eint,    -1);
  rb_define_method(cGMP_F, "li2",     r_gmpfr_li2,     -1);
  rb_define_method(cGMP_F, "gamma",   r_gmpfr_gamma,   -1);
  rb_define_method(cGMP_F, "lngamma", r_gmpfr_lngamma, -1);
  /* TODO rb_define_method(cGMP_F, "lgamma",  r_gmpfr_lgamma,   -1); */
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

  /* TODO "fma", r_gmpfr_fma */
  /* TODO "fms", r_gmpfr_fms */
  rb_define_method(cGMP_F, "agm",   r_gmpfr_agm,       -1);
  rb_define_method(cGMP_F, "hypot", r_gmpfr_hypot,     -1);
  /* TODO "ai", r_gmpfr_ai  !! 3.0.0 */

  // Formatted Output Functions
  rb_define_singleton_method(cGMP_F, "sprintf2", r_gmpfrsg_sprintf2, 2);

  rb_define_singleton_method(cGMP_F, "const_log2",    r_gmpfrsg_const_log2,    -1);
  rb_define_singleton_method(cGMP_F, "const_pi",      r_gmpfrsg_const_pi,      -1);
  rb_define_singleton_method(cGMP_F, "const_euler",   r_gmpfrsg_const_euler,   -1);
  rb_define_singleton_method(cGMP_F, "const_catalan", r_gmpfrsg_const_catalan, -1);

  // Integer and Remainder Related Functions
  rb_define_method(cGMP_F, "integer?", r_gmpfr_integer_p, 0);

  // Rounding Related Functions
  rb_define_singleton_method (cGMP_F, "default_rounding_mode", r_gmpfsg_get_default_rounding_mode, 0);
  rb_define_singleton_method (cGMP_F, "default_rounding_mode=", r_gmpfsg_set_default_rounding_mode, 1);
  rb_define_method(cGMP_F, "can_round?",  r_gmpf_can_round, 4);

  // Miscellaneous Functions
#if MPFR_VERSION_MAJOR > 2
  rb_define_singleton_method (cGMP_F, "mpfr_buildopt_decimal_p", r_gmpfsg_mpfr_buildopt_decimal_p, 0);
  rb_define_singleton_method (cGMP_F, "mpfr_buildopt_tls_p", r_gmpfsg_mpfr_buildopt_tls_p, 0);
#endif /* MPFR > 2 */
#endif /* MPFR */
}
