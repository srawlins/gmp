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
require 'tc_z_jac_leg_rem'

class TC_default_prec < Test::Unit::TestCase
  def test_default_prec
    assert_equal( 64, GMP::F.default_prec, "GMP::F.default_prec should be 64.")
    GMP::F.default_prec = 100
    assert_equal(128, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
    GMP::F.default_prec =  64
  end
end

class TC_division < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(5)
    @b = GMP::Z.new(7)
    @c = GMP::Z.new(25)
    @d = GMP::Q.new(3,11)
    @e = GMP::F.new(3.14)
    @f = 2**32
  end
  
  def test_z_div
    assert_equal(GMP::Q, (@a / @b   ).class, "GMP::Z / GMP::Z should be GMP::Q.")
    assert_equal(GMP::Q, (@a / 3    ).class, "GMP::Z / Fixnum should be GMP::Q.")
    assert_equal(GMP::Q, (@a / 2**32).class, "GMP::Z / Bignum should be GMP::Q.")
    assert_equal(GMP::Q, (@a / @c   ).class, "GMP::Z / GMP::Z should be GMP::Q.")
    assert_in_delta(0.7142857142, @a / @b, 1e-7, "GMP::Z./ should work.")
    assert_in_delta(1.4         , @b / @a, 1e-7, "GMP::Z./ should work.")
    assert_in_delta(1.6666666667, @a /  3, 1e-7, "GMP::Z./ should work.")
    assert_in_delta(0.6         ,  3 / @a, 1e-7, "GMP::Z./ should work.")
    assert_in_delta(0.2         , @a / @c, 1e-7, "GMP::Z./ should work.")
    assert_in_delta(5.0         , @c / @a, 1e-7, "GMP::Z./ should work.")
  end
  
  def test_z_tdiv
    assert_equal(GMP::Z, @a.tdiv(@b).class,    "GMP::Z.tdiv GMP::Z should be GMP::Z.")
    assert_equal(GMP::Z, @a.tdiv(3).class,     "GMP::Z.tdiv Fixnum should be GMP::Z.")
    assert_equal(GMP::Z, @a.tdiv(2**32).class, "GMP::Z.tdiv Bignum should be GMP::Z.")
    assert_equal(GMP::Z, @a.tdiv(@c).class,    "GMP::Z.tdiv GMP::Z should be GMP::Z.")
    assert_equal(0, @a.tdiv(@b), "GMP::Z.tdiv should work.")
    assert_equal(1, @b.tdiv(@a), "GMP::Z.tdiv should work.")
    assert_equal(1, @a.tdiv( 3), "GMP::Z.tdiv should work.")
    assert_equal(0, @a.tdiv(@c), "GMP::Z.tdiv should work.")
    assert_equal(5, @c.tdiv(@a), "GMP::Z.tdiv should work.")
  end
  
  def test_z_fdiv
    assert_equal(GMP::Z, @a.fdiv(@b).class,    "GMP::Z.fdiv GMP::Z should be GMP::Z.")
    assert_equal(GMP::Z, @a.fdiv(3).class,     "GMP::Z.fdiv Fixnum should be GMP::Z.")
    assert_equal(GMP::Z, @a.fdiv(2**32).class, "GMP::Z.fdiv Bignum should be GMP::Z.")
    assert_equal(0, @a.fdiv(@b), "GMP::Z.fdiv should work.")
    assert_equal(1, @b.fdiv(@a), "GMP::Z.fdiv should work.")
    assert_equal(1, @a.fdiv( 3), "GMP::Z.fdiv should work.")
    assert_equal(0, @a.fdiv(@c), "GMP::Z.fdiv should work.")
    assert_equal(5, @c.fdiv(@a), "GMP::Z.fdiv should work.")
  end
  
  def test_z_cdiv
    assert_equal(GMP::Z, @a.cdiv(@b).class,    "GMP::Z.cdiv GMP::Z should be GMP::Z.")
    assert_equal(GMP::Z, @a.cdiv(3).class,     "GMP::Z.cdiv Fixnum should be GMP::Z.")
    assert_equal(GMP::Z, @a.cdiv(2**32).class, "GMP::Z.cdiv Bignum should be GMP::Z.")
    assert_equal(1, @a.cdiv(@b), "GMP::Z.cdiv should work.")
    assert_equal(2, @b.cdiv(@a), "GMP::Z.cdiv should work.")
    assert_equal(2, @a.cdiv( 3), "GMP::Z.cdiv should work.")
    assert_equal(1, @a.cdiv(@c), "GMP::Z.cdiv should work.")
    assert_equal(5, @c.cdiv(@a), "GMP::Z.cdiv should work.")
  end
  
  def test_z_tmod
    assert_equal(GMP::Z, @a.tmod(@b).class,    "GMP::Z.tmod GMP::Z should be GMP::Z.")
    assert_equal(GMP::Z, @a.tmod(3).class,     "GMP::Z.tmod Fixnum should be GMP::Z.")
    assert_equal(GMP::Z, @a.tmod(2**32).class, "GMP::Z.tmod Bignum should be GMP::Z.")
    assert_equal(GMP::Z, @a.tmod(@c).class,    "GMP::Z.tmod GMP::Z should be GMP::Z.")
    assert_equal(5, @a.tmod(@b), "GMP::Z.tmod should work.")
    assert_equal(2, @b.tmod(@a), "GMP::Z.tmod should work.")
    assert_equal(2, @a.tmod( 3), "GMP::Z.tmod should work.")
    assert_equal(5, @a.tmod(@c), "GMP::Z.tmod should work.")
    assert_equal(0, @c.tmod(@a), "GMP::Z.tmod should work.")
  end
  
  def test_z_fmod
    assert_equal(GMP::Z, @a.fmod(@b).class,    "GMP::Z.fmod GMP::Z should be GMP::Z.")
    assert_equal(GMP::Z, @a.fmod(3).class,     "GMP::Z.fmod Fixnum should be GMP::Z.")
    assert_equal(GMP::Z, @a.fmod(2**32).class, "GMP::Z.fmod Bignum should be GMP::Z.")
    assert_equal(GMP::Z, @a.fmod(@c).class,    "GMP::Z.fmod GMP::Z should be GMP::Z.")
    assert_equal(5, @a.fmod(@b), "GMP::Z.fmod should work.")
    assert_equal(2, @b.fmod(@a), "GMP::Z.fmod should work.")
    assert_equal(2, @a.fmod( 3), "GMP::Z.fmod should work.")
    assert_equal(5, @a.fmod(@c), "GMP::Z.fmod should work.")
    assert_equal(0, @c.fmod(@a), "GMP::Z.fmod should work.")
  end
  
  def test_z_cmod
    assert_equal(GMP::Z, @a.cmod(@b).class,    "GMP::Z.cmod GMP::Z should be GMP::Z.")
    assert_equal(GMP::Z, @a.cmod(3).class,     "GMP::Z.cmod Fixnum should be GMP::Z.")
    assert_equal(GMP::Z, @a.cmod(2**32).class, "GMP::Z.cmod Bignum should be GMP::Z.")
    assert_equal(GMP::Z, @a.cmod(@c).class,    "GMP::Z.cmod GMP::Z should be GMP::Z.")
    assert_equal( -2, @a.cmod(@b), "GMP::Z.cmod should work.")
    assert_equal( -3, @b.cmod(@a), "GMP::Z.cmod should work.")
    assert_equal( -1, @a.cmod( 3), "GMP::Z.cmod should work.")
    assert_equal(-20, @a.cmod(@c), "GMP::Z.cmod should work.")
    assert_equal(  0, @c.cmod(@a), "GMP::Z.cmod should work.")
  end
end