require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcConstants < Test::Unit::TestCase
  def test_constants
    assert_instance_of(String, GMP::GMP_VERSION,       "GMP::GMP_VERSION should be a String")
    assert_instance_of(String, GMP::GMP_CC,            "GMP::GMP_CC should be a String")
    assert_instance_of(String, GMP::GMP_CFLAGS,        "GMP::GMP_CFLAGS should be a String")
    assert_instance_of(Fixnum, GMP::GMP_BITS_PER_LIMB, "GMP::GMP_BITS_PER_LIMB should be a Fixnum")
  end
end
