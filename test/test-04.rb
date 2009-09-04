#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(300)
b=GMP::Z.new(400)
c=GMP::Q.new(2000,11)
d=GMP::Q.new(3000,17)
e=700
f=2**32

g=[a>b, a>=b, a<b, a<=b, a==b, a==a, a<=>b]
h=[c>d, c>=d, c<d, c<=d, c==d, c==d, c<=>d]
i=[a<=>c, c<=>a]
j=[a<=>e, e<=>a, a<=>f, f<=>a]
k=[c<=>e, e<=>c, c<=>f, f<=>c]

[g,h,i,j,k].each { |var| p var }
