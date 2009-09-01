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
    MP_INT*, MP_RAT* and MP_FLOAT* are used because
    they don't have side-effects of single-element arrays mp*_t
    
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
#define BIGNUM_P(value)	(TYPE(value) == T_BIGNUM)
#define FLOAT_P(value)	(TYPE(value) == T_FLOAT)
#define STRING_P(value)	(TYPE(value) == T_STRING)
#define GMPZ_P(value)	(rb_obj_is_instance_of(value, cGMP_Z) == Qtrue)
#define GMPQ_P(value)	(rb_obj_is_instance_of(value, cGMP_Q) == Qtrue)
#define GMPF_P(value)	(rb_obj_is_instance_of(value, cGMP_F) == Qtrue)
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

VALUE mGMP, cGMP_Z, cGMP_Q, cGMP_F;

static void r_gmpz_free(void *ptr) { mpz_clear (ptr); free (ptr); }
static void r_gmpq_free(void *ptr) { mpq_clear (ptr); free (ptr); }
static void r_gmpf_free(void *ptr) { mpf_clear (ptr); free (ptr); }

static VALUE r_gmpzsg_new(int argc, VALUE *argv, VALUE klass)
{
    MP_INT *res_val;
    VALUE res;

    (void)klass;

    if (argc > 1)
	rb_raise(rb_eArgError, "wrong # of arguments(%d for 0 or 1)", argc);

    mpz_make_struct (res, res_val);
    mpz_init (res_val);

    rb_obj_call_init(res, argc, argv);
 
    return res;
}

static VALUE r_gmpqsg_new(int argc, VALUE *argv, VALUE klass)
{
    MP_RAT *res_val;
    VALUE res;

    (void)klass;

    if (argc > 2)
	rb_raise(rb_eArgError, "wrong # of arguments(%d for 0, 1 or 2)", argc);

    mpq_make_struct (res, res_val);
    mpq_init (res_val);
    rb_obj_call_init(res, argc, argv);
 
    return res;
}

static VALUE r_gmpfsg_new(int argc, VALUE *argv, VALUE klass)
{
    MP_FLOAT *res_val;
    VALUE res;

    (void)klass;

    if (argc > 2)
	rb_raise(rb_eArgError, "wrong # of arguments(%d for 0, 1 or 2)", argc);

    mpf_make_struct (res, res_val);
    rb_obj_call_init(res, argc, argv);
 
    return res;
}

static void mpz_set_value (MP_INT *target, VALUE source)
{
    MP_INT *source_val;

    if (GMPZ_P(source)) {
	mpz_get_struct(source, source_val);
	mpz_set (target, source_val);
    } else if (FIXNUM_P(source)) {
	mpz_set_si (target, NUM2INT(source));
    } else if (STRING_P(source)) {
	mpz_set_str (target, STR2CSTR(source), 0);
    } else if (BIGNUM_P(source)) {
	mpz_set_bignum(target, source);
    } else {
	rb_raise (rb_eTypeError, "Don't know how to convert %s into GMP_Z", rb_class2name(rb_class_of(source)));
    }
}

static VALUE r_gmpz_initialize(int argc, VALUE *argv, VALUE self)
{
    MP_INT *self_val;

    if (argc != 0)
	{
	    mpz_get_struct(self,self_val);
	    mpz_set_value (self_val, argv[0]);
	}
    return Qnil;
}

static void mpq_str_set (MP_RAT *ROP, char *str)
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
    }
    else
    {
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
	    if (argc == 2)
    	    {
	        mpz_set_value (mpq_denref(self_val), argv[1]);
	        mpq_canonicalize(self_val);
	    }
	}
    }
    return Qnil;
}

