#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

/*
 * Document-class: GMP::Z
 *
 * GMP Multiple Precision Integer.
 *
 * Instances of this class can store variables of the type mpz_t. This class
 * also contains many methods that act as the functions for mpz_t variables,
 * as well as a few methods that attempt to make this library more Ruby-ish.
 */

/**********************************************************************
 *    Macros                                                          *
 **********************************************************************/

/*
 * DEFUN_INT2INT defines two functions. The first takes a GMP::Z as
 * self, calls mpz_fname on the contained mpz_t, whose arguments are
 * exactly (0) the return argument and (1) self. The second is the same
 * destructive method.
 */
#define DEFUN_INT2INT(fname,mpz_fname)         \
static VALUE r_gmpz_##fname(VALUE self)        \
{                                              \
  MP_INT *self_val, *res_val;                  \
  VALUE res;                                   \
  mpz_get_struct(self, self_val);              \
  mpz_make_struct_init(res, res_val);          \
  mpz_fname(res_val, self_val);                \
  return res;                                  \
}                                              \
                                               \
static VALUE r_gmpz_##fname##_self(VALUE self) \
{                                              \
  MP_INT *self_val;                            \
  mpz_get_struct(self, self_val);              \
  mpz_fname(self_val, self_val);               \
  return self;                                 \
}

/*
 * DEFUNN_INT_F_UL defines a function that takes a GMP::Z as self,
 * and a FIXNUM or GMP::Z as exp. It calls mpz_fname on the contained
 * mpz_t, whose arguments are (0) the return argument, (1) self, and
 * (2) exp_value. exp must fit into a ulong.
 */
#define DEFUN_INT_F_UL(fname,mpz_fname,argname)          \
static VALUE r_gmpz_##fname(VALUE self, VALUE exp)       \
{                                                        \
  MP_INT *self_val, *res_val;                            \
  VALUE res;                                             \
  unsigned long exp_val;                                 \
                                                         \
  if (FIXNUM_P(exp)) {                                   \
    if (FIX2NUM(exp) < 0)                                \
      rb_raise(rb_eRangeError, argname " out of range"); \
    exp_val = FIX2NUM(exp);                              \
  } else if (GMPZ_P(exp)) {                              \
    mpz_get_struct(exp, res_val);                        \
    if (!mpz_fits_ulong_p(res_val))                      \
      rb_raise(rb_eRangeError, argname " out of range"); \
    exp_val = mpz_get_ui(res_val);                       \
    if (exp_val == 0)                                    \
      rb_raise(rb_eRangeError, argname " out of range"); \
  } else {                                               \
    typeerror_as(ZX, argname);                           \
  }                                                      \
                                                         \
  mpz_make_struct_init(res, res_val);                    \
  mpz_get_struct(self, self_val);                        \
  mpz_fname(res_val, self_val, exp_val);                 \
                                                         \
  return res;                                            \
}

#define DEFUN_INT_CMP(name,CMP_OP)                             \
static VALUE r_gmpz_cmp_##name(VALUE self, VALUE arg)          \
{                                                              \
  MP_INT *self_val;                                            \
  mpz_get_struct(self,self_val);                               \
  return (mpz_cmp_value(self_val, arg) CMP_OP 0)?Qtrue:Qfalse; \
}

#define DEFUN_INT_DIV(fname,gmp_fname)                \
static VALUE r_gmpz_##fname(VALUE self, VALUE arg)    \
{                                                     \
  MP_INT *self_val, *arg_val, *res_val;               \
  VALUE res;                                          \
  long arg_val_i;                                     \
                                                      \
  mpz_get_struct(self, self_val);                     \
  mpz_make_struct_init(res, res_val);                 \
                                                      \
  if (GMPZ_P(arg)) {                                  \
    mpz_get_struct(arg,arg_val);                      \
    if (mpz_cmp_ui(arg_val, 0) == 0)                  \
      rb_raise (rb_eZeroDivError, "divided by 0");    \
    gmp_fname (res_val, self_val, arg_val);           \
  } else if (FIXNUM_P(arg)) {                         \
    arg_val_i = FIX2NUM(arg);                         \
    if (arg_val_i > 0) {                              \
      gmp_fname##_ui (res_val, self_val, arg_val_i);  \
    } else if (arg_val_i == 0) {                      \
      rb_raise (rb_eZeroDivError, "divided by 0");    \
    } else {                                          \
      mpz_neg (res_val, self_val);                    \
      gmp_fname##_ui (res_val, self_val, -arg_val_i); \
    }                                                 \
  } else if (BIGNUM_P(arg)) {                         \
    mpz_set_bignum (res_val, arg);                    \
    if (mpz_cmp_ui(res_val, 0) == 0)                  \
      rb_raise (rb_eZeroDivError, "divided by 0");    \
    gmp_fname (res_val, self_val, res_val);           \
  } else {                                            \
    typeerror(ZXB);                                   \
  }                                                   \
  return res;                                         \
}

#define DEFUN_INT_LOGIC(fname, mpz_fname)          \
static VALUE r_gmpz_##fname(VALUE self, VALUE arg) \
{                                                  \
  MP_INT *self_val, *arg_val, *res_val;            \
  VALUE res;                                       \
                                                   \
  mpz_get_struct (self, self_val);                 \
                                                   \
  mpz_make_struct (res, res_val);                  \
  if (GMPZ_P (arg)) {                              \
    mpz_get_struct (arg,arg_val);                  \
    mpz_init (res_val);                            \
    mpz_fname (res_val, self_val, arg_val);        \
  } else if (FIXNUM_P (arg)) {                     \
    mpz_init_set_si (res_val, FIX2NUM (arg));      \
    mpz_fname (res_val, self_val, res_val);        \
  } else if (BIGNUM_P (arg)) {                     \
    mpz_init (res_val);                            \
    mpz_set_bignum (res_val, arg);                 \
    mpz_fname (res_val, self_val, res_val);        \
  } else  {                                        \
    typeerror (ZXB);                               \
  }                                                \
  return res;                                      \
}

#define DEFUN_INT_SINGLETON_UI(fname,mpz_fname)              \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE arg)        \
{                                                            \
  MP_INT *arg_val_z, *res_val;                               \
  unsigned long arg_val_ul;                                  \
  VALUE res;                                                 \
                                                             \
  (void)klass;                                               \
                                                             \
  if (FIXNUM_P (arg)) {                                      \
    arg_val_ul = FIX2NUM (arg);                              \
  } else if (GMPZ_P (arg)) {                                 \
    mpz_get_struct (arg, arg_val_z);                         \
    if (!mpz_fits_ulong_p (arg_val_z))                       \
      rb_raise (rb_eRangeError, "argument out of range");    \
    arg_val_ul = mpz_get_ui (arg_val_z);                     \
    if (arg_val_ul == 0)                                     \
      rb_raise (rb_eRangeError, "argument out of range");    \
  } else {                                                   \
    typeerror_as (ZX, "argument");                           \
  }                                                          \
  mpz_make_struct_init (res, res_val);                       \
  mpz_fname (res_val, arg_val_ul);                           \
  return res;                                                \
}

#define DEFUN_INT_COND_P(fname,mpz_fname)       \
static VALUE r_gmpz_##fname(VALUE self)         \
{                                               \
  MP_INT *self_val;                             \
  mpz_get_struct (self, self_val);              \
  return mpz_fname (self_val) ? Qtrue : Qfalse; \
}


/**********************************************************************
 *    Functional Mappings                                             *
 **********************************************************************/

/*
 * 01 mpz_t__mpz_t_or_ui__to__mpz_t__returns__void
 * FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID defines a GMP::Z singleton function that takes a
 * GMP::Z as rop, GMP::Z as op1, and a GMP::Z, Bignum, or Fixnum as op2. It calls
 * mpz_fname, whose arguments are rop (the return argument), op1, and op2. If op2 is a
 * Fixnum, and >= 0, the ui variant of mpz_fname will be used.
 */
#define FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(fname,mpz_fname)  \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE rop, VALUE op1, VALUE op2)  \
{                                                               \
  MP_INT *rop_val, *op1_val, *op2_val;                          \
  (void)klass;                                                  \
                                                                \
  if (! GMPZ_P(rop)) {                                          \
    typeerror_as(Z, "rop");                                     \
  }                                                             \
  mpz_get_struct (rop, rop_val);                                \
                                                                \
  if (! GMPZ_P(op1)) {                                          \
    typeerror_as (Z, "op1");                                    \
  }                                                             \
  mpz_get_struct (op1, op1_val);                                \
                                                                \
  if (FIXNUM_P (op2)) {                                         \
    if (FIX2NUM (op2) >= 0) {                                   \
      mpz_fname##_ui (rop_val, op1_val, FIX2NUM (op2));         \
    } else {                                                    \
      mpz_set_si (rop_val, FIX2NUM (op2));                      \
      mpz_fname (rop_val, op1_val, rop_val);                    \
    }                                                           \
  } else if (BIGNUM_P (op2)) {                                  \
    mpz_set_bignum (rop_val, op2);                              \
    mpz_fname (rop_val, op1_val, rop_val);                      \
  } else if (GMPZ_P (op2)) {                                    \
    mpz_get_struct (op2, op2_val);                              \
    mpz_fname (rop_val, op1_val, op2_val);                      \
  } else {                                                      \
    typeerror_as (ZXB, "op2");                                  \
  }                                                             \
                                                                \
  return Qnil;                                                  \
}

