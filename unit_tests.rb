#!/usr/bin/env ruby

require 'test/unit'
require 'gmp'

class TC_Z < Test::Unit::TestCase
    def test_init_null
        assert_equal(GMP::Z.new(), 0, "GMP::Z.new() should initialize to 0")
    end
    
    def test_init_fixnum
        assert_equal(GMP::Z.new(1), 1, "GMP::Z.new(x : Fixnum) should initialize to x")
    end
    
    def test_init_z
        b = GMP::Z.new(1)
        assert_equal(GMP::Z.new(b), b, "GMP::Z.new(x : GMP::Z) should initialize to x")
    end

    def test_init_string        
        assert_equal(GMP::Z.new("1"), 1, "GMP::Z.new(x : String) should initialize to x")
    end

    def test_init_bignum
        assert_equal(GMP::Z.new(2**32), 2**32, "GMP::Z.new(x : Bignum) should initialize to x")
    end
    
    def test_cmp_z
        a = GMP::Z.new(2)
        b = GMP::Z.new(1)
        assert_equal(a >  b, true,  "GMP::Z should compare correctly with GMP::Z")
        assert_equal(a >  a, false, "GMP::Z should compare correctly with GMP::Z")
        assert_equal(b >  a, false, "GMP::Z should compare correctly with GMP::Z")

        assert_equal(a >= b, true,  "GMP::Z should compare correctly with GMP::Z")
        assert_equal(a >= a, true,  "GMP::Z should compare correctly with GMP::Z")
        assert_equal(b >= a, false, "GMP::Z should compare correctly with GMP::Z")

        assert_equal(a <= b, false, "GMP::Z should compare correctly with GMP::Z")
        assert_equal(a <= a, true,  "GMP::Z should compare correctly with GMP::Z")
        assert_equal(b <= a, true,  "GMP::Z should compare correctly with GMP::Z")

        assert_equal(a <  b, false, "GMP::Z should compare correctly with GMP::Z")
        assert_equal(a <  a, false, "GMP::Z should compare correctly with GMP::Z")
        assert_equal(b <  a, true,  "GMP::Z should compare correctly with GMP::Z")

        assert_equal(a == b, false, "GMP::Z should == correctly with GMP::Z")
        assert_equal(a == a, true,  "GMP::Z should == correctly with GMP::Z")

        assert_equal(a != b, true,  "GMP::Z should != correctly with GMP::Z")
        assert_equal(a != a, false, "GMP::Z should != correctly with GMP::Z")

        assert_equal(a <=> b,  1, "GMP::Z should <=> correctly with GMP::Z")
        assert_equal(a <=> a,  0, "GMP::Z should <=> correctly with GMP::Z")
        assert_equal(b <=> a, -1, "GMP::Z should <=> correctly with GMP::Z")
    end
    
    def test_to_s
        assert_equal(GMP::Z.new(1).to_s, "1", "Z(1).to_s should equal '1'")
        assert_equal(GMP::Z.new(-1).to_s, "-1", "Z(1).to_s should equal '-1'")
    end
end

