#!/usr/bin/env ruby

require 'gmp'

a = GMP::F.new 0
b = GMP::F.new 1
c = b / a
d = -c
e = a * c

[a,b,c,d,e].map{|x| p [x, x.finite?, x.infinite?] }
[a,b,c,d,e].map{|x| p [x, x.number?, x.nan?] }
