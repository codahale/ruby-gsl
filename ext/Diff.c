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

/* $Id: Diff.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_diff.h"
#include "gsl/gsl_math.h"
#include "ruby-gsl.h"

/* Numerical Differentiation */

static VALUE Diff_forward(VALUE self, VALUE f, VALUE x) {
  int ret;
  gsl_function * ptr;
  double abserr, result;
  Data_Get_Struct(f, gsl_function, ptr);
  ret = gsl_diff_forward(ptr, NUM2DBL(x), &result, &abserr);
  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

static VALUE Diff_backward(VALUE self, VALUE f, VALUE x) {
  int ret;
  gsl_function * ptr;
  double abserr, result;
  Data_Get_Struct(f, gsl_function, ptr);
  ret = gsl_diff_backward(ptr, NUM2DBL(x), &result, &abserr);
  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

static VALUE Diff_central(VALUE self, VALUE f, VALUE x) {
  int ret;
  gsl_function * ptr;
  double abserr, result;
  Data_Get_Struct(f, gsl_function, ptr);
  ret = gsl_diff_central(ptr, NUM2DBL(x), &result, &abserr);
  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

/* Module definition */

VALUE rbgsl_mDiff;

void Init_Diff() {
  rbgsl_mDiff = rb_define_module_under(rbgsl_mGSL, "Diff");

  rb_define_module_function(rbgsl_mDiff, "central", Diff_central, 2);
  rb_define_module_function(rbgsl_mDiff, "forward", Diff_forward, 2);
  rb_define_module_function(rbgsl_mDiff, "backward", Diff_backward, 2);
}

/* vim: set ts=4: */
