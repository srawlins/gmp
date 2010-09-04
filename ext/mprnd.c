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
  char *prefix;
  char name_val[10];
  if (MPFR_VERSION_MAJOR < 3)
    prefix = "GMP";
  else
    prefix = "MPFR";
  switch (FIX2INT(mode)) {
  case 0:
    sprintf(name_val, "%s_RNDN", prefix);
    ieee754 = rb_str_new2("roundTiesToEven");
    break;
  case 1:
    sprintf(name_val, "%s_RNDZ", prefix);
    ieee754 = rb_str_new2("roundTowardZero");
    break;
  case 2:
    sprintf(name_val, "%s_RNDU", prefix);
    ieee754 = rb_str_new2("roundTowardPositive");
    break;
  case 3:
    sprintf(name_val, "%s_RNDD", prefix);
    ieee754 = rb_str_new2("roundTowardNegative");
    break;
  case 4:
    sprintf(name_val, "%s_RNDA", prefix);
    ieee754 = rb_str_new2("roundAwayFromZero");
    break;
  default:
    sprintf(name_val, "%s_RNDN", prefix);
    ieee754 = rb_str_new2("roundTiesToEven");
  }
  name = rb_str_new2(name_val);
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
  /* MPFR 3.0.0 */
  rb_define_const(mGMP, "MPFR_RNDN", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(0)));
  rb_define_const(mGMP, "MPFR_RNDZ", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(1)));
  rb_define_const(mGMP, "MPFR_RNDU", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(2)));
  rb_define_const(mGMP, "MPFR_RNDD", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(3)));
  rb_define_const(mGMP, "MPFR_RNDA", rb_funcall (cGMP_Rnd, new_id, 1, INT2FIX(4)));
  /* end MPFR 3.0.0 */
}

#endif /* MPFR */
