<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<style>
body {
  margin: 0 auto;
  width: 800px;
}

table {
  border-collapse: collapse;
  border-top:    solid 1px #000000;
  border-bottom: solid 1px #000000;
  border-right:  solid 1px #000000;
  border-left:   solid 1px #000000;
  margin: 0 auto;
}

th {
  font-weight: normal;
  border-top:    solid 1px #000000;
  border-bottom: solid 2px #000000;
  border-right:  solid 1px #000000;
  border-left:   solid 1px #000000;
}

td {
  border-top:    solid 1px #CCCCCC;
  border-bottom: solid 1px #CCCCCC;
  border-right:  solid 1px #666666;
  border-left:   solid 1px #666666;
}

td, th {
  padding: 2px 4px;
}

pre {
  background-color: #EEEEEE;
  padding: 3px 2px;
</style>

# GMP gem Performance

Performance analysis of the GMP gem

7 October 2013

\vfill

written by Sam Rawlins

\newpage

\tableofcontents

\newpage

## Introduction to the performance benchmarks

The benchmarking system used to test the performance of the gmp gem is inspired
by, and uses parts of, gmpbench 0.2. http://gmplib.org/gmpbench.html. gmpbench
consists of two parts:

* `multiply`, `divide`, `gcd`, `gcdext`, `rsa`, and `pi` are 6 small programs
  that use GMP to measure a specific piece of functionality. `multiply`,
  `divide`, `gcd`, and `gcdext` are the "base" benchmarks that test small
  pieces of functionality. `rsa` and `pi` are the "application" benchmarks that
  measure the performance of a larger concept implemented with GMP.
* `runbench` is a shell script that coordinates an execution of each of the
  benchmarking programs, applying a weight to the results of each, and yielding
  a total score for GMP on the current system.

The benchmarking system in the gmp gem uses Ruby versions of each of the 6
programs (actually, `pi` is still being ported), attempting to be identical
to their C code siblings. This system also just uses `runbench` unmodified from
the original gmpbench suite.

Due to a few issues with the gmp gem itself, there are actually
2x different versions of the benchmark suite that use the gmp gem:

* `benchmark/gmp/bin_op` uses binary operators, such as `*`, on `GMP::Z`
  integers. Since `a * b` creates a new `mpz_t` that it returns, the benchmark
  programs are constantly creating new objects, which is _not_ what the GMP
  benchmark programs do.
* `benchmark/gmp/functional` uses the "functional" `GMP::Z` singleton methods
  to perform what would otherwise be binary operations. For example, `x * y` is
  replaced with `GMP::Z.mul(z,x,y)` in order to use `z` as the "return
  argument" through each iteration of the benchmark. In this version, as in the
  GMP benchmark programs, `z` is only created once, before the benchmark begins
  measuring time.

## Run the Benchmarks

In order to run a directory of benchmarks (a directory containing `multiply`,
`runbench`, etc.), just cd into the directory, then use the command:

    ./runbench -n

Next to each test case, program, and category, a "score" will be printed
(iterations per second). For program, category, and overall scores, this
represents a weighted geometric mean, and so should just be thought of as a
unitless score more than an actual real-world metric.

\newpage

## Ruby benchmarks

In addition to the above variations of the benchmark suite located in
`benchmark/gmp`, there is one more variation of the benchmark suite that
measure's Ruby's Bignum algorithms. This suite is located at `benchmark/ruby`.

### New `Bignum` methods

Several methods provided in `GMP::Z` are not provided in `Bignum`, in Ruby's
standard library. In order to attempt a vague comparison between `Bignum` and
`GMP::Z`, a simple and "fast enough" version of the following methods is
provided in `benchmark/ruby/ruby-enhancements`:

* `Bignum.gcdext`
* `Bignum.invert`
* `Bignum.powmod`
* `Bignum#[]=`
* `Bignum#gcd`

`Bignum.gcdext`, `Bignum.invert`, and `Bignum.powmod` are all borrwed from John
Nishinaga, available at <https://gist.github.com/2388745>.

### Modifications to `benchmark/ruby` benchmarks

Ruby's `Bignum` class is not always advanced enough to handle several of the
benchmark test cases. In Ruby 2.0, for example, 2 can only be raised to
approximately 8,320,000, which is just short of 2^23. Bignum was largely
re-written in Ruby 2.1 though, so that 2 can be raised to approximately
32_500_000 (just short of 2^25).

In any case, Ruby 2.0 cannot handle the following benchmark cases:

* `multiply 16777216 512`
* `multiply 16777216 262144`
* `divide 8388608 4194304`
* `divide 16777216 262144`

In the `benchmark/ruby` suite, these have been removed, so that summary scores
can still be produced. In order to compare these summary scores against
`GMP::Z` benchmarks, there is also a `benchmark/gmp/reduced` suite that uses
the same test cases. `benchmark/gmp/reduced` is the only test suite that should
be compared against `benchmark/ruby` (or, with some work, one can manually
calculate the weighted geometric means, using the same method found in
`runbench`.

\newpage

## Results

Raw benchmark results can be found in `benchmark/results-5.1.3_0.6.31.ods`, a
LibreOffice spreadsheet.  Below I show some interpreted results.

### Ruby v Ruby

I benchmarked two different versions of Ruby's Bignum implementation: Ruby
2.0.0-p247 and Ruby 2.1.0-preview1. These tests only measured Ruby's Bignum,
and did not use the GMP library at all. Ruby 2.0.0 and 2.1.0 performed very
similarly, within XX% of each other in most cases. `XXX` is the only benchmark
that averaged out slower in Ruby 2.1.0 over 2.0.0:

Program      Ruby 1.9.3    Ruby 2.0.0-rc1    2.0.0-rc1 over 1.9.3*
---------  ------------  ----------------  -----------------------
multiply       1.17e+04          1.12e+04                     0.95
divide         5.00e+01          5.41e+04                     1.08
gcd            7.51e+01          7.52e+01                     1.00
gcdext         2.36e+01          2.35e+01                     1.00
[base]         2.91e+03          2.94e+03                     1.01
rsa            3.58e+02          3.81e+02                     1.06
[app]          3.58e+02          3.81e+02                     1.06
[bench]        1.02e+03          1.06e+03                     1.04

\* Calculated as $\frac{2.0.0-rc1 \text{ score}}{1.9.3 \text{ score}}$ so that 1.08 means "1.08 times as fast" or equivalently "0.08 times faster."

We can look at individual tests to see where 2.0.0-rc1 specifically is slower
than 1.9.3:

* Firstly, in the `multiply` test, 2.0.0-rc1 and 1.9.3 are actually
  neck-and-neck (within 5% of eachother) in any tests where the operands are
  larger than 512-bit.  This likely reveals that the process of running the
  multiplications is the same speed in either interpreter, but that 2.0.0-rc1
  requires more overhead to loop. When benchmarking 128- and 512-bit operands,
  we are looping much more than when benchmarking 8192-bit and larger operands.

\newpage

## gmp gem: Binary Operators v In-Place Operators

It is beneficial to look at the two different forms of methods sometimes
offered: binary operators (such as `GMP::Z#+` which is used like `c = a + b`)
and "in-place" operators (class methods) (such as `GMP::Z.add` which is used like
`GMP::Z.add(c, a, b)`). At this time, only the `GMP::Z#*` binary operator is
available as a functional operator (`GMP::Z.multiply`), which can change gears
to a squaring algorithm if it detects that the operands are equal. (Squaring is
thus faster than multiplication.) We can look at those results below:

1.96
1.81
1.07
1.02
1.01
1.96
1.69
1.05
1.00
1.01
1.03
1.02
1.03
1.02
1.00


Test Case                       Bin Op    Functional    Functional over Bin Op
-----------                 ----------  ------------  ------------------------
multiply(128)                 4.11e+06      8.06e+06                      1.96
multiply(512)                 3.27e+06      5.91e+06                      1.81
multiply(8192)                1.78e+05      1.90e+05                      1.07
multiply(131072)              3.56e+03      3.61e+03                      1.02
multiply(2097152)             1.33e+02      1.34e+02                      1.01
multiply(128, 128)            4.12e+06      8.08e+06                      1.96
multiply(512, 512)            3.08e+06      5.22e+06                      1.69
multiply(8192, 8192)          1.28e+05      1.35e+05                      1.05
multiply(131072, 131072)      2.63e+03      2.64e+03                      1.00
multiply(2097152, 2097152)    8.94e+01      9.00e+01                      1.01
multiply(15000, 10000)        6.54e+04      6.74e+04                      1.03
multiply(20000, 10000)        5.17e+04      5.26e+04                      1.02
multiply(30000, 10000)        3.32e+04      3.40e+04                      1.03
multiply(16777216, 512)       4.32e+02      4.41e+02                      1.02
multiply(16777216, 262144)    2.10e+01      2.11e+01                      1.00

We can see the effects of allocating new `GMP::Z` objects every iteration of
the benchmark loop. When we are squaring or multiplying "small," 128-bit or
512-bit numbers, allocating objects and garbage collection can slow down the
computation by two-fold (note: this slowdown was three- or four-fold on a
machine with less RAM; 4GB vs the 16GB for these results), if the computation
is multiplying numbers (using `GMP::Z#*`) in a tight loop.

Once we get to squaring (or multiplying) 8192-bit numbers, however, the time
spent inside GMP becomes great enough, that garbage collection and object
allocation fades into the background. Above this size, binary operators can be
only 7% slower. When squaring 131072-bit numbers, or multiplying 10000-bit
numbers, binary operators are 3%, or less, slower.

\newpage

### GNU Multiple Precision Arithmetic Library, without Ruby

Here I present a few select raw benchmark results of GMP 5.1.0, using the
original gmpbench 0.2 software. These tests do not involve the Ruby interpreter
in any way.

Program                       GMP 5.1.0
--------------------------  -----------
multiply(128, 128)             9.78e+07
multiply(2097152, 2097152)     9.00e+01
multiply(16777216, 262144)     2.11e+01
multiply                       4.61e+04
divide(8192, 32)               1.85e+06
divide(16777216, 262144)       1.07e+01
divide                         4.70e+04
gcd                            8.54e+03
gcdext                         5.63e+03
[base]                         2.47e+04
rsa                            5.85e+03
[app]                          5.22e+02
[bench]                        1.20e+04

In both columns of results, the `pi` results have not been presented, as they
cannot be compared to anything in the Ruby gmp gem, yet.

\newpage

### GMP v gmp gem v Ruby Bignum

Now that we have all of the required test results, and the known limitations of
gmp gem's binary operators, we can do a proper comparison between raw GMP, the
gmp gem, and Ruby's Bignum. First, a table with some summarized results, and no
direct comparisons:

Program         GMP    gmp gem    Ruby Bignum
---------  --------  ---------  -------------
multiply   4.61e+04   2.38e+04       1.17e+04
divide     4.70e+04   3.10e+04       4.99e+04
gcd        8.54e+03   7.42e+03       7.51e+01
gcdext     5.63e+03   4.78e+03       2.36e+01
[base]     2.47e+04   1.64e+04       2.91e+03
rsa        5.85e+03   5.65e+03       3.58e+02
[app]      5.85e+03   5.65e+03       3.58e+02
[bench]    1.20e+04   9.62e+03       1.02e+03

At a glance, it looks like GMP is consistently faster than the gmp gem. They
are, however, on the same order of magnitude. We can also see that the gmp gem
is consistently faster than Ruby's Bignum, by one or two orders of magnitude.

Here are the specifics of these tests:

* The pure GMP tests used GMP 5.1.0, compiled with GCC 4.7.2.
* The gmp gem tests used the master branch of the gmp gem (roughly equivalent
  to gmp gem version 0.6.19), compiled against GMP 5.1.0, on Ruby 1.9.3,
  compiled with GCC 4.7.2.
* The Ruby Bignum tests used Ruby 1.9.3, compiled with GCC 4.7.2.

\newpage

### gmp gem v Ruby Bignum

Perhaps the most useful results to come out of the benchmark testing are the
comparisons between Ruby's Bignum and the gmp gem. These results show the
possible performance gains when refactoring Ruby code to use the gmp gem:

# ONLY FIRST TWO COLUMNS ARE NEW DATA

Test Case                    Ruby Bignum    gmp gem    gmp gem over Bignum
-------------------------  -------------  ---------  ---------------------
multiply(128)                   4.88e+06   4.11e+06                   0.59
multiply(512)                   2.70e+06   3.27e+06                   1.08
multiply(2097152)               6.91e+00   1.33e+02                  21.95
multiply(128,128)               4.54e+06   4.12e+06                   0.62
multiply(512,512)               1.92e+05   3.08e+06                   1.06
multiply(2097152,2097152)       4.90e+00   8.94e+01                  17.48
multiply                        1.17e+04   2.38e+04                   4.43
divide(8192,32)                 3.39e+05   1.06e+06                   1.97
divide(8192,4096)               3.54e+04   3.75e+05                   8.81
divide(131072,65536)            1.25e+02   3.55e+03                  24.47
divide                          4.99e+04   3.10e+04                   6.29
gcd(128,128)                    6.66e+04   2.29e+06                  21.05
gcd(8192,8192)                  1.87e+02   1.13e+04                  76.83
gcd(1048576,1048576)            1.92e-02   9.01e+00                 532.98
gcd                             3.08e+01   7.42e+03                  95.38
gcdext(128,128)                 2.02e+04   1.08e+06                  26.03
gcdext(8192,8192)               5.43e+01   8.13e+03                 152.61
gcdext(1048576,1048576)         7.33e-03   5.98e+00                 876.51
gcdext                          2.36e+01   4.78e+03                 165.59
[base]                          2.91e+03   1.64e+04                  15.20
rsa(512)                        1.42e+03   3.27e+04                  24.88
rsa(2048)                       8.64e+01   9.18e+02                  12.80
rsa                             3.58e+02   5.65e+03                  17.84
[app]                           3.58e+02   5.65e+03                  17.84
[bench]                         1.02e+03   9.62e+03                  16.47

Let's analyze the multiplication results first. We can see that below a
XXX threshold of squaring (or multiplying together) 512-bit numbers, Ruby's Bignum
implementation is actually faster than using the gmp gem. Beyond this threshold
however, the gmp gem gets continually faster. The greatest improvement measured
XXX in multiplication is the case of squaring a 2097152-bit number, where the gmp
XXX gem is approximately 22 times as fast as Ruby's Bignum. The (geometric) average
XXX improvement is 4.43x. The reason for an improved speedup with larger numbers is
of course attributable to asymptotically faster algorithms used in GMP.

XXX The division results show much the same thing. When dividing an 8192-bit by a
32-bit number, the gmp outperforms Ruby's Bignum at abouttwice as fast. Beyond
XXX that, the gmp grows to be up to 25 times as fast as Ruby's Bignum. This growing
gap is again attributable to asymptotically faster algorithms in GMP.

The GCD and GCD Extended cases show that the gmp gem is dramatically faster than Ruby's Bignum. However, this test is not actually benchmarking any GCD algorithms written into the Bignum C extension; it is using the GCD algorithms that were written in Ruby, for the benchmark tests. It is likely that a faster algorithm could be implemented in Ruby in a few hours, or that a faster implementation could be written in C, as a Bignum C extension function, in a few dozen hours. This is something that should be examined in the future.

The RSA test results show the reverse phenomenon as all of the previous results: The gmp gem is an order of magnitude faster than pure Ruby in every test, but the Ruby Bignum implementation appears to be _catching up_ to the gmp gem. This is currently not understood.

\newpage

### Pure GMP vs gmp gem

The other question that scientific computation experts will want answered is this: what is the cost of refactoring a GMP application into Ruby and the gmp gem? Let's compare those two:

Test Case                     gmp gem    Pure GMP    GMP over gmp gem
-------------------------  ----------  ----------  ------------------
multiply(128)                1.06e+06    4.56e+07               43.05
multiply(512)                9.13e+05    8.79e+06                9.63
multiply(2097152)            6.24e+01    6.38e+01                1.02
multiply(128,128)            1.07e+06    4.55e+07               42.50
multiply(512,512)            8.77e+05    6.39e+06                7.29
multiply(2097152,2097152)    3.99e+01    4.09e+01                1.02
multiply                     2.17e+04    5.58e+04                2.57
divide(8192,32)              3.90e+05    7.23e+05                1.86
divide(8192,4096)            1.41e+05    1.72e+05                1.22
divide(131072,65536)         1.62e+03    1.64e+03                1.01
divide                       1.46e+05    2.77e+05                1.90
gcd(128,128)                 7.53e+05    1.82e+06                2.42
gcd(8192,8192)               5.11e+03    5.16e+03                1.01
gcd(1048576,1048576)         4.36e+00    4.37e+00                1.00
gcd                          2.94e+03    3.68e+03                1.25
gcdext(128,128)              3.44e+05    8.40e+05                2.44
gcdext(8192,8192)            3.16e+03    3.20e+03                1.01
gcdext(1048576,1048576)      2.83e+00    2.85e+00                1.01
gcdext                       1.74e+03    2.22e+03                1.28
[base]                       1.93e+04    3.53e+04                1.83
rsa(512)                     1.37e+04    1.49e+04                1.08
rsa(2048)                    4.46e+02    4.48e+02                1.00
rsa                          2.59e+03    2.68e+03                1.04
[app]                        2.59e+03    2.68e+03                1.04
[bench]                      7.06e+03    9.73e+03                1.38

These results are all very exciting for potential users of the gmp gem. All of the tests show the same trend: as the operand size grows, the performance of the gmp gem gets asymptotically closer to the GMP library itself. For example, when multiplying two 128-bit numbers together, the GMP library by itself is more than 40 times as fast as the gmp gem, but this gap shrinks to just 7x when multiplying two 512-bit numbers, and shrinks all the way to 1.02x when multiplying two 2097152-bit numbers.

None of the other programs start off with such a gap between GMP performance and gmp gem performance as the multiplication tests. For example in calculating the GCD between two 128-bit numbers, GMP itself is only 2.4 times as fast as the gmp gem.

These can all be easily explained as Ruby overhead. The Ruby VM and the Ruby garbage collector and all of the dynamic calls are responsible for the gap between GMP and the gmp gem. As the operands get larger, and more CPU time is spent inside the GMP algorithms, the overhead shrinks to almost nothing.

The goal, of course, in future releases of the gmp gem, is to shrink that gap even more. Even though the gap when multiplying two 2097152-bit numbers is negligible, that does not help the developer who is multiplying two 128-bit numbers. Theoretically, improvements in a number of different arenas can help shrink the gap:

* Improvements in "Matz's Ruby Interpreter" may reduce the overhead.
* A different Ruby VM, such as Rubinius and JRuby, may reduce the overhead.
* Compiling Ruby and the gmp gem with an improved C compiler (such as a "modern" GCC, as opposed to GCC 4.2.1, or LLVM) may reduce the overhead.
* Coding improvements in the gmp gem may reduce the overhead. This could include reordered type-checking, and complete bindings for functional forms of theGMP methods.

## Future Plans

There is a lot of work to be done in comparing pure GMP, the gmp gem, and Ruby's Bignum. These plans are not listed in any particular order:

* The `pi` program (benchmark test) needs to be written, in order to compare more closely the gmp gem with GMP.
* Various Bignum methods need to be written more seriously, namely `gcd` and `gcdext`. These can use faster alrogithms, but still exist as Ruby code (see <http://gmplib.org/manual/Greatest-Common-Divisor-Algorithms.html>), or be reimplemented as Ruby C extensions. Also, `Bignum#[]=` should probably be reimplemented as a C extension. All of these would be candidates to contribute back to Ruby Core.
* The `rsa` results between Ruby's Bignum and gmp gem need to be understood.
* All of the tests represented in this report used software compiled with Apple's GCC 4.2.1, which is notoriously a bad choice to compile GMP with. Smoke tests should be conducted against a more modern GCC, such as GCC 4.6.x or GCC 4.7.x. Alternatively, LLVM should compile GMP and Ruby without much difficulty these days.
* There are new releases of both Ruby (2.0.0) and GMP (5.1.0) on the horizon. As previews, betas, and release candidates are made available, some benchmarking should be performed.
* Most (all?) alternative Ruby VMs in the wild today support Ruby C Extensions. These include: JRuby 1.6+, Rubinius 1.1+, MacRuby 0.7+, and MagLev. JRuby and Rubinius, at a minimum, have the real possibility of outperforming MRI, with their different garbage collectors and JIT compilers.
* The results listed in this report were all conducted on Mac OS X 10.6.8. While they should certainly translate _roughly_ to other platforms, an effort should be made to test the gmp gem on other platforms. I don't expect any surprises on BSD or Linux, but coupling GMP, Ruby, and Windows together yield something different. Additionally, I think that testing GMP and Ruby on ARM (on Android, for example) sounds incredibly fun.
