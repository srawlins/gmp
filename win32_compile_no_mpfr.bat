cd ext && ^
make clean & ^
ruby extconf.rb --with-gmp-dir=\Ruby19\devkit\msys\1.0.11\usr\local\gmp-5.0.1 && ^
make && ^
cd ..