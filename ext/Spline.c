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

/* $Id: Spline.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_interp.h"
#include "gsl/gsl_spline.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Internal use only */

typedef double (* fun_t)(const gsl_spline *, double, gsl_interp_accel *);
typedef int (* fun_e_t)(const gsl_spline *, double, gsl_interp_accel *, double *);

static VALUE call(fun_t fun, VALUE class, VALUE x, VALUE a) {
  gsl_spline * spline;
  gsl_interp_accel * acc;
  Data_Get_Struct(a, gsl_interp_accel, acc);
  Data_Get_Struct(class, gsl_spline, spline);
  return rb_float_new((*fun)(spline, NUM2DBL(x), acc));
}

static VALUE call_e(fun_e_t fun, VALUE class, VALUE x, VALUE a, VALUE d) {
  int ret;
  gsl_spline * spline;
  gsl_interp_accel * acc;
  gsl_sf_result my_d;
  Data_Get_Struct(a, gsl_interp_accel, acc);
  Data_Get_Struct(class, gsl_spline, spline);
  ret = (*fun)(spline, NUM2DBL(x), acc, &my_d.val);
  COPYRESULT(d, my_d);
  return INT2FIX(ret);
}

/* Interpolation: Higher-level interface */

/*
static VALUE Spline_initialize(VALUE self, VALUE func, VALUE size) {
  return self;
}
*/

static void Spline_free(void * p) {
  gsl_spline_free(p);
}

VALUE Spline_new(VALUE class, VALUE func, VALUE size) {
  VALUE spline;
  gsl_spline * ptr;
  switch (NUM2INT(func)) {
    case INTERP_LINEAR: 
      ptr = gsl_spline_alloc(gsl_interp_linear, NUM2INT(size));
      break;
	case INTERP_POLYNOMIAL:
	  ptr = gsl_spline_alloc(gsl_interp_polynomial, NUM2INT(size));
	  break;
    case INTERP_CSPLINE: 
      ptr = gsl_spline_alloc(gsl_interp_cspline, NUM2INT(size));
      break;
    case INTERP_CSPLINE_PERIODIC: 
      ptr = gsl_spline_alloc(gsl_interp_cspline_periodic, NUM2INT(size));
      break;
    case INTERP_AKIMA: 
      ptr = gsl_spline_alloc(gsl_interp_akima, NUM2INT(size));
      break;
    case INTERP_AKIMA_PERIODIC: 
      ptr = gsl_spline_alloc(gsl_interp_akima_periodic, NUM2INT(size));
      break;
    default:
      rb_raise(rb_eArgError, "Illegal interpolation type!");
  }
  spline = Data_Wrap_Struct(class, 0, Spline_free, ptr);
  return spline;
}

static VALUE Spline_init(VALUE self, VALUE xa, VALUE ya) {
  long my_size = RARRAY(xa)->len;
  double * my_xa,  * my_ya;
  gsl_spline * ptr;
  if (RARRAY(xa)->len != RARRAY(ya)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  Data_Get_Struct(self, gsl_spline, ptr);
  COPYRUBYARRAY(xa, my_xa);
  COPYRUBYARRAY(ya, my_ya);
  return INT2FIX(gsl_spline_init(ptr, my_xa, my_ya, my_size));
}

static VALUE Spline_name(VALUE self) {
  gsl_spline * ptr;
  Data_Get_Struct(self, gsl_spline, ptr);
  return rb_str_new2(gsl_interp_name(ptr->interp));
}

static VALUE Spline_min_size(VALUE self) {
  gsl_spline * ptr;
  Data_Get_Struct(self, gsl_spline, ptr);
  return INT2FIX(gsl_interp_min_size(ptr->interp));
}

/* Evaluation of interpolating functions */

static VALUE Spline_eval(VALUE self, VALUE x, VALUE a) {
  return call(gsl_spline_eval, self, x, a);
}

static VALUE Spline_eval_deriv(VALUE self, VALUE x, VALUE a) {
  return call(gsl_spline_eval_deriv, self, x, a);
}

static VALUE Spline_eval_deriv2(VALUE self, VALUE x, VALUE a) {
  return call(gsl_spline_eval_deriv2, self, x, a);
}

static VALUE Spline_eval_e(VALUE self, VALUE x, VALUE a, 
  VALUE y) {
  return call_e(gsl_spline_eval_e, self, x, a, y);
}

static VALUE Spline_eval_deriv_e(VALUE self, VALUE x, VALUE a, VALUE d) {
  return call_e(gsl_spline_eval_deriv_e, self, x, a, d);
}

static VALUE Spline_eval_deriv2_e(VALUE self, VALUE x, VALUE a, VALUE d2) {
  return call_e(gsl_spline_eval_deriv2_e, self, x, a, d2);
}

static VALUE Spline_eval_integ(VALUE self, VALUE a, VALUE b, VALUE acc) {
  gsl_spline * spline;
  gsl_interp_accel * ptr;
  Data_Get_Struct(acc, gsl_interp_accel, ptr);
  Data_Get_Struct(self, gsl_spline, spline);
  return rb_float_new(gsl_spline_eval_integ(spline, NUM2DBL(a), NUM2DBL(b), ptr));
}

static VALUE Spline_eval_integ_e(VALUE self, VALUE a, VALUE b, VALUE acc, 
  VALUE result) {
  int ret;
  gsl_spline * spline;
  gsl_interp_accel * ptr;
  gsl_sf_result my_result;
  Data_Get_Struct(acc, gsl_interp_accel, ptr);
  Data_Get_Struct(self, gsl_spline, spline);
  ret = gsl_spline_eval_integ_e(spline, NUM2DBL(a), NUM2DBL(b), ptr, 
    &my_result.val);
  COPYRESULT(result, my_result);
  return INT2FIX(ret);
}

/* Class definition */

VALUE rbgsl_cSpline;

void Init_Spline() {
  rbgsl_cSpline = rb_define_class_under(rbgsl_mInterp, "Spline", rb_cObject);

  rb_define_singleton_method(rbgsl_cSpline, "new", Spline_new, 2);

  rb_define_method(rbgsl_cSpline, "init", Spline_init, 2);
  rb_define_method(rbgsl_cSpline, "name", Spline_name, 0);
  rb_define_alias(rbgsl_cSpline, "to_s", "name");
  rb_define_method(rbgsl_cSpline, "min_size", Spline_min_size, 0);
 
  rb_define_method(rbgsl_cSpline, "eval", Spline_eval, 2);
  rb_define_method(rbgsl_cSpline, "eval_e", Spline_eval_e, 3);
  rb_define_method(rbgsl_cSpline, "eval_deriv", Spline_eval_deriv, 2);
  rb_define_method(rbgsl_cSpline, "eval_deriv_e", Spline_eval_deriv_e, 3);
  rb_define_method(rbgsl_cSpline, "eval_deriv2", Spline_eval_deriv2, 2);
  rb_define_method(rbgsl_cSpline, "eval_deriv2_e", Spline_eval_deriv2_e, 3);
  rb_define_method(rbgsl_cSpline, "eval_integ", Spline_eval_integ, 3);
  rb_define_method(rbgsl_cSpline, "eval_integ_e", Spline_eval_integ_e, 4);

}

/* vim: set ts=4: */
