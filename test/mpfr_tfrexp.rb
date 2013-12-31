require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class MPFR_TFrexp < Test::Unit::TestCase
  def setup
  end

  def special
    exp, y = GMP::F.nan.frexp
    assert_true(y.nan?, "frexp of NaN should be correct")

    exp, y = GMP::F.inf.frexp
    assert_true(y.infinite? && y > 0, "frexp of Inf should be correct")

    exp, y = GMP::F.inf(-1).frexp
    assert_true(y.infinite? && y < 0, "frexp of -Inf should be correct")

    exp, y = GMP::F.zero.frexp
    assert_equal(y, GMP::F.zero, "frexp of zero should be correct")

    exp, y = GMP::F.zero(-1).frexp
    assert_equal(y, GMP::F.zero(-1), "frexp of -zero should be correct")

    exp, y = GMP::F(17).frexp
    assert_equal(exp, 5, "frexp of 17 should return exp of 5")
    assert_equal(y, GMP::F("0.53125"), "frexp of 17 should return exp of 5")

    exp, y = GMP::F(17).frexp(GMP::GMP_RNDN, 4)
    assert_equal(exp, 5, "frexp of 17 should return exp of 5")
    assert_equal(y, GMP::F("0.5", 4), "frexp of 17 should return exp of 5")

    exp, y = GMP::F(17).frexp(GMP::GMP_RNDZ, 4)
    assert_equal(exp, 5, "frexp of 17 should return exp of 5")
    assert_equal(y, GMP::F("0.5", 4), "frexp of 17 should return exp of 5")

    exp, y = GMP::F(17).frexp(GMP::GMP_RNDD, 4)
    assert_equal(exp, 5, "frexp of 17 should return exp of 5")
    assert_equal(y, GMP::F("0.5", 4), "frexp of 17 should return exp of 5")

    exp, y = GMP::F(17).frexp(GMP::GMP_RNDU, 4)
    assert_equal(exp, 5, "frexp of 17 should return exp of 5")
    assert_equal(y, GMP::F("0.562", 4), "frexp of 17 should return exp of 5")
  end

  def test_fac
    special
  end
end
