@ECHO OFF
set OLD_PATH=%PATH%
set PATH=C:\Ruby191\devkit\msys\1.0.11\usr\local\%GMP_DIR%\bin;%PATH%;C:\Ruby191\devkit\msys\1.0.11\usr\local\%MPFR_DIR%\bin;%PATH%
cd test
ruby unit_tests.rb
cd ..

set PATH=%OLD_PATH%
set OLD_PATH=
