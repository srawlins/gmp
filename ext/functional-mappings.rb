file = "functional-mappings.txt"
codes = {
  "double"          => "D",
  "gmp_randstate_t" => "Rnd",
  "int"             => "X",
  "mp_bitcnt_t"     => "X",
  "mpf_t"           => "F",
  "mpq_t"           => "Q",
  "mpz_t"           => "Z",
  "si"              => "XB",
  "ui"              => "XB+",
  "void"            => "nil"
}
Format = Struct.new(:i, :inp, :out, :ret, :inp2, :out2, :ret2, :count)
formats = []
this_f = nil
this_count = 0

File.open(file).each do |line|
  line.chomp!

  if line =~ /^>>> (\d+) (.*)__to__(.*)__returns__(.*)  #=>/
    inp = $2.split("__")
    out = $3.split("__")
    ret = $4.split("__")[0]
    
    if $3 == "none"; out2 = ""
    else;            out2 = out.map { |e| codes[e] }.join(", ") + ", "
    end
    inp2 = inp.map { |e|
      e2 = e.split("_or_")
      if e2.size > 1; e2.map { |f| codes[f] }.join("")
      else;           codes[e]
      end
    }.join(", ")
    ret = ret.split("_or_")
    ret2 = ret.map { |e| codes[e] }.join("")
    
    puts "%-5s  GMP.foo(%-10s%-13s) => %-6s   //   #{$3.ljust(21, '.')}#{$2.ljust(40, '.')}%s" % [$1, out2, inp2, ret2, $4]
    this_f = Format.new($1, $2, $3, $4, inp2, out2, ret2, 0)
  elsif this_f && line =~ /\S/
    this_count += 1
  elsif this_f && line =~ /^\s*\n?$/
    this_f.count = this_count
    formats << this_f.dup
    this_count = 0
    this_f = nil
  end
end
puts ""

formats = formats.sort_by { |f| [f.out, f.inp, f.ret] }
formats.each do |f|
  puts "%2s,%2s  GMP.foo(%-10s%-13s) => %-6s   //   #{f.out.ljust(21, '.')}#{f.inp.ljust(40, '.')}%s" % [f.i, f.count, f.out2, f.inp2, f.ret2, f.ret]
end