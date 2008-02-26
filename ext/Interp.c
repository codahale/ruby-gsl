/*
* ruby-gsl - using GSL from Ruby
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Id: Interp.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_interp.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Internal use only */

typedef double (* fun_t)(const gsl_interp *, const double *, const double *, double, gsl_interp_accel *);
typedef int (* fun_e_t)(const gsl_interp *, const double *, const double *, double, gsl_interp_accel *, double *);

static VALUE call(fun_t fun, VALUE class, VALUE xa, VALUE ya, VALUE x, VALUE a) {
  double * my_xa, * my_ya;
  gsl_interp * ptr;
  gsl_interp_accel * acc;

  if (RARRAY(xa)->len != RARRAY(ya)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  Data_Get_Struct(a, gsl_interp_accel, acc);
  Data_Get_Struct(class, gsl_interp, ptr);
  COPYRUBYARRAY(xa, my_xa);
  COPYRUBYARRAY(ya, my_ya);

  return rb_float_new((*fun)(ptr, my_xa, my_ya, NUM2DBL(x), acc));
}

static VALUE call_e(fun_e_t fun, VALUE class, VALUE xa, VALUE ya, VALUE x, 
  VALUE a, VALUE d) {
  int ret;
  double * my_xa, * my_ya;
  gsl_interp * ptr;
  gsl_interp_accel * acc;
  gsl_sf_result my_d;

  if (RARRAY(xa)->len != RARRAY(ya)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  Data_Get_Struct(a, gsl_interp_accel, acc);
  Data_Get_Struct(class, gsl_interp, ptr);
  COPYRUBYARRAY(xa, my_xa);
  COPYRUBYARRAY(ya, my_ya);
  ret = (*fun)(ptr, my_xa, my_ya, NUM2DBL(x), acc, &my_d.val);
  COPYRESULT(d, my_d);

  return INT2FIX(ret);
}

/* Interpolation */

static VALUE Interp_initialize(VALUE self, VALUE func, VALUE size) {
  return self;
}

static void Interp_free(void * p) {
  gsl_interp_free(p);
}

VALUE Interp_new(VALUE class, VALUE func, VALUE size) {
  VALUE argv[2];
  VALUE interp;
  gsl_interp * ptr;

  switch (NUM2INT(func)) {
    case INTERP_LINEAR: 
      ptr = gsl_interp_alloc(gsl_interp_linear, NUM2INT(size));
      break;
    case INTERP_POLYNOMIAL: 
      ptr = gsl_interp_alloc(gsl_interp_polynomial, NUM2INT(size));
      break;
    case INTERP_CSPLINE: 
      ptr = gsl_interp_alloc(gsl_interp_cspline, NUM2INT(size));
      break;
    case INTERP_CSPLINE_PERIODIC: 
      ptr = gsl_interp_alloc(gsl_interp_cspline_periodic, NUM2INT(size));
      break;
    case INTERP_AKIMA: 
      ptr = gsl_interp_alloc(gsl_interp_akima, NUM2INT(size));
      break;
    case INTERP_AKIMA_PERIODIC: 
      ptr = gsl_interp_alloc(gsl_interp_akima_periodic, NUM2INT(size));
      break;
    default:
      rb_raise(rb_eArgError, "Illegal interpolation type!");
  }
  interp = Data_Wrap_Struct(class, 0, Interp_free, ptr);
  argv[0] = func;
  argv[1] = size;
  rb_obj_call_init(interp, 2, argv);

  return interp;
}

static VALUE Interp_name(VALUE self) {
  gsl_interp * ptr;

  Data_Get_Struct(self, gsl_interp, ptr);

  return rb_str_new2(gsl_interp_name(ptr));
}

static VALUE Interp_min_size(VALUE self) {
  gsl_interp * ptr;

  Data_Get_Struct(self, gsl_interp, ptr);

  return INT2FIX(gsl_interp_min_size(ptr));
}

static VALUE Interp_init(VALUE self, VALUE xa, VALUE ya) {
  double * my_xa,  * my_ya;
  gsl_interp * ptr;

  if (RARRAY(xa)->len != RARRAY(ya)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  Data_Get_Struct(self, gsl_interp, ptr);
  COPYRUBYARRAY(xa, my_xa);
  COPYRUBYARRAY(ya, my_ya);

  return INT2FIX(gsl_interp_init(ptr, my_xa, my_ya, RARRAY(xa)->len));
}

/* Evaluation of interpolating functions */

static VALUE Interp_eval(VALUE self, VALUE xa, VALUE ya, VALUE x, VALUE a) {
  return call(gsl_interp_eval, self, xa, ya, x, a);
}

static VALUE Interp_eval_deriv(VALUE self, VALUE xa, VALUE ya, VALUE x, VALUE a) {
  return call(gsl_interp_eval_deriv, self, xa, ya, x, a);
}

static VALUE Interp_eval_deriv2(VALUE self, VALUE xa, VALUE ya, VALUE x, VALUE a) {
  return call(gsl_interp_eval_deriv2, self, xa, ya, x, a);
}

static VALUE Interp_eval_e(VALUE self, VALUE xa, VALUE ya, VALUE x, VALUE a, 
  VALUE y) {
  return call_e(gsl_interp_eval_e, self, xa, ya, x, a, y);
}

static VALUE Interp_eval_deriv_e(VALUE self, VALUE xa, VALUE ya, VALUE x, 
  VALUE a, VALUE d) {
  return call_e(gsl_interp_eval_deriv_e, self, xa, ya, x, a, d);
}

static VALUE Interp_eval_deriv2_e(VALUE self, VALUE xa, VALUE ya, VALUE x, 
  VALUE a, VALUE d2) {
  return call_e(gsl_interp_eval_deriv2_e, self, xa, ya, x, a, d2);
}

static VALUE Interp_eval_integ(VALUE self, VALUE xa, VALUE ya, VALUE a, 
  VALUE b, VALUE acc) {
  double * my_xa, * my_ya;
  gsl_interp * ptr;
  gsl_interp_accel * ptr2;
  if (RARRAY(xa)->len != RARRAY(ya)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  Data_Get_Struct(acc, gsl_interp_accel, ptr2);
  Data_Get_Struct(self, gsl_interp, ptr);
  COPYRUBYARRAY(xa, my_xa)
  COPYRUBYARRAY(ya, my_ya);
  return rb_float_new(gsl_interp_eval_integ(ptr, my_xa, my_ya, NUM2DBL(a),
    NUM2DBL(b), ptr2));
}

static VALUE Interp_eval_integ_e(VALUE self, VALUE xa, VALUE ya, VALUE a, 
  VALUE b, VALUE acc, VALUE result) {
  int ret;
  double * my_xa, * my_ya;
  gsl_interp * ptr;
  gsl_interp_accel * ptr2;
  gsl_sf_result my_result;
  if (RARRAY(xa)->len != RARRAY(ya)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  Data_Get_Struct(acc, gsl_interp_accel, ptr2);
  Data_Get_Struct(self, gsl_interp, ptr);
  COPYRUBYARRAY(xa, my_xa)
  COPYRUBYARRAY(ya, my_ya);
  ret = gsl_interp_eval_integ_e(ptr, my_xa, my_ya, NUM2DBL(a), NUM2DBL(b), 
    ptr2, &my_result.val);
  COPYRESULT(result, my_result);
  return INT2FIX(ret);
}

/* Class definition */

VALUE rbgsl_cInterp;
VALUE rbgsl_mInterp;

void Init_Interp() {
rbgsl_mInterp = rb_define_module_under(rbgsl_mGSL, "Interpolation");
  rbgsl_cInterp = rb_define_class_under(rbgsl_mInterp, "Interp", rb_cObject);

  rb_define_singleton_method(rbgsl_cInterp, "new", Interp_new, 2);
  rb_define_method(rbgsl_cInterp, "initialize", Interp_initialize, 2);

  rb_define_method(rbgsl_cInterp, "init", Interp_init, 2);
  rb_define_method(rbgsl_cInterp, "name", Interp_name, 0);
  rb_define_alias(rbgsl_cInterp, "to_s", "name");
  rb_define_method(rbgsl_cInterp, "min_size", Interp_min_size, 0);
 
  rb_define_method(rbgsl_cInterp, "eval", Interp_eval, 4);
  rb_define_method(rbgsl_cInterp, "eval_e", Interp_eval_e, 5);
  rb_define_method(rbgsl_cInterp, "eval_deriv", Interp_eval_deriv, 4);
  rb_define_method(rbgsl_cInterp, "eval_deriv_e", Interp_eval_deriv_e, 5);
  rb_define_method(rbgsl_cInterp, "eval_deriv2", Interp_eval_deriv2, 4);
  rb_define_method(rbgsl_cInterp, "eval_deriv2_e", Interp_eval_deriv2_e, 5);
  rb_define_method(rbgsl_cInterp, "eval_integ", Interp_eval_integ, 5);
  rb_define_method(rbgsl_cInterp, "eval_integ_e", Interp_eval_integ_e, 6);

  rb_define_const(rbgsl_mInterp, "LINEAR", INT2FIX(INTERP_LINEAR));
  rb_define_const(rbgsl_mInterp, "POLYNOMIAL", INT2FIX(INTERP_POLYNOMIAL));
  rb_define_const(rbgsl_mInterp, "CSPLINE", INT2FIX(INTERP_CSPLINE));
  rb_define_const(rbgsl_mInterp, "CSPLINE_PERIODIC", INT2FIX(INTERP_CSPLINE_PERIODIC));
  rb_define_const(rbgsl_mInterp, "AKIMA", INT2FIX(INTERP_AKIMA));
  rb_define_const(rbgsl_mInterp, "AKIMA_PERIODIC", INT2FIX(INTERP_AKIMA_PERIODIC));

  Init_Accel();
  Init_Spline();
}

/* vim: set ts=4: */
