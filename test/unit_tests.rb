#!/usr/bin/env ruby

require 'test_helper'
require 'tc_z'
require 'tc_z_basic'
require 'tc_z_logic'
require 'tc_q'

class TC_Cmp < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(180)
    @c=GMP::Q.new(2000,11) # ~181.82
    @d=GMP::Q.new(3000,17) # ~176.47
    @e=700
    @f=2**32
    @g=GMP::Q(360,2)
  end

  def test_cmp_q_q
    assert_equal(@c >  @d, true,  "GMP::Q should compare correctly")
    assert_equal(@c >= @d, true,  "GMP::Q should compare correctly")
    assert_equal(@c <  @d, false, "GMP::Q should compare correctly")
    assert_equal(@c <= @d, false, "GMP::Q should compare correctly")
    assert_equal(@c == @d, false, "GMP::Q should == correctly")
    assert_equal(@c == @c, true,  "GMP::Q should == correctly")
    assert_equal(@c != @d, true,  "GMP::Q should != correctly")
    assert_equal(@c != @c, false, "GMP::Q should != correctly")

    assert_equal(@c <=> @d,  1, "GMP::Q should <=> correctly")
    assert_equal(@c <=> @c,  0, "GMP::Q should <=> correctly")
    assert_equal(@d <=> @c, -1, "GMP::Q should <=> correctly")
  end

  def test_cmp_q_z
    assert_equal(@c >  @a, true,  "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c >= @a, true,  "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c <  @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c <= @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c == @a, false, "GMP::Q should == correctly with GMP::Z")
    assert_equal(@c != @a, true,  "GMP::Q should != correctly with GMP::Z")

    assert_equal(@a <  @c, true,  "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a <= @c, true,  "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >  @c, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >= @c, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a == @c, false, "GMP::Z should == correctly with GMP::Q")
    assert_equal(@a != @c, true,  "GMP::Z should != correctly with GMP::Q")

    assert_equal(@g == @a, true,  "GMP::Q should == correctly with GMP::Z")
    assert_equal(@g != @a, false, "GMP::Q should != correctly with GMP::Z")

    assert_equal(@d >  @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@d >= @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@d <  @a, true,  "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@d <= @a, true,  "GMP::Q should compare correctly with GMP::Z")

    assert_equal(@a <  @d, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a <= @d, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >  @d, true,  "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >= @d, true,  "GMP::Z should compare correctly with GMP::Q")

    assert_equal(@c <=> @a,  1, "GMP::Q should <=> correctly with GMP::Z")
    assert_equal(@g <=> @a,  0, "GMP::Q should <=> correctly with GMP::Z")
    assert_equal(@a <=> @c, -1, "GMP::Q should <=> correctly with GMP::Z")
  end

  def test_cmp_z_int
    assert_equal(@a <=> @e, -1, "GMP::Z should <=> correctly with Fixnum")
    assert_equal(@e <=> @a,  1, "Fixnum should <=> correctly with GMP::Z")
    assert_equal(@a <=> @f, -1, "GMP::Z should <=> correctly with Bignum")
    assert_equal(@f <=> @a,  1, "Bignum should <=> correctly with GMP::Z")
  end

  def test_cmp_q_int
    assert_equal(@c <=> @e, -1, "GMP::Q should <=> correctly with Fixnum")
    assert_equal(@e <=> @c,  1, "Fixnum should <=> correctly with GMP::Q")
    assert_equal(@c <=> @f, -1, "GMP::Q should <=> correctly with Bignum")
    assert_equal(@f <=> @c,  1, "Bignum should <=> correctly with GMP::Q")
  end
end

# [Q op Q, Q op Z, Z op Q, Q op FixNum, Q op BigNum, FixNum op Q, BigNum op Q]
class TC_Q_Basic < Test::Unit::TestCase
  def setup
    @a=GMP::Q.new(100,11)
    @b=GMP::Q.new(200,17)
    @c=GMP::Z.new(40)
    @d=2**32
  end

  def test_add
    assert_equal(@a + @b, GMP::Q(3900, 187),       "GMP::Q should add GMP::Q correctly")
    assert_equal(@a + @c, GMP::Q(540,  11),        "GMP::Q should add GMP::Z correctly")
    assert_equal(@c + @a, GMP::Q(540,  11),        "GMP::Z should add GMP::Q correctly")
    assert_equal(@a +  2, GMP::Q(122,  11),        "GMP::Z should add Fixnum correctly")
    assert_equal(@a + @d, GMP::Q(47244640356, 11), "GMP::Z should add Bignum correctly")
    assert_equal( 2 + @a, GMP::Q(122,  11),        "Fixnum should add GMP::Q correctly")
    assert_equal(@d + @a, GMP::Q(47244640356, 11), "Bignum should add GMP::Q correctly")
  end

  def test_sub
    assert_equal(@a - @b, GMP::Q(-500, 187),       "GMP::Q should subtract GMP::Q correctly")
    assert_equal(@a - @c, GMP::Q(-340, 11),        "GMP::Q should subtract GMP::Z correctly")
    assert_equal(@c - @a, GMP::Q(340,  11),        "GMP::Z should subtract GMP::Q correctly")
    assert_equal(@a -  2, GMP::Q(78,  11),         "GMP::Z should subtract Fixnum correctly")
    assert_equal(@a - @d, GMP::Q(-47244640156, 11),"GMP::Z should subtract Bignum correctly")
    assert_equal( 2 - @a, GMP::Q(-78,  11),        "Fixnum should subtract GMP::Q correctly")
    assert_equal(@d - @a, GMP::Q(47244640156, 11), "Bignum should subtract GMP::Q correctly")
  end

  def test_mul
    assert_equal(@a * @b, GMP::Q(20000, 187),      "GMP::Q should multiply GMP::Q correctly")
    assert_equal(@a * @c, GMP::Q(4000,  11),       "GMP::Q should multiply GMP::Z correctly")
    assert_equal(@c * @a, GMP::Q(4000,  11),       "GMP::Z should multiply GMP::Q correctly")
    assert_equal(@a *  2, GMP::Q(200,  11),        "GMP::Z should multiply Fixnum correctly")
    assert_equal(@a * @d, GMP::Q(429496729600, 11),"GMP::Z should multiply Bignum correctly")
    assert_equal( 2 * @a, GMP::Q(200,  11),        "Fixnum should multiply GMP::Q correctly")
    assert_equal(@d * @a, GMP::Q(429496729600, 11),"Bignum should multiply GMP::Q correctly")
  end
end
