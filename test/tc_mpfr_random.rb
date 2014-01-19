require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcMpfrRandom < Test::Unit::TestCase
  def setup
  end

  def test_urandomb
    a = GMP::RandState.new
    a.seed(577)
    if (GMP::MPFR_VERSION_MAJOR == 3 and GMP::MPFR_VERSION_MINOR < 1) or (GMP::MPFR_VERSION_MAJOR < 3)
      g1 = [
        GMP::F("0.39810885576093713"), GMP::F("0.97212443610368071"), GMP::F("0.23084385480845748"),
        GMP::F("0.29428636987062717"), GMP::F("0.76895428585342840"), GMP::F("0.69329332764704654"),
        GMP::F("0.60252711974045448"), GMP::F("0.03530920538566051"), GMP::F("0.60242124958975374")
      ]
    else # GMP::MPFR_VERSION >= 3.1
      g1 = [
        GMP::F("0.32693659839925215"), GMP::F("0.91084514033812047"), GMP::F("0.76821464772097192"),
        GMP::F("0.69848549504452184"), GMP::F("0.81837742782145961"), GMP::F("0.86473502115143552"),
        GMP::F("0.97554122845083246"), GMP::F("0.31325262983287028"), GMP::F("0.75871915981575699")
      ]
    end
    g1.size.times do |i|
      assert_in_delta(g1[i], a.mpfr_urandomb, 1e-12, "GMP::RandState should mpfr_urandomb predictably.")
    end
  end

  def test_urandomb_with_prec
    return unless GMP::MPFR_VERSION_MAJOR >= 3 and GMP::MPFR_VERSION_MINOR >= 1

    a = GMP::RandState.new
    a.seed(577)
    g1 = [
      GMP::F("0.74120618845", 32), GMP::F("0.39810885559", 32), GMP::F("0.70775037003", 32),
      GMP::F("0.97212443594", 32), GMP::F("0.88489733171", 32), GMP::F("0.23084385460", 32),
      GMP::F("0.25290360907", 32), GMP::F("0.29428636981", 32), GMP::F("0.85951062967", 32)
    ]

    g1.size.times do |i|
      assert_in_delta(g1[i], a.mpfr_urandomb(32), 1e-12, "GMP::RandState should mpfr_urandomb with a precision predictably.")
    end
  end

  def test_urandom
    return unless GMP::MPFR_VERSION_MAJOR >= 3

    a = GMP::RandState.new
    a.seed(577)

    g1 = [
      GMP::F("0.48637900307389631"), GMP::F("0.94659148733322840"), GMP::F("0.27564210151536123"),
      GMP::F("0.25908603728464119"), GMP::F("0.54208688851416087"), GMP::F("0.45824186569746128e-1"),
      GMP::F("0.52706696601697367"), GMP::F("0.10260542184139945"), GMP::F("0.38678438390336384")
    ]
    g1.size.times do |i|
      assert_in_delta(g1[i], a.mpfr_urandom, 1e-12, "GMP::RandState should mpfr_urandom predictably.")
    end
  end

  def test_urandom_with_prec
    return unless GMP::MPFR_VERSION_MAJOR >= 3

    a = GMP::RandState.new
    a.seed(577)

    g1 = [
      GMP::F("0.44905442791", 32), GMP::F("0.88489733171", 32), GMP::F("0.39714318502", 32),
      GMP::F("0.19476944383", 32), GMP::F("0.30126355996", 32), GMP::F("0.22586148366", 32),
      GMP::F("0.67203788180", 32), GMP::F("0.11863605876", 32), GMP::F("0.54191642837", 32)
    ]
    g1.size.times do |i|
      assert_in_delta(g1[i], a.mpfr_urandom(GMP::GMP_RNDN, 32), 1e-12, "GMP::RandState should mpfr_urandom predictably.")
    end

    a = GMP::RandState.new
    a.seed(577)

    g2 = [
      GMP::F("0.44905442779", 32), GMP::F("0.97212443594", 32), GMP::F("0.73084385460", 32),
      GMP::F("0.39714318491", 32), GMP::F("0.38447714283", 32), GMP::F("0.86661665933e-1", 32),
      GMP::F("0.30126355984", 32), GMP::F("0.53530920530", 32), GMP::F("0.60242124950", 32)
    ]
    g2.size.times do |i|
      assert_in_delta(g2[i], a.mpfr_urandom(GMP::GMP_RNDZ, 32), 1e-12, "GMP::RandState should mpfr_urandom predictably.")
    end
  end

  def test_reseed
    @c = GMP::RandState.new
    if (GMP::MPFR_VERSION_MAJOR == 3 and GMP::MPFR_VERSION_MINOR < 1) or
       (GMP::MPFR_VERSION_MAJOR < 3)
      @c.seed(1000)
      assert_in_delta(GMP::F("0.78522728792921048"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.94422653925308231"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.60678541956109799"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      @c.seed(1000)
      assert_in_delta(GMP::F("0.78522728792921048"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.94422653925308231"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.60678541956109799"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      @c.seed(1000)
      assert_in_delta(GMP::F("0.78522728792921048"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.94422653925308231"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.60678541956109799"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    else
      @c.seed(1000)
      assert_in_delta(GMP::F("0.14548567902310305"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.77595239031275598"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.69653926112870401"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      @c.seed(1000)
      assert_in_delta(GMP::F("0.14548567902310305"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.77595239031275598"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.69653926112870401"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      @c.seed(1000)
      assert_in_delta(GMP::F("0.14548567902310305"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.77595239031275598"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
      assert_in_delta(GMP::F("0.69653926112870401"), @c.mpfr_urandomb, 1e-12, "GMP::RandState should re-seed correctly.")
    end
  end

  def test_random_independent_states
    @d = GMP::RandState.new
    @d.seed(577)
    @e = GMP::RandState.new
    @e.seed(577)

    if (GMP::MPFR_VERSION_MAJOR == 3 and GMP::MPFR_VERSION_MINOR < 1) or
       (GMP::MPFR_VERSION_MAJOR < 3)
      assert_in_delta(GMP::F("0.39810885576093713"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.97212443610368071"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.39810885576093713"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.97212443610368071"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.23084385480845748"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.23084385480845748"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
    else
      assert_in_delta(GMP::F("0.32693659839925215"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.91084514033812047"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.32693659839925215"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.91084514033812047"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.76821464772097192"), @d.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
      assert_in_delta(GMP::F("0.76821464772097192"), @e.mpfr_urandomb, 1e-12, "GMP::RandState should be independent correctly.")
        #GMP::F("0.32693659839925215"), GMP::F("0.91084514033812047"), GMP::F("0.76821464772097192"),
    end
  end
end
