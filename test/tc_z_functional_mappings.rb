require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcIntegerFunctionalMappings < MiniTest::Test
  def setup
    @_64bit = 1_000_000_000_000.is_a? Fixnum
    @xp1 = 7
    @xp2 = 2**30 - 1
    @xn1 = -5
    @b1 = 2**70
    # TODO: Add edge cases along Fixnum/Bignum border!!
    @z1 = GMP::Z(12)
  end

  # 01 mpz_t__mpz_t_or_ui__to__mpz_t__returns__void
  def test_FUNC_MAP__Z_ZUI__TO__Z__RETURNS__VOID
    functions = [:add, :addmul, :submul, :divexact, :lcm]
    rop = GMP::Z(0)
    op1s = [@z1]
    op2s = [@xp1, @xp2, @xn1, @b1, @z1]
    functions.each do |f|
      op1s.each do |op1|
        op2s.each do |op2|
          assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{rop.class}, #{op1.class}, #{op2.class})") {
            GMP::Z.send(f, rop, op1, op2)
          }
        end
      end
    end
  end
  
  # 02 mpz_t_or_ui__mpz_t_or_ui__to__mpz_t__returns__void
  def test_FUNC_MAP__ZUI_ZUI__TO__Z__RETURNS__VOID
    functions = [:sub]
    rop = GMP::Z(0)
    op1s = [@xp1, @xp2, @xn1, @b1, @z1]
    op2s = [@xp1, @xp2, @xn1, @b1, @z1]
    functions.each do |f|
      op1s.each do |op1|
        op2s.each do |op2|
          assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{rop.class}, #{op1.class}, #{op2.class})") {
            GMP::Z.send(f, rop, op1, op2)
          }
        end
      end
    end
  end
  
  # 03 mpz_t__mpz_t_or_si_or_ui__to__mpz_t__returns__void
  def test_FUNC_MAP__Z_ZSIUI__TO__Z__RETURNS__VOID
    functions = [:mul]
    rop = GMP::Z(0)
    op1s = [@z1]
    op2s = [@xp1, @xp2, @xn1, @b1, @z1]
    functions.each do |f|
      op1s.each do |op1|
        op2s.each do |op2|
          assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{rop.class}, #{op1.class}, #{op2.class})") {
            GMP::Z.send(f, rop, op1, op2)
          }
        end
      end
    end
  end
  
  # 04 mpz_t__mp_bitcnt_t__to__mpz_t__returns__void
  def test_FUNC_MAP__Z_BITCNT__TO__Z__RETURNS__VOID
    functions = [:mul_2exp, :cdiv_q_2exp, :cdiv_r_2exp, :fdiv_q_2exp, :fdiv_r_2exp, :tdiv_q_2exp, :tdiv_r_2exp]
    rop = GMP::Z(0)
    op1s = [@z1]
    functions.each do |f|
      op1s.each do |op1|
        op2s = [@xp1, @xp2]
        op2s.each do |op2|
          assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{rop.class}, #{op1.class}, #{op2.class})") {
            GMP::Z.send(f, rop, op1, op2)
          }
        end
        op2s = [@xn1]
        op2s.each do |op2|
          assert_raise(RangeError, "GMP::Z.#{f.to_s} should raise a RangeError when passed (#{rop.class}, #{op1.class}, #{op2.class})") {
            GMP::Z.send(f, rop, op1, op2)
          }
        end
      end
    end
  end
  
  # 05 mpz_t__to__mpz_t__returns__void
  def test_FUNC_MAP__Z__TO__Z__RETURNS__VOID
    functions = [:neg, :abs, :sqrt, :nextprime, :com]
    rop = GMP::Z(0)
    op1s = [@z1]
    functions.each do |f|
      op1s.each do |op1|
        assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{rop.class}, #{op1.class})") {
          GMP::Z.send(f, rop, op1)
        }
      end
    end
  end
  
  # 09 mpz_t__mpz_t_or_ui__to__none__returns__int
  def test_FUNC_MAP__Z_Z__TO__VOID__RETURNS__BOOL
    functions = [:divisible?]
    op1s = [@z1]
    functions.each do |f|
      op1s.each do |op1|
        op2s = [@xp1, @xp2, @xn1, @b1, @z1]
        op2s.each do |op2|
          assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{op1.class}, #{op2.class})") {
            GMP::Z.send(f, op1, op2)
          }
        end
      end
    end
  end
  
  # 11 mpz_t__mpz_t_or_ui__mpz_t_or_ui__to__none__returns__int
  def test_FUNC_MAP__Z_ZXB_ZXB__TO__VOID__RETURNS__BOOL
    functions = [:congruent?]
    op1s = [@z1]
    functions.each do |f|
      op1s.each do |op1|
        op2s = [@xp1, @xp2, @xn1, @b1, @z1]
        op2s.each do |op2|
          op3s = [@xp1, @xp2, @xn1, @b1, @z1]
          op3s.each do |op3|
            assert_nothing_raised("GMP::Z.#{f.to_s} should not raise when passed (#{op1.class}, #{op2.class}, #{op3.class})") {
              GMP::Z.send(f, op1, op2, op3)
            }
          end
        end
      end
    end
  end
end
