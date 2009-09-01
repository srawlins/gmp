#!/usr/bin/env ruby

require 'gmp'

a=GMP::Q.new(200,11)
b=-a
c=a.floor
d=a.ceil
e=a.trunc
f=b.floor
g=b.ceil
h=b.trunc

[c,d,e,f,g,h].each { |var| p var }
