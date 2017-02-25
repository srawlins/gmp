require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class GMP_TCong < MiniTest::Test
  Data = [
    # strict equality mod 0
    [ "0",  "0", "0", true],
    ["11", "11", "0", true],
    [ "3", "11", "0", false],

    # anything congruent mod 1
    [                   "0",                    "0", "1", true ],
    [                   "1",                    "0", "1", true ],
    [                   "0",                    "1", "1", true ],
    [                 "123",                  "456", "1", true ],
    ["0x123456789123456789", "0x987654321987654321", "1", true ],

    # csize==1, dsize==2 changing to 1 after stripping 2s
    [                "0x3333333333333333",         "0x33333333",         "0x180000000", true],
    ["0x33333333333333333333333333333333", "0x3333333333333333", "0x18000000000000000", true],

    # another dsize==2 becoming 1, with opposite signs this time */
    [        "0x444444441",         "-0x22222221F",         "0x333333330", true],
    ["0x44444444444444441", "-0x2222222222222221F", "0x33333333333333330", true]
  ]
  RS = GMP::RandState.new(11213)
  Reps = 2_000  # The original was 10_000 but that took like 5 seconds

  def test_congruent()
    Data.each do |a, c, d, want|
      got = GMP::Z(a).congruent?(GMP::Z(c), GMP::Z(d))
      assert_equal(want, got, "congruent? should produce the correct result for #{a}, #{c}, #{d}")
    end

    Reps.times do
      size = RS.urandomb(32)
      size = RS.urandomb((size % 16 + 1).to_i)
      a = RS.rrandomb(size.to_i)

      size = RS.urandomb(32)
      size = RS.urandomb((size % 16 + 1).to_i)
      c = RS.rrandomb(size.to_i)

      d = 0
      until d != 0
        size = RS.urandomb(32)
        size = RS.urandomb((size % 16 + 1).to_i)
        d = RS.rrandomb(size.to_i)
      end

      a = negrandom(a)
      c = negrandom(c)
      d = negrandom(d)

      ra = a.fmod d
      rc = c.fmod d
      want = (ra <=> rc) == 0

      got = GMP::Z(a).congruent?(GMP::Z(c), GMP::Z(d))
      assert_equal(want, got, "congruent? should produce the correct result for #{a}, #{c}, #{d}")

      ra = ra - rc
      a  =  a - ra

      got = GMP::Z(a).congruent?(GMP::Z(c), GMP::Z(d))
      assert_equal(true, got, "congruent? should produce the correct result for #{a}, #{c}, #{d}")
    end
  end

  def negrandom(z)
    return (RS.urandomb(1) != 0) ? z.neg : z
  end
end
