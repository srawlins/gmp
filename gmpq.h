static VALUE r_gmpq_add(VALUE self, VALUE arg)
{
    MP_RAT *self_val, *arg_val_q, *res_val;
    MP_INT *arg_val_z, *res_val_num;
    VALUE res;
    
    mpq_get_struct(self, self_val);
    mpq_make_struct_init(res, res_val);

    if (GMPQ_P(arg)) {
        mpq_get_struct(arg,arg_val_q);
	mpq_add (res_val, self_val, arg_val_q);
    } else if (GMPZ_P(arg)) {
	res_val_num = mpq_numref(res_val);
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_get_struct(arg, arg_val_z);
	mpz_mul (res_val_num, mpq_denref(self_val), arg_val_z);
	mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
    } else if (FIXNUM_P(arg)) {
	res_val_num = mpq_numref(res_val);
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_mul_si (res_val_num, mpq_denref(self_val), FIX2INT(arg));
	mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
    } else if (GMPF_P(arg)) {
	return r_gmpf_add(arg,self);
    } else if (BIGNUM_P(arg)) {
	res_val_num = mpq_numref(res_val);
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_set_bignum (res_val_num, arg);
	mpz_mul (res_val_num, res_val_num, mpq_denref(self_val));
	mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
    } else {
	typeerror (ZQFXB);
    }
    return res;
}

static VALUE r_gmpq_sub(VALUE self, VALUE arg)
{
    MP_RAT *self_val, *arg_val_q, *res_val;
    MP_INT *arg_val_z, *res_val_num;
    MP_FLOAT *arg_val_f, *res_val_f;
    VALUE res;
    unsigned int prec;

    mpq_get_struct(self, self_val);
    mpq_make_struct_init(res, res_val);

    if (GMPQ_P(arg)) {
        mpq_get_struct(arg,arg_val_q);
	mpq_sub (res_val, self_val, arg_val_q);
    } else if (GMPZ_P(arg)) {
	res_val_num = mpq_numref(res_val);
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_get_struct(arg, arg_val_z);
	mpz_mul (res_val_num, mpq_denref(self_val), arg_val_z);
	mpz_neg (res_val_num, res_val_num);
	mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
    } else if (FIXNUM_P(arg)) {
	res_val_num = mpq_numref(res_val);
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_mul_si (res_val_num, mpq_denref(self_val), -FIX2INT(arg));
	mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
    } else if (GMPF_P(arg)) {
	mpf_get_struct_prec (arg, arg_val_f, prec);
	mpf_make_struct_init(res, res_val_f, prec);
	mpf_set_q (res_val_f, self_val);
	mpf_sub (res_val_f, res_val_f, arg_val_f);
    } else if (BIGNUM_P(arg)) {
	res_val_num = mpq_numref(res_val);
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_set_bignum (res_val_num, arg);
	mpz_mul (res_val_num, res_val_num, mpq_denref(self_val));
	mpz_neg (res_val_num, res_val_num);
	mpz_add (res_val_num, res_val_num, mpq_numref(self_val));
    } else {
	typeerror (ZQFXB);
    }
    return res;
}

