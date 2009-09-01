#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=GMP::Z.fib(100)
c=GMP::Z.fib(a)
d=GMP::Z.fac(100)
e=GMP::Z.fac(a)
f=b.nextprime

[b,c,d,e,f].each { |var| p var }