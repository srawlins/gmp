require './test_helper'

class TC_to_i_to_d < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)
    #@b = GMP::Z.pow(2,32)
    @c = GMP::Q.new(200,11)
  end
  
  def test_to_i
    assert_equal(@a.to_i, 100, "GMP::Z should to_i correctly.")
    assert_equal(@a.to_i.class, Fixnum, "GMP::Z.to_i should be a Fixnum.")
    #assert_equal(@b.to_i, 2**32, "GMP::Z (Bignum) should to_i correctly.")
    #assert_equal(@b.to_i.class, Bignum, "GMP::Z.to_i should be a Bignum.")
  end
  
  def test_to_d
    assert_equal(@a.to_d, 100.0, "GMP::Z should to_d correctly.")
    assert_equal(@a.to_d.class, Float, "GMP::Z.to_d should be a Float.")
    #assert_equal(@b.to_d, 2**32*1.0, "GMP::Z should to_d correctly.")
    #assert_equal(@b.to_d.class, Float, "GMP::Z.to_d should be a Float.")
    assert_equal(@c.to_d.class, Float, "GMP::Q.to_d should be a Float.")
  end
end
