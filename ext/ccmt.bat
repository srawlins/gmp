@ECHO OFF
set GMP_DIR=--with-gmp-dir=\Ruby191\devkit\msys\1.0.11\usr\local
set MPFR_DIR=--with-mpfr-dir=\Ruby191\devkit\msys\1.0.11\usr\local\mpfr-3.0.0
set EXTCONF_OPTS=
set OLD_PATH=%PATH%

IF NOT "%~1"=="no-mpfr" GOTO CheckGMP
set EXTCONF_OPTS=%1
SHIFT

:CheckGMP
echo EXTCONF_OPTS=%EXTCPNF_OPTS%
IF "%~1"=="" GOTO CheckMPFR
set GMP_DIR=--with-gmp-dir=\Ruby191\devkit\msys\1.0.11\usr\local\gmp-%1
set PATH=C:\Ruby191\devkit\msys\1.0.11\usr\local\gmp-%1\bin;%PATH%
SHIFT

:CheckMPFR
IF "%~1"=="" GOTO MakeClean
set MPFR_DIR=--with-mpfr-dir=\Ruby191\devkit\msys\1.0.11\usr\local\mpfr-%1
set PATH=C:\Ruby191\devkit\msys\1.0.11\usr\local\mpfr-%1\bin;%PATH%
SHIFT

:MakeClean
cmd /C make clean
echo about to: ruby extconf.rb %EXTCONF_OPTS% %GMP_DIR% %MPFR_DIR%
ruby extconf.rb %EXTCONF_OPTS% %GMP_DIR% %MPFR_DIR%
IF %errorlevel% EQU 0 GOTO Make
echo ERROR: ruby extconf.rb didn't work so hot. Quitting.
GOTO TheEnd

:Make
cmd /C make
IF %errorlevel% EQU 0 GOTO Test
echo ERROR: make didn't work so hot. Quitting.
Goto TheEnd

:Test
cd ..\test
ruby unit_tests.rb
cd ..\ext

REM Windows batch scripting is so pathetic. Bill Gates wrote BASIC for the Altair. Who did he hire to write CMD.EXE?!?
FOR /F "tokens=1* delims=" %%x IN ('cmd /q/c ruby -v')                                     DO echo RUBY:   %%x
FOR /F "tokens=1* delims=" %%x IN ('cmd /q/c ruby -r "./gmp" -e "puts GMP::GMP_VERSION"')  DO echo GMP:    %%x
FOR /F "tokens=1* delims=" %%x IN ('cmd /q/c ruby -r "./gmp" -e "puts GMP::GMP_CC"')       DO echo GMP_CC: %%x
IF "%EXTCONF_ARGS%" NEQ "" GOTO EchoNoMPFR
FOR /F "tokens=1* delims=" %%x IN ('cmd /q/c ruby -r "./gmp" -e "puts GMP::MPFR_VERSION"') DO echo MPFR:   %%x
GOTO :TheEnd

:EchoNoMPFR
echo MPFR:   --no-mpfr

:TheEnd
set PATH=%OLD_PATH%
set OLD_PATH=
