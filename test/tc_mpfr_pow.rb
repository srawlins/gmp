require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcMpfrPow < MiniTest::Test
  def setup
    @a = GMP::F(0.25)
  end

  def test_mpfr_pow
    assert_equal(@a ** 2,            0.0625, "GMP::F#pow(x : Fixnum) calculates correctly.")
    assert_equal(@a ** -2,          16,      "GMP::F#pow(x : Fixnum) calculates correctly.")
    assert_equal(@a ** 0,            1,      "GMP::F#pow(x : Fixnum) calculates correctly.")
    assert_equal(@a ** 0.5,          0.5,    "GMP::F#pow(x : Float) calculates correctly.")
    assert_equal(@a ** 0.0,          1,      "GMP::F#pow(x : Float) calculates correctly.")
    assert_equal(@a ** GMP::Z(2),    0.0625, "GMP::F#pow(x : GMP::Z) calculates correctly.")
    assert_equal(@a ** GMP::Q(1, 2), 0.5,    "GMP::F#pow(x : GMP::Q) calculates correctly.")
  end
end