# Tested: [Z op Z, Z op FixNum, Z op BigNum, FixNum op Z, BigNum op Z]
# Things are tested both ways because the implementation is asymetrical
class TC_Z_Basic < Test::Unit::TestCase
    def setup
        @a=GMP::Z.new(100)
        @b=GMP::Z.new(200)
        @c=2**32
    end

    def test_add
        assert_equal(@a + @b, 100   + 200,   "GMP::Z should add correctly")
        assert_equal(@a + 2,  100   + 2,     "GMP::Z should add correctly")
        assert_equal(@a + @c, 100   + 2**32, "GMP::Z should add correctly")
        assert_equal( 2 + @a, 2     + 100,   "GMP::Z should add correctly")
        assert_equal(@c + @a, 2**32 + 100,   "GMP::Z should add correctly")
    end

    def test_sub
        assert_equal(@a - @b, 100   - 200,   "GMP::Z should subtract correctly")
        assert_equal(@a - 2,  100   - 2,     "GMP::Z should subtract correctly")
        assert_equal(@a - @c, 100   - 2**32, "GMP::Z should subtract correctly")
        assert_equal( 2 - @a, 2     - 100,   "GMP::Z should subtract correctly")
        assert_equal(@c - @a, 2**32 - 100,   "GMP::Z should subtract correctly")
    end

    def test_mul
        assert_equal(@a * @b, 100   * 200,   "GMP::Z should multiply correctly")
        assert_equal(@a * 2,  100   * 2,     "GMP::Z should multiply correctly")
        assert_equal(@a * @c, 100   * 2**32, "GMP::Z should multiply correctly")
        assert_equal( 2 * @a, 2     * 100,   "GMP::Z should multiply correctly")
        assert_equal(@c * @a, 2**32 * 100,   "GMP::Z should multiply correctly")
    end
end

class TC_Z_Logic < Test::Unit::TestCase
    def setup
        @a=GMP::Z.new(0xffff00000000ffff)
        @b=GMP::Z.new(0xffff0000ffff0000)
        @c=0xff00ff00ff00ff00
        @d=0x1248 
    end
    
    def test_and
        assert_equal(@a & @b, 0xffff00000000ffff & 0xffff0000ffff0000, "GMP::Z should AND correctly")
        assert_equal(@a & @d, 0xffff00000000ffff & 0x1248,             "GMP::Z should AND correctly")
        assert_equal(@a & @c, 0xffff00000000ffff & 0xff00ff00ff00ff00, "GMP::Z should AND correctly")
        assert_equal(@d & @a, 0x1248             & 0xffff00000000ffff, "GMP::Z should AND correctly")
        assert_equal(@c & @a, 0xff00ff00ff00ff00 & 0xffff00000000ffff, "GMP::Z should AND correctly")
    end

    def test_or
        assert_equal(@a | @b, 0xffff00000000ffff | 0xffff0000ffff0000, "GMP::Z should OR correctly")
        assert_equal(@a | @d, 0xffff00000000ffff | 0x1248,             "GMP::Z should OR correctly")
        assert_equal(@a | @c, 0xffff00000000ffff | 0xff00ff00ff00ff00, "GMP::Z should OR correctly")
        assert_equal(@d | @a, 0x1248             | 0xffff00000000ffff, "GMP::Z should OR correctly")
        assert_equal(@c | @a, 0xff00ff00ff00ff00 | 0xffff00000000ffff, "GMP::Z should OR correctly")
    end

    def test_xor
        assert_equal(@a ^ @b, 0xffff00000000ffff ^ 0xffff0000ffff0000, "GMP::Z should XOR correctly")
        assert_equal(@a ^ @d, 0xffff00000000ffff ^ 0x1248,             "GMP::Z should XOR correctly")
        assert_equal(@a ^ @c, 0xffff00000000ffff ^ 0xff00ff00ff00ff00, "GMP::Z should XOR correctly")
        assert_equal(@d ^ @a, 0x1248             ^ 0xffff00000000ffff, "GMP::Z should XOR correctly")
        assert_equal(@c ^ @a, 0xff00ff00ff00ff00 ^ 0xffff00000000ffff, "GMP::Z should XOR correctly")
    end

    def test_com
        assert_equal(GMP::Z().com, -1, "GMP::Z.com should complement")
        assert_equal(GMP::Z(1).com, -2, "GMP::Z.com should complement")
        assert_equal(GMP::Z(-1).com, 0, "GMP::Z.com should complement")
        assert_equal(@a.com, -0xffff00000000ffff-1, "GMP::Z.com should complement")
    end
    
    def test_logic
        a=GMP::Z.new(0x1234)
        b=GMP::Z.new(a)
        b[0]=true

        assert_equal(a,     0x1234, "GMP::Z.new() should create a new copy, not alias")
        assert_equal(b,     0x1235, "GMP::Z#[]= should modify bits")
        assert_equal(a[2],    true, "GMP::Z#[] should access bits")
        assert_equal(a.scan0(0), 0, "GMP::Z#scan0 should scan for 0s")
        assert_equal(a.scan1(0), 2, "GMP::Z#scan1 should scan for 1s")

    end
