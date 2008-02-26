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

/* $Id: Sum.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sum.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Series Acceleration */

/* Acceleration functions */

static VALUE Levin_init(VALUE self, VALUE n) {
  return self;
}

static void Levin_free(void *p) {
  gsl_sum_levin_u_free(p);
}

VALUE Levin_new(VALUE class, VALUE n) {
  VALUE argv[1];
  VALUE sum;
  gsl_sum_levin_u_workspace * ptr;
  ptr = gsl_sum_levin_u_alloc(NUM2INT(n));
  sum = Data_Wrap_Struct(class, 0, Levin_free, ptr);
  argv[0] = n;
  rb_obj_call_init(sum, 1, argv);
  return sum;
}

static VALUE Levin_size(VALUE self) {
  gsl_sum_levin_u_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_u_workspace, ptr);
  return INT2FIX(ptr->size);
}

static VALUE Levin_terms_used(VALUE self) {
  gsl_sum_levin_u_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_u_workspace, ptr);
  return INT2FIX(ptr->terms_used);
}

static VALUE Levin_sum_plain(VALUE self) {
  gsl_sum_levin_u_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_u_workspace, ptr);
  return rb_float_new(ptr->sum_plain);
}

static VALUE Levin_accel(VALUE self, VALUE array) {
  double * my_array;
  double sum, abserr;
  int ret;
  gsl_sum_levin_u_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_u_workspace, ptr);
  COPYRUBYARRAY(array, my_array);
  ret = gsl_sum_levin_u_accel(my_array, RARRAY(array)->len, ptr, &sum, &abserr);
  return rb_ary_new3(2, rb_float_new(sum), rb_float_new(abserr));
}

static VALUE Levin_step(VALUE self, VALUE term, VALUE n, VALUE nmax) {
  double sum;
  int ret;
  gsl_sum_levin_u_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_u_workspace, ptr);
  ret = gsl_sum_levin_u_step(NUM2INT(term), NUM2INT(n), NUM2INT(nmax), ptr, 
    &sum);
  return rb_float_new(sum);
}

static VALUE Levin_minmax(VALUE self, VALUE array, VALUE min, VALUE max) {
  double * my_array;
  double sum, abserr;
  int ret;
  gsl_sum_levin_u_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_u_workspace, ptr);
  COPYRUBYARRAY(array, my_array);
  ret = gsl_sum_levin_u_minmax(my_array, RARRAY(array)->len, NUM2INT(min),
    NUM2INT(max), ptr, &sum, &abserr);
  return rb_ary_new3(2, rb_float_new(sum), rb_float_new(abserr));
}

/* Acceleration functions without error estimation */

static VALUE LevinTrunc_init(VALUE self, VALUE n) {
  return self;
}

static void LevinTrunc_free(void *p) {
  gsl_sum_levin_utrunc_free(p);
}

VALUE LevinTrunc_new(VALUE class, VALUE n) {
  VALUE argv[1];
  VALUE sum;
  gsl_sum_levin_utrunc_workspace * ptr;
  ptr = gsl_sum_levin_utrunc_alloc(NUM2INT(n));
  sum = Data_Wrap_Struct(class, 0, LevinTrunc_free, ptr);
  argv[0] = n;
  rb_obj_call_init(sum, 1, argv);
  return sum;
}

static VALUE LevinTrunc_size(VALUE self) {
  gsl_sum_levin_utrunc_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_utrunc_workspace, ptr);
  return INT2FIX(ptr->size);
}

static VALUE LevinTrunc_terms_used(VALUE self) {
  gsl_sum_levin_utrunc_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_utrunc_workspace, ptr);
  return INT2FIX(ptr->terms_used);
}

static VALUE LevinTrunc_sum_plain(VALUE self) {
  gsl_sum_levin_utrunc_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_utrunc_workspace, ptr);
  return rb_float_new(ptr->sum_plain);
}

static VALUE LevinTrunc_accel(VALUE self, VALUE array) {
  double * my_array;
  double sum, abserr;
  int ret;
  gsl_sum_levin_utrunc_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_utrunc_workspace, ptr);
  COPYRUBYARRAY(array, my_array);
  ret = gsl_sum_levin_utrunc_accel(my_array, RARRAY(array)->len, ptr, &sum, &abserr);
  return rb_ary_new3(2, rb_float_new(sum), rb_float_new(abserr));
}

static VALUE LevinTrunc_minmax(VALUE self, VALUE array, VALUE min, VALUE max) {
  double * my_array;
  double sum, abserr;
  int ret;
  gsl_sum_levin_utrunc_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_utrunc_workspace, ptr);
  COPYRUBYARRAY(array, my_array);
  ret = gsl_sum_levin_utrunc_minmax(my_array, RARRAY(array)->len, NUM2INT(min), 
    NUM2INT(max), ptr, &sum, &abserr);
  return rb_ary_new3(2, rb_float_new(sum), rb_float_new(abserr));
}

static VALUE LevinTrunc_step(VALUE self, VALUE term, VALUE n) {
  double sum;
  int ret;
  gsl_sum_levin_utrunc_workspace * ptr;
  Data_Get_Struct(self, gsl_sum_levin_utrunc_workspace, ptr);
  ret = gsl_sum_levin_utrunc_step(NUM2INT(term), NUM2INT(n), ptr, &sum);
  return rb_float_new(sum);
}

/* Class definition */

VALUE rbgsl_mSum;
VALUE rbgsl_cLevin;
VALUE rbgsl_cLevinTrunc;

void Init_Sum() {
  rbgsl_mSum = rb_define_module_under(rbgsl_mGSL, "Sum");
  rbgsl_cLevin = rb_define_class_under(rbgsl_mSum, "Levin", rb_cObject);
  rbgsl_cLevinTrunc = rb_define_class_under(rbgsl_mSum, "LevinTrunc", rb_cObject);

  rb_define_singleton_method(rbgsl_cLevin, "new", Levin_new, 1);
  rb_define_method(rbgsl_cLevin, "initialize", Levin_init, 1);

  rb_define_method(rbgsl_cLevin, "size", Levin_size, 0);
  rb_define_method(rbgsl_cLevin, "sum_plain", Levin_sum_plain, 0);
  rb_define_method(rbgsl_cLevin, "terms_used", Levin_terms_used, 0);

  rb_define_method(rbgsl_cLevin, "accel", Levin_accel, 1);
  rb_define_method(rbgsl_cLevin, "minmax", Levin_minmax, 3);
  rb_define_method(rbgsl_cLevin, "step", Levin_step, 3);

  rb_define_singleton_method(rbgsl_cLevinTrunc, "new", LevinTrunc_new, 1);
  rb_define_method(rbgsl_cLevinTrunc, "initialize", LevinTrunc_init, 1);

  rb_define_method(rbgsl_cLevinTrunc, "size", LevinTrunc_size, 0);
  rb_define_method(rbgsl_cLevinTrunc, "sum_plain", LevinTrunc_sum_plain, 0);
  rb_define_method(rbgsl_cLevinTrunc, "terms_used", LevinTrunc_terms_used, 0);

  rb_define_method(rbgsl_cLevinTrunc, "accel", LevinTrunc_accel, 1);
  rb_define_method(rbgsl_cLevinTrunc, "minmax", LevinTrunc_minmax, 3);
  rb_define_method(rbgsl_cLevinTrunc, "step", LevinTrunc_step, 2);
}

/* vim: set ts=4: */
