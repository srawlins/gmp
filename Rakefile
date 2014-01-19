# Dependencies base directory. I have to figure out how to... not hard code this?
DEPENDENCIES_DIR = ENV["DEPENDENCIES_DIR"] || "/usr/local"

task :clean do
  sh "cd ext && make clean; echo"
end

task :extconf => [:clean] do
  sh "cd ext && ruby extconf.rb #{with_option("gmp")} #{with_option("mpfr")}"
end

task :make => [:extconf] do
  sh "cd ext && make"
end

task :test => [:make] do
  sh "ruby test/unit_tests.rb"
end

task :default => [:report]
task :report => [:test] do
  puts "RUBY:   " + `ruby -v`
  puts "GMP:    " + `ruby -r './lib/gmp' -e "puts GMP::GMP_VERSION"`
  puts "GMP_CC: " + `ruby -r './lib/gmp' -e "puts GMP::GMP_CC"`
  if ENV['MPFR'] != 'no-mpfr'
    puts "MPFR:   " + `ruby -r './lib/gmp' -e "puts GMP::MPFR_VERSION"`
  else
    puts "MPFR:   --no-mpfr"
  end
end

task :console do
  require "irb"
  require File.join(File.dirname(__FILE__), "lib", "gmp")
  ARGV.clear
  IRB.start
end

namespace :dependencies do
  task :list do
    print_possible_dependencies("gmp")
    puts ""
    print_possible_dependencies("mpfr")
  end
end

def print_possible_dependencies(lib)
  puts "#{lib.upcase} packages installed in #{DEPENDENCIES_DIR}:"
  suffix = (DEPENDENCIES_DIR.size+1)..-1
  possible_dependencies("gmp").each { |pd| puts "  #{pd[suffix]}" }
end

def possible_dependencies(lib)
  Dir.glob(File.join(DEPENDENCIES_DIR, lib) + "*")
end

def with_option(lib)
  default_versions = { "gmp" => "5.1.3", "mpfr" => "3.1.2" }

  if ENV[lib.upcase].nil? || ENV[lib.upcase].empty?
    version = default_versions[lib]
  else
    version = ENV[lib.upcase]
  end

  if version == "no-#{lib}"
    return "--no-#{lib}"
  end

  directory = File.join(DEPENDENCIES_DIR, "#{lib}-#{version}")

  if File.exist? directory
    return "--with-#{lib}-dir=" + directory
  end

  # Homebrew directories
  directory2 = File.join(DEPENDENCIES_DIR, lib, version)

  if File.exist? directory2
    return "--with-#{lib}-dir=" + directory2
  end

  puts "========================================"
  puts "==  Warning: neither target #{lib.upcase} installation directory exists:"
  puts "==  * #{directory}"
  puts "==  * #{directory2}"

  latest = possible_dependencies(lib)[-1]
  if latest.nil?
    puts "========================================"
    return nil
  else
    puts "==  but #{latest} exists; we'll try that."
    puts "========================================"
    return "--with-#{lib}-dir=" + latest
  end
end
