require './test_helper'

class TC_Z_Exponentiation < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)
    @b = GMP::Z.new(16)
    @c = @a**5
  end
  
  def test_exponentiation
    assert_equal(@a**5, GMP::Z(10000000000),                          "GMP::Z should **(Fixnum) correctly")
    assert_equal(GMP::Z("100000000000000000000000000000000"), @a**@b, "GMP::Z should **(GMP::Z) correctly")
    assert_equal(GMP::Z(65536), 2**@b,                                "Fixnum should **(GMP::Z) correctly")
  end
  
  def test_powmod
    assert_equal(GMP::Z(16), @a.powmod(2,256),  "(a : GMP::Z).powmod((b : Fixnum), (c : Fixnum)) should work correctly")
    assert_equal(GMP::Z(76), @b.powmod(10,@a),  "(a : GMP::Z).powmod((b : Fixnum), (c : GMP::Z)) should work correctly")
    assert_equal(GMP::Z(0), @a.powmod(@b,256),  "(a : GMP::Z).powmod((b : GMP::Z), (c : Fixnum)) should work correctly")
    assert_equal(GMP::Z(0), @a.powmod(@b,@c), "(a : GMP::Z).powmod((b : GMP::Z), (c : GMP::Z)) should work correctly")
  end
end
