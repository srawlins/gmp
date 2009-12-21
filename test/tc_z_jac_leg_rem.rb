require 'test_helper'

class TC_Z_Jacobi_Legendre_Remove < Test::Unit::TestCase
  def setup
    one         = GMP::(1)
    two         = GMP::(2)
    three       = GMP::(3)
    four        = GMP::(4)
    forty_five  = GMP::(45)
  end
  
  # I don't actually know the Jacobi symbol, so these are just from Wolfram Alpha.
  def test_jacobi
    assert_equal(one.jacobi(one),          1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(two.jacobi(one),          1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(forty_five.jacobi(one),   1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(one.jacobi(three),        1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(two.jacobi(three),       -1, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(three.jacobi(three),      0, "GMP::Z should jacobi(GMP::Z) correctly")
    assert_equal(forty_five.jacobi(three), 0, "GMP::Z should jacobi(GMP::Z) correctly")
    
    assert_equal(GMP::Z.jacobi(one, one),    1, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(four, one),   1, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(two, three), -1, "GMP::Z should jacobi(GMP::Z, GMP::Z) correctly")
  end
end