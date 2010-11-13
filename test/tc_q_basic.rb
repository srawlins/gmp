require './test_helper'

# [Q op Q, Q op Z, Z op Q, Q op FixNum, Q op BigNum, FixNum op Q, BigNum op Q]
class TC_Q_Basic < Test::Unit::TestCase
  def setup
    @a=GMP::Q.new(100,11)
    @b=GMP::Q.new(200,17)
    @c=GMP::Z.new(40)
    @d=2**32
  end

  def test_add
    assert_equal(@a + @b, GMP::Q(3900, 187),       "GMP::Q should add GMP::Q correctly")
    assert_equal(@a + @c, GMP::Q(540,  11),        "GMP::Q should add GMP::Z correctly")
    assert_equal(@c + @a, GMP::Q(540,  11),        "GMP::Z should add GMP::Q correctly")
    assert_equal(@a +  2, GMP::Q(122,  11),        "GMP::Z should add Fixnum correctly")
    assert_equal(GMP::Q(47244640356, 11), @a + @d, "GMP::Z should add Bignum correctly")
    assert_equal(GMP::Q(122,  11),         2 + @a, "Fixnum should add GMP::Q correctly")
    assert_equal(@d + @a, GMP::Q(47244640356, 11), "Bignum should add GMP::Q correctly")
  end

  def test_sub
    assert_equal(@a - @b, GMP::Q(-500, 187),       "GMP::Q should subtract GMP::Q correctly")
    assert_equal(@a - @c, GMP::Q(-340, 11),        "GMP::Q should subtract GMP::Z correctly")
    assert_equal(@c - @a, GMP::Q(340,  11),        "GMP::Z should subtract GMP::Q correctly")
    assert_equal(@a -  2, GMP::Q(78,  11),         "GMP::Z should subtract Fixnum correctly")
    assert_equal(@a - @d, GMP::Q(-47244640156, 11),"GMP::Z should subtract Bignum correctly")
    assert_equal( 2 - @a, GMP::Q(-78,  11),        "Fixnum should subtract GMP::Q correctly")
    assert_equal(@d - @a, GMP::Q(47244640156, 11), "Bignum should subtract GMP::Q correctly")
  end

  def test_mul
    assert_equal(@a * @b, GMP::Q(20000, 187),      "GMP::Q should multiply GMP::Q correctly")
    assert_equal(@a * @c, GMP::Q(4000,  11),       "GMP::Q should multiply GMP::Z correctly")
    assert_equal(@c * @a, GMP::Q(4000,  11),       "GMP::Z should multiply GMP::Q correctly")
    assert_equal(@a *  2, GMP::Q(200,  11),        "GMP::Z should multiply Fixnum correctly")
    #assert_equal(@a * @d, GMP::Q(429496729600, 11),"GMP::Z should multiply Bignum correctly")  # SEGFAULT
    assert_equal( 2 * @a, GMP::Q(200,  11),        "Fixnum should multiply GMP::Q correctly")
    #assert_equal(@d * @a, GMP::Q(429496729600, 11),"Bignum should multiply GMP::Q correctly")  # SEGFAULT
  end
end
