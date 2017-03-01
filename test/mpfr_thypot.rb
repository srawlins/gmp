require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class MpfrTHypot < MiniTest::Test
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

  def small_tests
    x = GMP::F.new_2exp(1, GMP::F.emin-1, 8)
    y = GMP::F.new_2exp(1, GMP::F.emin-1, 8)
    z1 = GMP::F.new(2, 8)
    z1 = z1.sqrt * x
    z2 = x.hypot(y)

    assert_equal(z1, z2, "x.hypot(y) should == sqrt(2) * x")
  end

  def large_small_tests
    x = GMP::F.new_2exp(1, GMP::F.emax/2, 3)
    y = GMP::F.new_2exp(1, -1, 2)
    z = x.hypot(y)

    assert_equal(x, z, "hypot... something abount large_small")
  end

  def test_hypot
    small_tests
    large_small_tests

    emin = GMP::F.emin
    emax = GMP::F.emax
    GMP::F.emin = GMP::F.emin_min
    GMP::F.emax = GMP::F.emax_max

    if GMP::F.emin != emin || GMP::F.emax != emax
      small_tests
      large_small_tests
      GMP::F.emin = emin
      GMP::F.emax = emax
    end
  end
end
