#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=GMP::Z.new(27)
c=GMP::Z.new(99)

d=[a.even?, b.even?, c.even?]
e=[a.odd?, b.odd?, c.odd?]
f=[a.square?, b.square?, c.square?]
g=[a.power?, b.power?, c.power?]
h=a.sqrtrem
i=b.sqrtrem
j=c.sqrtrem
k=[a.sqrt, b.sqrt, c.sqrt]
l=[a.root(3), b.root(3), c.root(3)]

[d,e,f,g,h,i,j,k,l].each { |var| p var }
