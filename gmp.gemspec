Gem::Specification.new do |s|
  s.name = "gmp"
  s.version = "0.1.5.4"
  s.authors = ["Tomasz Wegrzanowski", "srawlins"]
  s.date = "2009-09-21"
  s.description = "gmp - providing Ruby bindings to the GMP library."
  s.email = ["Tomasz.Wegrzanowski@gmail.com", "sam.rawlins@gmail.com"]
  
  s.has_rdoc = false
  s.homepage = "http://github.com/srawlins/gmp/tree/master"
  s.summary = "Provides Ruby bindings to the GMP library."

  s.required_ruby_version = '>= 1.8.1'
  s.requirements = ["GMP compiled and working properly."]
  s.require_paths = ["ext"]
  #s.files = FileList["[A-Z]*", "{ext,test}/**/*", 'README.rdoc']
  s.files = ["ext/extconf.rb", "ext/gmp.c", "ext/gmpf.c", "ext/gmpf.h", "ext/gmpq.c",
             "ext/gmpq.h", "ext/gmpz.c", "ext/gmpz.h", "ext/ruby_gmp.h",
             "ext/takeover.h"]
  s.files += ["test/README", "test/tc_cmp.rb", "test/tc_fib_fac_nextprime.rb",
              "test/tc_q.rb", "test/tc_q_basic.rb", "test/tc_sgn_neg_abs.rb",
              "test/swap.rb", "test/tc_z.rb", "test/tc_z_basic.rb",
              "test/tc_z_exponentiation.rb", "test/tc_z_logic.rb",
              "test/tc_zerodivisionexceptions.rb", "test/test_helper.rb",
              "test/unit_tests.rb"]
  s.files += ["CHANGELOG", "INSTALL", "README.rdoc", "manual.tex"]
end
