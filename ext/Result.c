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

/* $Id: Result.c,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_elementary.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Result */

static VALUE Result_init(VALUE self, VALUE v, VALUE e) {
  gsl_sf_result * ptr;
  Data_Get_Struct(self, gsl_sf_result, ptr);
  ptr->val = NUM2DBL(v);
  ptr->err = NUM2DBL(e);
  return self;
}

VALUE Result_new(VALUE class) {
  VALUE res;
  gsl_sf_result * ptr;
  res = Data_Make_Struct(class, gsl_sf_result, 0, free, ptr);
  ptr->val = 0.0;
  ptr->err = 0.0;
  return res;
}

VALUE Result_new2(VALUE class, VALUE v, VALUE e) {
  VALUE res;
  VALUE argv[2];
  gsl_sf_result * ptr;
  res = Data_Make_Struct(class, gsl_sf_result, 0, free, ptr);
  argv[0] = v;
  argv[1] = e;
  rb_obj_call_init(res, 2, argv);
  return res;
}

VALUE Result_new_intern(VALUE class, gsl_sf_result * r) {
  gsl_sf_result * ptr;
  VALUE res = Data_Make_Struct(class, gsl_sf_result, 0, free, ptr);
  ptr->val = r->val;
  ptr->err = r->err;
  return res;
}

static VALUE Result_val(VALUE self) {
  gsl_sf_result * ptr;
  Data_Get_Struct(self, gsl_sf_result, ptr);
  return rb_float_new(ptr->val);
}

static VALUE Result_err(VALUE self) {
  gsl_sf_result * ptr;
  Data_Get_Struct(self, gsl_sf_result, ptr);
  return rb_float_new(ptr->err);
}

static VALUE Result_mult(VALUE self, VALUE other) {
  int ret;
  gsl_sf_result r;
  gsl_sf_result * ptr, * optr;
  Data_Get_Struct(self, gsl_sf_result, ptr);
  Data_Get_Struct(other, gsl_sf_result, optr);
  ret = gsl_sf_multiply_err_e(ptr->val, ptr->err, optr->val, optr->err, &r);
  return RESULT(&r);
}

/* Result10 */

static VALUE Result10_init(VALUE self) {
  gsl_sf_result_e10 * ptr;
  Data_Get_Struct(self, gsl_sf_result_e10, ptr);
  ptr->val = 0.0;
  ptr->err = 0.0;
  ptr->e10 = 1.0;
  return self;
}

VALUE Result10_new(VALUE class) {
  VALUE res;
  gsl_sf_result_e10 *result;
  res = Data_Make_Struct(class, gsl_sf_result_e10, 0, free, result);
  rb_obj_call_init(res, 0, 0);
  return res;
}

VALUE Result10_new_intern(VALUE class, gsl_sf_result_e10 * r) {
  gsl_sf_result_e10 * ptr;
  VALUE res = Data_Make_Struct(class, gsl_sf_result_e10, 0, free, ptr);
  ptr->val = r->val;
  ptr->err = r->err;
  ptr->e10 = r->e10;
  return res;
}

static VALUE Result10_val(VALUE self) {
  gsl_sf_result_e10 *p;
  Data_Get_Struct(self, gsl_sf_result_e10, p);
  return rb_float_new(p->val);
}

static VALUE Result10_err(VALUE self) {
  gsl_sf_result_e10 *p;
  Data_Get_Struct(self, gsl_sf_result_e10, p);
  return rb_float_new(p->err);
}

static VALUE Result10_e10(VALUE self) {
  gsl_sf_result_e10 *p;
  Data_Get_Struct(self, gsl_sf_result_e10, p);
  return INT2NUM(p->e10);
}

/* Class definition */

VALUE rbgsl_cResult;
VALUE rbgsl_cResult10;

void Init_Result() {
  rbgsl_cResult = rb_define_class_under(rbgsl_mGSL, "Result", rb_cObject);
  
  rb_define_singleton_method(rbgsl_cResult, "new", Result_new, 0);
  rb_define_singleton_method(rbgsl_cResult, "new2", Result_new2, 2);
  
  rb_define_method(rbgsl_cResult, "initialize", Result_init, 2);
  rb_define_method(rbgsl_cResult, "val", Result_val, 0);
  rb_define_method(rbgsl_cResult, "err", Result_err, 0);
  rb_define_method(rbgsl_cResult, "*", Result_mult, 1);

  rbgsl_cResult10 = rb_define_class_under(rbgsl_mGSL, "Result10", rb_cObject);
  
  rb_define_singleton_method(rbgsl_cResult10, "new", Result10_new, 0);
  
  rb_define_method(rbgsl_cResult10, "initialize", Result10_init, 0);
  rb_define_method(rbgsl_cResult10, "val", Result10_val, 0);
  rb_define_method(rbgsl_cResult10, "err", Result10_err, 0);
  rb_define_method(rbgsl_cResult10, "e10", Result10_e10, 0);
}

/* vim: set ts=4: */
