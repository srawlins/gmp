#include <gmpz.h>
#include <gmpq.h>
#include <gmpf.h>

/*
 * Document-class: GMP::Z
 *
 * GMP Multiple Precision Integer.
 *
 * Instances of this class can store variables of the type gmp_randstate_t.
 * This class also contains many methods that act as the functions for
 * gmp_randstate_t variables, as well as a few methods that attempt to make
 * this library more Ruby-ish.
 *
 * The following list is just a simple checklist for me, really. A better
 * reference should be found in the rdocs.
 *
 *   Ruby method    C Extension function     GMP function
 *   new            r_gmprandstatesg_new     gmp_randinit_default
 *   seed           r_gmprandstate_seed      gmp_randseed
 *   \---           \------------------      gmp_randseed_ui
 *   urandomb       r_gmprandstate_urandomb  mpz_urandomb
 */

/**********************************************************************
 *    Random State Initialization                                     *
 **********************************************************************/

/*
 * call-seq:
 *   GMP::RandState.new()
 *   GMP::RandState.new(:mt) #=> uses gmp_randinit_mt
 *   GMP::RandState.new(:lc_2exp, a, c, m2exp) #=> uses gmp_randinit_lc_2exp
 *   GMP::RandState.new(:lc_2exp_size, size) #=> uses gmp_randinit_lc_2exp_size
 *
 * Initializes a new Random State object. Multiple GMP::RandState objects can
 * be instantiated. They may use different generators and the states
 * are kept separate.
 */
VALUE r_gmprandstatesg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_RANDSTATE *rs_val;
  VALUE rs;
  VALUE algorithm, arg2, arg3, arg4;
  ID algorithm_id = rb_intern("default");
  MP_INT *a_val;
  unsigned long c_val, m2exp_val;
  unsigned long size_val;
  int free_a_val = 0;
  
  ID default_algorithm      = rb_intern("default");
  ID mt_algorithm           = rb_intern("mt");
  ID lc_2exp_algorithm      = rb_intern("lc_2exp");
  ID lc_2exp_size_algorithm = rb_intern("lc_2exp_size");
  
  (void)klass;
  
  mprandstate_make_struct(rs, rs_val);
  rb_scan_args(argc, argv, "04", &algorithm, &arg2, &arg3, &arg4);
  if (NIL_P(algorithm))    { algorithm_id = rb_intern("default"); }  /* default value */
  if (SYMBOL_P(algorithm)) { algorithm_id = rb_to_id(algorithm); }
  if (algorithm_id == default_algorithm ||
      algorithm_id == mt_algorithm) {
    if (argc > 1)
      rb_raise(rb_eArgError, "wrong # of arguments (%d for 0 or 1)", argc);
    gmp_randinit_default(rs_val);
  } else if (algorithm_id == lc_2exp_algorithm) {
    if (argc != 4)
      rb_raise(rb_eArgError, "wrong # of arguments (%d for 4)", argc);
    if (GMPZ_P(arg2)) {
      mpz_get_struct(arg2, a_val);
    } else if (FIXNUM_P(arg2)) {
      mpz_temp_alloc(a_val);
      mpz_init_set_ui(a_val, FIX2INT(arg2));
      free_a_val = 1;
    } else if (BIGNUM_P(arg2)) {
      mpz_temp_from_bignum(a_val, arg2);
      free_a_val = 1;
    } else {
      typeerror_as(ZXB, "b");
    }
    c_val     = NUM2LONG(arg3);
    m2exp_val = NUM2LONG(arg4);
    gmp_randinit_lc_2exp(rs_val, a_val, c_val, m2exp_val);
  } else if (algorithm_id == lc_2exp_size_algorithm) {
    if (argc != 2)
      rb_raise(rb_eArgError, "wrong # of arguments (%d for 2)", argc);
    size_val = NUM2LONG(arg2);
    if (size_val > 128)
      rb_raise(rb_eArgError, "size must be within [0..128]");
    int rv = gmp_randinit_lc_2exp_size(rs_val, size_val);
    if (rv == 0)
      rb_raise(rb_eArgError, "could not gmp_randinit_lc_2exp_size with %lu", size_val);
  }
  
  if (free_a_val) { mpz_temp_free(a_val); }
  rb_obj_call_init(rs, argc, argv);
  
  return rs;
}

VALUE r_gmprandstate_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_RANDSTATE *self_val;
  (void)argv;

  if (argc != 0) {
    mprandstate_get_struct(self,self_val);
  }
  return Qnil;
}

/*
 * call-seq:
 *   GMP::RandState(arg)
 *
 * A convenience method for +GMP::RandState.new(arg)+.
 */
VALUE r_gmpmod_randstate(int argc, VALUE *argv, VALUE module)
{
  (void)module;
  return r_gmprandstatesg_new(argc, argv, cGMP_RandState);
}


/**********************************************************************
 *    Random State Initialization                                     *
 **********************************************************************/

/*
 * call-seq:
 *   rand_state.seed(integer)
 *
 * From the GMP Manual:
 *
 * Set an initial seed value into state.
 *
 * The size of a seed determines how many different sequences of random numbers
 * that it's possible to generate. The “quality” of the seed is the randomness
 * of a given seed compared to the previous seed used, and this affects the
 * randomness of separate number sequences. The method for choosing a seed is
 * critical if the generated numbers are to be used for important applications,
 * such as generating cryptographic keys.
 *
 * Traditionally the system time has been used to seed, but care needs to be
 * taken with this. If an application seeds often and the resolution of the
 * system clock is low, then the same sequence of numbers might be repeated.
 * Also, the system time is quite easy to guess, so if unpredictability is
 * required then it should definitely not be the only source for the seed
 * value. On some systems there's a special device /dev/random which provides
 * random data better suited for use as a seed.
 */
