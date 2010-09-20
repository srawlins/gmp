require './test_helper'

class MPFR_TCONST_EULER < Test::Unit::TestCase
  def setup
    @rand_state = GMP::RandState.new
    
    if GMP::MPFR_VERSION >= "3.0.0"
      @mpfr_rnd_max = 5
    else
      @mpfr_rnd_max = 4
    end
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

      (0...@mpfr_rnd_max).each do |rnd|
        y.prec = yprec
        GMP::F.const_euler(y, rnd)
        err = rnd == GMP::GMP_RNDN ? yprec+1 : yprec
        if y.can_round?(err, rnd, rnd, p)
        
        end
      end
    end
  end
end
