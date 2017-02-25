require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcRationalDenNum < MiniTest::Test
  def setup
    @p = GMP::Q.new(22, 7)
    @q = GMP::Q.new(-0.625)
  end

  def test_num
    assert_equal(@p.num, 22, "GMP::Q#num returns the numerator")
    assert_equal(@q.num, -5, "GMP::Q#num returns the numerator")
  end

  def test_den
    assert_equal(@p.den, 7, "GMP::Q#den returns the denominator")
    assert_equal(@q.den, 8, "GMP::Q#den returns the denominator")
  end
end
