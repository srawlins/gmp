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
  sh "cd test && ruby unit_tests.rb"
end

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
  version = ENV['GMP'] || '5.0.5'
  '--with-gmp-dir='+File.join(DEPENDENCIES_DIR, "gmp-#{version}")
end

def mpfr_opt
  version = ENV['MPFR'] || '3.1.1'
  if version == 'no-mpfr'
    return '--no-mpfr'
  else
    return '--with-mpfr-dir='+File.join(DEPENDENCIES_DIR, "mpfr-#{version}")
  end
end


