require './test_helper'

class TC_floor_ceil_truncate < Test::Unit::TestCase
  def setup
    @a = GMP::Q.new(200,11)
    @b = -@a
    @c = GMP::Q.new(70,10)
  end
  
  def test_floor_ceil_truncate
    assert_equal(@a.floor, 18, "GMP::Q should floor.")
    assert_equal(@a.ceil, 19, "GMP::Q should ceil.")
    assert_equal(@a.trunc, 18, "GMP::Q should truncate.")
    assert_equal(@b.floor, -19, "GMP::Q (negative) should floor.")
    assert_equal(@b.ceil, -18, "GMP::Q (negative) should ceil.")
    assert_equal(@b.trunc, -18, "GMP::Q (negative) should truncate.")
    assert_equal(@c.floor, 7, "GMP::Q (integer) should floor.")
    assert_equal(@c.ceil, 7, "GMP::Q (integer) should ceil.")
    assert_equal(@c.trunc, 7, "GMP::Q (integer) should truncate.")
  end
end
