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

class TC_swap < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(100)
    @b=GMP::Z.new(200)
    @c=GMP::Q.new(100,11)
    @d=GMP::Q.new(200,17)
  end
  
  def test_swap
    @a.swap @b
    @c.swap @d
    assert_equal(@a, 200, "GMP::Z should swap with GMP::Z.")
    assert_equal(@b, 100, "GMP::Z should swap with GMP::Z.")
    assert_equal(@c, GMP::Q.new(200,17), "GMP::Z should swap with GMP::Z.")
    assert_equal(@d, GMP::Q.new(100,11), "GMP::Z should swap with GMP::Z.")
  end
end

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