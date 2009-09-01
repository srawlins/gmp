#!/usr/bin/env ruby

a=1234
b=5678

(1..100000).each { |i|
    a+=b
    b+=a
}

# print a,"\n","----\n",b,"\n"
