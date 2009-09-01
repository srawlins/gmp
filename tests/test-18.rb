#!/usr/bin/env ruby

require 'gmp'

p GMP::F.default_prec
GMP::F.default_prec=100
p GMP::F.default_prec
