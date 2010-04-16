/*
 * gmp.c
 *
 * This file contains everything you would expect from a C extension.
 */

#include <ruby_gmp.h>

VALUE mGMP, cGMP_Z, cGMP_Q, cGMP_F, cGMP_RandState;
#ifdef MPFR
VALUE cGMP_Rnd;
#endif

void r_gmpz_free(void *ptr)         { mpz_clear (ptr);     free (ptr); }
void r_gmpq_free(void *ptr)         { mpq_clear (ptr);     free (ptr); }
void r_gmpf_free(void *ptr)         { mpf_clear (ptr);     free (ptr); }
void r_gmprandstate_free(void *ptr) { gmp_randclear (ptr); free (ptr); }

static void mpq_str_set(MP_RAT *ROP, char *str)
{
  int i=0;

  while (str[i] && str[i] != '/')
    i++;

  if (str[i])
  {
    str[i] = 0; /* You didn't see that :) */
    mpz_set_str (mpq_numref(ROP), str, 0);
    str[i] = '/';
    mpz_set_str (mpq_denref(ROP), str+i+1, 0);
  } else {
    mpz_set_str (mpq_numref(ROP), str, 0);
    mpz_set_ui (mpq_denref(ROP), 1);
  }
  mpq_canonicalize (ROP);
}

static VALUE r_gmpq_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_RAT *self_val, *arg_val;

  if (argc != 0) {
    mpq_get_struct(self, self_val);
    if (argc == 1 && GMPQ_P(argv[0])) {
      mpq_get_struct(argv[0], arg_val);
      mpq_set (self_val, arg_val);
    } else if (argc == 1 && STRING_P(argv[0])) {
      mpq_str_set (self_val, STR2CSTR(argv[0]));
    } else {
      mpz_set_value (mpq_numref(self_val), argv[0]);
      if (argc == 2) {
        mpz_set_value (mpq_denref(self_val), argv[1]);
        mpq_canonicalize(self_val);
      }
    }
  }
  return Qnil;
}

static VALUE r_gmpz_coerce(VALUE self, VALUE arg)
{
  return rb_assoc_new(r_gmpzsg_new(1, &arg, cGMP_Z), self);
}

static VALUE r_gmpq_coerce(VALUE self, VALUE arg)
{
  return rb_assoc_new(r_gmpqsg_new(1, &arg, cGMP_Q), self);
}

static VALUE r_gmpf_coerce(VALUE self, VALUE arg)
{
  return rb_assoc_new(r_gmpfsg_new(1, &arg, cGMP_F), self);
}

static VALUE r_gmpfsg_get_default_prec(VALUE klass)
{
  (void)klass;
  return INT2NUM(mpf_get_default_prec());
}

static VALUE r_gmpfsg_set_default_prec(VALUE klass, VALUE arg)
{
  (void)klass;
  if (FIXNUM_P(arg)) {
    if (FIX2NUM(arg) <= 0) {
      rb_raise(rb_eRangeError, "prec must be positive");
    }
    mpf_set_default_prec (FIX2NUM(arg));
  } else {
    rb_raise(rb_eTypeError, "prec must be FixNum");
  }
  return Qnil;
}

#ifdef MPFR
static VALUE r_gmpfsg_get_default_rounding_mode(VALUE klass)
{
  (void)klass;
  const char *rounding_string_val;
  rounding_string_val = mpfr_print_rnd_mode (mpfr_get_default_rounding_mode ());
  if ( rounding_string_val == NULL ) {
    return Qnil;
  }
  else {
    return rb_const_get (mGMP, rb_intern (rounding_string_val));
  }
}

static VALUE r_gmpfsg_set_default_rounding_mode(VALUE klass, VALUE arg)
{
  (void)klass;
  VALUE mode;
  if (GMPRND_P(arg)) {
    mode = rb_funcall (arg, rb_intern("mode"), 0);
    if (FIX2INT(mode) < 0 || FIX2INT(mode) > 3) {
      rb_raise(rb_eRangeError, "rounding mode must be one of the rounding mode constants.");
    }
  } else {
    rb_raise(rb_eTypeError, "rounding mode must be one of the rounding mode constants.");
  }
  
  switch (FIX2INT(mode)) {
    case 0:
      mpfr_set_default_rounding_mode (GMP_RNDN);
      break;
    case 1:
      mpfr_set_default_rounding_mode (GMP_RNDZ);
      break;
    case 2:
      mpfr_set_default_rounding_mode (GMP_RNDU);
      break;
    case 3:
      mpfr_set_default_rounding_mode (GMP_RNDD);
      break;
  }
  
  return Qnil;
}

