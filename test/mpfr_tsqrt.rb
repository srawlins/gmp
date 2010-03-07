require 'test_helper'

GMP::GMP_RNDN

class MPFR_TSQRT < Test::Unit::TestCase
  def property1(p, rounding)
    x = GMP::F()
  end
  
  def test_prec
    (GMP::MPFR_PREC_MIN..128).each do |p|
      property1(p, GMP::GMP_RNDN)
    end
    #assert_equal(GMP::Z.new(), 0, "GMP::Z.new() should initialize to 0")
  end
end