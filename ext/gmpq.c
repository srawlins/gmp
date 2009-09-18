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
 */

/**********************************************************************
 *    Macros                                                          *
 **********************************************************************/


/**********************************************************************
 *    Initializing Rationals                                          *
 **********************************************************************/


/**********************************************************************
 *    Rational Conversions                                            *
 **********************************************************************/


/**********************************************************************
 *    Rational Arithmetic                                             *
 **********************************************************************/


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



