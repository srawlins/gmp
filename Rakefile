# Dependencies base directory. I have to figure out how to... not hard code this?
DEPENDENCIES_DIR = '/usr/local'

task :clean do
  sh "cd ext && make clean; echo"
end

task :extconf => [:clean] do
  sh "cd ext && ruby extconf.rb #{gmp_opt} #{mpfr_opt}"
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
  require 'irb'
  require File.join(File.dirname(__FILE__), 'lib', 'gmp')
  ARGV.clear
  IRB.start
end

namespace :dependencies do
  task :list do
    suffix = (DEPENDENCIES_DIR.size+1)..-1
    puts "GMP packages installed into #{DEPENDENCIES_DIR}:"
    Dir.glob(File.join(DEPENDENCIES_DIR, 'gmp') + '*').each do |dir|
      puts dir[suffix]
    end
    puts ""

    puts "MPFR packages installed #{DEPENDENCIES_DIR}:"
    Dir.glob(File.join(DEPENDENCIES_DIR, 'mpfr') + '*').each do |dir|
      puts dir[suffix]
    end
  end
end

def gmp_opt
  version = ENV['GMP'] || '5.1.0'
  directory = File.join(DEPENDENCIES_DIR, "gmp-#{version}")
  if !File.exist? directory
    puts "========================================"
    puts "==  Warning: target GMP installation directory does not exist: #{directory}"
    puts "========================================"
  else
    '--with-gmp-dir=' + directory
  end
end

def mpfr_opt
  version = ENV['MPFR'] || '3.1.1'
  if version == 'no-mpfr'
    return '--no-mpfr'
  end

  directory = File.join(DEPENDENCIES_DIR, "mpfr-#{version}")
  if !File.exist? directory
    puts "========================================"
    puts "==  Warning: target MPFR installation directory does not exist: #{directory}"
    puts "========================================"
  else
    return '--with-mpfr-dir=' + directory
  end
end
