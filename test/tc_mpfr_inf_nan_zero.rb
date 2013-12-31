require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TC_MPFR_Inf_Nan_Zero < Test::Unit::TestCase
  def setup
    @neg_inf = GMP::F(0).log  # -Inf
    @inf = -@neg_inf          # Inf
    @nan = @neg_inf + @inf    # NaN
  end

  def test_nan
    assert_equal(@nan, GMP::F.nan, "NaN should be NaN")
  end

  def test_inf
    assert_equal(@inf, GMP::F.inf, "inf should be inf")
    assert_equal(@neg_inf, GMP::F.inf(-1), "neg_inf should be neg_inf")
  end
end
