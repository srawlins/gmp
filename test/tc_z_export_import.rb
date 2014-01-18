require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

class TcIntegerExportImport < Test::Unit::TestCase
  def setup
    @a = GMP::Z.new(3)**40
  end

  def test_export_import_identity
    assert_equal(@a, GMP::Z.import(@a.export),         "An integer should export and then import to itself")
    assert_equal(@a, GMP::Z.import(@a.export(-1)),     "An integer should export and then import to itself")
    assert_equal(@a, GMP::Z.import(@a.export, -1),     "An integer should export and then import to itself")
    assert_equal(@a, GMP::Z.import(@a.export(-1), -1), "An integer should export and then import to itself")
  end

  def test_export_import_order
    assert_equal(@a, GMP::Z.import(@a.export(1), 1), "An integer should export and then import to itself")

    assert_false(@a == GMP::Z.import(@a.export( 1), -1), "An integer exported one way and then imported should not be equal")
    assert_false(@a == GMP::Z.import(@a.export(-1),  1), "An integer exported one way and then imported should not be equal")
  end
end
