require './test_helper'

class TC_Cmp < Test::Unit::TestCase
  def setup
    @a=GMP::Z.new(180)
    @c=GMP::Q.new(2000,11) # ~181.82
    @d=GMP::Q.new(3000,17) # ~176.47
    @e=700
    @f=2**32
    @g=GMP::Q(360,2)
  end

  def test_cmp_q_q
    assert_equal(@c >  @d, true,  "GMP::Q should compare correctly")
    assert_equal(@c >= @d, true,  "GMP::Q should compare correctly")
    assert_equal(@c <  @d, false, "GMP::Q should compare correctly")
    assert_equal(@c <= @d, false, "GMP::Q should compare correctly")
    assert_equal(@c == @d, false, "GMP::Q should == correctly")
    assert_equal(@c == @c, true,  "GMP::Q should == correctly")
    assert_equal(@c != @d, true,  "GMP::Q should != correctly")
    assert_equal(@c != @c, false, "GMP::Q should != correctly")

    assert_equal(@c <=> @d,  1, "GMP::Q should <=> correctly")
    assert_equal(@c <=> @c,  0, "GMP::Q should <=> correctly")
    assert_equal(@d <=> @c, -1, "GMP::Q should <=> correctly")
  end

  def test_cmp_q_z
    assert_equal(@c >  @a, true,  "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c >= @a, true,  "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c <  @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c <= @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@c == @a, false, "GMP::Q should == correctly with GMP::Z")
    assert_equal(@c != @a, true,  "GMP::Q should != correctly with GMP::Z")

    assert_equal(@a <  @c, true,  "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a <= @c, true,  "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >  @c, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >= @c, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a == @c, false, "GMP::Z should == correctly with GMP::Q")
    assert_equal(@a != @c, true,  "GMP::Z should != correctly with GMP::Q")

    assert_equal(@g == @a, true,  "GMP::Q should == correctly with GMP::Z")
    assert_equal(@g != @a, false, "GMP::Q should != correctly with GMP::Z")

    assert_equal(@d >  @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@d >= @a, false, "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@d <  @a, true,  "GMP::Q should compare correctly with GMP::Z")
    assert_equal(@d <= @a, true,  "GMP::Q should compare correctly with GMP::Z")

    assert_equal(@a <  @d, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a <= @d, false, "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >  @d, true,  "GMP::Z should compare correctly with GMP::Q")
    assert_equal(@a >= @d, true,  "GMP::Z should compare correctly with GMP::Q")

    assert_equal(@c <=> @a,  1, "GMP::Q should <=> correctly with GMP::Z")
    assert_equal(@g <=> @a,  0, "GMP::Q should <=> correctly with GMP::Z")
    assert_equal(@a <=> @c, -1, "GMP::Q should <=> correctly with GMP::Z")
  end

  def test_cmp_z_int
    assert_equal(@a <=> @e, -1, "GMP::Z should <=> correctly with Fixnum")
    assert_equal(@e <=> @a,  1, "Fixnum should <=> correctly with GMP::Z")
    assert_equal(-1, @a <=> @f, "GMP::Z should <=> correctly with Bignum")
    assert_equal(@f <=> @a,  1, "Bignum should <=> correctly with GMP::Z")
  end

  def test_cmp_q_int
    assert_equal(@c <=> @e, -1, "GMP::Q should <=> correctly with Fixnum")
    assert_equal(@e <=> @c,  1, "Fixnum should <=> correctly with GMP::Q")
    assert_equal(@c <=> @f, -1, "GMP::Q should <=> correctly with Bignum")
    assert_equal(1, @f <=> @c, "Bignum should <=> correctly with GMP::Q")
  end
end