/*
 * Document-method: GMP::Z.add
 *
 * call-seq:
 *   GMP::Z.add(rop, op1, op2)
 *
 * Returns _a_ times 2 raised to _n_. This operation can also be defined as a left shift
 * by _n_ bits.
 */
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(add,mpz_add)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(addmul,mpz_addmul)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(submul,mpz_submul)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(divexact,mpz_divexact)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(lcm,mpz_lcm)


/**********************************************************************
 *    Initializing, Assigning Integers                                *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::Z.new(value = 0)
 *
 * Creates a new GMP::Z integer, with _value_ as its value, converting where necessary.
 * _value_ must be an instance of one of the following classes:
 *
 * * GMP::Z
 * * Fixnum
 * * String
 * * Bignum
 *
 * @example
 *   GMP::Z.new(5)        #=> 5
 *   GMP::Z.new(2**32)    #=> 4_294_967_296
 *   GMP::Z.new(2**101)   #=> 2_535_301_200_456_458_802_993_406_410_752
 *   GMP::Z.new("20")     #=> 20
 *   GMP::Z.new("0x20")   #=> 32
 *   GMP::Z.new("020")    #=> 16
 *   GMP::Z.new("0b101")  #=> 5
 */
VALUE r_gmpzsg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_INT *res_val;
  VALUE res;
  (void)klass;

  if (argc > 1)
    rb_raise(rb_eArgError, "wrong # of arguments (%d for 0 or 1)", argc);

  mpz_make_struct(res, res_val);
  mpz_init(res_val);
  rb_obj_call_init(res, argc, argv);

  return res;
}

VALUE r_gmpz_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_INT *self_val;

  if (argc != 0) {
    mpz_get_struct(self,self_val);
    mpz_set_value (self_val, argv[0]);
  }
  return Qnil;
}

/*
 * call-seq:
 *   a = b
 *
 * Sets the value of _a_ to the value of _b_. _b_ must be an instance of one of the
 * following classes:
 *
 * * GMP::Z
 * * Fixnum
 * * String
 * * Bignum
 */
void mpz_set_value(MP_INT *target, VALUE source)
{
  MP_INT *source_val;

  if (GMPZ_P(source)) {
    mpz_get_struct(source, source_val);
    mpz_set(target, source_val);
  } else if (FIXNUM_P(source)) {
    mpz_set_si(target, FIX2NUM(source));
  } else if (STRING_P(source)) {
    mpz_set_str(target, StringValuePtr(source), 0);
  } else if (BIGNUM_P(source)) {
    mpz_set_bignum(target, source);
  } else {
    rb_raise(rb_eTypeError, "Don't know how to convert %s into GMP_Z", rb_class2name(rb_class_of(source)));
  }
}

/*
 * call-seq:
 *   GMP::Z(value)
 *
 * A convenience method for GMP::Z.new(value).
 */
VALUE r_gmpmod_z(int argc, VALUE *argv, VALUE module)
{
  (void)module;
  return r_gmpzsg_new(argc, argv, cGMP_Z);
}

/*
 * call-seq:
 *   a.swap(b)
 *
 * @return nil
 *
 * Efficiently swaps the contents of _a_ with _b_. _b_ must be an instance of GMP::Z.
 */
VALUE r_gmpz_swap(VALUE self, VALUE arg)
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


/**********************************************************************
 *    Converting Integers                                             *
 **********************************************************************/

/*
 * call-seq:
 *   a.to_i
 *
 * @todo Implement mpz_fits_slong_p
 *
 * Returns _a_ as an Fixnum if _a_ fits into a Fixnum.
 *
 * Otherwise returns the least significant part of _a_, with the same sign as _a_.
 *
 * If _a_ is too big to fit in a Fixnum, the returned result is probably not very useful.
 * To find out if the value will fit, use the function +mpz_fits_slong_p+
 * (<b>Unimplemented</b>).
 */
VALUE r_gmpz_to_i(VALUE self)
{
  MP_INT *self_val;
  char *str;
  VALUE res;

  mpz_get_struct(self, self_val);
  if (mpz_fits_slong_p(self_val))
    return rb_int2inum(mpz_get_si(self_val));
  str = mpz_get_str(NULL, 0, self_val);
  res = rb_cstr2inum(str, 10);
  free(str);
  return res;
}

/*
 * call-seq:
 *   a.to_d
 *
 * @todo Implement mpz_fits_slong_p
 *
 * Returns _a_ as a Float if _a_ fits in a Float.
 *
 * Otherwise returns the least significant part of _a_, with the same sign as _a_.
 *
 * If _a_ is too big to fit in a Float, the returned result is probably not very useful.
 * To find out if the value will fit, use the function +mpz_fits_slong_p+
 * (<b>Unimplemented</b>).
 */
VALUE r_gmpz_to_d(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);

  return rb_float_new(mpz_get_d(self_val));
}

/*
 * Document-method: to_s
 *
 * call-seq:
 *   a.to_s(base = 10)
 *   a.to_s(:bin)
 *   a.to_s(:oct)
 *   a.to_s(:dec)
 *   a.to_s(:hex)
 *
 * Returns _a_, as a String. If _base_ is not provided, then the decimal
 * representation will be returned.
 *
 * From the GMP Manual:
 *
 * Convert _a_ to a string of digits in base _base_. The _base_ argument may vary from 2
 * to 62 or from -2 to -36.
 *
 * For _base_ in the range 2..36, digits and lower-case letters are used; for -2..-36,
 * digits and upper-case letters are used; for 37..62, digits, upper-case letters, and
 * lower-case letters (in that significance order) are used.
 */
VALUE r_gmpz_to_s(int argc, VALUE *argv, VALUE self)
{
  MP_INT *self_val;
  char *str;
  VALUE res;
  VALUE base;
  int base_val = 10;
  ID base_id;
  const char * bin_base = "bin";                            /* binary */
  const char * oct_base = "oct";                             /* octal */
  const char * dec_base = "dec";                           /* decimal */
  const char * hex_base = "hex";                       /* hexadecimal */
  ID bin_base_id = rb_intern(bin_base);
  ID oct_base_id = rb_intern(oct_base);
  ID dec_base_id = rb_intern(dec_base);
  ID hex_base_id = rb_intern(hex_base);  
  
  rb_scan_args(argc, argv, "01", &base);
  if (NIL_P(base)) { base = INT2FIX(10); }           /* default value */
  if (FIXNUM_P(base)) {
    base_val = FIX2INT(base);
    if ((base_val >=   2 && base_val <= 62) ||
        (base_val >= -36 && base_val <= -2)) {
      /* good base */
    } else {
      base_val = 10;
      rb_raise(rb_eRangeError, "base must be within [2, 62] or [-36, -2].");
    }
  } else if (SYMBOL_P(base)) {
    base_id = rb_to_id(base);
    if (base_id == bin_base_id) {
      base_val =  2;
    } else if (base_id == oct_base_id) {
      base_val =  8;
    } else if (base_id == dec_base_id) {
      base_val = 10;
    } else if (base_id == hex_base_id) {
      base_val = 16;
    } else {
      base_val = 10;  /* should raise an exception here. */
    }
  }

  Data_Get_Struct(self, MP_INT, self_val);
  str = mpz_get_str(NULL, base_val, self_val);
  res = rb_str_new2(str);
  free (str);

  return res;
}


/**********************************************************************
 *    Integer Arithmetic                                              *
 **********************************************************************/

