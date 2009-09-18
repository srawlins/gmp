#ifndef _GMPZ_H_
#define _GMPZ_H_

/*
 * gmpz.h
 *
 * This file contains GMP::Z method definitions.
 */

#include <ruby_gmp.h>

#define DEFUN_INT_COND_P(fname,mpz_fname) \
static VALUE r_gmpz_##fname(VALUE self) \
{ \
  MP_INT *self_val; \
  mpz_get_struct(self, self_val); \
  return mpz_fname(self_val)?Qtrue:Qfalse; \
}

// DEFUN_INT_F_UL(pow,mpz_pow_ui,"exponent")
// DEFUN_INT_F_UL(shl,mpz_mul_2exp,"shift size")
// DEFUN_INT_F_UL(fshr,mpz_fdiv_q_2exp,"shift size")
// DEFUN_INT_F_UL(tshr,mpz_tdiv_q_2exp,"shift size")
// DEFUN_INT_F_UL(fshrm,mpz_fdiv_r_2exp,"mark size")
// DEFUN_INT_F_UL(tshrm,mpz_tdiv_r_2exp,"mark size")

static VALUE r_gmpz_eq(VALUE self, VALUE arg)
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

// static VALUE r_gmpz_cmp(VALUE self, VALUE arg)
// {
  // MP_INT *self_val;
  // int res;
  // mpz_get_struct (self,self_val);
  // res = mpz_cmp_value(self_val, arg);
  // if (res > 0)
    // return INT2FIX(1);
  // else if (res == 0)
    // return INT2FIX(0);
  // else
    // return INT2FIX(-1);
// }

// static VALUE r_gmpz_jacobi(VALUE self)
// {
  // MP_INT *self_val, *res_val;
  // VALUE res;
  // mpz_get_struct(self, self_val);
  // if (mpz_sgn(self_val) != 1)
    // rb_raise(rb_eRangeError, "you can take jacobi symbol only of positive value");
  // if (mpz_even_p(self_val))
    // rb_raise(rb_eRangeError, "you can't take jacobi symbol of even value");
  // mpz_make_struct_init(res, res_val);
  // mpz_jacobi(res_val, self_val);
  // return res;
// }

// static VALUE r_gmpz_legendre(VALUE self)
// {
  // MP_INT *self_val, *res_val;
  // VALUE res;
  // mpz_get_struct(self, self_val);
  // if (mpz_sgn(self_val) != 1)
    // rb_raise(rb_eRangeError, "you can take legendre symbol only of positive value");
  // if (mpz_even_p(self_val))
    // rb_raise(rb_eRangeError, "you can't take legendre symbol of even value");
    // mpz_make_struct_init(res, res_val);
  // mpz_legendre(res_val, self_val);
  // return res;
// }

// static VALUE r_gmpz_sqrtrem(VALUE self)
// {
  // MP_INT *self_val, *sqrt_val, *rem_val;
  // VALUE sqrt, rem;

  // mpz_get_struct (self, self_val);
  // mpz_make_struct_init(sqrt, sqrt_val);
  // mpz_make_struct_init(rem, rem_val);
  // mpz_sqrtrem (sqrt_val, rem_val, self_val);
  // return rb_assoc_new(sqrt, rem);
// }

// static VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp)
// {
  // MP_INT *res_val;
  // VALUE res;

  // if (FIXNUM_P(base) && FIXNUM_P(exp))
  // {
    // if (FIX2INT(base) < 0)
      // rb_raise (rb_eRangeError, "base must not be negative");
    // if (FIX2INT(exp) < 0) 
      // rb_raise (rb_eRangeError, "exponent must not be negative");
    // mpz_make_struct_init (res, res_val);
    // mpz_ui_pow_ui (res_val, base, exp);
    // return res;
  // }
  // return r_gmpz_pow (r_gmpzsg_new(1, &base, klass), exp);
// }

// static VALUE r_gmpz_to_s(VALUE self)
// {
  // MP_INT *self_val;
  // char *str;
  // VALUE res;

  // Data_Get_Struct(self, MP_INT, self_val);
  // str = mpz_get_str(NULL, 10, self_val);
  // res = rb_str_new2(str);
  // free (str);

  // return res;
// }

#endif
