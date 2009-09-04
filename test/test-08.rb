#!/usr/bin/env ruby

require 'gmp'

a=GMP::Z.new(10)
b=GMP::Z.new()
c=GMP::Q.new(1)
d=GMP::Q.new()

begin
    a.tdiv(0)
rescue ZeroDivisionError
    print "Divide by zero 1\n"
end

begin
    a.tdiv(b)
rescue ZeroDivisionError
    print "Divide by zero 2\n"
end

begin
    d.inv
rescue ZeroDivisionError
    print "Divide by zero 3\n"
end

begin
    d.inv!
rescue ZeroDivisionError
    print "Divide by zero 4\n"
end

begin
    c/d
rescue ZeroDivisionError
    print "Divide by zero 5\n"
end

begin
    c/0
rescue ZeroDivisionError
    print "Divide by zero 6\n"
end
