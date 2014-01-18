require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcRational < Test::Unit::TestCase
  def setup
    @z_1 = GMP::Z(1)
    @z_2 = GMP::Z(2)
    @z_neg_11 = GMP::Z(-11)

  end

  def test_init_null
    assert_equal(GMP::Q.new(), 0, "GMP::Q.new() should initialize to 0")
  end

  def test_init_fixnum
    assert_equal(GMP::Q.new(  1),   1, "GMP::Q.new(x) should accept a Fixnum")
    assert_equal(GMP::Q.new(-11), -11, "GMP::Q.new(x) should accept a negative Fixnum")
  end

  def test_init_z
    assert_equal(GMP::Q.new(@z_1),        1, "GMP::Q.new(x) should accept a GMP::Z")
    assert_equal(GMP::Q.new(@z_neg_11), -11, "GMP::Q.new(x) should accept a negative GMP::Z")
  end

  def test_init_string
    assert_equal(GMP::Q.new("1/2").to_s, "1/2", "GMP::Q.new(x : String) should initialize to x")
    assert_equal(GMP::Q.new("1/3").to_s, "1/3", "GMP::Q.new(x : String) should initialize to x")
  end

  def test_init_q
    a = GMP::Q.new(1, 2)
    assert_equal(GMP::Q.new(a), a, "GMP::Q.new(x : Q) should initialize to x")
  end

  def test_init_pair
    assert_equal(GMP::Q.new(1, 2).to_s, "1/2", "GMP::Q.new(n : Fixnum, d : Fixnum) initializes correctly")
    assert_equal(GMP::Q.new(1, 3).to_s, "1/3", "GMP::Q.new(n : Fixnum, d : Fixnum) initializes correctly")
    assert_equal(GMP::Q.new(2, 4).to_s, "1/2", "GMP::Q.new(n : Fixnum, d : Fixnum) initializes correctly")
    assert_equal(GMP::Q.new(1, 2).to_s, "1/2", "GMP::Q.new(n : Fixnum, d : Fixnum) initializes correctly")

    assert_equal(GMP::Q.new(@z_1, 2).to_s, "1/2", "GMP::Q.new(n : GMP::Z, d : Fixnum) initializes correctly")
    assert_equal(GMP::Q.new(@z_1, @z_2).to_s, "1/2", "GMP::Q.new(n : GMP::Z, d : GMP::Z) initializes correctly")
  end

  def test_bad_init
    assert_raise(ArgumentError) { GMP::Q.new(1, 2, 3) }
  end
  
  def test_neg
        a = GMP::Q( 1, 2)
    neg_a = GMP::Q(-1, 2)
    assert_equal(neg_a, -a,     "GMP::Q#-@ should work.")
    assert_equal(neg_a,  a.neg, "GMP::Q#neg should work.")
    a.neg!
    assert_equal(neg_a,  a,     "GMP::Q#neg! should work.")
    
    b     = GMP::Q(-4, 3)
    neg_b = GMP::Q( 4, 3)
    assert_equal(neg_b, -b,     "GMP::Q#-@ should work.")
    assert_equal(neg_b,  b.neg, "GMP::Q#neg should work.")
    b.neg!
    assert_equal(neg_b,  b,     "GMP::Q#neg! should work.")
    
    c     = GMP::Q( "9753108642/2")
    neg_c = GMP::Q("-9753108642/2")
    assert_equal(neg_c, -c,     "GMP::Q#-@ should work.")
    assert_equal(neg_c,  c.neg, "GMP::Q#neg should work.")
    c.neg!
    assert_equal(neg_c,  c,     "GMP::Q#neg! should work.")
    
    d     = GMP::Q(0)
    neg_d = GMP::Q(0)
    assert_equal(neg_d, -d,     "GMP::Q#-@ should work.")
    assert_equal(neg_d,  d.neg, "GMP::Q#neg should work.")
    d.neg!
    assert_equal(neg_d,  d,     "GMP::Q#neg! should work.")
  end
  
  def test_abs
    a     = GMP::Q(1, 2)
    abs_a = GMP::Q(1, 2)
    assert_equal(abs_a,  a.abs, "GMP::Q#abs should work.")
    a.abs!
    assert_equal(abs_a,  a,     "GMP::Q#abs! should work.")
    
    b     = GMP::Q(-4, 3)
    abs_b = GMP::Q( 4, 3)
    assert_equal(abs_b,  b.abs, "GMP::Q#abs should work.")
    b.abs!
    assert_equal(abs_b,  b,     "GMP::Q#abs! should work.")
    
    c     = GMP::Q("9753108642/2")
    abs_c = GMP::Q("9753108642/2")
    assert_equal(abs_c,  c.abs, "GMP::Q#abs should work.")
    c.abs!
    assert_equal(abs_c,  c,     "GMP::Q#abs! should work.")
    
    d     = GMP::Q(0)
    abs_d = GMP::Q(0)
    assert_equal(abs_d,  d.abs, "GMP::Q#abs should work.")
    d.abs!
    assert_equal(abs_d,  d,     "GMP::Q#abs! should work.")
  end
  
  def test_inv
    a     = GMP::Q(1,2)
    inv_a = GMP::Q(2,1)
    assert_equal(inv_a,  a.inv, "GMP::Q#inv should work.")
    a.inv!
    assert_equal(inv_a,  a,     "GMP::Q#inv! should work.")
    
    b     = GMP::Q(-4,3)
    inv_b = GMP::Q(-3,4)
    assert_equal(inv_b,  b.inv, "GMP::Q#inv should work.")
    b.inv!
    assert_equal(inv_b,  b,     "GMP::Q#inv! should work.")
    
    c     = GMP::Q("9753108642/2")
    inv_c = GMP::Q("2/9753108642")
    assert_equal(inv_c,  c.inv, "GMP::Q#inv should work.")
    c.inv!
    assert_equal(inv_c,  c,     "GMP::Q#inv! should work.")
    
    d = GMP::Q(0)
    assert_raise(ZeroDivisionError) { d.inv }
    assert_raise(ZeroDivisionError) { d.inv! }
  end
  
  def test_to_s
    assert_equal("1/2", GMP::Q(1, 2).to_s, "GMP::Q should to_s properly.")
    assert_equal("1/4294967296", GMP::Q(1, 2**32).to_s, "GMP::Q should to_s properly.")
    assert_equal("1/4294967296", GMP::Q(1, 2**32).to_s, "GMP::Q should to_s properly.")
    assert_equal("-22/7", GMP::Q(-22, 7).to_s, "GMP::Q should to_s properly.")
    assert_equal("-22/7", GMP::Q(22, -7).to_s, "GMP::Q should to_s properly.")
    assert_equal("22/7", GMP::Q(-22, -7).to_s, "GMP::Q should to_s properly.")
    assert_equal("0", GMP::Q(0, 1).to_s, "GMP::Q should to_s properly.")
    assert_equal("0", GMP::Q(0, 2000).to_s, "GMP::Q should to_s properly.")
    assert_equal("0", GMP::Q(0, -2000).to_s, "GMP::Q should to_s properly.")

    assert_equal("1/2", GMP::Q(1, 2).inspect, "GMP::Q should inspect properly.")
  end
end
