require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcConstants < MiniTest::Test
  def test_constants
    assert_instance_of(String,  GMP::GMP_VERSION,       "GMP::GMP_VERSION should be a String")
    assert_instance_of(String,  GMP::GMP_CC,            "GMP::GMP_CC should be a String")
    assert_instance_of(String,  GMP::GMP_CFLAGS,        "GMP::GMP_CFLAGS should be a String")
    assert_instance_of(0.class, GMP::GMP_BITS_PER_LIMB, "GMP::GMP_BITS_PER_LIMB should be a Fixnum")
  end
end
