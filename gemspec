Gem::Specification.new do |s|
  s.name = "gmp"
  s.version = "0.1.4"
  s.authors = ["Tomasz Wegrzanowski", "srawlins"]
  s.date = "2009-09-21"
  s.description = "gmp is a library providing Ruby bindings to GMP library."
  s.email = ["Tomasz.Wegrzanowski@gmail.com", "sam.rawlins@gmail.com"]
  
  s.has_rdoc = false
  s.homepage = "http://github.com/srawlins/gmp/tree/master"
  s.summary = "A library providing Ruby bindings to GMP library."

  #s.files = FileList["[A-Z]*", "{ext,test}/**/*", 'README.rdoc']
  s.files = ["ext/extconf.rb", "ext/gmp.c", "ext/gmpf.h", "ext/gmpq.h", "ext/gmpz.h", "ext/takeover.h"]
  s.files += ["test/README", "test/tc_cmp.rb", "test/tc_q.rb", "test/tc_q_basic.rb", "test/tc_z.rb",
              "test/tc_z_basic.rb", "test/tc_z_exponentiation.rb", "test/tc_z_logic.rb",
              "test/tc_z_zerodivisionexceptions.rb", "test/test_helper.rb", "test/unit_tests.rb"]
  s.files += ["CHANGELOG", "INSTALL", "README.rdoc"]
end
