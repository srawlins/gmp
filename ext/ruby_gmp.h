#ifndef _RUBY_GMP_H_
#define _RUBY_GMP_H_

#include <stdio.h>
#include <ruby.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
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
/*
 * Here, MPFR is not included, so we are pure GMP. In GMP, mpf_get_prec returns
 * an unsigned long int, so we will too.
 */
typedef unsigned long mpfr_prec_t;
#endif /* HAVE_MPF2MPFR_H */

#if __GNU_MP_VERSION < 5
typedef unsigned long int mp_bitcnt_t;
#endif

/*
  I don't like this solution, because in gmp.h, all of these typedefs are
  marked with "gmp 1 source compatibility". :(.
*/
typedef __gmp_randstate_struct MP_RANDSTATE;

#define mpz_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_INT, c_var); }
#define mpq_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_RAT, c_var); }
#define mpf_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_FLOAT, c_var); }
#define mprandstate_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_RANDSTATE, c_var); }
#define mpz_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Z, MP_INT, 0, r_gmpz_free, c_var); }
#define mpq_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Q, MP_RAT, 0, r_gmpq_free, c_var); }
#define mpf_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_F, MP_FLOAT, 0, r_gmpf_free, c_var); }
#define mprandstate_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_RandState, MP_RANDSTATE, 0, r_gmprandstate_free, c_var); }
#define mpz_make_struct_init(ruby_var,c_var) { mpz_make_struct(ruby_var,c_var); mpz_init (c_var); }
#define mpq_make_struct_init(ruby_var,c_var) { mpq_make_struct(ruby_var,c_var); mpq_init (c_var); }
#define BIGNUM_P(value) (TYPE(value) == T_BIGNUM)
#define FLOAT_P(value)  (TYPE(value) == T_FLOAT)
#define STRING_P(value) (TYPE(value) == T_STRING)
#define GMPZ_P(value)   (rb_obj_is_instance_of(value, cGMP_Z) == Qtrue)
#define GMPQ_P(value)   (rb_obj_is_instance_of(value, cGMP_Q) == Qtrue)
#define GMPF_P(value)   (rb_obj_is_instance_of(value, cGMP_F) == Qtrue)
#define mpz_set_bignum(var_mpz,var_bignum) {                   \
  VALUE tmp = rb_funcall (var_bignum, rb_intern ("to_s"), 1, INT2FIX(32));  \
  mpz_set_str (var_mpz, StringValuePtr (tmp), 32);              \
}
#define mpz_temp_alloc(var) { var=malloc(sizeof(MP_INT)); }
#define mpz_temp_init(var) { mpz_temp_alloc(var); mpz_init(var); }
#define mpz_temp_from_bignum(var,var_bignum) {                 \
  mpz_temp_alloc(var);                                         \
  VALUE tmp = rb_funcall (var_bignum, rb_intern ("to_s"), 1, INT2FIX(32));  \
  mpz_init_set_str (var, StringValuePtr (tmp), 32);             \
}
#define mpz_temp_free(var) { mpz_clear(var); free(var); }
#define mpf_temp_alloc(var) { var=malloc(sizeof(MP_FLOAT)); }
#if defined(MPFR) && defined(HAVE_MPFR_H)
#define prec_max(prec,var) {if(mpfr_get_prec(var) > prec) prec = mpfr_get_prec(var); }
#else
#define prec_max(prec,var) {if(mpf_get_prec(var) > prec) prec = mpf_get_prec(var); }
#endif

