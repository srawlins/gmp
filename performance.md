<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<style>
body {
  margin: 0 auto;
  width: 800px;
}

table {
  border-collapse: collapse;
  width:96%;
}

th {
  font-weight: normal;
  text-align: right;
  white-space: nowrap;
}

tr.new-method th {
  border-top: solid 2px black;
}

th:first-child {
  text-align: left;
  width: 0.8in;
}

tr.last-header th {
  border-bottom: solid 1px black;
}

tr.last-header th:first-child {
  border-bottom: 0;
}

td:last-child {
  padding-bottom: 1em;
}

pre {
  background-color: #EEEEEE;
  padding: 3px 2px;
</style>

# GMP gem Performance

Performance analysis of the GMP gem

28 November 2012

\vfill

written by Sam Rawlins

\newpage

\tableofcontents

\newpage

## Introduction to the performance benchmarks

The benchmarking system used to test the performance of the gmp gem is inspired by, and uses parts of, gmpbench 0.2. http://gmplib.org/gmpbench.html. gmpbench consists of two parts:

* `multiply`, `divide`, `gcd`, `gcdext`, `rsa`, and `pi` are 6 small programs that use GMP to measure a specific piece of functionality. `multiply`, `divide`, `gcd`, and `gcdext` are the "base" benchmarks that test small pieces of functionality. `rsa` and `pi` are the "application" benchmarks that measure the performance of a larger concept implemented with GMP.
* `runbench` is a shell script that coordinates an execution of each of the benchmarking programs, applying a weight to the results of each, and yielding a total score for GMP on the current system.

The benchmarking system in the gmp gem uses Ruby versions of each of the 6 programs (actually, `gcdext`, and `pi` are still being ported), attempting to be as identical to their C code siblings. This system also just uses `runbench` unmodified from the original gmpbench suite.

Due to a few issues with Ruby 1.8.7, and the gmp gem itself, there are actually 3x different versions of the benchmark suite that use the gmp gem:

* `benchmark/gmp/bin_op` uses binary operators, such as `*`, on `GMP::Z` integers. Since `a * b` creates a new `mpz_t` that it returns, the benchmark programs are constantly creating new objects, which is not what the GMP benchmark programs do. The real problem that this creates is Ruby 1.8.7 running out of memory.
* `benchmark/gmp/gc` also uses binary operators, but invokes Ruby's garbage collector every 512 iterations of each test. This allows all of the benchmarks to complete in Ruby 1.8.7, but is still not the best comparison with GMP's benchmark programs.
* `benchmark/gmp/functional` uses the "functional", `GMP::Z` singleton methods to perform what would otherwise be binary operations. For example, `x * y` is replaced with `GMP::Z.mul(z,x,y)` in order to use `z` as the "return argument" through each iteration of a benchmark.

### Run the Benchmarks

In order to run a set of benchmarks (a directory containing `multiply`, `runbench`, etc.), just use the command:

    ./runbench -n

### `Bignum#gcd`

`Bignum#gcd` is not provided by Ruby's standard library. A simple and fast enough version is provided in `benchmark/ruby/gcd`.

## Complex Functions

### Initializing, Assigning Complex Numbers

\begin{tabular}{p{\methwidth} l r}
\toprule
\textbf{new} & & MPC.new $\rightarrow$ \textit{integer} \\
& & MPC.new(\textit{numeric = 0}) $\rightarrow$ \textit{integer} \\
& & MPC.new(\textit{str}, \textit{base = 0}) $\rightarrow$ \textit{integer} \\
\cmidrule(r){2-3}
& \multicolumn{2}{p{\defnwidth}}{
  This method creates a new \texttt{MPC} integer. It typically takes one optional argument for
  the value of the integer. This argument can be one of several classes. If the first
  argument is a String, then a second argument, the base, may be optionally supplied.
  Here are some examples:\newline

  \texttt{MPC.new \qqqquad\qqqquad \#=> 0 (default) \newline
          MPC.new(1) \qqqquad\qquad\  \#=> 1 (Ruby Fixnum) \newline
          MPC.new("127") \qqqquad\  \#=> 127 (Ruby String)\newline
          MPC.new("FF", 16) \qquad\ \  \#=> 255 (Ruby String with base)\newline
          MPC.new("1Z", 36) \qquad\ \  \#=> 71 (Ruby String with base)\newline
          MPC.new(4294967296) \qquad \#=> 4294967296 (Ruby Bignum)\newline
          MPC.new(GMP::Z.new(31))  \#=> 31 (GMP Integer)}
}
\end{tabular}

\ifdef{HTML}
<table>
  <tr class='new-method'>
    <th>new</th><th>`MPC.new` $\rightarrow$ _integer_</th>
  </tr>
  <tr>
    <th></th>   <th><code>MPC.new(<em>numeric=0</em>)</code> $\rightarrow$ _integer_</th>
  </tr>
  <tr class='last-header'>
    <th></th>   <th><code>MPC.new(<em>string</em>)</code> $\rightarrow$ _integer_</th>
  </tr>
  <tr>
    <td></td>
    <td>
      This method creates a new `MPC.new` integer. It typically takes one optional
argument for the value of the integer. This argument can be one of several
classes. If the first argument is a String, then a second argument, the base,
may be optionally supplied. Here are some examples:

<pre><code>MPC.new                  #=> 0 (default)
MPC.new(1)               #=> 1 (Ruby Fixnum)
MPC.new("127")           #=> 127 (Ruby String)
MPC.new("FF", 16)        #=> 255 (Ruby String with base)
MPC.new("1Z", 36)        #=> 71 (Ruby String with base)
MPC.new(4294967296)      #=> 4294967296 (Ruby Bignum)
MPC.new(GMP::Z.new(31))  #=> 31 (GMP Integer)
</code></pre>
    </td>
  </tr>
</table>
\endif

