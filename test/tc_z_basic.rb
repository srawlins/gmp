require './test_helper'

# Tested: [Z op Z, Z op FixNum, Z op BigNum, FixNum op Z, BigNum op Z]
# Things are tested both ways because the implementation is asymetrical
class TC_Z_Basic < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(100)
    @b=GMP::Z.new(200)
    @c=2**32
  end

  def test_add
    assert_equal(100   + 200,   @a + @b, "GMP::Z should add correctly")
    assert_equal(100   + 2,     @a + 2,  "GMP::Z should add correctly")
    assert_equal(100   + 2**32, @a + @c, "GMP::Z should add correctly")
    assert_equal(  2   + 100,   2 + @a,  "GMP::Z should add correctly")
    assert_equal(2**32 + 100,   @c + @a, "GMP::Z should add correctly")
  end

  def test_sub
    assert_equal(@a - @b, 100   - 200,   "GMP::Z should subtract correctly")
    assert_equal(@a - 2,  100   - 2,     "GMP::Z should subtract correctly")
    assert_equal(@a - @c, 100   - 2**32, "GMP::Z should subtract correctly")
    assert_equal( 2 - @a, 2     - 100,   "GMP::Z should subtract correctly")
    assert_equal(@c - @a, 2**32 - 100,   "GMP::Z should subtract correctly")
  end

  def test_mul
    assert_equal(@a * @b, 100   * 200,   "GMP::Z should multiply correctly")
    assert_equal(@a * 2,  100   * 2,     "GMP::Z should multiply correctly")
    assert_equal(@a * @c, 100   * 2**32, "GMP::Z should multiply correctly")
    assert_equal( 2 * @a, 2     * 100,   "GMP::Z should multiply correctly")
    assert_equal(@c * @a, 2**32 * 100,   "GMP::Z should multiply correctly")
  end
end
