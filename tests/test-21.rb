#!/usr/bin/env ruby

require 'gmp'

pi = GMP::F.new(1).asin * 2

a = GMP::F.new 0
b = pi / 4
c = pi / 2
d = -pi / 2

print "cos:\n"
[a,b,c,d].map{|x| p x.cos}

print "sin:\n"
[a,b,c,d].map{|x| p x.sin}

print "tan:\n"
[a,b,c,d].map{|x| p x.tan}

print "cosh:\n"
[a,b,c,d].map{|x| p x.cosh}

print "sinh:\n"
[a,b,c,d].map{|x| p x.sinh}

print "tanh:\n"
[a,b,c,d].map{|x| p x.tanh}


print "inverses:\n"
[a,b,c,d].map{|x| p [x,x.cos.acos] }
[a,b,c,d].map{|x| p [x,x.sin.asin] }
[a,b,c,d].map{|x| p [x,x.tan.atan] }
[a,b,c,d].map{|x| p [x,x.cosh.acosh] }
[a,b,c,d].map{|x| p [x,x.sinh.asinh] }
[a,b,c,d].map{|x| p [x,x.tanh.atanh] }
