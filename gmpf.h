static VALUE r_gmpf_to_d(VALUE self)
{
    MP_FLOAT *self_val;
    mpf_get_struct (self, self_val);

    return rb_float_new(mpf_get_d(self_val));
}

static VALUE r_gmpf_to_s(VALUE self)
{
    MP_FLOAT *self_val;
    char *str, *str2;
    VALUE res;
    mp_exp_t exponent;

    mpf_get_struct(self, self_val);

    str = mpf_get_str(NULL, &exponent, 10, 0, self_val);
    if ((strcmp (str,  "NaN") == 0) ||
	(strcmp (str,  "Inf") == 0) ||
	(strcmp (str, "-Inf") == 0))
    {
	res = rb_str_new2(str);
    }
    else
    {
	if (str[0] == '-')
	    __gmp_asprintf (&str2, "-0.%se%+ld", str+1, exponent);
	else
	    __gmp_asprintf (&str2, "0.%se%+ld", str, exponent);
	res = rb_str_new2(str2);
	free (str2);
    }
    free (str);
    return res;
}

static VALUE r_gmpf_add(VALUE self, VALUE arg)
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

static VALUE r_gmpf_sub(VALUE self, VALUE arg)
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
	mpf_sub(res_val, self_val, arg_val_f);
    } else if (GMPQ_P(arg)) {
	mpq_get_struct (arg, arg_val_q);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_q (res_val, arg_val_q);
	mpf_sub (res_val, self_val, res_val);
    } else if (GMPZ_P(arg)) {
	mpz_get_struct (arg, arg_val_z);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_z (res_val, arg_val_z);
	mpf_sub (res_val, self_val, res_val);
    } else if (FLOAT_P(arg)) {
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_d (res_val, FLT2DBL(arg));
	mpf_sub (res_val, self_val, res_val);
    } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_si (res_val, FIX2INT(arg));
	mpf_sub (res_val, self_val, res_val);
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum(arg_val_z, arg);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_z (res_val, arg_val_z);
	mpf_sub (res_val, res_val, self_val);	
	mpz_temp_free(arg_val_z);
    } else {
	typeerror(ZQFXBD);
    }

    return res;
}

static VALUE r_gmpf_mul(VALUE self, VALUE arg)
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
	mpf_mul(res_val, self_val, arg_val_f);
    } else if (GMPQ_P(arg)) {
	mpq_get_struct (arg, arg_val_q);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_q (res_val, arg_val_q);
	mpf_mul (res_val, self_val, res_val);
    } else if (GMPZ_P(arg)) {
	mpz_get_struct (arg, arg_val_z);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_z (res_val, arg_val_z);
	mpf_mul (res_val, self_val, res_val);
    } else if (FLOAT_P(arg)) {
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_d (res_val, FLT2DBL(arg));
	mpf_mul (res_val, self_val, res_val);
    } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_si (res_val, FIX2INT(arg));
	mpf_mul (res_val, self_val, res_val);
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum(arg_val_z, arg);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_z (res_val, arg_val_z);
	mpf_mul (res_val, res_val, self_val);	
	mpz_temp_free(arg_val_z);
    } else {
	typeerror(ZQFXBD);
    }

    return res;
}

static VALUE r_gmpf_div(VALUE self, VALUE arg)
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
	mpf_div(res_val, self_val, arg_val_f);
    } else if (GMPQ_P(arg)) {
	mpq_get_struct (arg, arg_val_q);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_q (res_val, arg_val_q);
	mpf_div (res_val, self_val, res_val);
    } else if (GMPZ_P(arg)) {
	mpz_get_struct (arg, arg_val_z);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_z (res_val, arg_val_z);
	mpf_div (res_val, self_val, res_val);
    } else if (FLOAT_P(arg)) {
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_d (res_val, FLT2DBL(arg));
	mpf_div (res_val, self_val, res_val);
    } else if (FIXNUM_P(arg)) { // _ui with sign control instead ?
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_si (res_val, FIX2INT(arg));
	mpf_div (res_val, self_val, res_val);
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum(arg_val_z, arg);
	mpf_make_struct_init(res, res_val, prec);
	mpf_set_z (res_val, arg_val_z);
	mpf_div (res_val, res_val, self_val);	
	mpz_temp_free(arg_val_z);
    } else {
	typeerror(ZQFXBD);
    }

    return res;
}

#define DEFUN_FLOAT2FLOAT(fname,mpf_fname) \
static VALUE r_gmpf_##fname(VALUE self) \
{\
    MP_FLOAT *self_val, *res_val; \
    VALUE res; \
    mpf_get_struct(self, self_val); \
    mpf_make_struct_init(res, res_val, mpf_get_prec(self_val)); \
    mpf_fname(res_val, self_val); \
    return res; \
}\
\
static VALUE r_gmpf_##fname##_self(VALUE self) \
{\
    MP_FLOAT *self_val; \
    mpf_get_struct(self, self_val); \
    mpf_fname(self_val, self_val); \
    return Qnil; \
}

DEFUN_FLOAT2FLOAT(abs,mpf_abs)
DEFUN_FLOAT2FLOAT(neg,mpf_neg)
DEFUN_FLOAT2FLOAT(floor,mpf_floor)
DEFUN_FLOAT2FLOAT(trunc,mpf_trunc)
DEFUN_FLOAT2FLOAT(ceil,mpf_ceil)

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
    return (mpf_cmp_value(self_val, arg) == 0)?Qtrue:Qfalse;
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

#define DEFUN_FLOAT_CMP(name,CMP_OP) \
static VALUE r_gmpf_cmp_##name(VALUE self, VALUE arg) \
{ \
    MP_FLOAT *self_val; \
    mpf_get_struct (self,self_val); \
    return (mpf_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}

DEFUN_FLOAT_CMP(lt,<)
DEFUN_FLOAT_CMP(le,<=)
DEFUN_FLOAT_CMP(gt,>)
DEFUN_FLOAT_CMP(ge,>=)

static VALUE r_gmpf_sgn(VALUE self)
{
    MP_FLOAT *self_val;
    mpf_get_struct(self, self_val);
    return INT2FIX(mpf_sgn(self_val));
}

static VALUE r_gmpf_get_prec(VALUE self)
{
    MP_FLOAT *self_val;
    mpf_get_struct(self, self_val);
    return INT2NUM(mpf_get_prec(self_val));
}

#ifdef MPFR
#define MPFR_SINGLE_FUNCTION(name)					\
static VALUE r_gmpfr_##name(VALUE self)					\
{									\
    MP_FLOAT *self_val, *res_val;					\
    unsigned long prec;							\
    VALUE res;								\
									\
    mpf_get_struct_prec(self, self_val, prec);				\
    mpf_make_struct_init(res, res_val, prec);				\
    mpfr_##name(res_val, self_val, __gmp_default_rounding_mode);	\
									\
    return res;								\
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
