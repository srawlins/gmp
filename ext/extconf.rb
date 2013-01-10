#!/usr/bin/env ruby

require 'mkmf'

dir_config('gmp')
dir_config('mpfr')

use_if_mpfr = ! ARGV.include?('--no-mpfr')
ok = true
unless have_header('gmp.h')
  $stderr.puts "can't find gmp.h, try --with-gmp-include=<path>"
  ok = false
end

unless have_library('gmp', '__gmpz_init')
  $stderr.puts "can't find -lgmp, try --with-gmp-lib=<path>"
  ok = false
end

if use_if_mpfr
if (have_header('mpfr.h') and
    have_header('mpf2mpfr.h') and
    have_library('mpfr', 'mpfr_init'))
  $CFLAGS += ' -DMPFR'
end
end

unless have_macro('SIZEOF_INTPTR_T')
  check_sizeof('intptr_t', 'ruby.h')
end

# Need check for which ruby vm to see how to require various things
if (begin; Rubinius; rescue NameError; end) != nil
  $CFLAGS += ' -DRUBY_ENGINE=rbx'
end

if (begin; JRuby; rescue NameError; end) != nil
  $CFLAGS += ' -DRUBY_ENGINE=jruby'
end


$CFLAGS += ' -Wall -W -O6 -g'
if ok
  create_makefile('gmp')
else
  raise "Unable to build, correct above errors and rerun"
end

