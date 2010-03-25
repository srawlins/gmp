#include <mprnd.h>

#ifdef MPFR

VALUE r_mprnd_initialize(int argc, VALUE *argv, VALUE self);

VALUE r_mprndsg_new(int argc, VALUE *argv, VALUE klass)
{
  VALUE res;
  int *res_value;
  (void)klass;
  res_value = 0;
  
  mprnd_make_struct(res, res_value);
  rb_obj_call_init(res, argc, argv);
  return res;
}

VALUE r_mprnd_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE mode, name, ieee754;
  mode = argv[0];
  (void)argc;
  switch (FIX2INT(mode)) {
  case 0:
    name = rb_str_new2("GMP_RNDN");
    ieee754 = rb_str_new2("roundTiesToEven");
    break;
  case 1:
    name = rb_str_new2("GMP_RNDZ");
    ieee754 = rb_str_new2("roundTowardZero");
    break;
  case 2:
    name = rb_str_new2("GMP_RNDU");
    ieee754 = rb_str_new2("roundTowardPositive");
    break;
  case 3:
    name = rb_str_new2("GMP_RNDD");
    ieee754 = rb_str_new2("roundTowardNegative");
    break;
  default:
    name = rb_str_new2("GMP_RNDN");
    ieee754 = rb_str_new2("roundTiesToEven");
  }
  rb_iv_set(self, "@mode",    mode);
  rb_iv_set(self, "@name", name);
  rb_iv_set(self, "@ieee754", ieee754);
  return Qnil;
}

VALUE r_mprnd_inspect(VALUE self)
{
  return rb_iv_get(self, "@name");
}

void init_gmprnd()
{
  mGMP = rb_define_module("GMP");
  ID new_id = rb_intern("new");

  cGMP_Rnd = rb_define_class_under(mGMP, "Rnd", rb_cObject);
  
  rb_define_singleton_method(cGMP_Rnd, "new", r_mprndsg_new, -1);
  rb_define_method(cGMP_Rnd, "initialize", r_mprnd_initialize, -1);
  rb_define_method(cGMP_Rnd, "inspect", r_mprnd_inspect, 0);
  
  rb_define_attr (cGMP_Rnd, "mode",    1, 0);
  rb_define_attr (cGMP_Rnd, "name",    1, 0);
  rb_define_attr (cGMP_Rnd, "ieee754", 1, 0);
  
  rb_define_const(mGMP, "GMP_RNDN", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(0)));
  rb_define_const(mGMP, "GMP_RNDZ", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(1)));
  rb_define_const(mGMP, "GMP_RNDU", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(2)));
  rb_define_const(mGMP, "GMP_RNDD", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(3)));
}

#endif /* MPFR */
