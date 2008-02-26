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

/* $Id: Debye.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_debye.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Debye Functions */

static VALUE Debye_D1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_debye_1(NUM2DBL(x)));
}

static VALUE Debye_D1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_debye_1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Debye_D2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_debye_2(NUM2DBL(x)));
}

static VALUE Debye_D2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_debye_2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Debye_D3(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_debye_3(NUM2DBL(x)));
}

static VALUE Debye_D3_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_debye_3_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Debye_D4(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_debye_4(NUM2DBL(x)));
}

static VALUE Debye_D4_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_debye_4_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mDebye;

void Init_Debye() {
  rbgsl_mDebye = rb_define_module_under(rbgsl_mSpecial, "Debye");

  rb_define_module_function(rbgsl_mDebye, "D1", Debye_D1, 1);
  rb_define_module_function(rbgsl_mDebye, "D1_e", Debye_D1_e, 1);
  rb_define_module_function(rbgsl_mDebye, "D2", Debye_D2, 1);
  rb_define_module_function(rbgsl_mDebye, "D2_e", Debye_D2_e, 1);
  rb_define_module_function(rbgsl_mDebye, "D3", Debye_D3, 1);
  rb_define_module_function(rbgsl_mDebye, "D3_e", Debye_D3_e, 1);
  rb_define_module_function(rbgsl_mDebye, "D4", Debye_D4, 1);
  rb_define_module_function(rbgsl_mDebye, "D4_e", Debye_D4_e, 1);
}

/* vim: set ts=4: */
