require './test_helper'

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
