require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class MPFR_TCONST_EULER < MiniTest::Test
  def test_const_euler
    prec = 53

    y = GMP::F.const_euler(GMP::GMP_RNDN, 32)
    z = GMP::F("0.10010011110001000110011111100011", 32, 2)
    assert_equal(z, y, "Const Euler to precision 32 should be accurate.")

    (2..200).each do |prec|
      yprec = prec + 10

      (RND_MODES).each do |rnd|
        y = GMP::F.const_euler(rnd, yprec)
        err = rnd == GMP::GMP_RNDN ? yprec+1 : yprec

        if y.can_round?(err, rnd, rnd, prec)
          t = GMP::F(y, prec, rnd)
          z = GMP::F.const_euler(rnd, prec)
          assert_equal(t, z, "const_euler should be equal at prec=#{prec}, rnd_mode=#{rnd.inspect}")
        end
      end
    end
  end

  def test_bad_prec
    assert_raise(TypeError) { GMP::F.const_euler(GMP::GMP_RNDN, "not a precision") }
  end
end
