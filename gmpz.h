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

static VALUE r_gmpz_add_self(VALUE self, VALUE arg)
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

static VALUE r_gmpz_setbit(VALUE self, VALUE bitnr, VALUE set_to)
{
    MP_INT *self_val;
    int bitnr_val;

    mpz_get_struct(self, self_val);
    
    if (FIXNUM_P(bitnr)) {
	bitnr_val = FIX2INT (bitnr);
    } else {
	typeerror_as(X, "index");
    }
    if (RTEST(set_to)) {
	mpz_setbit (self_val, bitnr_val);
    } else {
	mpz_clrbit (self_val, bitnr_val);
    }
    return Qnil;
}

static VALUE r_gmpz_getbit(VALUE self, VALUE bitnr)
{
    MP_INT *self_val;
    int bitnr_val;
    mpz_get_struct(self, self_val);
    if (FIXNUM_P(bitnr)) {
	bitnr_val = FIX2INT (bitnr);
    } else {
	typeerror_as(X, "index");
    }
    return mpz_tstbit(self_val, bitnr_val)?Qtrue:Qfalse;
}

static VALUE r_gmpz_scan0(VALUE self, VALUE bitnr)
{
    MP_INT *self_val;
    int bitnr_val;
    mpz_get_struct(self, self_val);
    if (FIXNUM_P(bitnr)) {
	bitnr_val = FIX2INT (bitnr);
    } else {
	typeerror_as(X, "index");
    }
    return INT2FIX(mpz_scan0(self_val, bitnr_val));
}

static VALUE r_gmpz_scan1(VALUE self, VALUE bitnr)
{
    MP_INT *self_val;
    int bitnr_val;

    mpz_get_struct(self, self_val);
    
    if (FIXNUM_P(bitnr)) {
	bitnr_val = FIX2INT (bitnr);
    } else {
	typeerror_as(X, "index");
    }

    return INT2FIX(mpz_scan1(self_val, bitnr_val));
}

#define DEFUN_INT_COND_P(fname,mpz_fname) \
static VALUE r_gmpz_##fname(VALUE self) \
{ \
    MP_INT *self_val; \
    mpz_get_struct(self, self_val); \
    return mpz_fname(self_val)?Qtrue:Qfalse; \
}

DEFUN_INT_COND_P(is_even,mpz_even_p)
DEFUN_INT_COND_P(is_odd,mpz_odd_p)
DEFUN_INT_COND_P(is_square,mpz_perfect_square_p)
DEFUN_INT_COND_P(is_power,mpz_perfect_power_p)

static VALUE r_gmpz_sgn(VALUE self)
{
    MP_INT *self_val;
    mpz_get_struct(self, self_val);
    return INT2FIX(mpz_sgn(self_val));
}

static VALUE r_gmpz_powm(VALUE self, VALUE exp, VALUE mod)
{
    MP_INT *self_val, *res_val, *mod_val, *exp_val;
    VALUE res;
    int free_mod_val = 0;

    if (GMPZ_P(mod)) {
	mpz_get_struct(mod, mod_val);
	if (mpz_sgn(mod_val) <= 0) {
	    rb_raise (rb_eRangeError, "modulus must be positive");
	}
    } else if (FIXNUM_P(mod)) {
        if (FIX2INT(mod) <= 0) {
	    rb_raise (rb_eRangeError, "modulus must be positive");
	}
	mpz_temp_alloc (mod_val);
	mpz_init_set_ui(mod_val, FIX2INT(mod));
	free_mod_val = 1;
    } else if (BIGNUM_P(mod)) {
	mpz_temp_from_bignum (mod_val, mod);
	if (mpz_sgn(mod_val) <= 0) {
	    mpz_temp_free(mod_val);
	    rb_raise (rb_eRangeError, "modulus must be positive");
	}
	free_mod_val = 1;
    } else {
	typeerror_as (ZXB, "modulus");
    }
    mpz_make_struct_init(res, res_val);
    mpz_get_struct(self, self_val);

    if (GMPZ_P(exp)) {
        mpz_get_struct(exp, exp_val);
	if (mpz_sgn(mod_val) < 0) {
	    rb_raise (rb_eRangeError, "exponent must be nonnegative");
	}
	mpz_powm (res_val, self_val, exp_val, mod_val);
    } else if (FIXNUM_P(exp)) {
	if (FIX2INT(exp) < 0)
	{
	    if (free_mod_val)
		mpz_temp_free(mod_val);
	    rb_raise (rb_eRangeError, "exponent must be nonnegative");
	}
	mpz_powm_ui (res_val, self_val, FIX2INT(exp), mod_val);
    } else if (BIGNUM_P(exp)) {
	mpz_temp_from_bignum (exp_val, exp);
	mpz_powm (res_val, self_val, exp_val, mod_val);
	mpz_temp_free (exp_val);
    } else {
	if (free_mod_val)
	    mpz_temp_free(mod_val);
	typeerror_as (ZXB, "exponent");
    }
    if (free_mod_val)
        mpz_temp_free(mod_val);
    return res;
}

