require './test_helper'

class TC_precision < Test::Unit::TestCase
  def setup
    begin
      GMP::MPFR_VERSION
      @initial_default_prec = GMP::F.default_prec
    rescue
      @initial_default_prec = GMP::F.default_prec
    end
  end
  
  def test_initial_default_precision
    begin
      GMP::MPFR_VERSION
      assert_equal(53, GMP::F.default_prec, "The initial default precision with MPFR should be 53.")
    rescue
      assert_equal(64, GMP::F.default_prec, "The initial default precision without MPFR should be 64.")
    end
  end
  
  def test_initial_default_precision2
    @pi = GMP::F.new(3.14)
    @seven_halves = GMP::F.new(GMP::Q.new(7,2), 1000)
    assert_in_delta(0,                      GMP::F.new(),                1e-12)
    assert_equal(@initial_default_prec, GMP::F.new().prec)
    assert_in_delta(3.14000000000000012434, @pi,                         1e-12)
    assert_equal(@initial_default_prec, @pi.prec)
    assert_in_delta(1,                      GMP::F.new(1),               1e-12)
    assert_equal(@initial_default_prec, GMP::F.new(1).prec)
    assert_in_delta(3.14,                   GMP::F.new("3.14"),          1e-12)
    assert_equal(@initial_default_prec, GMP::F.new("3.14").prec)
    assert_in_delta(4294967296,             GMP::F.new(2**32),           1e-12)
    assert_equal(@initial_default_prec, GMP::F.new(2**32).prec)
    assert_in_delta(3,                      GMP::F.new(GMP::Z.new(3)),   1e-12)
    assert_equal(@initial_default_prec, GMP::F.new(GMP::Z.new(3)).prec)
    assert_in_delta(3.5,                    GMP::F.new(GMP::Q.new(7,2)), 1e-12)
    assert_equal(@initial_default_prec, GMP::F.new(GMP::Q.new(7,2)).prec)
    assert_in_delta(3.14000000000000012434, GMP::F.new(@pi),             1e-12)
    assert_equal(@initial_default_prec,        GMP::F.new(@pi).prec)
  end
  
  def test_default_precision
    GMP::F.default_prec = 128
    @pi = GMP::F.new(3.14)
    @seven_halves = GMP::F.new(GMP::Q.new(7,2), 1000)
    assert_in_delta(0,                      GMP::F.new(),                1e-12)
    assert_equal(128, GMP::F.new().prec)
    assert_in_delta(3.14000000000000012434, @pi,                         1e-12)
    assert_equal(128, @pi.prec)
    assert_in_delta(1,                      GMP::F.new(1),               1e-12)
    assert_equal(128, GMP::F.new(1).prec)
    assert_in_delta(3.14,                   GMP::F.new("3.14"),          1e-12)
    assert_equal(128, GMP::F.new("3.14").prec)
    assert_in_delta(4294967296,             GMP::F.new(2**32),           1e-12)
    assert_equal(128, GMP::F.new(2**32).prec)
    assert_in_delta(3,                      GMP::F.new(GMP::Z.new(3)),   1e-12)
    assert_equal(128, GMP::F.new(GMP::Z.new(3)).prec)
    assert_in_delta(3.5,                    GMP::F.new(GMP::Q.new(7,2)), 1e-12)
    assert_equal(128, GMP::F.new(GMP::Q.new(7,2)).prec)
    assert_in_delta(3.14000000000000012434, GMP::F.new(@pi),             1e-12)
    assert_equal(128, GMP::F.new(@pi).prec)
    GMP::F.default_prec =  @initial_default_prec
  end
  
  def test_specific_precision
    @pi = GMP::F.new(3.14)
    @seven_halves = GMP::F.new(GMP::Q.new(7,2), 1024)
    assert_equal(0, GMP::F.new(3.14, 1024).to_s =~ /0\.31400000000000001243449787580175325274467468261718750*e\+1/)
    assert_equal(1024, GMP::F.new(3.14, 1024).prec)
    assert_in_delta(1, GMP::F.new(1, 1024), 1e-12)
    assert_equal(1024, GMP::F.new(1, 1024).prec)
    assert_in_delta(3.14, GMP::F.new("3.14", 1024), 1e-12)
    assert_equal(1024, GMP::F.new("3.14", 1024).prec)
    assert_in_delta(4294967296, GMP::F.new(2**32, 1024), 1e-12)
    assert_equal(1024, GMP::F.new(2**32, 1024).prec)
    assert_in_delta(3, GMP::F.new(GMP::Z.new(3), 1024), 1e-12)
    assert_equal(1024, GMP::F.new(GMP::Z.new(3), 1024).prec)
    assert_in_delta(3.5, @seven_halves, 1e-12)
    assert_equal(1024, @seven_halves.prec)
    assert_in_delta(3.5, GMP::F.new(@seven_halves), 1e-12)
    assert_equal(1024, GMP::F.new(@seven_halves).prec)
    assert_equal(0, GMP::F.new(@pi, 1024).to_s =~ /0\.31400000000000001243449787580175325274467468261718750*e\+1/)
    assert_equal(1024, GMP::F.new(@pi, 1024).prec)
    assert_in_delta(3.5, GMP::F.new(@seven_halves, 0), 1e-12)
    assert_equal(@initial_default_prec, GMP::F.new(@seven_halves, 0).prec)
  end
  
  def test_set_default_prec
    begin
      GMP::MPFR_VERSION
      GMP::F.default_prec = 100
      assert_equal(100, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
      GMP::F.default_prec = 130
      assert_equal(130, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
      GMP::F.default_prec = 1000
      assert_equal(1000, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
      assert_raise(RangeError) { GMP::F.default_prec = -64 }
      assert_raise(TypeError) { GMP::F.default_prec = "Cow" }
    rescue NameError => err
      raise unless err.to_s =~ /uninitialized constant:? GMP::MPFR_VERSION/
      GMP::F.default_prec = 100
      assert_equal(128, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
      GMP::F.default_prec = 130
      adjusted = (130*1.0/GMP::GMP_BITS_PER_LIMB).ceil*GMP::GMP_BITS_PER_LIMB
      assert_equal(adjusted, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
      GMP::F.default_prec = 1000
      assert_equal(1024, GMP::F.default_prec, "GMP::F.default_prec should be assignable.")
      assert_raise(RangeError) { GMP::F.default_prec = -64 }
      assert_raise(TypeError) { GMP::F.default_prec = "Cow" }
    end
    GMP::F.default_prec =  @initial_default_prec
  end
end
