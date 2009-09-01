#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(1000)
b=GMP::Z.new(1000)

(1..12).each { |i|
    a*=b
    b*=a
}

# print a,"\n","----\n",b,"\n"
