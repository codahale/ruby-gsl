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

/* $Id: Legendre.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_legendre.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_errno.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Legendre Functions and Spherical Harmonics */

/* Internal use only */
typedef int (* fun1_t) (int, int, double, double *);
typedef int (* fun2_t) (int, double, double *);

static VALUE call3(fun1_t fun, VALUE max, VALUE min, VALUE x) {
  int mmin = NUM2INT(min), mmax = NUM2INT(max);
  int dim, ret;
  double * result;
  VALUE ary;
  if (mmin > mmax)
    rb_raise(rb_eArgError, "Parameter max must be greater than min.");
  dim = mmax - mmin + 1;
  result = (double *)ALLOCA_N(double, dim);
  ret = (* fun)(mmax, mmin, NUM2DBL(x), result);
  if (ret == GSL_EDOM || ret == GSL_EUNDRFLW || ret == GSL_EOVRFLW)
    rb_raise(rb_eArgError, "Operation failed.");
  COPYCARRAY(result, ary, dim);
  return ary;
}

static VALUE call2(fun2_t fun, VALUE max, VALUE x) {
  int mmax = NUM2INT(max);
  int  dim, ret;
  double * result;
  VALUE ary;
  if (mmax < 0)
    rb_raise(rb_eArgError, "Parameter max must be positive.");
  dim = mmax + 1;
  result = (double *)ALLOCA_N(double, dim);
  ret = (* fun)(mmax, NUM2DBL(x), result);
  if (ret == GSL_EDOM || ret == GSL_EUNDRFLW || ret == GSL_EOVRFLW)
    rb_raise(rb_eArgError, "Operation failed.");
  COPYCARRAY(result, ary, dim);
  return ary;
}
/* Legendre Polynomials */

static VALUE Legendre_P1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_legendre_P1(NUM2DBL(x)));
}

static VALUE Legendre_P1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_P1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_P2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_legendre_P2(NUM2DBL(x)));
}

static VALUE Legendre_P2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_P2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_P3(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_legendre_P3(NUM2DBL(x)));
}

static VALUE Legendre_P3_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_P3_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_Pl(VALUE self, VALUE l, VALUE x) {
  return rb_float_new(gsl_sf_legendre_Pl(NUM2INT(l), NUM2DBL(x)));
}

static VALUE Legendre_Pl_e(VALUE self, VALUE l, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_Pl_e(NUM2INT(l), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_Pl_array(VALUE self, VALUE lmax, VALUE x) {
  return call2(gsl_sf_legendre_Pl_array, lmax, x);
}

static VALUE Legendre_Q0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_legendre_Q0(NUM2DBL(x)));
}

static VALUE Legendre_Q0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_Q0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_Q1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_legendre_Q1(NUM2DBL(x)));
}

static VALUE Legendre_Q1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_Q1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_Ql(VALUE self, VALUE l, VALUE x) {
  return rb_float_new(gsl_sf_legendre_Ql(NUM2INT(l), NUM2DBL(x)));
}