/*
 * call-seq:
 *   a + b
 *
 * Adds _a_ to _b_. _b_ must be an instance of one of:
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
    mpz_add(res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    if (FIX2NUM(arg) > 0)
      mpz_add_ui(res_val, self_val, FIX2NUM(arg));
    else
      mpz_sub_ui(res_val, self_val, -FIX2NUM(arg));
  } else if (GMPQ_P(arg)) {
    return r_gmpq_add(arg, self);
  } else if (GMPF_P(arg)) {
#ifndef MPFR
    return r_gmpf_add(arg, self);
#else
    return rb_funcall(arg, rb_intern("+"), 1, self);
#endif
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_init(res_val);
    mpz_set_bignum(res_val, arg);
    mpz_add(res_val, res_val, self_val);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   a.add!(_b_)
 *
 * Adds _a_ to _b_ in-place, setting _a_ to the sum. _b_ must be an instance of one of:
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_add_self(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    mpz_add(self_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    if (FIX2NUM(arg) > 0)
      mpz_add_ui(self_val, self_val, FIX2NUM(arg));
    else
      mpz_sub_ui(self_val, self_val, -FIX2NUM(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    mpz_add(self_val, self_val, arg_val);
    mpz_temp_free(arg_val);
  } else {
    typeerror(ZXB);
  }
  return Qnil;
}

/*
 * call-seq:
 *   a - b
 *
 * Subtracts _b_ from _a_. _b_ must be an instance of one of:
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_sub(VALUE self, VALUE arg)
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
    if (FIX2NUM(arg) > 0)
      mpz_sub_ui (res_val, self_val, FIX2NUM(arg));
    else
      mpz_add_ui (res_val, self_val, -FIX2NUM(arg));
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

/*
 * call-seq:
 *   a.sub!(b)
 *
 * Subtracts _b_ from _a_ in-place, setting _a_ to the difference. _b_ must be an
 * instance of one of:
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_sub_self(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val);
    mpz_sub (self_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    if (FIX2NUM(arg) > 0)
      mpz_sub_ui (self_val, self_val, FIX2NUM(arg));
    else
      mpz_add_ui (self_val, self_val, -FIX2NUM(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    mpz_sub (self_val, self_val, arg_val);
    mpz_temp_free (arg_val);
  } else {
    typeerror (ZXB);
  }
  return Qnil;
}

/*
 * call-seq:
 *   a * b
 *
 * Multiplies _a_ with _b_. _a_ must be an instance of one of
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_mul(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_get_struct(arg,arg_val);
    mpz_mul(res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_mul_si(res_val, self_val, FIX2NUM(arg));
  } else if (GMPQ_P(arg)) {
    return r_gmpq_mul(arg, self);
  } else if (GMPF_P(arg)) {
#ifndef MPFR
    return r_gmpf_mul(arg, self);
#else
    return rb_funcall(arg, rb_intern("*"), 1, self);
#endif
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_set_bignum(res_val, arg);
    mpz_mul(res_val, res_val, self_val);
  } else {
    typeerror(ZQFXB);
  }
  return res;
}

/*
 * call-seq:
 *   a.addmul!(b, c)
 *
 * @since 0.4.19
 *
 * Sets _a_ to _a_ plus _b_ times _c_. _b_ and _c_ must each be an instance of one of
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
static VALUE r_gmpz_addmul_self(VALUE self, VALUE b, VALUE c)
{
  MP_INT *self_val, *b_val, *c_val;
  int free_b_val = 0;

  if (GMPZ_P(b)) {
    mpz_get_struct(b, b_val);
  } else if (FIXNUM_P(b)) {
    mpz_temp_alloc(b_val);
    mpz_init_set_si(b_val, FIX2NUM(b));
    free_b_val = 1;
  } else if (BIGNUM_P(b)) {
    mpz_temp_from_bignum(b_val, b);
    free_b_val = 1;
  } else {
    typeerror_as(ZXB, "addend");
  }
  mpz_get_struct(self, self_val);

  if (GMPZ_P(c)) {
    mpz_get_struct(c, c_val);
    mpz_addmul(self_val, b_val, c_val);
  } else if (FIXNUM_P(c)) {
    if (FIX2NUM(c) < 0)
    {
      if (free_b_val) { mpz_temp_free(b_val); }
      rb_raise(rb_eRangeError, "multiplicand (Fixnum) must be nonnegative");
    }
    mpz_addmul_ui(self_val, b_val, FIX2NUM(c));
  } else if (BIGNUM_P(c)) {
    mpz_temp_from_bignum(c_val, c);
    mpz_addmul(self_val, b_val, c_val);
    mpz_temp_free(c_val);
  } else {
    if (free_b_val)
      mpz_temp_free(b_val);
    typeerror_as(ZXB, "multiplicand");
  }
  if (free_b_val)
    mpz_temp_free(b_val);
  return self;
}

/*
 * call-seq:
 *   a.submul!(b, c)
 *
 * @since 0.5.23
 *
 * Sets _a_ to _a_ minus _b_ times _c_. _b_ and _c_ must each be an instance of one of
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
static VALUE r_gmpz_submul_self(VALUE self, VALUE b, VALUE c)
{
  MP_INT *self_val, *b_val, *c_val;
  int free_b_val = 0;

  if (GMPZ_P(b)) {
    mpz_get_struct(b, b_val);
  } else if (FIXNUM_P(b)) {
    mpz_temp_alloc(b_val);
    mpz_init_set_si(b_val, FIX2NUM(b));
    free_b_val = 1;
  } else if (BIGNUM_P(b)) {
    mpz_temp_from_bignum(b_val, b);
    free_b_val = 1;
  } else {
    typeerror_as(ZXB, "addend");
  }
  mpz_get_struct(self, self_val);

  if (GMPZ_P(c)) {
    mpz_get_struct(c, c_val);
    mpz_submul(self_val, b_val, c_val);
  } else if (FIXNUM_P(c)) {
    if (FIX2NUM(c) < 0)
    {
      if (free_b_val) { mpz_temp_free(b_val); }
      rb_raise(rb_eRangeError, "multiplicand (Fixnum) must be nonnegative");
    }
    mpz_submul_ui(self_val, b_val, FIX2NUM(c));
  } else if (BIGNUM_P(c)) {
    mpz_temp_from_bignum(c_val, c);
    mpz_submul(self_val, b_val, c_val);
    mpz_temp_free(c_val);
  } else {
    if (free_b_val)
      mpz_temp_free(b_val);
    typeerror_as(ZXB, "multiplicand");
  }
  if (free_b_val)
    mpz_temp_free(b_val);
  return self;
}

/*
 * Document-method: <<
 *
 * call-seq:
 *   a << n
 *
 * Returns _a_ times 2 raised to _n_. This operation can also be defined as a left shift
 * by _n_ bits.
 */
DEFUN_INT_F_UL(shl,mpz_mul_2exp,"shift size")

/*
 * Document-method: neg
 *
 * call-seq:
 *   a.neg
 *   -a
 *
 * Returns -_a_.
 */
/*
 * Document-method: neg!
 *
 * call-seq:
 *   a.neg!
 *
 * Sets _a_ to -_a_.
 */
DEFUN_INT2INT(neg, mpz_neg)
/*
 * Document-method: abs
 *
 * call-seq:
 *   a.abs
 *
 * Returns the absolute value of _a_.
 */
/*
 * Document-method: abs!
 *
 * call-seq:
 *   a.abs!
 *
 * Sets _a_ to its absolute value.
 */
DEFUN_INT2INT(abs, mpz_abs)


/**********************************************************************
 *    Integer Division                                                *
 **********************************************************************/

/*
 * call-seq:
 *   a / b
 *
 * Divides _a_ by _b_. Combines the different GMP division functions to provide what one
 * is hopefully looking for. The result will either be an instance of GMP::Q or GMP::F,
 * depending on _b_. _b_ must be an instance of one of the following:
 * * GMP::Z
 * * Fixnum
 * * GMP::Q
 * * GMP::F
 * * Bignum
 */
