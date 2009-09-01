#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new()
b=GMP.Z(1)
c=GMP::Z.new(b)
d=GMP.Z("1")
e=GMP::Z.new(2**32)

f=GMP::Q.new()
g=GMP.Q(1)
h=GMP::Q.new(1,2)
i=GMP.Q("1/2")
j=GMP::Q.new(h)

[a,b,c,d,e,f,g,h,i,j].each { |var| p var }
