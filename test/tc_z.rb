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
    assert_equal("1",          GMP::Z.new(1).to_s,          "Z(1).to_s should equal '1'")
    assert_equal("-1",         GMP::Z.new(-1).to_s,         "Z(1).to_s should equal '-1'")
    assert_equal("1234567890", GMP::Z.new(1234567890).to_s, "GMP::Z should to_s correctly.")
    assert_equal("100000",     GMP::Z.new(32).to_s(   2),   "GMP::Z should to_s(   2) correctly.")
    assert_equal("200",        GMP::Z.new(32).to_s(   4),   "GMP::Z should to_s(   4) correctly.")
    assert_equal("40",         GMP::Z.new(32).to_s(   8),   "GMP::Z should to_s(   8) correctly.")
    assert_equal("32",         GMP::Z.new(32).to_s(  10),   "GMP::Z should to_s(  10) correctly.")
    assert_equal("20",         GMP::Z.new(32).to_s(  16),   "GMP::Z should to_s(  16) correctly.")
    assert_equal("a",          GMP::Z.new(10).to_s(  16),   "GMP::Z should to_s(  16) correctly.")
    assert_equal("A",          GMP::Z.new(10).to_s( -16),   "GMP::Z should to_s( -16) correctly.")
    assert_equal("11111111",   GMP::Z.new(255).to_s(:bin),  "GMP::Z should to_s(:bin) correctly.")
    assert_equal("377",        GMP::Z.new(255).to_s(:oct),  "GMP::Z should to_s(:oct) correctly.")
    assert_equal("255",        GMP::Z.new(255).to_s(:dec),  "GMP::Z should to_s(:dec) correctly.")
    assert_equal("ff",         GMP::Z.new(255).to_s(:hex),  "GMP::Z should to_s(:hex) correctly.")
  end
end