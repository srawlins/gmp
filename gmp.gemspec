Gem::Specification.new do |s|
  s.name = "gmp"
  s.version = "0.2.1"
  s.authors = ["Tomasz Wegrzanowski", "srawlins"]
  s.date = "2009-12-21"
  s.description = "gmp - providing Ruby bindings to the GMP library."
  s.email = ["Tomasz.Wegrzanowski@gmail.com", "sam.rawlins@gmail.com"]
  
  s.extensions = ["ext/extconf.rb"]
  s.has_rdoc = false
  s.homepage = "http://github.com/srawlins/gmp"
  s.summary = "Provides Ruby bindings to the GMP library."

  s.required_ruby_version = '>= 1.8.1'
  s.requirements = ["GMP compiled and working properly."]
  s.require_paths = ["ext"]
  #s.files = FileList["[A-Z]*", "{ext,test}/**/*", 'README.rdoc']
  s.files = ["ext/extconf.rb", "ext/gmp.c", "ext/gmpf.c", "ext/gmpf.h", "ext/gmpq.c", "ext/gmpq.h",
             "ext/gmpz.c", "ext/gmpz.h", "ext/ruby_gmp.h", "ext/takeover.h"]
  s.files += ["test/README", "test/tc_cmp.rb", "test/tc_f_precision.rb",
              "test/tc_f_arithmetics_coersion.rb", "test/tc_fib_fac_nextprime.rb",
              "test/tc_floor_ceil_truncate.rb", "test/tc_logical_roots.rb", "test/tc_q.rb",
              "test/tc_q_basic.rb", "test/tc_sgn_neg_abs.rb", "test/tc_swap.rb", "test/tc_z.rb",
              "test/tc_z_basic.rb", "test/tc_z_exponentiation.rb", "test/tc_z_logic.rb",
              "test/tc_z_shifts_last_bits.rb", "test/tc_z_to_d_to_i.rb",
              "test/tc_zerodivisionexceptions.rb", "test/test_helper.rb", "test/unit_tests.rb"]
  s.files += ["CHANGELOG", "INSTALL", "README.rdoc", "manual.pdf", "manual.tex"]
end
