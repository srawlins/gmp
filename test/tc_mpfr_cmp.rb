require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcMpfrCmp < MiniTest::Test
  def setup
    @neg_inf = GMP::F(0).log  # -Inf
    @inf = -@neg_inf          # Inf
    @nan = @neg_inf + @inf    # NaN
  end

  def test_mpfr_lessgreater
    assert_true(GMP::F(5).lessgreater?(GMP::F(12)), "lessgreater? should work")
    assert_true(GMP::F(5).lessgreater?(@inf), "lessgreater? should work")
    assert_false(GMP::F(5).lessgreater?(@nan), "lessgreater? should work")
    assert_false(@nan.lessgreater?(@nan), "lessgreater? should work")
  end

  def test_mpfr_lessgreater_types
    assert_raise(TypeError) { GMP::F(5).lessgreater? 3.14 }
  end

  def test_mpfr_unordered
    assert_true(@nan.unordered?(GMP::F(5)), "unordered? should work")
    assert_false(@neg_inf.unordered?(GMP::F(5)), "unordered? should work")
  end

  def test_mpfr_unordered_types
    assert_raise(TypeError) { GMP::F(5).unordered? 3.14 }
  end
end
