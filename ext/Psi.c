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

/* $Id: Psi.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_psi.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Psi (Digamma) Function */

/* Digamma Function */

static VALUE Psi_psi(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_psi(NUM2DBL(x)));
}

static VALUE Psi_psi_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_psi_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Psi_psi_int(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_psi_int(NUM2INT(n)));
}

static VALUE Psi_psi_int_e(VALUE self, VALUE n) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_psi_int_e(NUM2INT(n), &r);
  return RESULT(&r);
}

static VALUE Psi_psi_1piy(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_psi_1piy(NUM2DBL(x)));
}

static VALUE Psi_psi_1piy_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_psi_1piy_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Trigamma Function */

static VALUE Psi_psi_1_int(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_psi_1_int(NUM2INT(n)));
}

static VALUE Psi_psi_1_int_e(VALUE self, VALUE n) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_psi_1_int_e(NUM2INT(n), &r);
  return RESULT(&r);
}

/* Polygamma Function */

static VALUE Psi_psi_n(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_psi_n(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Psi_psi_n_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_psi_n_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mPsi;

void Init_Psi() {
  rbgsl_mPsi = rb_define_module_under(rbgsl_mSpecial, "Psi");

  rb_define_module_function(rbgsl_mPsi, "psi", Psi_psi, 1);
  rb_define_module_function(rbgsl_mPsi, "psi_e", Psi_psi_e, 1);
  rb_define_module_function(rbgsl_mPsi, "psi_int", Psi_psi_int, 1);
  rb_define_module_function(rbgsl_mPsi, "psi_int_e", Psi_psi_int_e, 1);
  rb_define_module_function(rbgsl_mPsi, "psi_1piy", Psi_psi_1piy, 1);
  rb_define_module_function(rbgsl_mPsi, "psi_1piy_e", Psi_psi_1piy_e, 1);

  rb_define_module_function(rbgsl_mPsi, "psi_1_int", Psi_psi_1_int, 1);
  rb_define_module_function(rbgsl_mPsi, "psi_1_int_e", Psi_psi_1_int_e, 1);

  rb_define_module_function(rbgsl_mPsi, "psi_n", Psi_psi_n, 2);
  rb_define_module_function(rbgsl_mPsi, "psi_n_e", Psi_psi_n_e, 2);
}

/* vim: set ts=4: */
