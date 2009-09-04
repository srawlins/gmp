#!/usr/bin/env ruby

require 'gmp'

a=GMP::Q.new(100,11)
b=GMP::Q.new(200,17)
c=GMP::Z.new(40)
d=2**32
# [Q op Q, Q op Z, Z op Q, Q op FixNum, Q op BigNum, FixNum op Q, BigNum op Q]
e=[a+b, a+c, c+a, a+2, a+d, 2+a, d+a]
f=[a-b, a-c, c-a, a-2, a-d, 2-a, d-a]
g=[a*b, a*c, c*a, a*2, a*d, 2*a, d*a]

[e,f,g].each { |var| p var }