/* don't pass GMP::F here, it should be handled separately */
static void mpf_set_value(MP_FLOAT *self_val, VALUE arg)
{
    MP_RAT *arg_val_q;
    MP_INT *arg_val_z;

     if (GMPQ_P(arg)) {
        mpq_get_struct (arg, arg_val_q);
	mpf_set_q(self_val, arg_val_q);
    } else if (GMPZ_P(arg)) {
        mpz_get_struct (arg, arg_val_z);
	mpf_set_z(self_val, arg_val_z);
    } else if (FLOAT_P(arg)) {
	mpf_set_d(self_val, FLT2DBL(arg));
    } else if (FIXNUM_P(arg)) {
	mpf_set_si(self_val, FIX2INT(arg));
    } else if (STRING_P(arg)) {
	if (mpf_set_str(self_val, STR2CSTR(arg), 10) == -1) {
	    rb_raise (rb_eRuntimeError, "Badly formatted string");
	}
    } else if (BIGNUM_P(arg)) {
#if 1 /* GMP3 code */
	mpz_temp_from_bignum(arg_val_z, arg);
	mpf_set_z(self_val, arg_val_z);
	mpz_temp_free(arg_val_z);
#endif
    } else {
	rb_raise (rb_eTypeError, "Don't know how to convert %s into GMP::F", rb_class2name(rb_class_of(arg)));
    }
}

static VALUE r_gmpf_initialize(int argc, VALUE *argv, VALUE self)
{
    MP_FLOAT *self_val, *arg_val_f;
    unsigned long prec = 0;
    VALUE arg;
    
    mpf_get_struct (self, self_val);
    
    if (argc==0) {
	mpf_init(self_val);
	mpf_set_si(self_val, 0);
	return Qnil;
    }

    arg = argv[0];

    if (argc == 2) {
	 if (FIXNUM_P(argv[1])) {
	      if (FIX2INT(argv[1]) >= 0)
		   prec = FIX2INT(argv[1]);
	      else
		   rb_raise(rb_eRangeError, "prec must be non-negative");
	 } else {
	      rb_raise(rb_eTypeError, "prec must be FixNum");
	 }
    } else if (GMPF_P(arg)) {
        mpf_get_struct (arg, arg_val_f);
	prec = mpf_get_prec (arg_val_f);
    }
    if (prec == 0)
	mpf_init (self_val);
    else
	mpf_init2 (self_val, prec);

    if (GMPF_P(arg)) {
        mpf_get_struct (arg, arg_val_f);
	mpf_set(self_val, arg_val_f);
    } else {
	mpf_set_value(self_val, arg);
    }
    
    return Qnil;
}

static VALUE r_gmpq_to_s(VALUE self)
{
    MP_RAT *self_val;
    MP_INT *self_val_num, *self_val_den;
    char *str;
    VALUE res;
    int sizeinbase;
    int offset;

    Data_Get_Struct(self, MP_RAT, self_val);

    if (mpz_cmp_ui(mpq_denref(self_val), 1) == 0)
	{
    	    str = mpz_get_str(NULL, 10, mpq_numref (self_val));
	    res = rb_str_new2(str);
	    free (str);
	    return res;
	}
    
    self_val_num = mpq_numref(self_val);
    self_val_den = mpq_denref(self_val);

    sizeinbase = mpz_sizeinbase (self_val_num, 10) + mpz_sizeinbase (self_val_den, 10) + 3;
    str = malloc (sizeinbase);

    mpz_get_str (str, 10, self_val_num);
    offset = strlen (str);
    str[offset] = '/';
    mpz_get_str (str + offset + 1, 10, self_val_den);
    res = rb_str_new2(str);
    free (str);

    return res;
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

static VALUE r_gmpmod_z(int argc, VALUE *argv, VALUE module)
{
    (void)module;
    return r_gmpzsg_new(argc, argv, cGMP_Z);
}

static VALUE r_gmpmod_q(int argc, VALUE *argv, VALUE module)
{
    (void)module;
    return r_gmpqsg_new(argc, argv, cGMP_Q);
}

static VALUE r_gmpmod_f(int argc, VALUE *argv, VALUE module)
{
    (void)module;
    return r_gmpfsg_new(argc, argv, cGMP_F);
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
	if (FIX2INT(arg) <= 0)
	{
	    rb_raise(rb_eRangeError, "prec must be positive");
	}
	mpf_set_default_prec (FIX2INT(arg));
    } else {
	rb_raise(rb_eTypeError, "prec must be FixNum");
    }
    return Qnil;
}

