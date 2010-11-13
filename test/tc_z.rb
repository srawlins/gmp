require './test_helper'

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
    assert_equal(2**32, GMP::Z.new(2**32), "GMP::Z.new(x : Bignum) should initialize to x")
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
  
  def test_divisible_ui
    a = GMP::Z(120)
    b = GMP::Z(17)
    a_factors = [1,2,3,4,5,6,8,10,12,15,20,24,30,40,60,120]
    a_factors.each do |d|
      assert_true(a.divisible?(d), "GMP::Z#divisible? ui should be true: #{a}.divisible? #{d}")
    end
    ((1..120).to_a - a_factors).each do |d|
      assert_false(a.divisible?(d), "GMP::Z#divisible? ui should be false: #{a}.divisible? #{d}")
    end
    (2..16).to_a.each do |d|
      assert_false(b.divisible?(d), "GMP::Z#divisible? ui should be false: #{a}.divisible? #{d}")
    end
  end
  
  def test_divisible_bignum
    a = GMP::Z(2**71 - 3**31)
    a_factors = [29,    743,        109_582_894_312_963_583,
                     29*743,     29*109_582_894_312_963_583,
                                743*109_582_894_312_963_583,
                             29*743*109_582_894_312_963_583]
    a_factors.each do |d|
      assert_true(a.divisible?(d), "GMP::Z#divisible? bignum should be true: #{a}.divisible? #{d}")
    end
    a_factors.map { |n| n*2 }.each do |d|
      assert_false(a.divisible?(d), "GMP::Z#divisible? ui should be false: #{a}.divisible? #{d}")
    end
    a_factors.map { |n| n*3 }.each do |d|
      assert_false(a.divisible?(d), "GMP::Z#divisible? ui should be false: #{a}.divisible? #{d}")
    end
  end
  
  def test_divisible_z
    a = GMP::Z(120)
    b = GMP::Z(17)
    a_factors = [1,2,3,4,5,6,8,10,12,15,20,24,30,40,60,120].map { |n| GMP::Z(n) }
    a_factors.each do |d|
      assert_true(a.divisible?(d), "GMP::Z#divisible? GMP::Z should be true: #{a}.divisible? #{d}")
    end
    ((1..120).to_a.map { |n| GMP::Z(n) } - a_factors).each do |d|
      assert_false(a.divisible?(d), "GMP::Z#divisible? ui should be false: #{a}.divisible? #{d}")
    end
    (2..16).to_a.map { |n| GMP::Z(n) }.each do |d|
      assert_false(b.divisible?(d), "GMP::Z#divisible? ui should be false: #{a}.divisible? #{d}")
    end
  end
end
