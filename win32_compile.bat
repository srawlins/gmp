@ECHO OFF
set GMP_DIR=gmp-5.0.1
set MPFR_DIR=mpfr-3.0.0

:CheckGMP
IF "%~1"=="" GOTO CheckMPFR
set GMP_DIR=gmp-%1
SHIFT

:CheckMPFR
IF "%~1"=="" GOTO RunIt
set MPFR_DIR=mpfr-%1
SHIFT

:RunIt
cd ext && ^
make clean & ^
ruby extconf.rb --with-gmp-dir=\Ruby191\devkit\msys\1.0.11\usr\local\%GMP_DIR% --with-mpfr-dir=\Ruby191\devkit\msys\1.0.11\usr\local\%MPFR_DIR% && ^
make && ^
cd ..
