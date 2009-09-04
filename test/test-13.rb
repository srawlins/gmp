#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=GMP::Z.pow(2,32)
c=GMP::Q.new(200,11)

d=a.to_i
e=b.to_i
f=a.to_d
g=b.to_d
h=c.to_d

[d,e,f,g,h].each { |var| p var }
