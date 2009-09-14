#include <gmpz.h>

/*
 * call-seq: +(other)
 *
 * Adds this GMP::Z to other. Other can be
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
static VALUE r_gmpz_add(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    mpz_make_struct_init(res, res_val);
    mpz_add (res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    if (FIX2INT(arg) > 0)
      mpz_add_ui (res_val, self_val, FIX2INT(arg));
    else
      mpz_sub_ui (res_val, self_val, -FIX2INT(arg));
  } else if (GMPQ_P(arg)) {
    return r_gmpq_add(arg, self);
  } else if (GMPF_P(arg)) {
    return r_gmpf_add(arg, self);
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_init (res_val);
    mpz_set_bignum (res_val, arg);
    mpz_add (res_val, res_val, self_val);
  } else {
    typeerror (ZQFXB);
  }
  return res;
}