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

/* $Id: ExpInt.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_expint.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Exponential Integrals */

/* Exponential Integral */

static VALUE ExpInt_E1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_E1(NUM2DBL(x)));
}

static VALUE ExpInt_E1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_E1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE ExpInt_E2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_E2(NUM2DBL(x)));
}

static VALUE ExpInt_E2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_E2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE ExpInt_E1_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_E1_scaled(NUM2DBL(x)));
}

static VALUE ExpInt_E1_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_E1_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE ExpInt_E2_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_E2_scaled(NUM2DBL(x)));
}

static VALUE ExpInt_E2_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_E2_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Ei(x) */

static VALUE ExpInt_Ei(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_Ei(NUM2DBL(x)));
}

static VALUE ExpInt_Ei_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_Ei_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE ExpInt_Ei_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_Ei_scaled(NUM2DBL(x)));
}

static VALUE ExpInt_Ei_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_Ei_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Hyperbolic Integrals */

static VALUE ExpInt_Shi(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_Shi(NUM2DBL(x)));
}

static VALUE ExpInt_Shi_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_Shi_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE ExpInt_Chi(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_Chi(NUM2DBL(x)));
}

static VALUE ExpInt_Chi_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_Chi_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Ei_3(x) */

static VALUE ExpInt_Ei3(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expint_3(NUM2DBL(x)));
}

static VALUE ExpInt_Ei3_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expint_3_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Trigonometric Integrals */

static VALUE ExpInt_Si(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_Si(NUM2DBL(x)));
}

static VALUE ExpInt_Si_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_Si_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE ExpInt_Ci(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_Ci(NUM2DBL(x)));
}

static VALUE ExpInt_Ci_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_Ci_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Arctangent Integral */

static VALUE ExpInt_atanint(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_atanint(NUM2DBL(x)));
}

static VALUE ExpInt_atanint_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_atanint_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mExpInt;

void Init_ExpInt() {
  rbgsl_mExpInt = rb_define_module_under(rbgsl_mSpecial, "ExpInt");

  rb_define_module_function(rbgsl_mExpInt, "E1", ExpInt_E1, 1);
  rb_define_module_function(rbgsl_mExpInt, "E1_e", ExpInt_E1_e, 1);
  rb_define_module_function(rbgsl_mExpInt, "E2", ExpInt_E2, 1);
  rb_define_module_function(rbgsl_mExpInt, "E2_e", ExpInt_E2_e, 1);
  rb_define_module_function(rbgsl_mExpInt, "E1_scaled", ExpInt_E1_scaled, 1);
  rb_define_module_function(rbgsl_mExpInt, "E1_scaled_e", ExpInt_E1_scaled_e, 1);
  rb_define_module_function(rbgsl_mExpInt, "E2_scaled", ExpInt_E2_scaled, 1);
  rb_define_module_function(rbgsl_mExpInt, "E2_scaled_e", ExpInt_E2_scaled_e, 1);
  
  rb_define_module_function(rbgsl_mExpInt, "Ei", ExpInt_Ei, 1);
  rb_define_module_function(rbgsl_mExpInt, "Ei_e", ExpInt_Ei_e, 1);
  rb_define_module_function(rbgsl_mExpInt, "Ei_scaled", ExpInt_Ei_scaled, 1);
  rb_define_module_function(rbgsl_mExpInt, "Ei_scaled_e", ExpInt_Ei_scaled_e, 1);
  
  rb_define_module_function(rbgsl_mExpInt, "Shi", ExpInt_Shi, 1);
  rb_define_module_function(rbgsl_mExpInt, "Shi_e", ExpInt_Shi_e, 1);
  rb_define_module_function(rbgsl_mExpInt, "Chi", ExpInt_Chi, 1);
  rb_define_module_function(rbgsl_mExpInt, "Chi_e", ExpInt_Chi_e, 1);
  
  rb_define_module_function(rbgsl_mExpInt, "Ei3", ExpInt_Ei3, 1);
  rb_define_module_function(rbgsl_mExpInt, "Ei3_e", ExpInt_Ei3_e, 1);

  rb_define_module_function(rbgsl_mExpInt, "Si", ExpInt_Si, 1);
  rb_define_module_function(rbgsl_mExpInt, "Si_e", ExpInt_Si_e, 1);
  rb_define_module_function(rbgsl_mExpInt, "Ci", ExpInt_Ci, 1);
  rb_define_module_function(rbgsl_mExpInt, "Ci_e", ExpInt_Ci_e, 1);

  rb_define_module_function(rbgsl_mExpInt, "atanint", ExpInt_atanint, 1);
  rb_define_module_function(rbgsl_mExpInt, "atanint_e", ExpInt_atanint_e, 1);
}

/* vim: set ts=4: */
