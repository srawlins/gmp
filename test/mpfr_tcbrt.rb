require './test_helper'

class MPFR_TCBRT < Test::Unit::TestCase
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
    x = @zero.cbrt(GMP::GMP_RNDN)
    assert_true(x.zero?, "@zero.cbrt should be +0")
    assert_equal(GMP::F(0), x, "@zero.cbrt should be +0")
    x = @neg_zero.cbrt(GMP::GMP_RNDN)
    assert_true(x.zero?, "@neg_zero.cbrt should be -0")
    assert_equal(GMP::F("-0"), x, "@neg_zero.cbrt should be -0")

    x = GMP::F("8.39005285514734966412e-01", 53)
    x = x.cbrt(GMP::GMP_RNDN)
    y = GMP::F("9.43166207799662426048e-01")
    assert_equal(y, x, "cbrt should be correct")

    x = GMP::F("0.10000100001100101001001001011001", 32, 2)
    x = x.cbrt(GMP::GMP_RNDN)
    y = GMP::F("0.11001101011000100111000111111001", 32, 2)
    assert_equal(y, x, "cbrt should be correct")

    x = GMP::F("-0.1100001110110000010101011001011", 32, 2)
    x = x.cbrt(GMP::GMP_RNDD)
    y = GMP::F("-0.11101010000100100101000101011001", 32, 2)
    assert_equal(y, x, "cbrt should be correct")

    x = GMP::F("0.1010001111011101011011000111001011001101100011110110010011011011011010011001100101e-7", 82, 2)
    y = x.cbrt(GMP::GMP_RNDD, 27)
    x = GMP::F("0.101011110001110001000100011E-2", 82, 2)
    assert_equal(x, y, "cbrt should be correct")

    x = GMP::F("0.1010000000011010000000011001111110111110011101101000011110" +
                 "0010011010011100110110011111000111000101101101011001001110" +
                 "0101111001111100001010010100111011101100000011011000101100" +
                 "010000000011000101001010001001E-5", 204, 2)
    y = x.cbrt(GMP::GMP_RNDD, 38)
    x = GMP::F("0.10001001111010011011101000010110110010E-1", 204, 2)
    assert_equal(x, y, "cbrt should be correct")

    x = GMP::F("1.1000E-2", 5, 2)
    x = x.cbrt(GMP::GMP_RNDN)
    y = GMP::F("1.0111E-1", 5, 2)
    assert_equal(y, x, "cbrt should be correct")
  end
  
  def test_cbrt
    special

    (2..100).each do |p|
      [GMP::GMP_RNDN, GMP::GMP_RNDZ, GMP::GMP_RNDD, GMP::GMP_RNDU].each do |rnd|
        x = GMP::F(1).cbrt(rnd)
        assert_equal(GMP::F(1), x, "1.cbrt should be 1 (prec #{p}) in rounding mode #{rnd}")
        x = GMP::F(-1).cbrt(rnd)
        assert_equal(GMP::F(-1), x, "-1.cbrt should be -1 (prec #{p}) in rounding mode #{rnd}")

        next unless p>=5

        (-12..12).each do |i|
          x = GMP::F(27)
          x *= GMP::F(2)**(3*i)
          x = x.cbrt
          assert_equal(GMP::F(3)*GMP::F(2)**i, x,
                       "27*2^#{3*i} should be 3*2^#{i} (prec #{p}, rounding mode #{rnd}")
        end
      end
    end
  end
end
