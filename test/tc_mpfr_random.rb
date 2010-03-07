require 'test_helper'

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
  
  # def test_reseed
    # @c = GMP::RandState.new
    # @c.seed(1000)
    # assert_equal( 6461, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # assert_equal(42961, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # assert_equal(44699, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # @c.seed(1000)
    # assert_equal( 6461, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # assert_equal(42961, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # assert_equal(44699, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # @c.seed(1000)
    # assert_equal( 6461, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # assert_equal(42961, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    # assert_equal(44699, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
  # end
  
  # def test_random_independent_states
    # @d = GMP::RandState.new
    # @d.seed(13579)
    # @e = GMP::RandState.new
    # @e.seed(13579)
    
    # assert_equal( 392, @d.urandomb(12), "GMP::RandState should be independent correctly.")
    # assert_equal( 507, @d.urandomb(12), "GMP::RandState should be independent correctly.")
    # assert_equal( 392, @e.urandomb(12), "GMP::RandState should be independent correctly.")
    # assert_equal( 507, @e.urandomb(12), "GMP::RandState should be independent correctly.")
    # assert_equal(1657, @d.urandomb(12), "GMP::RandState should be independent correctly.")
    # assert_equal(1657, @e.urandomb(12), "GMP::RandState should be independent correctly.")
  # end
end