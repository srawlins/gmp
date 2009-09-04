#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(0xffff00000000ffff)
b=GMP::Z.new(0xffff0000ffff0000)
c=0xff00ff00ff00ff00
# [Z op Z, Z op FixNum, Z op BigNum, FixNum op Z, BigNum op Z]
d=[a & b, a & 0x1248, a & c, 0x1248 & a, c & a]
e=[a | b, a | 0x1248, a | c, 0x1248 | a, c | a]
f=[a ^ b, a ^ 0x1248, a ^ c, 0x1248 ^ a, c ^ a]
g=a.com

[d,e,f,a,g].each { |var| p var }
