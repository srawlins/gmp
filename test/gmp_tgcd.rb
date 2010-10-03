require './test_helper'

class GMP_TGCD < Test::Unit::TestCase
  def setup
    @rand_state = GMP::RandState.new
    @min_operand_bitsize = 1
  end
  
  def one_test(op1, op2, ref, i)
    g1, s, t = op1.gcdext(op2)
    
    if ref# and ref != g1
      assert_true(ref == g1, "GMP::Z#gcdext should work...")
    end
    
    gcdext_valid_p(op1, op2, g1, s)
    
    g2 = op1.gcd(op2)
    assert_equal(g1, g2, "gcd and gcdext should produce the same gcd.")
    
    g2, temp1, temp2 = op1.gcdext(op2)
    temp1 *= op1
    temp2 *= op2
    temp1 += temp2
    assert_true(g1 == g2 || g2 == temp1, "gcdext should work...")
  end
  
  def gcdext_valid_p(a, b, g, s)
    assert_true(g >= 0, "When [g, s, t] = a.gcdext(b), g must > 0 (g=#{g}, s=#{s}, a=#{a}, b=#{b})")
    
    if a.sgn == 0
      assert_true(g.cmpabs(b) == 0, "When [g, s, t] = a.gcdext(b), if a == 0, g must == |b|")
      assert_true(s == 0, "When [g, s, t] = a.gcdext(b), if a == 0, s should == 0")
      return
    end
    
    if b.sgn == 0
      assert_true(g.cmpabs(a) == 0, "When [g, s, t] = a.gcdext(b), if b == 0, g must == |a|")
      assert_true(s == a.sgn, "When [g, s, t] = a.gcdext(b), if a == 0, s should == a.sgn")
      return
    end
    
    assert_false(g.sgn <= 0, "When [g, s, t] = a.gcdext(b), if neither a == 0 nor b == 0, g must > 0")
    
    temp1 = a.tdiv(g)
    temp3 = a.tmod(g)
    assert_true(temp3.sgn == 0, "When [g, s, t] = a.gcdext(b), g must divide a.")
    temp2 = b.tdiv(g)
    temp3 = b.tmod(g)
    assert_true(temp3.sgn == 0, "When [g, s, t] = a.gcdext(b), g must divide b.")
    
    if GMP::GMP_VERSION > "4.3.1"
    assert_true(s.cmpabs(GMP::Z(1)) == 0 || (s*2).abs.cmpabs(temp2) <= 0, "GMP::Z#gcdext should work: #{s}.cmpabs(1)==0 or (#{s*2}.abs.cmpabs(#{temp2})<=0")
    end
    
    temp2 = s * a
    temp2 = g - temp2
    temp3 = temp2.tmod(b)
    temp2 = temp2.tdiv(b)
    assert_true(temp3.sgn == 0, "When [g, s, t] = a.gcdext(b), g must divide a.")
    
    if GMP::GMP_VERSION > "4.3.1"
    assert_true(temp2.cmpabs(GMP::Z(1)) == 0 || (temp2*2).abs.cmpabs(temp1) <= 0, "GMP::Z#gcdext should work...")
    end
  end

  def test_gcdext
    reps = 200
    #check_data
    
    op2 = GMP::Z(GMP::GMP_NUMB_MAX)
    op1 = op2 << 100
    op1 += op2
    op2 *= 2
    one_test(op1, op2, nil, -1)
    
    (0...reps).each do |i|
      bs = @rand_state.urandomb(32)
      size_range = bs.to_i % 17 + 2
      bs = @rand_state.urandomb(size_range)
      op1 = @rand_state.urandomb(bs.to_i + @min_operand_bitsize)
      bs = @rand_state.urandomb(size_range)
      op2 = @rand_state.urandomb(bs.to_i + @min_operand_bitsize)
      
      bs = @rand_state.urandomb(8)
      bsi = bs.to_i
      
      if bsi & 0x3c == 4
        op1 *= op2
      elsif bsi & 0x3c == 8
        op2 *= op1
      end
      
      op1.neg! if bsi & 1 != 0
      op2.neg! if bsi & 2 != 0
      
      one_test(op1, op2, nil, i)
      
      op1 = GMP::Z(0)
      bs = @rand_state.urandomb(32)
      bs = @rand_state.urandomb(bs.to_i % 16 + 1)
      op2 = @rand_state.rrandomb(bs.to_i)
      op2 += 1
      ref = op2
      
      # UNTIL I CAN ACCESS GCD_DC_THRESHOLD
      chain_len = 10
      (1..chain_len).each do |j|
        bs = @rand_state.urandomb(32)
	    bs = @rand_state.urandomb(bs.to_i % 12 + 1)
	    temp2 = @rand_state.rrandomb(bs.to_i + 1)
        temp2 += 1
	    temp1 = op2 * temp2
        op1 += temp1

        bs = @rand_state.urandomb(32)
	    bs = @rand_state.urandomb(bs.to_i % 12 + 1)
	    temp2 = @rand_state.rrandomb(bs.to_i + 1)
        temp2 += 1
        temp1 = op1 * temp2
        op2 += temp1
      end
      one_test(op1, op2, ref, i)
    end
  end
end
