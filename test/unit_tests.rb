#!/usr/bin/env ruby

require 'test_helper'
require 'tc_z'
require 'tc_z_basic'
require 'tc_z_logic'
require 'tc_q'
require 'tc_cmp'
require 'tc_q_basic'
require 'tc_z_exponentiation'


class TC_ZeroDivisionExceptions < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(10)
    @b = GMP::Z.new()
    @c = GMP::Q.new(1)
    @d = GMP::Q.new()
  end
  
  def test_division_by_zero
    assert_raise(ZeroDivisionError) { @a.tdiv(0) }
    assert_raise(ZeroDivisionError) { @a.tdiv(@b) }
    assert_raise(ZeroDivisionError) { @d.inv }
    assert_raise(ZeroDivisionError) { @d.inv! }
    assert_raise(ZeroDivisionError) { @c/0 }
    assert_raise(ZeroDivisionError) { @c/@d }
  end
end

class TC_sgn_neg_abs < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(10)
    @b=GMP::Z.new()
    @c=GMP::Z.new(-10)
    @d=GMP::Q.new(10)
    @e=GMP::Q.new()
    @f=GMP::Q.new(-10)
    @k=GMP::Z.new(10)
    @l=GMP::Z.new()
    @m=GMP::Z.new(-10)
    @n=GMP::Q.new(10)
    @o=GMP::Q.new()
    @p=GMP::Q.new(-10)
  end
  
  def test_sgn
    assert_equal([1, 0, 1, 0], [@a.sgn, @b.sgn, @d.sgn, @e.sgn], "GMP::Z, GMP::Q should calculate sgn correctly")
    assert_equal([-1, -1],     [@c.sgn, @f.sgn],                 "GMP::Z, GMP::Q should calculate sgn correctly")
  end
  
  def test_neg
    assert_equal(-@a, @c,                                        "-(x : GMP::Z) should be calculated correctly.")
    assert_equal(-@c, @a,                                        "-(x : GMP::Z) should be calculated correctly.")
    assert_equal(-@b, @b,                                        "-GMP::Z.new() should equal GMP::Z.new().")
    assert_equal(-@d, @f,                                        "-(x : GMP::Q) should be calculated correctly.")
    assert_equal(-@f, @d,                                        "-(x : GMP::Q) should be calculated correctly.")
    assert_equal(-@e, @e,                                        "-GMP::Q.new() should equal GMP::Q.new().")
    @k.neg!; @l.neg!; @m.neg!; @n.neg!; @o.neg!; @p.neg!
    assert_equal(@k, @c,                                         "(x : GMP::Z).neg! should be calculated correctly.")
    assert_equal(@l, @b,                                         "(x : GMP::Z).neg! should be calculated correctly.")
    assert_equal(@m, @a,                                         "GMP::Z.new().neg! should equal GMP::Z.new().")
    assert_equal(@n, @f,                                         "(x : GMP::Q).neg! should be calculated correctly.")
    assert_equal(@o, @e,                                         "(x : GMP::Q).neg! should be calculated correctly.")
    assert_equal(@p, @d,                                         "GMP::Q.new().neg! should equal GMP::Q.new().")
  end
  
  def test_abs
    assert_equal([10, 0, 10], [@a.abs, @b.abs, @c.abs],          "(x : GMP::Z).abs should be calculated correctly.")
    assert_equal([10, 0, 10], [@d.abs, @e.abs, @f.abs],          "(x : GMP::Q).abs should be calculated correctly.")
    @a.abs!; @b.abs!; @c.abs!; @d.abs!; @e.abs!; @f.abs!
    assert_equal([10, 0, 10], [@a, @b, @c],                      "(x : GMP::Z).abs! should be calculated correctly.")
    assert_equal([10, 0, 10], [@d, @e, @f],                      "(x : GMP::Q).abs! should be calculated correctly.")
  end
end

class TC_fib_fac_nextprime < Test::Unit::TestCase
  def setup
    @z10 = GMP::Z.new(10)
     @z7 = GMP::Z.new( 7)
     @z8 = GMP::Z.new( 8)
    @z11 = GMP::Z.new(11)
    @z13 = GMP::Z.new(13)
    @z17 = GMP::Z.new(17)
    @z19 = GMP::Z.new(19)
  end
  
  def test_fib
    assert_equal(  1, GMP::Z.fib(   1), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(  1, GMP::Z.fib(   2), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(  2, GMP::Z.fib(   3), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(  3, GMP::Z.fib(   4), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(  5, GMP::Z.fib(   5), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(  8, GMP::Z.fib(   6), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal( 13, GMP::Z.fib(   7), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal( 21, GMP::Z.fib(   8), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal( 34, GMP::Z.fib(   9), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal( 55, GMP::Z.fib(  10), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal( 55, GMP::Z.fib(@z10), "GMP::Z::fib(x : GMP::Z) should be calculated correctly.")
    assert_equal( 89, GMP::Z.fib(  11), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(144, GMP::Z.fib(  12), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(233, GMP::Z.fib(  13), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
    assert_equal(377, GMP::Z.fib(  14), "GMP::Z::fib(x : Fixnum) should be calculated correctly.")
  end
  
  def test_fac
    assert_equal(   1, GMP::Z.fac( 0), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal(   1, GMP::Z.fac( 1), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal(   2, GMP::Z.fac( 2), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal(   6, GMP::Z.fac( 3), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal(  24, GMP::Z.fac( 4), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal( 120, GMP::Z.fac( 5), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal( 720, GMP::Z.fac( 6), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
    assert_equal(5040, GMP::Z.fac( 7), "GMP::Z::fac(x : Fixnum) should be calculated correctly.")
  end

  def test_nextprime
    assert_equal(@z11,  @z7.nextprime, "GMP::Z.nextprime should work.")
    assert_equal(@z11,  @z8.nextprime, "GMP::Z.nextprime should work.")
    assert_equal(@z11,  @z8.nextprime, "GMP::Z.nextprime should work.")
    assert_equal(@z13, @z11.nextprime, "GMP::Z.nextprime should work.")
    assert_equal(@z17, @z13.nextprime, "GMP::Z.nextprime should work.")
    assert_equal(@z19, @z17.nextprime, "GMP::Z.nextprime should work.")
  end
end