static VALUE r_gmpz_swap(VALUE self, VALUE arg)
{
    MP_INT *self_val, *arg_val;
    if (!GMPZ_P(arg)) {
	rb_raise(rb_eTypeError, "Can't swap GMP::Z with object of other class");
    }
    mpz_get_struct(self, self_val);
    mpz_get_struct(arg, arg_val);
    mpz_swap(self_val,arg_val);
    return Qnil;
}


#define DEFUN_INT_F_UL(fname,mpz_fname,argname) \
static VALUE r_gmpz_##fname(VALUE self, VALUE exp) \
{ \
    MP_INT *self_val, *res_val; \
    VALUE res; \
    unsigned long exp_val; \
 \
    if (FIXNUM_P(exp)) { \
	if (FIX2INT (exp) < 0) \
	    rb_raise (rb_eRangeError, argname " out of range"); \
	exp_val = FIX2INT (exp); \
    } else if (GMPZ_P(exp)) {\
	mpz_get_struct (exp, res_val); \
	if (!mpz_fits_ulong_p (res_val)) \
	    rb_raise (rb_eRangeError, argname " out of range"); \
	exp_val = mpz_get_ui (res_val); \
	if (exp_val == 0) \
	    rb_raise (rb_eRangeError, argname " out of range"); \
    } else { \
	typeerror_as (ZX, argname); \
    } \
 \
    mpz_make_struct_init(res, res_val); \
    mpz_get_struct(self, self_val); \
    mpz_fname(res_val, self_val, exp_val); \
 \
    return res; \
}

DEFUN_INT_F_UL(pow,mpz_pow_ui,"exponent")
DEFUN_INT_F_UL(shl,mpz_mul_2exp,"shift size")
DEFUN_INT_F_UL(fshr,mpz_fdiv_q_2exp,"shift size")
DEFUN_INT_F_UL(tshr,mpz_tdiv_q_2exp,"shift size")
DEFUN_INT_F_UL(fshrm,mpz_fdiv_r_2exp,"mark size")
DEFUN_INT_F_UL(tshrm,mpz_tdiv_r_2exp,"mark size")
DEFUN_INT_F_UL(root,mpz_root,"root number")

static int mpz_cmp_value (MP_INT *OP, VALUE arg)
{
    MP_RAT *arg_val_q;
    MP_INT *arg_val_z;
    int res;

    if (GMPZ_P(arg)) {
	mpz_get_struct(arg,arg_val_z);
	return mpz_cmp (OP,arg_val_z);
    } else if (FIXNUM_P(arg)) {
	return mpz_cmp_si (OP, FIX2INT(arg));
    } else if (GMPQ_P(arg)) {
	mpq_get_struct(arg,arg_val_q);
	mpz_temp_alloc (arg_val_z);
	mpz_init(arg_val_z);
	mpz_mul(arg_val_z, OP, mpq_denref(arg_val_q));
	res = mpz_cmp (arg_val_z, mpq_numref(arg_val_q));
	mpz_temp_free(arg_val_z);
	return res;
    } else if (GMPF_P(arg)) {
        not_yet;
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum (arg_val_z, arg);
	res = mpz_cmp (OP, arg_val_z);
	mpz_temp_free(arg_val_z);
	return res;
    } else {
	typeerror_as (ZQFXB, "exponent");
    }
}

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

static VALUE r_gmpz_cmp(VALUE self, VALUE arg)
{
    MP_INT *self_val;
    int res;
    mpz_get_struct (self,self_val);
    res = mpz_cmp_value(self_val, arg);
    if (res > 0)
	return INT2FIX(1);
    else if (res == 0)
	return INT2FIX(0);
    else
	return INT2FIX(-1);
}

