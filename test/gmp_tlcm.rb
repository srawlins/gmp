require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class GMP_TLCM < Test::Unit::TestCase
  def setup
    Primes = [
      2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
      101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,
      191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,
      281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,
      389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,
    ]
  end

  def the_test()
    want = 1
    Primes.each do |y|
      x = want
      want = want * y
      check(want, x, y)
    end

    x = want
    Primes.each do |y|
      check(want, x, y)
    end

    want = Primes[0]
    (1..NumPrimes).each do |i|
      x = want
      y = Primes[i] * Primes[i-1]
      want = want * Primes[i]
      check(want, x, y)
    end

    want = 1
    x     = 1
    y     = 1
    for (i=0; i+2 < NumPrimes; i += 3)
      want = want * Primes[i]
      want = want * Primes[i+1]
      want = want * Primes[i+2]

      x = x * Primes[i]
      x = x * Primes[i+1]

      y = y * Primes[i+1]
      y = y * Primes[i+2]

      check(want, x, y)
    end
  end
end
