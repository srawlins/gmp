require 'test_helper'

class TC_precision < Test::Unit::TestCase
  def setup
    @pi = GMP::F.new(3.14)
    @seven_halves = GMP::F.new(GMP::Q.new(7,2), 1000)
  end
  
  def test_default_precision
    assert_equal("0.e+0", GMP::F.new().to_s)
    assert_equal(64,      GMP::F.new().prec)
    assert_equal("0.314000000000000012434e+1", @pi.to_s)
    assert_equal(64,                           @pi.prec)
    assert_equal("0.1e+1", GMP::F.new(1).to_s)
    assert_equal(64,       GMP::F.new(1).prec)
    assert_equal("0.314e+1", GMP::F.new("3.14").to_s)
    assert_equal(64,         GMP::F.new("3.14").prec)
    assert_equal("0.4294967296e+10", GMP::F.new(2**32).to_s)
    assert_equal(64,                 GMP::F.new(2**32).prec)
    assert_equal("0.3e+1", GMP::F.new(GMP::Z.new(3)).to_s)
    assert_equal(64,       GMP::F.new(GMP::Z.new(3)).prec)
    assert_equal("0.35e+1", GMP::F.new(GMP::Q.new(7,2)).to_s)
    assert_equal(64,        GMP::F.new(GMP::Q.new(7,2)).prec)
    assert_equal("0.314000000000000012434e+1", GMP::F.new(@pi).to_s)
    assert_equal(64,                           GMP::F.new(@pi).prec)
  end
  
  def test_specific_precision
    assert_equal("0.3140000000000000124344978758017532527446746826171875e+1", GMP::F.new(3.14, 1000).to_s)
    assert_equal(1024,                                                        GMP::F.new(3.14, 1000).prec)
    assert_equal("0.1e+1", GMP::F.new(1, 1000).to_s)
    assert_equal(1024,     GMP::F.new(1, 1000).prec)
    assert_equal("0.314e+1", GMP::F.new("3.14", 1000).to_s)
    assert_equal(1024,       GMP::F.new("3.14", 1000).prec)
    assert_equal("0.4294967296e+10", GMP::F.new(2**32, 1000).to_s)
    assert_equal(1024,               GMP::F.new(2**32, 1000).prec)
    assert_equal("0.3e+1", GMP::F.new(GMP::Z.new(3), 1000).to_s)
    assert_equal(1024,     GMP::F.new(GMP::Z.new(3), 1000).prec)
    assert_equal("0.35e+1", @seven_halves.to_s)
    assert_equal(1024,      @seven_halves.prec)
    assert_equal("0.35e+1", GMP::F.new(@seven_halves).to_s)
    assert_equal(1024,      GMP::F.new(@seven_halves).prec)
    assert_equal("0.3140000000000000124344978758017532527446746826171875e+1", GMP::F.new(@pi, 1000).to_s)
    assert_equal(1024,                                                        GMP::F.new(@pi, 1000).prec)
    assert_equal("0.35e+1", GMP::F.new(@seven_halves, 0).to_s)
    assert_equal(64,        GMP::F.new(@seven_halves, 0).prec)
  end
end