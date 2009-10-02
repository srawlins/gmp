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

class TC_shifts_last_bits < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)  # 01100100
    @b =- @a              # 10011100
  end
  
  def test_shifts
    assert_equal(   3, @a >> 5, "GMP::Z should >> correctly.")  # 00000011
    assert_equal(-  4, @b >> 5, "GMP::Z should >> correctly.")  # 11111100
    assert_equal( 400, @a << 2, "GMP::Z should << correctly.")  # 0110010000
    assert_equal(-400, @b << 2, "GMP::Z should << correctly.")  # 1110010000
  end
  
  def test_last_bits
    #assert_equal( 5, @a.lastbits_pos(5), "GMP::Z should lastbits_pos correctly.")  #   100
    #assert_equal(28, @b.lastbits_pos(5), "GMP::Z should lastbits_pos correctly.")  # 11100
    #assert_equal(-4, @b.lastbits_sgn(5), "GMP::Z should lastbits_sgn correctly.")
    # a.tshr 5 ???
  end
end