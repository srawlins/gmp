#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(100)
b=GMP::Z.new(200)
c=GMP::Q.new(100,11)
d=GMP::Q.new(200,17)

[a,b,c,d].each { |var| p var }

a.swap b
c.swap d

[a,b,c,d].each { |var| p var }