VALUE r_gmpz_div(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val_z, *tmp_z;
  MP_RAT *arg_val_q, *res_val_q;
  MP_FLOAT *arg_val_f, *res_val_f;
  VALUE res;
#if defined(MPFR) && MPFR_VERSION_MAJOR>2
  mpfr_prec_t prec;
#else
  unsigned int prec;
#endif

  mpz_get_struct (self,self_val);

  if (GMPZ_P (arg)) {
    mpz_get_struct (arg, arg_val_z);
    if (mpz_cmp_ui (arg_val_z, 0) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init (res, res_val_q);
    mpq_set_num (res_val_q, self_val);
    mpq_set_den (res_val_q, arg_val_z);
    mpq_canonicalize (res_val_q);
  } else if (FIXNUM_P (arg)) {
    if (FIX2NUM (arg) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_make_struct_init (res, res_val_q);
    mpq_set_num (res_val_q, self_val);
    mpz_set_ui (mpq_denref (res_val_q), FIX2NUM (arg));
    mpq_canonicalize (res_val_q);
  } else if (GMPQ_P (arg)) {
    mpq_get_struct (arg, arg_val_q);
    if (mpz_cmp_ui (mpq_numref (arg_val_q), 0) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpz_temp_init (tmp_z);
    mpq_make_struct_init (res, res_val_q);
    mpz_gcd (tmp_z, mpq_numref (arg_val_q), self_val);
    mpz_divexact (mpq_numref (res_val_q), self_val, tmp_z);
    mpz_divexact (mpq_denref (res_val_q), mpq_numref (arg_val_q), tmp_z);
    mpz_mul (mpq_numref (res_val_q), mpq_numref (res_val_q), mpq_denref (arg_val_q));
    mpz_temp_free (tmp_z);
  } else if (GMPF_P(arg)) {
    mpf_get_struct_prec (arg, arg_val_f, prec);
    mpf_make_struct_init (res, res_val_f, prec);
    mpf_set_z (res_val_f, self_val);
    mpf_div (res_val_f, res_val_f, arg_val_f);
  } else if (BIGNUM_P (arg)) {
    mpq_make_struct_init (res, res_val_q);
    mpz_set_bignum (mpq_denref (res_val_q), arg);
    if (mpz_cmp_ui (mpq_denref (res_val_q), 0) == 0)
      rb_raise (rb_eZeroDivError, "divided by 0");
    mpq_set_num (res_val_q, self_val);
    mpq_canonicalize (res_val_q);
  } else {
    typeerror (ZQFXB);
  }
  return res;
}

/*
 * Document-method: tdiv
 *
 * call-seq:
 *   n.tdiv(d)
 * 
 * Divides _n_ by _d_, forming a quotient _q_. tdiv rounds _q_ towards zero. The _t_
 * stands for "truncate".
 *
 * _q_ will satisfy <i>n=q*d+r</i>, and _r_ will satisfy <i>0 <= abs( r ) < abs( d )</i>.
 *
 * This function calculates only the quotient.
 */
DEFUN_INT_DIV(tdiv, mpz_tdiv_q)
/*
 * Document-method: tmod
 *
 * call-seq:
 *   n.tmod(d)
 * 
 * Divides _n_ by _d_, forming a remainder _r_. _r_ will have the same sign as _n_. The
 * _t_ stands for "truncate". 
 *
 * _r_ will satisfy <i>n=q*d+r</i>, and _r_ will satisfy <i>0 <= abs( r ) < abs( d )</i>.
 *
 * This function calculates only the remainder.
 *
 * The remainder can be negative, so the return value is the absolute value of the
 * remainder.
 */
DEFUN_INT_DIV(tmod, mpz_tdiv_r)
/*
 * Document-method: tshr
 *
 * call-seq:
 *   n.tshr(d)
 * 
 * Divides _n_ by 2^<i>d</i>, forming a quotient _q_. tshr rounds _q_ towards zero. The
 * _t_ stands for "truncate". 
 *
 * _q_ will satisfy <i>n=q*d+r</i>, and _r_ will satisfy <i>0 <= abs( r ) < abs( d )</i>.
 *
 * This function calculates only the quotient.
 */
DEFUN_INT_F_UL(tshr,mpz_tdiv_q_2exp,"shift size")
DEFUN_INT_F_UL(tshrm,mpz_tdiv_r_2exp,"mark size")

/*
 * Document-method: fdiv
 *
 * call-seq:
 *   n.fdiv(d)
 *
 * Divide _n_ by _d_, forming a quotient _q_. fdiv rounds _q_ down towards -infinity.
 * The f stands for "floor".
 *
 * _q_ will satisfy <i>n=q*d+r</i>.
 *
 * This function calculates only the quotient.
 */
DEFUN_INT_DIV(fdiv, mpz_fdiv_q)
/*
 * Document-method: fmod
 *
 * call-seq:
 *   n.fmod(d)
 *
 * Divides _n_ by _d_, forming a remainder _r_. _r_ will have the same sign as _d_. The
 * _f_ stands for "floor". 
 *
 * _r_ will satisfy <i>n=q*d+r</i>, and _r_ will satisfy <i>0 <= abs( r ) < abs( d )</i>.
 *
 * This function calculates only the remainder.
 *
 * The remainder can be negative, so the return value is the absolute value of the
 * remainder.
 */
DEFUN_INT_DIV(fmod, mpz_fdiv_r)
DEFUN_INT_F_UL(fshr,mpz_fdiv_q_2exp,"shift size")
DEFUN_INT_F_UL(fshrm,mpz_fdiv_r_2exp,"mark size")

/*
 * Document-method: cdiv
 *
 * call-seq:
 *   n.cdiv(d)
 *
 * Divide _n_ by _d_, forming a quotient _q_. cdiv rounds _q_ up towards
 * _+infinity_. The c stands for "ceil".
 *
 * _q_ will satisfy <i>n=q*d+r</i>.
 *
 * This function calculates only the quotient.
 */
DEFUN_INT_DIV(cdiv, mpz_cdiv_q)
/*
 * Document-method: cmod
 *
 * call-seq:
 *   n.cmod(d)
 *
 * Divides _n_ by _d_, forming a remainder _r_. _r_ will have the opposite sign of _d_.
 * The c stands for "ceil".
 *
 * _r_ will satisfy <i>n=q*d+r</i>, and _r_ will satisfy <i>0 <= abs( r ) < abs( d )</i>.
 *
 * This function calculates only the remainder.
 */
DEFUN_INT_DIV(cmod, mpz_cdiv_r)

/*
 * call-seq:
 *   a % b
 *
 * @since 0.2.10
 *
 * Returns _a_ mod _b_. _b_ can be an instance any of the following:
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
VALUE r_gmpz_mod(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct(self,self_val);

  if (GMPZ_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_get_struct(arg, arg_val);
    mpz_mod(res_val, self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_mod_ui(res_val, self_val, FIX2NUM(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_set_bignum(res_val, arg);
    mpz_mod(res_val, res_val, self_val);
  } else {
    typeerror(ZXB);
  }
  return res;
}

/*
 * call-seq:
 *   a.divisible?(b)
 *
 * @since 0.5.23
 *
 * Returns true if _a_ is divisible by _b_. _b_ can be an instance any of the following:
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
static VALUE r_gmpz_divisible(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;
  int res;
  mpz_get_struct (self, self_val);
  
  if (FIXNUM_P (arg) && FIX2NUM (arg) > 0) {
    mpz_temp_alloc(arg_val);
    mpz_init_set_ui(arg_val, FIX2NUM(arg));
    res = mpz_divisible_ui_p (self_val, FIX2NUM (arg));
    mpz_temp_free(arg_val);
  } else if (FIXNUM_P (arg)) {
    mpz_temp_alloc(arg_val);
    mpz_make_struct_init (arg, arg_val);
    mpz_init_set_si(arg_val, FIX2NUM(arg));
    res = mpz_divisible_p (self_val, arg_val);
    mpz_temp_free(arg_val);
  } else if (BIGNUM_P (arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    res = mpz_divisible_p (self_val, arg_val);
    mpz_temp_free(arg_val);
  } else if (GMPZ_P (arg)) {
    mpz_get_struct(arg, arg_val);
    res = mpz_divisible_p (self_val, arg_val);
  } else {
    typeerror_as (ZXB, "argument");
  }
  return (res != 0) ? Qtrue : Qfalse;
}


/**********************************************************************
 *    Integer Exponentiation                                          *
 **********************************************************************/

/*
 * Document-method: **
 *
 * call-seq:
 *   a ** b
 *
 * Returns _a_ raised to _b_. The case 0^0 yields 1.
 */
DEFUN_INT_F_UL(pow,mpz_pow_ui,"exponent")

/*
 * Document-method: GMP::Z.pow
 *
 * call-seq:
 *   GMP::Z.pow(a, b)
 *
 * Returns _a_ raised to _b_. The case 0^0 yields 1.
 */
VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp)
{
  MP_INT *res_val;
  VALUE res;

  if (FIXNUM_P(base) && FIXNUM_P(exp))
  {
    if (FIX2NUM(base) < 0)
      rb_raise (rb_eRangeError, "base must not be negative");
    if (FIX2NUM(exp) < 0) 
      rb_raise (rb_eRangeError, "exponent must not be negative");
    mpz_make_struct_init (res, res_val);
    mpz_ui_pow_ui (res_val, FIX2NUM(base), FIX2NUM(exp));
    return res;
  }
  return r_gmpz_pow (r_gmpzsg_new(1, &base, klass), exp);
}

/*
 * call-seq:
 *   a.powmod(b, c)
 *
 * Returns _a_ raised to _b_ modulo _c_.
 *
 * Negative _b_ is supported if an inverse <i>a^-1</i> mod _c_ exists. If an inverse
 * doesn't exist then a divide by zero is raised.
 */
VALUE r_gmpz_powm(VALUE self, VALUE exp, VALUE mod)
{
  MP_INT *self_val, *res_val, *mod_val, *exp_val;
  VALUE res;
  int free_mod_val = 0;

  if (GMPZ_P(mod)) {
    mpz_get_struct(mod, mod_val);
    if (mpz_sgn(mod_val) <= 0) {
      rb_raise(rb_eRangeError, "modulus must be positive");
    }
  } else if (FIXNUM_P(mod)) {
    if (FIX2NUM(mod) <= 0) {
      rb_raise(rb_eRangeError, "modulus must be positive");
    }
    mpz_temp_alloc(mod_val);
    mpz_init_set_ui(mod_val, FIX2NUM(mod));
    free_mod_val = 1;
  } else if (BIGNUM_P(mod)) {
    mpz_temp_from_bignum(mod_val, mod);
    if (mpz_sgn(mod_val) <= 0) {
      mpz_temp_free(mod_val);
      rb_raise(rb_eRangeError, "modulus must be positive");
    }
    free_mod_val = 1;
  } else {
    typeerror_as(ZXB, "modulus");
  }
  mpz_make_struct_init(res, res_val);
  mpz_get_struct(self, self_val);

  if (GMPZ_P(exp)) {
    mpz_get_struct(exp, exp_val);
    if (mpz_sgn(mod_val) < 0) {
      rb_raise(rb_eRangeError, "exponent must be nonnegative");
    }
    mpz_powm(res_val, self_val, exp_val, mod_val);
  } else if (FIXNUM_P(exp)) {
    if (FIX2NUM(exp) < 0)
    {
      if (free_mod_val)
        mpz_temp_free(mod_val);
      rb_raise(rb_eRangeError, "exponent must be nonnegative");
    }
    mpz_powm_ui(res_val, self_val, FIX2NUM(exp), mod_val);
  } else if (BIGNUM_P(exp)) {
    mpz_temp_from_bignum(exp_val, exp);
    mpz_powm(res_val, self_val, exp_val, mod_val);
    mpz_temp_free(exp_val);
  } else {
    if (free_mod_val)
      mpz_temp_free(mod_val);
    typeerror_as(ZXB, "exponent");
  }
  if (free_mod_val)
    mpz_temp_free(mod_val);
  return res;
}


/**********************************************************************
 *    Integer Roots                                                   *
 **********************************************************************/

/*
 * Document-method: root
 *
 * call-seq:
 *   a.root(b)
 *
 * Returns the truncated integer part of the <i>b</i>th root of _a_.
 */
DEFUN_INT_F_UL(root,mpz_root,"root number")

/*
 * Document-method: sqrt
 *
 * call-seq:
 *   a.sqrt
 *
 * Returns the truncated integer part of the square root of _a_.
 */
/*
 * Document-method: sqrt!
 *
 * call-seq:
 *   a.sqrt!
 *
 * Sets _a_ to the truncated integer part of its square root.
 */
DEFUN_INT2INT(sqrt, mpz_sqrt)

/*
 * call-seq:
 *   a.sqrtrem #=> s, r
 *
 * Returns the truncated integer part of the square root of _a_ as _s_ and the remainder
 * <i>a - s * s</i> as _r_, which will be zero if _a_ is a perfect square.
 */
static VALUE r_gmpz_sqrtrem(VALUE self)
{
  MP_INT *self_val, *sqrt_val, *rem_val;
  VALUE sqrt, rem;

  mpz_get_struct(self, self_val);
  mpz_make_struct_init(sqrt, sqrt_val);
  mpz_make_struct_init(rem, rem_val);
  mpz_sqrtrem(sqrt_val, rem_val, self_val);
  return rb_assoc_new(sqrt, rem);
}

/*
 * Document-method: power?
 *
 * call-seq:
 *   p.power?
 *
 * Returns true if _p_ is a perfect power, i.e., if there exist integers _a_ and _b_,
 * with <i>b > 1</i>, such that _p_ equals _a_ raised to the power _b_.
 *
 * Under this definition both 0 and 1 are considered to be perfect powers. Negative
 * values of integers are accepted, but of course can only be odd perfect powers.
 */
DEFUN_INT_COND_P(is_power,mpz_perfect_power_p)
/*
 * Document-method: square?
 *
 * call-seq:
 *   p.square?
 *
 * Returns true if _p_ is a perfect square, i.e., if the square root of _p_ is an
 * integer. Under this definition both 0 and 1 are considered to be perfect squares.
 */
DEFUN_INT_COND_P(is_square,mpz_perfect_square_p)


/**********************************************************************
 *    Number Theoretic Functions                                      *
 **********************************************************************/

/*
 * call-seq:
 *   n.probab_prime?(reps = 5)
 *
 * Determine whether _n_ is prime. Returns 2 if _n_ is definitely prime, returns 1 if _n_
 * is probably prime (without being certain), or returns 0 if _n_ is definitely
 * composite.
 *
 * This function does some trial divisions, then some Miller-Rabin probabilistic
 * primality tests. +reps+ controls how many such tests are done, 5 to 10 is a reasonable
 * number, more will reduce the chances of a composite being returned as "probably
 * prime".
 *
 * Miller-Rabin and similar tests can be more properly called compositeness tests.
 * Numbers which fail are known to be composite but those which pass might be prime or
 * might be composite. Only a few composites pass, hence those which pass are considered
 * probably prime.
 */
VALUE r_gmpz_is_probab_prime(int argc, VALUE* argv, VALUE self)
{
  MP_INT *self_val;
  int reps_val;
  VALUE reps;
  mpz_get_struct (self, self_val);
  rb_scan_args (argc, argv, "01", &reps);
  if(NIL_P(reps)){
    reps = INT2FIX (5);
  }
  if (FIXNUM_P (reps)) {
    reps_val = FIX2INT (reps);
  } else {
    typeerror_as (X, "reps");
  }
  return INT2FIX (mpz_probab_prime_p(self_val, reps_val));
}

/*
 * Document-method: nextprime
 *
 * call-seq:
 *   n.nextprime
 *   n.next_prime
 *
 * Returns the next prime greater than _n_.
 *
 * This function uses a probabilistic algorithm to identify primes. For practical
 * purposes it's adequate, the chance of a composite passing will be extremely small.
 */
/*
 * Document-method: nextprime!
 *
 * call-seq:
 *   n.nextprime!
 *   n.next_prime!
 *
 * Sets _n_ to the next prime greater than _n_.
 *
 * This function uses a probabilistic algorithm to identify primes. For practical
 * purposes it's adequate, the chance of a composite passing will be extremely small.
 */
DEFUN_INT2INT(nextprime, mpz_nextprime)

/*
 * call-seq:
 *   a.gcd(b)
 *
 * @since 0.2.11
 *
 * Returns the greatest common divisor of a and b. The result is always positive even if
 * one or both of _a_ or _b_ are negative.
 */
VALUE r_gmpz_gcd(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct (self,self_val);

  if (GMPZ_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_get_struct (arg, arg_val);
    mpz_gcd (res_val, self_val, arg_val);
  } else if (FIXNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_gcd_ui (res_val, self_val, FIX2NUM(arg));
  } else if (BIGNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_set_bignum (res_val, arg);
    mpz_gcd (res_val, res_val, self_val);
  } else {
    typeerror (ZXB);
  }
  return res;
}

/*
 * call-seq:
 *   a.gcdext(b) #=> g, s, t
 *
 * @since 0.5.23
 *
 * Returns the greatest common divisor of _a_ and _b_, in addition to _s_ and _t_, the
 * coefficients satisfying <i>a*s + b*t = g</i>. _g_ is always positive, even if one or
 * both of _a_ and _b_ are negative. _s_ and _t_ are chosen such that
 * <i>abs(s) <= abs(b)</i> and <i>abs(t) <= abs(a)</i>.
 */
VALUE r_gmpz_gcdext(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val, *s_val, *t_val;
  VALUE res, s, t, ary;
  int free_arg_val = 0;

  mpz_get_struct (self,self_val);

  if (GMPZ_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_make_struct_init (s, s_val);
    mpz_make_struct_init (t, t_val);
    mpz_get_struct (arg, arg_val);
    mpz_gcdext (res_val, s_val, t_val, self_val, arg_val);
  } else if (FIXNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_make_struct_init (s, s_val);
    mpz_make_struct_init (t, t_val);
    mpz_temp_alloc(arg_val);
    mpz_init_set_ui(arg_val, FIX2NUM(arg));
    free_arg_val = 1;
    mpz_gcdext (res_val, s_val, t_val, self_val, arg_val);
  } else if (BIGNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_make_struct_init (s, s_val);
    mpz_make_struct_init (t, t_val);
    mpz_set_bignum (res_val, arg);
    mpz_gcdext (res_val, s_val, t_val, res_val, self_val);
  } else {
    typeerror (ZXB);
  }
  
  if (free_arg_val)
    mpz_temp_free(arg_val);
  
  ary = rb_ary_new3(3, res, s, t);
  return ary;
}

/*
 * call-seq:
 *   a.invert(b)
 *
 * @since 0.2.11
 *
 * Returns the inverse of _a_ modulo _b_. If the inverse exists, the return value is
 * non-zero and the result will be non-negative and less than _b_. If an inverse doesn't
 * exist, the result is undefined.
 */
VALUE r_gmpz_invert(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct (self,self_val);

  if (GMPZ_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_get_struct (arg, arg_val);
    mpz_invert (res_val, self_val, arg_val);
  } else if (FIXNUM_P (arg)) {
    mpz_temp_alloc(arg_val);
    mpz_init_set_ui(arg_val, FIX2NUM(arg));
    mpz_make_struct_init (res, res_val);
    mpz_invert (res_val, self_val, arg_val);
  } else if (BIGNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_set_bignum (res_val, arg);
    mpz_invert (res_val, res_val, self_val);
  } else {
    typeerror (ZXB);
  }
  return res;
}

/*
 * call-seq:
 *   a.jacobi(b)
 *
 * Calculate the Jacobi symbol <i>(a/b)</i>. This is defined only for _b_ odd and
 * positive.
 */
VALUE r_gmpz_jacobi(VALUE self, VALUE b)
{
  MP_INT *self_val, *b_val;
  int res_val;
  mpz_get_struct(self, self_val);
  mpz_get_struct(   b,    b_val);
  if (mpz_sgn(b_val) != 1)
    rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is non-positive.");
  if (mpz_even_p(b_val))
    rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is even.");
  res_val = mpz_jacobi(self_val, b_val);
  return INT2FIX(res_val);
}

/*
 * call-seq:
 *   GMP::Z.jacobi(a, b)
 * 
 * Calculate the Jacobi symbol <i>(a/b)</i>. This is defined only for _b_ odd and
 * positive.
 */
VALUE r_gmpzsg_jacobi(VALUE klass, VALUE a, VALUE b)
{
  MP_INT *a_val, *b_val;
  int res_val;
  int free_a_val = 0;
  int free_b_val = 0;
  (void)klass;
  
  if (GMPZ_P(a)) {
    mpz_get_struct(a, a_val);
  } else if (FIXNUM_P(a)) {
    mpz_temp_alloc(a_val);
    mpz_init_set_ui(a_val, FIX2NUM(a));
    free_a_val = 1;
  } else if (BIGNUM_P(a)) {
    mpz_temp_from_bignum(a_val, a);
    free_a_val = 1;
  } else {
    typeerror_as(ZXB, "a");
  }
  
  if (GMPZ_P(b)) {
    mpz_get_struct(b, b_val);
    if (mpz_sgn(b_val) != 1)
      rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is non-positive.");
    if (mpz_even_p(b_val))
      rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is even.");
  } else if (FIXNUM_P(b)) {
    if (FIX2NUM(b) <= 0)
      rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is non-positive.");
    if (FIX2NUM(b) % 2 == 0)
      rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is even.");
    mpz_temp_alloc(b_val);
    mpz_init_set_ui(b_val, FIX2NUM(b));
    free_b_val = 1;
  } else if (BIGNUM_P(b)) {
    mpz_temp_from_bignum(b_val, b);
    if (mpz_sgn(b_val) != 1) {
      mpz_temp_free(b_val);
      rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is non-positive.");
    }
    if (mpz_even_p(b_val)) {
      mpz_temp_free(b_val);
      rb_raise(rb_eRangeError, "Cannot take Jacobi symbol (a/b) where b is even.");
    }
    free_b_val = 1;
  } else {
    typeerror_as(ZXB, "b");
  }
  
  res_val = mpz_jacobi(a_val, b_val);
  if (free_a_val) { mpz_temp_free(a_val); }
  if (free_b_val) { mpz_temp_free(b_val); }
  return INT2FIX(res_val);
}

/*
 * call-seq:
 *   a.legendre(p)
 *
 * Calculate the Legendre symbol <i>(a/p)</i>. This is defined only for _p_ an odd
 * positive prime, and for such _p_ it's identical to the Jacobi symbol. 
 */
VALUE r_gmpz_legendre(VALUE self, VALUE p)
{
  MP_INT *self_val, *p_val;
  int res_val;
  mpz_get_struct(self, self_val);
  mpz_get_struct(   p,    p_val);
  if (mpz_sgn(p_val) != 1)
    rb_raise(rb_eRangeError, "Cannot take Legendre symbol (a/p) where p is non-positive.");
  if (mpz_even_p(p_val))
    rb_raise(rb_eRangeError, "Cannot take Legendre symbol (a/p) where p is even.");
  if (mpz_probab_prime_p(p_val, 5) == 0)
    rb_raise(rb_eRangeError, "Cannot take Legendre symbol (a/p) where p is composite.");
  res_val = mpz_legendre(self_val, p_val);
  return INT2FIX(res_val);
}

/*
 * call-seq:
 *   n.remove(f) #=> r, t
 *
 * Remove all occurrences of the factor _f_ from _n_, returning the result as _r_. _t_,
 * how many such occurrences were removed, is also returned.
 */
VALUE r_gmpz_remove(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;
#if __GNU_MP_VERSION>2
  unsigned long removed_val;
#else
  int   removed_val;
#endif
  int free_arg_val = 0;

  mpz_get_struct(self, self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    if (mpz_sgn(arg_val) != 1)
      rb_raise(rb_eRangeError, "argument must be positive");
  } else if (FIXNUM_P(arg)) {
    if (FIX2NUM(arg) <= 0)
      rb_raise(rb_eRangeError, "argument must be positive");
    mpz_temp_alloc(arg_val);
    mpz_init_set_ui(arg_val, FIX2NUM(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    if (mpz_sgn(arg_val) != 1) {
      mpz_temp_free(arg_val);
      rb_raise(rb_eRangeError, "argument must be positive");
    }
  } else {
    typeerror(ZXB);
  }

  mpz_make_struct_init(res, res_val);
  removed_val = mpz_remove(res_val, self_val, arg_val);

  if (free_arg_val)
    mpz_temp_free(arg_val);

  return rb_assoc_new(res, INT2FIX(removed_val));
}

/*
 * Document-method: GMP::Z.fac
 *
 * call-seq:
 *   GMP::Z.fac(n)
 *
 * Returns <i>n!</i>, the factorial of _n_.
 *
 * Examples:
 * * GMP::Z.fac(0)  #=>  1
 * * GMP::Z.fac(1)  #=>  1
 * * GMP::Z.fac(2)  #=>  2
 * * GMP::Z.fac(3)  #=>  6
 * * GMP::Z.fac(4)  #=> 24
 */
DEFUN_INT_SINGLETON_UI(fac, mpz_fac_ui)
/*
 * Document-method: GMP::Z.fib
 *
 * call-seq:
 *   GMP::Z.fib(n)
 *
 * Returns <i>F[n]</i>, the <i>n</i>th Fibonacci number.
 *
 * Examples:
 * * GMP::Z.fib(1)  #=>  1
 * * GMP::Z.fib(2)  #=>  1
 * * GMP::Z.fib(3)  #=>  2
 * * GMP::Z.fib(4)  #=>  3
 * * GMP::Z.fib(5)  #=>  5
 * * GMP::Z.fib(6)  #=>  8
 * * GMP::Z.fib(7)  #=> 13
 */
DEFUN_INT_SINGLETON_UI(fib, mpz_fib_ui)


/**********************************************************************
 *    Integer Comparisons                                             *
 **********************************************************************/

/*
 * call-seq:
 *   a == b
 * 
 * Returns true if _a_ is equal to _b_, and false otherwise.
 */
VALUE r_gmpz_eq(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val_z;
  MP_RAT *arg_val_q;

  mpz_get_struct (self, self_val);
  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val_z);
    return (mpz_cmp (self_val, arg_val_z)==0) ? Qtrue : Qfalse;
  } else if (FIXNUM_P(arg)) {
    return (mpz_cmp_si (self_val, FIX2NUM(arg))==0) ? Qtrue : Qfalse;
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

int mpz_cmp_value(MP_INT *OP, VALUE arg)
{
  MP_RAT *arg_val_q;
  MP_INT *arg_val_z;
  int res;

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val_z);
    return mpz_cmp(OP,arg_val_z);
  } else if (FIXNUM_P(arg)) {
    return mpz_cmp_si(OP, FIX2NUM(arg));
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpz_temp_alloc(arg_val_z);
    mpz_init(arg_val_z);
    mpz_mul(arg_val_z, OP, mpq_denref(arg_val_q));
    res = mpz_cmp(arg_val_z, mpq_numref(arg_val_q));
    mpz_temp_free(arg_val_z);
    return res;
  } else if (GMPF_P(arg)) {
    not_yet;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    res = mpz_cmp(OP, arg_val_z);
    mpz_temp_free(arg_val_z);
    return res;
  } else {
    typeerror_as(ZQFXB, "exponent");
  }
}

/*
 * call-seq:
 *   a <=> b
 * 
 * Returns negative if _a_ is less than _b_.
 *
 * Returns 0 if _a_ is equal to _b_.
 *
 * Returns positive if _a_ is greater than _b_.
 */
VALUE r_gmpz_cmp(VALUE self, VALUE arg)
{
  MP_INT *self_val;
  int res;
  mpz_get_struct(self,self_val);
  res = mpz_cmp_value(self_val, arg);
  if (res > 0)
    return INT2FIX(1);
  else if (res == 0)
    return INT2FIX(0);
  else
    return INT2FIX(-1);
}

/*
 * Document-method: <
 *
 * call-seq:
 *   a < b
 * 
 * Returns whether _a_ is strictly less than _b_.
 */
DEFUN_INT_CMP(lt,<)
/*
 * Document-method: <=
 *
 * call-seq:
 *   a <= b
 * 
 * Returns whether _a_ is less than or equal to _b_.
 */
DEFUN_INT_CMP(le,<=)
/*
 * Document-method: >
 *
 * call-seq:
 *   a > b
 * 
 * Returns whether _a_ is strictly greater than _b_.
 */
DEFUN_INT_CMP(gt,>)
/*
 * Document-method: >=
 *
 * call-seq:
 *   a >= b
 *
 * Returns whether _a_ is greater than or equal to _b_.
 */
DEFUN_INT_CMP(ge,>=)

/*
 * call-seq:
 *   a.cmpabs(b)
 * 
 * Returns negative if abs(_a_) is less than abs(_b_).
 *
 * Returns 0 if abs(_a_) is equal to abs(_b_).
 *
 * Returns positive if abs(_a_) is greater than abs(_b_).
 */
VALUE r_gmpz_cmpabs(VALUE self, VALUE arg)
{
  MP_INT *arg_val_z, *self_val;
  MP_RAT *arg_val_q;
  int res;

  mpz_get_struct(self, self_val);

  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val_z);
    return INT2FIX(mpz_cmpabs(self_val, arg_val_z));
  } else if (FIXNUM_P(arg)) {
    if (FIX2NUM(arg) >= 0)
      return INT2FIX(mpz_cmpabs_ui(self_val, FIX2NUM(arg)));
    else
      return INT2FIX(mpz_cmpabs_ui(self_val, -FIX2NUM(arg)));
  } else if (GMPQ_P(arg)) {
    mpq_get_struct(arg,arg_val_q);
    mpz_temp_alloc(arg_val_z);
    mpz_init(arg_val_z);
    mpz_mul(arg_val_z, self_val, mpq_denref(arg_val_q));
    res = mpz_cmpabs(arg_val_z, mpq_numref(arg_val_q));
    mpz_temp_free(arg_val_z);
    return INT2FIX(res);
  } else if (GMPF_P(arg)) {
    not_yet;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val_z, arg);
    res = mpz_cmpabs(self_val, arg_val_z);
    mpz_temp_free(arg_val_z);
    return INT2FIX(res);
  } else {
    typeerror(ZQFXB);
  }
}

