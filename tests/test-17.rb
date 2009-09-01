#!/usr/bin/env ruby

require 'gmp'

a=GMP::F.new(3.14, 100)
b=GMP::F.new(2.71, 200)
c=GMP::Z(3)
d=GMP::Q(7,2)
e=2**32

f=[a+b,b+a,a+c,c+a,a+d,d+a,a+2,2+a,a+e,e+a,a+4.51,4.51+a]
g=[a-b,b-a,a-c,c-a,a-d,d-a,a-2,2-a,a-e,e-a,a-4.51,4.51-a]
h=[a*b,b*a,a*c,c*a,a*d,d*a,a*2,2*a,a*e,e*a,a*4.51,4.51*a]
i=[a/b,b/a,a/c,c/a,a/d,d/a,a/2,2/a,a/e,e/a,a/4.51,4.51/a]

[f,g,h,i].each { |var|
    var.each { |var2|
	print var2, "(", var2.prec, " bits of precision)\n"
    }
}
