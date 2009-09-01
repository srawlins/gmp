#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=-a

c=a>>5
d=a.lastbits_pos(5)
e=a.tshr 5
f=a.lastbits_sgn(5)
g=b>>5
h=b.lastbits_pos(5)
i=b.tshr 5
j=b.lastbits_sgn(5)
k=a<<2
l=b<<2

[c,d,e,f,g,h,i,j,k,l].each { |var| p var }
