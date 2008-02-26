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


/* $Id: Bessel.c,v 1.3 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_sf_bessel.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Bessel Functions */

/* Internal use only */
typedef int (* fun1_t) (int, int, double, double *);
typedef int (* fun2_t) (int, double, double *);

static VALUE call1(fun1_t fun, VALUE min, VALUE max, VALUE x) {
  int mmin = NUM2INT(min), mmax = NUM2INT(max);
  int dim, ret;
  double * result;
  VALUE ary;
  if (mmin > mmax)
    rb_raise(rb_eArgError, "Parameter max must be greater than min.");
  dim = mmax - mmin + 1;
  result = (double *)ALLOCA_N(double, dim);
  ret = (* fun)(mmin, mmax, NUM2DBL(x), result);
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

/* Regular Cylindrical Bessel Functions */

static VALUE Bessel_J0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_J0(NUM2DBL(x)));
}

static VALUE Bessel_J0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_J0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_J1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_J1(NUM2DBL(x)));
}

static VALUE Bessel_J1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_J1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Jn(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Jn(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Bessel_Jn_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Jn_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Jn_array(VALUE self, VALUE min, VALUE max, VALUE x) {
  return call1(gsl_sf_bessel_Jn_array, min, max, x);
}

/* Irregular Cylindrical Bessel Functions */

static VALUE Bessel_Y0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Y0(NUM2DBL(x)));
}

static VALUE Bessel_Y0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Y0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Y1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Y1(NUM2DBL(x)));
}

static VALUE Bessel_Y1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Y1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Yn(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Yn(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Bessel_Yn_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Yn_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Yn_array(VALUE self, VALUE min, VALUE max, VALUE x) {
  return call1(gsl_sf_bessel_Yn_array, min, max, x);
}

/* Regular Modified Cylindrical Bessel Functions */

static VALUE Bessel_I0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_I0(NUM2DBL(x)));
}

static VALUE Bessel_I0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_I0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_I1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_I1(NUM2DBL(x)));
}

static VALUE Bessel_I1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_I1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_In(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_bessel_In(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Bessel_In_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_In_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_In_array(VALUE self, VALUE min, VALUE max, VALUE x) {
  return call1(gsl_sf_bessel_In_array, min, max, x);
}

static VALUE Bessel_I0_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_I0_scaled(NUM2DBL(x)));
}

static VALUE Bessel_I0_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_I0_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_I1_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_I1_scaled(NUM2DBL(x)));
}

static VALUE Bessel_I1_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_I1_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_In_scaled(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_bessel_In_scaled(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Bessel_In_scaled_array(VALUE self, VALUE min, VALUE max, VALUE x) {
  return call1(gsl_sf_bessel_In_scaled_array, min, max, x);
}

static VALUE Bessel_In_scaled_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_In_scaled_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Irregular Modified Cylindrical Bessel Functions */

static VALUE Bessel_K0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_K0(NUM2DBL(x)));
}

static VALUE Bessel_K0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_K0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_K1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_K1(NUM2DBL(x)));
}

static VALUE Bessel_K1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_K1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Kn(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Kn(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Bessel_Kn_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Kn_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Kn_array(VALUE self, VALUE min, VALUE max, VALUE x) {
  return call1(gsl_sf_bessel_Kn_array, min, max, x);
}

static VALUE Bessel_K0_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_K0_scaled(NUM2DBL(x)));
}

static VALUE Bessel_K0_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_K0_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_K1_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_K1_scaled(NUM2DBL(x)));
}

static VALUE Bessel_K1_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_K1_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Kn_scaled(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Kn_scaled(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Bessel_Kn_scaled_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Kn_scaled_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Kn_scaled_array(VALUE self, VALUE min, VALUE max, VALUE x) {
  return call1(gsl_sf_bessel_Kn_scaled_array, min, max, x);
}

/* Regular Spherical Bessel Functions */

static VALUE Bessel_j0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_j0(NUM2DBL(x)));
}

static VALUE Bessel_j0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_j0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_j1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_j1(NUM2DBL(x)));
}

static VALUE Bessel_j1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_j1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_j2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_j2(NUM2DBL(x)));
}

static VALUE Bessel_j2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_j2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_jl(VALUE self, VALUE l, VALUE x) {
  return rb_float_new(gsl_sf_bessel_jl(NUM2INT(l), NUM2DBL(x)));
}

