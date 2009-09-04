#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(0x1234)
b=GMP::Z.new(a)
b[0]=true
c=a[2]
d=a.scan0(0)
e=a.scan1(0)

[a,b,c,d,e].each { |var| p var }
