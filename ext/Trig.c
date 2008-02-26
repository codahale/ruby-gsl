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

/* $Id: Trig.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_sf_trig.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Trigonometric Functions */

/* Circular Trigonometric Functions */

static VALUE Trig_sin(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_sin(NUM2DBL(x)));
}

static VALUE Trig_sin_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_sin_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Trig_cos(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_cos(NUM2DBL(x)));
}

static VALUE Trig_cos_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_cos_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Trig_hypot(VALUE self, VALUE x, VALUE y) {
  return rb_float_new(gsl_sf_hypot(NUM2DBL(x), NUM2DBL(y)));
}

static VALUE Trig_hypot_e(VALUE self, VALUE x, VALUE y) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hypot_e(NUM2DBL(x), NUM2DBL(y), &r);
  return RESULT(&r);
}

static VALUE Trig_sinc(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_sinc(NUM2DBL(x)));
}

static VALUE Trig_sinc_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_sinc_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Trigonometric Functions for Complex Arguments */

static VALUE Trig_complex_sin_e(VALUE self, VALUE zr, VALUE zi) {
  int ret;
  gsl_sf_result r1, r2;
  ret = gsl_sf_complex_sin_e(NUM2DBL(zr), NUM2DBL(zi), &r1, &r2);
  return rb_ary_new3(2, RESULT(&r1), RESULT(&r2));
}

static VALUE Trig_complex_cos_e(VALUE self, VALUE zr, VALUE zi) {
  int ret;
  gsl_sf_result r1, r2;
  ret = gsl_sf_complex_cos_e(NUM2DBL(zr), NUM2DBL(zi), &r1, &r2);
  return rb_ary_new3(2, RESULT(&r1), RESULT(&r2));
}

static VALUE Trig_complex_logsin_e(VALUE self, VALUE zr, VALUE zi) {
  int ret;
  gsl_sf_result r1, r2;
  ret = gsl_sf_complex_logsin_e(NUM2DBL(zr), NUM2DBL(zi), &r1, &r2);
  return rb_ary_new3(2, RESULT(&r1), RESULT(&r2));
}

/* Hyperbolic Trigonometric Functions */

static VALUE Trig_lnsinh(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_lnsinh(NUM2DBL(x)));
}

static VALUE Trig_lnsinh_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lnsinh_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Trig_lncosh(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_lncosh(NUM2DBL(x)));
}

static VALUE Trig_lncosh_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lncosh_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Conversion Functions */

static VALUE Trig_polar_to_rect(VALUE self, VALUE r, VALUE theta) {
  int ret;
  gsl_sf_result my_x, my_y;
  ret = gsl_sf_polar_to_rect(NUM2DBL(r), NUM2DBL(theta), &my_x, &my_y);
  return rb_ary_new3(2, RESULT(&my_x), RESULT(&my_y));
}

static VALUE Trig_rect_to_polar(VALUE self, VALUE x, VALUE y) {
  int ret;
  gsl_sf_result my_r, my_theta;
  ret = gsl_sf_rect_to_polar(NUM2DBL(x), NUM2DBL(y), &my_r, &my_theta);
  return rb_ary_new3(2, RESULT(&my_r), RESULT(&my_theta));
}

/* Restriction Functions */

static VALUE Trig_angle_restrict_symm(VALUE self, VALUE theta) {
  return rb_float_new(gsl_sf_angle_restrict_symm(NUM2DBL(theta)));
}

static VALUE Trig_angle_restrict_symm_err_e(VALUE self, VALUE theta) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_angle_restrict_symm_err_e(NUM2DBL(theta), &r);
  return RESULT(&r);
}

static VALUE Trig_angle_restrict_pos(VALUE self, VALUE theta) {
  return rb_float_new(gsl_sf_angle_restrict_pos(NUM2DBL(theta)));
}

static VALUE Trig_angle_restrict_pos_err_e(VALUE self, VALUE theta) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_angle_restrict_pos_err_e(NUM2DBL(theta), &r);
  return RESULT(&r);
}

/* Trigonometric Functions With Error Estimates */

static VALUE Trig_sin_err_e(VALUE self, VALUE x, VALUE dx) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_sin_err_e(NUM2DBL(x), NUM2DBL(dx), &r);
  return RESULT(&r);
}

static VALUE Trig_cos_err_e(VALUE self, VALUE x, VALUE dx) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_cos_err_e(NUM2DBL(x), NUM2DBL(dx), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mTrig;

void Init_Trig() {
  rbgsl_mTrig = rb_define_module_under(rbgsl_mSpecial, "Trig");

  rb_define_module_function(rbgsl_mTrig, "sin", Trig_sin, 1);
  rb_define_module_function(rbgsl_mTrig, "sin_e", Trig_sin_e, 1);
  rb_define_module_function(rbgsl_mTrig, "cos", Trig_cos, 1);
  rb_define_module_function(rbgsl_mTrig, "cos_e", Trig_cos_e, 1);
  rb_define_module_function(rbgsl_mTrig, "hypot", Trig_hypot, 2);
  rb_define_module_function(rbgsl_mTrig, "hypot_e", Trig_hypot_e, 2);
  rb_define_module_function(rbgsl_mTrig, "sinc", Trig_sinc, 1);
  rb_define_module_function(rbgsl_mTrig, "sinc_e", Trig_sinc_e, 1);

  rb_define_module_function(rbgsl_mTrig, "complex_sin_e", Trig_complex_sin_e, 2);
  rb_define_module_function(rbgsl_mTrig, "complex_cos_e", Trig_complex_cos_e, 2);
  rb_define_module_function(rbgsl_mTrig, "complex_logsin_e", Trig_complex_logsin_e, 2);

  rb_define_module_function(rbgsl_mTrig, "lnsinh", Trig_lnsinh, 1);
  rb_define_module_function(rbgsl_mTrig, "lnsinh_e", Trig_lnsinh_e, 1);
  rb_define_module_function(rbgsl_mTrig, "lncosh", Trig_lncosh, 1);
  rb_define_module_function(rbgsl_mTrig, "lncosh_e", Trig_lncosh_e, 1);

  rb_define_module_function(rbgsl_mTrig, "polar2rect", Trig_polar_to_rect, 2);
  rb_define_module_function(rbgsl_mTrig, "rect2polar", Trig_rect_to_polar, 2);

  rb_define_module_function(rbgsl_mTrig, "angle_restrict_symm", Trig_angle_restrict_symm, 1);
  rb_define_module_function(rbgsl_mTrig, "angle_restrict_symm_err_e", Trig_angle_restrict_symm_err_e, 1);
  rb_define_module_function(rbgsl_mTrig, "angle_restrict_pos", Trig_angle_restrict_pos, 1);
  rb_define_module_function(rbgsl_mTrig, "angle_restrict_pos_err_e", Trig_angle_restrict_pos_err_e, 1);

  rb_define_module_function(rbgsl_mTrig, "sin_err_e", Trig_sin_err_e, 2);
  rb_define_module_function(rbgsl_mTrig, "cos_err_e", Trig_cos_err_e, 2);
}

/* vim: set ts=4: */
