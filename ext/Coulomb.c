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

/* $Id: Coulomb.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_sf_coulomb.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Coulomb Functions */

/* Normalized Hydrogenic Bound States */

static VALUE Coulomb_hydrogenicR_1(VALUE self, VALUE z, VALUE r) {
  return rb_float_new(gsl_sf_hydrogenicR_1(NUM2DBL(z), NUM2DBL(r)));
}

static VALUE Coulomb_hydrogenicR_1_e(VALUE self, VALUE z, VALUE r) {
  gsl_sf_result my_result;
  gsl_sf_hydrogenicR_1_e(NUM2DBL(z), NUM2DBL(r), &my_result);
  return RESULT(&my_result);
}

static VALUE Coulomb_hydrogenicR(VALUE self, VALUE n, VALUE l, VALUE z, VALUE r) {
  return rb_float_new(gsl_sf_hydrogenicR(NUM2INT(n), NUM2INT(l), NUM2DBL(z), NUM2DBL(r)));
}

static VALUE Coulomb_hydrogenicR_e(VALUE self, VALUE n, VALUE l, VALUE z, 
  VALUE r) {
  gsl_sf_result my_result;
  gsl_sf_hydrogenicR_e(NUM2INT(n), NUM2INT(l), NUM2DBL(z), NUM2DBL(r), 
    &my_result);
  return RESULT(&my_result);
}

/* TODO: Coulomb Wave Functions */

/* Coulomb Wave Function Normalization Constant */

static VALUE Coulomb_CL_e(VALUE self, VALUE l, VALUE eta) {
  gsl_sf_result my_result;
  gsl_sf_coulomb_CL_e(NUM2DBL(l), NUM2DBL(eta), &my_result);
  return RESULT(&my_result);
}

static VALUE Coulomb_CL_array(VALUE self, VALUE l, VALUE k, VALUE eta) {
  double * cl;
  VALUE ary;
  int ret;
  int kk = NUM2INT(k);
  cl = ALLOC_N(double, kk+1);
  ret = gsl_sf_coulomb_CL_array(NUM2DBL(l), kk, NUM2DBL(eta), cl);
  if (ret != GSL_SUCCESS)
    rb_raise(rb_eStandardError, "Error in CL_array");
  COPYCARRAY(cl, ary, kk+1);
  return ary;
}

/* Module definition */

VALUE rbgsl_mCoulomb;

void Init_Coulomb() {
  rbgsl_mCoulomb = rb_define_module_under(rbgsl_mSpecial, "Coulomb");

  rb_define_module_function(rbgsl_mCoulomb, "hydrogenicR", Coulomb_hydrogenicR, 4);
  rb_define_module_function(rbgsl_mCoulomb, "hydrogenicR_e", Coulomb_hydrogenicR_e, 4);
  rb_define_module_function(rbgsl_mCoulomb, "hydrogenicR_1", Coulomb_hydrogenicR_1, 2);
  rb_define_module_function(rbgsl_mCoulomb, "hydrogenicR_1_e", Coulomb_hydrogenicR_1_e, 2);

  rb_define_module_function(rbgsl_mCoulomb, "CL_e", Coulomb_CL_e, 2);
  rb_define_module_function(rbgsl_mCoulomb, "CL_array", Coulomb_CL_array, 3);
}

/* vim: set ts=4: */
