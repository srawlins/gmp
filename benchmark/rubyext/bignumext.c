#include <ruby.h>

ID mod_id;
ID minus_id;
ID greater_than_id;
ID lshift_id;
ID rshift_id;
ID lshift_bang_id;
ID rshift_bang_id;
ID and_id;
ID pipe_id;

/*
 * Using the Euclidean Algorithm (division-based):
 * http://en.wikipedia.org/wiki/Euclidean_algorithm#Implementations
 *
 * In ruby:
 *     a = self
 *     while b != 0
 *       a, b = b, a % b
 *     end
 *     return a
 */
VALUE
rb_big_gcd_euclidean_division(VALUE a, VALUE b)
{
  VALUE temp;

  /* borrowed from bignum.c, rb_big_modulo() */
  switch (TYPE(b)) {
    case T_FIXNUM:
      b = rb_int2big(FIX2LONG(b));
      break;

    case T_BIGNUM:
      break;

    default:
      return rb_num_coerce_bin(a, b, '%');
  }

  while ((TYPE(b) == T_BIGNUM && !rb_bigzero_p(b)) ||
         (TYPE(b) == T_FIXNUM && FIX2LONG(b) != 0)) {
      if (TYPE(b) == T_BIGNUM) {
        temp = rb_big_clone(b);
      }
      else {
        temp = b;
      }
      //b = rb_big_modulo(a, b);
      b = rb_funcall(a, mod_id, 1, b);
      a = temp;
  }

  return a;
}

VALUE
rb_big_gcd_euclidean_subtraction(VALUE a, VALUE b)
{
  if ((TYPE(a) == T_BIGNUM && rb_bigzero_p(a)) ||
      (TYPE(a) == T_FIXNUM && FIX2LONG(a) == 0))
    return b;

  while ((TYPE(b) == T_BIGNUM && !rb_bigzero_p(b)) ||
         (TYPE(b) == T_FIXNUM && FIX2LONG(b) != 0)) {
    if (rb_funcall(a, greater_than_id, 1, b) == Qtrue) {
      a = rb_funcall(a, minus_id, 1, b);
    }
    else {
      b = rb_funcall(b, minus_id, 1, a);
    }
  }

  return a;
}

VALUE
rb_big_gcd_binary(VALUE a, VALUE b)
{
  int shift;
  VALUE temp;

  /* GCD(0,b) == b; GCD(a,0) == a, GCD(0,0) == 0 */
  if ((TYPE(a) == T_BIGNUM && rb_bigzero_p(a)) ||
      (TYPE(a) == T_FIXNUM && FIX2LONG(a) == 0))
    return b;
  if ((TYPE(b) == T_BIGNUM && rb_bigzero_p(b)) ||
      (TYPE(b) == T_FIXNUM && FIX2LONG(b) == 0))
    return a;

  /* Let shift := lg K, where K is the greatest power of 2
   *         dividing both a and b. */
  for (shift = 0; rb_funcall(rb_funcall(a, pipe_id, 1, b), and_id, 1, INT2FIX(1)) == INT2FIX(0); ++shift) {
    a = rb_funcall(a, rshift_id, 1, INT2FIX(1));
    b = rb_funcall(b, rshift_id, 1, INT2FIX(1));
  }

  while (rb_funcall(a, and_id, 1, INT2FIX(1)) == INT2FIX(0)) {
    a = rb_funcall(a, rshift_id, 1, INT2FIX(1));
  }

  /* From here on, a is always odd. */
  do {
    /* remove all factors of 2 in b -- they are not common
     * note: b is not zero, so while will terminate */
    while (rb_funcall(b, and_id, 1, INT2FIX(1)) == INT2FIX(0)) {
      b = rb_funcall(b, rshift_id, 1, INT2FIX(1));
    }

    /* Now a and b are both odd. Swap if necessary so a <= b,
     * then set b = b - a (which is even). For bignums, the
     * swapping is just pointer movement, and the subtraction
     * can be done in-place. */
    if (rb_funcall(a, greater_than_id, 1, b)) {
      temp = a;
      a = b;
      b = temp;
    }
    b = rb_funcall(b, minus_id, 1, a);

  } while ((TYPE(b) == T_BIGNUM && !rb_bigzero_p(b)) ||
           (TYPE(b) == T_FIXNUM && FIX2LONG(b) != 0));

  return rb_funcall(a, lshift_id, 1, INT2FIX(shift));
}


void
Init_bignumext(void)
{
    rb_cBignum = rb_define_class("Bignum", rb_cInteger);
    mod_id          = rb_intern("%");
    minus_id        = rb_intern("-");
    greater_than_id = rb_intern(">");
    lshift_id       = rb_intern("<<");
    rshift_id       = rb_intern(">>");
    lshift_bang_id  = rb_intern("<<=");
    rshift_bang_id  = rb_intern(">>=");
    pipe_id         = rb_intern("|");
    and_id          = rb_intern("&");

    rb_define_method(rb_cBignum, "gcd", rb_big_gcd_euclidean_division, 1);
    rb_define_method(rb_cBignum, "gcdes", rb_big_gcd_euclidean_subtraction, 1);
    rb_define_method(rb_cBignum, "gcdbin", rb_big_gcd_binary, 1);
}