static VALUE Legendre_Ql_e(VALUE self, VALUE l, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_Ql_e(NUM2INT(l), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Asscociated Legendre Polynomials and Spherical Harmonics */

static VALUE Legendre_Plm(VALUE self, VALUE l, VALUE m, VALUE x) {
  return rb_float_new(gsl_sf_legendre_Plm(NUM2INT(l), NUM2INT(m), NUM2DBL(x)));
}

static VALUE Legendre_Plm_e(VALUE self, VALUE l, VALUE m, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_Plm_e(NUM2INT(l), NUM2INT(m), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_Plm_array(VALUE self, VALUE lmax, VALUE m, VALUE x) {
  return call3(gsl_sf_legendre_Plm_array, lmax, m, x);
}

static VALUE Legendre_sphPlm(VALUE self, VALUE l, VALUE m, VALUE x) {
  return rb_float_new(gsl_sf_legendre_sphPlm(NUM2INT(l), NUM2INT(m), NUM2DBL(x)));
}

static VALUE Legendre_sphPlm_e(VALUE self, VALUE l, VALUE m, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_sphPlm_e(NUM2INT(l), NUM2INT(m), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Legendre_sphPlm_array(VALUE self, VALUE lmax, VALUE m, VALUE x) {
    return call3(gsl_sf_legendre_sphPlm_array, lmax, m, x);
}


/* Radial Functions for Hyperbolic Space */

static VALUE Legendre_H3d_0(VALUE self, VALUE lambda, VALUE eta) {
  return rb_float_new(gsl_sf_legendre_H3d_0(NUM2DBL(lambda), NUM2DBL(eta)));
}

static VALUE Legendre_H3d_0_e(VALUE self, VALUE lambda, VALUE eta) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_H3d_0_e(NUM2DBL(lambda), NUM2DBL(eta), &r);
  return RESULT(&r);
}

static VALUE Legendre_H3d_1(VALUE self, VALUE lambda, VALUE eta) {
  return rb_float_new(gsl_sf_legendre_H3d_1(NUM2DBL(lambda), NUM2DBL(eta)));
}

static VALUE Legendre_H3d_1_e(VALUE self, VALUE lambda, VALUE eta) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_H3d_1_e(NUM2DBL(lambda), NUM2DBL(eta), &r);
  return RESULT(&r);
}

static VALUE Legendre_H3d(VALUE self, VALUE l, VALUE lambda, VALUE eta) {
  return rb_float_new(gsl_sf_legendre_H3d(NUM2INT(l), NUM2DBL(lambda), 
    NUM2DBL(eta)));
}

static VALUE Legendre_H3d_e(VALUE self, VALUE l, VALUE lambda, VALUE eta) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_legendre_H3d_e(NUM2INT(l), NUM2DBL(lambda), NUM2DBL(eta), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mLegendre;

void Init_Legendre() {
  rbgsl_mLegendre = rb_define_module_under(rbgsl_mSpecial, "Legendre");

  rb_define_module_function(rbgsl_mLegendre, "P1", Legendre_P1, 1);
  rb_define_module_function(rbgsl_mLegendre, "P1_e", Legendre_P1_e, 1);
  rb_define_module_function(rbgsl_mLegendre, "P2", Legendre_P2, 1);
  rb_define_module_function(rbgsl_mLegendre, "P2_e", Legendre_P2_e, 1);
  rb_define_module_function(rbgsl_mLegendre, "P3", Legendre_P3, 1);
  rb_define_module_function(rbgsl_mLegendre, "P3_e", Legendre_P3_e, 1);
  rb_define_module_function(rbgsl_mLegendre, "Pl", Legendre_Pl, 2);
  rb_define_module_function(rbgsl_mLegendre, "Pl_e", Legendre_Pl_e, 2);
  rb_define_module_function(rbgsl_mLegendre, "Pl_array", Legendre_Pl_array, 2);
  rb_define_module_function(rbgsl_mLegendre, "Q0", Legendre_Q0, 1);
  rb_define_module_function(rbgsl_mLegendre, "Q0_e", Legendre_Q0_e, 1);
  rb_define_module_function(rbgsl_mLegendre, "Q1", Legendre_Q1, 1);
  rb_define_module_function(rbgsl_mLegendre, "Q1_e", Legendre_Q1_e, 1);
  rb_define_module_function(rbgsl_mLegendre, "Ql", Legendre_Ql, 2);
  rb_define_module_function(rbgsl_mLegendre, "Ql_e", Legendre_Ql_e, 2);

  rb_define_module_function(rbgsl_mLegendre, "Plm", Legendre_Plm, 3);
  rb_define_module_function(rbgsl_mLegendre, "Plm_e", Legendre_Plm_e, 3);
  rb_define_module_function(rbgsl_mLegendre, "Plm_array", Legendre_Plm_array, 3);
  rb_define_module_function(rbgsl_mLegendre, "sphPlm", Legendre_sphPlm, 3);
  rb_define_module_function(rbgsl_mLegendre, "sphPlm_e", Legendre_sphPlm_e, 3);
  rb_define_module_function(rbgsl_mLegendre, "sphPlm_array", Legendre_sphPlm_array, 3);
  rb_define_module_function(rbgsl_mLegendre, "H3d_0", Legendre_H3d_0, 2);
  rb_define_module_function(rbgsl_mLegendre, "H3d_0_e", Legendre_H3d_0_e, 2);
  rb_define_module_function(rbgsl_mLegendre, "H3d_1", Legendre_H3d_1, 2);
  rb_define_module_function(rbgsl_mLegendre, "H3d_1_e", Legendre_H3d_1_e, 2);
  rb_define_module_function(rbgsl_mLegendre, "H3d", Legendre_H3d, 3);
  rb_define_module_function(rbgsl_mLegendre, "H3d_e", Legendre_H3d_e, 3);
}

/* vim: set ts=4: */
