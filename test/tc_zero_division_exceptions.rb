require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TCZeroDivisionExceptions < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(10)
    @b = GMP::Z.new()
    @c = GMP::Q.new(1)
    @d = GMP::Q.new()
  end

  def test_division_by_zero
    assert_raise(ZeroDivisionError) { @a.tdiv(0) }
    assert_raise(ZeroDivisionError) { @a.tdiv(@b) }
    assert_raise(ZeroDivisionError) { @d.inv }
    assert_raise(ZeroDivisionError) { @d.inv! }
    assert_raise(ZeroDivisionError) { @c/0 }
    assert_raise(ZeroDivisionError) { @c/@d }
  end
end
