require 'minitest/autorun'
require File.expand_path(File.join(File.dirname(__FILE__), 'test_unit', 'assertions'))
#require './test_unit/assertions'  # Monkey patch
require 'rbconfig'

ENV['PATH'] = [
  File.expand_path(File.join(File.dirname(__FILE__), "..", "ext")),
  ENV['PATH']
].compact.join(';') if RbConfig::CONFIG['host_os'] =~ /(mswin|mingw|mingw32)/i

require File.dirname(__FILE__) + '/../lib/gmp'

if GMP.const_defined?(:MPFR_VERSION)
  if GMP::MPFR_VERSION >= "3.0.0"
    RND_MODES = [GMP::MPFR_RNDN, GMP::MPFR_RNDZ, GMP::MPFR_RNDU, GMP::MPFR_RNDD, GMP::MPFR_RNDA]
  else
    RND_MODES = [GMP::GMP_RNDN, GMP::GMP_RNDZ, GMP::GMP_RNDU, GMP::GMP_RNDD]
  end
end
