require './test_helper'

class MPFR_TCBRT < Test::Unit::TestCase
  def setup
    @one = GMP::F(1)
    @zero = GMP::F(0)
    @inf = @one/@zero
    @nan = @inf - @inf
    @neg_one = GMP::F(-1)
    @neg_inf = @neg_one/@zero
    @rand_state = GMP::RandState.new
  end

  # This really should be moved to be a method of GMP::F
  def integer_component(f)
    index_e = f.to_s.index('e')
    exp = f.to_s[(index_e+1)..-1].to_i
    return 0 if exp < 0
    f.to_s[2..(index_e-1)][0,exp]
  end
  
  def special
    x = @nan.cbrt(GMP::GMP_RNDN)
    assert_true(x.nan?, "@nan.cbrt should be NaN")
    x = @inf.cbrt(GMP::GMP_RNDN)
    assert_true(x.infinite?, "@inf.cbrt should be +inf")
    assert_true(x > 0, "@inf.cbrt should be +inf")
    x = @neg_inf.cbrt(GMP::GMP_RNDN)
    assert_true(x.infinite?, "@neg_inf.cbrt should be -inf")
    assert_true(x < 0, "@neg_inf.cbrt should be -inf")
  end
  
  def test_cbrt
    special
  end
end
