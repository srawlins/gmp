gmp
===

[![Build Status](https://travis-ci.org/srawlins/gmp.png?branch=master)](https://travis-ci.org/srawlins/gmp)

gmp is a Rubygem providing Ruby bindings to the [GMP](https://gmplib.org/)
library, and to the [MPFR](http://www.mpfr.org/) library (optional). Here is the
introduction paragraph at [GMP's homepage](http://gmplib.org/#WHAT):

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
>
> GMP is faster than any other bignum library. The advantage for GMP increases
> with the operand sizes for many operations, since GMP uses asymptotically
> faster algorithms.
>
> The first GMP release was made in 1991. It is continually developed and
> maintained, with a new release about once a year.
>
> GMP is distributed under the GNU LGPL. This license makes the library free to
> use, share, and improve, and allows you to pass on the result. The license
> gives freedoms, but also sets firm restrictions on the use with non-free
> programs.
>
> GMP is part of the GNU project. For more information about the GNU project,
> please see the official GNU web site.
>
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

### Support

* GMP 4.3.x and GMP 5.x are each supported against Ruby (MRI) 1.9.x, 2.0.0, and
  2.1.0. GMP 5.1.x is the only version of GMP that has been tested recently.
* MPFR 3.x is supported.
* Other Ruby platforms (such as Rubinius and JRuby) should be supported, but
  have not been tested recently.

#### Previously

Previously, on version 0.5.47, this gem was verified as functioning in the
following combinations:

<table border="0">
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
    <td rowspan="3">Mac OS X 10.6.8 on x86_64 (64-bit)</td>
    <td>(MRI) Ruby 1.8.7</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.5 (3.1.1)</td>
  </tr>
  <tr>
    <td>(MRI) Ruby 1.9.3</td>
    <td>GMP 4.3.2 (2.4.2)<br />
        GMP 5.0.5 (3.1.1)</td>
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
* `GMP::GMP_BITS_PER_LIMB` The number of bits per limb
* `GMP::GMP_NUMB_MAX` - The maximum value that can be stored in the number part of a limb

If MPFR is available:
* `GMP::MPFR_VERSION` - A string like "2.4.2"
* `GMP::MPFR_PREC_MIN` - The minimum precision available
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
    GMP::Q.new(Fixnum)
    GMP::Q.new(GMP::Z)
    GMP::Q.new(GMP::Q)
    GMP::Q.new(String)
    GMP::Q.new(any GMP::Z initializer)
    GMP::Q.new(Fixnum, Fixnum)
    GMP::Q.new(any GMP::Z initializer, any GMP::Z initializer)
    GMP::F.new()
    GMP::F.new(GMP::Z, precision=0, rounding_mode=default)
    GMP::F.new(GMP::Q, precision=0, rounding_mode=default)
    GMP::F.new(GMP::F, precision=0, rounding_mode=default)
    GMP::F.new(String, precision=0)
    GMP::F.new(String, precision=0, base=10)
    GMP::F.new(String, precision=0, base=10, rounding_mode=default)
    GMP::F.new(Fixnum, precision=0, rounding_mode=default)
    GMP::F.new(Bignum, precision=0, rounding_mode=default)
    GMP::F.new(Float,  precision=0, roundung_mode=default)
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
      to_s                     convert to string. For GMP::Z and GMP::F, this
                               method takes one optional argument, a base. The
                               base can be a Fixnum in the ranges \[2, 62\] or
                               \[-36, -2\] or a Symbol: one of :bin, :oct,
                               :dec, or :hex.
      inspect                  alias for #to_s
      -@                       negation
      neg!                     in-place negation
      abs                      absolute value
      abs!                     in-place absolute value
      coerce                   promotion of arguments
      ==                       equality test
      <=>, >=, >, <=, <        comparisions
    class methods of GMP::Z
      fac(n)                   factorial of n
      2fac(n), double_fac(n)   double factorial of n
      mfac(n)                  m-multi-factorial of n
      primorial(n)             primorial of n
      fib(n)                   nth Fibonacci number
      fib2(n)                  nth and (n-1)th Fibonacci numbers
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
      congruent?(c,d)          true if congruent to c modulus d
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
      sqrtrem                  square root, with remainder
      root(n)                  nth root
      rootrem(n)               nth root, with remainder
      probab_prime?            0 if composite, 1 if probably prime, 2 if
                               certainly prime
      nextprime                next *probable* prime
      nextprime!               change the object into its next *probable* prime
      gcd, gcdext, gcdext2     greatest common divisor
      lcm                      least common multiple
      invert(m)                invert mod m
      jacobi                   jacobi symbol
      legendre                 legendre symbol
      remove(n)                remove all occurences of factor n
      popcount                 the number of bits equal to 1
      hamdist                  the hamming distance between two integers
      out_raw                  output to IO object
      inp_raw                  input from IO object
      export                   export to a byte array (String)
      import                   import from a byte array (String)
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
      fac(n)                   new GMP::F, equal to factorial of n
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
      nan                      returns NaN
      inf(sign = 1)            returns Inf or -Inf
      zero(sign = 1)           returns zero or -zero
      const_log2               returns the natural log of 2
      const_pi                 returns pi
      const_euler              returns euler
      const_catalan            returns catalan
      mpfr_buildopt_tls_p      returns whether MPFR was built as thread safe
      mpfr_buildopt_decimal_p  returns whether MPFR was compiled with decimal
                               float support
    GMP::F
      frexp                    frexp
      lessgreater?(y)          x < y or y < x?
      unordered?(y)            either x or y is NaN?
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
      mpfr_urandomb(prec = default)   get uniformly distributed random floating-point
                                      number within 0 <= rop < 1
      mpfr_urandom(prec = default)    get uniformly distributed random floating-point
                                      number (MPFR_VERSION >= "3.0.0")

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
      .congruent?   .divexact     .divisible?   .fdiv_q_2exp  .fdiv_r_2exp  .lcm
      .mul          .mul_2exp
      .neg          .nextprime    .sqrt         .sub          .submul       .tdiv_q_2exp
      .tdiv_r_2exp

Method Tables
-------------

The following is organized in the same categories as in the GMP and MPFR manuals.

### Integer Functions

<table>
  <tr><th colspan="2"><h4>Assigning Integers</h4></th></tr>
  <tr><th>GMP Function</th><th>GMP::Z method</th></tr>
  <tr><td>mpz_swap</td><td>GMP::Z#swap</td></tr>

  <tr><th colspan="2"><h4>Converting Integers</h4>
                      4x C functions mapped to 3x Ruby methods; 1x unmapped C function</th></tr>
  <tr><th>GMP Function</th><th>GMP::Z method</th></tr>
  <tr><td>mpz_get_ui<br />
          mpz_get_si</td>      <td>GMP::Z#to_i</td></tr>
  <tr><td>mpz_get_d</td>       <td>GMP::Z#to_d</td></tr>
  <tr><td>mpz_get_d_2exp</td>  <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_get_str</td>     <td>GMP::Z#to_s</td></tr>

  <tr><th colspan="2"><h4>Integer Arithmetic</h4>
                      12x C functions mapped to 19x Ruby methods; 2x unmapped C functions</th></tr>
  <tr><th>GMP Function</th><th>GMP::Z method</th></tr>
  <tr><td>mpz_add<br />
          mpz_add_ui</td>               <td>GMP::Z#+<br />
                                            GMP::Z#add! (destructive method)<br />
                                            GMP::Z.add (in-place singleton method)</td></tr>
  <tr><td>mpz_sub<br />
          mpz_sub_ui<br />
          mpz_ui_sub (never used)</td>  <td>GMP::Z#-<br />
                                            GMP::Z#sub! (destructive method)<br />
                                            GMP::Z.sub (in-place singleton method)</td></tr>
  <tr><td>mpz_mul<br />
          mpz_mul_si<br />
          mpz_mul_ui (not used yet)</td><td>GMP::Z#*<br />
                                            GMP::Z.mul (in-place singleton method)</td></tr>
  <tr><td>mpz_addmul<br />
          mpz_addmul_ui</td>            <td>GMP::Z#addmul! (destructive method)<br />
                                            GMP::Z.addmul (in-place singleton method)</td></tr>
  <tr><td>mpz_submul<br />
          mpz_submul_ui</td>            <td>GMP::Z#submul! (destructive method)<br />
                                            GMP::Z.submul (in-place singleton method)</td></tr>
  <tr><td>mpz_mul_2exp</td>             <td>GMP::Z.mul_2exp (in-place singleton method)</td></tr>
  <tr><td>mpz_neg</td>                  <td>GMP::Z@-<br />
                                            GMP::Z#neg! (destructive method)<br />
                                            GMP::Z.neg (in-place singleton method)</td></tr>
  <tr><td>mpz_abs</td>                  <td>GMP::Z#abs<br />
                                            GMP::Z#abs! (destructive method)<br />
                                            GMP::Z.abs (in-place singleton method)</td></tr>

  <tr><th colspan="2"><h4>Integer Division</h4>
                      26x C functions mapped to 18x Ruby methods; 11x unmapped C functions</th></tr>
  <tr><th>GMP Function</th><th>GMP::Z method</th></tr>
  <tr><td>mpz_cdiv_q<br />
          mpz_cdiv_q_ui</td>            <td>GMP::Z#cdiv</td></tr>
  <tr><td>mpz_cdiv_r<br />
          mpz_cdiv_r_ui</td>            <td>GMP::Z#cmod</td></tr>
  <tr><td>mpz_cdiv_qr<br />
          mpz_cdiv_qr_ui</td>           <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_cdiv_ui</td>              <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_cdiv_q_2exp<br />
          mpz_cdiv_r_2exp</td>          <td>GMP::Z.cdiv_q_2exp (in-place singleton method)<br />
                                            GMP::Z.cdiv_r_2exp (in-place singleton method)</td></tr>
  <tr><td>mpz_fdiv_q<br />
          mpz_fdiv_q_ui</td>            <td>GMP::Z#fdiv</td></tr>
  <tr><td>mpz_fdiv_r<br />
          mpz_fdiv_r_ui</td>            <td>GMP::Z#fmod</td></tr>
  <tr><td>mpz_fdiv_qr<br />
          mpz_fdiv_qr_ui</td>           <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_fdiv_ui</td>              <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_fdiv_q_2exp<br />
          mpz_fdiv_r_2exp</td>          <td>GMP::Z.fdiv_q_2exp (in-place singleton method)<br />
                                            GMP::Z.fdiv_r_2exp (in-place singleton method)</td></tr>
  <tr><td>mpz_tdiv_q<br />
          mpz_tdiv_q_ui</td>            <td>GMP::Z#tdiv</td></tr>
  <tr><td>mpz_tdiv_r<br />
          mpz_tdiv_r_ui</td>            <td>GMP::Z#tmod</td></tr>
  <tr><td>mpz_tdiv_qr<br />
          mpz_tdiv_qr_ui</td>           <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_tdiv_ui</td>              <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_tdiv_q_2exp<br />
          mpz_tdiv_r_2exp</td>          <td>GMP::Z.tdiv_q_2exp (in-place singleton method)<br />
                                            GMP::Z.tdiv_r_2exp (in-place singleton method)</td></tr>
  <tr><td>mpz_mod<br />
          mpz_mod_ui</td>               <td>GMP::Z#mod</td></tr>
  <tr><td>mpz_divexact<br />
          mpz_divexact_ui</td>          <td>GMP::Z.divexact (in-place singleton method)</td></tr>
  <tr><td>mpz_divisible_p<br />
          mpz_divisible_ui_p</td>       <td>GMP::Z#divisible?<br />
                                            GMP::Z.divisible? (in-place singleton method)</td></tr>
  <tr><td>mpz_divisible_2exp_p</td>     <td><em>not implemented yet</em></td></tr>
  <tr><td>mpz_congruent_p<br />
          mpz_congruent_ui_p</td>       <td>GMP::Z#congruent?<br />
                                            GMP::Z.congruent? (in-place singleton method)</td></tr>
  <tr><td>mpz_congruent_2exp_p</td>     <td><em>not implemented yet</em></td></tr>

  <tr><th colspan="2"><h4>Integer Exponentiation</h4>

  <tr><th colspan="2"><h4>Integer Roots</h4>

  <tr><th colspan="2"><h4>Number Theoretic Functions</h4>

  <tr><th colspan="2"><h4>Integer Comparisons</h4>

  <tr><th colspan="2"><h4>Integer Logic and Bit Fiddling</h4>

  <tr><th colspan="2"><h4>I/O of Integers</h4>

  <tr><th colspan="2"><h4>Integer Random Numbers</h4>

  <tr><th colspan="2"><h4>Integer Import and Export</h4>

  <tr><th colspan="2"><h4>Miscellaneous Integer Functions</h4>
</table>

Documentation
-------------

* [This README](https://github.com/srawlins/gmp)
* Loren Segal and the guys at RubyGems.org are awesome: [YARDoc](http://rubydoc.info/gems/gmp/frames).
* There should be a manual.pdf [here](https://github.com/srawlins/gmp/blob/master/manual.pdf). I spend waaay too much time working on this, but it looks very pretty.
* [CHANGELOG](https://github.com/srawlins/gmp/blob/master/CHANGELOG)

Testing
-------

Tests can be run with `rake test`. I like to run tests and print out a quick report of Ruby, GMP, and MPFR versions with `rake report`. You can precede any rake task with `GMP=gmp_install_dir` and `MPFR=mpfr_install_dir` (or `MPFR=--no-mpfr`) to pass `--with-gmp-dir` and `--with-mpfr-dir` options to `extconf.rb` as well.

You can also run tests from an individual file with:

    cd test
    ruby some_test_file.rb

Known Issues
------------

Don't call `GMP::RandState(:lc_2exp_size)`. Give a 2nd arg.

Don't use multiple assignment (`a = b = GMP::Z(0)`) with functional mappings:

```ruby
    >> a = b = GMP::Z(0)
    => 0
    >> GMP::Z.mul(a, GMP::Z(2), GMP::Z(3))
    => nil
    >> a
    => 6
    >> b
    => 6
```

JRuby has some interesting bugs and flickering tests. GMP::Z(GMP::GMP_NUMB_MAX) for example, blows up.

Precision
---------

Precision can be explicitely set as second argument for `GMP::F.new()`. If there is no explicit precision, highest precision of all `GMP::F` arguments is used. That doesn't ensure that result will be exact. For details, consult any paper about floating point arithmetics.

Default precision can be explicitely set by passing `0` as the second argument for to `GMP::F.new()`. In particular, you can set precision of copy of `GMP::F` object by:

    new_obj = GMP::F.new(old_obj, 0)

Precision argument, and default_precision will be rounded up to whatever GMP thinks is appropriate.

Benchmarking
------------

Please see [performance](https://github.com/srawlins/gmp/raw/master/performance.2012.pdf) on GitHub.

License
-------

This gem is now licensed under the [Apache License Version 2.0](COPYING.md).

Todo
----

* `GMP::Z#to_d_2exp`, `#kronecker`, `#bin`, `::lucnum2`, `#combit`, `#fits_x?`
* `GMP::Q#to_s(base)`, `GMP::F#to_s(base)` (test it!)
* benchmark pi
* a butt-load of functional mappings. 47-ish sets.
* investigate possible memory leaks when using `GMP::Q(22/7)` for example
* beef up `r_gmpq_initialize`; I don't like to rely on `mpz_set_value`.
* finish compile-results.rb
* check if `mpz_mul_ui` would optimize `GMP::Z#*`
* New in MPFR 3.1.0: mpfr_frexp, mpfr_grandom, mpfr_z_sub, divide-by-zero exception (?)
* JRuby doesn't like some MPFR stuff, specifically sqrt tests fail.
* Rubinius in 1.9 mode fails a sprintf test in a minor way.

The below are inherited from Tomasz. I will go through these and see which are
still relevant, and which I understand.

* `mpz_fits_*` and 31 vs. 32 integer variables
* check if `mpz_addmul_ui` would optimize some statements
* some system that allows using denref and numref as normal ruby objects
* takeover code that replaces all `Bignums` with `GMP::Z`
* better bignum parser (crawling into the Bignum extension)
* zero-copy method for strings generation
* benchmarks against Python GMP and Perl GMP
* `dup` methods for GMP::Q and GMP::F
* integrate `F` into system
* should `Z.\[\]` bits be 0/1 or true/false, 0 is true, which might surprise users
* `any2small_integer()`
* powm with negative exponents
* check if different sorting of operations gives better cache usage
* `GMP::\*` op `RubyFloat` and `RubyFloat` op `GMP::\*`
