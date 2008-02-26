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

/* $Id: Conical.c,v 1.2 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_legendre.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Conical Functions */

static VALUE Conical_Phalf(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_conicalP_half(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Conical_Phalf_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_conicalP_half_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Conical_Pmhalf(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_conicalP_mhalf(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Conical_Pmhalf_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_conicalP_mhalf_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Conical_P0(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_conicalP_0(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Conical_P0_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_conicalP_0_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Conical_P1(VALUE self, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_conicalP_1(NUM2DBL(lambda), NUM2DBL(x)));
}

static VALUE Conical_P1_e(VALUE self, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_conicalP_1_e(NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Conical_sph_reg(VALUE self, VALUE l, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_conicalP_sph_reg(NUM2INT(l), NUM2DBL(lambda), 
    NUM2DBL(x)));
}

static VALUE Conical_sph_reg_e(VALUE self, VALUE l, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_conicalP_sph_reg_e(NUM2INT(l), NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Conical_cyl_reg(VALUE self, VALUE m, VALUE lambda, VALUE x) {
  return rb_float_new(gsl_sf_conicalP_cyl_reg(NUM2INT(m), NUM2DBL(lambda), 
    NUM2DBL(x)));
}

static VALUE Conical_cyl_reg_e(VALUE self, VALUE l, VALUE lambda, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_conicalP_cyl_reg_e(NUM2INT(l), NUM2DBL(lambda), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mConical;

void Init_Conical() {
  rbgsl_mConical = rb_define_module_under(rbgsl_mSpecial, "Conical");

  rb_define_module_function(rbgsl_mConical, "P_half", Conical_Phalf, 2);
  rb_define_module_function(rbgsl_mConical, "P_half_e", Conical_Phalf_e, 2);
  rb_define_module_function(rbgsl_mConical, "P_mhalf", Conical_Pmhalf, 2);
  rb_define_module_function(rbgsl_mConical, "P_mhalf_e", Conical_Pmhalf_e, 2);
  rb_define_module_function(rbgsl_mConical, "P_0", Conical_P0, 2);
  rb_define_module_function(rbgsl_mConical, "P_0_e", Conical_P0_e, 2);
  rb_define_module_function(rbgsl_mConical, "P_1", Conical_P1, 2);
  rb_define_module_function(rbgsl_mConical, "P_1_e", Conical_P1_e, 2);
  rb_define_module_function(rbgsl_mConical, "P_sph_reg", Conical_sph_reg, 3);
  rb_define_module_function(rbgsl_mConical, "P_sph_reg_e", Conical_sph_reg_e, 3);
  rb_define_module_function(rbgsl_mConical, "P_cyl_reg", Conical_cyl_reg, 3);
  rb_define_module_function(rbgsl_mConical, "P_cyl_reg_e", Conical_cyl_reg_e, 3);
}

/* vim: set ts=4: */
