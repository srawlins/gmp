#! /usr/bin/env ruby

# Copyright 2003 Free Software Foundation, Inc.

# This file is part of the GNU GMPbench.

# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.

# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place - Suite 330, Boston, MA 02111-1307, USA.

class String
  def bs
    gsub("/", "\\")
  end

  def sh_interpolate
    gsub(/\$([A-Z]+)/) { |s| ENV[$1] }
  end
end

puts "***** GMPbench version 0.2 *****"

no_compile = false
default = false
gnu_bin = "/cygwin/bin/"
gnu_bin = "/Ruby19/devkit/msys/1.0.11/bin/"

while ARGV[0] =~ /^-/
  case ARGV[0]
  when "-g" then gnu_bin = ARGV[1]; ARGV.shift; ARGV.shift
  when "-n" then no_compile = true;    ARGV.shift
  else ARGV.shift
  end
end

if RUBY_VERSION =~ /mingw/ and !( ENV["PATH"] =~ /;\.$|^\.;|;\.;/ )
  ENV["PATH"] += ";."
end

if no_compile
  puts "Suppressing compilation"
else
  unless ENV["CFLAGS"]
    ENV["CFLAGS"] = "-O3 -fomit-frame-pointer"
    puts "Using default CFLAGS = \"$CFLAGS\"".sh_interpolate
    default = true
  else
    puts "Using CFLAGS = \"$CFLAGS\" from your environment".sh_interpolate
  end
  unless ENV["CC"]
    ENV["CC"] = "gcc"
    puts "Using default CC = \"$CC\"".sh_interpolate
    default = true
  else
    puts "Using CC = \"$CC\" from your environment".sh_interpolate
  end
  unless ENV["LIBS"]
    ENV["LIBS"] = "-static -lgmp"
    puts "Using default LIBS = \"$LIBS\"".sh_interpolate
    default = true
  else
    puts "Using LIBS = \"$LIBS\" from your environment".sh_interpolate
  end

  puts "Using compilation command: $CC $CFLAGS foo.c -o foo $LIBS".sh_interpolate
  if default
    puts "You may want to override CC, CFLAGS, and LIBS"
  end

  `#{"$CC $CFLAGS gmpver.c $LIBS".sh_interpolate}`
  puts "Using `./a.out`"

  puts "Compiling benchmarks"
  `#{"$CC $CFLAGS gcd.c -o gcd $LIBS".sh_interpolate}`
  `#{"$CC $CFLAGS gcdext.c -o gcdext $LIBS".sh_interpolate}`
  `#{"$CC $CFLAGS multiply.c -o multiply $LIBS".sh_interpolate}`
  `#{"$CC $CFLAGS divide.c -o divide $LIBS".sh_interpolate}`
  `#{"$CC $CFLAGS rsa.c -o rsa $LIBS".sh_interpolate}`
  `#{"$CC $CFLAGS pi.c -o pi $LIBS -lm".sh_interpolate}`
end

multiply_args   = "128 512 8192 131072 2097152 128,128 512,512 8192,8192 " +
                  "131072,131072 2097152,2097152 15000,10000 20000,10000 " +
                  "30000,10000 16777216,512 16777216,262144"
multiply_weight = 1

divide_args     = "8192,32 8192,64 8192,128 8192,4096 131072,65536 " +
                  "8388608,4194304 8192,8064 16777216,262144"
divide_weight   = 1

gcd_args        = "128,128 512,512 8192,8192 131072,131072 1048576,1048576"
gcd_weight      = 0.5

gcdext_args     = "128,128 512,512 8192,8192 131072,131072 1048576,1048576"
gcdext_weight   = 0.5

rsa_args        = "512 1024 2048"
rsa_weight      = 1

pi_args         = "10000 100000 1000000"
pi_weight       = 1

#base_tests      = "multiply divide gcd gcdext"
base_tests      = "multiply divide gcd"
#app_tests       = "rsa pi"
app_tests       = "rsa"

tests           = "base app"

puts "Running benchmarks (propagated score accuracy exceeds printed intermediates)"

acc2 = 1
n2   = 0
tests.split(" ").each do |cat|
  puts "  Category #{cat}"
  tests = eval "#{cat}_tests"

  acc1 = 1
  n1   = 0
  tests.split(" ").each do |t|
    weight = eval "#{t}_weight"
    puts "    Program #{t} (weight=#{weight})"
    args   = eval "#{t}_args"

    acc = 1
    n   = 0
    args.split(" ").each do |a|
      ta  = `echo #{a} | #{gnu_bin}sed 's;,; ;g'`.chomp
      printf '      %-48s', "GMPbench.#{cat}.#{t}(#{a})"
      `ruby #{t} #{ta} > "RES-#{t}-#{a}"`
      sed_cmd = "#{gnu_bin}sed 's;^RESULT: \\([0-9.]*\\).*$;\\1;'"
      res_cmd = "#{gnu_bin}grep \"^RESULT\" RES-#{t}-#{a} | #{sed_cmd}"
      res = `#{res_cmd}`
      printf "%12s\n", `gexpr -prec 4 "#{res}"`.chomp
      acc = `gexpr -prec 10 "#{acc} * #{res}"`
      n   = `gexpr #{n}+1`
    end

	out  = `gexpr -prec 10 "#{acc}^(1/#{n})"`
	printf "    %-40s%12s\n", "GMPbench.#{cat}.#{t}", `gexpr -prec 5 "#{out}"`
	acc1 = `gexpr -prec 10 "#{acc1} * #{out}^#{weight}"`
	n1   = `gexpr #{n1}+#{weight}`
  end

  out  = `gexpr -prec 10 "#{acc1}^(1/#{n1})"`
  printf "  %-32s%12s\n", "GMPbench.#{cat}", `gexpr -prec 5 "#{out}"`
  acc2 = `gexpr -prec 10 "#{acc2} * #{out}"`
  n2   = `gexpr #{n2}+1`
end


out = `gexpr "#{acc2}^(1/#{n2})"`
puts "GMPbench:         #{out}"

exit 0
