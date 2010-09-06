require './test_helper'

class TC_F_arithmetics_coersion < Test::Unit::TestCase
  def setup
    @a = GMP::F.new(3.14, 100)
    @b = GMP::F.new(2.71, 200)
    @c = GMP::Z(3)
    @d = GMP::Q(7,2)
    @e = 2**32
  end
  
  def test_add
    assert_in_delta(         5.85,   @a +   @b, 1e-6)
    assert_in_delta(         5.85,   @b +   @a, 1e-6)
    assert_in_delta(         6.14,   @a +   @c, 1e-6)
    assert_in_delta(         6.14,   @c +   @a, 1e-6)
    assert_in_delta(         6.64,   @a +   @d, 1e-6)
    assert_in_delta(         6.64,   @d +   @a, 1e-6)
    assert_in_delta(         5.14,   @a +    2, 1e-6)
    assert_in_delta(         5.14,    2 +   @a, 1e-6)
    assert_in_delta(4294967299.14,   @a +   @e, 1e-6)
    assert_in_delta(4294967299.14,   @e +   @a, 1e-6)
    assert_in_delta(         7.65,   @a + 4.51, 1e-6)
    assert_in_delta(         7.65, 4.51 +   @a, 1e-6)
  end
  
  def test_sub
    assert_in_delta(          0.43,   @a -   @b, 1e-6)
    assert_in_delta(         -0.43,   @b -   @a, 1e-6)
    assert_in_delta(          0.14,   @a -   @c, 1e-6)
    assert_in_delta(         -0.14,   @c -   @a, 1e-6)
    assert_in_delta(         -0.36,   @a -   @d, 1e-6)
    assert_in_delta(          0.36,   @d -   @a, 1e-6)
    assert_in_delta(          1.14,   @a -    2, 1e-6)
    assert_in_delta(         -1.14,    2 -   @a, 1e-6)
    assert_in_delta(-4294967292.86,   @a -   @e, 1e-6)
    assert_in_delta( 4294967292.86,   @e -   @a, 1e-6)
    assert_in_delta(         -1.37,   @a - 4.51, 1e-6)
    assert_in_delta(          1.37, 4.51 -   @a, 1e-6)
  end
  
  def test_mul
    assert_in_delta(          8.5094,   @a *   @b, 1e-6)
    assert_in_delta(          8.5094,   @b *   @a, 1e-6)
    assert_in_delta(          9.42  ,   @a *   @c, 1e-6)
    assert_in_delta(          9.42  ,   @c *   @a, 1e-6)
    assert_in_delta(         10.99  ,   @a *   @d, 1e-6)
    assert_in_delta(         10.99  ,   @d *   @a, 1e-6)
    assert_in_delta(          6.28  ,   @a *    2, 1e-6)
    assert_in_delta(          6.28  ,    2 *   @a, 1e-6)
    assert_in_delta(13486197309.44  ,   @a *   @e, 1e-6)
    assert_in_delta(13486197309.44  ,   @e *   @a, 1e-6)
    assert_in_delta(         14.1614,   @a * 4.51, 1e-6)
    assert_in_delta(         14.1614, 4.51 *   @a, 1e-6)
  end
  
  def test_div
    assert_in_delta(          1.1586715867,   @a /   @b, 1e-6)
    assert_in_delta(          0.8630573248,   @b /   @a, 1e-6)
    assert_in_delta(          1.0466666667,   @a /   @c, 1e-6)
    assert_in_delta(          0.9554140127,   @c /   @a, 1e-6)
    assert_in_delta(          0.8971428571,   @a /   @d, 1e-6)
    assert_in_delta(          1.1146496815,   @d /   @a, 1e-6)
    assert_in_delta(          1.57        ,   @a /    2, 1e-6)
    assert_in_delta(          0.6369426752,    2 /   @a, 1e-6)
    assert_in_delta(          0.0000000007,   @a /   @e, 1e-6)
    assert_in_delta( 1367823979.6178343949,   @e /   @a, 1e-6)
    assert_in_delta(          0.6962305987,   @a / 4.51, 1e-6)
    assert_in_delta(          1.4363057325, 4.51 /   @a, 1e-6)
  end
end
