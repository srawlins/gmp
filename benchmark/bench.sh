#! /bin/bash
source ~/.rvm/scripts/rvm

function out {
  echo "========================================"
  echo "$*"
  echo "========================================"
}

echo "RUBY:   `ruby -v`"
echo "GMP:    `ruby -r '../ext/gmp' -e \"puts GMP::GMP_VERSION\"`"
echo "GMP_CC: `ruby -r '../ext/gmp' -e \"puts GMP::GMP_CC\"`"
echo "MPFR:   `ruby -r '../ext/gmp' -e \"puts GMP::MPFR_VERSION\"`"

if [[ $1 == --help ]]; then
  echo "bench.sh benchmark_directory ..."
  exit 1
fi

ruby_version=`ruby -v |cut -d' ' -f 2`
PATH=.:$PATH
for directory in "$@"; do
  out "Benchmarking in $directory..."
  cd $directory
  if [ ! -f runbench ]; then
    echo "Oops, not a benchmark directory! 'runbench' does not exist."
  else
    echo "./runbench -n |tee ${ruby_version}.txt"
    ./runbench -n |tee ${ruby_version}.txt
    cd -
  fi
done
