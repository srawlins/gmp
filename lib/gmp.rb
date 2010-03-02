# Thank you, Nokogiri

require 'rbconfig'

ENV['PATH'] = [File.expand_path(
  File.join(File.dirname(__FILE__), "..", "ext")
), ENV['PATH']].compact.join(';') if RbConfig::CONFIG['host_os'] =~ /(mswin|mingw|mingw32)/i

require File.dirname(__FILE__) + '/../ext/gmp'