mp_rnd_t r_get_rounding_mode(VALUE rnd)
{
  VALUE mode;
  
  if (GMPRND_P(rnd)) {
    mode = rb_funcall (rnd, rb_intern("mode"), 0);
    if (FIX2INT(mode) < 0 || FIX2INT(mode) > 3) {
      rb_raise(rb_eRangeError, "rounding mode must be one of the rounding mode constants.");
    }
  } else {
    rb_raise(rb_eTypeError, "rounding mode must be one of the rounding mode constants.");
  }
  
  switch (FIX2INT(mode)) {
    case 0:
      return GMP_RNDN;
    case 1:
      return GMP_RNDZ;
    case 2:
      return GMP_RNDU;
    case 3:
      return GMP_RNDD;
    default:
      return GMP_RNDN;
  }
}

#endif /* MPFR */

#include "gmpf.h"
#include "gmpq.h"
/* #include "gmpz.h" */
#include "takeover.h"

#define REGISTER_TAKEOVER(fname, ruby_fname, old_fname) \
  rb_define_alias(rb_cFixnum, old_fname, ruby_fname); \
  rb_define_method(rb_cFixnum, ruby_fname, takeover_fixnum_##fname, -1); \
  rb_define_alias(rb_cBignum, old_fname, ruby_fname); \
  rb_define_method(rb_cBignum, ruby_fname, takeover_bignum_##fname, -1);

void Init_gmp() {
  mGMP = rb_define_module("GMP");
  rb_define_const(mGMP, "GMP_VERSION", rb_str_new2(gmp_version));
  rb_define_const(mGMP, "GMP_CC", rb_str_new2(__GMP_CC));
  rb_define_const(mGMP, "GMP_CFLAGS", rb_str_new2(__GMP_CFLAGS));
  rb_define_const(mGMP, "GMP_BITS_PER_LIMB", INT2FIX(mp_bits_per_limb));
#ifdef MPFR
  rb_define_const(mGMP, "MPFR_VERSION", rb_str_new2(MPFR_VERSION_STRING));
  rb_define_const(mGMP, "MPFR_PREC_MIN", INT2FIX(MPFR_PREC_MIN));
  rb_define_const(mGMP, "MPFR_PREC_MAX", INT2FIX(MPFR_PREC_MAX));
#endif /* MPFR */

  cGMP_Z = rb_define_class_under(mGMP, "Z", rb_cInteger);
  init_gmpz ();
  rb_define_method(cGMP_Z, "coerce", r_gmpz_coerce, 1);

  cGMP_Q = rb_define_class_under (mGMP, "Q", rb_cNumeric);
  init_gmpq ();
  rb_define_method(cGMP_Q, "initialize", r_gmpq_initialize, -1);
  rb_define_method(cGMP_Q, "coerce", r_gmpq_coerce, 1);
  rb_define_method(cGMP_Q, "num", r_gmpq_num, 0);
  rb_define_method(cGMP_Q, "den", r_gmpq_den, 0);

  cGMP_F = rb_define_class_under (mGMP, "F", rb_cNumeric);
  init_gmpf ();
  rb_define_singleton_method (cGMP_F, "default_prec", r_gmpfsg_get_default_prec, 0);
  rb_define_singleton_method (cGMP_F, "default_prec=", r_gmpfsg_set_default_prec, 1);
#ifdef MPFR
  cGMP_Rnd = rb_define_class_under (mGMP, "Rnd", rb_cObject);
  rb_define_singleton_method (cGMP_F, "default_rounding_mode", r_gmpfsg_get_default_rounding_mode, 0);
  rb_define_singleton_method (cGMP_F, "default_rounding_mode=", r_gmpfsg_set_default_rounding_mode, 1);
  init_gmprnd ();
#endif /* MPFR */
  rb_define_method (cGMP_F, "coerce", r_gmpf_coerce, 1); // new method - testing

/*  rb_define_method(cGMP_F, "cmpabs",  r_gmpf_cmpabs, 1);*/
  
  cGMP_RandState = rb_define_class_under (mGMP, "RandState", rb_cObject);
  init_gmprandstate ();
  
  init_gmpbench_timing ();

  // more

  REGISTER_TAKEOVER(and, "&", "old_and")
  REGISTER_TAKEOVER(or, "|", "old_or")
  REGISTER_TAKEOVER(xor, "^", "old_xor")
  /* takeover cmpabs ? */
}
