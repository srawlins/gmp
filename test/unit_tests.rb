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
require 'tc_to_d_to_i'

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

class TC_logical_roots < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)
    @b = GMP::Z.new( 27)
    @c = GMP::Z.new( 99)
  end
  
  def test_parity
    assert  @a.even?, "GMP::Z should even? correctly."
    assert !@b.even?, "GMP::Z should even? correctly."
    assert !@c.even?, "GMP::Z should even? correctly."
    assert !@a.odd?,  "GMP::Z should odd? correctly."
    assert  @b.odd?,  "GMP::Z should odd? correctly."
    assert  @c.odd?,  "GMP::Z should odd? correctly."
  end
  
  def test_square
    assert  @a.square?, "GMP::Z should square? correctly."
    assert !@b.square?, "GMP::Z should square? correctly."
    assert !@c.square?, "GMP::Z should square? correctly."
  end
  
  def test_power
    assert  @a.power?, "GMP::Z should power? correctly."
    assert  @b.power?, "GMP::Z should power? correctly."
    assert !@c.power?, "GMP::Z should power? correctly."
  end
  
  def sqrtrem
    assert_equal([10,  0], @a.sqrtrem, "GMP::Z should sqrtrem correctly.")
    assert_equal([ 5,  2], @b.sqrtrem, "GMP::Z should sqrtrem correctly.")
    assert_equal([ 9, 18], @c.sqrtrem, "GMP::Z should sqrtrem correctly.")
  end
  
  def sqrt
    assert_equal(10, @a.sqrt, "GMP::Z should sqrt correctly.")
    assert_equal( 5, @b.sqrt, "GMP::Z should sqrt correctly.")
    assert_equal( 9, @c.sqrt, "GMP::Z should sqrt correctly.")
  end
  
  def root
    assert_equal(4, @a.root(3), "GMP::Z should root correctly.")
    assert_equal(3, @b.root(3), "GMP::Z should root correctly.")
    assert_equal(4, @c.root(3), "GMP::Z should root correctly.")
  end
end