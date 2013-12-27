require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TC_F_to_s < Test::Unit::TestCase
  def setup
    @initial_default_prec = GMP::F.default_prec
  end

  def teardown
    GMP::F.default_prec = @initial_default_prec
  end

  def test_to_s_default_prec
    rs = GMP::RandState.new(11213)
    if GMP.const_defined? :MPFR_VERSION
      strings = [
        "0.25352409199307085e+0",     "0.63701645188411673e+1",     "0.11775590017886098e+0",
        "0.48237355226726111e+1",     "0.77264809773028509e+0",     "0.69469939456849605e+0",
        "0.49355516375037611e+0",     "0.10614725492864392e+1",     "0.68528515884539964e+0"
      ]
    else
      strings = [
        "0.253524091993070886469e+0", "0.637016451884116743191e+1", "0.117755900178860983509e+0",
        "0.482373552267261146111e+1", "0.772648097730285124905e+0", "0.694699394568496142094e+0",
        "0.493555163750376091773e+0", "0.10614725492864391615e+1",  "0.685285158845399685616e+0"
      ]
    end
    # In this test, each numerator, n, and denominator, d, pair is generated in
    # succession from rs. Each pair is used to create a floating point number.
    # The first pair are uniformly distributed random integers between 0 and
    # 2^100 - 1. The next two are distributed between 0 and 2^110 - 1. The last
    # are distributed between 0 and 2^190 - 1.
    (0..8).each do |i|
      n = GMP::F(rs.urandomb(10 * (i+10)), 53)
      d = GMP::F(rs.urandomb(10 * (i+10)), 53)
      assert_equal(strings[i], (n/d).to_s, "GMP::F#to_s should be correct for default precision numbers.")
    end
  end

  def test_to_s_bigger_prec
    return if not GMP.const_defined? :MPFR_VERSION
    rs  = GMP::RandState.new(19937)
    rs2 = GMP::RandState.new(21701)
    if GMP.const_defined? :MPFR_VERSION
      strings = [
        "0.655393663692186710105e-2",               "0.36430999484273987746204e+4",                "0.11976788674896651459533244e+3",
        "0.2606381328092210487064345210e+0",        "0.977167229464647196661643216645e-2",         "0.755006431167835276749654244022956e-1",
        "0.41292361235767736087211972667683449e+1", "0.41857769284263224368201967544187408820e-2", "0.5145285970595521572673546620320724197645e-3"
      ]
    else # todo
    end

    (0..8).each do |i|
      GMP::F.default_prec = 64+(8*i)
      n = GMP::F(rs.urandomb(144+rs2.urandomb(4).to_i))
      d = GMP::F(rs.urandomb(144+rs2.urandomb(4).to_i))
      assert_equal(strings[i], (n/d).to_s, "GMP::F#to_s should be correct for different precision numbers.")
    end
  end

  def test_different_bases
    f = GMP::F(0.5)
    g = GMP::F(0.8)

    if GMP.const_defined? :MPFR_VERSION
      assert_equal("0.50000000000000000e+0",                                     f.to_s)
      assert_equal("0.10000000000000000000000000000000000000000000000000000e+0", f.to_s(2))
      assert_equal("0.10000000000000000000000000000000000000000000000000000e+0", f.to_s(:bin))
      assert_equal("0.11111111111111111111111111111111112e+0",                   f.to_s(3))
      assert_equal("0.200000000000000000000000000e+0",                           f.to_s(4))
      assert_equal("0.50000000000000000e+0",                                     f.inspect)

      assert_equal("0.80000000000000004e+0",                   g.to_s)
      assert_equal("0.21012101210121012101210121012101220e+0", g.to_s(3))
      assert_equal("0.400000000000000000000003e+0",            g.to_s(5))
      assert_equal("0.k00000000002ge+0",                       g.to_s(25))
      assert_equal("0.80000000000000004e+0",                   g.inspect)
    else
      assert_equal("0.5e+0",                                          f.to_s)
      assert_equal("0.1e+0",                                          f.to_s(2))
      assert_equal("0.1e+0",                                          f.to_s(:bin))
      assert_equal("0.111111111111111111111111111111111111111111e+0", f.to_s(3))
      assert_equal("0.2e+0",                                          f.to_s(4))
      assert_equal("0.5e+0",                                          f.inspect)

      assert_equal("0.800000000000000044409e+0",                     g.to_s)
      assert_equal("0.21012101210121012101210121012101212211012e+0", g.to_s(3))
      assert_equal("0.40000000000000000000000231042e+0",             g.to_s(5))
      assert_equal("0.k00000000002g49e+0",                           g.to_s(25))
      assert_equal("0.800000000000000044409e+0",                     g.inspect)
    end
  end
end
