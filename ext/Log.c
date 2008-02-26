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

/* $Id: Log.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_log.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Logarithm and Related Functions */

static VALUE Log_log(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_log(NUM2DBL(x)));
}

static VALUE Log_log_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_log_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Log_log_abs(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_log_abs(NUM2DBL(x)));
}

static VALUE Log_log_abs_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_log_abs_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Log_log_1px(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_log_1plusx(NUM2DBL(x)));
}

static VALUE Log_log_1px_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_log_1plusx_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Log_log_1px_mx(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_log_1plusx_mx(NUM2DBL(x)));
}

static VALUE Log_log_1px_mx_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_log_1plusx_mx_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mLog;

void Init_Log() {
  rbgsl_mLog = rb_define_module_under(rbgsl_mSpecial, "Log");

  rb_define_module_function(rbgsl_mLog, "log", Log_log, 1);
  rb_define_module_function(rbgsl_mLog, "log_e", Log_log_e, 1);
  rb_define_module_function(rbgsl_mLog, "log_abs", Log_log_abs, 1);
  rb_define_module_function(rbgsl_mLog, "log_abs_e", Log_log_abs_e, 1);
  rb_define_module_function(rbgsl_mLog, "log_1px", Log_log_1px, 1);
  rb_define_module_function(rbgsl_mLog, "log_1px_e", Log_log_1px_e, 1);
  rb_define_module_function(rbgsl_mLog, "log_1px_mx", Log_log_1px_mx, 1);
  rb_define_module_function(rbgsl_mLog, "log_1px_mx_e", Log_log_1px_mx_e, 1);
}

/* vim: set ts=4: */