#if defined(MPFR) && defined(HAVE_MPFR_H)
#define mpf_get_struct_prec(ruby_var,c_var,prec) { mpf_get_struct(ruby_var,c_var); prec = mpfr_get_prec(c_var); }
#define mpf_make_struct_init(ruby_var,c_var,prec) { mpf_make_struct(ruby_var,c_var); mpfr_init2 (c_var,prec); }
#define mpf_temp_init(var,prec) { mpf_temp_alloc(var); mpfr_init2(var,prec); }
#define mpf_temp_free(var) { mpfr_clear(var); free(var); }
#define r_mpf_init(var1) (mpfr_init(var1))
#define r_mpf_init2(var1, var2) (mpfr_init2(var1, var2))
#define r_mpf_set_z(var1, var2) (mpfr_set_z(var1, var2, __gmp_default_rounding_mode))
#define r_mpf_set_q(var1, var2) (mpfr_set_q(var1, var2, __gmp_default_rounding_mode))
#define r_mpf_set_d(var1, var2) (mpfr_set_d(var1, var2, __gmp_default_rounding_mode))
#define r_mpf_set_str(var1, var2, var3) (mpfr_set_str(var1, var2, var3, __gmp_default_rounding_mode))
#define r_mpf_cmp(var1, var2) (mpfr_cmp(var1, var2))
#else
#define mpf_get_struct_prec(ruby_var,c_var,prec) { mpf_get_struct(ruby_var,c_var); prec = mpf_get_prec(c_var); }
#define mpf_make_struct_init(ruby_var,c_var,prec) { mpf_make_struct(ruby_var,c_var); mpf_init2 (c_var,prec); }
#define mpf_temp_init(var,prec) { mpf_temp_alloc(var); mpf_init2(var,prec); }
#define mpf_temp_free(var) { mpf_clear(var); free(var); }
#define r_mpf_init(var1) (mpf_init(var1))
#define r_mpf_init2(var1, var2) (mpf_init2(var1, var2))
#define r_mpf_set_z(var1, var2) (mpf_set_z(var1, var2))
#define r_mpf_set_q(var1, var2) (mpf_set_q(var1, var2))
#define r_mpf_set_d(var1, var2) (mpf_set_d(var1, var2))
#define r_mpf_set_str(var1, var2, var3) (mpf_set_str(var1, var2, var3))
#define r_mpf_cmp(var1, var2) (mpf_cmp(var1, var2))
#endif

#ifdef FIXNUM_WIDTH       /* RBX check */
//#if ((FIXNUM_WIDTH()) == 62)       /* 64-bit */
#if (((8*SIZEOF_INTPTR_T) - TAG_FIXNUM_SHIFT -1) == 63)
#define FIX2NUM(x) FIX2LONG(x)
//#else                        /* 32-bit */
//#define FIX2NUM(x) FIX2INT(x)
#endif
#else                     /* RBX check */
#if SIZEOF_INT < SIZEOF_LONG /* 64-bit */
#define FIX2NUM(x) FIX2LONG(x)
#else                        /* 32-bit */
#define FIX2NUM(x) FIX2INT(x)
#endif         /* MRI's 32-vs-64 check */
#endif                    /* RBX check */

#define EXPECTED_ZQFXBD "Expected GMP::Z, GMP::Q, GMP::F, Fixnum, Bignum or Float"
#define EXPECTED_ZQFXB "Expected GMP::Z, GMP::Q, GMP::F, Fixnum or Bignum"
#define EXPECTED_ZFXBD "Expected GMP::Z, GMP::F, Fixnum, Bignum or Float"
#define EXPECTED_ZXB "Expected GMP::Z, Fixnum or Bignum"
#define EXPECTED_ZX "Expected GMP::Z or Fixnum"
#define EXPECTED_X "Expected Fixnum"
#define EXPECTED_Z "Expected GMP::Z"
#define EXPECTED_FD "Expected GMP::F or Float"
#define typeerror(expected) rb_raise(rb_eTypeError, EXPECTED_##expected)
#define typeerror_as(expected, argname) rb_raise(rb_eTypeError, EXPECTED_##expected " as " argname)

//should change exception type
#define not_yet rb_raise(rb_eTypeError,"Not implemented yet")

#ifdef MPFR
#define mprnd_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, int, c_var); }
#define mprnd_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Rnd, int, 0, 0, c_var); }
#define GMPRND_P(value)   (rb_obj_is_instance_of(value, cGMP_Rnd) == Qtrue)
#endif /* MPFR */

extern VALUE mGMP, cGMP_Z, cGMP_Q, cGMP_F, cGMP_RandState;
#ifdef MPFR
extern VALUE cGMP_Rnd;
#endif /* MPFR */

extern void r_gmpz_free(void *ptr);
extern void r_gmpq_free(void *ptr);
extern void r_gmpf_free(void *ptr);
extern void r_gmprandstate_free(void *ptr);


/* from gmpz.h */

// Initializing, Assigning Integers
extern VALUE r_gmpzsg_new(int argc, VALUE *argv, VALUE klass);
extern VALUE r_gmpz_initialize(int argc, VALUE *argv, VALUE self);
extern void mpz_set_value(MP_INT *target, VALUE source);
extern VALUE r_gmpmod_z(int argc, VALUE *argv, VALUE module);
extern VALUE r_gmpz_swap(VALUE self, VALUE arg);

