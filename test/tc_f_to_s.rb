require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TC_F_to_s < Test::Unit::TestCase
  def setup
    @rs = GMP::RandState.new(11213)
  end

  def test_to_s
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
    # succession from @rs. Each pair is used to create a floating point number.
    # The first pair are uniformly distributed random integers between 0 and
    # 2^100 - 1. The next two are distributed between 0 and 2^110 - 1. The last
    # are distributed between 0 and 2^190 - 1.
    (0..8).each do |i|
      n = GMP::F(@rs.urandomb(10 * (i+10)), 53)
      d = GMP::F(@rs.urandomb(10 * (i+10)), 53)
      assert_equal(strings[i], (n/d).to_s, "GMP::F#to_s should be correct for default precision numbers.")
    end
  end
end
