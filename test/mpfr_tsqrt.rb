require 'test_helper'

class MPFR_TSQRT < Test::Unit::TestCase
  def setup
    @rand_state = GMP::RandState.new
  end

  # This really should be moved to be a method of GMP::F
  def integer_component(f)
    index_e = f.to_s.index('e')
    exp = f.to_s[(index_e+1)..-1].to_i
    return 0 if exp < 0
    f.to_s[2..(index_e-1)][0,exp]
  end
  
  def check_diverse(as, p, qs)
    q = GMP::F(as, p)
    q = q.sqrt
    assert_equal(integer_component(q), qs)
  end
  
  def special
    x = GMP::F(0b1010000010100011011001010101010010001100001101011101110001011001 / 2, 64)  # Ruby 0b does not support e,
    y = x.sqrt(GMP::GMP_RNDN, 32)                                                           # ie 0b11e-2 = 0.75 doesnt work
    #assert_equal(2405743844, y, "Error for n^2+n+1/2 with n=2405743843")  # Pending
    
    x = GMP::F(0b10100000101000110110010101010100100011000011010111011100010110001 / 4, 65)
    y = x.sqrt(GMP::GMP_RNDN, 32)
    #assert_equal(2405743844, y, "Error for n^2+n+1/4 with n=2405743843")  # Pending
    
    x = GMP::F(0b101000001010001101100101010101001000110000110101110111000101100011 / 8, 66)
    y = x.sqrt(GMP::GMP_RNDN, 32)
    #assert_equal(2405743844, y, "Error for n^2+n+1/4+1/8 with n=2405743843")  # Pending
    
    x = GMP::F(0b101000001010001101100101010101001000110000110101110111000101100001 / 8, 66)
    y = x.sqrt(GMP::GMP_RNDN, 32)
    assert_equal(2405743843, y, "Error for n^2+n+1/8 with n=2405743843")
    
    #####
    ##### Inexact flag test. Not implemented.
    
    #####
    ##### Test uses mpfr_nexttoinf. Not implemented.
    
    # Not an accurate test at all. Reassigning z generates a new mpfr object.
    x = GMP::F(1)
    z = GMP::F(-1)
    z = x.sqrt(GMP::GMP_RNDN)
    
    #z = GMP::F(0.1011010100000100100100100110011001011100100100000011000111011001011101101101110000110100001000100001100001011000E1, 160)
    #x = z.sqrt
    # z = x.sqrt
    # x.prec = 53
  end
  
  def property1(p, rounding)
    x = @rand_state.mpfr_urandomb(p)
    y = @rand_state.mpfr_urandomb(p)
    z = x**2 + y**2
    z = x / z.sqrt(rounding)
    assert_between(-1, 1, z, "-1 <= x/sqrt(x^2+y^2) <= 1 should hold.")
  end
  
  def property2(p, rounding)
    x = @rand_state.mpfr_urandomb(p)
    y = (x * x).sqrt(rounding)
    assert_true(x == y, "sqrt(#{x.to_s}^2) should be #{x.to_s}, but is #{y.to_s} (rounding: #{GMP::F.default_rounding_mode.inspect}, prec: #{p.to_s}).")
  end
  
  def check3(as, rounding, qs)
    q = GMP::F(as, 53)
    q = q.sqrt(rounding)
    assert_equal(qs, q.to_s, "Sqrt of -0.0 should be 0.0.")
  end
  
  def test_prec
    (GMP::MPFR_PREC_MIN..128).each do |p|
      property1(p, GMP::GMP_RNDN)
      property1(p, GMP::GMP_RNDU)
      property2(p, GMP::GMP_RNDN)
    end
    
    check_diverse("635030154261163106768013773815762607450069292760790610550915652722277604820131530404842415587328",
                  160,
                  "796887792767063979679855997149887366668464780637")
    special
    #check_nan
    
    (2...200).each do |p|
      200.times do
        #check_inexact p
      end
    end
    #check_float
    
    check3("-0.0", GMP::GMP_RNDN, "0.0")
  end
end