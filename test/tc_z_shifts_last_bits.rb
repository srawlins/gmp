require './test_helper'

class TC_shifts_last_bits < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)  # 01100100
    @b =- @a              # 10011100
  end
  
  def test_shifts
    assert_equal(   3, @a >> 5, "GMP::Z should >> correctly.")  # 00000011
    assert_equal(-  4, @b >> 5, "GMP::Z should >> correctly.")  # 11111100
    assert_equal( 400, @a << 2, "GMP::Z should << correctly.")  # 0110010000
    assert_equal(-400, @b << 2, "GMP::Z should << correctly.")  # 1110010000
  end
  
  def test_last_bits
    #assert_equal( 5, @a.lastbits_pos(5), "GMP::Z should lastbits_pos correctly.")  #   100
    #assert_equal(28, @b.lastbits_pos(5), "GMP::Z should lastbits_pos correctly.")  # 11100
    #assert_equal(-4, @b.lastbits_sgn(5), "GMP::Z should lastbits_sgn correctly.")
    # a.tshr 5 ???
  end
end
