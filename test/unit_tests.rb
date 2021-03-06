#!/usr/bin/env ruby

DIR = File.expand_path(File.dirname(__FILE__))

# miscellaneous tests
require File.join(DIR, 'tc_cmp')
require File.join(DIR, 'tc_constants')
require File.join(DIR, 'tc_division')
require File.join(DIR, 'tc_hashes')
require File.join(DIR, 'tc_random')
require File.join(DIR, 'tc_sgn_neg_abs')
require File.join(DIR, 'tc_swap')
require File.join(DIR, 'tc_zero_division_exceptions')

# float tests
require File.join(DIR, 'tc_f_abs_neg')
require File.join(DIR, 'tc_f_arithmetics_coersion')
require File.join(DIR, 'tc_f_precision')
require File.join(DIR, 'tc_f_to_s')

# integer tests
require File.join(DIR, 'tc_z')
require File.join(DIR, 'tc_z_addmul')
require File.join(DIR, 'tc_z_basic')
require File.join(DIR, 'tc_z_exponentiation')
require File.join(DIR, 'tc_z_export_import')
require File.join(DIR, 'tc_z_fib_fac_nextprime')
require File.join(DIR, 'tc_z_functional_mappings')
require File.join(DIR, 'tc_z_gcd_lcm_invert')
require File.join(DIR, 'tc_z_hamdist')
require File.join(DIR, 'tc_z_io')
require File.join(DIR, 'tc_z_jac_leg_rem')
require File.join(DIR, 'tc_z_logic')
require File.join(DIR, 'tc_z_logical_roots')
require File.join(DIR, 'tc_z_shifts_last_bits')
require File.join(DIR, 'tc_z_submul')
require File.join(DIR, 'tc_z_to_dis')

# rational tests
require File.join(DIR, 'tc_q')
require File.join(DIR, 'tc_q_basic')
require File.join(DIR, 'tc_q_floor_ceil_truncate')
require File.join(DIR, 'tc_q_num_den')

# gmp suite tests
require File.join(DIR, 'gmp_tcong')
require File.join(DIR, 'gmp_tgcd')
require File.join(DIR, 'gmp_tlcm')
require File.join(DIR, 'gmp_tprintf')
require File.join(DIR, 'gmp_troot')

if GMP.const_defined? :MPFR_VERSION
  require File.join(DIR, 'tc_mpfr_cmp')
  require File.join(DIR, 'tc_mpfr_constants')
  require File.join(DIR, 'tc_mpfr_inf_nan_zero')
  require File.join(DIR, 'tc_mpfr_integer')
  require File.join(DIR, 'tc_mpfr_pow')
  require File.join(DIR, 'tc_mpfr_random')
  require File.join(DIR, 'tc_mpfr_functions')
  require File.join(DIR, 'tc_mpfr_rounding')
  require File.join(DIR, 'tc_mpfr_new_rounding')
  require File.join(DIR, 'mpfr_tcbrt')
  require File.join(DIR, 'mpfr_tconst_euler')
  require File.join(DIR, 'mpfr_tfac')
  if GMP::MPFR_VERSION >= "3.1.0"
    require File.join(DIR, 'mpfr_tfrexp')
  end
  require File.join(DIR, 'mpfr_thypot')
  require File.join(DIR, 'mpfr_tisnan')
  require File.join(DIR, 'mpfr_trec_sqrt')
  require File.join(DIR, 'mpfr_tsqrt')
end
