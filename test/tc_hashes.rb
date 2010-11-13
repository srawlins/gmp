require './test_helper'

class TC_Hashes < Test::Unit::TestCase
  def setup
  end
  
  def test_z_hashes
    h = {}
    h[GMP::Z(131)] = [GMP::Z(41), GMP::Z(43), GMP::Z(47)]
    assert(h[GMP::Z(131)] != nil, "Newly created GMP::Zs should hash equally if they are equal.")
    assert(h[GMP::Z(59)].nil?,    "Newly created GMP::Zs should hash differently if they are different.")
    10.times do
      assert(GMP::Z(73).hash == GMP::Z(73).hash)
    end
    
    100.times do |i|
      assert(GMP::Z(101).hash != GMP::Z(i).hash)
    end
    
    # GMP::Z(5) and "5" might (I think, 'do') hash the same, but should not be equal
    assert(! GMP::Z(101).eql?("101"))
    
    h["127"] = "String"
    h[GMP::Z(127)] = "GMP::Z"
    assert(h["127"] != "GMP::Z")
    assert(h[GMP::Z(127)] != "String")
    assert(h[GMP::Z(127)] == "GMP::Z")
    assert(h["127"] == "String")
  end
end
