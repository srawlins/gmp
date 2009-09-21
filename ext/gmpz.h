#ifndef _GMPZ_H_
#define _GMPZ_H_

/*
 * gmpz.h
 *
 * This file contains GMP::Z method definitions.
 */

#include <ruby_gmp.h>

#define DEFUN_INT_COND_P(fname,mpz_fname) \
static VALUE r_gmpz_##fname(VALUE self) \
{ \
  MP_INT *self_val; \
  mpz_get_struct(self, self_val); \
  return mpz_fname(self_val)?Qtrue:Qfalse; \
}

#endif
