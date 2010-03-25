Gem::Specification.new do |s|
  s.name = "gmp"
  s.version = "0.4.7"
  s.authors = ["Tomasz Wegrzanowski", "srawlins"]
  s.date = "2010-03-25"
  s.description = "gmp - providing Ruby bindings to the GMP library."
  s.email = ["Tomasz.Wegrzanowski@gmail.com", "sam.rawlins@gmail.com"]
  
  s.extensions = ["ext/extconf.rb"]
  s.platform = Gem::Platform::CURRENT
  s.has_rdoc = "yard"
  s.homepage = "http://github.com/srawlins/gmp"
  s.summary = "Provides Ruby bindings to the GMP library."

  s.required_ruby_version = '>= 1.8.1'
  s.requirements = ["GMP compiled and working properly."]
  s.require_paths = ["lib"]
  s.files  = Dir["ext/*.c"] + Dir["ext/*.h"] + ["ext/extconf.rb"]
  s.files += ["ext/libgmp-10.dll"] + ["ext/libmpfr-1.dll"] + ["ext/gmp.so"]
  s.files += Dir["test/*.rb"] + ["test/README"]
  s.files += Dir["benchmark/*"] - Dir["benchmark/gexpr"]
  s.files += ["CHANGELOG", "INSTALL", "README.rdoc", "manual.pdf", "manual.tex"]
end
