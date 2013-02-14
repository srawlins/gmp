#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

#ifdef HAVE_RUBY_IO_H
#include <ruby/io.h>
#endif

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

#define DEFUN_INT_SINGLETON_UIUI(fname,mpz_fname)                     \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE arg1, VALUE arg2)    \
{                                                                     \
  MP_INT *arg1_val_z, *res_val;                                       \
  unsigned long arg1_val_ul, arg2_val_ul;                             \
  VALUE res;                                                          \
                                                                      \
  (void)klass;                                                        \
                                                                      \
  if (FIXNUM_P (arg1)) {                                              \
    arg1_val_ul = FIX2NUM (arg1);                                     \
  } else if (GMPZ_P (arg1)) {                                         \
    mpz_get_struct (arg1, arg1_val_z);                                \
    if (!mpz_fits_ulong_p (arg1_val_z))                               \
      rb_raise (rb_eRangeError, "first argument out of range");       \
    arg1_val_ul = mpz_get_ui (arg1_val_z);                            \
    if (arg1_val_ul == 0)                                             \
      rb_raise (rb_eRangeError, "first argument out of range");       \
  } else {                                                            \
    typeerror_as (ZX, "first argument");                              \
  }                                                                   \
                                                                      \
  if (FIXNUM_P (arg2)) {                                              \
    arg2_val_ul = FIX2NUM (arg2);                                     \
  } else {                                                            \
    typeerror_as (X, "second argument");                              \
  }                                                                   \
                                                                      \
  mpz_make_struct_init (res, res_val);                                \
  mpz_fname (res_val, arg1_val_ul, arg2_val_ul);                      \
  return res;                                                         \
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
 * GMP::Z as rop, a GMP::Z as op1, and a GMP::Z, Bignum, or Fixnum as op2. It calls
 * mpz_fname, whose arguments are rop (the return argument), op1, and op2. If op2 is a
 * Fixnum, and >= 0, the ui variant of mpz_fname will be used.
 *
 * TODO: Accept Fixnum, Bignum as op1 and just convert to GMP::Z.
 */
#define FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(fname,mpz_fname)  \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE rop, VALUE op1, VALUE op2)  \
{                                                               \
  MP_INT *rop_val, *op1_val, *op2_val;                          \
  (void)klass;                                                  \
                                                                \
  if (! GMPZ_P (rop)) {                                         \
    typeerror_as(Z, "rop");                                     \
  }                                                             \
  mpz_get_struct (rop, rop_val);                                \
                                                                \
  if (! GMPZ_P (op1)) {                                         \
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
 * ...
 */
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(add,mpz_add)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(addmul,mpz_addmul)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(submul,mpz_submul)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(divexact,mpz_divexact)
FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID(lcm,mpz_lcm)

/*
 * 02 mpz_t_or_ui__mpz_t_or_ui__to__mpz_t__returns__void
 * FUNC_MAP__ZUI_ZUI__TO__Z__RETURNS__VOID defines a GMP::Z singleton function that takes
 * a GMP::Z as rop, a GMP::Z, Bignum, or Fixnum as op1, and a GMP::Z, Bignum, or Fixnum
 * as op2. It calls mpz_fname, whose arguments are rop (the return argument), op1, and
 * op2. If op1 is a Fixnum and >= 0, xor if op2 is a Fixnum and >= 0, one ui variant or
 * the other of mpz_fname will be used:
 *
 *       op2 --> |  FIXNUM >=0  |  FIXNUM < 0  |    BIGNUM    |    GMP::Z    |
 *     v op1 v   +--------------+--------------+--------------+--------------+
 *   FIXNUM >= 0 | mpz_ui_fname | mpz_ui_fname | mpz_ui_fname | mpz_ui_fname |
 *   FIXNUM <  0 | mpz_fname_ui | mpz_fname    | mpz_fname    | mpz_fname    |
 *   BIGNUM      | mpz_fname_ui | mpz_fname    | mpz_fname    | mpz_fname    |
 *   GMP::Z      | mpz_fname_ui | mpz_fname    | mpz_fname    | mpz_fname    |
 *
 * TODO: Maybe take si's, negate them, and perform mpz_additive_inverse_fname *choke*
 */
#define FUNC_MAP__ZUI_ZUI__TO__Z__RETURNS__VOID(fname,mpz_fname)  \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE rop, VALUE op1, VALUE op2)  \
{                                                                 \
  MP_INT *rop_val, *op1_val, *op2_val;                            \
  int free_op1_val = 0;                                           \
  (void)klass;                                                    \
                                                                  \
  if (! GMPZ_P (rop)) {                                           \
    typeerror_as(Z, "rop");                                       \
  }                                                               \
  mpz_get_struct (rop, rop_val);                                  \
                                                                  \
  if (FIXNUM_P (op1) && FIX2NUM (op1) >= 0) {                     \
      if (FIXNUM_P (op2)) {                                       \
        mpz_set_si (rop_val, FIX2NUM (op2));                      \
        mpz_ui_##fname (rop_val, FIX2NUM (op1), rop_val);         \
      } else if (BIGNUM_P (op2)) {                                \
        mpz_set_bignum (rop_val, op2);                            \
        mpz_ui_##fname (rop_val, FIX2NUM (op1), rop_val);         \
      } else if (GMPZ_P (op2)) {                                  \
        mpz_get_struct (op2, op2_val);                            \
        mpz_ui_##fname (rop_val, FIX2NUM (op1), op2_val);         \
      } else {                                                    \
        typeerror_as (ZXB, "op2");                                \
      }                                                           \
  } else {                                                        \
    if (FIXNUM_P (op1)) {                                         \
      mpz_temp_alloc (op1_val);                                   \
      mpz_init_set_si (op1_val, FIX2NUM (op1));                   \
      free_op1_val = 1;                                           \
    } else if (BIGNUM_P (op1)) {                                  \
      mpz_temp_from_bignum (op1_val, op1);                        \
      free_op1_val = 1;                                           \
    } else if (GMPZ_P (op1)) {                                    \
      mpz_get_struct (op1, op1_val);                              \
    } else {                                                      \
      typeerror_as (ZXB, "op1");                                  \
    }                                                             \
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
    if (free_op1_val)                                             \
      mpz_temp_free (op1_val);                                    \
  }                                                               \
                                                                  \
  return Qnil;                                                    \
}

FUNC_MAP__ZUI_ZUI__TO__Z__RETURNS__VOID(sub,mpz_sub)

/*
 * 03 mpz_t__mpz_t_or_si_or_ui__to__mpz_t__returns__void
 * FUNC_MAP__Z_ZSIUI__TO__Z__RETURNS__VOID defines a GMP::Z singleton function that takes
 * a GMP::Z as rop, a GMP::Z as op1, and a GMP::Z, Bignum, or Fixnum as op2. It calls
 * mpz_fname, whose arguments are rop (the return argument), op1, and op2. If op2 is a
 * Fixnum, then if op2 >= 0 the ui variant of mpz_fname (else the si variant of
 * mpz_fname) will be used.
 *
 * TODO: Accept Fixnum, Bignum as op1 and just convert to GMP::Z.
 */
#define FUNC_MAP__Z_ZSIUI__TO__Z__RETURNS__VOID(fname,mpz_fname)  \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE rop, VALUE op1, VALUE op2)  \
{                                                                 \
  MP_INT *rop_val, *op1_val, *op2_val;                            \
  (void)klass;                                                    \
                                                                  \
  if (! GMPZ_P (rop)) {                                           \
    typeerror_as(Z, "rop");                                       \
  }                                                               \
  mpz_get_struct (rop, rop_val);                                  \
                                                                  \
  if (! GMPZ_P (op1)) {                                           \
    typeerror_as (Z, "op1");                                      \
  }                                                               \
  mpz_get_struct (op1, op1_val);                                  \
                                                                  \
  if (FIXNUM_P (op2)) {                                           \
    if (FIX2NUM (op2) >= 0) {                                     \
      mpz_fname##_ui (rop_val, op1_val, FIX2NUM (op2));           \
    } else {                                                      \
      mpz_fname##_si (rop_val, op1_val, FIX2NUM (op2));           \
    }                                                             \
  } else if (BIGNUM_P (op2)) {                                    \
    mpz_set_bignum (rop_val, op2);                                \
    mpz_fname (rop_val, op1_val, rop_val);                        \
  } else if (GMPZ_P (op2)) {                                      \
    mpz_get_struct (op2, op2_val);                                \
    mpz_fname (rop_val, op1_val, op2_val);                        \
  } else {                                                        \
    typeerror_as (ZXB, "op2");                                    \
  }                                                               \
                                                                  \
  return Qnil;                                                    \
}

FUNC_MAP__Z_ZSIUI__TO__Z__RETURNS__VOID(mul,mpz_mul)

/*
 * 04 mpz_t__mp_bitcnt_t__to__mpz_t__returns__void
 * FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID defines a GMP::Z singleton function that takes a
 * GMP::Z as rop, a GMP::Z as op1, and a Fixnum as op2. It calls mpz_fname, whose
 * arguments are rop (the return argument), op1, and op2. op2 must be >= 0.
 *
 * TODO: Accept Fixnum, Bignum as op1 and just convert to GMP::Z.
 */
#define FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(fname,mpz_fname)   \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE rop, VALUE op1, VALUE op2)  \
{                                                                   \
  MP_INT *rop_val, *op1_val;                                        \
  (void)klass;                                                      \
                                                                    \
  if (! GMPZ_P (rop)) {                                             \
    typeerror_as(Z, "rop");                                         \
  }                                                                 \
  mpz_get_struct (rop, rop_val);                                    \
                                                                    \
  if (! GMPZ_P (op1)) {                                             \
    typeerror_as (Z, "op1");                                        \
  }                                                                 \
  mpz_get_struct (op1, op1_val);                                    \
                                                                    \
  if (! FIXNUM_P (op2)) {                                           \
    typeerror_as (X, "op2");                                        \
  }                                                                 \
                                                                    \
  if (FIX2NUM (op2) >= 0) {                                         \
    mpz_fname (rop_val, op1_val, FIX2NUM (op2));                    \
  } else {                                                          \
      rb_raise(rb_eRangeError, "op2 (Fixnum) must be nonnegative"); \
  }                                                                 \
                                                                    \
  return Qnil;                                                      \
}

FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(mul_2exp,mpz_mul_2exp)
FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(cdiv_q_2exp,mpz_cdiv_q_2exp)
FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(cdiv_r_2exp,mpz_cdiv_r_2exp)
FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(fdiv_q_2exp,mpz_fdiv_q_2exp)
FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(fdiv_r_2exp,mpz_fdiv_r_2exp)
FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(tdiv_q_2exp,mpz_tdiv_q_2exp)
FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID(tdiv_r_2exp,mpz_tdiv_r_2exp)

/*
 * 05 mpz_t__mpz_t__to__mpz_t__returns__void
 * FUNC_MAP__Z__TO__Z__RETURNS__VOID defines a GMP::Z singleton function that takes a
 * GMP::Z as rop, and a GMP::Z as op1. It calls mpz_fname, whose arguments are rop (the
 * return argument), and op1.
 *
 * TODO: Accept Fixnum, Bignum as op1 and just convert to GMP::Z.
 */
#define FUNC_MAP__Z__TO__Z__RETURNS__VOID(fname,mpz_fname)   \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE rop, VALUE op1)  \
{                                                                   \
  MP_INT *rop_val, *op1_val;                                        \
  (void)klass;                                                      \
                                                                    \
  if (! GMPZ_P (rop)) {                                             \
    typeerror_as(Z, "rop");                                         \
  }                                                                 \
  mpz_get_struct (rop, rop_val);                                    \
                                                                    \
  if (! GMPZ_P (op1)) {                                             \
    typeerror_as (Z, "op1");                                        \
  }                                                                 \
  mpz_get_struct (op1, op1_val);                                    \
                                                                    \
  mpz_fname (rop_val, op1_val);                            \
                                                                    \
  return Qnil;                                                      \
}

FUNC_MAP__Z__TO__Z__RETURNS__VOID(neg,mpz_neg)
FUNC_MAP__Z__TO__Z__RETURNS__VOID(abs,mpz_abs)
FUNC_MAP__Z__TO__Z__RETURNS__VOID(sqrt,mpz_sqrt)
FUNC_MAP__Z__TO__Z__RETURNS__VOID(nextprime,mpz_nextprime)
FUNC_MAP__Z__TO__Z__RETURNS__VOID(com,mpz_com)

/*
 * 09 mpz_t__mpz_t_or_ui__to__none__returns__int
 * FUNC_MAP__Z_Z__TO__VOID__RETURNS__BOOL defines a GMP::Z singleton function that
 * doesn't take a rop, a GMP::Z as op1, and a GMP::Z, Fixnum, or Bignum as op2. It calls
 * mpz_fname[_ui]_p, whose arguments are op1 and op2.
 *
 * TODO: Accept Fixnum, Bignum as op1 and just convert to GMP::Z.
 */
#define FUNC_MAP__Z_Z__TO__VOID__RETURNS__BOOL(fname,mpz_fname)     \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE op1, VALUE op2)    \
{                                                                   \
  MP_INT *op1_val, *op2_val;                                        \
  int res;                                                          \
  (void)klass;                                                      \
                                                                    \
  if (! GMPZ_P (op1)) {                                             \
    typeerror_as (Z, "op1");                                        \
  }                                                                 \
  mpz_get_struct (op1, op1_val);                                    \
                                                                    \
  if (FIXNUM_P (op2)) {                                             \
    if (FIX2NUM (op2) >= 0) {                                       \
      res = mpz_fname##_ui_p (op1_val, FIX2NUM (op2));              \
    } else {                                                        \
      mpz_temp_alloc (op2_val);                                     \
      mpz_init_set_si (op2_val, FIX2NUM (op2));                     \
      res = mpz_fname##_p (op1_val, op2_val);                       \
      mpz_temp_free (op2_val);                                      \
    }                                                               \
  } else if (BIGNUM_P (op2)) {                                      \
    mpz_temp_from_bignum (op2_val, op2);                            \
    res = mpz_fname##_p (op1_val, op2_val);                         \
    mpz_temp_free (op2_val);                                        \
  } else if (GMPZ_P (op2)) {                                        \
    mpz_get_struct (op2, op2_val);                                  \
    res = mpz_fname##_p (op1_val, op2_val);                         \
  } else {                                                          \
    typeerror_as (ZXB, "op2");                                      \
  }                                                                 \
                                                                    \
  return (res ? Qtrue : Qfalse);                                    \
}

FUNC_MAP__Z_Z__TO__VOID__RETURNS__BOOL(divisible,mpz_divisible)

/*
 * 11 mpz_t__mpz_t_or_ui__mpz_t_or_ui__to__none__returns__int
 * FUNC_MAP__Z_ZXB_ZXB__TO__VOID__RETURNS__BOOL defines a GMP::Z singleton function that
 * doesn't take a rop, a GMP::Z as op1, and a GMP::Z, Fixnum, or Bignum as both op2 and
 * op3. It calls mpz_fname[_ui]_p, whose arguments are op1, op2, and op3.
 *
 * TODO: Accept Fixnum, Bignum as op1 and just convert to GMP::Z.
 */
#define FUNC_MAP__Z_ZXB_ZXB__TO__VOID__RETURNS__BOOL(fname,mpz_fname)  \
static VALUE r_gmpzsg_##fname(VALUE klass, VALUE op1, VALUE op2, VALUE op3)  \
{                                                                   \
  MP_INT *op1_val, *op2_val, *op3_val;                              \
  int res;                                                          \
  int free_op2_val = 0;                                             \
  int free_op3_val = 0;                                             \
  (void)klass;                                                      \
                                                                    \
  if (! GMPZ_P (op1)) {                                             \
    typeerror_as (Z, "op1");                                        \
  }                                                                 \
  mpz_get_struct (op1, op1_val);                                    \
                                                                    \
  if (FIXNUM_P (op2)) {                                             \
    if (FIX2NUM (op2) >= 0) {                                       \
      if (FIXNUM_P (op3) && FIX2NUM (op3) >= 0) {                   \
        res = mpz_fname##_ui_p (op1_val, FIX2NUM (op2), FIX2NUM (op3));  \
        return (res ? Qtrue : Qfalse);                              \
      }                                                             \
    }                                                               \
    mpz_temp_alloc (op2_val);                                       \
    mpz_init_set_si (op2_val, FIX2NUM (op2));                       \
    free_op2_val = 1;                                               \
  } else if (BIGNUM_P (op2)) {                                      \
    mpz_temp_from_bignum (op2_val, op2);                            \
    free_op2_val = 1;                                               \
  } else if (GMPZ_P (op2)) {                                        \
    mpz_get_struct (op2, op2_val);                                  \
  } else {                                                          \
    typeerror_as (ZXB, "op2");                                      \
  }                                                                 \
                                                                    \
  if (FIXNUM_P (op3)) {                                             \
      mpz_temp_alloc (op3_val);                                     \
      mpz_init_set_si (op3_val, FIX2NUM (op3));                     \
      free_op3_val = 1;                                             \
  } else if (BIGNUM_P (op3)) {                                      \
    mpz_temp_from_bignum (op3_val, op3);                            \
    free_op3_val = 1;                                               \
  } else if (GMPZ_P (op3)) {                                        \
    mpz_get_struct (op3, op3_val);                                  \
  } else {                                                          \
    typeerror_as (ZXB, "op3");                                      \
  }                                                                 \
                                                                    \
  res = mpz_fname##_p (op1_val, op2_val, op3_val);                  \
  if (free_op2_val) { free(op2_val); }                              \
  if (free_op3_val) { free(op3_val); }                              \
                                                                    \
  return (res ? Qtrue : Qfalse);                                    \
}

FUNC_MAP__Z_ZXB_ZXB__TO__VOID__RETURNS__BOOL(congruent,mpz_congruent)


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
 *   GMP::Z.new(5)         #=> 5
 *   GMP::Z.new(2**32)     #=> 4_294_967_296
 *   GMP::Z.new(2**101)    #=> 2_535_301_200_456_458_802_993_406_410_752
 *   GMP::Z.new("20")      #=> 20
 *   GMP::Z.new("0x20")    #=> 32
 *   GMP::Z.new("020")     #=> 16
 *   GMP::Z.new("0b101")   #=> 5
 *   GMP::Z.new("20", 16)  #=> 32
 *   GMP::Z.new("1Z", 36)  #=> 71
 */
VALUE r_gmpzsg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_INT *res_val;
  VALUE res;
  (void)klass;

  if (argc > 2)
    rb_raise(rb_eArgError, "wrong # of arguments (%d for 0, 1, or 2)", argc);

  mpz_make_struct(res, res_val);
  mpz_init(res_val);
  rb_obj_call_init(res, argc, argv);

  return res;
}

static VALUE r_gmpz_alloc(VALUE klass) {
  MP_INT *z;
  VALUE obj;

  //mpz_init(z);
  //obj = Data_Make_Struct(klass, MP_INT, 0, r_gmpz_free, z);
  //obj = Data_Wrap_Struct(klass, 0, r_gmpz_free, z);
//  mpz_make_struct_init (z, obj);
 // mpz_make_struct_init (obj, z);
  obj = Data_Make_Struct(klass, MP_INT, 0, r_gmpz_free, z);
  mpz_init (z);

  return obj;
}

VALUE r_gmpz_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_INT *self_val;
  int base = 0;

  // Set up the base if 2 arguments are passed
  if (argc == 2) { // only ok if String, Fixnum
    if (STRING_P(argv[0])) {  // first arg must be a String
      if (FIXNUM_P(argv[1])) {  //  second arg must be a Fixnum
        base = FIX2INT(argv[1]);
        if ( base != 0 && ( base < 2 || base > 62) )
          rb_raise (rb_eRangeError, "base must be either 0 or between 2 and 62");
      } else {
        rb_raise (rb_eTypeError, "base must be a Fixnum between 2 and 62, not a %s.", rb_class2name (rb_class_of (argv[1])));
      }
    } else {
      rb_raise(
        rb_eTypeError,
        "GMP::Z.new() must be passed a String as the 1st argument (not a %s), if a base is passed as the 2nd argument.",
        rb_class2name (rb_class_of (argv[0]))
      );
    }
  }

  if (argc != 0) {
    mpz_get_struct (self,self_val);
    mpz_set_value (self_val, argv[0], base);
  }
  return Qnil;
}

static VALUE r_gmpz_initialize_copy(VALUE copy, VALUE orig) {
  MP_INT *orig_z, *copy_z;

  if (copy == orig) return copy;

  if (TYPE(orig) != T_DATA) {
    rb_raise(rb_eTypeError, "wrong argument type");
  }

  mpz_get_struct (orig, orig_z);
  mpz_get_struct (copy, copy_z);
  mpz_set (copy_z, orig_z);

  return copy;
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
void mpz_set_value(MP_INT *target, VALUE source, int base)
{
  MP_INT *source_val;

  if (GMPZ_P (source)) {
    mpz_get_struct (source, source_val);
    mpz_set (target, source_val);
  } else if (TYPE (source) == T_FIXNUM) {
    mpz_set_si (target, FIX2NUM (source));
  } else if (STRING_P (source)) {
    mpz_set_str (target, StringValuePtr (source), base);
  } else if (BIGNUM_P (source)) {
    mpz_set_bignum (target, source);
  } else {
    rb_raise (rb_eTypeError, "Don't know how to convert %s into GMP_Z", rb_class2name (rb_class_of (source)));
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

  mpz_get_struct (self, self_val);
  if (mpz_fits_slong_p (self_val)) {
#ifdef RUBY_ENGINE_JRUBY
    /* JRuby has this as INT2FIX which is no good. Patch. */
    return FIXABLE (mpz_get_si (self_val)) ? LONG2FIX (mpz_get_si (self_val)) : rb_ll2inum (mpz_get_si (self_val));
#else
    return rb_int2inum (mpz_get_si (self_val));
#endif
  }

  str = mpz_get_str (NULL, 0, self_val);
  res = rb_cstr2inum (str, 10);
  free (str);
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
    /* TODO: use mpz_mul_ui */
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

  if (GMPZ_P (b)) {
    mpz_get_struct (b, b_val);
  } else if (FIXNUM_P (b)) {
    mpz_temp_alloc (b_val);
    mpz_init_set_si (b_val, FIX2NUM (b));
    free_b_val = 1;
  } else if (BIGNUM_P (b)) {
    mpz_temp_from_bignum (b_val, b);
    free_b_val = 1;
  } else {
    typeerror_as (ZXB, "addend");
  }
  mpz_get_struct (self, self_val);

  if (GMPZ_P (c)) {
    mpz_get_struct (c, c_val);
    mpz_addmul (self_val, b_val, c_val);
  } else if (TYPE (c) == T_FIXNUM) {
    if (FIX2NUM (c) < 0)
    {
      if (free_b_val) { mpz_temp_free (b_val); }
      rb_raise (rb_eRangeError, "multiplicand (Fixnum) must be nonnegative");
    }
    mpz_addmul_ui (self_val, b_val, FIX2NUM (c));
  } else if (BIGNUM_P (c)) {
    mpz_temp_from_bignum (c_val, c);
    mpz_addmul (self_val, b_val, c_val);
    mpz_temp_free (c_val);
  } else {
    if (free_b_val)
      mpz_temp_free (b_val);
    typeerror_as (ZXB, "multiplicand");
  }
  if (free_b_val)
    mpz_temp_free (b_val);
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

  if (GMPZ_P (c)) {
    mpz_get_struct (c, c_val);
    mpz_submul (self_val, b_val, c_val);
  } else if (TYPE (c) == T_FIXNUM) {
    if (FIX2NUM (c) < 0)
    {
      if (free_b_val) { mpz_temp_free (b_val); }
      rb_raise (rb_eRangeError, "multiplicand (Fixnum) must be nonnegative");
    }
    mpz_submul_ui (self_val, b_val, FIX2NUM (c));
  } else if (BIGNUM_P (c)) {
    mpz_temp_from_bignum (c_val, c);
    mpz_submul (self_val, b_val, c_val);
    mpz_temp_free (c_val);
  } else {
    if (free_b_val)
      mpz_temp_free (b_val);
  //  rb_raise (rb_eTypeError, "base must be a Fixnum between 2 and 62, not a %s.", rb_class2name (rb_class_of (c)));
    typeerror_as (ZXB, "multiplicand");
  }
  if (free_b_val)
    mpz_temp_free (b_val);
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
 *
 * If _b_ is a GMP::Z, Fixnum, GMP::Q, or Bignum, then no division is actually
 * performed. Instead, we simply construct a new GMP::Q number, using _a_ and
 * _b_ as the numerator and denominator (not exactly true for the GMP::Q case).
 *
 * If _b_ is a GMP::F, then the result is calculated via `mpf_div`.
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
    mpz_temp_alloc (arg_val);
    mpz_init_set_ui (arg_val, FIX2NUM (arg));
    res = mpz_divisible_ui_p (self_val, FIX2NUM (arg));
    mpz_temp_free (arg_val);
  } else if (TYPE (arg) == T_FIXNUM) {
    mpz_temp_alloc (arg_val);
    mpz_make_struct_init (arg, arg_val);
    mpz_init_set_si (arg_val, FIX2NUM (arg));
    res = mpz_divisible_p (self_val, arg_val);
    mpz_temp_free (arg_val);
  } else if (BIGNUM_P (arg)) {
    mpz_temp_from_bignum (arg_val, arg);
    res = mpz_divisible_p (self_val, arg_val);
    mpz_temp_free (arg_val);
  } else if (GMPZ_P (arg)) {
    mpz_get_struct (arg, arg_val);
    res = mpz_divisible_p (self_val, arg_val);
  } else {
    typeerror_as (ZXB, "argument");
  }
  return (res != 0) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   n.congruent?(c, d)
 *
 * @since 0.6.19
 *
 * Returns true if _n_ is congruent to _c_ modulo _d_. _c_ and _d_ can be an instance any of the following:
 * * GMP::Z
 * * Fixnum
 * * Bignum
 */
static VALUE r_gmpz_congruent(VALUE self_val, VALUE c_val, VALUE d_val)
{
  MP_INT *self, *c, *d;
  int res, free_c, free_d;
  mpz_get_struct (self_val, self);
  free_c = free_d = 0;

  if (FIXNUM_P (c_val) && FIX2NUM (c_val) > 0 &&
      FIXNUM_P (d_val) && FIX2NUM (d_val) > 0) {
    res = mpz_congruent_ui_p (self, FIX2NUM (c_val), FIX2NUM (d_val));
  } else {
    if (FIXNUM_P (c_val)) {
      mpz_make_struct_init (c_val, c);
      mpz_init_set_si (c, FIX2NUM (c_val));
    } else if (BIGNUM_P (c_val)) {
      mpz_temp_from_bignum (c, c_val);
      free_c = 1;
    } else if (GMPZ_P (c_val)) {
      mpz_get_struct (c_val, c);
    } else {
      typeerror_as (ZXB, "c");
    }

    if (FIXNUM_P (d_val)) {
      mpz_make_struct_init (d_val, d);
      mpz_init_set_si (d, FIX2NUM (d_val));
    } else if (BIGNUM_P (d_val)) {
      mpz_temp_from_bignum (d, d_val);
      free_d = 1;
    } else if (GMPZ_P (d_val)) {
      mpz_get_struct (d_val, d);
    } else {
      if (free_c) { mpz_temp_free (c); }
      typeerror_as (ZXB, "d");
    }

    res = mpz_congruent_p (self, c, d);
    if (free_c) { mpz_temp_free (c); }
    if (free_d) { mpz_temp_free (d); }
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
 * Document-method: rootrem
 *
 * call-seq:
 *   a.rootrem(b)
 *
 * Returns the truncated integer part of the <i>b</i>th root of _a_, and the remainder, _a - root**b_.
 */
static VALUE r_gmpz_rootrem(VALUE self_val, VALUE exp_val)
{
  MP_INT *self, *root, *rem;
  VALUE root_val, rem_val, ary;
  unsigned long exp;

  if (FIXNUM_P(exp_val)) {
    if (FIX2NUM(exp_val) < 0)
      rb_raise(rb_eRangeError, "root number out of range");
    exp = FIX2NUM(exp_val);
  } else if (GMPZ_P(exp_val)) {
    mpz_get_struct(exp_val, root);
    if (!mpz_fits_ulong_p(root))
      rb_raise(rb_eRangeError, "root number out of range");
    exp = mpz_get_ui(root);
    if (exp_val == 0)
      rb_raise(rb_eRangeError, "root number out of range");
  } else {
    typeerror_as(ZX, "root number");
  }

  mpz_make_struct_init(root_val, root);
  mpz_make_struct_init(rem_val, rem);
  mpz_get_struct(self_val, self);
  mpz_rootrem(root, rem, self, exp);

  ary = rb_ary_new3 (2, root_val, rem_val);
  return ary;
}

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
    mpz_temp_alloc (arg_val);
    mpz_init_set_ui (arg_val, FIX2NUM (arg));
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
    mpz_temp_free (arg_val);

  ary = rb_ary_new3 (3, res, s, t);
  return ary;
}

/*
 * call-seq:
 *   a.gcdext2(b) #=> g, s
 *
 * @since 0.5.x
 *
 * Returns the greatest common divisor of _a_ and _b_, in addition to _s_, the
 * coefficient satisfying <i>a*s + b*t = g</i>. _g_ is always positive, even if one or
 * both of _a_ and _b_ are negative. _s_ and _t_ are chosen such that
 * <i>abs(s) <= abs(b)</i> and <i>abs(t) <= abs(a)</i>.
 */
VALUE r_gmpz_gcdext2(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val, *s_val;
  VALUE res, s, ary;
  int free_arg_val = 0;

  mpz_get_struct (self,self_val);

  if (GMPZ_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_make_struct_init (s, s_val);
    mpz_get_struct (arg, arg_val);
    mpz_gcdext (res_val, s_val, NULL, self_val, arg_val);
  } else if (FIXNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_make_struct_init (s, s_val);
    mpz_temp_alloc (arg_val);
    mpz_init_set_ui (arg_val, FIX2NUM(arg));
    free_arg_val = 1;
    mpz_gcdext (res_val, s_val, NULL, self_val, arg_val);
  } else if (BIGNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_make_struct_init (s, s_val);
    mpz_set_bignum (res_val, arg);
    mpz_gcdext (res_val, s_val, NULL, res_val, self_val);
  } else {
    typeerror (ZXB);
  }

  if (free_arg_val)
    mpz_temp_free (arg_val);

  ary = rb_ary_new3 (2, res, s);
  return ary;
}

/*
 * call-seq:
 *   a.lcm(b)
 *
 * @since 0.2.11
 *
 * Returns the least common multiple of a and b. The result is always positive even if
 * one or both of _a_ or _b_ are negative.
 */
VALUE r_gmpz_lcm(VALUE self, VALUE arg)
{
  MP_INT *self_val, *arg_val, *res_val;
  VALUE res;

  mpz_get_struct (self,self_val);

  if (GMPZ_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_get_struct (arg, arg_val);
    mpz_lcm (res_val, self_val, arg_val);
  } else if (FIXNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_lcm_ui (res_val, self_val, FIX2NUM(arg));
  } else if (BIGNUM_P (arg)) {
    mpz_make_struct_init (res, res_val);
    mpz_set_bignum (res_val, arg);
    mpz_lcm (res_val, res_val, self_val);
  } else {
    typeerror (ZXB);
  }
  return res;
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
#if __GNU_MP_VERSION >= 5 && __GNU_MP_VERSION_MINOR >= 1  // 5.1.0 and newer

/*
 * Document-method: GMP::Z.2fac
 *
 * call-seq:
 *   GMP::Z.send(:"2fac", n)
 *   GMP::Z.double_fac(n)
 *
 * Returns <i>n!!</i>, the double factorial of _n_.
 *
 * Examples:
 * * GMP::Z.double_fac(  0)  #=>    1
 * * GMP::Z.double_fac(  1)  #=>    1
 * * GMP::Z.double_fac(  2)  #=>    2
 * * GMP::Z.double_fac(  3)  #=>    3
 * * GMP::Z.double_fac(  4)  #=>    8
 * * GMP::Z.double_fac(  5)  #=>   15
 * * GMP::Z.double_fac(  6)  #=>   48
 * * GMP::Z.double_fac(  7)  #=>  105
 * * GMP::Z.double_fac(  8)  #=>  384
 * * GMP::Z.double_fac(  9)  #=>  945
 * * GMP::Z.double_fac( 10)  #=> 3840
 * * GMP::Z.double_fac(100)
 *     #=> 34243224702511976248246432895208185975118675053719198827915654463488000000000000
 */
DEFUN_INT_SINGLETON_UI(2fac,       mpz_2fac_ui)

/*
 * Document-method: GMP::Z.mfac
 *
 * call-seq:
 *   GMP::Z.mfac(n, m)
 *
 * Returns <i>n!^(m)</i>, the m-multi-factorial of _n_.
 *
 * Examples:
 * * GMP::Z.mfac(0,   3)  #=>    1
 * * GMP::Z.mfac(1,   3)  #=>    1
 * * GMP::Z.mfac(2,   3)  #=>    2
 * * GMP::Z.mfac(3,   3)  #=>    3
 * * GMP::Z.mfac(4,   3)  #=>    4
 * * GMP::Z.mfac(5,   3)  #=>   10
 * * GMP::Z.mfac(6,   3)  #=>   18
 * * GMP::Z.mfac(7,   3)  #=>   28
 * * GMP::Z.mfac(8,   3)  #=>   80
 * * GMP::Z.mfac(9,   3)  #=>  162
 * * GMP::Z.mfac(10,  3)  #=>  280
 * * GMP::Z.mfac(11,  3)  #=>  880
 * * GMP::Z.mfac(12,  3)  #=> 1944
 */
DEFUN_INT_SINGLETON_UIUI(mfac,     mpz_mfac_uiui)

/*
 * Document-method: GMP::Z.primorial
 *
 * call-seq:
 *   GMP::Z.primorial(n)
 *
 * Returns the primorial of _n_.
 *
 * Examples:
 * * GMP::Z.primorial(0)  #=>   1
 * * GMP::Z.primorial(1)  #=>   1
 * * GMP::Z.primorial(2)  #=>   2
 * * GMP::Z.primorial(3)  #=>   6
 * * GMP::Z.primorial(4)  #=>   6
 * * GMP::Z.primorial(5)  #=>  30
 * * GMP::Z.primorial(6)  #=>  30
 * * GMP::Z.primorial(7)  #=> 210
 */
DEFUN_INT_SINGLETON_UI(primorial,  mpz_primorial_ui)
#endif

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
DEFUN_INT_SINGLETON_UI(fib,    mpz_fib_ui)
DEFUN_INT_SINGLETON_UI(lucnum, mpz_lucnum_ui)


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
 * b.class == GMP::Z, and a.hash == b.hash.
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
 * if and only if b.class == GMP::Z, and a.hash == b.hash.
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
 *   a.hamdist(b)
 *
 * If _a_ and _b_ are both >= 0 or both < 0, calculate the hamming distance between _a_ and _b_. If one operand is >= 0 and the other is less than 0, then return "infinity" (the largest possible `mp_bitcnt_t`.
 * positive.
 */
VALUE r_gmpz_hamdist(VALUE self_val, VALUE b_val)
{
  MP_INT *self, *b;
  mpz_get_struct (self_val, self);
  mpz_get_struct (   b_val,    b);
  if (! GMPZ_P (b_val)) {
    typeerror_as (Z, "b");
  }

  return INT2FIX (mpz_hamdist(self, b));
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
 *    I/O of Integers                                                 *
 **********************************************************************/

/*
 * call-seq:
 *   a.out_raw(stream) #=> Fixnum
 *
 * Output _a_ on IO object _stream_, in raw binary format. The integer is
 * written in a portable format, with 4 bytes of size information, and that
 * many bytes of limbs. Both the size and the limbs are written in decreasing
 * significance order (i.e., in big-endian).
 *
 * The output can be read with `GMP::Z.inp_raw`.
 *
 * Return the number of bytes written, or if an error occurred, return 0.
 */
#ifdef HAVE_RUBY_IO_H
VALUE r_gmpz_out_raw(VALUE self, VALUE stream)
{
  MP_INT *self_val;
  FILE *fd;
  mpz_get_struct(self, self_val);
  if (TYPE (stream) != T_FILE) {
    rb_raise (rb_eTypeError, "stream must be an IO.");
  }
  fd = rb_io_stdio_file (RFILE (stream)->fptr);
  return INT2FIX (mpz_out_raw (fd, self_val));
}

/*
 * call-seq:
 *   GMP::Z.inp_raw(a, stream) #=> Fixnum
 *
 * Input from IO object _stream_ in the format written by `GMP::Z#out_raw`, and
 * put the result in _a_. Return the number of bytes read, or if an error
 * occurred, return 0.
 */
VALUE r_gmpzsg_inp_raw(VALUE klass, VALUE a_val, VALUE stream_val)
{
  MP_INT *a;
  FILE *stream;
  (void)klass;

  if (! GMPZ_P(a_val))
    typeerror_as(Z, "a");

  if (TYPE (stream_val) != T_FILE)
    rb_raise (rb_eTypeError, "stream must be an IO.");

  mpz_get_struct(a_val, a);
  stream = rb_io_stdio_file (RFILE (stream_val)->fptr);
  return INT2FIX (mpz_inp_raw (a, stream));
}
#endif


/**********************************************************************
 *    Integer Import and Export                                       *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::Z.import(str, order = -1) #=> GMP::Z
 *
 * Return a GMP::Z from a String, `str`.
 *
 * `order` can be 1 for most significant word first or -1 for least significant first.
 *
 * There is no sign taken from the data, the result will simply be a positive integer. An application can handle any sign itself, and apply it for instance with `GMP::Z#neg`.
 */
VALUE r_gmpzsg_import(int argc, VALUE *argv, VALUE klass)
{
  MP_INT *res;
  VALUE string_val, order_val, res_val;
  char *string;
  int order, endian;
  size_t nails;
  (void)klass;

  endian = 0;
  nails = 0;

  rb_scan_args (argc, argv, "11", &string_val, &order_val);

  if (NIL_P (order_val))
    order = -1;
  else if (! FIXNUM_P (order_val))
    typeerror_as (X, "order");
  else
    order = FIX2INT (order_val);

  mpz_make_struct(res_val, res);
  mpz_init(res);

  string = StringValuePtr (string_val);

  mpz_import (res, RSTRING_LEN(string_val), order, sizeof(char), endian, nails, string);
  return res_val;
}

/*
 * call-seq:
 *   a.export(order = -1) #=> String
 *
 * Return a String with word data from _a_.
 *
 * `order` can be 1 for most significant word first or -1 for least significant
 * first.
 *
 * If `a` is non-zero then the most significant word produced will be non-zero.
 * `GMP::Z(0).export` returns `""`.
 *
 * The sign of _a_ is ignored, just the absolute value is exported. An
 * application can use `GMP::Z#sgn` to get the sign and handle it as desired.
 */
VALUE r_gmpz_export(int argc, VALUE *argv, VALUE self_val)
{
  MP_INT *self;
  VALUE order_val, res;
  int order, endian;
  size_t countp, nails;
  char *string;

  endian = 0;
  nails = 0;
  mpz_get_struct(self_val, self);

  rb_scan_args (argc, argv, "01", &order_val);

  if (NIL_P (order_val))
    order = -1;
  else if (! FIXNUM_P (order_val))
    typeerror_as (X, "order");
  else
    order = FIX2INT (order_val);

  string = mpz_export (NULL, &countp, order, sizeof(char), endian, nails, self);
  res = rb_str_new (string, countp);
  free (string);

  return res;
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
  rb_define_singleton_method(cGMP_Z, "new",   r_gmpzsg_new, -1);
  rb_define_method(    cGMP_Z, "initialize",      r_gmpz_initialize, -1);
  rb_define_method(    cGMP_Z, "initialize_copy", r_gmpz_initialize_copy, 1);
  rb_define_alloc_func(cGMP_Z,                    r_gmpz_alloc);
  rb_define_method(    cGMP_Z, "swap",            r_gmpz_swap, 1);

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
  rb_define_singleton_method(cGMP_Z, "sub", r_gmpzsg_sub, 3);
  rb_define_singleton_method(cGMP_Z, "mul", r_gmpzsg_mul, 3);
  rb_define_singleton_method(cGMP_Z, "addmul", r_gmpzsg_addmul, 3);
  rb_define_singleton_method(cGMP_Z, "submul", r_gmpzsg_submul, 3);
  rb_define_singleton_method(cGMP_Z, "mul_2exp", r_gmpzsg_mul_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "neg", r_gmpzsg_neg, 2);
  rb_define_singleton_method(cGMP_Z, "abs", r_gmpzsg_abs, 2);

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
  rb_define_method(cGMP_Z, "congruent?",   r_gmpz_congruent, 2);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "divexact",    r_gmpzsg_divexact, 3);
  rb_define_singleton_method(cGMP_Z, "cdiv_q_2exp", r_gmpzsg_cdiv_q_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "cdiv_r_2exp", r_gmpzsg_cdiv_r_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "fdiv_q_2exp", r_gmpzsg_fdiv_q_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "fdiv_r_2exp", r_gmpzsg_fdiv_r_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "tdiv_q_2exp", r_gmpzsg_tdiv_q_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "tdiv_r_2exp", r_gmpzsg_tdiv_r_2exp, 3);
  rb_define_singleton_method(cGMP_Z, "divisible?",  r_gmpzsg_divisible, 2);
  rb_define_singleton_method(cGMP_Z, "congruent?",  r_gmpzsg_congruent, 3);

  // Integer Exponentiation
  rb_define_singleton_method(cGMP_Z, "pow",    r_gmpzsg_pow, 2);
  rb_define_method(cGMP_Z,           "**",     r_gmpz_pow, 1);
  rb_define_method(cGMP_Z,           "powmod", r_gmpz_powm, 2);

  // Integer Roots
  rb_define_method(cGMP_Z, "root",    r_gmpz_root,      1);
  rb_define_method(cGMP_Z, "rootrem", r_gmpz_rootrem,   1);
  rb_define_method(cGMP_Z, "sqrt",    r_gmpz_sqrt,      0);
  rb_define_method(cGMP_Z, "sqrt!",   r_gmpz_sqrt_self, 0);
  rb_define_method(cGMP_Z, "sqrtrem", r_gmpz_sqrtrem,   0);
  rb_define_method(cGMP_Z, "square?", r_gmpz_is_square, 0);
  rb_define_method(cGMP_Z, "power?",  r_gmpz_is_power,  0);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "sqrt", r_gmpzsg_sqrt, 2);

  // Number Theoretic Functions
  rb_define_method(          cGMP_Z, "probab_prime?", r_gmpz_is_probab_prime, -1);
  rb_define_method(          cGMP_Z, "nextprime",     r_gmpz_nextprime, 0);
  rb_define_method(          cGMP_Z, "nextprime!",    r_gmpz_nextprime_self, 0);
  rb_define_alias(           cGMP_Z, "next_prime",    "nextprime");
  rb_define_alias(           cGMP_Z, "next_prime!",   "nextprime!");
  rb_define_method(          cGMP_Z, "gcd",           r_gmpz_gcd,         1);
  rb_define_method(          cGMP_Z, "gcdext",        r_gmpz_gcdext,      1);
  rb_define_method(          cGMP_Z, "gcdext2",       r_gmpz_gcdext2,     1);
  rb_define_method(          cGMP_Z, "lcm",           r_gmpz_lcm,         1);
  rb_define_method(          cGMP_Z, "invert",        r_gmpz_invert,      1);
  rb_define_method(          cGMP_Z, "jacobi",        r_gmpz_jacobi,      1);
  rb_define_singleton_method(cGMP_Z, "jacobi",        r_gmpzsg_jacobi,    2);
  rb_define_method(          cGMP_Z, "legendre",      r_gmpz_legendre,    1);
  rb_define_method(          cGMP_Z, "remove",        r_gmpz_remove,      1);
  rb_define_singleton_method(cGMP_Z, "fac",           r_gmpzsg_fac,       1);
#if __GNU_MP_VERSION >= 5 && __GNU_MP_VERSION_MINOR >= 1  // 5.1.0 and newer
  rb_define_singleton_method(cGMP_Z, "2fac",          r_gmpzsg_2fac,      1);
  rb_define_singleton_method(cGMP_Z, "double_fac",    r_gmpzsg_2fac,      1);
  rb_define_singleton_method(cGMP_Z, "mfac",          r_gmpzsg_mfac,      2);
  rb_define_singleton_method(cGMP_Z, "primorial",     r_gmpzsg_primorial, 1);
#endif
  rb_define_singleton_method(cGMP_Z, "fib",           r_gmpzsg_fib,       1);
  rb_define_singleton_method(cGMP_Z, "lucnum",        r_gmpzsg_lucnum,    1);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "lcm",           r_gmpzsg_lcm,       3);
  rb_define_singleton_method(cGMP_Z, "nextprime",     r_gmpzsg_nextprime, 2);

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
  rb_define_method(cGMP_Z, "&",        r_gmpz_and,      1);
  rb_define_method(cGMP_Z, "|",        r_gmpz_or,       1);
  rb_define_method(cGMP_Z, "^",        r_gmpz_xor,      1);
  rb_define_method(cGMP_Z, "com",      r_gmpz_com,      0);
  rb_define_method(cGMP_Z, "com!",     r_gmpz_com_self, 0);
  rb_define_method(cGMP_Z, "popcount", r_gmpz_popcount, 0);
  rb_define_method(cGMP_Z, "hamdist",  r_gmpz_hamdist,  1);
  rb_define_method(cGMP_Z, "scan0",    r_gmpz_scan0,    1);
  rb_define_method(cGMP_Z, "scan1",    r_gmpz_scan1,    1);
  rb_define_method(cGMP_Z, "[]=",      r_gmpz_setbit,   2);
  rb_define_method(cGMP_Z, "[]",       r_gmpz_getbit,   1);
  // Functional Mappings
  rb_define_singleton_method(cGMP_Z, "com", r_gmpzsg_com, 2);

  // I/O of Integers
#ifdef HAVE_RUBY_IO_H
  rb_define_method(cGMP_Z, "out_raw", r_gmpz_out_raw, 1);
  // Functional Mapping
  rb_define_singleton_method(cGMP_Z, "inp_raw", r_gmpzsg_inp_raw, 2);
#endif

  // Integer Import and Export
  rb_define_singleton_method(cGMP_Z, "import", r_gmpzsg_import, -1);
  rb_define_method(cGMP_Z, "export", r_gmpz_export, -1);

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
