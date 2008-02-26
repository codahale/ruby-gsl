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

/* $Id: FermiDirac.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_fermi_dirac.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Fermi-Dirac Function*/

/* Complete Fermi-Dirac Integrals */

static VALUE FermiDirac_Fm1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_m1(NUM2DBL(x)));
}

static VALUE FermiDirac_Fm1_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_m1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_F0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_0(NUM2DBL(x)));
}

static VALUE FermiDirac_F0_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_F1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_1(NUM2DBL(x)));
}

static VALUE FermiDirac_F1_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_F2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_2(NUM2DBL(x)));
}

static VALUE FermiDirac_F2_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_Fmhalf(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_mhalf(NUM2DBL(x)));
}

static VALUE FermiDirac_Fmhalf_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_mhalf_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_Fhalf(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_half(NUM2DBL(x)));
}

static VALUE FermiDirac_Fhalf_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_half_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_F3half(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_3half(NUM2DBL(x)));
}

static VALUE FermiDirac_F3half_e(VALUE self, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_3half_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE FermiDirac_Fint(VALUE self, VALUE j, VALUE x) {
  return rb_float_new(gsl_sf_fermi_dirac_int(NUM2INT(j), NUM2DBL(x)));
}

static VALUE FermiDirac_Fint_e(VALUE self, VALUE j, VALUE x, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_int_e(NUM2INT(j), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Incomplete Fermi-Dirac Integrals */

static VALUE FermiDirac_Finc_0(VALUE self, VALUE x, VALUE b) {
  return rb_float_new(gsl_sf_fermi_dirac_inc_0(NUM2DBL(x), NUM2DBL(b)));
}

static VALUE FermiDirac_Finc_0_e(VALUE self, VALUE x, VALUE b, VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fermi_dirac_inc_0_e(NUM2DBL(x), NUM2DBL(b), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mFermiDirac;

void Init_FermiDirac() {
  rbgsl_mFermiDirac = rb_define_module_under(rbgsl_mSpecial, "FermiDirac");

  rb_define_module_function(rbgsl_mFermiDirac, "Fm1", FermiDirac_Fm1, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "Fm1_e", FermiDirac_Fm1_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F0", FermiDirac_F0, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F0_e", FermiDirac_F0_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F1", FermiDirac_F1, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F1_e", FermiDirac_F1_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F2", FermiDirac_F2, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F2_e", FermiDirac_F2_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "Fmhalf", FermiDirac_Fmhalf, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "Fmhalf_e", FermiDirac_Fmhalf_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "Fhalf", FermiDirac_Fhalf, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "Fhalf_e", FermiDirac_Fhalf_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F3half", FermiDirac_F3half, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "F3half_e", FermiDirac_F3half_e, 1);
  rb_define_module_function(rbgsl_mFermiDirac, "Fint", FermiDirac_Fint, 2);
  rb_define_module_function(rbgsl_mFermiDirac, "Fint_e", FermiDirac_Fint_e, 2);

  rb_define_module_function(rbgsl_mFermiDirac, "Finc_0", FermiDirac_Finc_0, 2);
  rb_define_module_function(rbgsl_mFermiDirac, "Finc_0_e", FermiDirac_Finc_0_e, 2);
}

/* vim: set ts=4: */
