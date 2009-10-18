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

class TC_precision < Test::Unit::TestCase
  def setup
    @pi = GMP::F.new(3.14)
    @seven_halves = GMP::F.new(GMP::Q.new(7,2), 1000)
  end
  
  def test_default_precision
    assert_equal("0.e+0", GMP::F.new().to_s)
    assert_equal(64,      GMP::F.new().prec)
    assert_equal("0.314000000000000012434e+1", @pi.to_s)
    assert_equal(64,                           @pi.prec)
    assert_equal("0.1e+1", GMP::F.new(1).to_s)
    assert_equal(64,       GMP::F.new(1).prec)
    assert_equal("0.314e+1", GMP::F.new("3.14").to_s)
    assert_equal(64,         GMP::F.new("3.14").prec)
    assert_equal("0.4294967296e+10", GMP::F.new(2**32).to_s)
    assert_equal(64,                 GMP::F.new(2**32).prec)
    assert_equal("0.3e+1", GMP::F.new(GMP::Z.new(3)).to_s)
    assert_equal(64,       GMP::F.new(GMP::Z.new(3)).prec)
    assert_equal("0.35e+1", GMP::F.new(GMP::Q.new(7,2)).to_s)
    assert_equal(64,        GMP::F.new(GMP::Q.new(7,2)).prec)
    assert_equal("0.314000000000000012434e+1", GMP::F.new(@pi).to_s)
    assert_equal(64,                           GMP::F.new(@pi).prec)
  end
  
  def test_specific_precision
    assert_equal("0.3140000000000000124344978758017532527446746826171875e+1", GMP::F.new(3.14, 1000).to_s)
    assert_equal(1024,                                                        GMP::F.new(3.14, 1000).prec)
    assert_equal("0.1e+1", GMP::F.new(1, 1000).to_s)
    assert_equal(1024,     GMP::F.new(1, 1000).prec)
    assert_equal("0.314e+1", GMP::F.new("3.14", 1000).to_s)
    assert_equal(1024,       GMP::F.new("3.14", 1000).prec)
    assert_equal("0.4294967296e+10", GMP::F.new(2**32, 1000).to_s)
    assert_equal(1024,               GMP::F.new(2**32, 1000).prec)
    assert_equal("0.3e+1", GMP::F.new(GMP::Z.new(3), 1000).to_s)
    assert_equal(1024,     GMP::F.new(GMP::Z.new(3), 1000).prec)
    assert_equal("0.35e+1", @seven_halves.to_s)
    assert_equal(1024,      @seven_halves.prec)
    assert_equal("0.35e+1", GMP::F.new(@seven_halves).to_s)
    assert_equal(1024,      GMP::F.new(@seven_halves).prec)
    assert_equal("0.3140000000000000124344978758017532527446746826171875e+1", GMP::F.new(@pi, 1000).to_s)
    assert_equal(1024,                                                        GMP::F.new(@pi, 1000).prec)
    assert_equal("0.35e+1", GMP::F.new(@seven_halves, 0).to_s)
    assert_equal(64,        GMP::F.new(@seven_halves, 0).prec)
  end
end

class TC_F_arithmetics_coersion < Test::Unit::TestCase
  def setup
    @a = GMP::F.new(3.14, 100)
    @b = GMP::F.new(2.71, 200)
    @c = GMP::Z(3)
    @d = GMP::Q(7,2)
    @e = 2**32
  end
  
  def test_add
    assert_in_delta(5.85,            @a +   @b, 1e-5)
    assert_in_delta(5.85,            @b +   @a, 1e-5)
    assert_in_delta(6.14,            @a +   @c, 1e-5)
    assert_in_delta(6.14,            @c +   @a, 1e-5)
    assert_in_delta(6.64,            @a +   @d, 1e-5)
    assert_in_delta(6.64,            @d +   @a, 1e-5)
    assert_in_delta(5.14,            @a +    2, 1e-5)
    assert_in_delta(5.14,             2 +   @a, 1e-5)
    assert_in_delta(4294967299.14,   @a +   @e, 1e-5)
    assert_in_delta(4294967299.14,   @e +   @a, 1e-5)
    assert_in_delta(7.65,            @a + 4.51, 1e-5)
    assert_in_delta(7.65,          4.51 +   @a, 1e-5)
  end
end