#define DEFUN_INT_CMP(name,CMP_OP) \
static VALUE r_gmpz_cmp_##name(VALUE self, VALUE arg) \
{ \
    MP_INT *self_val; \
    mpz_get_struct (self,self_val); \
    return (mpz_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}

DEFUN_INT_CMP(lt,<)
DEFUN_INT_CMP(le,<=)
DEFUN_INT_CMP(gt,>)
DEFUN_INT_CMP(ge,>=)

#define DEFUN_INT_DIV(fname,gmp_fname) \
static VALUE r_gmpz_##fname(VALUE self, VALUE arg) \
{ \
    MP_INT *self_val, *arg_val, *res_val; \
    VALUE res; \
    int arg_val_i; \
    \
    mpz_get_struct(self, self_val); \
    mpz_make_struct_init(res, res_val); \
    \
    if (GMPZ_P(arg)) { \
        mpz_get_struct(arg,arg_val); \
	if (mpz_cmp_ui(arg_val, 0) == 0) \
	    rb_raise (rb_eZeroDivError, "divided by 0"); \
	gmp_fname (res_val, self_val, arg_val); \
    } else if (FIXNUM_P(arg)) { \
	arg_val_i = FIX2INT(arg); \
	if (arg_val_i > 0) { \
	    gmp_fname##_ui (res_val, self_val, arg_val_i); \
	} else if (arg_val_i == 0) { \
	    rb_raise (rb_eZeroDivError, "divided by 0"); \
	} else { \
	    mpz_neg (res_val, self_val); \
	    gmp_fname##_ui (res_val, self_val, -arg_val_i); \
	} \
    } else if (BIGNUM_P(arg)) { \
	mpz_set_bignum (res_val, arg); \
	if (mpz_cmp_ui(res_val, 0) == 0) \
	    rb_raise (rb_eZeroDivError, "divided by 0"); \
	gmp_fname (res_val, self_val, res_val); \
    } else { \
	typeerror(ZXB); \
    } \
    return res; \
}

DEFUN_INT_DIV(tdiv, mpz_tdiv_q)
DEFUN_INT_DIV(tmod, mpz_tdiv_r)
DEFUN_INT_DIV(fdiv, mpz_fdiv_q)
DEFUN_INT_DIV(fmod, mpz_fdiv_r)
DEFUN_INT_DIV(cdiv, mpz_cdiv_q)
DEFUN_INT_DIV(cmod, mpz_cdiv_r)

#define DEFUN_INT2INT(fname,mpz_fname) \
static VALUE r_gmpz_##fname(VALUE self) \
{\
    MP_INT *self_val, *res_val; \
    VALUE res; \
    mpz_get_struct(self, self_val); \
    mpz_make_struct_init(res, res_val); \
    mpz_fname(res_val, self_val); \
    return res; \
}\
\
static VALUE r_gmpz_##fname##_self(VALUE self) \
{\
    MP_INT *self_val; \
    mpz_get_struct(self, self_val); \
    mpz_fname(self_val, self_val); \
    return Qnil; \
}

DEFUN_INT2INT(abs, mpz_abs)
DEFUN_INT2INT(neg, mpz_neg)
DEFUN_INT2INT(com, mpz_com)
DEFUN_INT2INT(sqrt, mpz_sqrt)
DEFUN_INT2INT(nextprime, mpz_nextprime)

static VALUE r_gmpz_jacobi(VALUE self)
{
    MP_INT *self_val, *res_val;
    VALUE res;
    mpz_get_struct(self, self_val);
    if (mpz_sgn(self_val) != 1)
	rb_raise(rb_eRangeError, "you can take jacobi symbol only of positive value");
    if (mpz_even_p(self_val))
	rb_raise(rb_eRangeError, "you can't take jacobi symbol of even value");
    mpz_make_struct_init(res, res_val);
    mpz_jacobi(res_val, self_val);
    return res;
}

static VALUE r_gmpz_legendre(VALUE self)
{
    MP_INT *self_val, *res_val;
    VALUE res;
    mpz_get_struct(self, self_val);
    if (mpz_sgn(self_val) != 1)
	rb_raise(rb_eRangeError, "you can take legendre symbol only of positive value");
    if (mpz_even_p(self_val))
	rb_raise(rb_eRangeError, "you can't take legendre symbol of even value");
    mpz_make_struct_init(res, res_val);
    mpz_legendre(res_val, self_val);
    return res;
}

#define DEFUN_INT_LOGIC(fname, mpz_fname) \
static VALUE r_gmpz_##fname(VALUE self, VALUE arg) \
{ \
    MP_INT *self_val, *arg_val, *res_val; \
    VALUE res; \
 \
    mpz_get_struct(self, self_val); \
 \
    mpz_make_struct(res, res_val); \
    if (GMPZ_P(arg)) { \
        mpz_get_struct(arg,arg_val); \
        mpz_init (res_val); \
	mpz_fname (res_val, self_val, arg_val); \
    } else if (FIXNUM_P(arg)) { \
        mpz_init_set_si (res_val, FIX2INT(arg)); \
	mpz_fname (res_val, self_val, res_val); \
    } else if (BIGNUM_P(arg)) { \
        mpz_init (res_val); \
	mpz_set_bignum (res_val, arg); \
        mpz_fname (res_val, self_val, res_val); \
    } else  { \
	typeerror(ZXB); \
    } \
    return res; \
}

DEFUN_INT_LOGIC(and, mpz_and)
DEFUN_INT_LOGIC(xor, mpz_xor)
DEFUN_INT_LOGIC(or, mpz_ior)

static VALUE r_gmpz_sqrtrem(VALUE self)
{
    MP_INT *self_val, *sqrt_val, *rem_val;
    VALUE sqrt, rem;

    mpz_get_struct (self, self_val);
    mpz_make_struct_init(sqrt, sqrt_val);
    mpz_make_struct_init(rem, rem_val);
    mpz_sqrtrem (sqrt_val, rem_val, self_val);
    return rb_assoc_new(sqrt, rem);
}

static VALUE r_gmpz_to_d(VALUE self)
{
    MP_INT *self_val;
    mpz_get_struct (self, self_val);

    return rb_float_new(mpz_get_d(self_val));
}

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

static VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp)
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

