#!/usr/bin/env ruby

require 'gmp'

a = GMP::F.new 256
b = GMP::F.new 10
c = GMP::F.new 0.5

[a,b].map{|x| p x.sqrt }
[a,b].map{|x| p x ** c }

