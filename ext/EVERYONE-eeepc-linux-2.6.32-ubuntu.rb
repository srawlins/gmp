# do ccmt for every conceivable configuration
#
# This one is for the MacBook, running OS X 10.6

puts "In case you were wondering, this ruby is #{RUBY_VERSION}"
sleep 1

benchmark = false
mpfr = ["", "--no-mpfr"]

until ARGV.empty?
  case
  when ARGV[0]=="--benchmark" then benchmark=true; ARGV.shift
  when ARGV[0]=~/--mpfr=(.*)/
    mpfr=$1.split(','); ARGV.shift
  else ARGV.shift
  end
end

mpfr.each do |no_mpfr|
  #%w(1.8.7 1.9.1 1.9.2 ree).each do |ruby_v|
  %w(ree).each do |ruby_v|
    ["5.0.1 3.0.0", "4.3.2 2.4.2", "4.3.1 2.4.2"].each do |gmp_mpfr|
      system("./ccmt.sh #{no_mpfr} #{ruby_v} #{gmp_mpfr} 2>&1 | " +
             "tee ../results-test-#{ruby_v}-" +
             "#{no_mpfr!='' ? gmp_mpfr.sub(/ .*/, '') : gmp_mpfr.sub(/ /, '-')}")
      if benchmark
        Dir.chdir('../benchmark')

        system("./srb.sh #{ruby_v} 2>&1 | " +
               "tee ../results-benchmark-#{ruby_v}-" +
               "#{no_mpfr!='' ? gmp_mpfr.sub(/ .*/, '') : gmp_mpfr.sub(/ /, '-')}")
        Dir.chdir('../ext')
      else
        sleep 1
      end
    end
  end
end