/*
 * call-seq:
 *   a.sgn
 *
 * Returns +1 if _a_ > 0, 0 if _a_ == 0, and -1 if _a_ < 0.
 */
VALUE r_gmpz_sgn(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);
  return INT2FIX(mpz_sgn(self_val));
}

/*
 * call-seq:
 *   a.eql?(b)
 *
 * @since 0.4.7
 *
 * Returns true if _a_ is equal to _b_. _a_ and _b_ must then be equal in cardinality,
 * and both be instances of GMP::Z. Otherwise, returns false. a.eql?(b) if and only if
 * a.hash == b.hash.
 */
VALUE r_gmpz_eql(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val;
  mpz_get_struct(self,self_val);
  
  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val);
    return (mpz_cmp (self_val, arg_val) == 0) ? Qtrue : Qfalse;
  }
  else {
    return Qfalse;
  }
}

/*
 * call-seq:
 *   a.hash
 *
 * @since 0.4.7
 *
 * Returns the computed hash value of _a_. This method first converts _a_ into a String
 * (base 10), then calls String#hash on the result, returning the hash value. a.eql?(b)
 * if and only if a.hash == b.hash.
 */
VALUE r_gmpz_hash(VALUE self)
{
  ID to_s_sym = rb_intern("to_s");
  ID hash_sym = rb_intern("hash");
  return rb_funcall(rb_funcall(self, to_s_sym, 0), hash_sym, 0);
}


