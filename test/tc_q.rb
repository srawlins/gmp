require 'test_helper'

class TC_Q < Test::Unit::TestCase
  def test_init_null
    assert_equal(GMP::Q.new(), 0, "GMP::Q.new() should initialize to 0")
  end

  def test_init_fixnum
    assert_equal(GMP::Q.new(1), 1, "GMP::Q.new(x : Fixnum) should initialize to 0")
  end

  def test_init_fixnum2
    assert_equal(GMP::Q.new(1, 2).to_s, "1/2", "GMP::Q.new(num : Fixnum, den : Fixnum) should initialize to num/den")
    assert_equal(GMP::Q.new(1, 3).to_s, "1/3", "GMP::Q.new(num : Fixnum, den : Fixnum) should initialize to num/den")
    assert_equal(GMP::Q.new(2, 4).to_s, "1/2", "GMP::Q.new(num : Fixnum, den : Fixnum) should initialize to num/den")
  end

  def test_init_string
    assert_equal(GMP::Q.new("1/2").to_s, "1/2", "GMP::Q.new(x : String) should initialize to x")
    assert_equal(GMP::Q.new("1/3").to_s, "1/3", "GMP::Q.new(x : String) should initialize to x")
  end

  def test_init_q
    a = GMP::Q.new(1,2)
    assert_equal(GMP::Q.new(a), a, "GMP::Q.new(x : Q) should initialize to x")
  end
end