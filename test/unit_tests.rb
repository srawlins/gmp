#!/usr/bin/env ruby

require 'test_helper'
require 'tc_z'
require 'tc_z_basic'
require 'tc_z_logic'
require 'tc_q'
require 'tc_cmp'
require 'tc_q_basic'
require 'tc_z_exponentiation'


class TC_ZeroDivisionExceptions < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(10)
    @b = GMP::Z.new()
    @c = GMP::Q.new(1)
    @d = GMP::Q.new()
  end
  
  def test_division_by_zero
    assert_raise(ZeroDivisionError) { @a.tdiv(0) }
    assert_raise(ZeroDivisionError) { @a.tdiv(@b) }
    assert_raise(ZeroDivisionError) { @d.inv }
    assert_raise(ZeroDivisionError) { @d.inv! }
    assert_raise(ZeroDivisionError) { @c/0 }
    assert_raise(ZeroDivisionError) { @c/@d }
  end
end