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

class TC_floor_ceil_truncate < Test::Unit::TestCase
  def setup
    @a = GMP::Q.new(200,11)
    @b = -@a
    @c = GMP::Q.new(70,10)
  end
  
  def test_floor_ceil_truncate
    assert_equal(@a.floor, 18, "GMP::Q should floor.")
    assert_equal(@a.ceil, 19, "GMP::Q should ceil.")
    assert_equal(@a.trunc, 18, "GMP::Q should truncate.")
    assert_equal(@b.floor, -19, "GMP::Q (negative) should floor.")
    assert_equal(@b.ceil, -18, "GMP::Q (negative) should ceil.")
    assert_equal(@b.trunc, -18, "GMP::Q (negative) should truncate.")
    assert_equal(@c.floor, 7, "GMP::Q (integer) should floor.")
    assert_equal(@c.ceil, 7, "GMP::Q (integer) should ceil.")
    assert_equal(@c.trunc, 7, "GMP::Q (integer) should truncate.")
  end
end

class TC_to_i_to_d < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)
    #@b = GMP::Z.pow(2,32)
    @c = GMP::Q.new(200,11)
  end
  
  def test_to_i
    assert_equal(@a.to_i, 100, "GMP::Z should to_i correctly.")
    assert_equal(@a.to_i.class, Fixnum, "GMP::Z.to_i should be a Fixnum.")
    #assert_equal(@b.to_i, 2**32, "GMP::Z (Bignum) should to_i correctly.")
    #assert_equal(@b.to_i.class, Bignum, "GMP::Z.to_i should be a Bignum.")
  end
  
  def test_to_d
    assert_equal(@a.to_d, 100.0, "GMP::Z should to_d correctly.")
    assert_equal(@a.to_d.class, Float, "GMP::Z.to_d should be a Float.")
    #assert_equal(@b.to_d, 2**32*1.0, "GMP::Z should to_d correctly.")
    #assert_equal(@b.to_d.class, Float, "GMP::Z.to_d should be a Float.")
    assert_equal(@c.to_d.class, Float, "GMP::Q.to_d should be a Float.")
  end
end