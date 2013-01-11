require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))
require 'fileutils'

if (RUBY_DESCRIPTION =~ /rubinius/i and RUBY_VERSION =~ /^1.8/) ||
   (RUBY_DESCRIPTION =~ /jruby/i)
  # Sorry charlie
else
class TC_Z_IO < Test::Unit::TestCase
  def setup
    @two_pow_100   = GMP::Z.pow(GMP::Z(2), 100)
    @three_pow_100 = GMP::Z.pow(GMP::Z(3), 100)
    @rs = GMP::RandState.new
    @rs.seed(13579)
  end

  def test_io_raw_with_two_pow_100
    handle = File.new('test_io_raw', 'w')
    @two_pow_100.out_raw(handle)
    handle.close

    handle = File.open('test_io_raw')
    result = GMP::Z()
    GMP::Z.inp_raw(result, handle)
    FileUtils.rm('test_io_raw')
    assert_equal(@two_pow_100, result, "GMP::Z should out_raw and inp_raw correctly")
  end

  def test_io_raw_with_three_pow_100
    handle = File.new('test_io_raw', 'w')
    @three_pow_100.out_raw(handle)
    handle.close

    handle = File.open('test_io_raw')
    result = GMP::Z()
    GMP::Z.inp_raw(result, handle)
    FileUtils.rm('test_io_raw')
    assert_equal(@three_pow_100, result, "GMP::Z should out_raw and inp_raw correctly")
  end

  def test_io_raw_randoms
    100.times do |i|
      handle = File.new('test_io_raw', 'w')
      z = @rs.urandomb(1024)
      z.out_raw(handle)
      handle.close

      handle = File.open('test_io_raw')
      result = GMP::Z()
      GMP::Z.inp_raw(result, handle)
      assert_equal(z, result, "GMP::Z should out_raw and inp_raw correctly")
    end
    FileUtils.rm('test_io_raw')
  end
end
end
