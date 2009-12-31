#!/usr/bin/env ruby

require 'test_helper'
require 'tc_z'
require 'tc_z_basic'
require 'tc_z_logic'
require 'tc_q'
require 'tc_cmp'
require 'tc_q_basic'
require 'tc_z_exponentiation'
require 'tc_zerodivisionexceptions'
require 'tc_sgn_neg_abs'
require 'tc_fib_fac_nextprime'
require 'tc_swap'
require 'tc_floor_ceil_truncate'
require 'tc_z_to_d_to_i'
require 'tc_z_shifts_last_bits'
require 'tc_logical_roots'
require 'tc_f_precision'
require 'tc_f_arithmetics_coersion'
require 'tc_division'
require 'tc_z_jac_leg_rem'

class TC_default_prec < Test::Unit::TestCase
  def test_default_prec
    assert_equal( 64, GMP::F.default_prec, "GMP::F.default_prec should be 64.")
    GMP::F.default_prec = 100
    assert_equal(128, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
    GMP::F.default_prec =  64
  end
end