// Converting Integers
extern VALUE r_gmpz_to_i(VALUE self);
extern VALUE r_gmpz_to_d(VALUE self);
extern VALUE r_gmpz_to_s(int argc, VALUE *argv, VALUE self);

// Integer Arithmetic
extern VALUE r_gmpz_add(VALUE self, VALUE arg);
extern VALUE r_gmpz_add_self(VALUE self, VALUE arg);
extern VALUE r_gmpz_sub(VALUE self, VALUE arg);
extern VALUE r_gmpz_sub_self(VALUE self, VALUE arg);
extern VALUE r_gmpz_mul(VALUE self, VALUE arg);
extern VALUE r_gmpz_div(VALUE self, VALUE arg);

// Integer Division
extern VALUE r_gmpz_mod(VALUE self, VALUE arg);

// Integer Exponentiation
extern VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp);
extern VALUE r_gmpz_powm(VALUE self, VALUE exp, VALUE mod);

// Integer Roots

// Number Theoretic Functions
extern VALUE r_gmpz_is_probab_prime(int argc, VALUE* argv, VALUE self);
extern VALUE r_gmpz_gcd(VALUE self, VALUE arg);
extern VALUE r_gmpz_invert(VALUE self, VALUE arg);
extern VALUE r_gmpz_jacobi(VALUE self, VALUE b);
extern VALUE r_gmpzsg_jacobi(VALUE klass, VALUE a, VALUE b);
extern VALUE r_gmpz_legendre(VALUE self, VALUE p);
extern VALUE r_gmpz_remove(VALUE self, VALUE arg);

// Integer Comparisons
extern VALUE r_gmpz_eq(VALUE self, VALUE arg);
extern VALUE r_gmpz_cmp(VALUE self, VALUE arg);
extern VALUE r_gmpz_cmpabs(VALUE self, VALUE arg);
extern VALUE r_gmpz_sgn(VALUE self);
extern int mpz_cmp_value(MP_INT *OP, VALUE arg);

// Integer Logic and Bit Fiddling
extern VALUE r_gmpz_popcount(VALUE self);
extern VALUE r_gmpz_scan0(VALUE self, VALUE bitnr);
extern VALUE r_gmpz_scan1(VALUE self, VALUE bitnr);
extern VALUE r_gmpz_setbit(VALUE self, VALUE bitnr, VALUE set_to);
extern VALUE r_gmpz_getbit(VALUE self, VALUE bitnr);

// Miscelaneous Integer Functions
extern VALUE r_gmpz_sizeinbase(VALUE self, VALUE base);
extern VALUE r_gmpz_size_in_bin(VALUE self);


/* from gmpq.h */

// Initializing Rationals
extern VALUE r_gmpqsg_new(int argc, VALUE *argv, VALUE klass);
extern VALUE r_gmpmod_q(int argc, VALUE *argv, VALUE module);
extern VALUE r_gmpq_swap(VALUE self, VALUE arg);

// Rational Conversions
extern VALUE r_gmpq_to_d(VALUE self);
extern VALUE r_gmpq_to_s(VALUE self);

// Rational Arithmetic
extern VALUE r_gmpq_add(VALUE self, VALUE arg);
extern VALUE r_gmpq_sub(VALUE self, VALUE arg);
extern VALUE r_gmpq_mul(VALUE self, VALUE arg);
extern VALUE r_gmpq_div(VALUE self, VALUE arg);
extern VALUE r_gmpq_inv(VALUE self);
extern VALUE r_gmpq_inv_self(VALUE self);

// Comparing Rationals
extern VALUE r_gmpq_eq(VALUE self, VALUE arg);
extern VALUE r_gmpq_cmp(VALUE self, VALUE arg);
extern int mpq_cmp_value(MP_RAT *OP, VALUE arg);
extern VALUE r_gmpq_sgn(VALUE self);

// Applying Integer Functions
extern VALUE r_gmpq_num(VALUE self);
extern VALUE r_gmpq_den(VALUE self);

// _unsorted_


/* from gmpf.h */

// Initializing, Assigning Floats
extern VALUE r_gmpfsg_new(int argc, VALUE *argv, VALUE klass);
extern VALUE r_gmpf_initialize(int argc, VALUE *argv, VALUE self);
extern void mpf_set_value(MP_FLOAT *self_val, VALUE arg);
extern VALUE r_gmpmod_f(int argc, VALUE *argv, VALUE module);
extern VALUE r_gmpf_get_prec(VALUE self);
extern VALUE r_gmpf_set_prec(VALUE self, VALUE arg);
extern VALUE r_gmpf_set_prec_raw(VALUE self, VALUE arg);

