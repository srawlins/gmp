#! /bin/bash
source ~/.rvm/scripts/rvm

if [ $# -gt 0 ]; then
  rvm use $1
  if [ $? -ne 0 ]; then
    echo "ERROR: rvm doesn't like \"$1\". Quitting."
    exit 1
  fi
fi

LIBS=-lgmp ./runbench -n
if [ $? -ne 0 ]; then
  echo "ERROR: ruby extconf.rb didn't work so hot. Quitting."
  exit 2
fi

echo "RUBY:   `ruby -v`"
echo "GMP:    `ruby -r '../lib/gmp' -e \"puts GMP::GMP_VERSION\"`"
echo "GMP_CC: `ruby -r '../lib/gmp' -e \"puts GMP::GMP_CC\"`"