static VALUE r_gmpq_mul(VALUE self, VALUE arg)
{
    MP_RAT *self_val, *arg_val_q, *res_val;
    MP_INT *arg_val_z, *tmp_z;
    VALUE res;
#if GMP >= 4
    unsigned long tmp_ui;
#endif

    mpq_get_struct(self, self_val);
    mpq_make_struct_init(res, res_val);

    if (GMPQ_P(arg)) {
        mpq_get_struct(arg,arg_val_q);
	mpq_mul (res_val, self_val, arg_val_q);
    } else if (GMPZ_P(arg)) {
        mpz_get_struct(arg,arg_val_z);
        mpz_temp_init(tmp_z);
	mpz_gcd (tmp_z, mpq_denref(self_val), arg_val_z);
	mpz_divexact (mpq_denref(res_val), mpq_denref(self_val), tmp_z);
        mpz_divexact (mpq_numref(res_val), arg_val_z, tmp_z);
	mpz_mul (mpq_numref(res_val), mpq_numref(res_val), mpq_numref(self_val));
	mpz_temp_free(tmp_z);
    } else if (FIXNUM_P(arg)) {
#if GMP >= 4
	if (FIX2INT(arg) > 0) {
	    tmp_ui = mpz_gcd_ui (0, mpq_denref(self_val), FIX2INT(arg));
	} else if (FIX2INT(arg) < 0) {
	    tmp_ui = mpz_gcd_ui (0, mpq_denref(self_val), -FIX2INT(arg));
	} else {
	    mpz_set_ui(mpq_numref(res_val), 0);
	    mpz_set_ui(mpq_denref(res_val), 1);
	    return res;
	}
	mpz_divexact_ui (mpq_denref(res_val), mpq_denref(self_val), tmp_ui);
        mpz_mul_ui (mpq_numref(res_val), mpq_numref(self_val), FIX2INT(arg)/tmp_ui);
#else
	mpz_set (mpq_denref(res_val), mpq_denref(self_val));
	mpz_mul_si (mpq_numref(res_val), mpq_numref(self_val), FIX2INT(arg));
	mpq_canonicalize (res_val);
#endif
    } else if (GMPF_P(arg)) {
	return r_gmpf_mul (arg, self);
    } else if (BIGNUM_P(arg)) {
        mpz_temp_alloc(tmp_z);
	mpz_set_bignum(tmp_z, arg);
	mpz_gcd(mpq_denref(res_val), mpq_denref(self_val), tmp_z);
	mpz_divexact(mpq_numref(res_val), tmp_z, mpq_denref(res_val));
	mpz_divexact(mpq_denref(res_val), mpq_denref(self_val), mpq_denref(res_val));
	mpz_mul(mpq_numref(res_val), mpq_numref(res_val), mpq_numref(self_val));
	mpz_temp_free(tmp_z);
    } else {
	typeerror (ZQFXB);
    }
    return res;
}

static VALUE r_gmpq_div(VALUE self, VALUE arg)
{
    MP_RAT *self_val, *arg_val_q, *res_val;
    MP_INT *arg_val_z, *tmp_z;
    MP_FLOAT *arg_val_f, *res_val_f;
    VALUE res;
    unsigned long tmp_ui, prec;

    mpq_get_struct(self, self_val);
    mpq_make_struct_init(res, res_val);

    if (GMPQ_P(arg)) {
        mpq_get_struct(arg,arg_val_q);
	if (mpz_sgn(mpq_numref(arg_val_q)) == 0)
	    rb_raise (rb_eZeroDivError, "divided by 0");
	mpq_div (res_val, self_val, arg_val_q);
    } else if (GMPZ_P(arg)) {
        mpz_get_struct(arg,arg_val_z);
        mpz_temp_init(tmp_z);
	mpz_gcd (tmp_z, mpq_numref(self_val), arg_val_z);
	mpz_divexact (mpq_numref(res_val), mpq_numref(self_val), tmp_z);
        mpz_divexact (mpq_denref(res_val), arg_val_z, tmp_z);
	mpz_mul (mpq_denref(res_val), mpq_denref(res_val), mpq_denref(self_val));
	mpz_temp_free(tmp_z);
    } else if (FIXNUM_P(arg)) {
	if (FIX2INT(arg) == 0)
	    rb_raise (rb_eZeroDivError, "divided by 0");
	if (FIX2INT(arg) > 0) {
	    tmp_ui = mpz_gcd_ui (0, mpq_numref(self_val), FIX2INT(arg));
	} else {
	    tmp_ui = mpz_gcd_ui (0, mpq_numref(self_val), -FIX2INT(arg));
	}
	mpz_divexact_ui (mpq_numref(res_val), mpq_numref(self_val), tmp_ui);
        mpz_mul_ui (mpq_denref(res_val), mpq_denref(self_val), FIX2INT(arg)/tmp_ui);
    } else if (GMPF_P(arg)) {
	mpf_get_struct_prec (arg, arg_val_f, prec);
	mpf_make_struct_init(res, res_val_f, prec);
	mpf_set_q (res_val_f, self_val);
	mpf_div (res_val_f, res_val_f, arg_val_f);
    } else if (BIGNUM_P(arg)) {
        mpz_temp_alloc(tmp_z);
	mpz_set_bignum(tmp_z, arg);
	mpz_gcd(mpq_numref(res_val), mpq_numref(self_val), tmp_z);
	mpz_divexact(mpq_denref(res_val), tmp_z, mpq_numref(res_val));
	mpz_divexact(mpq_numref(res_val), mpq_numref(self_val), mpq_numref(res_val));
	mpz_mul(mpq_denref(res_val), mpq_denref(res_val), mpq_denref(self_val));
	mpz_temp_free(tmp_z);
    } else {
	typeerror (ZQFXB);
    }
    return res;
}