end

class TC_Q < Test::Unit::TestCase
    def test_init_null
        assert_equal(GMP::Q.new(), 0, "GMP::Q.new() should initialize to 0")
    end

    def test_init_fixnum
        assert_equal(GMP::Q.new(1), 1, "GMP::Q.new(x : Fixnum) should initialize to 0")
    end

    def test_init_fixnum2
        assert_equal(GMP::Q.new(1, 2).to_s, "1/2", "GMP::Q.new(num : Fixnum, den : Fixnum) should initialize to num/den")
    end

    def test_init_string
        assert_equal(GMP::Q.new("1/2").to_s, "1/2", "GMP::Q.new(x : String) should initialize to x")
    end

    def test_init_q
        a = GMP::Q.new(1,2)
        assert_equal(GMP::Q.new(a), a, "GMP::Q.new(x : Q) should initialize to x")
    end
end

class TC_Cmp < Test::Unit::TestCase
    def setup
        @a=GMP::Z.new(180)
        @c=GMP::Q.new(2000,11) # ~181.82
        @d=GMP::Q.new(3000,17) # ~176.47
        @e=700
        @f=2**32
        @g=GMP::Q(360,2)
    end

    def test_cmp_q_q
        assert_equal(@c >  @d, true,  "GMP::Q should compare correctly")
        assert_equal(@c >= @d, true,  "GMP::Q should compare correctly")
        assert_equal(@c <  @d, false, "GMP::Q should compare correctly")
        assert_equal(@c <= @d, false, "GMP::Q should compare correctly")
        assert_equal(@c == @d, false, "GMP::Q should == correctly")
        assert_equal(@c == @c, true,  "GMP::Q should == correctly")
        assert_equal(@c != @d, true,  "GMP::Q should != correctly")
        assert_equal(@c != @c, false, "GMP::Q should != correctly")

        assert_equal(@c <=> @d,  1, "GMP::Q should <=> correctly")
        assert_equal(@c <=> @c,  0, "GMP::Q should <=> correctly")
        assert_equal(@d <=> @c, -1, "GMP::Q should <=> correctly")
    end

    def test_cmp_q_z
        assert_equal(@c >  @a, true,  "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@c >= @a, true,  "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@c <  @a, false, "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@c <= @a, false, "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@c == @a, false, "GMP::Q should == correctly with GMP::Z")
        assert_equal(@c != @a, true,  "GMP::Q should != correctly with GMP::Z")

        assert_equal(@a <  @c, true,  "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a <= @c, true,  "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a >  @c, false, "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a >= @c, false, "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a == @c, false, "GMP::Z should == correctly with GMP::Q")
        assert_equal(@a != @c, true,  "GMP::Z should != correctly with GMP::Q")

        assert_equal(@g == @a, true,  "GMP::Q should == correctly with GMP::Z")
        assert_equal(@g != @a, false, "GMP::Q should != correctly with GMP::Z")

        assert_equal(@d >  @a, false, "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@d >= @a, false, "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@d <  @a, true,  "GMP::Q should compare correctly with GMP::Z")
        assert_equal(@d <= @a, true,  "GMP::Q should compare correctly with GMP::Z")

        assert_equal(@a <  @d, false, "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a <= @d, false, "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a >  @d, true,  "GMP::Z should compare correctly with GMP::Q")
        assert_equal(@a >= @d, true,  "GMP::Z should compare correctly with GMP::Q")

        assert_equal(@c <=> @a,  1, "GMP::Q should <=> correctly with GMP::Z")
        assert_equal(@g <=> @a,  0, "GMP::Q should <=> correctly with GMP::Z")
        assert_equal(@a <=> @c, -1, "GMP::Q should <=> correctly with GMP::Z")
    end

    def test_cmp_z_int
        assert_equal(@a <=> @e, -1, "GMP::Z should <=> correctly with Fixnum")
        assert_equal(@e <=> @a,  1, "Fixnum should <=> correctly with GMP::Z")
        assert_equal(@a <=> @f, -1, "GMP::Z should <=> correctly with Bignum")
        assert_equal(@f <=> @a,  1, "Bignum should <=> correctly with GMP::Z")
    end

    def test_cmp_q_int
        assert_equal(@c <=> @e, -1, "GMP::Q should <=> correctly with Fixnum")
        assert_equal(@e <=> @c,  1, "Fixnum should <=> correctly with GMP::Q")
        assert_equal(@c <=> @f, -1, "GMP::Q should <=> correctly with Bignum")
        assert_equal(@f <=> @c,  1, "Bignum should <=> correctly with GMP::Q")
    end
