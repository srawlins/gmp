require './test_helper'

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
    assert_equal(@z11,  @z7.next_prime, "GMP::Z.nextprime should work.")
    assert_equal(@z11,  @z8.nextprime,  "GMP::Z.nextprime should work.")
    assert_equal(@z11,  @z8.next_prime, "GMP::Z.nextprime should work.")
    assert_equal(@z13, @z11.nextprime,  "GMP::Z.nextprime should work.")
    assert_equal(@z17, @z13.next_prime, "GMP::Z.nextprime should work.")
    assert_equal(@z19, @z17.nextprime,  "GMP::Z.nextprime should work.")
  end
end
