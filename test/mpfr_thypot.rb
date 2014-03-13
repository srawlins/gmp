require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class MpfrTHypot < Test::Unit::TestCase
  def test_special
    x = GMP::F.nan
    y = GMP::F.new
    assert_equal(GMP::F.nan, x.hypot(y), "NaN.hypot(...) is NaN")

    x = GMP::F.inf
    y = GMP::F.inf(-1)
    assert_equal(GMP::F.inf, x.hypot(y), "Inf.hypot(-Inf) is Inf")

    x = GMP::F.nan
    y = GMP::F.inf(-1)
    assert_equal(GMP::F.inf, x.hypot(y), "NaN.hypot(-Inf) is Inf")
  end

  def test_large
    x = GMP::F(21)
    y = GMP::F(28)
    z = GMP::F(35)

    # TODO: need mpfr_mul_2ui!!!
  end

  def test_small
    # TODO: need GMP::F.new_2exp
    #x = mpfr_set_si_2exp(1, GMP::F.emin - 1, MPFR_RNDN)
  end

  def test_large_small
    # TODO: need GMP::F.new_2exp
  end

  def test_hypot
    emin = GMP::F.emin
    emax = GMP::F.emax
    # TODO: need GMP::F.emin_min, GMP::F.emax_max
  end
end
