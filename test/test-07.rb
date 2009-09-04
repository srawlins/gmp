#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=GMP::Z.new(16)
c=a**5
d=a**b
e=2**b
f=a.powmod(2,256)
g=b.powmod(10,a)
h=a.powmod(b,256)
i=a.powmod(b,c)

[a,b,c,d,e,f,g,h,i].each { |var| p var }
