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
end
