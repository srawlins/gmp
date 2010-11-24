gmp
===

gmp is library providing Ruby bindings to GMP library. Here is the introduction
paragraph at [their homepage](http://gmplib.org/#WHAT):

> GMP is a free library for arbitrary precision arithmetic, operating on
> signed integers, rational numbers, and floating point numbers. There is no
> practical limit to the precision except the ones implied by the available
> memory in the machine GMP runs on. GMP has a rich set of functions, and the
> functions have a regular interface.
> 
> The main target applications for GMP are cryptography applications and
> research, Internet security applications, algebra systems, computational
> algebra research, etc.
>
> GMP is carefully designed to be as fast as possible, both for small operands
> and for huge operands. The speed is achieved by using fullwords as the basic
> arithmetic type, by using fast algorithms, with highly optimised assembly
> code for the most common inner loops for a lot of CPUs, and by a general
> emphasis on speed.

> GMP is faster than any other bignum library. The advantage for GMP increases
> with the operand sizes for many operations, since GMP uses asymptotically
> faster algorithms.

> The first GMP release was made in 1991. It is continually developed and
> maintained, with a new release about once a year.

> GMP is distributed under the GNU LGPL. This license makes the library free to
> use, share, and improve, and allows you to pass on the result. The license
> gives freedoms, but also sets firm restrictions on the use with non-free
> programs.

> GMP is part of the GNU project. For more information about the GNU project,
> please see the official GNU web site.

> GMP's main target platforms are Unix-type systems, such as GNU/Linux,
> Solaris, HP-UX, Mac OS X/Darwin, BSD, AIX, etc. It also is known to work on
> Windoze in 32-bit mode.
> 
> GMP is brought to you by a team listed in the manual.
> 
> GMP is carefully developed and maintained, both technically and legally. We
> of course inspect and test contributed code carefully, but equally
> importantly we make sure we have the legal right to distribute the
> contributions, meaning users can safely use GMP. To achieve this, we will ask
> contributors to sign paperwork where they allow us to distribute their work."

Only GMP 4 or newer is supported. The following environments have been tested
by me: gmp gem 0.5.41 on:

<table border="1">
  <tr>
    <th>Platform</th>
    <th>Ruby</th>
    <th>GMP (MPFR)</th>
  </tr>
  <tr>
    <td rowspan="3">Linux (Ubuntu NR 10.04) on x86 (32-bit)</td>
    <td>(MRI) Ruby 1.8.7</td>
    <td>GMP 4.3.1 (2.4.2)<br />
        GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.1</td>
    <td>GMP 4.3.1 (2.4.2)<br />
        GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.2</td>
    <td>GMP 4.3.1 (2.4.2)<br />
        GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td rowspan="4">Linux (Ubuntu 10.04) on x86_64 (64-bit)</td>
    <td>(MRI) Ruby 1.8.7</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.1</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.2</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(RBX) Rubinius 1.1.0</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td rowspan="4">Mac OS X 10.6.4 on x86_64 (64-bit)</td>
    <td>(MRI) Ruby 1.8.7</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.1</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.2</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(RBX) Rubinius 1.1.0</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td rowspan="3">Windows 7 on x86_64 (64-bit)</td>
    <td>(MRI) Ruby 1.8.7</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.1</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.2</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
  <tr>
    <td>Windows XP on x86 (32-bit)</td>
    <td>(MRI) Ruby 1.9.1</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.1 (3.0.0)</td>
  </tr>
</table>

Authors
-------

* Tomasz Wegrzanowski
* srawlins

Constants
---------

The GMP module includes the following constants. Mathematical constants, such
as pi, are defined under class methods of GMP::F, listed below.

* `GMP::GMP_VERSION` - A string like "5.0.1"
* `GMP::GMP_CC` - The compiler used to compile GMP
* `GMP::GMP_CFLAGS` - The CFLAGS used to compile GMP
* `GMP::GMP\_BITS_PER_LIMB` The number of bits per limb
* `GMP::GMP_NUMB_MAX` - The maximum value that can be stored in the number part of a limb

if MPFR is available:
* `GMP::MPFR_VERSION` - A string like "2.4.2"
* `GMP::MPFR\_PREC_MIN` - The minimum precision available
* `GMP::MPFR_PREC_MAX` - The maximum precision available
* `GMP::GMP_RNDN` - The constant representing "round to nearest"
* `GMP::GMP_RNDZ` - The constant representing "round toward zero"
* `GMP::GMP_RNDU` - The constant representing "round toward plus infinity"
* `GMP::GMP_RNDD` - The constant representing "round toward minus infinity"

New in MPFR 3.0.0:
* `GMP::MPFR_RNDN`
* `GMP::MPFR_RNDZ`
* `GMP::MPFR_RNDU`
* `GMP::MPFR_RNDD`
* `GMP::MPFR_RNDA` - The constant representing "round away from zero"

Classes
-------

The GMP module is provided with following classes:
* `GMP::Z` - infinite precision integer numbers
* `GMP::Q` - infinite precision rational numbers
* `GMP::F` - arbitrary precision floating point numbers
* `GMP::RandState` - states of individual random number generators

Numbers are created by using `new()`. Constructors can take following arguments:

    GMP::Z.new()
    GMP::Z.new(GMP::Z)
    GMP::Z.new(Fixnum)
    GMP::Z.new(Bignum)
    GMP::Z.new(String)
    GMP::Q.new()
    GMP::Q.new(GMP::Q)
    GMP::Q.new(String)
    GMP::Q.new(any GMP::Z initializer)
    GMP::Q.new(any GMP::Z initializer, any GMP::Z initializer)
    GMP::F.new()
    GMP::F.new(GMP::Z, precision=0)
    GMP::F.new(GMP::Q, precision=0)
    GMP::F.new(GMP::F)
    GMP::F.new(GMP::F, precision)
    GMP::F.new(String, precision=0)
    GMP::F.new(Fixnum, precision=0)
    GMP::F.new(Bignum, precision=0)
    GMP::F.new(Float,  precision=0)
    GMP::RandState.new(\[algorithm\] \[, algorithm_args\])

You can also call them as:

    GMP.Z(args)
    GMP.Q(args)
    GMP.F(args)
    GMP.RandState()

Methods
-------

    GMP::Z, GMP::Q and GMP::F
      +                        addition
      -                        substraction
      *                        multiplication
      /                        division
      to_s                     convert to string. For GMP::Z, this method takes
                               one optional argument, a base. The base can be a
                               Fixnum in the ranges \[2, 62\] or \[-36, -2\] or a
                               Symbol: :bin, :oct, :dec, or :hex.
      -@                       negation
      neg!                     in-place negation
      abs                      absolute value
      asb!                     in-place absolute value
      coerce                   promotion of arguments
      ==                       equality test
      <=>,>=,>,<=,<            comparisions
    class methods of GMP::Z
      fac(n)                   factorial of n
      fib(n)                   nth fibonacci number
      pow(n,m)                 n to mth power
    GMP::Z and GMP::Q
      swap                     efficiently swap contents of two objects, there
                               is no GMP::F.swap because various GMP::F objects
                               may have different precisions, which would make
                               them unswapable
    GMP::Z
      to_i                     convert to Fixnum or Bignum
      add!                     in-place addition
      sub!                     in-place subtraction
      addmul!(b,c)             in-place += b*c
      submul!(b,c)             in-place -= b*c
      tdiv,fdiv,cdiv           truncate, floor and ceil division
      tmod,fmod,cmod           truncate, floor and ceil modulus
      >>                       shift right, floor
      divisible?(b)            true if divisible by b
      **                       power
      powmod                   power modulo
      \[\],\[\]=                   testing and setting bits (as booleans)
      scan0,scan1              starting at bitnr (1st arg), scan for a 0 or 1
                               (respectively), then return the index of the
                               first instance.
      cmpabs                   comparison of absolute value
      com                      2's complement
      com!                     in-place 2's complement
      &,|,^                    logical operations: and, or, xor
      even?                    is even
      odd?                     is odd
      <<                       shift left
      tshr                     shift right, truncate
      lastbits_pos(n)          last n bits of object, modulo if negative
      lastbits_sgn(n)          last n bits of object, preserve sign
      power?                   is perfect power
      square?                  is perfect square
      sqrt                     square root
      sqrt!                    change the object into its square root
      sqrtrem                  square root, remainder
      root(n)                  nth root
      probab_prime?            0 if composite, 1 if probably prime, 2 if
                               certainly prime
      nextprime                next *probable* prime
      nextprime!               change the object into its next *probable* prime
      gcd, gcdext              greatest common divisor
      invert(m)                invert mod m
      jacobi                   jacobi symbol
      legendre                 legendre symbol
      remove(n)                remove all occurences of factor n
      popcount                 the number of bits equal to 1
      sizeinbase(b)            digits in base b
      size_in_bin              digits in binary
      size                     number of limbs
    GMP::Q
      num                      numerator
      den                      denominator
      inv                      inversion
      inv!                     in-place inversion
      floor,ceil,trunc         nearest integer
    class methods of GMP::F
      default_prec             get default precision
      default_prec=            set default precision
    GMP::F
      prec                     get precision
      floor,ceil,trunc         nearest integer, GMP::F is returned, not GMP::Z
      floor!,ceil!,trunc!      in-place nearest integer
    GMP::RandState
      seed(integer)            seed the generator with a Fixnum or GMP::Z
      urandomb(fixnum)         get uniformly distributed random number between 0
                               and 2^fixnum-1, inclusive
      urandomm(integer)        get uniformly distributed random number between 0
                               and integer-1, inclusive
    GMP (timing functions for GMPbench (0.2))
      cputime                  milliseconds of cpu time since Ruby start
      time                     times the execution of a block
  
    *only if MPFR is available*
    class methods of GMP::F
      const_log2               returns the natural log of 2
      const_pi                 returns pi
      const_euler              returns euler
      const_catalan            returns catalan
    GMP::F
      sqrt                     square root of the object
      rec_sqrt                 square root of the recprical of the object
      cbrt                     cube root of the object
      **                       power
      log                      natural logarithm of object
      log2                     binary logarithm of object
      log10                    decimal logarithm of object
      exp                      e^object
      exp2                     2^object
      exp10                    10^object
      log1p                    the same as (object + 1).log, with better
                               precision
      expm1                    the same as (object.exp) - 1, with better
                               precision
      eint                     exponential integral of object
      li2                      real part of the dilogarithm of object
      gamma                    Gamma fucntion of object
      lngamma                  logarithm of the Gamma function of object
      digamma                  Digamma function of object (MPFR_VERSION >= "3.0.0")
      zeta                     Reimann Zeta function of object
      erf                      error function of object
      erfc                     complementary error function of object
      j0                       first kind Bessel function of order 0 of object
      j1                       first kind Bessel function of order 1 of object
      jn                       first kind Bessel function of order n of object
      y0                       second kind Bessel function of order 0 of object
      y1                       second kind Bessel function of order 1 of object
      yn                       second kind Bessel function of order n of object
      agm                      arithmetic-geometric mean
      hypot
      cos                      \
      sin                      |
      tan                      |
      sin_cos                  |
      sec                      |
      csc                      |
      cot                      |
      acos                     |
      asin                     |
      atan                     | trigonometric functions
      atan2                    |
      cosh                     | of the object
      sinh                     |
      tanh                     |
      sinh_cosh                |
      sec                      |
      csc                      |
      cot                      |
      acosh                    |
      asinh                    |
      atanh                    /
      nan?                     \
      infinite?                | type of floating point number
      finite?                  |
      number?                  |
      regular?                 / (MPFR_VERSION >= "3.0.0")
    GMP::RandState
      mpfr_urandomb(fixnum)    get uniformly distributed random floating-point
                               number within 0 <= rop < 1

Functional Mappings
-------------------

In order to align better with the GMP paradigms of using return arguments, I have started creating "functional mappings", singleton methods that map directly to functions in GMP. These methods take return arguments, so that passing an object to a functional mapping may change the object itself. For example:

    a = GMP::Z(0)
    b = GMP::Z(13)
    c = GMP::Z(17)
    GMP::Z.add(a, b, c)
    a  #=> 30

Here's a fun list of all of the functional mappings written so far:

    GMP::Z
      .abs          .add          .addmul       .cdiv_q_2exp  .cdiv_r_2exp  .com
      .divexact     .divisible?   .fdiv_q_2exp  .fdiv_r_2exp  .lcm          .mul
      .mul_2exp
      .neg          .nextprime    .sqrt         .sub          .submul       .tdiv_q_2exp
      .tdiv_r_2exp

Documentation
-------------

* [This README](https://github.com/srawlins/gmp)
* Loren Segal and the guys at RubyGems.org are badasses: [YARDoc](http://rubydoc.info/gems/gmp/frames).
* There should be a manual.pdf [here](https://github.com/srawlins/gmp/blob/master/manual.pdf). I spend waaay too much time working on this, but it looks very pretty.
* [CHANGELOG](https://github.com/srawlins/gmp/blob/master/CHANGELOG)

Testing
-------

Tests can be run with:

    cd test
    ruby unit_tests.rb

If you have the unit\_test gem installed, all tests should pass. Otherwise, one test may error. I imagine there is a bug in Ruby's built-in `Test::Unit` package that is fixed with the unit_test gem.

Known Issues
------------

* Don't call `GMP::RandState(:lc_2exp_size)`. Give a 2nd arg.
* Don't use multiple assignment (`a = b = GMP::Z(0)`) with functional mappings.

Precision
---------

Precision can be explicitely set as second argument for `GMP::F.new()`. If there is no explicit precision, highest precision of all `GMP::F` arguments is used. That doesn't ensure that result will be exact. For details, consult any paper about floating point arithmetics.

Default precision can be explicitely set by passing `0` as the second argument for to `GMP::F.new()`. In particular, you can set precision of copy of `GMP::F` object by:

    new_obj = GMP::F.new(old_obj, 0)

Precision argument, and default_precision will be rounded up to whatever GMP thinks is appropriate.

Benchmarking
------------

"GMP is carefully designed to be as fast as possible." Therefore, I believe it is very important for GMP, and its various language bindings to be benchmarked. In recent years, the GMP team developed GMPbench, an elegant, weighted benchmark. Currently, they maintain a list of recent benchmark [results](http://www.gmplib.org/gmpbench.html), broken down by CPU, CPU freq, ABI, and compiler flags; GMPbench compares different processor's performance against eachother, rather than GMP against other bignum libraries, or comparing different versions of GMP.

I intend to build a plug-in to GMPbench that will test the ruby gmp gem. The results of this benchmark should be directly comparable with the results of GMP (on same CPU, etc.). Rather than write a benchmark from the ground up, or try to emulate what GMPbench does, a plug-in will allow for this type of comparison. And in fact, GMPbench is (perhaps intentionally) written perfectly to allow for plugging in.

Various scores are derived from GMPbench by running the `runbench` script. This script compiles and runs various individual programs that measure the performance of base functions, such as multiply, and app functions such as rsa.

The gmp gem benchmark uses the GMPbench framework (that is, runbench, gexpr, and the timing methods), and plugs in ruby scripts as the individual programs. Right now, there are only four (of six) such plugged in ruby scripts:

* multiply - measures performance of multiplying (or squaring) `GMP::Z` objects whose size (in bits) is given by one or two operands.
* divide - measures performance of dividing two `GMP::Z` objects (using `tdiv`) whose size (in bits) is given by two operands.
* gcd - measure...
* rsa - measures performance of using RSA to sign messages. The size of `pq`, the product of the two co-prime `GMP::Z` objects, `p` and `q`, is given by one operand.

**insert table here**

My guess is that the increase in ruby gmp gem overhead is caused by increased efficiency in GMP; the inefficiencies of the gmp gem are relatively greater.

Todo
----

* `GMP::Z#to_d_2exp`, `#congruent?`, `#rootrem`, `#lcm`, `#kronecker`, `#bin`, `#fib2`, `#lucnum`, `#lucnum2`, `#hamdist`, `#combit`, `#fits_x?`
* `GMP::Q#to_s(base)`, `GMP::F#to_s(base)` (test it!)
* benchmark gcdext, pi
* a butt-load of functional mappings. 47-ish sets.
* use Rake use Rake use Rake
* investigate possible memory leaks when using `GMP::Q(22/7)` for example
* beef up `r_gmpq_initialize`; I don't want to rely on `mpz_set_value`.
* finish compile-results.rb

The below are inherited from Tomasz. I will go through these and see which are
still relevant, and which I understand.

* `mpz_fits_*` and 31 vs. 32 integer variables
* fix all sign issues (don't know what these are)
* `to_s` vs. `inspect`
* check if `mpz_addmul_ui` would optimize some statements
* some system that allows using denref and numref as normal ruby objects (?)
* takeover code that replaces all `Bignums` with `GMP::Z`
* better bignum parser (how? `to_s` seems good to me.)
* zero-copy method for strings generation
* benchmarks against Python GMP and Perl GMP
* `dup` methods
* integrate `F` into system
* should `Z.\[\]` bits be 0/1 or true/false, 0 is true, which might surprise users
* `any2small_integer()`
* check asm output, especially local memory efficiency
* it might be better to use 'register' for some local variables
* powm with negative exponents
* check if different sorting of operatations gives better cache usage
* `GMP::\*` op `RubyFloat` and `RubyFloat` op `GMP::\*`
* sort checks
