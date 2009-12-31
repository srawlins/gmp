require 'test_helper'

# Tested: [Z op Z, Z op FixNum, Z op BigNum, FixNum op Z, BigNum op Z]
# Things are tested both ways because the implementation is asymetrical
class TC_Z_GCD_LCM_Invert < Test::Unit::TestCase
  def setup
    @a = GMP::Z( 24)  # 2^3 * 3
    @b = GMP::Z(  8)  # 2^3
    @c = GMP::Z(  3)  # 3
    @d = GMP::Z(  7)  # 7
    @e = GMP::Z( 11)  # 11
    @f = GMP::Z( 25)  # 25
    @g = GMP::Z(720)  # 2^4 * 3^2 * 5 = 1*2*3*4*5*6
  end

  def test_gcd
    assert_equal(GMP::Z( 8), @a.gcd(@b), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z( 3), @a.gcd(@c), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z( 1), @c.gcd(@d), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z( 1), @a.gcd(@f), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z(24), @a.gcd(@g), "GMP::Z should gcd correctly")
  end
end