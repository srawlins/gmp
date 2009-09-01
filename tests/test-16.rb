#!/usr/bin/env ruby

require 'gmp'

a=GMP::F.new()
b=GMP::F.new(3.14)
c=GMP::F.new(1)
d=GMP::F.new("3.14")
e=GMP::F.new(2**32)
f=GMP::F.new(GMP::Z.new(3))
g=GMP::F.new(GMP::Q.new(7,2))
h=GMP::F.new(b)
i=GMP::F.new(3.14, 1000)
j=GMP::F.new(1, 1000)
k=GMP::F.new("3.14", 1000)
l=GMP::F.new(2**32, 1000)
m=GMP::F.new(GMP::Z.new(3), 1000)
n=GMP::F.new(GMP::Q.new(7,2), 1000)
o=GMP::F.new(n)
p=GMP::F.new(b, 1000)
q=GMP::F.new(n, 0)

[a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q].each { |var|
    print var, "(", var.prec, " bits of precision)\n"
}
