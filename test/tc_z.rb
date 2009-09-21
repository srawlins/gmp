require 'test_helper'

class TC_Z < Test::Unit::TestCase
  def test_init_null
    assert_equal(GMP::Z.new(), 0, "GMP::Z.new() should initialize to 0")
  end

  def test_init_fixnum
    assert_equal(GMP::Z.new(1), 1, "GMP::Z.new(x : Fixnum) should initialize to x")
  end

  def test_init_z
    b = GMP::Z.new(1)
    assert_equal(GMP::Z.new(b), b, "GMP::Z.new(x : GMP::Z) should initialize to x")
  end

  def test_init_string
    assert_equal(GMP::Z.new("1"), 1, "GMP::Z.new(x : String) should initialize to x")
  end

  def test_init_bignum
    assert_equal(GMP::Z.new(2**32), 2**32, "GMP::Z.new(x : Bignum) should initialize to x")
  end

  def test_cmp_z
    a = GMP::Z.new(2)
    b = GMP::Z.new(1)
    assert_equal(a >  b, true,  "GMP::Z should compare correctly with GMP::Z")
    assert_equal(a >  a, false, "GMP::Z should compare correctly with GMP::Z")
    assert_equal(b >  a, false, "GMP::Z should compare correctly with GMP::Z")

    assert_equal(a >= b, true,  "GMP::Z should compare correctly with GMP::Z")
    assert_equal(a >= a, true,  "GMP::Z should compare correctly with GMP::Z")
    assert_equal(b >= a, false, "GMP::Z should compare correctly with GMP::Z")

    assert_equal(a <= b, false, "GMP::Z should compare correctly with GMP::Z")
    assert_equal(a <= a, true,  "GMP::Z should compare correctly with GMP::Z")
    assert_equal(b <= a, true,  "GMP::Z should compare correctly with GMP::Z")

    assert_equal(a <  b, false, "GMP::Z should compare correctly with GMP::Z")
    assert_equal(a <  a, false, "GMP::Z should compare correctly with GMP::Z")
    assert_equal(b <  a, true,  "GMP::Z should compare correctly with GMP::Z")

    assert_equal(a == b, false, "GMP::Z should == correctly with GMP::Z")
    assert_equal(a == a, true,  "GMP::Z should == correctly with GMP::Z")

    assert_equal(a != b, true,  "GMP::Z should != correctly with GMP::Z")
    assert_equal(a != a, false, "GMP::Z should != correctly with GMP::Z")

    assert_equal(a <=> b,  1, "GMP::Z should <=> correctly with GMP::Z")
    assert_equal(a <=> a,  0, "GMP::Z should <=> correctly with GMP::Z")
    assert_equal(b <=> a, -1, "GMP::Z should <=> correctly with GMP::Z")
  end

  def test_to_s
    assert_equal(GMP::Z.new(1).to_s, "1", "Z(1).to_s should equal '1'")
    assert_equal(GMP::Z.new(-1).to_s, "-1", "Z(1).to_s should equal '-1'")
  end
end