static VALUE r_gmpz_remove(VALUE self, VALUE arg)
{
    MP_INT *self_val, *arg_val, *res_val;
    VALUE res;
    int free_arg_val = 0;

    mpz_get_struct(self,self_val);

    if (GMPZ_P(arg)) {
        mpz_get_struct(arg,arg_val);
	if (mpz_sgn(arg_val) != 1)
	    rb_raise(rb_eRangeError, "argument must be positive");
    } else if (FIXNUM_P(arg)) {
	if (FIX2INT(arg) <= 0)
	    rb_raise(rb_eRangeError, "argument must be positive");
	mpz_temp_alloc(arg_val);
	mpz_init_set_ui(arg_val, FIX2INT(arg));
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum(arg_val, arg);
	if (mpz_sgn(arg_val) != 1)
	{
	    mpz_temp_free (arg_val);
	    rb_raise(rb_eRangeError, "argument must be positive");
	}
    } else {
	typeerror(ZXB);
    }

    mpz_make_struct_init (res, res_val);
    mpz_remove (res_val, self_val, arg_val);
    
    if (free_arg_val)
	mpz_temp_free(arg_val);
    
    return res;
}

static VALUE r_gmpz_to_i (VALUE self)
{
    MP_INT *self_val;
    char *str;
    VALUE res;

    mpz_get_struct(self, self_val);
    if (mpz_fits_slong_p(self_val))
	return rb_int2inum(mpz_get_si(self_val));
    str = mpz_get_str(NULL, 0, self_val);
    res = rb_cstr2inum (str, 10);
    free (str);
    return res;
}

static VALUE r_gmpz_cmpabs (VALUE self, VALUE arg)
{
    MP_INT *arg_val_z, *self_val;
    MP_RAT *arg_val_q;
    int res;

    mpz_get_struct (self, self_val);

    if (GMPZ_P(arg)) {
	mpz_get_struct(arg,arg_val_z);
	return INT2FIX(mpz_cmpabs (self_val, arg_val_z));
    } else if (FIXNUM_P(arg)) {
	if (FIX2INT(arg) >= 0)
	    return INT2FIX(mpz_cmpabs_ui (self_val, FIX2INT(arg)));
	else
	    return INT2FIX(mpz_cmpabs_ui (self_val, -FIX2INT(arg)));
    } else if (GMPQ_P(arg)) {
	mpq_get_struct(arg,arg_val_q);
	mpz_temp_alloc (arg_val_z);
	mpz_init(arg_val_z);
	mpz_mul(arg_val_z, self_val, mpq_denref(arg_val_q));
	res = mpz_cmpabs (arg_val_z, mpq_numref(arg_val_q));
	mpz_temp_free(arg_val_z);
	return INT2FIX(res);
    } else if (GMPF_P(arg)) {
    	not_yet;
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum (arg_val_z, arg);
	res = mpz_cmpabs (self_val, arg_val_z);
	mpz_temp_free(arg_val_z);
	return INT2FIX(res);
    } else {
	typeerror(ZQFXB);
    }
}

static VALUE r_gmpz_to_s(VALUE self)
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
