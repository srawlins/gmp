cc -o conftest \
   -I. \
   -I/Users/sam/.rvm/rubies/jruby-head/lib/native/include/ruby \
   -I.  \
   -D_XOPEN_SOURCE -D_DARWIN_C_SOURCE         \
   -fPIC \
   -DTARGET_RT_MAC_CFM=0  \
   -fno-omit-frame-pointer \
   -fno-strict-aliasing  \
   -fexceptions     \
   -DMPFR comftest_check_sizeof.c  \
   -L"." \
   -L"/Users/sam/.rvm/rubies/jruby-head/lib"  \
   -bundle \
   -framework JavaVM \
   -Wl,-syslibroot,  \
   -mmacosx-version-min=10.4    \
   -lmpfr \
   -lgmp

echo -arch x86_64  \
