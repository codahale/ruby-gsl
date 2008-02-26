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

/* $Id: Lambert.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_lambert.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Lambert W Functions */

static VALUE Lambert_W0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_lambert_W0(NUM2DBL(x)));
}

static VALUE Lambert_W0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lambert_W0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Lambert_Wm1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_lambert_Wm1(NUM2DBL(x)));
}

static VALUE Lambert_Wm1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lambert_Wm1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mLambert;

void Init_Lambert() {
  rbgsl_mLambert = rb_define_module_under(rbgsl_mSpecial, "Lambert");

  rb_define_module_function(rbgsl_mLambert, "W0", Lambert_W0, 1);
  rb_define_module_function(rbgsl_mLambert, "W0_e", Lambert_W0_e, 1);
  rb_define_module_function(rbgsl_mLambert, "Wm1", Lambert_Wm1, 1);
  rb_define_module_function(rbgsl_mLambert, "Wm1_e", Lambert_Wm1_e, 1);
}

/* vim: set ts=4: */
