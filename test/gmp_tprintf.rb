require File.expand_path(File.join(File.dirname(__FILE__), 'test_helper'))

unless RUBY_VERSION =~ /^1.8/
  class GMP_TPRINTF < MiniTest::Test
    MAX_OUTPUT = 1024
    def setup
    end

    def check_plain(want, fmt_orig, *args)
      fmt = ''
      idx2 = 0
      fmt_orig.split(//).each_with_index do |char, idx|
        case char
        # The exact value of the exponent isn't guaranteed in glibc, and it and
        # gmp_printf do slightly different things, so don't compare directly.
        when 'a' then return
        when 'A' then return
        when 'F'
          if idx > 0 and fmt_orig[idx-1] == '.'
            return  # don't test the "all digits" cases
          end
          # discard 'F' type
          next
        when 'Z'
          # transmute
          # was 'l' in t-printf.c, but Ruby does not have such an 'l'
          next
        else
          fmt[idx2] = fmt_orig[idx]
          idx2 += 1
        end
      end
      assert(fmt.size < MAX_OUTPUT)
      got = GMP.sprintf(fmt, *args)

      assert_equal(want, got, "GMP.sprintf(#{fmt}, #{args.join(', ')}) generates correct output.")
    end

    def check_one(want, fmt, *args)
      assert_equal(want, GMP.sprintf(fmt, *args))
    end

    def hex_or_octal(fmt)
      fmt =~ /[xXo]/
    end

    def test_check_z
      data = [
        ['%Zd',    '0',    '0'],
        ['%Zd',    '1',    '1'],
        ['%Zd',  '123',  '123'],
        ['%Zd',   '-1',   '-1'],
        ['%Zd', '-123', '-123'],

        ['%+Zd',    '0',   '+0'],
        ['%+Zd',  '123', '+123'],
        ['%+Zd', '-123', '-123'],

        ['%Zx',  '123',   '7b'],
        ['%ZX',  '123',   '7B'],
        ['%Zx', '-123',  '-7b'],
        ['%ZX', '-123',  '-7B'],
        ['%Zo',  '123',  '173'],
        ['%Zo', '-123', '-173'],

        ['%#Zx',    '0',     '0'],
        ['%#ZX',    '0',     '0'],
        ['%#Zx',  '123',  '0x7b'],
        ['%#ZX',  '123',  '0X7B'],
        ['%#Zx', '-123', '-0x7b'],
        ['%#ZX', '-123', '-0X7B'],

        ['%#Zo',    '0',     '0'],
        ['%#Zo',  '123',  '0173'],
        ['%#Zo', '-123', '-0173'],

        ['%10Zd',    '0', '         0'],
        ['%10Zd',  '123', '       123'],
        ['%10Zd', '-123', '      -123'],

        ['%-10Zd',    '0', '0         '],
        ['%-10Zd',  '123', '123       '],
        ['%-10Zd', '-123', '-123      '],

        ['%+10Zd',   '123', '      +123'],
        ['%+-10Zd',  '123', '+123      '],
        ['%+10Zd',  '-123', '      -123'],
        ['%+-10Zd', '-123', '-123      '],

        ['%08Zd',    '0', '00000000'],
        ['%08Zd',  '123', '00000123'],
        ['%08Zd', '-123', '-0000123'],

        ['%+08Zd',    '0', '+0000000'],
        ['%+08Zd',  '123', '+0000123'],
        ['%+08Zd', '-123', '-0000123'],

        ['%#08Zx',    '0', '00000000'],
        ['%#08Zx',  '123', '0x00007b'],
        ['%#08Zx', '-123', '-0x0007b'],

        ['%+#08Zx',    '0', '+0000000'],
        ['%+#08Zx',  '123', '+0x0007b'],
        ['%+#08Zx', '-123', '-0x0007b'],

        ['%.0Zd', '0', ''],
        ['%.1Zd', '0', '0'],
        ['%.2Zd', '0', '00'],
        ['%.3Zd', '0', '000']
      ]

      data.each do |parameters|
        z = GMP::Z(parameters[1])
        if not hex_or_octal(parameters[0]) and (parameters[0]['+'] or z < 0)
          check_plain(parameters[2], parameters[0], z.to_i)
        end

        check_one(parameters[2], parameters[0], z)

        # Same again, with %N and possibly some high zero limbs
        nfmt = parameters[0].dup
        #lots more weird code... resulting in another check_one
      end
    end
  end
end
