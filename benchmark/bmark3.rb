#!/usr/bin/env ruby

bits = ARGV[0]
if bits.nil?
    print "Usage #{$0} <bits> [iterations]\n"
    exit 1
end

iterations=ARGV[1]
iterations=100000 if iterations.nil?

# Generate

bmark = [
"#!/usr/bin/ruby

a=2**#{bits}-1
b=a.dup

#{iterations}.times { |i|
    a+b
}
",

"#!/usr/bin/ruby

require 'gmp'

a=GMP::Z.new(GMP::Z.new(2)**#{bits})
b=GMP::Z.new(a)

#{iterations}.times { |i|
    a+b
}
",

"#include <gmp.h>

int main()
{
    mpz_t a,b,c;
    int i;
    
    mpz_init (a);
    mpz_ui_pow_ui (a, 2, #{bits});
    mpz_init_set (b, a);
    
    for(i=0; i<#{iterations}; i++)
    {
	mpz_init (c);
	mpz_add (c,a,b);
	mpz_clear (c);
    }
    return 0;
}"
]

File.open("tmp/bn-#{bits}.rb","w") {|f| f.print bmark[0]}
File.open("tmp/gmp-#{bits}.rb","w") {|f| f.print bmark[1]}
File.open("tmp/gmpc-#{bits}.c","w") {|f| f.print bmark[2]}
system "chmod +x tmp/bn-#{bits}.rb"
system "chmod +x tmp/gmp-#{bits}.rb"
system "gcc -Wall -W -O6 tmp/gmpc-#{bits}.c -o tmp/gmpc-#{bits} -lgmp"

# Run

print "BigNum\n"
3.times { |i| print "\t",`sh -c 'time tmp/bn-#{bits}.rb' 2>&1`.gsub("\n","\t"),"\n" }
print "Ruby GMP\n"
3.times { |i| print "\t",`sh -c 'time tmp/gmp-#{bits}.rb' 2>&1`.gsub("\n","\t"),"\n" }
print "C GMP\n"
3.times { |i| print "\t",`sh -c 'time tmp/gmpc-#{bits}' 2>&1`.gsub("\n","\t"),"\n" }

# Delete

File.unlink ("tmp/bn-#{bits}.rb", "tmp/gmp-#{bits}.rb", "tmp/gmpc-#{bits}.c", "tmp/gmpc-#{bits}")
