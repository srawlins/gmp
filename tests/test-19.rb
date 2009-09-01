#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(5)
b=GMP::Z.new(7)
c=GMP::Q.new(3,11)
d=GMP::F.new(3.14)
e=2**32

f=[a/b,a/3,3/a,a/c,c/a,a/d,d/a,a/e,e/a]

f.each {|var| p var}
