require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TC_f_abs_neg < Test::Unit::TestCase
  def setup
    @a=GMP::F.new(3.14)
    @b=GMP::F.new()
    @c=GMP::F.new(-3.14)
    @d=GMP::F.new(3.14)
    @e=GMP::F.new()
    @f=GMP::F.new(-3.14)
  end

  def test_neg
    assert_equal(-@a, @c,                                        "-(x : GMP::Z) should be calculated correctly.")
    assert_equal(-@c, @a,                                        "-(x : GMP::Z) should be calculated correctly.")
    assert_equal(-@b, @b,                                        "-GMP::Z.new() should equal GMP::Z.new().")
    @d.neg!; @e.neg!; @f.neg!
    assert_equal(@d, @c,                                         "(x : GMP::Z).neg! should be calculated correctly.")
    assert_equal(@e, @b,                                         "(x : GMP::Z).neg! should be calculated correctly.")
    assert_equal(@f, @a,                                         "GMP::Z.new().neg! should equal GMP::Z.new().")
  end

  def test_abs
    assert_equal([@a, 0, @a], [@a.abs, @b.abs, @c.abs],          "(x : GMP::Z).abs should be calculated correctly.")
    @a.abs!; @b.abs!; @c.abs!
    assert_equal([@a, 0, @a], [@a, @b, @c],                      "(x : GMP::Z).abs! should be calculated correctly.")
  end
end
