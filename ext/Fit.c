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

/*
  rb_gsl_fit.c
  Ruby/GSL: Ruby extension library for GSL (GNU Scientific Library)
    (C) Copyright 2001 by Yoshiki Tsunesada

  Ruby/GSL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.
*/

/* $Id: Fit.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_fit.h"
#include "gsl/gsl_vector.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Least-Squares Fitting */

/* Linear regression */

/* linear fit without weights: y = c0 + c1 x */
/* This returns 6 elements array */
static VALUE Fit_linear(VALUE obj, VALUE xx, VALUE yy) {
  double c0, c1, cov00, cov01, cov11, sumsq;

  if (TYPE(xx) == T_ARRAY) {
    double * x, * y;
    COPYRUBYARRAY(xx, x);
    COPYRUBYARRAY(yy, y);
    gsl_fit_linear(x, 1, y, 1, RARRAY(xx)->len, &c0, &c1, 
      &cov00, &cov01, &cov11, &sumsq);
  } else {
    gsl_vector * x, * y;
    Data_Get_Struct(xx, gsl_vector, x);
    Data_Get_Struct(yy, gsl_vector, y);
    gsl_fit_linear(x->data, 1, y->data, 1, x->size, &c0, &c1, 
      &cov00, &cov01, &cov11, &sumsq);
  }
  return rb_ary_new3(6, rb_float_new(c0), rb_float_new(c1), rb_float_new(cov00),
             rb_float_new(cov01), rb_float_new(cov11), rb_float_new(sumsq));
}

/* linear fit with weights: y = c0 + c1 x */
static VALUE Fit_wlinear(VALUE obj, VALUE xx, VALUE ww, VALUE yy) {
  double c0, c1, cov00, cov01, cov11, sumsq;

  if (TYPE(xx) == T_ARRAY) {
    double * x, * y, * w;
    COPYRUBYARRAY(xx, x);
    COPYRUBYARRAY(yy, y);
    COPYRUBYARRAY(ww, w);
    gsl_fit_wlinear(x, 1, w, 1, y, 1, RARRAY(xx)->len, &c0, &c1, 
      &cov00, &cov01, &cov11, &sumsq);
  } else {
    gsl_vector * x, * y, * w;
    Data_Get_Struct(xx, gsl_vector, x);
    Data_Get_Struct(yy, gsl_vector, y);
    Data_Get_Struct(ww, gsl_vector, w);
    gsl_fit_wlinear(x->data, 1, w->data, 1, y->data, 1, x->size, &c0, &c1, 
      &cov00, &cov01, &cov11, &sumsq);
  }
  return rb_ary_new3(6, rb_float_new(c0), rb_float_new(c1), rb_float_new(cov00),
             rb_float_new(cov01), rb_float_new(cov11), rb_float_new(sumsq));
}

static VALUE Fit_linear_est(VALUE obj, VALUE x, VALUE c0, VALUE c1, 
                   VALUE c00, VALUE c01, VALUE c11) {
  double y, yerr;

  gsl_fit_linear_est(NUM2DBL(x), NUM2DBL(c0), NUM2DBL(c1), NUM2DBL(c00),
             NUM2DBL(c01), NUM2DBL(c11), &y, &yerr);
  return rb_ary_new3(2, rb_float_new(y), rb_float_new(yerr));
}


static VALUE Fit_mul(VALUE obj, VALUE xx, VALUE yy) {
  double c1, cov11, sumsq;

  if (TYPE(xx) == T_ARRAY) {
    double * x, * y;
    COPYRUBYARRAY(xx, x);
    COPYRUBYARRAY(yy, y);
    gsl_fit_mul(x, 1, y, 1, RARRAY(xx)->len, &c1, &cov11, &sumsq);
  } else {
    gsl_vector * x, * y;
    Data_Get_Struct(xx, gsl_vector, x);
    Data_Get_Struct(yy, gsl_vector, y);
    gsl_fit_mul(x->data, 1, y->data, 1, x->size, &c1, &cov11, &sumsq);
  }
  return rb_ary_new3(3, rb_float_new(c1), 
             rb_float_new(cov11), rb_float_new(sumsq));
}

static VALUE Fit_wmul(VALUE obj, VALUE xx, VALUE ww, VALUE yy) {
  double c1, cov11, sumsq;

  if (TYPE(xx) == T_ARRAY) {
    double * x, * y, * w;
    COPYRUBYARRAY(xx, x);
    COPYRUBYARRAY(yy, y);
    COPYRUBYARRAY(ww, w);
    gsl_fit_wmul(x, 1, w, 1, y, 1, RARRAY(xx)->len, &c1, &cov11, &sumsq);
  } else {
    gsl_vector * x, * y, * w;
    Data_Get_Struct(xx, gsl_vector, x);
    Data_Get_Struct(yy, gsl_vector, y);
    Data_Get_Struct(ww, gsl_vector, w);
    gsl_fit_wmul(x->data, 1, w->data, 1, y->data, 1, x->size, &c1, 
      &cov11, &sumsq);
  }
  return rb_ary_new3(3, rb_float_new(c1), rb_float_new(cov11), 
    rb_float_new(sumsq));
}

static VALUE Fit_mul_est(VALUE obj, VALUE x, VALUE c1, VALUE c11) {
  double y, yerr;

  gsl_fit_mul_est(NUM2DBL(x), NUM2DBL(c1), NUM2DBL(c11), &y, &yerr);
  return rb_ary_new3(2, rb_float_new(y), rb_float_new(yerr));
}

/* Module definition */

VALUE rbgsl_mFit;

void Init_Fit() {
  rbgsl_mFit = rb_define_module_under(rbgsl_mGSL, "Fit");

  rb_define_module_function(rbgsl_mFit, "linear", Fit_linear, 2);
  rb_define_module_function(rbgsl_mFit, "wlinear", Fit_wlinear, 3);
  rb_define_module_function(rbgsl_mFit, "linear_est", Fit_linear_est, 6);
  rb_define_module_function(rbgsl_mFit, "mul", Fit_mul, 2);
  rb_define_module_function(rbgsl_mFit, "wmul", Fit_wmul, 3);
  rb_define_module_function(rbgsl_mFit, "mul_est", Fit_mul_est, 6);
}

/* vim: set ts=4: */
