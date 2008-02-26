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

/* $Id: Dawson.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_dawson.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Dawson Function */

static VALUE Dawson_dawson(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_dawson(NUM2DBL(x)));
}

static VALUE Dawson_dawson_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_dawson_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mDawson;

void Init_Dawson() {
  rbgsl_mDawson = rb_define_module_under(rbgsl_mSpecial, "Dawson");

  rb_define_module_function(rbgsl_mDawson, "dawson", Dawson_dawson, 1);
  rb_define_module_function(rbgsl_mDawson, "dawson_e", Dawson_dawson_e, 1);
}

/* vim: set ts=4: */