static VALUE Bessel_jl_e(VALUE self, VALUE l, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_jl_e(NUM2INT(l), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_jl_array(VALUE self, VALUE max, VALUE x) {
  return call2(gsl_sf_bessel_jl_array, max, x);
}

static VALUE Bessel_jl_steed_array(VALUE self, VALUE max, VALUE x) {
  return call2(gsl_sf_bessel_jl_steed_array, max, x);
}

/* Irregular Spherical Bessel Functions */

static VALUE Bessel_y0(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_y0(NUM2DBL(x)));
}

static VALUE Bessel_y0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_y0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_y1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_y1(NUM2DBL(x)));
}

static VALUE Bessel_y1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_y1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_y2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_y2(NUM2DBL(x)));
}

static VALUE Bessel_y2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_y2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_yl(VALUE self, VALUE l, VALUE x) {
  return rb_float_new(gsl_sf_bessel_yl(NUM2INT(l), NUM2DBL(x)));
}

static VALUE Bessel_yl_e(VALUE self, VALUE l, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_yl_e(NUM2INT(l), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_yl_array(VALUE self, VALUE max, VALUE x) {
  return call2(gsl_sf_bessel_yl_array, max, x);
}

/* Regular Modified Spherical Bessel Functions */

static VALUE Bessel_k0_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_k0_scaled(NUM2DBL(x)));
}

static VALUE Bessel_k0_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_k0_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_k1_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_k1_scaled(NUM2DBL(x)));
}

static VALUE Bessel_k1_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_k1_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_k2_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_k2_scaled(NUM2DBL(x)));
}

static VALUE Bessel_k2_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_k2_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_kl_scaled(VALUE self, VALUE l, VALUE x) {
  return rb_float_new(gsl_sf_bessel_kl_scaled(NUM2INT(l), NUM2DBL(x)));
}

static VALUE Bessel_kl_scaled_e(VALUE self, VALUE l, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_kl_scaled_e(NUM2INT(l), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_kl_scaled_array(VALUE self, VALUE max, VALUE x) {
  return call2(gsl_sf_bessel_kl_scaled_array, max, x);
}

/* Irregular Modified Spherical Bessel Functions */

static VALUE Bessel_i0_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_i0_scaled(NUM2DBL(x)));
}

static VALUE Bessel_i0_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_i0_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_i1_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_i1_scaled(NUM2DBL(x)));
}

static VALUE Bessel_i1_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_i1_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_i2_scaled(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_bessel_i2_scaled(NUM2DBL(x)));
}

static VALUE Bessel_i2_scaled_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_i2_scaled_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_il_scaled(VALUE self, VALUE l, VALUE x) {
  return rb_float_new(gsl_sf_bessel_il_scaled(NUM2INT(l), NUM2DBL(x)));
}

