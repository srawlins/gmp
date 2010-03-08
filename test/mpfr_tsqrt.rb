require 'test_helper'

class MPFR_TSQRT < Test::Unit::TestCase
  def setup
    @rand_state = GMP::RandState.new
  end

  # This really should be moved to be a method of GMP::F
  def integer_component(f)
    index_e = f.to_s.index('e')
    exp = f.to_s[(index_e+1)..-1].to_i
    return 0 if exp < 0
    f.to_s[2..(index_e-1)][0,exp]
  end
  
  def check_diverse(as, p, qs)
    q = GMP::F(as, p)
    q = q.sqrt
    assert_equal(integer_component(q), qs)
  end
  
  def property1(p, rounding)
    current_rounding_mode = GMP::F.default_rounding_mode
    GMP::F.default_rounding_mode = rounding
    x = @rand_state.mpfr_urandomb(p)
    y = @rand_state.mpfr_urandomb(p)
    z = x**2 + y**2
    z = x / z.sqrt
    assert_between(-1, 1, z, "-1 <= x/sqrt(x^2+y^2) <= 1 should hold.")
    GMP::F.default_rounding_mode = GMP.const_get(current_rounding_mode)
  end
  
  def property2(p, rounding)
    current_rounding_mode = GMP::F.default_rounding_mode
    GMP::F.default_rounding_mode = rounding
    x = @rand_state.mpfr_urandomb(p)
    y = (x ** 2).sqrt
    assert_true(x == y, "sqrt(x^2) = x should hold.")
    GMP::F.default_rounding_mode = GMP.const_get(current_rounding_mode)
  end
  
  def test_prec
    (GMP::MPFR_PREC_MIN..128).each do |p|
      property1(p, GMP::GMP_RNDN)
      property1(p, GMP::GMP_RNDU)
      property2(p, GMP::GMP_RNDN)
    end
    
    check_diverse("635030154261163106768013773815762607450069292760790610550915652722277604820131530404842415587328",
                  160,
                  "796887792767063979679855997149887366668464780637")
  end
end