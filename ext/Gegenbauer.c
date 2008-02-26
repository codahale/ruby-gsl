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

/* $Id: Gegenbauer.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_sf_gegenbauer.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Gegenbauer Functions */

static VALUE Gegenbauer_poly_1(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_gegenpoly_1(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Gegenbauer_poly_1_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gegenpoly_1_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gegenbauer_poly_2(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_gegenpoly_2(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Gegenbauer_poly_2_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gegenpoly_2_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gegenbauer_poly_3(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_gegenpoly_3(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Gegenbauer_poly_3_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gegenpoly_3_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gegenbauer_poly_n(VALUE self, VALUE n, VALUE lambda, VALUE x) {
  if (NUM2INT(n) < 0)
    rb_raise(rb_eArgError, "Parameter n must be positive.");
  if (NUM2DBL(lambda) <= -0.5)
    rb_raise(rb_eArgError, "Parameter lambda must be > -1/2.");
  return rb_float_new(gsl_sf_gegenpoly_n(NUM2INT(n), NUM2DBL(lambda), 
    NUM2DBL(x)));
}

static VALUE Gegenbauer_poly_n_e(VALUE self, VALUE n, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  if (NUM2INT(n) < 0)
    rb_raise(rb_eArgError, "Parameter n must be positive.");
  if (NUM2DBL(lambda) <= -0.5)
    rb_raise(rb_eArgError, "Parameter lambda must be > -1/2.");
  ret = gsl_sf_gegenpoly_n_e(NUM2INT(n), NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gegenbauer_poly_array(VALUE self, VALUE nmax, VALUE lambda, VALUE x) {
  int ret, dim = NUM2INT(nmax);
  VALUE ary;
  double * result;
  if (dim < 0)
    rb_raise(rb_eArgError, "Array size must be positive.");
  if (NUM2DBL(lambda) <= -0.5)
    rb_raise(rb_eArgError, "Parameter lambda must be > -1/2.");
  result = (double *)ALLOC_N(double, dim);
  ret = gsl_sf_gegenpoly_array(dim, NUM2DBL(lambda), NUM2DBL(x), result);
  if (ret == GSL_EDOM)
    rb_raise(rb_eArgError, "Operation failed.");
  COPYCARRAY(result, ary, dim);
  return ary;
}

/* Module definition */

VALUE rbgsl_mGegenbauer;

void Init_Gegenbauer() {
  rbgsl_mGegenbauer = rb_define_module_under(rbgsl_mSpecial, "Gegenbauer");

  rb_define_module_function(rbgsl_mGegenbauer, "poly_1", Gegenbauer_poly_1, 2);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_1_e", Gegenbauer_poly_1_e, 2);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_2", Gegenbauer_poly_2, 2);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_2_e", Gegenbauer_poly_2_e, 2);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_3", Gegenbauer_poly_3, 2);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_3_e", Gegenbauer_poly_3_e, 2);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_n", Gegenbauer_poly_n, 3);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_n_e", Gegenbauer_poly_n_e, 3);
  rb_define_module_function(rbgsl_mGegenbauer, "poly_array", Gegenbauer_poly_array, 3);
}

/* vim: set ts=4: */
