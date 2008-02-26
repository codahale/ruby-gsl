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

/* $Id: Hyperg.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_hyperg.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Hypergeometric Functions */

static VALUE Hyperg_F01(VALUE self, VALUE c, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_0F1(NUM2DBL(c), NUM2DBL(x)));
}

static VALUE Hyperg_F01_e(VALUE self, VALUE c, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_0F1_e(NUM2DBL(c), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F11(VALUE self, VALUE a, VALUE b, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_1F1(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x)));
}

static VALUE Hyperg_F11_e(VALUE self, VALUE a, VALUE b, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_1F1_e(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F11_int(VALUE self, VALUE m, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_1F1_int(NUM2INT(m), NUM2INT(n), NUM2DBL(x)));
}

static VALUE Hyperg_F11_int_e(VALUE self, VALUE m, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_1F1_int_e(NUM2INT(m), NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_U(VALUE self, VALUE a, VALUE b, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_U(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x)));
}

static VALUE Hyperg_U_e(VALUE self, VALUE a, VALUE b, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_U_e(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_U_int(VALUE self, VALUE m, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_U_int(NUM2INT(m), NUM2INT(n), NUM2DBL(x)));
}

static VALUE Hyperg_U_int_e(VALUE self, VALUE m, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_U_int_e(NUM2INT(m), NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F21(VALUE self, VALUE a, VALUE b, VALUE c, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_2F1(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), 
    NUM2DBL(x)));
}

static VALUE Hyperg_F21_e(VALUE self, VALUE a, VALUE b, VALUE c, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_2F1_e(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F21_conj(VALUE self, VALUE aR, VALUE bI, VALUE c, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_2F1_conj(NUM2DBL(aR), NUM2DBL(bI), 
    NUM2DBL(c), NUM2DBL(x)));
}

static VALUE Hyperg_F21_conj_e(VALUE self, VALUE aR, VALUE bI, VALUE c, 
  VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_2F1_conj_e(NUM2DBL(aR), NUM2DBL(bI), NUM2DBL(c), 
    NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F21_renorm(VALUE self, VALUE a, VALUE b, VALUE c, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_2F1_renorm(NUM2DBL(a), NUM2DBL(b), 
    NUM2DBL(c), NUM2DBL(x)));
}

static VALUE Hyperg_F21_renorm_e(VALUE self, VALUE a, VALUE b, VALUE c, 
  VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_2F1_renorm_e(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), 
    NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F21_conj_renorm(VALUE self, VALUE aR, VALUE bI, VALUE c, 
  VALUE x) {
  return rb_float_new(gsl_sf_hyperg_2F1_conj_renorm(NUM2DBL(aR), NUM2DBL(bI), 
    NUM2DBL(c), NUM2DBL(x)));
}

static VALUE Hyperg_F21_conj_renorm_e(VALUE self, VALUE aR, VALUE bI, VALUE c, 
  VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_2F1_conj_renorm_e(NUM2DBL(aR), NUM2DBL(bI), NUM2DBL(c), 
    NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Hyperg_F20(VALUE self, VALUE a, VALUE b, VALUE x) {
  return rb_float_new(gsl_sf_hyperg_2F0(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x)));
}

static VALUE Hyperg_F20_e(VALUE self, VALUE a, VALUE b, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hyperg_2F0_e(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mHyperg;

void Init_Hyperg() {
  rbgsl_mHyperg = rb_define_module_under(rbgsl_mSpecial, "Hyperg");

  rb_define_module_function(rbgsl_mHyperg, "F01", Hyperg_F01, 2);
  rb_define_module_function(rbgsl_mHyperg, "F01_e", Hyperg_F01_e, 2);
  rb_define_module_function(rbgsl_mHyperg, "F11_int", Hyperg_F11_int, 3);
  rb_define_module_function(rbgsl_mHyperg, "F11_int_e", Hyperg_F11_int_e, 3);
  rb_define_module_function(rbgsl_mHyperg, "F11", Hyperg_F11, 3);
  rb_define_module_function(rbgsl_mHyperg, "F11_e", Hyperg_F11_e, 3);
  rb_define_module_function(rbgsl_mHyperg, "U_int", Hyperg_U_int, 3);
  rb_define_module_function(rbgsl_mHyperg, "U_int_e", Hyperg_U_int_e, 3);
  rb_define_module_function(rbgsl_mHyperg, "U", Hyperg_U, 3);
  rb_define_module_function(rbgsl_mHyperg, "U_e", Hyperg_U_e, 3);
  rb_define_module_function(rbgsl_mHyperg, "F21", Hyperg_F21, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_e", Hyperg_F21_e, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_conj", Hyperg_F21_conj, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_conj_e", Hyperg_F21_conj_e, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_renorm", Hyperg_F21_renorm, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_renorm_e", Hyperg_F21_renorm_e, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_conj_renorm", Hyperg_F21_conj_renorm, 4);
  rb_define_module_function(rbgsl_mHyperg, "F21_conj_renorm_e", Hyperg_F21_conj_renorm_e, 4);
  rb_define_module_function(rbgsl_mHyperg, "F20", Hyperg_F20, 3);
  rb_define_module_function(rbgsl_mHyperg, "F20_e", Hyperg_F20_e, 3);
}

/* vim: set ts=4: */
