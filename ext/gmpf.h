#ifndef _GMPF_H_
#define _GMPF_H_

/*
 * gmpf.h
 *
 * This file contains GMP::F stuff.
 */

#include <ruby_gmp.h>

#ifdef MPFR
#define MPFR_SINGLE_FUNCTION(name)                                \
static VALUE r_gmpfr_##name(VALUE self)                           \
{                                                                 \
  MP_FLOAT *self_val, *res_val;                                   \
  unsigned long prec;                                             \
  VALUE res;                                                      \
                                                                  \
  mpf_get_struct_prec(self, self_val, prec);                      \
  mpf_make_struct_init(res, res_val, prec);                       \
  mpfr_##name(res_val, self_val, __gmp_default_rounding_mode);    \
                                                                  \
  return res;                                                     \
}

MPFR_SINGLE_FUNCTION(log)
MPFR_SINGLE_FUNCTION(exp)
MPFR_SINGLE_FUNCTION(sqrt)
MPFR_SINGLE_FUNCTION(cos)
MPFR_SINGLE_FUNCTION(sin)
MPFR_SINGLE_FUNCTION(tan)
MPFR_SINGLE_FUNCTION(acos)
MPFR_SINGLE_FUNCTION(asin)
MPFR_SINGLE_FUNCTION(atan)
MPFR_SINGLE_FUNCTION(cosh)
MPFR_SINGLE_FUNCTION(sinh)
MPFR_SINGLE_FUNCTION(tanh)
MPFR_SINGLE_FUNCTION(acosh)
MPFR_SINGLE_FUNCTION(asinh)
MPFR_SINGLE_FUNCTION(atanh)
MPFR_SINGLE_FUNCTION(log1p)
MPFR_SINGLE_FUNCTION(expm1)
MPFR_SINGLE_FUNCTION(log2)
MPFR_SINGLE_FUNCTION(log10)

static VALUE r_gmpfr_nan_p(VALUE self)
{
  MP_FLOAT *self_val;

  mpf_get_struct(self, self_val);
  if (mpfr_nan_p(self_val)) {
    return Qtrue;
  }
  else {
    return Qfalse;
  }
}

static VALUE r_gmpfr_inf_p(VALUE self)
{
  MP_FLOAT *self_val;

  mpf_get_struct(self, self_val);
  if (mpfr_inf_p(self_val)) {
    return Qtrue;
  }
  else {
    return Qfalse;
  }
}

static VALUE r_gmpfr_fin_p(VALUE self)
{
  if (r_gmpfr_inf_p(self)) {
    return Qfalse;
  }
  else {
    return Qtrue;
  }
}

static VALUE r_gmpfr_number_p(VALUE self)
{
  MP_FLOAT *self_val;

  mpf_get_struct(self, self_val);
  if (mpfr_number_p(self_val)) {
    return Qtrue;
  }
  else {
    return Qfalse;
  }
}

static VALUE r_gmpfr_pow(VALUE self, VALUE arg)
{
  MP_FLOAT *self_val, *res_val, *arg_val_f;
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  unsigned long prec;
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
      mpf_set_d(res_val, FLT2DBL(arg));
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


#endif
