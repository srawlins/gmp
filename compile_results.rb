results = Dir["results*"]
# big_ugly_hash[ruby_ver][gmp_ver][mpfr_ver]
big_ugly_hash = {}
results.each do |r|
  this_tests = nil
  this_assertions = nil
  this_failures = nil
  this_errors = nil
  this_ruby = nil
  this_gmp  = nil
  this_mpfr = nil
  File.open(r) do |h|
    while (line=h.read)
      if line =~ /^(\d+) tests, (\d+) assertions, (\d+) failures, (\d+) errors/
        this_tests      = $1
        this_assertions = $2
        this_failures   = $3
        this_errors     = $4
      if line =~ /^RUBY: (.*)/
        this_ruby = $1
      elsif line =~ /^GMP: (.*)/
        this_gmp  = $1
      elsif line =~ /^MPFR: (.*)/
        this_mpfr = $1
      end
    end
  end
end
