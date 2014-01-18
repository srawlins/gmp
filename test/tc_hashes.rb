require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcHashes < Test::Unit::TestCase
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
  
  def test_q_hashes
    h = {}
    h[GMP::Q(1,4)] = [GMP::Q(16,64), GMP::Q(166,664)]
    assert(h[GMP::Q(1,4)] != nil, "Newly created GMP::Zs should hash equally if they are equal.")
    assert(h[GMP::Q(233,144)].nil?,    "Newly created GMP::Zs should hash differently if they are different.")
    10.times do
      assert(GMP::Q(11,13).hash == GMP::Q(11,13).hash)
    end
    
    100.times do |i|
      assert(GMP::Q(1,1).hash != GMP::Q(i,101).hash)
    end
    
    # GMP::Q(5,1) and "5" might (I think, 'do') hash the same, but should not be equal
    assert(! GMP::Q(101,1).eql?("101"))
    
    h["22/7"] = "String"
    h[GMP::Q(22,7)] = "GMP::Z"
    assert(h["22/7"] != "GMP::Z")
    #assert(h[GMP::Q(22/7)] != "String")  # caused segfaults, i swear!
    assert(h[GMP::Q(22,7)] != "String")
    assert(h[GMP::Q(22,7)] == "GMP::Z")
    assert(h["22/7"] == "String")
  end
end