VALUE r_gmprandstate_seed(VALUE self, VALUE arg)
{
  MP_RANDSTATE *self_val;
  MP_INT *arg_val;

  mprandstate_get_struct(self,self_val);
  
  if (GMPZ_P(arg)) {
    mpz_get_struct(arg,arg_val);
    gmp_randseed(self_val, arg_val);
  } else if (FIXNUM_P(arg)) {
    gmp_randseed_ui(self_val, FIX2INT(arg));
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    gmp_randseed(self_val, arg_val);
  } else {
    typeerror(ZXB);
  }
  return arg;
}


/**********************************************************************
 *    Integer Random Numbers                                          *
 **********************************************************************/

/*
 * call-seq:
 *   rand_state.urandomb(fixnum)
 *
 * From the GMP Manual:
 *
 * Generate a uniformly distributed random integer in the range 0 to
 * 2^fixnum-1, inclusive. 
 */
VALUE r_gmprandstate_urandomb(VALUE self, VALUE arg)
{
  MP_RANDSTATE *self_val;
  MP_INT *res_val;
  VALUE res;

  mprandstate_get_struct(self,self_val);
  
  if (FIXNUM_P(arg)) {
    mpz_make_struct_init(res, res_val);
    mpz_urandomb(res_val, self_val, FIX2INT(arg));
  } else {
    typeerror(X);
  }
  
  return res;
}

/*
 * call-seq:
 *   rand_state.urandomm(integer)
 *
 * From the GMP Manual:
 *
 * Generate a uniformly distributed random integer in the range 0 to
 * _integer-1_, inclusive. _integer_ can be an instance of GMP::Z,
 *  Fixnum, or Bignum
 */
VALUE r_gmprandstate_urandomm(VALUE self, VALUE arg)
{
  MP_RANDSTATE *self_val;
  MP_INT *res_val, *arg_val;
  int free_arg_val = 0;
  VALUE res;

  mprandstate_get_struct(self,self_val);
  
  if (GMPZ_P(arg)) {
    mpz_get_struct(arg, arg_val);
  } else if (FIXNUM_P(arg)) {
    mpz_temp_alloc(arg_val);
    mpz_init_set_ui(arg_val, FIX2INT(arg));
    free_arg_val = 1;
  } else if (BIGNUM_P(arg)) {
    mpz_temp_from_bignum(arg_val, arg);
    free_arg_val = 1;
  } else {
    typeerror_as(ZXB, "arg");
  }
  
  mpz_make_struct_init(res, res_val);
  mpz_urandomm(res_val, self_val, arg_val);
  if (free_arg_val) { mpz_temp_free(arg_val); }
  
  return res;
}


#ifdef MPFR
/**********************************************************************
 *    MPFR Random Numbers                                             *
 **********************************************************************/

/*
 * call-seq:
 *   rand_state.mpfr_urandomb()
 *
 * From the MPFR Manual:
 *
 * Generate a uniformly distributed random float in the interval 0 <= rop < 1.
 */
VALUE r_gmprandstate_mpfr_urandomb(int argc, VALUE *argv, VALUE self)
{
  MP_RANDSTATE *self_val;
  MP_FLOAT *res_val;
  VALUE res;
  unsigned long prec = 0;
  
  if (argc > 1)
    rb_raise (rb_eArgError, "wrong # of arguments(%d for 0 or 1)", argc);

  mprandstate_get_struct (self,self_val);
  
  if (argc == 1) {
    if (FIXNUM_P (argv[0])) {
      if (FIX2INT (argv[0]) >= 0)
        prec = FIX2INT (argv[0]);
      else
        rb_raise (rb_eRangeError, "prec must be non-negative");
    } else {
      rb_raise (rb_eTypeError, "prec must be a Fixnum");
    }
  }
  
  mpf_make_struct (res, res_val);
  if (prec == 0) { mpf_init (res_val); }
  else           { mpf_init2 (res_val, prec); }
  
  mpfr_urandomb (res_val, self_val);
  
  return res;
}
#endif /* MPFR */


void init_gmprandstate()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "RandState", r_gmpmod_randstate, -1);

  cGMP_RandState = rb_define_class_under(mGMP, "RandState", rb_cObject);

  // Random State Initialization
  rb_define_singleton_method(cGMP_RandState, "new", r_gmprandstatesg_new, -1);
  rb_define_method(cGMP_RandState, "initialize", r_gmprandstate_initialize, -1);
  
  // Random State Seeding
  rb_define_method(cGMP_RandState, "seed", r_gmprandstate_seed, 1);
  
  // Integer Random Numbers
  rb_define_method(cGMP_RandState, "urandomb", r_gmprandstate_urandomb, 1);
  rb_define_method(cGMP_RandState, "urandomm", r_gmprandstate_urandomm, 1);
  
#ifdef MPFR
  // Float Random Numbers
  rb_define_method(cGMP_RandState, "mpfr_urandomb", r_gmprandstate_mpfr_urandomb, -1);
#endif /* MPFR */
}