/**********************************************************************
 *    Integer Logic and Bit Fiddling                                  *
 **********************************************************************/

/*
 * Document-method: &
 *
 * call-seq:
 *   a & b
 *
 * Returns _a_ bitwise-and _b_. _b_ must be an instance of one of the following:
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
DEFUN_INT_LOGIC(and, mpz_and)
/*
 * Document-method: |
 *
 * call-seq:
 *   a | b
 *
 * Returns _a_ bitwise inclusive-or _b_. _b_ must be an instance of one of the following:
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
DEFUN_INT_LOGIC(or, mpz_ior)
/*
 * Document-method: ^
 *
 * call-seq:
 *   a ^ b
 *
 * Returns _a_ bitwise exclusive-or _b_. _b_ must be an instance of one of the following:
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
DEFUN_INT_LOGIC(xor, mpz_xor)

/*
 * Document-method: com
 *
 * call-seq:
 *   a.com
 *
 * Returns the one's complement of _a_.
 */
/*
 * Document-method: com!
 *
 * call-seq:
 *   a.com!
 *
 * Sets _a_ to its one's complement.
 */
DEFUN_INT2INT(com, mpz_com)

/*
 * Document-method: popcount
 *
 * call-seq:
 *   a.popcount
 *
 * If _a_ >= 0, return the population count of _a_, which is the number of 1 bits in the
 * binary representation. If _a_ < 0, the number of 1s is infinite, and the return value
 * is INT2FIX(ULONG_MAX), the largest possible unsigned long.
 */
