#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=GMP::Z.new(200)
c=2**32
# [Z op Z, Z op FixNum, Z op BigNum, FixNum op Z, BigNum op Z]
d=[a+b, a+2, a+c, 2+a, c+a]
e=[a-b, a-2, a-c, 2-a, c-a]
f=[a*b, a*2, a*c, 2*a, c*a]

[d,e,f].each { |var| p var }
