require './test_helper'

class TC_swap < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(100)
    @b=GMP::Z.new(200)
    @c=GMP::Q.new(100,11)
    @d=GMP::Q.new(200,17)
  end
  
  def test_swap
    @a.swap @b
    @c.swap @d
    assert_equal(@a, 200, "GMP::Z should swap with GMP::Z.")
    assert_equal(@b, 100, "GMP::Z should swap with GMP::Z.")
    assert_equal(@c, GMP::Q.new(200,17), "GMP::Z should swap with GMP::Z.")
    assert_equal(@d, GMP::Q.new(100,11), "GMP::Z should swap with GMP::Z.")
  end
end
