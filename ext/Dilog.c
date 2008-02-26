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

/* $Id: Dilog.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_dilog.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Dilogarithm */

/* Real Argument */

static VALUE Dilog_dilog(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_dilog(NUM2DBL(x)));
}

static VALUE Dilog_dilog_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_dilog_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Complex Argument */

static VALUE Dilog_complex_dilog_e(VALUE self, VALUE r, VALUE theta) {
  int ret;
  gsl_sf_result re, im;
  ret = gsl_sf_complex_dilog_e(NUM2DBL(r), NUM2DBL(theta), &re, &im);
  return rb_ary_new3(2, RESULT(&re), RESULT(&im));
}

/* Module definition */

VALUE rbgsl_mDilog;

void Init_Dilog() {
  rbgsl_mDilog = rb_define_module_under(rbgsl_mSpecial, "Dilog");

  rb_define_module_function(rbgsl_mDilog, "dilog", Dilog_dilog, 1);
  rb_define_module_function(rbgsl_mDilog, "dilog_e", Dilog_dilog_e, 1);
  rb_define_module_function(rbgsl_mDilog, "complex_dilog_e", Dilog_complex_dilog_e, 2);
}

/* vim: set ts=4: */
