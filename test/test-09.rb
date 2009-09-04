#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(10)
b=GMP::Z.new()
c=GMP::Z.new(-10)
d=GMP::Q.new(10)
e=GMP::Q.new()
f=GMP::Q.new(-10)
k=GMP::Z.new(10)
l=GMP::Z.new()
m=GMP::Z.new(-10)
n=GMP::Q.new(10)
o=GMP::Q.new()
p=GMP::Q.new(-10)

g = [a.sgn, b.sgn, c.sgn, d.sgn, e.sgn, f.sgn]
h = [-a, -b, -c, -d, -e, -f]
i = [a.abs, b.abs, c.abs, d.abs, e.abs, f.abs]
a.abs!; b.abs!; c.abs!; d.abs!; e.abs!; f.abs!
j = [a, b, c, d, e, f]
k.neg!; l.neg!; m.neg!; n.neg!; o.neg!; p.neg!
q =  [k, l, m, n, o, p]

[g,h,i,q].each { |var| p var }
