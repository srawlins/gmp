require 'test_helper'

class TC_MPFR_Random < Test::Unit::TestCase
  def setup
  end

  def test_urandomb
    @a = GMP::RandState.new
    @a.seed(13579)
    g1 = [ 392,  507, 1657, 3963, 2275, 1264, 2370, 2152, 3282,  407,
          1746, 2488, 1753, 1639,  721, 3291, 3697, 2258, 3990, 1019]
    g1.size.times do |i|
      assert_equal(g1[i], @a.urandomb(12), "GMP::RandState should urandomb predictably.")
    end
    
    @b = GMP::RandState.new
    @b.seed(314159)
    g2 = [   20,     5,    40,    228,    223,    205,   1750,    690,    5794,   13752,
          31377, 49064, 37010, 187285, 489973, 312091, 550520, 916752, 3213943, 1561294]
    g2.size.times do |i|
      assert_equal(g2[i], @b.urandomb(i+5), "GMP::RandState should urandomb predictably.")
    end
  end
  
  def test_reseed
    @c = GMP::RandState.new
    @c.seed(1000)
    assert_equal( 6461, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    assert_equal(42961, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    assert_equal(44699, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    @c.seed(1000)
    assert_equal( 6461, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    assert_equal(42961, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    assert_equal(44699, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    @c.seed(1000)
    assert_equal( 6461, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    assert_equal(42961, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
    assert_equal(44699, @c.urandomb(16), "GMP::RandState should re-seed correctly.")
  end
  
  def test_random_independent_states
    @d = GMP::RandState.new
    @d.seed(13579)
    @e = GMP::RandState.new
    @e.seed(13579)
    
    assert_equal( 392, @d.urandomb(12), "GMP::RandState should be independent correctly.")
    assert_equal( 507, @d.urandomb(12), "GMP::RandState should be independent correctly.")
    assert_equal( 392, @e.urandomb(12), "GMP::RandState should be independent correctly.")
    assert_equal( 507, @e.urandomb(12), "GMP::RandState should be independent correctly.")
    assert_equal(1657, @d.urandomb(12), "GMP::RandState should be independent correctly.")
    assert_equal(1657, @e.urandomb(12), "GMP::RandState should be independent correctly.")
  end
end