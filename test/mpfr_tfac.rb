require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class MPFR_TFAC < Test::Unit::TestCase
  def setup
    @one = GMP::F(1)
    @zero = GMP::F(0)
    @inf = @one/@zero
    @nan = @inf - @inf
    @neg_one = GMP::F(-1)
    @neg_inf = @neg_one/@zero
    @neg_zero = GMP::F("-0")
    @rand_state = GMP::RandState.new
  end

  def special
    x = GMP::F.fac(119, GMP::GMP_RNDZ, 21)
    y = GMP::F("0.101111101110100110110E654", 21, 2)
    assert_equal(y, x, "fac should be correct")

    x = GMP::F("0.110111100001000001101010010001000111000100000100111000010011100011011111001100011110101000111101101100110001001100110100001001111110000101010000100100011100010011101110000001000010001100010000101001111E6250", 206, 2)
    y = GMP::F.fac(767, GMP::GMP_RNDN, 206)
  end

  def int
    f = GMP::Z.fac(0)
    (1..80).each do |n|
      f = f * n
      prec = f.size_in_base(2) - f.scan1(0)
      (GMP::MPFR_PREC_MIN..prec).each do |p|
        RND_MODES.each do |rnd|
          x = GMP::F.fac(n, rnd, p)
          y = GMP::F.new(f, p, rnd)
          assert_equal(y, x, "fac should be correct for integers")
        end
      end
    end
  end

  def test_fac
    special

    int

    assert_equal(1, GMP::F.fac(0), "fac(0) should give 1")

    # There are more tests in tfactorial.c, but they rely on return values and
    # mpfr_can_round... so not yet.
  end
end
