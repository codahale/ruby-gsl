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

/* $Id: Chebyshev.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_chebyshev.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Chebyshev Approximations */

static VALUE Cheb_initialize(VALUE self, VALUE order, VALUE f, VALUE a, VALUE b) {
  gsl_cheb_series * ptr;
  gsl_function * func;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  Data_Get_Struct(f, gsl_function, func);
  return INT2FIX(gsl_cheb_init(ptr, func, NUM2DBL(a), NUM2DBL(b)));
}

static void Cheb_free(void *p) {
  gsl_cheb_free(p);
}

VALUE Cheb_new(VALUE class, VALUE order, VALUE f, VALUE a, VALUE b) {
  gsl_cheb_series * ptr;
  VALUE cheb;
  VALUE argv[4];
  ptr = gsl_cheb_alloc(NUM2INT(order));
  cheb = Data_Wrap_Struct(class, 0, Cheb_free, ptr);
  argv[0] = order;
  argv[1] = f;
  argv[2] = a;
  argv[3] = b;
  rb_obj_call_init(cheb, 4, argv);
  return cheb;
}

/* Chebyshev Series Evaluation */

static VALUE Cheb_eval(VALUE self, VALUE x) {
  gsl_cheb_series * ptr;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  return rb_float_new(gsl_cheb_eval(ptr, NUM2DBL(x)));
}

static VALUE Cheb_eval_n(VALUE self, VALUE order, VALUE x) {
  gsl_cheb_series * ptr;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  return rb_float_new(gsl_cheb_eval_n(ptr, NUM2INT(order), NUM2DBL(x)));
}

static VALUE Cheb_eval_mode(VALUE self, VALUE x, VALUE mode) {
  gsl_cheb_series * ptr;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  return rb_float_new(gsl_cheb_eval_mode(ptr, NUM2DBL(x), NUM2INT(mode)));
}

static VALUE Cheb_eval_err(VALUE self, VALUE x) {
  gsl_cheb_series * ptr;
  gsl_sf_result result;
  int ret;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  ret = gsl_cheb_eval_err(ptr, NUM2DBL(x), &result.val, &result.err);
  return RESULT(&result);
}

static VALUE Cheb_eval_n_err(VALUE self, VALUE order, VALUE x) {
  gsl_cheb_series * ptr;
  gsl_sf_result result;
  int ret;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  ret = gsl_cheb_eval_n_err(ptr, NUM2INT(order), NUM2DBL(x), &result.val, &result.err);
  return RESULT(&result);
}

static VALUE Cheb_eval_mode_e(VALUE self, VALUE x, VALUE mode) {
  gsl_cheb_series * ptr;
  gsl_sf_result result;
  int ret;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  ret = gsl_cheb_eval_mode_e(ptr, NUM2DBL(x), NUM2INT(mode), &result.val, &result.err);
  return RESULT(&result);
}

/* Derivatives and Integrals */

static VALUE Cheb_calc_deriv(VALUE self, VALUE deriv) {
  gsl_cheb_series * ptr, * other;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  Data_Get_Struct(deriv, gsl_cheb_series, other);
  return INT2FIX(gsl_cheb_calc_deriv(other, ptr));
}

static VALUE Cheb_calc_integ(VALUE self, VALUE integ) {
  gsl_cheb_series * ptr, * other;
  Data_Get_Struct(self, gsl_cheb_series, ptr);
  Data_Get_Struct(integ, gsl_cheb_series, other);
  return INT2FIX(gsl_cheb_calc_integ(other, ptr));
}

/* Class definition */

VALUE rbgsl_cCheb;

void Init_Cheb() {
  rbgsl_cCheb = rb_define_class_under(rbgsl_mGSL, "Chebyshev", rb_cObject);

  rb_define_singleton_method(rbgsl_cCheb, "new", Cheb_new, 4);
  rb_define_method(rbgsl_cCheb, "initialize", Cheb_initialize, 4);

  rb_define_method(rbgsl_cCheb, "eval", Cheb_eval, 1);
  rb_define_method(rbgsl_cCheb, "eval_err", Cheb_eval_err, 1);
  rb_define_method(rbgsl_cCheb, "eval_n", Cheb_eval_n, 2);
  rb_define_method(rbgsl_cCheb, "eval_n_err", Cheb_eval_n_err, 2);
  rb_define_method(rbgsl_cCheb, "eval_mode", Cheb_eval_mode, 2);
  rb_define_method(rbgsl_cCheb, "eval_mode_e", Cheb_eval_mode_e, 2);

  rb_define_method(rbgsl_cCheb, "calc_deriv", Cheb_calc_deriv, 1);
  rb_define_alias(rbgsl_cCheb, "deriv", "calc_deriv");
  rb_define_method(rbgsl_cCheb, "calc_integ", Cheb_calc_integ, 1);
  rb_define_alias(rbgsl_cCheb, "integ", "calc_integ");
}

/* vim: set ts=4: */
