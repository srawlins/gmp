class Bignum
  def []=(n, v)
    if self[n] == 1
      return ( v ? self : self - (2**n) )
    else
      return ( v ? self | (2**n) : self )
    end
  end

  #def gcd(b)
  #  a = self
  #  while b != 0
  #    a, b = b, a % b
  #  end
  #  return a
  #end

  # Borrowed from https://gist.github.com/2388745
  # Returns an array of the form `[gcd(x, y), a, b]`, where
  # `ax + by = gcd(x, y)`.
  #
  # @param [Integer] x
  # @param [Integer] y
  # @return [Array<Integer>]
  def self.gcdext(x, y)
    if x < 0
      g, a, b = self.gcdext(-x, y)
      return [g, -a, b]
    end
    if y < 0
      g, a, b = self.gcdext(x, -y)
      return [g, a, -b]
    end
    r0, r1 = x, y
    a0 = b1 = 1
    a1 = b0 = 0
    until r1.zero?
      q = r0 / r1
      r0, r1 = r1, r0 - q*r1
      a0, a1 = a1, a0 - q*a1
      b0, b1 = b1, b0 - q*b1
    end
    [r0, a0, b0]
  end

  # Borrowed from https://gist.github.com/2388745
  # Returns the inverse of `num` modulo `mod`.
  #
  # @param [Integer] num the number
  # @param [Integer] mod the modulus
  # @return [Integer]
  # @raise ZeroDivisionError if the inverse of `base` does not exist
  def self.invert(num, mod)
    g, a, b = self.gcdext(num, mod)
    unless g == 1
      raise ZeroDivisionError.new("#{num} has no inverse modulo #{mod}")
    end
    a % mod
  end

  # Borrowed from https://gist.github.com/2388745
  # Returns `base` raised to `exp` modulo `mod`.
  #
  # @param [Integer] base the base
  # @param [Integer] exp the exponent
  # @param [Integer] mod the modulus
  # @return [Integer]
  # @raise ZeroDivisionError if the `exp` is negative and the inverse
  #   of `base` does not exist
  def self.powmod(base, exp, mod)
    if exp < 0
      base = self.invert(base, mod)
      exp = -exp
    end
    result = 1
    multi = base % mod
    until exp.zero?
      result = (result * multi) % mod if exp.odd?
      exp >>= 1
      multi = (multi * multi) % mod
    end
    result
  end
end
