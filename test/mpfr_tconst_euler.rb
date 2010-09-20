require './test_helper'

class MPFR_TCONST_EULER < Test::Unit::TestCase
  def setup
    @rand_state = GMP::RandState.new
  end

  def test_const_euler
    prec = 53

    y = GMP::F.const_euler(GMP::GMP_RNDN, 32)
    z = GMP::F("0.10010011110001000110011111100011", 32, 2)
    assert_equal(z, y, "Const Euler to precision 32 should be accurate.")

    (2..200).each do |p|
      z.prec= p
      t = GMP::F(0, p)
      yprec = p+10

      (0...GMP::MPFR_RND_MAX).each do |rnd|

      end
    end
  end
end
