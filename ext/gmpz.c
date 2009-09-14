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
VALUE r_gmpz_add(VALUE self, VALUE arg)
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

VALUE r_gmpz_add_self(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    mpz_add (self_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) > 0)
      mpz_add_ui (self_val, self_val, FIX2INT(arg));
    else
      mpz_sub_ui (self_val, self_val, -FIX2INT(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum (arg_val, arg);
    mpz_add (self_val, self_val, arg_val);
    mpz_temp_free (arg_val);
  } else {
    typeerror (ZXB);
  }
  return Qnil;
}

static VALUE r_gmpz_sub(VALUE self, VALUE arg)
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

static VALUE r_gmpz_sub_self(VALUE self, VALUE arg)
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

static VALUE r_gmpz_mul(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_get_struct(arg,arg_val);
    mpz_mul (res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_mul_si (res_val, self_val, FIX2INT(arg));
  } else if (GMPQ_P(arg)) {
    return r_gmpq_mul(arg, self);
  } else if (GMPF_P(arg)) {
    return r_gmpf_mul(arg, self);
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_set_bignum (res_val, arg);
    mpz_mul (res_val, res_val, self_val);
  } else {
    typeerror (ZQFXB);
  }
  return res;
}

static VALUE r_gmpz_div(VALUE self, VALUE arg)
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
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init(res, res_val_q);
    mpq_set_num (res_val_q, self_val);
    mpq_set_den (res_val_q, arg_val_z);
    mpq_canonicalize (res_val_q);
  } else if (FIXNUM_P(arg)) {
    if (FIX2INT(arg) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init(res, res_val_q);
    mpq_set_num (res_val_q, self_val);
    mpz_set_ui (mpq_denref(res_val_q), FIX2INT(arg));
    mpq_canonicalize (res_val_q);
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg, arg_val_q);
    if (mpz_cmp_ui(mpq_numref(arg_val_q), 0) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpz_temp_init(tmp_z);
    mpq_make_struct_init(res, res_val_q);
    mpz_gcd (tmp_z, mpq_numref(arg_val_q), self_val);
    mpz_divexact (mpq_numref(res_val_q), self_val, tmp_z);
    mpz_divexact (mpq_denref(res_val_q), mpq_numref(arg_val_q), tmp_z);
    mpz_mul (mpq_numref(res_val_q), mpq_numref(res_val_q), mpq_denref(arg_val_q));
    mpz_temp_free(tmp_z);
  } else if (GMPF_P(arg)) {
    mpf_get_struct_prec (arg, arg_val_f, prec);
    mpf_make_struct_init(res, res_val_f, prec);
    mpf_set_z (res_val_f, self_val);
    mpf_div (res_val_f, res_val_f, arg_val_f);
  } else if (BIGNUM_P(arg)) {
    mpq_make_struct_init(res, res_val_q);
    mpz_set_bignum (mpq_denref(res_val_q), arg);
    if (mpz_cmp_ui(mpq_denref(res_val_q), 0) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_set_num (res_val_q, self_val);
    mpq_canonicalize (res_val_q);
  } else {
    typeerror (ZQFXB);
  }
  return res;
}

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
