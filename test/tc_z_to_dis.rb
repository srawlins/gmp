require './test_helper'

class TC_to_i_to_d < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(100)
    @b = GMP::Z.pow(2,32)
    @c = GMP::Q.new(200,11)
    @d = GMP::Z.new(-123456789)
  end
  
  def test_to_i
    assert_equal(@a.to_i, 100, "GMP::Z should to_i correctly.")
    assert_equal(@a.to_i.class, Fixnum, "GMP::Z.to_i should be a Fixnum.")
    assert_equal(@b.to_i, 2**32, "GMP::Z (Bignum) should to_i correctly.")
  end
  
  def test_to_d
    assert_equal(@a.to_d, 100.0, "GMP::Z should to_d correctly.")
    assert_equal(@a.to_d.class, Float, "GMP::Z.to_d should be a Float.")
    #assert_equal(@b.to_d, 2**32*1.0, "GMP::Z should to_d correctly.")
    assert_equal(@b.to_d.class, Float, "GMP::Z.to_d should be a Float.")
    assert_equal(@c.to_d.class, Float, "GMP::Q.to_d should be a Float.")
  end
  
  def test_to_s
    assert_equal(                         "100", @a.to_s      , "GMP::Z should to_s correctly.")
    assert_equal(                  "4294967296", @b.to_s      , "GMP::Z should to_s correctly.")
    assert_equal("-111010110111100110100010101", @d.to_s(   2), "GMP::Z should to_s correctly.")
    assert_equal(          "-22121022020212200", @d.to_s(   3), "GMP::Z should to_s correctly.")
    assert_equal(             "-13112330310111", @d.to_s(   4), "GMP::Z should to_s correctly.")
    assert_equal(               "-223101104124", @d.to_s(   5), "GMP::Z should to_s correctly.")
    assert_equal(                "-20130035113", @d.to_s(   6), "GMP::Z should to_s correctly.")
    assert_equal(                 "-3026236221", @d.to_s(   7), "GMP::Z should to_s correctly.")
    assert_equal(                  "-726746425", @d.to_s(   8), "GMP::Z should to_s correctly.")
    assert_equal(                  "-277266780", @d.to_s(   9), "GMP::Z should to_s correctly.")
    assert_equal(                  "-123456789", @d.to_s(  10), "GMP::Z should to_s correctly.")
    assert_equal(                   "-63762a05", @d.to_s(  11), "GMP::Z should to_s correctly.")
    assert_equal(                   "-35418a99", @d.to_s(  12), "GMP::Z should to_s correctly.")
    assert_equal(                   "-1c767471", @d.to_s(  13), "GMP::Z should to_s correctly.")
    assert_equal(                   "-12579781", @d.to_s(  14), "GMP::Z should to_s correctly.")
    assert_equal(                    "-ac89bc9", @d.to_s(  15), "GMP::Z should to_s correctly.")
    assert_equal(                    "-75bcd15", @d.to_s(  16), "GMP::Z should to_s correctly.")
    assert_equal(                   "-63762A05", @d.to_s( -11), "GMP::Z should to_s correctly.")
    assert_equal(                   "-35418A99", @d.to_s( -12), "GMP::Z should to_s correctly.")
    assert_equal(                   "-1C767471", @d.to_s( -13), "GMP::Z should to_s correctly.")
    assert_equal(                   "-12579781", @d.to_s( -14), "GMP::Z should to_s correctly.")
    assert_equal(                    "-AC89BC9", @d.to_s( -15), "GMP::Z should to_s correctly.")
    assert_equal(                    "-75BCD15", @d.to_s( -16), "GMP::Z should to_s correctly.")
    assert_equal("-111010110111100110100010101", @d.to_s(:bin), "GMP::Z should to_s correctly.")
    assert_equal(                  "-726746425", @d.to_s(:oct), "GMP::Z should to_s correctly.")
    assert_equal(                  "-123456789", @d.to_s(:dec), "GMP::Z should to_s correctly.")
    assert_equal(                    "-75bcd15", @d.to_s(:hex), "GMP::Z should to_s correctly.")
    
    assert_equal("1",          GMP::Z.new(1).to_s,          "Z(1).to_s should equal '1'")
    assert_equal("-1",         GMP::Z.new(-1).to_s,         "Z(1).to_s should equal '-1'")
    assert_equal("1234567890", GMP::Z.new(1234567890).to_s, "GMP::Z should to_s correctly.")
    assert_equal("100000",     GMP::Z.new(32).to_s(   2),   "GMP::Z should to_s(   2) correctly.")
    assert_equal("200",        GMP::Z.new(32).to_s(   4),   "GMP::Z should to_s(   4) correctly.")
    assert_equal("40",         GMP::Z.new(32).to_s(   8),   "GMP::Z should to_s(   8) correctly.")
    assert_equal("32",         GMP::Z.new(32).to_s(  10),   "GMP::Z should to_s(  10) correctly.")
    assert_equal("20",         GMP::Z.new(32).to_s(  16),   "GMP::Z should to_s(  16) correctly.")
    assert_equal("a",          GMP::Z.new(10).to_s(  16),   "GMP::Z should to_s(  16) correctly.")
    assert_equal("A",          GMP::Z.new(10).to_s( -16),   "GMP::Z should to_s( -16) correctly.")
    assert_equal("11111111",   GMP::Z.new(255).to_s(:bin),  "GMP::Z should to_s(:bin) correctly.")
    assert_equal("377",        GMP::Z.new(255).to_s(:oct),  "GMP::Z should to_s(:oct) correctly.")
    assert_equal("255",        GMP::Z.new(255).to_s(:dec),  "GMP::Z should to_s(:dec) correctly.")
    assert_equal("ff",         GMP::Z.new(255).to_s(:hex),  "GMP::Z should to_s(:hex) correctly.")
  end
end