VALUE r_gmpz_popcount(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);
  return INT2FIX(mpz_popcount(self_val));
}

/*
 * call-seq:
 *   a.scan0(starting_bit)
 *
 * Scan _a_, starting from bit _starting_bit_, towards more significant bits, until the
 * first 0 bit is found. Return the index of the found bit.
 *
 * If the bit at _starting_bit_ is already what's sought, then _starting_bit_ is
 * returned.
 * 
 * If there's no bit found, then INT2FIX(ULONG_MAX) is returned. This will happen in
 * scan0 past the end of a negative number.
 */
VALUE r_gmpz_scan0(VALUE self, VALUE bitnr)
{
  MP_INT *self_val;
  int bitnr_val;
  mpz_get_struct (self, self_val);
  if (FIXNUM_P (bitnr)) {
    bitnr_val = FIX2INT (bitnr);
  } else {
    typeerror_as (X, "index");
  }
  return INT2FIX (mpz_scan0 (self_val, bitnr_val));
}

/*
 * call-seq:
 *   a.scan1(starting_bit)
 *
 * Scan _a_, starting from bit _starting_bit_, towards more significant bits, until the
 * first 1 bit is found. Return the index of the found bit.
 * 
 * If the bit at _starting_bit_ is already what's sought, then _starting_bit_ is
 * returned.
 *
 * If there's no bit found, then INT2FIX(ULONG_MAX) is returned. This will happen in
 * scan1 past the end of a nonnegative number.
 */
VALUE r_gmpz_scan1(VALUE self, VALUE bitnr)
{
  MP_INT *self_val;
  int bitnr_val;

  mpz_get_struct (self, self_val);

  if (FIXNUM_P (bitnr)) {
    bitnr_val = FIX2INT (bitnr);
  } else {
    typeerror_as (X, "index");
  }

  return INT2FIX (mpz_scan1 (self_val, bitnr_val));
}

/*
 * call-seq:
 *   a[index] = x
 *
 * Sets the bit at _index_ to _x_.
 */
VALUE r_gmpz_setbit(VALUE self, VALUE bitnr, VALUE set_to)
{
  MP_INT *self_val;
  unsigned long bitnr_val;

  mpz_get_struct (self, self_val);

  if (FIXNUM_P (bitnr)) {
    if (FIX2NUM (bitnr) < 0) {
      rb_raise(rb_eRangeError, "index must be nonnegative");
    }
    bitnr_val = FIX2NUM (bitnr);
  } else {
    typeerror_as (X, "index");
  }
  if (RTEST (set_to)) {
    mpz_setbit (self_val, bitnr_val);
  } else {
    mpz_clrbit (self_val, bitnr_val);
  }
  return Qnil;
}

/*
 * call-seq:
 *   a[index] #=> boolean
 *
 * Gets the bit at _index_, returned as either true or false.
 */
VALUE r_gmpz_getbit(VALUE self, VALUE bitnr)
{
  MP_INT *self_val;
  unsigned long bitnr_val;
  mpz_get_struct(self, self_val);
  if (FIXNUM_P(bitnr)) {
    bitnr_val = FIX2NUM (bitnr);
  } else {
    typeerror_as(X, "index");
  }
  return mpz_tstbit(self_val, bitnr_val)?Qtrue:Qfalse;
}


/**********************************************************************
 *    Miscellaneous Integer Functions                                 *
 **********************************************************************/

/*
 * Document-method: even?
 *
 * call-seq:
 *   a.even?
 *
 * Determines whether _a_ is even. Returns true or false.
 */
DEFUN_INT_COND_P(is_even,mpz_even_p)
/*
 * Document-method: odd?
 *
 * call-seq:
 *   a.odd?
 *
 * Determines whether _a_ is odd. Returns true or false.
 */
DEFUN_INT_COND_P(is_odd,mpz_odd_p)

/*
 * call-seq:
 *   a.sizeinbase
 *   a.size_in_base
 *
 * @since 0.2.11
 *
 * Return the size of _a_ measured in number of digits in the given base. base can vary
 * from 2 to 62. The sign of _a_ is ignored, just the absolute value is used. The result
 * will be either exact or 1 too big. If base is a power of 2, the result is always
 * exact. If _a_ is zero the return value is always 1.
 */
VALUE r_gmpz_sizeinbase(VALUE self, VALUE base)
{
  MP_INT *self_val;
  int base_val;
  mpz_get_struct (self, self_val);
  base_val = FIX2INT (base);
  return INT2FIX (mpz_sizeinbase (self_val, base_val));
}

/*
 * call-seq:
 *   a.size_in_bin
 *
 * @since 0.2.11
 *
 * Return the size of _a_ measured in number of digits in binary. The sign of _a_ is
 * ignored, just the absolute value is used. If _a_ is zero the return value is 1.
 */
VALUE r_gmpz_size_in_bin(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct (self, self_val);
  return INT2FIX (mpz_sizeinbase (self_val, 2));
}


