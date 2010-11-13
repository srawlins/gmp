require './test_helper'

class TC_MPFR_Rounding < Test::Unit::TestCase
  def setup
    if GMP::MPFR_VERSION < "3.0.0"
      @prefix = "GMP"
    else
      @prefix = "MPFR"
    end
  end

  def test_rounding_mode
    assert_equal(0, GMP::GMP_RNDN.mode, "GMP::Rnd.mode should be correct.")
    assert_equal(1, GMP::GMP_RNDZ.mode, "GMP::Rnd.mode should be correct.")
    assert_equal(2, GMP::GMP_RNDU.mode, "GMP::Rnd.mode should be correct.")
    assert_equal(3, GMP::GMP_RNDD.mode, "GMP::Rnd.mode should be correct.")
  end
  
  def test_rounding_name
    assert_equal(@prefix+"_RNDN", GMP::GMP_RNDN.name, "GMP::Rnd.name should be correct.")
    assert_equal(@prefix+"_RNDZ", GMP::GMP_RNDZ.name, "GMP::Rnd.name should be correct.")
    assert_equal(@prefix+"_RNDU", GMP::GMP_RNDU.name, "GMP::Rnd.name should be correct.")
    assert_equal(@prefix+"_RNDD", GMP::GMP_RNDD.name, "GMP::Rnd.name should be correct.")
  end
  
  def test_rounding_ieee754
    assert_equal("roundTiesToEven", GMP::GMP_RNDN.ieee754, "GMP::Rnd.ieee754 should be correct.")
    assert_equal("roundTowardZero", GMP::GMP_RNDZ.ieee754, "GMP::Rnd.ieee754 should be correct.")
    assert_equal("roundTowardPositive", GMP::GMP_RNDU.ieee754, "GMP::Rnd.ieee754 should be correct.")
    assert_equal("roundTowardNegative", GMP::GMP_RNDD.ieee754, "GMP::Rnd.ieee754 should be correct.")
  end
end
