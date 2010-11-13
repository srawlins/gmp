require './test_helper'

class TC_Z_Jacobi_Legendre_Remove < Test::Unit::TestCase
  def setup
    @one         = GMP::Z(1)
    @two         = GMP::Z(2)
    @three       = GMP::Z(3)
    @four        = GMP::Z(4)
    @five        = GMP::Z(5)
    @nine        = GMP::Z(9)
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
    
    assert_equal(GMP::Z.jacobi(1, @one),    1, "GMP::Z should jacobi(Fixnum, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(4, @one),    1, "GMP::Z should jacobi(Fixnum, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(2, @three), -1, "GMP::Z should jacobi(Fixnum, GMP::Z) correctly")
    assert_equal(GMP::Z.jacobi(3, @three),  0, "GMP::Z should jacobi(Fixnum, GMP::Z) correctly")
    
    assert_equal(GMP::Z.jacobi(@one, 1),   1, "GMP::Z should jacobi(GMP::Z, Fixnum) correctly")
    assert_equal(GMP::Z.jacobi(@four, 1),  1, "GMP::Z should jacobi(GMP::Z, Fixnum) correctly")
    assert_equal(GMP::Z.jacobi(@two, 3),  -1, "GMP::Z should jacobi(GMP::Z, Fixnum) correctly")
    assert_equal(GMP::Z.jacobi(@three, 3), 0, "GMP::Z should jacobi(GMP::Z, Fixnum) correctly")
    
    assert_equal(GMP::Z.jacobi(1, 1),  1, "GMP::Z should jacobi(Fixnum, Fixnum) correctly")
    assert_equal(GMP::Z.jacobi(4, 1),  1, "GMP::Z should jacobi(Fixnum, Fixnum) correctly")
    assert_equal(GMP::Z.jacobi(2, 3), -1, "GMP::Z should jacobi(Fixnum, Fixnum) correctly")
    assert_equal(GMP::Z.jacobi(3, 3),  0, "GMP::Z should jacobi(Fixnum, Fixnum) correctly")
    
    assert_raise(RangeError) {   @two.jacobi(@two) }
    assert_raise(RangeError) { @three.jacobi(@neg_one) }
    assert_raise(RangeError) { GMP::Z.jacobi(@two, @two) }
    assert_raise(RangeError) { GMP::Z.jacobi(@three, @neg_one) }
    assert_raise(RangeError) { GMP::Z.jacobi(@two, 2) }
    assert_raise(RangeError) { GMP::Z.jacobi(3, @neg_one) }
    assert_raise(RangeError) { GMP::Z.jacobi(3, -1) }
  end
  
  def test_legendre
    assert_equal(@one.legendre(@three),  1, "GMP::Z should legendre(GMP::Z) correctly")
    assert_equal(@two.legendre(@three), -1, "GMP::Z should legendre(GMP::Z) correctly")
    
    assert_raise(RangeError) { @one.legendre(@one) }
    assert_raise(RangeError) { @one.legendre(@two) }
    assert_raise(RangeError) { @one.legendre(@neg_one) }
    assert_raise(RangeError) { @one.legendre(@four) }
    assert_raise(RangeError) { @one.legendre(@nine) }
  end
  
  def test_remove
    assert_equal(@nine, @forty_five.remove(5)[0],      "GMP::Z should remove(GMP::Z) correctly.")
    assert_equal(@nine, @forty_five.remove(@five)[0],  "GMP::Z should remove(GMP::Z) correctly.")
    assert_equal(@five, @forty_five.remove(@three)[0], "GMP::Z should remove(GMP::Z) correctly.")
    assert_equal(1, @forty_five.remove(5)[1],      "GMP::Z should remove(GMP::Z) correctly.")
    assert_equal(1, @forty_five.remove(@five)[1],  "GMP::Z should remove(GMP::Z) correctly.")
    assert_equal(2, @forty_five.remove(@three)[1], "GMP::Z should remove(GMP::Z) correctly.")
  end
end
