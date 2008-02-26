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

/* $Id: Elementary.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_elementary.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Elementary Operations */

static VALUE Elementary_multiply(VALUE self, VALUE x, VALUE y) {
  return rb_float_new(gsl_sf_multiply(NUM2DBL(x), NUM2DBL(y)));
}

static VALUE Elementary_multiply_e(VALUE self, VALUE x, VALUE y) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_multiply_e(NUM2DBL(x), NUM2DBL(y), &r);
  return RESULT(&r);
}

static VALUE Elementary_multiply_err_e(VALUE self, VALUE x, VALUE dx, VALUE y,
  VALUE dy) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_multiply_err_e(NUM2DBL(x), NUM2DBL(dx), NUM2DBL(y), NUM2DBL(dy), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mElementary;

void Init_Elementary() {
  rbgsl_mElementary = rb_define_module_under(rbgsl_mSpecial, "Elementary");

  rb_define_module_function(rbgsl_mElementary, "multiply", Elementary_multiply, 2);
  rb_define_module_function(rbgsl_mElementary, "multiply_e", Elementary_multiply_e, 2);
  rb_define_module_function(rbgsl_mElementary, "multiply_err_e", Elementary_multiply_err_e, 4);
}

/* vim: set ts=4: */
