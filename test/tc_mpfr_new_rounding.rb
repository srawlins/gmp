require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TC_MPFR_New_Rounding < Test::Unit::TestCase
  def test_new_fixnum
    ninety_six = GMP::F(96, 4)
    one_hundred_four = GMP::F(104, 4)

    a = GMP::F(100, 4)
    assert_equal(a, ninety_six, "GMP::F.new should default round with RNDN")

    a = GMP::F(100, 4, GMP::GMP_RNDN)
    assert_equal(a, ninety_six, "GMP::F.new should round with RNDN")

    a = GMP::F(100, 4, GMP::GMP_RNDD)
    assert_equal(a, ninety_six, "GMP::F.new should round with RNDD")

    a = GMP::F(100, 4, GMP::GMP_RNDZ)
    assert_equal(a, ninety_six, "GMP::F.new should round with RNDZ")

    a = GMP::F(100, 4, GMP::GMP_RNDU)
    assert_equal(a, one_hundred_four, "GMP::F.new should round with RNDU")
  end

  def test_new_gmpz
    z100 = GMP::Z(100)
    ninety_six = GMP::F(96, 4)
    one_hundred_four = GMP::F(104, 4)

    a = GMP::F(z100, 4)
    assert_equal(a, ninety_six, "GMP::F.new should default round with RNDN")

    a = GMP::F(z100, 4, GMP::GMP_RNDN)
    assert_equal(a, ninety_six, "GMP::F.new should round with RNDN")

    a = GMP::F(z100, 4, GMP::GMP_RNDD)
    assert_equal(a, ninety_six, "GMP::F.new should round with RNDD")

    a = GMP::F(z100, 4, GMP::GMP_RNDZ)
    assert_equal(a, ninety_six, "GMP::F.new should round with RNDZ")

    a = GMP::F(z100, 4, GMP::GMP_RNDU)
    assert_equal(a, one_hundred_four, "GMP::F.new should round with RNDU")
  end
end
