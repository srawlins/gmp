#!/usr/bin/env ruby

require 'mkmf'

dir_config('gmp');
dir_config('mpfr');

ok = true
unless have_header('gmp.h')
  $stderr.puts "can't find gmp.h, try --with-gmp-include=<path>"
  ok = false
end

unless have_library('gmp', '__gmpz_init')
  $stderr.puts "can't find -lgmp, try --with-gmp-lib=<path>"
  ok = false
end

if have_header('mpfr.h') and
    have_header('mpf2mpfr.h') and
    have_library('mpfr', 'mpfr_init') then
  $CFLAGS += ' -DMPFR'
end

$CFLAGS += ' -Wall -W -O6 -g'
if ok then
  create_makefile('gmp');
else
  raise "Unable to build, correct above errors and rerun"
end
