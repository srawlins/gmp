require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcMpfrInfNanZero < MiniTest::Test
  def setup
    @neg_inf = GMP::F(0).log  # -Inf
    @inf = -@neg_inf          # Inf
    @nan = @neg_inf + @inf    # NaN
    @zero = GMP::F(0)
  end

  def test_nan
    assert_equal(@nan, GMP::F.nan, "NaN should be NaN")
  end

  def test_inf
    assert_equal(@inf, GMP::F.inf, "inf should be inf")
    assert_equal(@neg_inf, GMP::F.inf(-1), "neg_inf should be neg_inf")
  end

  def test_zero
    assert_equal(@zero, GMP::F.zero, "zero should be zero")
    assert_equal(-@zero, GMP::F.zero(-1), "negaitve zero should be negative zero")
  end

  def test_type
    assert_raise(TypeError) { GMP::F.inf(0.5); }
    assert_raise(TypeError) { GMP::F.zero(0.5); }
  end
end
