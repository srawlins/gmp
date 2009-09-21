--- !ruby/object:Gem::Specification 
name: gmp
version: !ruby/object:Gem::Version 
  version: 0.1.4
  platform: ruby
authors: 
- Tomasz Wegrzanowski
- srawlins
autorequire: 
bindir: bin
cert_chain: []

date: 2009-09-21 00:00:00 -07:00
default_executable: 
dependencies:  
description: gmp is library providing Ruby bindings to GMP library.
email: 
- sam.rawlins@gmail.com
executables: []

extensions: []

extra_rdoc_files: []

files: 
- ext/extconf.rb
- ext/gmp.c
- ext/gmpf.h
- ext/gmpq.h
- ext/gmpz.h
- ext/takeover.h
- test/README
- test/tc_cmp.rb
- test/tc_q.rb
- test/tc_q_basic.rb
- test/tc_z.rb
- test/tc_z_basic.rb
- test/tc_z_exponentiation.rb
- test/tc_z_logic.rb
- test/test_helper.rb
- test/unit_tests.rb
- CHANGELOG
- INSTALL
- README.rdoc
has_rdoc: false
homepage: http://github.com/srawlins/gmp/tree/master
post_install_message: 
rdoc_options: 
- --quiet
- --title
- gmp documentation
- --opname
- index.html
- --line-numbers
- --main
- README
- --inline-source
require_paths: 
required_ruby_version: !ruby/object:Gem::Requirement 
  requirements: 
  - - ">="
    - !ruby/object:Gem::Version 
      version: "0"
  version: 
required_rubygems_version: !ruby/object:Gem::Requirement 
  requirements: 
  - - ">="
    - !ruby/object:Gem::Version 
      version: "0"
  version: 
requirements: []

rubyforge_project: 
rubygems_version: 1.2.0
signing_key: 
specification_version: 2
summary: A library providing Ruby bindings to GMP library.
test_files: 
- test/unit_tests.rb
- test/test_helper.rb



