require './test_helper'

class TC_MPFR_Functions < Test::Unit::TestCase
  def setup
    @a = GMP::F(1)
    @b = GMP::F(2)
  end
  
  def test_const_existence
    constants = ["const_log2", "const_pi", "const_euler", "const_catalan"]

    constants.each do |c|
      assert_nothing_raised("GMP::F##{c} can be called.") {
        GMP::F.send(c)
      }
      assert_nothing_raised("GMP::F##{c} can be called w/ 1 arg: rounding_mode.") {
        GMP::F.send(c, GMP::GMP_RNDN)
      }
      assert_nothing_raised("GMP::F##{c} can be called w/ 1 arg: rounding_mode.") {
        GMP::F.send(c, GMP::GMP_RNDZ)
      }
      assert_nothing_raised("GMP::F##{c} can be called w/ 2 args: rounding_mode, prec.") {
        GMP::F.send(c, GMP::GMP_RNDN, 32)
      }
      assert_nothing_raised("GMP::F##{c} can be called w/ 2 args: rounding_mode, prec.") {
        GMP::F.send(c, GMP::GMP_RNDN, 128)
      }
      assert_nothing_raised("GMP::F##{c} can be called w/ 2 args: rounding_mode, prec.") {
        GMP::F.send(c, GMP::GMP_RNDN, 200)
      }
    end
  end
  
  def test_function_existence
    assert_nothing_raised("GMP::F.nan? should be callable.") { @a.nan? }
    assert_nothing_raised("GMP::F.infinite? should be callable.") { @a.infinite? }
    assert_nothing_raised("GMP::F.finite? should be callable.") { @a.finite? }
    assert_nothing_raised("GMP::F.number? should be callable.") { @a.number? }
    if GMP::MPFR_VERSION >= "3.0.0"
    assert_nothing_raised("GMP::F.regular? should be callable.") { @a.regular? }
    end

    assert_nothing_raised("GMP::F.sqrt should be callable.") { @a.sqrt }
    assert_nothing_raised("GMP::F.rec_sqrt should be callable.") { @a.rec_sqrt }
    assert_nothing_raised("GMP::F.cbrt should be callable.") { @a.cbrt }
    
    assert_nothing_raised("GMP::F.log should be callable.") { @a.log }
    assert_nothing_raised("GMP::F.log2 should be callable.") { @a.log2 }
    assert_nothing_raised("GMP::F.log10 should be callable.") { @a.log10 }
    assert_nothing_raised("GMP::F.exp should be callable.") { @a.exp }
    assert_nothing_raised("GMP::F.exp2 should be callable.") { @a.exp2 }
    assert_nothing_raised("GMP::F.exp10 should be callable.") { @a.exp10 }
    assert_nothing_raised("GMP::F.cos should be callable.") { @a.cos }
    assert_nothing_raised("GMP::F.sin should be callable.") { @a.sin }
    assert_nothing_raised("GMP::F.tan should be callable.") { @a.tan }
    assert_nothing_raised("GMP::F.sin_cos should be callable.") { @a.sin_cos }
    assert_nothing_raised("GMP::F.sec should be callable.") { @a.sec }
    assert_nothing_raised("GMP::F.csc should be callable.") { @a.csc }
    assert_nothing_raised("GMP::F.cot should be callable.") { @a.cot }
    assert_nothing_raised("GMP::F.acos should be callable.") { @a.acos }
    assert_nothing_raised("GMP::F.asin should be callable.") { @a.asin }
    assert_nothing_raised("GMP::F.atan should be callable.") { @a.atan }
    assert_nothing_raised("GMP::F.atan2 should be callable.") { @a.atan2(@b) }
    
    assert_nothing_raised("GMP::F.cosh should be callable.") { @a.cosh }
    assert_nothing_raised("GMP::F.sinh should be callable.") { @a.sinh }
    assert_nothing_raised("GMP::F.tanh should be callable.") { @a.tanh }
    assert_nothing_raised("GMP::F.sinh_cosh should be callable.") { @a.sinh_cosh }
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
    if GMP::MPFR_VERSION >= "3.0.0"
    assert_nothing_raised("GMP::F.digamma should be callable.") { @a.digamma }
    end
    assert_nothing_raised("GMP::F.zeta should be callable.") { @a.zeta }
    assert_nothing_raised("GMP::F.erf should be callable.") { @a.erf }
    assert_nothing_raised("GMP::F.erfc should be callable.") { @a.erfc }
    assert_nothing_raised("GMP::F.j0 should be callable.") { @a.j0 }
    assert_nothing_raised("GMP::F.j1 should be callable.") { @a.j1 }
    assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(-1) }
    assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(0) }
    assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(1) }
    assert_nothing_raised("GMP::F.jn should be callable.") { @a.jn(2) }
    assert_nothing_raised("GMP::F.y0 should be callable.") { @a.y0 }
    assert_nothing_raised("GMP::F.y1 should be callable.") { @a.y1 }
    assert_nothing_raised("GMP::F.yn should be callable.") { @a.yn(-1) }
    assert_nothing_raised("GMP::F.yn should be callable.") { @a.yn(0) }
    assert_nothing_raised("GMP::F.yn should be callable.") { @a.yn(1) }
    assert_nothing_raised("GMP::F.yn should be callable.") { @a.yn(2) }
    
    assert_nothing_raised("GMP::F.agm should be callable.") { @a.agm(@b) }
    assert_nothing_raised("GMP::F.hypot should be callable.") { @a.hypot(@b) }
  end
  
  def test_function_parameters
    single_functions = [:sqrt, :rec_sqrt, :cbrt,
                        :log, :log2, :log10, :exp, :exp2, :exp10,
                        :cos, :sin, :tan, :sin_cos, :sec, :csc, :cot,
                        :acos, :asin, :atan,
                        :cosh, :sinh, :tanh, :sech, :csch, :coth,
                        :acosh, :asinh, :atanh,
                        :log1p, :expm1, :eint, :li2, :gamma, :lngamma,
                        :zeta, :erf, :erfc,
                        :j0, :j1, :y0, :y1
                       ]
    single_functions += [:digamma] if GMP::MPFR_VERSION >= "3.0.0"
    double_functions = [:sin_cos, :sinh_cosh]
    single_mpf_functions = [:atan2, :agm, :hypot]
    
    (single_functions + double_functions).each do |f|
      assert_nothing_raised("GMP::F.#{f} can be called w/ 1 arg: rounding_mode.") { @a.send(f, GMP::GMP_RNDN) }
      assert_nothing_raised("GMP::F.#{f} can be called w/ 1 arg: rounding_mode.") { @a.send(f, GMP::GMP_RNDZ) }
      assert_nothing_raised("GMP::F.#{f} can be called w/ 2 args: rounding_mode, prec.") { @a.send(f, GMP::GMP_RNDN, 32) }
      assert_nothing_raised("GMP::F.#{f} can be called w/ 2 args: rounding_mode, prec.") { @a.send(f, GMP::GMP_RNDN, 128) }
      assert_nothing_raised("GMP::F.#{f} can be called w/ 2 args: rounding_mode, prec.") { @a.send(f, GMP::GMP_RNDN, 200) }
    end
    double_functions.each do |f|
      assert_nothing_raised("GMP::F.#{f} can be called w/ 3 args: rounding_mode, prec, prec.") { @a.send(f, GMP::GMP_RNDN,  64,  64) }
      assert_nothing_raised("GMP::F.#{f} can be called w/ 3 args: rounding_mode, prec, prec.") { @a.send(f, GMP::GMP_RNDN, 128,  64) }
      assert_nothing_raised("GMP::F.#{f} can be called w/ 3 args: rounding_mode, prec, prec.") { @a.send(f, GMP::GMP_RNDN,  64, 128) }
    end
  end
end
