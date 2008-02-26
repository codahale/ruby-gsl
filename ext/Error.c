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

/* $Id: Error.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_erf.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Error Functions */

/* Error Function */

static VALUE Error_erf(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_erf(NUM2DBL(x)));
}

static VALUE Error_erf_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_erf_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Complementary Error Function */

static VALUE Error_erfc(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_erfc(NUM2DBL(x)));
}

static VALUE Error_erfc_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_erfc_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Log Complementary Error Function */

static VALUE Error_log_erfc(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_log_erfc(NUM2DBL(x)));
}

static VALUE Error_log_erfc_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_log_erfc_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Probability Functions */

static VALUE Error_erf_Z(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_erf_Z(NUM2DBL(x)));
}

static VALUE Error_erf_Z_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_erf_Z_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Error_erf_Q(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_erf_Q(NUM2DBL(x)));
}

static VALUE Error_erf_Q_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_erf_Q_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mError;

void Init_Error() {
  rbgsl_mError = rb_define_module_under(rbgsl_mSpecial, "Error");

  rb_define_module_function(rbgsl_mError, "erf", Error_erf, 1);
  rb_define_module_function(rbgsl_mError, "erf_e", Error_erf_e, 1);
  rb_define_module_function(rbgsl_mError, "erfc", Error_erfc, 1);
  rb_define_module_function(rbgsl_mError, "erfc_e", Error_erfc_e, 1);
  
  rb_define_module_function(rbgsl_mError, "log_erf", Error_log_erfc, 1);
  rb_define_module_function(rbgsl_mError, "log_erf_e", Error_log_erfc_e, 1);

  rb_define_module_function(rbgsl_mError, "erf_Z", Error_erf_Z, 1);
  rb_define_module_function(rbgsl_mError, "erf_Z_e", Error_erf_Z_e, 1);
  rb_define_module_function(rbgsl_mError, "erf_Q", Error_erf_Q, 1);
  rb_define_module_function(rbgsl_mError, "erf_Q_e", Error_erf_Q_e, 1);
}

/* vim: set ts=4: */
