require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcInteger < MiniTest::Test
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
    assert_equal(GMP::Z.new("5000000000"), 5_000_000_000, "GMP::Z.new(x : String) bigger than 32-bit should initialize to x")
  end

  def test_init_string_base
    assert_equal( 42, GMP::Z.new("42",  0), "GMP::Z.new(x : String, 0) should initialize to x")
    assert_equal( 42, GMP::Z.new("42", 10), "GMP::Z.new(x : String, 10) should initialize to x")
    assert_equal( 32, GMP::Z.new("20", 16), "GMP::Z.new(x : String, 16) should initialize to x")
    assert_equal(255, GMP::Z.new("FF", 16), "GMP::Z.new(x : String, 16), with uppercase A-F should initialize to x")
    assert_equal(255, GMP::Z.new("ff", 16), "GMP::Z.new(x : String, 16), with lowercase a-f should initialize to x")
    assert_equal(255, GMP::Z.new("7v", 32), "GMP::Z.new(x : String, 32), with lowercase a-v should initialize to x")
    assert_equal(511, GMP::Z.new("fv", 32), "GMP::Z.new(x : String, 32), with lowercase a-v should initialize to x")
    assert_equal( 71, GMP::Z.new("1Z", 36), "GMP::Z.new(x : String, 36), with uppercase A-Z should initialize to x")
    assert_equal(512, GMP::Z.new("e8", 36), "GMP::Z.new(x : String, 36), with lowercase a-z should initialize to x")
    assert_equal(523, GMP::Z.new("ej", 36), "GMP::Z.new(x : String, 36), with lowercase a-z should initialize to x")
    assert_equal( 77, GMP::Z.new("1Z", 42), "GMP::Z.new(x : String, 42), with A-Z,a-f should initialize to x")
    assert_equal( 78, GMP::Z.new("1a", 42), "GMP::Z.new(x : String, 42), with A-Z,a-f should initialize to x")
    assert_equal( 83, GMP::Z.new("1f", 42), "GMP::Z.new(x : String, 42), with A-Z,a-f should initialize to x")
    assert_equal( 84, GMP::Z.new("20", 42), "GMP::Z.new(x : String, 42), with A-Z,a-f should initialize to x")
    assert_equal(115, GMP::Z.new("1v", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(116, GMP::Z.new("20", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(126, GMP::Z.new("2A", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(151, GMP::Z.new("2Z", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(152, GMP::Z.new("2a", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(161, GMP::Z.new("2j", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(171, GMP::Z.new("2t", 58), "GMP::Z.new(x : String, 58), with A-Z,a-v should initialize to x")
    assert_equal(123, GMP::Z.new("1z", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(124, GMP::Z.new("20", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(134, GMP::Z.new("2A", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(160, GMP::Z.new("2a", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(169, GMP::Z.new("2j", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(179, GMP::Z.new("2t", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(185, GMP::Z.new("2z", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
    assert_equal(186, GMP::Z.new("30", 62), "GMP::Z.new(x : String, 62), with A-Z,a-z should initialize to x")
  end

  def test_init_bignum
    assert_equal(2**32, GMP::Z.new(2**32), "GMP::Z.new(x : Bignum) should initialize to x")
  end

  def test_init_float
    assert_equal(3, GMP::Z.new(3.14), "GMP::Z.new(x : Float) should initialize to x")
    assert_equal(3, GMP::Z.new(3.99), "GMP::Z.new(x : Float) should initialize to x")
  end

  def test_init_gmpf
    assert_equal(3, GMP::Z.new(GMP::F(3.14)), "GMP::Z.new(x : GMP::F) should initialize to x")
    assert_equal(577, GMP::Z.new(GMP::F("0.57721566")*1000), "GMP::Z.new(x : GMP::F) should initialize to x")
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
