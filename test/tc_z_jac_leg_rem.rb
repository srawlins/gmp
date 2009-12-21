require 'test_helper'

class TC_Z_Jacobi_Legendre_Remove < Test::Unit::TestCase
  def setup
    @one         = GMP::Z(1)
    @two         = GMP::Z(2)
    @three       = GMP::Z(3)
    @four        = GMP::Z(4)
    @forty_five  = GMP::Z(45)
    @neg_one     = GMP::Z(-1)
  end
  
  # I don't actually know the Jacobi symbol, so these are just from Wolfram Alpha.
  def test_jacobi
    assert_equal(@one.jacobi(@one),          1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(@two.jacobi(@one),          1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(@forty_five.jacobi(@one),   1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(@one.jacobi(@three),        1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(@two.jacobi(@three),       -1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(@three.jacobi(@three),      0, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(@forty_five.jacobi(@three), 0, "GMP::Z should jacobi(GMP::Z) correctly")
    
    assert_equal(GMP::Z.jacobi(@one, @one),     1, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(@four, @one),    1, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(@two, @three),  -1, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(@three, @three), 0, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
    
    assert_raise(RangeError) {   @two.jacobi(@two) }
    assert_raise(RangeError) { @three.jacobi(@neg_one) }
    assert_raise(RangeError) { GMP::Z.jacobi(@two, @two) }
    assert_raise(RangeError) { GMP::Z.jacobi(@three, @neg_one) }
  end
end