static VALUE Bessel_il_scaled_e(VALUE self, VALUE l, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_il_scaled_e(NUM2INT(l), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_il_scaled_array(VALUE self, VALUE max, VALUE x) {
  return call2(gsl_sf_bessel_il_scaled_array, max, x);
}

/* Regular Bessel Function - Fractional Order */

static VALUE Bessel_Jnu(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Jnu(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_Jnu_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Jnu_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Irregular Bessel Function - Fractional Order */

static VALUE Bessel_Ynu(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Ynu(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_Ynu_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Ynu_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Regular Modified Bessel Function - Fractional Order */

static VALUE Bessel_Inu(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Inu(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_Inu_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Inu_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Inu_scaled(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Inu_scaled(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_Inu_scaled_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Inu_scaled_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Irregular Modified Bessel Function - Fractional Order */

static VALUE Bessel_Knu(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Knu(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_Knu_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Knu_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_Knu_scaled(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_Knu_scaled(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_Knu_scaled_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_Knu_scaled_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_lnKnu(VALUE self, VALUE nu, VALUE x) {
  return rb_float_new(gsl_sf_bessel_lnKnu(NUM2DBL(nu), NUM2DBL(x)));
}

static VALUE Bessel_lnKnu_e(VALUE self, VALUE nu, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_lnKnu_e(NUM2DBL(nu), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Zeroes of Regular Bessel Functions */

static VALUE Bessel_zero_J0(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_bessel_zero_J0(NUM2UINT(s)));
}

static VALUE Bessel_zero_J0_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_zero_J0_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_zero_J1(VALUE self, VALUE s) {
  return rb_float_new(gsl_sf_bessel_zero_J1(NUM2UINT(s)));
}

static VALUE Bessel_zero_J1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_zero_J1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Bessel_zero_Jnu(VALUE self, VALUE nu, VALUE s) {
  return rb_float_new(gsl_sf_bessel_zero_Jnu(NUM2DBL(nu), NUM2UINT(s)));
}

static VALUE Bessel_zero_Jnu_e(VALUE self, VALUE nu, VALUE s) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_bessel_zero_Jnu_e(NUM2DBL(nu), NUM2UINT(s), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mBessel;

void Init_Bessel() {
  rbgsl_mBessel = rb_define_module_under(rbgsl_mSpecial, "Bessel");

  rb_define_module_function(rbgsl_mBessel, "J0", Bessel_J0, 1);
  rb_define_module_function(rbgsl_mBessel, "J0_e", Bessel_J0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "J1", Bessel_J1, 1);
  rb_define_module_function(rbgsl_mBessel, "J1_e", Bessel_J1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "Jn", Bessel_Jn, 2);
  rb_define_module_function(rbgsl_mBessel, "Jn_e", Bessel_Jn_e, 2);
  rb_define_module_function(rbgsl_mBessel, "Jn_array", Bessel_Jn_array, 3);

  rb_define_module_function(rbgsl_mBessel, "Y0", Bessel_Y0, 1);
  rb_define_module_function(rbgsl_mBessel, "Y0_e", Bessel_Y0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "Y1", Bessel_Y1, 1);
  rb_define_module_function(rbgsl_mBessel, "Y1_e", Bessel_Y1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "Yn", Bessel_Yn, 2);
  rb_define_module_function(rbgsl_mBessel, "Yn_e", Bessel_Yn_e, 2);
  rb_define_module_function(rbgsl_mBessel, "Yn_array", Bessel_Yn_array, 3);

  rb_define_module_function(rbgsl_mBessel, "I0", Bessel_I0, 1);
  rb_define_module_function(rbgsl_mBessel, "I0_e", Bessel_I0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "I1", Bessel_I1, 1);
  rb_define_module_function(rbgsl_mBessel, "I1_e", Bessel_I1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "In", Bessel_In, 2);
  rb_define_module_function(rbgsl_mBessel, "In_e", Bessel_In_e, 2);
  rb_define_module_function(rbgsl_mBessel, "In_array", Bessel_In_array, 3);
  rb_define_module_function(rbgsl_mBessel, "I0_scaled", Bessel_I0_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "I0_scaled_e", Bessel_I0_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "I1_scaled", Bessel_I1_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "I1_scaled_e", Bessel_I1_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "In_scaled", Bessel_In_scaled, 2);
  rb_define_module_function(rbgsl_mBessel, "In_scaled_e", Bessel_In_scaled_e, 2);
  rb_define_module_function(rbgsl_mBessel, "In_scaled_array", Bessel_In_scaled_array, 3);

  rb_define_module_function(rbgsl_mBessel, "K0", Bessel_K0, 1);
  rb_define_module_function(rbgsl_mBessel, "K0_e", Bessel_K0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "K1", Bessel_K1, 1);
  rb_define_module_function(rbgsl_mBessel, "K1_e", Bessel_K1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "Kn", Bessel_Kn, 2);
  rb_define_module_function(rbgsl_mBessel, "Kn_e", Bessel_Kn_e, 2);
  rb_define_module_function(rbgsl_mBessel, "Kn_array", Bessel_Kn_array, 3);
  rb_define_module_function(rbgsl_mBessel, "K0_scaled", Bessel_K0_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "K0_scaled_e", Bessel_K0_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "K1_scaled", Bessel_K1_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "K1_scaled_e", Bessel_K1_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "Kn_scaled", Bessel_Kn_scaled, 2);
  rb_define_module_function(rbgsl_mBessel, "Kn_scaled_e", Bessel_Kn_scaled_e, 2);
  rb_define_module_function(rbgsl_mBessel, "Kn_scaled_array", Bessel_Kn_scaled_array, 3);

  rb_define_module_function(rbgsl_mBessel, "j0", Bessel_j0, 1);
  rb_define_module_function(rbgsl_mBessel, "j0_e", Bessel_j0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "j1", Bessel_j1, 1);
  rb_define_module_function(rbgsl_mBessel, "j1_e", Bessel_j1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "j2", Bessel_j2, 1);
  rb_define_module_function(rbgsl_mBessel, "j2_e", Bessel_j2_e, 1);
  rb_define_module_function(rbgsl_mBessel, "jl", Bessel_jl, 2);
  rb_define_module_function(rbgsl_mBessel, "jl_e", Bessel_jl_e, 2);
  rb_define_module_function(rbgsl_mBessel, "jl_array", Bessel_jl_array, 2);
  rb_define_module_function(rbgsl_mBessel, "jl_steed_array", Bessel_jl_steed_array, 2);

  rb_define_module_function(rbgsl_mBessel, "y0", Bessel_y0, 1);
  rb_define_module_function(rbgsl_mBessel, "y0_e", Bessel_y0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "y1", Bessel_y1, 1);
  rb_define_module_function(rbgsl_mBessel, "y1_e", Bessel_y1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "y2", Bessel_y2, 1);
  rb_define_module_function(rbgsl_mBessel, "y2_e", Bessel_y2_e, 1);
  rb_define_module_function(rbgsl_mBessel, "yl", Bessel_yl, 2);
  rb_define_module_function(rbgsl_mBessel, "yl_e", Bessel_yl_e, 2);
  rb_define_module_function(rbgsl_mBessel, "yl_array", Bessel_yl_array, 2);

  rb_define_module_function(rbgsl_mBessel, "i0_scaled", Bessel_i0_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "i0_scaled_e", Bessel_i0_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "i1_scaled", Bessel_i1_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "i1_scaled_e", Bessel_i1_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "i2_scaled", Bessel_i2_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "i2_scaled_e", Bessel_i2_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "il_scaled", Bessel_il_scaled, 2);
  rb_define_module_function(rbgsl_mBessel, "il_scaled_e", Bessel_il_scaled_e, 2);
  rb_define_module_function(rbgsl_mBessel, "il_scaled_array", Bessel_il_scaled_array, 2);

  rb_define_module_function(rbgsl_mBessel, "k0_scaled", Bessel_k0_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "k0_scaled_e", Bessel_k0_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "k1_scaled", Bessel_k1_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "k1_scaled_e", Bessel_k1_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "k2_scaled", Bessel_k2_scaled, 1);
  rb_define_module_function(rbgsl_mBessel, "k2_scaled_e", Bessel_k2_scaled_e, 1);
  rb_define_module_function(rbgsl_mBessel, "kl_scaled", Bessel_kl_scaled, 2);
  rb_define_module_function(rbgsl_mBessel, "kl_scaled_e", Bessel_kl_scaled_e, 2);
  rb_define_module_function(rbgsl_mBessel, "kl_scaled_array", Bessel_kl_scaled_array, 2);

  rb_define_module_function(rbgsl_mBessel, "Jnu", Bessel_Jnu, 2);
  rb_define_module_function(rbgsl_mBessel, "Jnu_e", Bessel_Jnu_e, 2);

  rb_define_module_function(rbgsl_mBessel, "Ynu", Bessel_Ynu, 2);
  rb_define_module_function(rbgsl_mBessel, "Ynu_e", Bessel_Ynu_e, 2);

  rb_define_module_function(rbgsl_mBessel, "Inu", Bessel_Inu, 2);
  rb_define_module_function(rbgsl_mBessel, "Inu_e", Bessel_Inu_e, 2);
  rb_define_module_function(rbgsl_mBessel, "Inu_scaled", Bessel_Inu_scaled, 2);
  rb_define_module_function(rbgsl_mBessel, "Inu_scaled_e", Bessel_Inu_scaled_e, 2);

  rb_define_module_function(rbgsl_mBessel, "Knu", Bessel_Knu, 2);
  rb_define_module_function(rbgsl_mBessel, "Knu_e", Bessel_Knu_e, 2);
  rb_define_module_function(rbgsl_mBessel, "Knu_scaled", Bessel_Knu_scaled, 2);
  rb_define_module_function(rbgsl_mBessel, "Knu_scaled_e", Bessel_Knu_scaled_e, 2);
  rb_define_module_function(rbgsl_mBessel, "lnKnu", Bessel_lnKnu, 2);
  rb_define_module_function(rbgsl_mBessel, "lnKnu_e", Bessel_lnKnu_e, 2);

  rb_define_module_function(rbgsl_mBessel, "zero_J0", Bessel_zero_J0, 1);
  rb_define_module_function(rbgsl_mBessel, "zero_J0_e", Bessel_zero_J0_e, 1);
  rb_define_module_function(rbgsl_mBessel, "zero_J1", Bessel_zero_J1, 1);
  rb_define_module_function(rbgsl_mBessel, "zero_J1_e", Bessel_zero_J1_e, 1);
  rb_define_module_function(rbgsl_mBessel, "zero_Jnu", Bessel_zero_Jnu, 2);
  rb_define_module_function(rbgsl_mBessel, "zero_Jnu_e", Bessel_zero_Jnu_e, 2);

}

/* vim: set ts=4: */
