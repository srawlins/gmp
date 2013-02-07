require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class GMP_TRoot < Test::Unit::TestCase
  RS = GMP::RandState.new(11213)
  Reps = 500

  def one_test(root1, x2, nth, i)
    root2, rem2 = x2.rootrem(nth)
    temp = root1 ** nth
    temp2 = temp + rem2

    assert_equal(root1, root2, "rootrem should produce the correct root for: #{x2}.rootrem(#{nth})")
    assert_equal(x2,    temp2, "rootrem should produce the correct remainder for: #{x2}.rootrem(#{nth})")

    if nth > 1
      assert_false((temp > 1  &&  ! temp.power?), "error in perfect_power?")
    end

    if nth <= 10_000 && x2 > 0
      temp2 = root1 + 1
      temp2 = temp2 ** nth

      # Is square of (result + 1) <= argument?
      assert_true(temp2 > x2, "square of (result + 1) should not be less than or equal to argument")
    end
  end

  def test_root
    bs = GMP::Z()
    s2 = GMP::Z()
    root1 = GMP::Z()

    x2 = GMP::Z("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff80000000000000000000000000000000000000000000000000000000000000002", 16)
    root1 = x2.root(2)
    one_test(root1, x2, 2, -1)

    (0...Reps).each do |i|
      size = RS.urandomb(32)
      size = RS.urandomb((size % 17 + 2).to_i)
      x2 = RS.rrandomb(size.to_i + 10)

      size = RS.urandomb(15)
      #nth = mpz_getlimbn (bs, 0) % mpz_sizeinbase (x2, 2) + 2;
      nth = (size % 32).to_i % x2.size_in_bin + 2
      root1 = x2.root(nth)

      size = RS.urandomb(4)

      # With 50% probability, set x2 near a perfect power.
      one_test(root1, x2, nth, i)
      if (nth & 1 != 0)  &&  (size & 2 != 0)
        x2.neg!
        root1.neg!
        one_test(root1, x2, nth, i)
      end
    end
  end
end
