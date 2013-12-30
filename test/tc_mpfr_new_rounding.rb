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

  def test_new_bignum
    over = GMP::F(0.36472996384e+20, 32)
    under = GMP::F(0.36472996375e+20, 32)

    a = GMP::F(3**41, 32)
    assert_equal(a, under, "GMP::F.new should default round with RNDN")

    a = GMP::F(3**41, 32, GMP::GMP_RNDN)
    assert_equal(a, under, "GMP::F.new should round with RNDN")

    a = GMP::F(3**41, 32, GMP::GMP_RNDD)
    assert_equal(a, under, "GMP::F.new should round with RNDD")

    a = GMP::F(3**41, 32, GMP::GMP_RNDZ)
    assert_equal(a, under, "GMP::F.new should round with RNDZ")

    a = GMP::F(3**41, 32, GMP::GMP_RNDU)
    assert_equal(a, over, "GMP::F.new should round with RNDU")
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

  def test_new_gmpq
    pi = GMP::Q(22,7)
    below = GMP::F(0.3141e+1, 8)
    above = GMP::F(0.3156e+1, 8)

    a = GMP::F(pi, 8)
    assert_equal(a, below, "GMP::F.new should default round with RNDN")

    a = GMP::F(pi, 8, GMP::GMP_RNDN)
    assert_equal(a, below, "GMP::F.new should round with RNDN")

    a = GMP::F(pi, 8, GMP::GMP_RNDD)
    assert_equal(a, below, "GMP::F.new should round with RNDD")

    a = GMP::F(pi, 8, GMP::GMP_RNDZ)
    assert_equal(a, below, "GMP::F.new should round with RNDZ")

    a = GMP::F(pi, 8, GMP::GMP_RNDU)
    assert_equal(a, above, "GMP::F.new should round with RNDU")
  end

  def test_new_float
    ohoh977 = GMP::F(0.977e-2, 4)
    oh107 = GMP::F(0.107e-1, 4)

    a = GMP::F(0.01, 4)
    assert_equal(a, ohoh977, "GMP::F.new should default round with RNDN")

    a = GMP::F(0.01, 4, GMP::GMP_RNDN)
    assert_equal(a, ohoh977, "GMP::F.new should round with RNDN")

    a = GMP::F(0.01, 4, GMP::GMP_RNDD)
    assert_equal(a, ohoh977, "GMP::F.new should round with RNDD")

    a = GMP::F(0.01, 4, GMP::GMP_RNDZ)
    assert_equal(a, ohoh977, "GMP::F.new should round with RNDZ")

    a = GMP::F(0.01, 4, GMP::GMP_RNDU)
    assert_equal(a, oh107, "GMP::F.new should round with RNDU")
  end
end
