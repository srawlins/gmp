module Test::Unit::Assertions
  def assert_less_than(expected, actual, message=nil)
    assert_true(actual < expected, message)
  end
  alias :assert_lt :assert_less_than
  
  def assert_lte(expected, actual, message=nil)
    assert_true(actual <= expected, message)
  end
  
  def assert_greater_than(expected, actual, message=nil)
    assert_true(actual > expected, message)
  end
  alias :assert_gt :assert_greater_than
  
  def assert_gte(expected, actual, message=nil)
    assert_true(actual >= expected, message)
  end
  
  def assert_between(expected_low, expected_high, actual, message=nil)
    assert_true(actual <= expected_high && actual >= expected_low, message)
  end

  def assert_true(actual, message=nil)
    assert(actual, message)
  end

  def assert_false(actual, message=nil)
    assert(!actual, message)
  end
end
