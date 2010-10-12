#!/usr/bin/env ruby

require './test_helper'

require './tc_constants'
require './tc_division'
require './tc_f_precision'
require './tc_f_arithmetics_coersion'
require './tc_z'
require './tc_z_basic'
require './tc_z_addmul'
require './tc_z_submul'
require './tc_z_logic'
require './tc_z_exponentiation'
require './tc_z_to_dis'
require './tc_z_shifts_last_bits'
require './tc_z_jac_leg_rem'
require './tc_z_gcd_lcm_invert'
require './tc_q'
require './tc_q_basic'
require './tc_cmp'
require './tc_zerodivisionexceptions'
require './tc_sgn_neg_abs'
require './tc_fib_fac_nextprime'
require './tc_swap'
require './tc_floor_ceil_truncate'
require './tc_logical_roots'
require './tc_random'
require './tc_hashes'
require './tc_z_functional_mappings'

require './gmp_tgcd'

begin
  GMP::MPFR_VERSION
  require './tc_mpfr_constants'
  require './tc_mpfr_random'
  require './tc_mpfr_functions'
  require './tc_mpfr_rounding'
  require './mpfr_tcbrt'
  require './mpfr_tisnan'
  require './mpfr_trec_sqrt'
  require './mpfr_tsqrt'
rescue

end
