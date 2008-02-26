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

/* $Id: Zeta.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_zeta.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Zeta Functions */

/* Riemann Zeta Functions */

static VALUE Zeta_zeta(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_zeta(NUM2DBL(s)));
}

static VALUE Zeta_zeta_e(VALUE self, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_zeta_e(NUM2DBL(s), &r);
  return RESULT(&r);
}

static VALUE Zeta_zeta_int(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_zeta_int(NUM2INT(n)));
}

static VALUE Zeta_zeta_int_e(VALUE self, VALUE n) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_zeta_int_e(NUM2INT(n), &r);
  return RESULT(&r);
}

/* Hurwitz Zeta Functions */

static VALUE Zeta_hzeta(VALUE self, VALUE s, VALUE q) {
  return rb_float_new(gsl_sf_hzeta(NUM2DBL(s), NUM2DBL(q)));
}

static VALUE Zeta_hzeta_e(VALUE self, VALUE s, VALUE q) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_hzeta_e(NUM2DBL(s), NUM2DBL(q), &r);
  return RESULT(&r);
}

/* Eta Functions */

static VALUE Zeta_eta(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_eta(NUM2DBL(s)));
}

static VALUE Zeta_eta_e(VALUE self, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_eta_e(NUM2DBL(s), &r);
  return RESULT(&r);
}

static VALUE Zeta_eta_int(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_eta_int(NUM2INT(n)));
}

static VALUE Zeta_eta_int_e(VALUE self, VALUE n) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_zeta_int_e(NUM2INT(n), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mZeta;

void Init_Zeta() {
  rbgsl_mZeta = rb_define_module_under(rbgsl_mSpecial, "Zeta");

  rb_define_module_function(rbgsl_mZeta, "zeta", Zeta_zeta, 1);
  rb_define_module_function(rbgsl_mZeta, "zeta_e", Zeta_zeta_e, 1);
  rb_define_module_function(rbgsl_mZeta, "zeta_int", Zeta_zeta_int, 1);
  rb_define_module_function(rbgsl_mZeta, "zeta_int_e", Zeta_zeta_int_e, 1);

  rb_define_module_function(rbgsl_mZeta, "hzeta", Zeta_hzeta, 2);
  rb_define_module_function(rbgsl_mZeta, "hzeta_e", Zeta_hzeta_e, 2);

  rb_define_module_function(rbgsl_mZeta, "eta", Zeta_eta, 1);
  rb_define_module_function(rbgsl_mZeta, "eta_e", Zeta_eta_e, 1);
  rb_define_module_function(rbgsl_mZeta, "eta_int", Zeta_eta_int, 1);
  rb_define_module_function(rbgsl_mZeta, "eta_int_e", Zeta_eta_int_e, 1);
}

/* vim: set ts=4: */
