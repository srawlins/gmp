Gem::Specification.new do |s|
  s.name = "gmp"
  s.version = "0.1.2"
  s.authors = ["Tomasz Wegrzanowski", "srawlins"]
  s.date = "2009-09-21"
  s.description = "gmp is a library providing Ruby bindings to GMP library."
  s.email = ["Tomasz.Wegrzanowski@gmail.com", "sam.rawlins@gmail.com"]
  
  s.has_rdoc = false
  s.homepage = "http://github.com/srawlins/gmp/tree/master"
  s.summary = "A library providing Ruby bindings to GMP library."

  s.files = FileList["[A-Z]*", "{ext,test}/**/*", 'README.rdoc']
end
