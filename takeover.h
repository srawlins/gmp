#define DEFUN_TAKEOVER_LOGIC(fname, mpz_fname, old_fname) \
static VALUE takeover_fixnum_##fname(int argc, VALUE *argv, VALUE self) \
{ \
    MP_INT *res_val, *arg_val; \
    VALUE res; \
\
    if (argc != 1 || !GMPZ_P(argv[0])) { \
	    return rb_funcall2 (self, rb_intern (old_fname), argc, argv); \
    } else { \
        mpz_make_struct(res, res_val); \
        mpz_get_struct(argv[0], arg_val); \
        mpz_init_set_si (res_val, FIX2INT(self)); \
        mpz_fname (res_val, res_val, arg_val); \
        return res; \
    } \
} \
\
static VALUE takeover_bignum_##fname(int argc, VALUE *argv, VALUE self) \
{\
    MP_INT *res_val, *arg_val; \
    VALUE res; \
\
    if (argc != 1 || !GMPZ_P(argv[0])) { \
	return rb_funcall2 (self, rb_intern (old_fname), argc, argv); \
    } else { \
	mpz_get_struct(argv[0], arg_val); \
	mpz_make_struct_init(res, res_val); \
	mpz_set_bignum (res_val, self); \
	mpz_fname (res_val, res_val, arg_val); \
	return res; \
    } \
}

DEFUN_TAKEOVER_LOGIC(and, mpz_and, "old_and")
DEFUN_TAKEOVER_LOGIC(or, mpz_ior, "old_or")
DEFUN_TAKEOVER_LOGIC(xor, mpz_xor, "old_xor")
