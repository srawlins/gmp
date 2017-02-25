require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcIntegerHamdist < MiniTest::Test
  def setup
    @z32 = GMP::Z(32)  # 100000
    @z33 = GMP::Z(33)  # 100001
    @z34 = GMP::Z(34)  # 100010
    @z35 = GMP::Z(35)  # 100011
    @z36 = GMP::Z(36)  # 100100
    @z37 = GMP::Z(37)  # 100101
    @z38 = GMP::Z(38)  # 100110
    @z39 = GMP::Z(39)  # 100111
  end

  def test_hamdist
    assert_equal(0, @z32.hamdist(@z32), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(1, @z32.hamdist(@z33), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(1, @z32.hamdist(@z34), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(2, @z32.hamdist(@z35), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(1, @z32.hamdist(@z36), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(2, @z32.hamdist(@z37), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(2, @z32.hamdist(@z38), "GMP::Z should hamdist(GMP::Z:positive) correctly")
    assert_equal(3, @z32.hamdist(@z39), "GMP::Z should hamdist(GMP::Z:positive) correctly")
  end
end
