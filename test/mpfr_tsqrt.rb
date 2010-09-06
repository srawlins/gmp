require './test_helper'

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
  
  def check24(as, rnd_mode, qs)
    q = GMP::F(as, 24)
    q = q.sqrt(rnd_mode)
    assert_equal(q, GMP::F(qs), "Sqrt of #{as} (prec 24) should be #{qs}.")
  end
  
  def check_diverse(as, p, qs)
    q = GMP::F(as, p)
    q = q.sqrt
    assert_equal(integer_component(q), qs)
  end
  
  def check_float
    check24("70368760954880.0", GMP::GMP_RNDN, "8.388609e6")
    check24("281474943156224.0", GMP::GMP_RNDN, "1.6777215e7")
    check24("70368777732096.0", GMP::GMP_RNDN, "8.388610e6")
    check24("281474909601792.0", GMP::GMP_RNDN, "1.6777214e7")
    check24("100216216748032.0", GMP::GMP_RNDN, "1.0010805e7")
    check24("120137273311232.0", GMP::GMP_RNDN, "1.0960715e7")
    check24("229674600890368.0", GMP::GMP_RNDN, "1.5155019e7")
    check24("70368794509312.0", GMP::GMP_RNDN, "8.388611e6")
    check24("281474876047360.0", GMP::GMP_RNDN, "1.6777213e7")
    check24("91214552498176.0", GMP::GMP_RNDN, "9.550631e6")
    
    check24("70368760954880.0", GMP::GMP_RNDZ, "8.388608e6")
    check24("281474943156224.0", GMP::GMP_RNDZ, "1.6777214e7")
    check24("70368777732096.0", GMP::GMP_RNDZ, "8.388609e6")
    check24("281474909601792.0", GMP::GMP_RNDZ, "1.6777213e7")
    check24("100216216748032.0", GMP::GMP_RNDZ, "1.0010805e7")
    check24("120137273311232.0", GMP::GMP_RNDZ, "1.0960715e7")
    check24("229674600890368.0", GMP::GMP_RNDZ, "1.5155019e7")
    check24("70368794509312.0", GMP::GMP_RNDZ, "8.38861e6")
    check24("281474876047360.0", GMP::GMP_RNDZ, "1.6777212e7")
    check24("91214552498176.0", GMP::GMP_RNDZ, "9.550631e6")

    check24("70368760954880.0", GMP::GMP_RNDU, "8.388609e6")
    check24("281474943156224.0", GMP::GMP_RNDU, "1.6777215e7")
    check24("70368777732096.0", GMP::GMP_RNDU, "8.388610e6")
    check24("281474909601792.0", GMP::GMP_RNDU, "1.6777214e7")
    check24("100216216748032.0", GMP::GMP_RNDU, "1.0010806e7")
    check24("120137273311232.0", GMP::GMP_RNDU, "1.0960716e7")
    check24("229674600890368.0", GMP::GMP_RNDU, "1.515502e7")
    check24("70368794509312.0", GMP::GMP_RNDU, "8.388611e6")
    check24("281474876047360.0", GMP::GMP_RNDU, "1.6777213e7")
    check24("91214552498176.0", GMP::GMP_RNDU, "9.550632e6")

    check24("70368760954880.0", GMP::GMP_RNDD, "8.388608e6")
    check24("281474943156224.0", GMP::GMP_RNDD, "1.6777214e7")
    check24("70368777732096.0", GMP::GMP_RNDD, "8.388609e6")
    check24("281474909601792.0", GMP::GMP_RNDD, "1.6777213e7")
    check24("100216216748032.0", GMP::GMP_RNDD, "1.0010805e7")
    check24("120137273311232.0", GMP::GMP_RNDD, "1.0960715e7")
    check24("229674600890368.0", GMP::GMP_RNDD, "1.5155019e7")
    check24("70368794509312.0", GMP::GMP_RNDD, "8.38861e6")
    check24("281474876047360.0", GMP::GMP_RNDD, "1.6777212e7")
    check24("91214552498176.0", GMP::GMP_RNDD, "9.550631e6")
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
  
  def check_inexact(prec)
    x = @rand_state.mpfr_urandomb(p)
    
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
    assert_equal(GMP::F(qs), q, "Sqrt of -0.0 should be something like 0.0.")
  end
  
  def check4(as, rounding, qs)
    q = GMP::F(as, 53)
    q = q.sqrt(rounding)
    assert_equal(q, GMP::F(qs, GMP::F.default_prec, 16), "Sqrt of #{as} should be #{qs}.")
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
    check_float
    
    check3("-0.0", GMP::GMP_RNDN, "0.0")
    
    check4("6.37983013646045901440e+32", GMP::GMP_RNDN, "5.9bc5036d09e0c@13")
    check4("1.0", GMP::GMP_RNDN, "1")
    check4("1.0", GMP::GMP_RNDZ, "1")
    check4("3.725290298461914062500000e-9", GMP::GMP_RNDN, "4@-4")
    check4("3.725290298461914062500000e-9", GMP::GMP_RNDZ, "4@-4")
    
    check4("1190456976439861.0", GMP::GMP_RNDZ, "2.0e7957873529a@6")
    check4("1219027943874417664.0", GMP::GMP_RNDZ, "4.1cf2af0e6a534@7")
    # the following examples are bugs in Cygnus compiler/system, found by
    # Fabrice Rouillier while porting mpfr to Windows
    check4("9.89438396044940256501e-134", GMP::GMP_RNDU, "8.7af7bf0ebbee@-56")
    check4("7.86528588050363751914e+31", GMP::GMP_RNDZ, "1.f81fc40f32062@13")
    check4("0.99999999999999988897", GMP::GMP_RNDN, "f.ffffffffffff8@-1")
    check4("1.00000000000000022204", GMP::GMP_RNDN, "1")
    # the following examples come from the paper "Number-theoretic Test
    # Generation for Directed Rounding" from Michael Parks, Table 4
    check4("78652858805036375191418371571712.0", GMP::GMP_RNDN, "1.f81fc40f32063@13")
    check4("38510074998589467860312736661504.0", GMP::GMP_RNDN, "1.60c012a92fc65@13")
    check4("35318779685413012908190921129984.0", GMP::GMP_RNDN, "1.51d17526c7161@13")
    check4("26729022595358440976973142425600.0", GMP::GMP_RNDN, "1.25e19302f7e51@13")
    check4("22696567866564242819241453027328.0", GMP::GMP_RNDN, "1.0ecea7dd2ec3d@13")
    check4("22696888073761729132924856434688.0", GMP::GMP_RNDN, "1.0ecf250e8e921@13")
    check4("36055652513981905145251657416704.0", GMP::GMP_RNDN, "1.5552f3eedcf33@13")
    check4("30189856268896404997497182748672.0", GMP::GMP_RNDN, "1.3853ee10c9c99@13")
    check4("36075288240584711210898775080960.0", GMP::GMP_RNDN, "1.556abe212b56f@13")
    check4("72154663483843080704304789585920.0", GMP::GMP_RNDN, "1.e2d9a51977e6e@13")
    check4("78652858805036375191418371571712.0", GMP::GMP_RNDZ, "1.f81fc40f32062@13")
    check4("38510074998589467860312736661504.0", GMP::GMP_RNDZ, "1.60c012a92fc64@13")
    check4("35318779685413012908190921129984.0", GMP::GMP_RNDZ, "1.51d17526c716@13")
    check4("26729022595358440976973142425600.0", GMP::GMP_RNDZ, "1.25e19302f7e5@13")
    check4("22696567866564242819241453027328.0", GMP::GMP_RNDZ, "1.0ecea7dd2ec3c@13")
    check4("22696888073761729132924856434688.0", GMP::GMP_RNDZ, "1.0ecf250e8e92@13")
    check4("36055652513981905145251657416704.0", GMP::GMP_RNDZ, "1.5552f3eedcf32@13")
    check4("30189856268896404997497182748672.0", GMP::GMP_RNDZ, "1.3853ee10c9c98@13")
    check4("36075288240584711210898775080960.0", GMP::GMP_RNDZ, "1.556abe212b56e@13")
    check4("72154663483843080704304789585920.0", GMP::GMP_RNDZ, "1.e2d9a51977e6d@13")
    check4("78652858805036375191418371571712.0", GMP::GMP_RNDU, "1.f81fc40f32063@13")
    check4("38510074998589467860312736661504.0", GMP::GMP_RNDU, "1.60c012a92fc65@13")
    check4("35318779685413012908190921129984.0", GMP::GMP_RNDU, "1.51d17526c7161@13")
    check4("26729022595358440976973142425600.0", GMP::GMP_RNDU, "1.25e19302f7e51@13")
    check4("22696567866564242819241453027328.0", GMP::GMP_RNDU, "1.0ecea7dd2ec3d@13")
    check4("22696888073761729132924856434688.0", GMP::GMP_RNDU, "1.0ecf250e8e921@13")
    check4("36055652513981905145251657416704.0", GMP::GMP_RNDU, "1.5552f3eedcf33@13")
    check4("30189856268896404997497182748672.0", GMP::GMP_RNDU, "1.3853ee10c9c99@13")
    check4("36075288240584711210898775080960.0", GMP::GMP_RNDU, "1.556abe212b56f@13")
    check4("72154663483843080704304789585920.0", GMP::GMP_RNDU, "1.e2d9a51977e6e@13")
    check4("78652858805036375191418371571712.0", GMP::GMP_RNDD, "1.f81fc40f32062@13")
    check4("38510074998589467860312736661504.0", GMP::GMP_RNDD, "1.60c012a92fc64@13")
    check4("35318779685413012908190921129984.0", GMP::GMP_RNDD, "1.51d17526c716@13")
    check4("26729022595358440976973142425600.0", GMP::GMP_RNDD, "1.25e19302f7e5@13")
    check4("22696567866564242819241453027328.0", GMP::GMP_RNDD, "1.0ecea7dd2ec3c@13")
    check4("22696888073761729132924856434688.0", GMP::GMP_RNDD, "1.0ecf250e8e92@13")
    check4("36055652513981905145251657416704.0", GMP::GMP_RNDD, "1.5552f3eedcf32@13")
    check4("30189856268896404997497182748672.0", GMP::GMP_RNDD, "1.3853ee10c9c98@13")
    check4("36075288240584711210898775080960.0", GMP::GMP_RNDD, "1.556abe212b56e@13")
    check4("72154663483843080704304789585920.0", GMP::GMP_RNDD, "1.e2d9a51977e6d@13")
  end
end
