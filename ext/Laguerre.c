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

/* $Id: Laguerre.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_laguerre.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Laguerre Functions */

static VALUE Laguerre_L1(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_laguerre_1(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Laguerre_L1_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_laguerre_1_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Laguerre_L2(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_laguerre_2(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Laguerre_L2_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_laguerre_2_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Laguerre_L3(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_laguerre_3(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Laguerre_L3_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_laguerre_3_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Laguerre_Ln(VALUE self, VALUE n, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_laguerre_n(NUM2INT(n), NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Laguerre_Ln_e(VALUE self, VALUE n, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_laguerre_n_e(NUM2INT(n), NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mLaguerre;

void Init_Laguerre() {
  rbgsl_mLaguerre = rb_define_module_under(rbgsl_mSpecial, "Laguerre");

  rb_define_module_function(rbgsl_mLaguerre, "L1", Laguerre_L1, 2);
  rb_define_module_function(rbgsl_mLaguerre, "L1_e", Laguerre_L1_e, 2);
  rb_define_module_function(rbgsl_mLaguerre, "L2", Laguerre_L2, 2);
  rb_define_module_function(rbgsl_mLaguerre, "L2_e", Laguerre_L2_e, 2);
  rb_define_module_function(rbgsl_mLaguerre, "L3", Laguerre_L3, 2);
  rb_define_module_function(rbgsl_mLaguerre, "L3_e", Laguerre_L3_e, 2);
  rb_define_module_function(rbgsl_mLaguerre, "Ln", Laguerre_Ln, 3);
  rb_define_module_function(rbgsl_mLaguerre, "Ln_e", Laguerre_Ln_e, 3);
}

/* vim: set ts=4: */
