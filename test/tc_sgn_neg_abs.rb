require './test_helper'

class TC_sgn_neg_abs < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(10)
    @b=GMP::Z.new()
    @c=GMP::Z.new(-10)
    @d=GMP::Q.new(10)
    @e=GMP::Q.new()
    @f=GMP::Q.new(-10)
    @k=GMP::Z.new(10)
    @l=GMP::Z.new()
    @m=GMP::Z.new(-10)
    @n=GMP::Q.new(10)
    @o=GMP::Q.new()
    @p=GMP::Q.new(-10)
  end
  
  def test_sgn
    assert_equal([1, 0, 1, 0], [@a.sgn, @b.sgn, @d.sgn, @e.sgn], "GMP::Z, GMP::Q should calculate sgn correctly")
    assert_equal([-1, -1],     [@c.sgn, @f.sgn],                 "GMP::Z, GMP::Q should calculate sgn correctly")
  end
  
  def test_neg
    assert_equal(-@a, @c,                                        "-(x : GMP::Z) should be calculated correctly.")
    assert_equal(-@c, @a,                                        "-(x : GMP::Z) should be calculated correctly.")
    assert_equal(-@b, @b,                                        "-GMP::Z.new() should equal GMP::Z.new().")
    assert_equal(-@d, @f,                                        "-(x : GMP::Q) should be calculated correctly.")
    assert_equal(-@f, @d,                                        "-(x : GMP::Q) should be calculated correctly.")
    assert_equal(-@e, @e,                                        "-GMP::Q.new() should equal GMP::Q.new().")
    @k.neg!; @l.neg!; @m.neg!; @n.neg!; @o.neg!; @p.neg!
    assert_equal(@k, @c,                                         "(x : GMP::Z).neg! should be calculated correctly.")
    assert_equal(@l, @b,                                         "(x : GMP::Z).neg! should be calculated correctly.")
    assert_equal(@m, @a,                                         "GMP::Z.new().neg! should equal GMP::Z.new().")
    assert_equal(@n, @f,                                         "(x : GMP::Q).neg! should be calculated correctly.")
    assert_equal(@o, @e,                                         "(x : GMP::Q).neg! should be calculated correctly.")
    assert_equal(@p, @d,                                         "GMP::Q.new().neg! should equal GMP::Q.new().")
  end
  
  def test_abs
    assert_equal([10, 0, 10], [@a.abs, @b.abs, @c.abs],          "(x : GMP::Z).abs should be calculated correctly.")
    assert_equal([10, 0, 10], [@d.abs, @e.abs, @f.abs],          "(x : GMP::Q).abs should be calculated correctly.")
    @a.abs!; @b.abs!; @c.abs!; @d.abs!; @e.abs!; @f.abs!
    assert_equal([10, 0, 10], [@a, @b, @c],                      "(x : GMP::Z).abs! should be calculated correctly.")
    assert_equal([10, 0, 10], [@d, @e, @f],                      "(x : GMP::Q).abs! should be calculated correctly.")
  end
end
