#!/usr/bin/env ruby

require 'gmp'

a=GMP::F.new -1
b=GMP::F.new 0
c=GMP::F.new 0.1
d=GMP::F.new 1
e=GMP::F.new 2
f=GMP::F.new 4
g=GMP::F.new 10

print "exp:\n"
[a,b,c,d,e,f,g].map{|x| p x.exp}

print "log:\n"
[c,d,e,f,g].map{|x| p x.log}

print "log2:\n"
[c,d,e,f,g].map{|x| p x.log2}

print "log10:\n"
[c,d,e,f,g].map{|x| p x.log10}

print "log1p:\n"
[c,d,e,f,g].map{|x| p x.log1p}

print "expm1:\n"
[a,b,c,d,e,f,g].map{|x| p x.expm1}