static VALUE r_gmpq_neg(VALUE self)
{
    MP_RAT *self_val, *res_val;
    VALUE res;
    mpq_get_struct(self, self_val);
    mpq_make_struct_init(res, res_val);
    mpq_neg (res_val, self_val);
    return res;
}

static VALUE r_gmpq_neg_self(VALUE self)
{
    MP_RAT *self_val;    
    mpq_get_struct(self, self_val);
    mpz_neg (mpq_numref(self_val), mpq_numref(self_val));
    return Qnil;
}

static VALUE r_gmpq_abs(VALUE self)
{
    MP_RAT *self_val, *res_val;
    VALUE res;
    
    mpq_get_struct(self, self_val);
    mpq_make_struct_init(res, res_val);
    mpz_abs (mpq_numref(res_val), mpq_numref(self_val));
    mpz_set (mpq_denref(res_val), mpq_denref(self_val));

    return res;
}

static VALUE r_gmpq_abs_self(VALUE self)
{
    MP_RAT *self_val;    
    mpq_get_struct(self, self_val);
    mpz_abs (mpq_numref(self_val), mpq_numref(self_val));
    return Qnil;
}

static VALUE r_gmpq_inv(VALUE self)
{
    MP_RAT *self_val, *res_val;
    VALUE res;

    mpq_get_struct(self, self_val);
    if (mpq_sgn(self_val) == 0)
	rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init(res, res_val);
    mpq_inv (res_val, self_val);
    
    return res;
}

static VALUE r_gmpq_inv_self(VALUE self)
{
    MP_RAT *self_val;
    mpq_get_struct(self, self_val);
    if (mpq_sgn(self_val) == 0)
	rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_inv (self_val, self_val);
    return Qnil;
}

static VALUE r_gmpq_sgn(VALUE self)
{
    MP_RAT *self_val;
    mpq_get_struct(self, self_val);
    return INT2FIX(mpq_sgn(self_val));
}

static int mpq_cmp_value (MP_RAT *OP, VALUE arg)
{
    MP_INT *arg_val_z, *tmp_z;
    MP_RAT *arg_val_q;
    int res;

    if (GMPQ_P(arg)) {
	mpq_get_struct(arg,arg_val_q);
	return mpq_cmp (OP,arg_val_q);
    } else if (GMPZ_P(arg)) {
	mpz_get_struct(arg, arg_val_z);
	mpz_temp_alloc (tmp_z);
	mpz_init (tmp_z);
	mpz_mul (tmp_z, mpq_denref(OP), arg_val_z);
	res = mpz_cmp (mpq_numref(OP),tmp_z);
	mpz_temp_free (tmp_z);
	return res;
    } else if (FIXNUM_P(arg)) {
	mpz_temp_alloc (tmp_z);
	mpz_init (tmp_z);
	mpz_mul_si (tmp_z, mpq_denref(OP), FIX2INT(arg));
	res = mpz_cmp (mpq_numref(OP), tmp_z);
	mpz_temp_free (tmp_z);
	return res;
    } else if (GMPF_P(arg)) {
	not_yet;
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum (tmp_z, arg);
	mpz_mul (tmp_z, tmp_z, mpq_denref(OP));
	res = mpz_cmp (mpq_numref(OP), tmp_z);
	mpz_temp_free (tmp_z);
	return res;
    } else {
	typeerror (ZQFXB);
    }
}

static VALUE r_gmpq_eq(VALUE self, VALUE arg)
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

static VALUE r_gmpq_cmp(VALUE self, VALUE arg)
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

