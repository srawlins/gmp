require 'test_helper'

class TC_MPFR_Functions < Test::Unit::TestCase
  def setup
    @a = GMP::F(1)
  end
  
  def test_const_existence
    assert_nothing_raised("GMP::F#const_log2 should be callable.") { GMP::F.const_log2 }
    assert_nothing_raised("GMP::F#const_pi should be callable.") { GMP::F.const_pi }
    assert_nothing_raised("GMP::F#const_euler should be callable.") { GMP::F.const_euler }
    assert_nothing_raised("GMP::F#const_catalan should be callable.") { GMP::F.const_catalan }
  end
  
  def test_function_existence
    assert_nothing_raised("GMP::F.sqrt should be callable.") { @a.sqrt }
    
    assert_nothing_raised("GMP::F.log should be callable.") { @a.log }
    assert_nothing_raised("GMP::F.log2 should be callable.") { @a.log2 }
    assert_nothing_raised("GMP::F.log10 should be callable.") { @a.log10 }
    assert_nothing_raised("GMP::F.exp should be callable.") { @a.exp }
    assert_nothing_raised("GMP::F.exp2 should be callable.") { @a.exp2 }
    assert_nothing_raised("GMP::F.exp10 should be callable.") { @a.exp10 }
    assert_nothing_raised("GMP::F.cos should be callable.") { @a.cos }
    assert_nothing_raised("GMP::F.sin should be callable.") { @a.sin }
    assert_nothing_raised("GMP::F.tan should be callable.") { @a.tan }
    assert_nothing_raised("GMP::F.sec should be callable.") { @a.sec }
    assert_nothing_raised("GMP::F.csc should be callable.") { @a.csc }
    assert_nothing_raised("GMP::F.cot should be callable.") { @a.cot }
    
    assert_nothing_raised("GMP::F.acos should be callable.") { @a.acos }
    assert_nothing_raised("GMP::F.asin should be callable.") { @a.asin }
    assert_nothing_raised("GMP::F.atan should be callable.") { @a.atan }
    
    assert_nothing_raised("GMP::F.cosh should be callable.") { @a.cosh }
    assert_nothing_raised("GMP::F.sinh should be callable.") { @a.sinh }
    assert_nothing_raised("GMP::F.tanh should be callable.") { @a.tanh }

    assert_nothing_raised("GMP::F.sech should be callable.") { @a.sech }
    assert_nothing_raised("GMP::F.csch should be callable.") { @a.csch }
    assert_nothing_raised("GMP::F.coth should be callable.") { @a.coth }
    assert_nothing_raised("GMP::F.acosh should be callable.") { @a.acosh }
    assert_nothing_raised("GMP::F.asinh should be callable.") { @a.asinh }
    assert_nothing_raised("GMP::F.atanh should be callable.") { @a.atanh }
    
    assert_nothing_raised("GMP::F.log1p should be callable.") { @a.log1p }
    assert_nothing_raised("GMP::F.expm1 should be callable.") { @a.expm1 }
    assert_nothing_raised("GMP::F.eint should be callable.") { @a.eint }
    assert_nothing_raised("GMP::F.li2 should be callable.") { @a.li2 }
    assert_nothing_raised("GMP::F.gamma should be callable.") { @a.gamma }
    assert_nothing_raised("GMP::F.lngamma should be callable.") { @a.lngamma }
    #assert_nothing_raised("GMP::F.lgamma should be callable.") { @a.lgamma }
    assert_nothing_raised("GMP::F.zeta should be callable.") { @a.zeta }
    assert_nothing_raised("GMP::F.erf should be callable.") { @a.erf }
    assert_nothing_raised("GMP::F.erfc should be callable.") { @a.erfc }
    assert_nothing_raised("GMP::F.j0 should be callable.") { @a.j0 }
    assert_nothing_raised("GMP::F.j1 should be callable.") { @a.j1 }
    #assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(-1) }
    #assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(0) }
    #assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(1) }
    #assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(2) }  #WTF!!!
    assert_nothing_raised("GMP::F.y0 should be callable.") { @a.y0 }
    assert_nothing_raised("GMP::F.y1 should be callable.") { @a.y1 }
  end
end