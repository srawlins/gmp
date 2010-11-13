require './test_helper'

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
  
  def test_gcd2
    assert_equal(GMP::Z( 8), (-@a).gcd(@b), "GMP::Z should gcd negatives correctly")
    assert_equal(GMP::Z( 3), (-@a).gcd(@c), "GMP::Z should gcd negatives correctly")
    assert_equal(GMP::Z( 1), (-@c).gcd(@d), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z(24), (-@a).gcd(@g), "GMP::Z should gcd correctly")
    
    assert_equal(GMP::Z( 8), @a.gcd(-@b), "GMP::Z should gcd negatives correctly")
    assert_equal(GMP::Z( 3), @a.gcd(-@c), "GMP::Z should gcd negatives correctly")
    assert_equal(GMP::Z( 1), @c.gcd(-@d), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z(24), @a.gcd(-@g), "GMP::Z should gcd correctly")
    
    assert_equal(GMP::Z( 8), (-@a).gcd(-@b), "GMP::Z should gcd negatives correctly")
    assert_equal(GMP::Z( 3), (-@a).gcd(-@c), "GMP::Z should gcd negatives correctly")
    assert_equal(GMP::Z( 1), (-@c).gcd(-@d), "GMP::Z should gcd correctly")
    assert_equal(GMP::Z(24), (-@a).gcd(-@g), "GMP::Z should gcd correctly")
  end
  
  def test_invert
    assert_equal(GMP::Z( 1), GMP::Z( 1).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 6), GMP::Z( 2).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 4), GMP::Z( 3).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 3), GMP::Z( 4).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 9), GMP::Z( 5).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 2), GMP::Z( 6).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 8), GMP::Z( 7).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 7), GMP::Z( 8).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 5), GMP::Z( 9).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z(10), GMP::Z(10).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 0), GMP::Z(11).invert(@e), "GMP::Z should invert correctly")
    
    assert_equal(GMP::Z( 1), GMP::Z(12).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 6), GMP::Z(13).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 4), GMP::Z(14).invert(@e), "GMP::Z should invert correctly")
    assert_equal(GMP::Z( 3), GMP::Z(15).invert(@e), "GMP::Z should invert correctly")
  end
end