// Converting Floats
extern VALUE r_gmpf_to_d(VALUE self);
extern VALUE r_gmpf_to_s(VALUE self);

// Float Arithmetic
#ifndef MPFR
extern VALUE r_gmpf_add(VALUE self, VALUE arg);
extern VALUE r_gmpf_mul(VALUE self, VALUE arg);
#else
extern VALUE r_gmpfr_add(int argc, VALUE *argv, VALUE self);
extern VALUE r_gmpfr_mul(int argc, VALUE *argv, VALUE self);
#endif
extern VALUE r_gmpf_sub(VALUE self, VALUE arg);
extern VALUE r_gmpf_div(VALUE self, VALUE arg);

// Float Comparison
extern VALUE r_gmpf_eq(VALUE self, VALUE arg);
extern VALUE r_gmpf_cmp(VALUE self, VALUE arg);
extern int mpf_cmp_value(MP_FLOAT *OP, VALUE arg);

// MPFR
#ifdef MPFR
  extern void mpf_set_value2(MP_FLOAT *self_val, VALUE arg, int base);
  
  extern VALUE r_gmpfr_sqrt(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_rec_sqrt(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_cbrt(int argc, VALUE *argv, VALUE self);
  
  extern VALUE r_gmpfr_log(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_log2(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_log10(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_exp(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_exp2(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_exp10(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_cos(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_sin(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_tan(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_sec(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_csc(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_cot(int argc, VALUE *argv, VALUE self);
  
  extern VALUE r_gmpfr_acos(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_asin(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_atan(int argc, VALUE *argv, VALUE self);
  
  extern VALUE r_gmpfr_cosh(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_sinh(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_tanh(int argc, VALUE *argv, VALUE self);
  
  extern VALUE r_gmpfr_sech(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_csch(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_coth(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_acosh(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_asinh(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_atanh(int argc, VALUE *argv, VALUE self);
  
  extern VALUE r_gmpfr_log1p(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_expm1(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_eint(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_li2(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_gamma(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_lngamma(int argc, VALUE *argv, VALUE self);
  /*extern VALUE r_gmpfr_lgamma(int argc, VALUE *argv, VALUE self);*/
#if MPFR_VERSION_MAJOR > 2
  extern VALUE r_gmpfr_digamma(int argc, VALUE *argv, VALUE self);
#endif
  extern VALUE r_gmpfr_zeta(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_erf(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_erfc(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_j0(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_j1(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_jn(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_y0(int argc, VALUE *argv, VALUE self);
  extern VALUE r_gmpfr_y1(int argc, VALUE *argv, VALUE self);
  
  extern VALUE r_gmpfrsg_const_log2();
  extern VALUE r_gmpfrsg_const_pi();
  extern VALUE r_gmpfrsg_const_euler();
  extern VALUE r_gmpfrsg_const_catalan();
  
  extern mp_rnd_t r_get_rounding_mode(VALUE rnd);
#endif /* MPFR */

// _unsorted_
extern VALUE r_gmpf_sgn(VALUE self);


/* from gmprandstate.h */

// Random State Initialization
extern VALUE r_gmprandstatesg_new(int argc, VALUE *argv, VALUE klass);
extern VALUE r_gmprandstate_initialize(int argc, VALUE *argv, VALUE self);
extern VALUE r_gmpmod_randstate(int argc, VALUE *argv, VALUE module);

// Random State Seeding
extern VALUE r_gmprandstate_seed(VALUE self, VALUE arg);

// Integer Random Numbers
extern VALUE r_gmprandstate_urandomb(VALUE self, VALUE arg);
extern VALUE r_gmprandstate_urandomm(VALUE self, VALUE arg);

#ifdef MPFR
// Float Random Numbers
extern VALUE r_gmprandstate_mpfr_urandomb(int argc, VALUE *argv, VALUE self);
#endif /* MPFR */


/* from gmpbench_timing.c */

// GMPbench Timing
extern VALUE r_gmpmod_cputime(VALUE self);
extern VALUE r_gmpmod_time(VALUE self);



extern void init_gmpz();
extern void init_gmpq();
extern void init_gmpf();
extern void init_gmprandstate();
extern void init_gmpbench_timing();
#ifdef MPFR
extern void init_gmprnd();
#endif /* MPFR */

#endif
