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

/* $Id: EllInt.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_ellint.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Elliptic Integrals */

/* Legendre Form of Complete Elliptic Integrals */

static VALUE EllInt_Kcomp(VALUE self, VALUE k, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_Kcomp(NUM2DBL(k), NUM2INT(mode)));
}

static VALUE EllInt_Kcomp_e(VALUE self, VALUE k, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_Kcomp_e(NUM2DBL(k), NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_Ecomp(VALUE self, VALUE k, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_Ecomp(NUM2DBL(k), NUM2INT(mode)));
}

static VALUE EllInt_Ecomp_e(VALUE self, VALUE k, VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_Ecomp_e(NUM2DBL(k), NUM2INT(mode), &r);
  return RESULT(&r);
}

/* Legendre Form of Incomplete Elliptic Integrals */

static VALUE EllInt_F(VALUE self, VALUE phi, VALUE k, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_F(NUM2DBL(phi), NUM2DBL(k), NUM2INT(mode)));
}

static VALUE EllInt_F_e(VALUE self, VALUE phi, VALUE k, VALUE mode, 
  VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_F_e(NUM2DBL(phi), NUM2DBL(k), NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_E(VALUE self, VALUE phi, VALUE k, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_E(NUM2DBL(phi), NUM2DBL(k), NUM2INT(mode)));
}

static VALUE EllInt_E_e(VALUE self, VALUE phi, VALUE k, VALUE mode, 
  VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_E_e(NUM2DBL(phi), NUM2DBL(k), NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_P(VALUE self, VALUE phi, VALUE k, VALUE n, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_P(NUM2DBL(phi), NUM2DBL(k), NUM2DBL(n), 
    NUM2INT(mode)));
}

static VALUE EllInt_P_e(VALUE self, VALUE phi, VALUE k, VALUE n, 
  VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_P_e(NUM2DBL(phi), NUM2DBL(k), NUM2DBL(n), 
    NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_D(VALUE self, VALUE phi, VALUE k, VALUE n, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_D(NUM2DBL(phi), NUM2DBL(k), NUM2DBL(n),
    NUM2INT(mode)));
}

static VALUE EllInt_D_e(VALUE self, VALUE phi, VALUE k, VALUE n, 
  VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_D_e(NUM2DBL(phi), NUM2DBL(k), NUM2DBL(n), 
    NUM2INT(mode), &r);
  return RESULT(&r);
}

/* Carlson Forms */

static VALUE EllInt_RC(VALUE self, VALUE x, VALUE y, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_RC(NUM2DBL(x), NUM2DBL(y), NUM2INT(mode)));
}

static VALUE EllInt_RC_e(VALUE self, VALUE x, VALUE y, VALUE mode, 
  VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_RC_e(NUM2DBL(x), NUM2DBL(y), NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_RD(VALUE self, VALUE x, VALUE y, VALUE z, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_RD(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z),
    NUM2INT(mode)));
}

static VALUE EllInt_RD_e(VALUE self, VALUE x, VALUE y, VALUE z, VALUE mode, 
  VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_RD_e(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), 
    NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_RF(VALUE self, VALUE x, VALUE y, VALUE z, VALUE mode) {
  return rb_float_new(gsl_sf_ellint_RF(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z),
    NUM2INT(mode)));
}

static VALUE EllInt_RF_e(VALUE self, VALUE x, VALUE y, VALUE z, VALUE mode, 
  VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_RF_e(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), NUM2INT(mode), &r);
  return RESULT(&r);
}

static VALUE EllInt_RJ(VALUE self, VALUE x, VALUE y, VALUE z, VALUE p, 
  VALUE mode) {
  return rb_float_new(gsl_sf_ellint_RJ(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z),
    NUM2DBL(p), NUM2INT(mode)));
}

static VALUE EllInt_RJ_e(VALUE self, VALUE x, VALUE y, VALUE z, VALUE p, 
  VALUE mode) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_ellint_RJ_e(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), NUM2DBL(p), 
    NUM2INT(mode), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mEllInt;

void Init_EllInt() {
  rbgsl_mEllInt = rb_define_module_under(rbgsl_mSpecial, "EllInt");

  rb_define_module_function(rbgsl_mEllInt, "Kcomp", EllInt_Kcomp, 2);
  rb_define_module_function(rbgsl_mEllInt, "Kcomp_e", EllInt_Kcomp_e, 2);
  rb_define_module_function(rbgsl_mEllInt, "Ecomp", EllInt_Ecomp, 2);
  rb_define_module_function(rbgsl_mEllInt, "Ecomp_e", EllInt_Ecomp_e, 2);

  rb_define_module_function(rbgsl_mEllInt, "E", EllInt_E, 3);
  rb_define_module_function(rbgsl_mEllInt, "E_e", EllInt_E_e, 3);
  rb_define_module_function(rbgsl_mEllInt, "F", EllInt_F, 3);
  rb_define_module_function(rbgsl_mEllInt, "F_e", EllInt_F_e, 3);
  rb_define_module_function(rbgsl_mEllInt, "P", EllInt_P, 4);
  rb_define_module_function(rbgsl_mEllInt, "P_e", EllInt_P_e, 4);
  rb_define_module_function(rbgsl_mEllInt, "D", EllInt_D, 4);
  rb_define_module_function(rbgsl_mEllInt, "D_e", EllInt_D_e, 4);

  rb_define_module_function(rbgsl_mEllInt, "RC", EllInt_RC, 3);
  rb_define_module_function(rbgsl_mEllInt, "RC_e", EllInt_RC_e, 3);
  rb_define_module_function(rbgsl_mEllInt, "RD", EllInt_RD, 4);
  rb_define_module_function(rbgsl_mEllInt, "RD_e", EllInt_RD_e, 4);
  rb_define_module_function(rbgsl_mEllInt, "RF", EllInt_RF, 4);
  rb_define_module_function(rbgsl_mEllInt, "RF_e", EllInt_RF_e, 4);
  rb_define_module_function(rbgsl_mEllInt, "RJ", EllInt_RJ, 5);
  rb_define_module_function(rbgsl_mEllInt, "RJ_e", EllInt_RJ_e, 5);
}

/* vim: set ts=4: */
