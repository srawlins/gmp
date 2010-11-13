require './test_helper'

class TC_MPFR_Constants < Test::Unit::TestCase
  def test_mpfr_constants
    assert_instance_of(String,   GMP::MPFR_VERSION,  "GMP::MPFR_VERSION should be a String")
    assert_instance_of(Fixnum,   GMP::MPFR_PREC_MIN, "GMP::MPFR_PREC_MIN should be a Fixnum")
    assert_instance_of(Fixnum,   GMP::MPFR_PREC_MAX, "GMP::MPFR_PREC_MAX should be a Fixnum")
    assert_instance_of(GMP::Rnd, GMP::GMP_RNDN,      "GMP::GMP_RNDN should be a GMP::Rnd")
    assert_instance_of(GMP::Rnd, GMP::GMP_RNDZ,      "GMP::GMP_RNDZ should be a GMP::Rnd")
    assert_instance_of(GMP::Rnd, GMP::GMP_RNDU,      "GMP::GMP_RNDU should be a GMP::Rnd")
    assert_instance_of(GMP::Rnd, GMP::GMP_RNDD,      "GMP::GMP_RNDD should be a GMP::Rnd")
  end
end
