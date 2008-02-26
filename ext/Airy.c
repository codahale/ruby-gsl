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


/* $Id: Airy.c,v 1.2 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_airy.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_errno.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Airy Functions and Derivatives */

/* Airy Functions */

static VALUE Airy_Ai(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Ai(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Ai_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Ai_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

static VALUE Airy_Bi(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Bi(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Bi_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Bi_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

static VALUE Airy_Ai_scaled(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Ai_scaled(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Ai_scaled_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Ai_scaled_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

static VALUE Airy_Bi_scaled(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Bi_scaled(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Bi_scaled_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Bi_scaled_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

/* Derivatives of Airy Functions */

static VALUE Airy_Ai_deriv(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Ai_deriv(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Ai_deriv_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Ai_deriv_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

static VALUE Airy_Bi_deriv(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Bi_deriv(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Bi_deriv_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Bi_deriv_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

static VALUE Airy_Ai_deriv_scaled(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Ai_deriv_scaled(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Ai_deriv_scaled_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Ai_deriv_scaled_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

static VALUE Airy_Bi_deriv_scaled(VALUE self, VALUE x, VALUE mode) {
  return rb_float_new(gsl_sf_airy_Bi_deriv_scaled(NUM2DBL(x), NUM2UINT(mode)));
}

static VALUE Airy_Bi_deriv_scaled_e(VALUE self, VALUE x, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_Bi_deriv_scaled_e(NUM2DBL(x), NUM2UINT(mode), &r);
  return RESULT(&r);
}

/* Zeroes of Airy Functions */

static VALUE Airy_zero_Ai(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_airy_zero_Ai(NUM2UINT(s)));
}

static VALUE Airy_zero_Ai_e(VALUE self, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_zero_Ai_e(NUM2UINT(s), &r);
  return RESULT(&r);
}

static VALUE Airy_zero_Bi(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_airy_zero_Bi(NUM2UINT(s)));
}

static VALUE Airy_zero_Bi_e(VALUE self, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_zero_Bi_e(NUM2UINT(s), &r);
  return RESULT(&r);
}

/* Zeroes of Derivatives of Airy Functions */

static VALUE Airy_zero_Ai_deriv(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_airy_zero_Ai_deriv(NUM2UINT(s)));
}

static VALUE Airy_zero_Ai_deriv_e(VALUE self, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_zero_Ai_deriv_e(NUM2UINT(s), &r);
  return RESULT(&r);
}

static VALUE Airy_zero_Bi_deriv(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_airy_zero_Bi_deriv(NUM2UINT(s)));
}

static VALUE Airy_zero_Bi_deriv_e(VALUE self, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_airy_zero_Bi_deriv_e(NUM2UINT(s), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mAiry;

void Init_Airy() {
  rbgsl_mAiry = rb_define_module_under(rbgsl_mSpecial, "Airy");

  rb_define_module_function(rbgsl_mAiry, "Ai", Airy_Ai, 2);
  rb_define_module_function(rbgsl_mAiry, "Ai_e", Airy_Ai_e, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi", Airy_Bi, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_e", Airy_Bi_e, 2);
  rb_define_module_function(rbgsl_mAiry, "Ai_scaled", Airy_Ai_scaled, 2);
  rb_define_module_function(rbgsl_mAiry, "Ai_scaled_e", Airy_Ai_scaled_e, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_scaled", Airy_Bi_scaled, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_scaled_e", Airy_Bi_scaled_e, 2);

  rb_define_module_function(rbgsl_mAiry, "Ai_deriv", Airy_Ai_deriv, 2);
  rb_define_module_function(rbgsl_mAiry, "Ai_deriv_e", Airy_Ai_deriv_e, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_deriv", Airy_Bi_deriv, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_deriv_e", Airy_Bi_deriv_e, 2);
  rb_define_module_function(rbgsl_mAiry, "Ai_deriv_scaled", Airy_Ai_deriv_scaled, 2);
  rb_define_module_function(rbgsl_mAiry, "Ai_deriv_scaled_e", Airy_Ai_deriv_scaled_e, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_deriv_scaled", Airy_Bi_deriv_scaled, 2);
  rb_define_module_function(rbgsl_mAiry, "Bi_deriv_scaled_e", Airy_Bi_deriv_scaled_e, 2);

  rb_define_module_function(rbgsl_mAiry, "zero_Ai", Airy_zero_Ai, 1);
  rb_define_module_function(rbgsl_mAiry, "zero_Ai_e", Airy_zero_Ai_e, 1);
  rb_define_module_function(rbgsl_mAiry, "zero_Bi", Airy_zero_Bi, 1);
  rb_define_module_function(rbgsl_mAiry, "zero_Bi_e", Airy_zero_Bi_e, 1);

  rb_define_module_function(rbgsl_mAiry, "zero_Ai_deriv", Airy_zero_Ai_deriv, 1);
  rb_define_module_function(rbgsl_mAiry, "zero_Ai_deriv_e", Airy_zero_Ai_deriv_e, 1);
  rb_define_module_function(rbgsl_mAiry, "zero_Bi_deriv", Airy_zero_Bi_deriv, 1);
  rb_define_module_function(rbgsl_mAiry, "zero_Bi_deriv_e", Airy_zero_Bi_deriv_e, 1);
}

/* vim: set ts=4: */
