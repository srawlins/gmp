require './test_helper'

# Tested: [Z op (Z,Z), Z op (Z,Fixnum), Z op (Z,Bignum),
#          Z op (Fixnum,Z), Z op (Fixnum,Fixnum), Z op (Fixnum,Bignum)
#          Z op (Bignum,Z), Z op (Bignum,Bignum), Z op (Bignum,Fixnum)]
# Things are tested both ways because the implementation is asymetrical
class TC_Z_Addmul < Test::Unit::TestCase
  def setup
    @_64bit = 1_000_000_000_000.is_a? Fixnum
  end

  def test_z
    five = GMP::Z(5)
    seven = GMP::Z(7)
    five.addmul!(seven, seven)  # <= 5 + 49
    assert_equal(54, five, "GMP::Z should addmul GMP::Z correctly")
    five = GMP::Z(5)
    twenty = GMP::Z(20)
    neg2 = GMP::Z(-2)
    five.addmul!(twenty, neg2)  # <= 5 + -40
    assert_equal(-35, five, "GMP::Z should addmul GMP::Z correctly")
    m20 = GMP::Z(2)**20 - 1  # 1_048_575
    m19 = GMP::Z(2)**19 - 1  #   524_287
    neg_two = GMP::Z(-2)
    m20.addmul!(m19, neg_two)  # <= 1_048_575 + -1_048_574
    assert_equal(1, m20, "GMP::Z should addmul GMP::Z correctly")
    
    neg_m40 = -GMP::Z(2)**40  # -1_099_511_627_776
    ten_million  =        10_000_000
    neg_million  = -GMP::Z(1_000_000)
    million      =  GMP::Z(1_000_000)
    thirty_thou  =  GMP::Z(   30_000)
    thirty       =  GMP::Z(       30)
    neg_m40.addmul!(neg_million,   neg_million)   # <= -1_099_511_627_776 + 1_000_000_000_000
    assert_equal(GMP::Z(-99_511_627_776), neg_m40, "GMP::Z should addmul GMP::Z correctly")
    neg_m40.addmul!(ten_million, GMP::Z(9_000))   # <= -   99_511_627_776 +    90_000_000_000
    assert_equal(GMP::Z(- 9_511_627_776), neg_m40, "GMP::Z should addmul GMP::Z correctly")
    neg_m40.addmul!(    million, GMP::Z(9_000))   # <= -    9_511_627_776 +     9_000_000_000
    assert_equal(GMP::Z(-   511_627_776), neg_m40, "GMP::Z should addmul GMP::Z correctly")
    neg_m40.addmul!(GMP::Z(17_000), thirty_thou)  # <= -      511_627_776 +       510_000_000
    assert_equal(GMP::Z(-     1_627_776), neg_m40, "GMP::Z should addmul GMP::Z correctly")
    neg_m40.addmul!(GMP::Z(-400), GMP::Z(-4000))  # <= -        1_627_776 +         1_600_000
    assert_equal(GMP::Z(-        27_776), neg_m40, "GMP::Z should addmul GMP::Z correctly")
    neg_m40.addmul!(GMP::Z(-30), thirty*(-30))    # <= -           27_776 +            27_000
    assert_equal(GMP::Z(-           776), neg_m40, "GMP::Z should addmul GMP::Z correctly")
  end

  def test_fixnum
    five = GMP::Z(5)
    five.addmul!(7, 7)  # <= 5 + 49
    assert_equal(54, five, "GMP::Z should addmul Fixnum correctly")
    five = GMP::Z(5)
    five.addmul!(-20, 2)  # <= 5 + -40
    assert_equal(-35, five, "GMP::Z should addmul Fixnum correctly")
    m20 = GMP::Z(2)**20 - 1  # 1_048_575
    m20.addmul!(-524_287, 2)  # <= 1_048_575 + -1_048_574
    assert_equal(1, m20, "GMP::Z should addmul Fixnum correctly")
    if @_64bit
      neg_m40 = -GMP::Z(2)**40  # -1_099_511_627_776
      neg_m40.addmul!( 1_000_000_000_000,  1)  # <= -1_099_511_627_776 + 1_000_000_000_000
      assert_equal(GMP::Z(-99_511_627_776), neg_m40, "GMP::Z should addmul 64-bit Fixnum correctly")
    end
  end

  def test_bignum
    if ! @_64bit
      neg_m40 = -GMP::Z(2)**40  # -1_099_511_627_776
      neg_m40.addmul!( 1_000_000_000_000,  1)  # <= -1_099_511_627_776 + 1_000_000_000_000
      assert_equal(GMP::Z(-99_511_627_776), neg_m40, "GMP::Z should addmul Bignum correctly")
      neg_m40.addmul!(10,      9_000_000_000)  # <= -   99_511_627_776 +    90_000_000_000
      assert_equal(GMP::Z(- 9_511_627_776), neg_m40, "GMP::Z should addmul Bignum correctly")
      neg_m40.addmul!( 3_000_000_000,      3)  # <= -    9_511_627_776 +     9_000_000_000
      assert_equal(GMP::Z(-   511_627_776), neg_m40, "GMP::Z should addmul Bignum correctly")
    else
      m70 = GMP::Z(2)**70 - 1  # 1_180_591_620_717_411_303_423
      m70.addmul!(-11*10_000_000_000, 10_000_000_000)
      assert_equal(GMP::Z(80_591_620_717_411_303_423), m70, "GMP::Z should addmul 64-bit Bignum correctly")
      m70.addmul!(-1_000_000_000, 8*10_000_000_000)
      assert_equal(GMP::Z(   591_620_717_411_303_423), m70, "GMP::Z should addmul 64-bit Bignum correctly")
    end
  end

  def test_raise
    if ! @_64bit
      assert_raise(RangeError) { GMP::Z(123).addmul!(456, -7) }
      assert_nothing_raised(RangeError) { GMP::Z(123).addmul!(456, -7_000_000_000) }
    else
      assert_raise(RangeError) { GMP::Z(123).addmul!(456, -7_000_000_000) }
      assert_nothing_raised(RangeError) { GMP::Z(123).addmul!(456, -7_000_000_000_000_000_000) }
    end
      assert_nothing_raised(RangeError) { GMP::Z(123).addmul!(456, 7) }
      assert_nothing_raised(RangeError) { GMP::Z(123).addmul!(456, GMP::Z(-7)) }
  end
end
