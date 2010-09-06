require './test_helper'

class TC_Z_Logic < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(0xffff00000000ffff)
    @b=GMP::Z.new(0xffff0000ffff0000)
    @c=0xff00ff00ff00ff00
    @d=0x1248 
  end

  def test_and
    assert_equal(@a & @b, 0xffff00000000ffff & 0xffff0000ffff0000, "GMP::Z should AND correctly")
    assert_equal(@a & @d, 0xffff00000000ffff & 0x1248,             "GMP::Z should AND correctly")
    assert_equal(@a & @c, 0xffff00000000ffff & 0xff00ff00ff00ff00, "GMP::Z should AND correctly")
    assert_equal(@d & @a, 0x1248             & 0xffff00000000ffff, "GMP::Z should AND correctly")
    assert_equal(@c & @a, 0xff00ff00ff00ff00 & 0xffff00000000ffff, "GMP::Z should AND correctly")
  end

  def test_or
    assert_equal(@a | @b, 0xffff00000000ffff | 0xffff0000ffff0000, "GMP::Z should OR correctly")
    assert_equal(@a | @d, 0xffff00000000ffff | 0x1248,             "GMP::Z should OR correctly")
    assert_equal(@a | @c, 0xffff00000000ffff | 0xff00ff00ff00ff00, "GMP::Z should OR correctly")
    assert_equal(@d | @a, 0x1248             | 0xffff00000000ffff, "GMP::Z should OR correctly")
    assert_equal(@c | @a, 0xff00ff00ff00ff00 | 0xffff00000000ffff, "GMP::Z should OR correctly")
  end

  def test_xor
    assert_equal(@a ^ @b, 0xffff00000000ffff ^ 0xffff0000ffff0000, "GMP::Z should XOR correctly")
    assert_equal(@a ^ @d, 0xffff00000000ffff ^ 0x1248,             "GMP::Z should XOR correctly")
    assert_equal(@a ^ @c, 0xffff00000000ffff ^ 0xff00ff00ff00ff00, "GMP::Z should XOR correctly")
    assert_equal(@d ^ @a, 0x1248             ^ 0xffff00000000ffff, "GMP::Z should XOR correctly")
    assert_equal(@c ^ @a, 0xff00ff00ff00ff00 ^ 0xffff00000000ffff, "GMP::Z should XOR correctly")
  end

  def test_com
    assert_equal(GMP::Z().com, -1, "GMP::Z.com should complement")
    assert_equal(GMP::Z(1).com, -2, "GMP::Z.com should complement")
    assert_equal(GMP::Z(-1).com, 0, "GMP::Z.com should complement")
    assert_equal(@a.com, -0xffff00000000ffff-1, "GMP::Z.com should complement")
  end

  def test_logic
    a=GMP::Z.new(0x1234)
    b=GMP::Z.new(a)
    b[0]=true

    assert_equal(a,     0x1234, "GMP::Z.new() should create a new copy, not alias")
    assert_equal(b,     0x1235, "GMP::Z#[]= should modify bits")
    assert_equal(a[2],    true, "GMP::Z#[] should access bits")
    assert_equal(a.scan0(0), 0, "GMP::Z#scan0 should scan for 0s")
    assert_equal(a.scan1(0), 2, "GMP::Z#scan1 should scan for 1s")

  end
end
