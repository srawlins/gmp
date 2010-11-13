require './test_helper'

class TC_MPFR_Random < Test::Unit::TestCase
  def setup
  end

  def test_urandomb
    @a = GMP::RandState.new
    @a.seed(577)
    g1 = [
      GMP::F("0.39810885576093713"), GMP::F("0.97212443610368071"), GMP::F("0.23084385480845748"),
      GMP::F("0.29428636987062717"), GMP::F("0.76895428585342840"), GMP::F("0.69329332764704654"),
      GMP::F("0.60252711974045448"), GMP::F("0.03530920538566051"), GMP::F("0.60242124958975374"),
    ]
    g1.size.times do |i|
      assert_in_delta(g1[i], @a.mpfr_urandomb, 1e-12, "GMP::RandState should mpfr_urandomb predictably.")
    end
  end
  
  def test_reseed
    @c = GMP::RandState.new
    @c.seed(1000)
    assert_in_delta(GMP::F("0.78522728792921048"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    assert_in_delta(GMP::F("0.94422653925308231"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    assert_in_delta(GMP::F("0.60678541956109799"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    @c.seed(1000)
    assert_in_delta(GMP::F("0.78522728792921048"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    assert_in_delta(GMP::F("0.94422653925308231"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    assert_in_delta(GMP::F("0.60678541956109799"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    @c.seed(1000)
    assert_in_delta(GMP::F("0.78522728792921048"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    assert_in_delta(GMP::F("0.94422653925308231"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    assert_in_delta(GMP::F("0.60678541956109799"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
  end
  
  def test_random_independent_states
    @d = GMP::RandState.new
    @d.seed(577)
    @e = GMP::RandState.new
    @e.seed(577)
    
    assert_in_delta(GMP::F("0.39810885576093713"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
    assert_in_delta(GMP::F("0.97212443610368071"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
    assert_in_delta(GMP::F("0.39810885576093713"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
    assert_in_delta(GMP::F("0.97212443610368071"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
    assert_in_delta(GMP::F("0.23084385480845748"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
    assert_in_delta(GMP::F("0.23084385480845748"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
  end
end