#define DEFUN_RAT_CMP(name,CMP_OP) \
static VALUE r_gmpq_cmp_##name(VALUE self, VALUE arg) \
{ \
    MP_RAT *self_val; \
    mpq_get_struct (self,self_val); \
    return (mpq_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}

DEFUN_RAT_CMP(lt,<)
DEFUN_RAT_CMP(le,<=)
DEFUN_RAT_CMP(gt,>)
DEFUN_RAT_CMP(ge,>=)

static VALUE r_gmpq_swap(VALUE self, VALUE arg)
{
    MP_RAT *self_val, *arg_val;

    if (!GMPQ_P(arg)) {
	rb_raise(rb_eTypeError, "Can't swap GMP::Q with object of other class");
    }

    mpq_get_struct(self, self_val);
    mpq_get_struct(arg, arg_val);
    mpq_swap(self_val,arg_val);

    return Qnil;
}

#define DEFUN_RAT2INT(fname,mpz_fname) \
static VALUE r_gmpq_##fname(VALUE self) \
{ \
    MP_RAT *self_val; \
    MP_INT *res_val; \
    VALUE res; \
 \
    mpq_get_struct(self, self_val); \
    mpz_make_struct_init (res, res_val) \
    mpz_fname (res_val, mpq_numref(self_val), mpq_denref(self_val)); \
    return res; \
}

DEFUN_RAT2INT(floor,mpz_fdiv_q)
DEFUN_RAT2INT(trunc,mpz_tdiv_q)
DEFUN_RAT2INT(ceil,mpz_cdiv_q)

static VALUE r_gmpq_to_d(VALUE self)
{
    MP_RAT *self_val;
    mpq_get_struct (self, self_val);

    return rb_float_new(mpq_get_d(self_val));
}

static VALUE r_gmpq_cmpabs (VALUE self, VALUE arg)
{
    MP_RAT *arg_val_q, *self_val;
    MP_INT *arg_val_z, *tmp_z;
    int res;
    int sgnt;

    mpq_get_struct(self, self_val);

    if (GMPQ_P(arg)) {
	mpq_get_struct(arg,arg_val_q);
	sgnt = 3*mpz_sgn(mpq_numref(self_val)) + mpz_sgn(mpq_numref(arg_val_q));
	switch (sgnt)
	{
	default:
	case 0:
	    return INT2FIX(0);
	case 1:
	case -1:
	    return INT2FIX(-1);
	case 2:
	    tmp_z = mpq_numref(arg_val_q);
	    mpz_neg (tmp_z, tmp_z);
	    res = mpq_cmp (self_val, arg_val_q);
	    mpz_neg (tmp_z, tmp_z);
	    return res;
	case -2:
	    tmp_z = mpq_numref(arg_val_q);
	    mpz_neg (tmp_z, tmp_z);
	    res = mpq_cmp (self_val, arg_val_q);
	    mpz_neg (tmp_z, tmp_z);
	    return res;
	case 3:
	case -3:
	    return INT2FIX(1);
	case 4:
	case -4:
	    return INT2FIX(mpq_cmp (self_val,arg_val_q));
	}
    } else if (GMPZ_P(arg)) {
	mpz_get_struct(arg, arg_val_z);
	mpz_temp_alloc (tmp_z);
	mpz_init (tmp_z);
	mpz_mul (tmp_z, mpq_denref(self_val), arg_val_z);
	res = mpz_cmpabs (mpq_numref(self_val),tmp_z);
	mpz_temp_free (tmp_z);
	return res;
    } else if (FIXNUM_P(arg)) {
	mpz_temp_alloc (tmp_z);
	mpz_init (tmp_z);
	mpz_mul_si (tmp_z, mpq_denref(self_val), FIX2INT(arg));
	res = mpz_cmpabs (mpq_numref(self_val), tmp_z);
	mpz_temp_free (tmp_z);
	return res;
    } else if (GMPF_P(arg)) {
	not_yet;
    } else if (BIGNUM_P(arg)) {
	mpz_temp_from_bignum (tmp_z, arg);
	mpz_mul (tmp_z, tmp_z, mpq_denref(self_val));
	res = mpz_cmpabs (mpq_numref(self_val), tmp_z);
	mpz_temp_free (tmp_z);
	return res;
    } else {
	typeerror (ZQFXB);
    }
}

static VALUE r_gmpq_num(VALUE self)
{
    MP_RAT *self_val;
    MP_INT *res_val;
    VALUE res;
    mpq_get_struct(self,self_val);
    mpz_make_struct(res, res_val);
    mpz_init_set (res_val, mpq_numref (self_val));
    return res;
}

static VALUE r_gmpq_den(VALUE self)
{
    MP_RAT *self_val;
    MP_INT *res_val;
    VALUE res;
    mpq_get_struct(self,self_val);
    mpz_make_struct(res, res_val);
    mpz_init_set (res_val, mpq_denref (self_val));
    return res;
}
