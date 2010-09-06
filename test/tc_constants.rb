require './test_helper'

class TC_Constants < Test::Unit::TestCase
  def test_constants
    assert_instance_of(String, GMP::GMP_VERSION,       "GMP::GMP_VERSION should be a String")
    assert_instance_of(String, GMP::GMP_CC,            "GMP::GMP_CC should be a String")
    assert_instance_of(String, GMP::GMP_CFLAGS,        "GMP::GMP_CFLAGS should be a String")
    assert_instance_of(Fixnum, GMP::GMP_BITS_PER_LIMB, "GMP::GMP_BITS_PER_LIMB should be a Fixnum")
  end
end