#include "gmpf.h"
#include "gmpq.h"
#include "gmpz.h"
#include "takeover.h"

#define REGISTER_TAKEOVER(fname, ruby_fname, old_fname) \
    rb_define_alias(rb_cFixnum, old_fname, ruby_fname); \
    rb_define_method(rb_cFixnum, ruby_fname, takeover_fixnum_##fname, -1); \
    rb_define_alias(rb_cBignum, old_fname, ruby_fname); \
    rb_define_method(rb_cBignum, ruby_fname, takeover_bignum_##fname, -1);

void Init_gmp () {
    mGMP = rb_define_module("GMP");
    rb_define_module_function(mGMP, "Z", r_gmpmod_z, -1);
    rb_define_module_function(mGMP, "Q", r_gmpmod_q, -1);
    rb_define_module_function(mGMP, "F", r_gmpmod_f, -1);

    cGMP_Z = rb_define_class_under (mGMP, "Z", rb_cInteger);
    rb_define_singleton_method(cGMP_Z, "new", r_gmpzsg_new, -1);
    rb_define_singleton_method(cGMP_Z, "fib", r_gmpzsg_fib, 1);
    rb_define_singleton_method(cGMP_Z, "fac", r_gmpzsg_fac, 1);
    rb_define_singleton_method(cGMP_Z, "pow", r_gmpzsg_pow, 2);
    rb_define_method(cGMP_Z, "initialize", r_gmpz_initialize, -1);
    rb_define_method(cGMP_Z, "to_s", r_gmpz_to_s, 0);
    rb_define_method(cGMP_Z, "coerce", r_gmpz_coerce, 1);
    rb_define_method(cGMP_Z, "+", r_gmpz_add, 1);
    rb_define_method(cGMP_Z, "add!", r_gmpz_add_self, 1);
    rb_define_method(cGMP_Z, "-", r_gmpz_sub, 1);
    rb_define_method(cGMP_Z, "sub!", r_gmpz_sub_self, 1);
    rb_define_method(cGMP_Z, "*", r_gmpz_mul, 1);
    rb_define_method(cGMP_Z, "/", r_gmpz_div, 1);
    rb_define_method(cGMP_Z, "tdiv", r_gmpz_tdiv, 1);
    rb_define_method(cGMP_Z, "tmod", r_gmpz_tmod, 1);
    rb_define_method(cGMP_Z, "fdiv", r_gmpz_fdiv, 1);
    rb_define_method(cGMP_Z, "fmod", r_gmpz_fmod, 1);
    rb_define_method(cGMP_Z, "cdiv", r_gmpz_cdiv, 1);
    rb_define_method(cGMP_Z, "cmod", r_gmpz_cmod, 1);
    rb_define_method(cGMP_Z, "-@", r_gmpz_neg, 0);
    rb_define_method(cGMP_Z, "neg", r_gmpz_neg, 0);
    rb_define_method(cGMP_Z, "neg!", r_gmpz_neg_self, 0);
    rb_define_method(cGMP_Z, "abs", r_gmpz_abs, 0);
    rb_define_method(cGMP_Z, "abs!", r_gmpz_abs_self, 0);
    rb_define_method(cGMP_Z, "com", r_gmpz_com, 0);
    rb_define_method(cGMP_Z, "com!", r_gmpz_com_self, 0);
    rb_define_method(cGMP_Z, "&", r_gmpz_and, 1);
    rb_define_method(cGMP_Z, "|", r_gmpz_or, 1);
    rb_define_method(cGMP_Z, "^", r_gmpz_xor, 1);
    rb_define_method(cGMP_Z, "[]=", r_gmpz_setbit, 2);
    rb_define_method(cGMP_Z, "[]", r_gmpz_getbit, 1);
    rb_define_method(cGMP_Z, "scan0", r_gmpz_scan0, 1);
    rb_define_method(cGMP_Z, "scan1", r_gmpz_scan1, 1);
    rb_define_method(cGMP_Z, "**", r_gmpz_pow, 1);
    rb_define_method(cGMP_Z, "powmod", r_gmpz_powm, 2);
    rb_define_method(cGMP_Z, "even?", r_gmpz_is_even, 0);
    rb_define_method(cGMP_Z, "odd?", r_gmpz_is_odd, 0);
    rb_define_method(cGMP_Z, "sgn", r_gmpz_sgn, 0);
    rb_define_method(cGMP_Z, "<=>", r_gmpz_cmp, 1);
    rb_define_method(cGMP_Z, ">",   r_gmpz_cmp_gt, 1);
    rb_define_method(cGMP_Z, ">=",  r_gmpz_cmp_ge, 1);
    rb_define_method(cGMP_Z, "<",   r_gmpz_cmp_lt, 1);
    rb_define_method(cGMP_Z, "<=",  r_gmpz_cmp_le, 1);
    rb_define_method(cGMP_Z, "==",  r_gmpz_eq, 1);
    rb_define_method(cGMP_Z, ">>",  r_gmpz_fshr, 1);
    rb_define_method(cGMP_Z, "<<",  r_gmpz_shl, 1);
    rb_define_method(cGMP_Z, "tshr",  r_gmpz_tshr, 1);
    rb_define_method(cGMP_Z, "lastbits_sgn",  r_gmpz_tshrm, 1);
    rb_define_method(cGMP_Z, "lastbits_pos",  r_gmpz_fshrm, 1);
    rb_define_method(cGMP_Z, "square?",  r_gmpz_is_square, 0);
    rb_define_method(cGMP_Z, "power?",  r_gmpz_is_power, 0);
    rb_define_method(cGMP_Z, "swap",  r_gmpz_swap, 1);
    rb_define_method(cGMP_Z, "sqrt",  r_gmpz_sqrt, 0);
    rb_define_method(cGMP_Z, "sqrt!",  r_gmpz_sqrt_self, 0);
    rb_define_method(cGMP_Z, "sqrtrem",  r_gmpz_sqrtrem, 0);
    rb_define_method(cGMP_Z, "jacobi",  r_gmpz_jacobi, 0);
    rb_define_method(cGMP_Z, "legendre",  r_gmpz_legendre, 0);
    rb_define_method(cGMP_Z, "nextprime",  r_gmpz_nextprime, 0);
    rb_define_method(cGMP_Z, "nextprime!",  r_gmpz_nextprime_self, 0);
    rb_define_method(cGMP_Z, "to_d",  r_gmpz_to_d, 0);
    rb_define_method(cGMP_Z, "root",  r_gmpz_root, 1);
    rb_define_method(cGMP_Z, "remove",  r_gmpz_remove, 1);
    rb_define_method(cGMP_Z, "to_i",  r_gmpz_to_i, 0);
    rb_define_method(cGMP_Z, "cmpabs",  r_gmpz_cmpabs, 1);
/*
    rb_define_method(cGMP_Z, "gcd",  r_gmpz_gcd, 1);
    rb_define_method(cGMP_Z, "lcm",  r_gmpz_lcm, 1);
*/
    cGMP_Q = rb_define_class_under (mGMP, "Q", rb_cNumeric);
    rb_define_singleton_method(cGMP_Q, "new", r_gmpqsg_new, -1);
    rb_define_method(cGMP_Q, "initialize", r_gmpq_initialize, -1);
    rb_define_method(cGMP_Q, "to_s", r_gmpq_to_s, 0);
    rb_define_method(cGMP_Q, "coerce", r_gmpq_coerce, 1);
    rb_define_method(cGMP_Q, "num", r_gmpq_num, 0);
    rb_define_method(cGMP_Q, "den", r_gmpq_den, 0);
    rb_define_method(cGMP_Q, "+", r_gmpq_add, 1);
    rb_define_method(cGMP_Q, "-", r_gmpq_sub, 1);
    rb_define_method(cGMP_Q, "*", r_gmpq_mul, 1);
    rb_define_method(cGMP_Q, "/", r_gmpq_div, 1);
    rb_define_method(cGMP_Q, "inv", r_gmpq_inv, 0);
    rb_define_method(cGMP_Q, "inv!", r_gmpq_inv_self, 0);
    rb_define_method(cGMP_Q, "-@", r_gmpq_neg, 0);
    rb_define_method(cGMP_Q, "neg!", r_gmpq_neg_self, 0);
    rb_define_method(cGMP_Q, "abs", r_gmpq_abs, 0);
    rb_define_method(cGMP_Q, "abs!", r_gmpq_abs_self, 0);
    rb_define_method(cGMP_Q, "sgn", r_gmpq_sgn, 0);
    rb_define_method(cGMP_Q, "<=>", r_gmpq_cmp, 1);
    rb_define_method(cGMP_Q, ">",   r_gmpq_cmp_gt, 1);
    rb_define_method(cGMP_Q, ">=",  r_gmpq_cmp_ge, 1);
    rb_define_method(cGMP_Q, "<",   r_gmpq_cmp_lt, 1);
    rb_define_method(cGMP_Q, "<=",  r_gmpq_cmp_le, 1);
    rb_define_method(cGMP_Q, "==",  r_gmpq_eq, 1);
    rb_define_method(cGMP_Q, "swap",  r_gmpq_swap, 1);
    rb_define_method(cGMP_Q, "floor",  r_gmpq_floor, 0);
    rb_define_method(cGMP_Q, "ceil",  r_gmpq_ceil, 0);
    rb_define_method(cGMP_Q, "trunc",  r_gmpq_trunc, 0);
    rb_define_method(cGMP_Q, "to_d",  r_gmpq_to_d, 0);
    rb_define_method(cGMP_Q, "cmpabs",  r_gmpq_cmpabs, 1);

    cGMP_F = rb_define_class_under (mGMP, "F", rb_cNumeric);
    rb_define_singleton_method(cGMP_F, "new", r_gmpfsg_new, -1);
    rb_define_singleton_method(cGMP_F, "default_prec", r_gmpfsg_get_default_prec, 0);
    rb_define_singleton_method(cGMP_F, "default_prec=", r_gmpfsg_set_default_prec, 1);
    rb_define_method(cGMP_F, "initialize", r_gmpf_initialize, -1);
    rb_define_method(cGMP_F, "to_s", r_gmpf_to_s, 0);
    rb_define_method(cGMP_F, "coerce", r_gmpf_coerce, 1); // new method - testing
    rb_define_method(cGMP_F, "+", r_gmpf_add, 1);
    rb_define_method(cGMP_F, "-", r_gmpf_sub, 1);
    rb_define_method(cGMP_F, "*", r_gmpf_mul, 1);
    rb_define_method(cGMP_F, "/", r_gmpf_div, 1);
    rb_define_method(cGMP_F, "-@", r_gmpf_neg, 0);
    rb_define_method(cGMP_F, "neg!", r_gmpf_neg_self, 0);
    rb_define_method(cGMP_F, "abs", r_gmpf_abs, 0);
    rb_define_method(cGMP_F, "abs!", r_gmpf_abs_self, 0);
    rb_define_method(cGMP_F, "sgn", r_gmpf_sgn, 0);
    rb_define_method(cGMP_F, "<=>", r_gmpf_cmp, 1);
    rb_define_method(cGMP_F, ">",   r_gmpf_cmp_gt, 1);
    rb_define_method(cGMP_F, ">=",  r_gmpf_cmp_ge, 1);
    rb_define_method(cGMP_F, "<",   r_gmpf_cmp_lt, 1);
    rb_define_method(cGMP_F, "<=",  r_gmpf_cmp_le, 1);
    rb_define_method(cGMP_F, "==",  r_gmpf_eq, 1);
    rb_define_method(cGMP_F, "floor",  r_gmpf_floor, 0);
    rb_define_method(cGMP_F, "floor!",  r_gmpf_floor_self, 0);
    rb_define_method(cGMP_F, "ceil",  r_gmpf_ceil, 0);
    rb_define_method(cGMP_F, "ceil!",  r_gmpf_ceil_self, 0);
    rb_define_method(cGMP_F, "trunc",  r_gmpf_trunc, 0);
    rb_define_method(cGMP_F, "trunc!",  r_gmpf_trunc_self, 0);
    rb_define_method(cGMP_F, "to_d",  r_gmpf_to_d, 0);
    rb_define_method(cGMP_F, "prec", r_gmpf_get_prec, 0);

/*  rb_define_method(cGMP_F, "cmpabs",  r_gmpf_cmpabs, 1);*/

#ifdef MPFR
    rb_define_method(cGMP_F, "exp", r_gmpfr_exp, 0);
    rb_define_method(cGMP_F, "log", r_gmpfr_log, 0);
    rb_define_method(cGMP_F, "sqrt", r_gmpfr_sqrt, 0);
    rb_define_method(cGMP_F, "cos", r_gmpfr_cos, 0);
    rb_define_method(cGMP_F, "sin", r_gmpfr_sin, 0);
    rb_define_method(cGMP_F, "tan", r_gmpfr_tan, 0);
    rb_define_method(cGMP_F, "acos", r_gmpfr_acos, 0);
    rb_define_method(cGMP_F, "asin", r_gmpfr_asin, 0);
    rb_define_method(cGMP_F, "atan", r_gmpfr_atan, 0);
    rb_define_method(cGMP_F, "cosh", r_gmpfr_cosh, 0);
    rb_define_method(cGMP_F, "sinh", r_gmpfr_sinh, 0);
    rb_define_method(cGMP_F, "tanh", r_gmpfr_tanh, 0);
    rb_define_method(cGMP_F, "acosh", r_gmpfr_acosh, 0);
    rb_define_method(cGMP_F, "asinh", r_gmpfr_asinh, 0);
    rb_define_method(cGMP_F, "atanh", r_gmpfr_atanh, 0);
    rb_define_method(cGMP_F, "log1p", r_gmpfr_log1p, 0);
    rb_define_method(cGMP_F, "expm1", r_gmpfr_expm1, 0);
    rb_define_method(cGMP_F, "log2", r_gmpfr_log2, 0);
    rb_define_method(cGMP_F, "log10", r_gmpfr_log10, 0);

    rb_define_method(cGMP_F, "nan?", r_gmpfr_nan_p, 0);
    rb_define_method(cGMP_F, "infinite?", r_gmpfr_inf_p, 0);
    rb_define_method(cGMP_F, "finite?", r_gmpfr_fin_p, 0);
    rb_define_method(cGMP_F, "number?", r_gmpfr_number_p, 0);

    rb_define_method(cGMP_F, "**", r_gmpfr_pow, 1);
#endif /* MPFR */
    // more

    REGISTER_TAKEOVER(and, "&", "old_and")
    REGISTER_TAKEOVER(or, "|", "old_or")
    REGISTER_TAKEOVER(xor, "^", "old_xor")
    /* takeover cmpabs ? */
}