end

# [Q op Q, Q op Z, Z op Q, Q op FixNum, Q op BigNum, FixNum op Q, BigNum op Q]
class TC_Q_Basic < Test::Unit::TestCase
    def setup
        @a=GMP::Q.new(100,11)
        @b=GMP::Q.new(200,17)
        @c=GMP::Z.new(40)
        @d=2**32
    end

    def test_add
        assert_equal(@a + @b, GMP::Q(3900, 187),       "GMP::Q should add GMP::Q correctly")
        assert_equal(@a + @c, GMP::Q(540,  11),        "GMP::Q should add GMP::Z correctly")
        assert_equal(@c + @a, GMP::Q(540,  11),        "GMP::Z should add GMP::Q correctly")
        assert_equal(@a +  2, GMP::Q(122,  11),        "GMP::Z should add Fixnum correctly")
        assert_equal(@a + @d, GMP::Q(47244640356, 11), "GMP::Z should add Bignum correctly")
        assert_equal( 2 + @a, GMP::Q(122,  11),        "Fixnum should add GMP::Q correctly")
        assert_equal(@d + @a, GMP::Q(47244640356, 11), "Bignum should add GMP::Q correctly")
    end

    def test_sub
        assert_equal(@a - @b, GMP::Q(-500, 187),       "GMP::Q should subtract GMP::Q correctly")
        assert_equal(@a - @c, GMP::Q(-340, 11),        "GMP::Q should subtract GMP::Z correctly")
        assert_equal(@c - @a, GMP::Q(340,  11),        "GMP::Z should subtract GMP::Q correctly")
        assert_equal(@a -  2, GMP::Q(78,  11),         "GMP::Z should subtract Fixnum correctly")
        assert_equal(@a - @d, GMP::Q(-47244640156, 11),"GMP::Z should subtract Bignum correctly")
        assert_equal( 2 - @a, GMP::Q(-78,  11),        "Fixnum should subtract GMP::Q correctly")
        assert_equal(@d - @a, GMP::Q(47244640156, 11), "Bignum should subtract GMP::Q correctly")
    end

    def test_mul
        assert_equal(@a * @b, GMP::Q(20000, 187),      "GMP::Q should multiply GMP::Q correctly")
        assert_equal(@a * @c, GMP::Q(4000,  11),       "GMP::Q should multiply GMP::Z correctly")
        assert_equal(@c * @a, GMP::Q(4000,  11),       "GMP::Z should multiply GMP::Q correctly")
        assert_equal(@a *  2, GMP::Q(200,  11),        "GMP::Z should multiply Fixnum correctly")
        assert_equal(@a * @d, GMP::Q(429496729600, 11),"GMP::Z should multiply Bignum correctly")
        assert_equal( 2 * @a, GMP::Q(200,  11),        "Fixnum should multiply GMP::Q correctly")
        assert_equal(@d * @a, GMP::Q(429496729600, 11),"Bignum should multiply GMP::Q correctly")
    end
end
