require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TC_MPFR_Integer < Test::Unit::TestCase
  def test_mpfr_integer_positive
    assert(GMP::F(1.0).integer?,         "MPFR should decide whether a number is an integer correctly.")
    assert(GMP::F(2.0).integer?,         "MPFR should decide whether a number is an integer correctly.")
    assert(GMP::F(0.0).integer?,         "MPFR should decide whether a number is an integer correctly.")
    assert(GMP::F(1.0 * 2**30).integer?, "MPFR should decide whether a number is an integer correctly.")
    assert(GMP::F(-1.0).integer?,        "MPFR should decide whether a number is an integer correctly.")
    assert(GMP::F(-31.0).integer?,       "MPFR should decide whether a number is an integer correctly.")
  end

  def test_mpfr_integer_negative
    assert_false(GMP::F(1.1).integer?,           "MPFR should decide whether a number is an integer correctly.")
    assert_false(GMP::F(0.1).integer?,           "MPFR should decide whether a number is an integer correctly.")
    assert_false(GMP::F(0.0000001).integer?,     "MPFR should decide whether a number is an integer correctly.")
    assert_false(GMP::F(3.14159).integer?,       "MPFR should decide whether a number is an integer correctly.")
    assert_false(GMP::F(-0.1010010001).integer?, "MPFR should decide whether a number is an integer correctly.")
  end
end
