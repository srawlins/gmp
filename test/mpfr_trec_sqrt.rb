require './test_helper'

class MPFR_TREC_SQRT < Test::Unit::TestCase
  def setup
    @rand_state = GMP::RandState.new
    @one = GMP::F(1)
    @zero = GMP::F(0)
    @inf = @one/@zero
    @nan = @inf - @inf
    @neg_zero = GMP::F("-0")
    @neg_one = GMP::F(-1)
    @neg_inf = @neg_one/@zero
  end

  # This really should be moved to be a method of GMP::F
  def integer_component(f)
    index_e = f.to_s.index('e')
    exp = f.to_s[(index_e+1)..-1].to_i
    return 0 if exp < 0
    f.to_s[2..(index_e-1)][0,exp]
  end
  
  def special
    x = @nan.rec_sqrt(GMP::GMP_RNDN)
    assert_true(x.nan?, "@nan.rec_sqrt should be NaN")
    x = @inf.rec_sqrt(GMP::GMP_RNDN)
    assert_true(x.zero?, "@inf.rec_sqrt should be +0")
    assert_true(x.hash != @neg_zero.hash, "@inf.rec_sqrt should be +0")
    x = @neg_inf.rec_sqrt(GMP::GMP_RNDN)
    assert_true(x.nan?, "@neg_inf.rec_sqrt should be NaN")
    x = @zero.rec_sqrt(GMP::GMP_RNDN)
    assert_true(x.infinite?, "@zero.rec_sqrt should be inf")
    x = @neg_zero.rec_sqrt(GMP::GMP_RNDN)
    assert_true(x.infinite?, "@neg_zero.rec_sqrt should be inf")
    x = @neg_one.rec_sqrt(GMP::GMP_RNDN)
    assert_true(x.nan?, "@neg_one.rec_sqrt should be NaN")
    x = @one.rec_sqrt(GMP::GMP_RNDN)
    assert_equal(@one, x, "@one.rec_sqrt should be 1")
    
    # Can't do 0b1e-1 yet
    #x = GMP::F("0b1.0001110110101001010100e-1", 23)
    #y = x.rec_sqrt(GMP::GMP_RNDU, 33)
    #x = GMP::F("0b1.01010110101110100100100101011", 33)
    #assert_equal(x, y, "something about precision...")
  end

  def bad_case1
    x = GMP::F("1.08310518720928b30e@-120", 72, 16, GMP::GMP_RNDN)
    z = GMP::F("f.8@59", 6, 16, GMP::GMP_RNDN)
    # z = rec_sqrt(x) rounded on 6 bits toward 0, the exact value being
    #  ~= f.bffffffffffffffffa11@59
    y = x.rec_sqrt(GMP::GMP_RNDZ, 6)
    assert_equal(y, z, "These should be equal...")
  end
  
  def test_prec
    special
    bad_case1
    #generic
    #bad_cases
  end
end