/**********************************************************************
 *    Integer Special Functions                                       *
 **********************************************************************/

/*
 * call-seq:
 *   a.size
 *
 * @since 0.4.19
 *
 * Return the size of _a_ measured in number of limbs. If _a_ is zero, the returned
 * value will be zero. 
 */
VALUE r_gmpz_size(VALUE self)
{
  MP_INT *self_val;
  mpz_get_struct(self, self_val);
  return INT2FIX(mpz_size(self_val));
}


/**********************************************************************
 *    _unsorted_                                                      *
 **********************************************************************/


/**********************************************************************
 *    Init function                                                   *
 **********************************************************************/

void init_gmpz()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "Z", r_gmpmod_z, -1);

  cGMP_Z = rb_define_class_under(mGMP, "Z", rb_cInteger);

  // Initializing, Assigning Integers
  rb_define_singleton_method(cGMP_Z, "new", r_gmpzsg_new, -1);
  rb_define_method(cGMP_Z, "initialize",    r_gmpz_initialize, -1);
  rb_define_method(cGMP_Z, "swap",          r_gmpz_swap, 1);

  // Converting Integers
  rb_define_method(cGMP_Z, "to_i", r_gmpz_to_i, 0);
  rb_define_method(cGMP_Z, "to_d", r_gmpz_to_d, 0);
  rb_define_method(cGMP_Z, "to_s", r_gmpz_to_s, -1);
  
  // Integer Arithmetic
  rb_define_method(cGMP_Z, "+",       r_gmpz_add, 1);
  rb_define_method(cGMP_Z, "add!",    r_gmpz_add_self, 1);
  rb_define_method(cGMP_Z, "-",       r_gmpz_sub, 1);  
  rb_define_method(cGMP_Z, "sub!",    r_gmpz_sub_self, 1);
  rb_define_method(cGMP_Z, "*",       r_gmpz_mul, 1);
  rb_define_method(cGMP_Z, "addmul!", r_gmpz_addmul_self, 2);
  rb_define_method(cGMP_Z, "submul!", r_gmpz_submul_self, 2);
  rb_define_method(cGMP_Z, "<<",      r_gmpz_shl, 1);
  rb_define_method(cGMP_Z, "neg",     r_gmpz_neg, 0);
  rb_define_method(cGMP_Z, "neg!",    r_gmpz_neg_self, 0);
  rb_define_method(cGMP_Z, "-@",      r_gmpz_neg, 0);
  rb_define_method(cGMP_Z, "abs",     r_gmpz_abs, 0);
  rb_define_method(cGMP_Z, "abs!",    r_gmpz_abs_self, 0);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "add", r_gmpzsg_add, 3);
  rb_define_singleton_method(cGMP_Z, "addmul", r_gmpzsg_addmul, 3);
  rb_define_singleton_method(cGMP_Z, "submul", r_gmpzsg_submul, 3);
  
  // Integer Division
  rb_define_method(cGMP_Z, "/",            r_gmpz_div, 1);
  rb_define_method(cGMP_Z, "tdiv",         r_gmpz_tdiv, 1);
  rb_define_method(cGMP_Z, "tmod",         r_gmpz_tmod, 1);
  rb_define_method(cGMP_Z, "tshr",         r_gmpz_tshr, 1);
  rb_define_method(cGMP_Z, "lastbits_sgn", r_gmpz_tshrm, 1);
  rb_define_method(cGMP_Z, "fdiv",         r_gmpz_fdiv, 1);
  rb_define_method(cGMP_Z, "fmod",         r_gmpz_fmod, 1);
  rb_define_method(cGMP_Z, "cdiv",         r_gmpz_cdiv, 1);
  rb_define_method(cGMP_Z, "cmod",         r_gmpz_cmod, 1);
  rb_define_method(cGMP_Z, "%",            r_gmpz_mod, 1);
  rb_define_method(cGMP_Z, "divisible?",   r_gmpz_divisible, 1);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "divexact", r_gmpzsg_divexact, 3);
  
  // Integer Exponentiation
  rb_define_singleton_method(cGMP_Z, "pow",    r_gmpzsg_pow, 2);
  rb_define_method(cGMP_Z,           "**",     r_gmpz_pow, 1);
  rb_define_method(cGMP_Z,           "powmod", r_gmpz_powm, 2);

  // Integer Roots
  rb_define_method(cGMP_Z, "root",    r_gmpz_root, 1);
  rb_define_method(cGMP_Z, "sqrt",    r_gmpz_sqrt, 0);
  rb_define_method(cGMP_Z, "sqrt!",   r_gmpz_sqrt_self, 0);
  rb_define_method(cGMP_Z, "sqrtrem", r_gmpz_sqrtrem, 0);
  rb_define_method(cGMP_Z, "square?", r_gmpz_is_square, 0);
  rb_define_method(cGMP_Z, "power?",  r_gmpz_is_power, 0);
  
  // Number Theoretic Functions
  rb_define_method(          cGMP_Z, "probab_prime?", r_gmpz_is_probab_prime, -1);
  rb_define_method(          cGMP_Z, "nextprime",     r_gmpz_nextprime, 0);
  rb_define_method(          cGMP_Z, "nextprime!",    r_gmpz_nextprime_self, 0);
  rb_define_alias(           cGMP_Z, "next_prime",    "nextprime");
  rb_define_alias(           cGMP_Z, "next_prime!",   "nextprime!");
  rb_define_method(          cGMP_Z, "gcd",           r_gmpz_gcd, 1);
  rb_define_method(          cGMP_Z, "gcdext",        r_gmpz_gcdext, 1);
  rb_define_method(          cGMP_Z, "invert",        r_gmpz_invert, 1);
  rb_define_method(          cGMP_Z, "jacobi",        r_gmpz_jacobi, 1);
  rb_define_singleton_method(cGMP_Z, "jacobi",        r_gmpzsg_jacobi, 2);
  rb_define_method(          cGMP_Z, "legendre",      r_gmpz_legendre, 1);
  rb_define_method(          cGMP_Z, "remove",        r_gmpz_remove, 1);
  rb_define_singleton_method(cGMP_Z, "fac",           r_gmpzsg_fac, 1);
  rb_define_singleton_method(cGMP_Z, "fib",           r_gmpzsg_fib, 1);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "lcm", r_gmpzsg_lcm, 3);
  
  // Integer Comparisons
  rb_define_method(cGMP_Z, "<=>",     r_gmpz_cmp, 1);
  rb_define_method(cGMP_Z, ">",       r_gmpz_cmp_gt, 1);
  rb_define_method(cGMP_Z, ">=",      r_gmpz_cmp_ge, 1);
  rb_define_method(cGMP_Z, "==",      r_gmpz_eq, 1);
  rb_define_method(cGMP_Z, "<",       r_gmpz_cmp_lt, 1);
  rb_define_method(cGMP_Z, "<=",      r_gmpz_cmp_le, 1);
  rb_define_method(cGMP_Z, "cmpabs",  r_gmpz_cmpabs, 1);
  rb_define_method(cGMP_Z, "sgn",     r_gmpz_sgn, 0);
  
  rb_define_method(cGMP_Z, "eql?",    r_gmpz_eql, 1);
  rb_define_method(cGMP_Z, "hash",    r_gmpz_hash, 0);
  
  // Integer Logic and Bit Fiddling
  rb_define_method(cGMP_Z, "&",        r_gmpz_and, 1);
  rb_define_method(cGMP_Z, "|",        r_gmpz_or, 1);
  rb_define_method(cGMP_Z, "^",        r_gmpz_xor, 1);
  rb_define_method(cGMP_Z, "com",      r_gmpz_com, 0);
  rb_define_method(cGMP_Z, "com!",     r_gmpz_com_self, 0);
  rb_define_method(cGMP_Z, "popcount", r_gmpz_popcount, 0);
  rb_define_method(cGMP_Z, "scan0",    r_gmpz_scan0, 1);
  rb_define_method(cGMP_Z, "scan1",    r_gmpz_scan1, 1);
  rb_define_method(cGMP_Z, "[]=",      r_gmpz_setbit, 2);
  rb_define_method(cGMP_Z, "[]",       r_gmpz_getbit, 1);
  
  // Miscellaneous Integer Functions
  rb_define_method(cGMP_Z, "even?", r_gmpz_is_even, 0);
  rb_define_method(cGMP_Z, "odd?", r_gmpz_is_odd, 0);
  rb_define_method(cGMP_Z, "sizeinbase", r_gmpz_sizeinbase, 1);
  rb_define_alias( cGMP_Z, "size_in_base", "sizeinbase");
  rb_define_method(cGMP_Z, "size_in_bin", r_gmpz_size_in_bin, 0);
  
  // Integer Special Functions
  rb_define_method(cGMP_Z, "size", r_gmpz_size, 0);
  
  // _unsorted_
  rb_define_method(cGMP_Z, ">>",  r_gmpz_fshr, 1);
  rb_define_method(cGMP_Z, "lastbits_pos",  r_gmpz_fshrm, 1);

}
