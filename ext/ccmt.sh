#! /bin/bash
source ~/.rvm/scripts/rvm

extconf_opts=""
if [[ $1 == -* ]]; then
  if [ "$1" == "-no-mpfr" ]; then
    extconf_opts="$extconf_opts $1"
  fi
  shift
fi

if [ $# -gt 0 ]; then
  rvm use $1
  if [ $? -ne 0 ]; then
    echo "ERROR: rvm doesn't like \"$1\". Quitting."
    exit 1
  fi
fi

if [ $# -gt 1 ]; then
  gmp_dir="--with-gmp-dir=/usr/local/gmp-$2"
fi

if [ $# -gt 2 ]; then
  mpfr_dir="--with-mpfr-dir=$3"
fi

make clean
ruby extconf.rb $extconf_opts $gmp_dir $mpfr_dir
if [ $? -ne 0 ]; then
  echo "ERROR: ruby extconf.rb didn't work so hot. Quitting."
  exit 2
fi

make
if [ $? -ne 0 ]; then
  echo "ERROR: make didn't work so hot. Quitting."
  exit 3
fi

cd ../test
ruby unit_tests.rb
cd ../ext

echo "RUBY: `ruby -v`"
echo "GMP:  `ruby -r './gmp' -e \"puts GMP::GMP_VERSION\"`"
if [ -z `echo "$extconf_opts" | grep "no-mpfr"` ]; then
  echo "MPFR: `ruby -r './gmp' -e \"puts GMP::MPFR_VERSION\"`"
else
  echo "MPFR: -no-mpfr"
fi

