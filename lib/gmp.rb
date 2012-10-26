# Thank you, Nokogiri

require 'rbconfig'

ENV['PATH'] = [File.expand_path(
  File.join(File.dirname(__FILE__), "..", "ext")
), ENV['PATH']].compact.join(';') if RbConfig::CONFIG['host_os'] =~ /(mswin|mingw|mingw32)/i

require File.dirname(__FILE__) + '/../ext/gmp'

module GMP
  def self.sprintf(format, *args)
    first_pct = format.index '%'
    result = format[0...first_pct]
    format.gsub(/(?<!%)%[0#+ ']*[0-9]*.?[0-9]*[a-zA-Z][^%]*/) do |fragment|
      arg = args.shift
      if fragment =~ /%[0#+ ']*[0-9]*.?[0-9]*[ZQF]/
        result << sprintf2(fragment, arg)
      elsif fragment =~ /%[0#+ ']*[0-9]*.?[0-9]*[PR]/ && GMP.const_defined?(:MPFR_VERSION)
        result << GMP::F.sprintf2(fragment, arg)
      else
        result << (fragment % arg)
      end
    end
    result
  end
end
