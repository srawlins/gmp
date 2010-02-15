#include <ruby_gmp.h>

/*
 * Taken directly from GMPbench.
 */

#define TIME(t,func)                                       \
  do {                                                     \
    long int __t0, __times, __t, __tmp;                    \
    __times = 1;                                           \
    {func;}                                                \
    do {                                                   \
	  __times <<= 1;                                       \
	  __t0 = FIX2INT(r_gmpmod_cputime ());                 \
	  for (__t = 0; __t < __times; __t++)                  \
	    {func;}                                            \
	  __tmp = FIX2INT(r_gmpmod_cputime ()) - __t0;         \
    } while (__tmp < 250);                                 \
    (t) = (double) __tmp / __times;                        \
  } while (0)

/* Return user CPU time measured in milliseconds.  */
#if !defined (__sun) \
    && (defined (USG) || defined (__SVR4) || defined (_UNICOS) \
	|| defined (__hpux) || defined (_WIN32))
#include <time.h>

int
cputime ()
{
  return (int) ((double) clock () * 1000 / CLOCKS_PER_SEC);
}
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
#endif

VALUE
r_gmpmod_cputime (VALUE self)
{
  (void)self;
  return INT2FIX (cputime ());
}

VALUE
r_gmpmod_time (VALUE self)
{
  (void)self;
  long int __t0, __times, __t, __tmp;
  __times = 1;
  
  rb_need_block();
  
  rb_yield (Qnil);
  do {
    __times <<= 1;
    __t0 = cputime ();
    for (__t = 0; __t < __times; __t++)
      {rb_yield (Qnil);}
    __tmp = cputime () - __t0;
  } while (__tmp < 250);
  return rb_float_new ((double) __tmp / __times);
}

void init_gmpbench_timing()
{
  mGMP = rb_define_module("GMP");
  rb_define_module_function(mGMP, "cputime", r_gmpmod_cputime, 0);
  rb_define_module_function(mGMP, "time", r_gmpmod_time, 0);
}
