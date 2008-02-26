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

/* $Id: Exp.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_exp.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Exponential Functions */

/* Exponential Function */

static VALUE Exp_exp(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_exp(NUM2DBL(x)));
}

static VALUE Exp_exp_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exp_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Exp_exp_e10_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result_e10 r;
  ret = gsl_sf_exp_e10_e(NUM2DBL(x), &r);
  return RESULT10(&r);
}

static VALUE Exp_exp_mult(VALUE self, VALUE x, VALUE y) {
  return rb_float_new(gsl_sf_exp_mult(NUM2DBL(x), NUM2DBL(y)));
}

static VALUE Exp_exp_mult_e(VALUE self, VALUE x, VALUE y) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exp_mult_e(NUM2DBL(x), NUM2DBL(y), &r);
  return RESULT(&r);
}

static VALUE Exp_exp_mult_e10_e(VALUE self, VALUE x, VALUE y) {
  int ret;
  gsl_sf_result_e10 r;
  ret = gsl_sf_exp_mult_e10_e(NUM2DBL(x), NUM2DBL(y), &r);
  return RESULT10(&r);
}

/* Relative Exponential Functions */

static VALUE Exp_expm1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_expm1(NUM2DBL(x)));
}

static VALUE Exp_expm1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_expm1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Exp_exprel(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_exprel(NUM2DBL(x)));
}

static VALUE Exp_exprel_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exprel_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Exp_exprel_2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_exprel_2(NUM2DBL(x)));
}

static VALUE Exp_exprel_2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exprel_2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Exp_exprel_n(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_exprel_n(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Exp_exprel_n_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exprel_n_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Exponentiation with Error Estimation */

static VALUE Exp_exp_err_e(VALUE self, VALUE x, VALUE dx) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exp_err_e(NUM2DBL(x), NUM2DBL(dx), &r);
  return RESULT(&r);
}

static VALUE Exp_exp_err_e10_e(VALUE self, VALUE x, VALUE dx) {
  int ret;
  gsl_sf_result_e10 r;
  ret = gsl_sf_exp_err_e10_e(NUM2DBL(x), NUM2DBL(dx), &r);
  return RESULT10(&r);
}

static VALUE Exp_exp_mult_err_e(VALUE self, VALUE x, VALUE dx, VALUE y,
  VALUE dy) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_exp_mult_err_e(NUM2DBL(x), NUM2DBL(dx), NUM2DBL(y), 
    NUM2DBL(dy), &r);
  return RESULT(&r);
}

static VALUE Exp_exp_mult_err_e10_e(VALUE self, VALUE x, VALUE dx, VALUE y,
  VALUE dy) {
  int ret;
  gsl_sf_result_e10 r;
  ret = gsl_sf_exp_mult_err_e10_e(NUM2DBL(x), NUM2DBL(dx), NUM2DBL(y), 
    NUM2DBL(dy), &r);
  return RESULT10(&r);
}

/* Module definition */

VALUE rbgsl_mExp;

void Init_Exp() {
  rbgsl_mExp = rb_define_module_under(rbgsl_mSpecial, "Exp");

  rb_define_module_function(rbgsl_mExp, "exp", Exp_exp, 1);
  rb_define_module_function(rbgsl_mExp, "exp_e", Exp_exp_e, 1);
  rb_define_module_function(rbgsl_mExp, "exp_e10_e", Exp_exp_e10_e, 1);
  rb_define_module_function(rbgsl_mExp, "exp_mult", Exp_exp_mult, 2);
  rb_define_module_function(rbgsl_mExp, "exp_mult_e", Exp_exp_mult_e, 2);
  rb_define_module_function(rbgsl_mExp, "exp_mult_e10_e", Exp_exp_mult_e10_e, 2);

  rb_define_module_function(rbgsl_mExp, "expm1", Exp_expm1, 1);
  rb_define_module_function(rbgsl_mExp, "expm1_e", Exp_expm1_e, 1);
  rb_define_module_function(rbgsl_mExp, "exprel", Exp_exprel, 1);
  rb_define_module_function(rbgsl_mExp, "exprel_e", Exp_exprel_e, 1);
  rb_define_module_function(rbgsl_mExp, "exprel_2", Exp_exprel_2, 1);
  rb_define_module_function(rbgsl_mExp, "exprel_2_e", Exp_exprel_2_e, 1);
  rb_define_module_function(rbgsl_mExp, "exprel_n", Exp_exprel_n, 2);
  rb_define_module_function(rbgsl_mExp, "exprel_n_e", Exp_exprel_n_e, 2);

  rb_define_module_function(rbgsl_mExp, "exp_err_e", Exp_exp_err_e, 2);
  rb_define_module_function(rbgsl_mExp, "exp_err_e10_e", Exp_exp_err_e10_e, 2);
  rb_define_module_function(rbgsl_mExp, "exp_mult_err_e", Exp_exp_mult_err_e, 4);
  rb_define_module_function(rbgsl_mExp, "exp_mult_err_e10_e", Exp_exp_mult_err_e10_e, 4);
}

/* vim: set ts=4: */
