#ifndef _RUBY_GMP_H_
#define _RUBY_GMP_H_

#define _GNU_SOURCE
#include <stdio.h>

#include <ruby.h>
#include <gmp.h>

#ifdef MPFR

#ifdef HAVE_MPFR_H
#include <mpfr.h>
#endif /* HAVE_MPFR_H */

#ifdef HAVE_MPF2MPFR_H
#include <mpf2mpfr.h>
#endif /* HAVE_MPF2MPFR_H */

#endif /* MPFR */

#include <stdlib.h>

/*
  MP_INT*, MP_RAT* and MP_FLOAT* are used because they don't have side-effects
  of single-element arrays mp*_t

  MP_FLOAT is defined here, as it's commented out in gmp.h
*/
#if defined(MPFR) && defined(HAVE_MPFR_H)
typedef __mpfr_struct MP_FLOAT;
#else
typedef __mpf_struct MP_FLOAT;
#endif /* HAVE_MPF2MPFR_H */

#define mpz_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_INT, c_var); }
#define mpq_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_RAT, c_var); }
#define mpf_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_FLOAT, c_var); }
#define mpf_get_struct_prec(ruby_var,c_var,prec) { mpf_get_struct(ruby_var,c_var); prec = mpf_get_prec(c_var); }
#define mpz_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Z, MP_INT, 0, r_gmpz_free, c_var); }
#define mpq_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Q, MP_RAT, 0, r_gmpq_free, c_var); }
#define mpf_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_F, MP_FLOAT, 0, r_gmpf_free, c_var); }
#define mpz_make_struct_init(ruby_var,c_var) { mpz_make_struct(ruby_var,c_var); mpz_init (c_var); }
#define mpq_make_struct_init(ruby_var,c_var) { mpq_make_struct(ruby_var,c_var); mpq_init (c_var); }
#define mpf_make_struct_init(ruby_var,c_var,prec) { mpf_make_struct(ruby_var,c_var); mpf_init2 (c_var,prec); }
#define BIGNUM_P(value) (TYPE(value) == T_BIGNUM)
#define FLOAT_P(value)  (TYPE(value) == T_FLOAT)
#define STRING_P(value) (TYPE(value) == T_STRING)
#define GMPZ_P(value)   (rb_obj_is_instance_of(value, cGMP_Z) == Qtrue)
#define GMPQ_P(value)   (rb_obj_is_instance_of(value, cGMP_Q) == Qtrue)
#define GMPF_P(value)   (rb_obj_is_instance_of(value, cGMP_F) == Qtrue)
#define mpz_set_bignum(var_mpz,var_bignum) \
  mpz_set_str (var_mpz, STR2CSTR (rb_funcall (var_bignum, rb_intern ("to_s"), 0)), 0);
#define mpz_temp_alloc(var) { var=malloc(sizeof(MP_INT)); }
#define mpz_temp_init(var) { mpz_temp_alloc(var); mpz_init(var); }
#define mpz_temp_from_bignum(var,var_bignum)  \
  { mpz_temp_alloc(var); mpz_init_set_str (var, STR2CSTR (rb_funcall (var_bignum, rb_intern ("to_s"), 0)), 0); }
#define mpz_temp_free(var) { mpz_clear(var); free(var); }
#define mpf_temp_alloc(var) { var=malloc(sizeof(MP_FLOAT)); }
#define mpf_temp_init(var,prec) { mpf_temp_alloc(var); mpf_init2(var,prec); }
#define mpf_temp_free(var) { mpf_clear(var); free(var); }
#define FLT2DBL(var) (RFLOAT(var)->value)
#define prec_max(prec,var) {if(mpf_get_prec(var) > prec) prec = mpf_get_prec(var); }

#define EXPECTED_ZQFXBD "Expected GMP::Z, GMP::Q, GMP::F, FixNum, BigNum or Float"
#define EXPECTED_ZQFXB "Expected GMP::Z, GMP::Q, GMP::F, FixNum or BigNum"
#define EXPECTED_ZXB "Expected GMP::Z, FixNum or BigNum"
#define EXPECTED_ZX "Expected GMP::Z or FixNum"
#define EXPECTED_X "Expected FixNum"
#define typeerror(expected) rb_raise(rb_eTypeError, EXPECTED_##expected)
#define typeerror_as(expected, argname) rb_raise(rb_eTypeError, EXPECTED_##expected " as " argname)

//should change exception type
#define not_yet rb_raise(rb_eTypeError,"Not implemented yet")

extern VALUE mGMP, cGMP_Z, cGMP_Q, cGMP_F;

extern void r_gmpz_free(void *ptr);
extern void r_gmpq_free(void *ptr);
extern void r_gmpf_free(void *ptr);


/* FROM gmpz.h !!! */

extern VALUE r_gmpz_add(VALUE self, VALUE arg);
extern VALUE r_gmpz_add_self(VALUE self, VALUE arg);
extern VALUE r_gmpz_sub(VALUE self, VALUE arg);
extern VALUE r_gmpz_sub_self(VALUE self, VALUE arg);
extern VALUE r_gmpz_mul(VALUE self, VALUE arg);
extern VALUE r_gmpz_div(VALUE self, VALUE arg);

#define DEFUN_INT_SINGLETON_UI(fname,mpz_fname)  \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE arg) \
{ \
  MP_INT *arg_val_z, *res_val; \
  unsigned long arg_val_ul; \
  VALUE res; \
 \
  (void)klass; \
 \
  if (FIXNUM_P(arg)) { \
    arg_val_ul = FIX2INT (arg); \
  } else if (GMPZ_P(arg)) { \
    mpz_get_struct (arg, arg_val_z); \
    if (!mpz_fits_ulong_p (arg_val_z)) \
      rb_raise (rb_eRangeError, "argument out of range"); \
    arg_val_ul = mpz_get_ui (arg_val_z); \
    if (arg_val_ul == 0) \
      rb_raise (rb_eRangeError, "argument out of range"); \
  } else { \
    typeerror_as (ZX, "argument"); \
  } \
  mpz_make_struct_init(res, res_val); \
  mpz_fname (res_val, arg_val_ul); \
  return res; \
}

DEFUN_INT_SINGLETON_UI(fib,mpz_fib_ui)
DEFUN_INT_SINGLETON_UI(fac,mpz_fac_ui)

extern VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp);
extern VALUE r_gmpz_to_s(VALUE self);


#endif
