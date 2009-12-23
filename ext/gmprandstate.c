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
 *   GMP::RandState.new(arg = 0)
 *
 * Initializes a new Random State object. Multiple GMP::RandState objects can
 * be instantiated. They may use different generators (TODO) and the states
 * are kept separate.
 */
VALUE r_gmprandstatesg_new(int argc, VALUE *argv, VALUE klass)
{
  MP_RANDSTATE *rs_val;
  VALUE rs;
 
  (void)klass;

  if (argc > 1)
    rb_raise(rb_eArgError, "wrong # of arguments (%d for 0 or 1)", argc);
  
  mprandstate_make_struct(rs, rs_val);
  /* Process argv to search for an alternative generator, ie
     :mt (gmp_randinit_mt), :lc_2exp (gmp_randinit_lc_2exp), or
     :lc_2exp_size (gmp_randinit_lc_2exp_size) */
  gmp_randinit_default(rs_val);
  rb_obj_call_init(rs, argc, argv);
  
  return rs;
}

VALUE r_gmprandstate_initialize(int argc, VALUE *argv, VALUE self)
{
  MP_RANDSTATE *self_val;

  if (argc != 0) {
    mprandstate_get_struct(self,self_val);
    /* Process argv to search for an alternative generator, ie
       :mt (gmp_randinit_mt), :lc_2exp (gmp_randinit_lc_2exp), or
       :lc_2exp_size (gmp_randinit_lc_2exp_size) */
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
  
}
