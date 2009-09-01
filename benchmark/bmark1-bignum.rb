#!/usr/bin/env ruby

a=1000
b=1000

(1..12).each { |i|
    a*=b
    b*=a
}

# print a,"\n","----\n",b,"\n"
