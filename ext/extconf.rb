#!/usr/bin/env ruby

require 'mkmf'

# Workaround for Ruby bug #8074, introduced in Ruby 2.0.0, fixed in Ruby 2.1.0
# https://bugs.ruby-lang.org/issues/8074
@libdir_basename = "lib"

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
  $CFLAGS += ' -DRUBY_ENGINE_RBX'
end

if (begin; JRuby; rescue NameError; end) != nil
  $CFLAGS += ' -DRUBY_ENGINE_JRUBY'
end


# This test is actually due to a Clang 3.3 shortcoming, included in OS X 10.9,
# fixed in Clang 3.4:
# http://llvm.org/releases/3.4/tools/clang/docs/ReleaseNotes.html#new-compiler-flags
if try_compile('', '-O6')
  $CFLAGS += ' -Wall -W -O6 -g'
else
  $CFLAGS += ' -Wall -W -O3 -g'
end

if ok
  create_makefile('gmp')
else
  $stderr.puts "*********************************************************************"
  $stderr.puts "Your compiler was unable to link to GMP while installing the gmp gem."
  $stderr.puts "Please install GMP before installing the gmp gem."
  $stderr.puts "*********************************************************************"

  raise "Unable to build, correct above errors and rerun"
end

