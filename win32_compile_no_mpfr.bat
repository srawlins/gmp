@ECHO OFF
set GMP_DIR=gmp-5.0.1

:CheckGMP
IF "%~1"=="" GOTO RunIt
set GMP_DIR=gmp-%1
SHIFT

:RunIt
cd ext && ^
make clean & ^
ruby extconf.rb --with-gmp-dir=\Ruby191\devkit\msys\1.0.11\usr\local\%GMP_DIR% && ^
make && ^
cd ..