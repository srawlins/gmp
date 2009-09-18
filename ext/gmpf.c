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


/**********************************************************************
 *    Initializing, Assigning Floats                                  *
 **********************************************************************/


/**********************************************************************
 *    Converting Floats                                               *
 **********************************************************************/


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
    mpf_set_d (res_val, FLT2DBL(arg));
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
 *   float * other
 *
 * Returns the product of +float+ and +other+. +other+ can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
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
    mpf_set_d(res_val, FLT2DBL(arg));
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

VALUE r_gmpf_cmp (VALUE self, VALUE arg)
{
  MP_FLOAT *self_val;
  int res;
  mpf_get_struct (self,self_val);
  res = mpf_cmp_value(self_val, arg);
  if (res > 0)
    return INT2FIX(1);
  else if (res == 0)
    return INT2FIX(0);
  else
    return INT2FIX(-1);
}

