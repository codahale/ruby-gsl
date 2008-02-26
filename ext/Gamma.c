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

/* $Id: Gamma.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_gamma.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Gamma Functions */

static VALUE Gamma_lngamma(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_lngamma(NUM2DBL(x)));
}

static VALUE Gamma_lngamma_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lngamma_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_gamma(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_gamma(NUM2DBL(x)));
}

static VALUE Gamma_gamma_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gamma_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_gammastar(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_gammastar(NUM2DBL(x)));
}

static VALUE Gamma_gammastar_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gammastar_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_gammainv(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_gammainv(NUM2DBL(x)));
}

static VALUE Gamma_gammainv_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gammainv_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Taylor Coefficient */

static VALUE Gamma_taylorcoeff(VALUE self, VALUE n, VALUE x) {
  return rb_float_new(gsl_sf_taylorcoeff(NUM2INT(n), NUM2DBL(x)));
}

static VALUE Gamma_taylorcoeff_e(VALUE self, VALUE n, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_taylorcoeff_e(NUM2INT(n), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Factorials */

static VALUE Gamma_fact(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_fact(NUM2UINT(n)));
}

static VALUE Gamma_fact_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_fact_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_doublefact(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_doublefact(NUM2UINT(n)));
}

static VALUE Gamma_doublefact_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_doublefact_e(NUM2UINT(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_lnfact(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_lnfact(NUM2UINT( n)));
}

static VALUE Gamma_lnfact_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lnfact_e(NUM2UINT(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_lndoublefact(VALUE self, VALUE n) {
  return rb_float_new(gsl_sf_lndoublefact(NUM2UINT( n)));
}

static VALUE Gamma_lndoublefact_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lndoublefact_e(NUM2UINT(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_lnchoose(VALUE self, VALUE n, VALUE m) {
  return rb_float_new(gsl_sf_lnchoose(NUM2UINT(n), NUM2UINT(m)));
}

static VALUE Gamma_lnchoose_e(VALUE self, VALUE n, VALUE m) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lnchoose_e(NUM2UINT(n), NUM2UINT(m), &r);
  return RESULT(&r);
}

static VALUE Gamma_choose(VALUE self, VALUE n, VALUE m) {
  return rb_float_new(gsl_sf_choose(NUM2UINT(n), NUM2UINT(m)));
}

static VALUE Gamma_choose_e(VALUE self, VALUE n, VALUE m) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_choose_e(NUM2UINT(n), NUM2UINT(m), &r);
  return RESULT(&r);
}

/* Pochhammer Symbol */

static VALUE Gamma_lnpoch(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_lnpoch(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Gamma_lnpoch_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lnpoch_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_lnpoch_sgn_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  double sgn;
  gsl_sf_result r;
  ret = gsl_sf_lnpoch_sgn_e(NUM2DBL(a), NUM2DBL(x), &r, &sgn);
  return rb_ary_new3(2, RESULT(&r), rb_float_new(sgn));
}

static VALUE Gamma_poch(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_poch(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Gamma_poch_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_poch_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_pochrel(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_pochrel(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Gamma_pochrel_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_pochrel_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Incomplete Gamma Function */

static VALUE Gamma_gamma_inc_Q(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_gamma_inc_Q(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Gamma_gamma_inc_Q_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gamma_inc_Q_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_gamma_inc_P(VALUE self, VALUE a, VALUE x) {
  return rb_float_new(gsl_sf_gamma_inc_P(NUM2DBL(a), NUM2DBL(x)));
}

static VALUE Gamma_gamma_inc_P_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_gamma_inc_P_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Beta Function */

static VALUE Gamma_beta(VALUE self, VALUE a, VALUE b) {
  return rb_float_new(gsl_sf_beta(NUM2DBL(a), NUM2DBL(b)));
}

static VALUE Gamma_beta_e(VALUE self, VALUE a, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_beta_e(NUM2DBL(a), NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Gamma_lnbeta(VALUE self, VALUE a, VALUE b) {
  return rb_float_new(gsl_sf_lnbeta(NUM2DBL(a), NUM2DBL(b)));
}

static VALUE Gamma_lnbeta_e(VALUE self, VALUE a, VALUE b) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_lnbeta_e(NUM2DBL(a), NUM2DBL(b), &r);
  return RESULT(&r);
}

static VALUE Gamma_beta_inc(VALUE self, VALUE a, VALUE b, VALUE x) {
  return rb_float_new(gsl_sf_beta_inc(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x)));
}

static VALUE Gamma_beta_inc_e(VALUE self, VALUE a, VALUE b, VALUE x, 
  VALUE result) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_beta_inc_e(NUM2DBL(a), NUM2DBL(b), NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mGamma;

void Init_Gamma() {
  rbgsl_mGamma = rb_define_module_under(rbgsl_mSpecial, "Gamma");

  rb_define_module_function(rbgsl_mGamma, "gamma", Gamma_gamma, 1);
  rb_define_module_function(rbgsl_mGamma, "gamma_e", Gamma_gamma_e, 1);
  rb_define_module_function(rbgsl_mGamma, "lngamma", Gamma_lngamma, 1);
  rb_define_module_function(rbgsl_mGamma, "lngamma_e", Gamma_lngamma_e, 1);
  rb_define_module_function(rbgsl_mGamma, "gammastar", Gamma_gammastar, 1);
  rb_define_module_function(rbgsl_mGamma, "gammastar_e", Gamma_gammastar_e, 1);
  rb_define_module_function(rbgsl_mGamma, "gammainv", Gamma_gammainv, 1);
  rb_define_module_function(rbgsl_mGamma, "gammainv_e", Gamma_gammainv_e, 1);

  rb_define_module_function(rbgsl_mGamma, "taylorcoeff", Gamma_taylorcoeff, 2);
  rb_define_module_function(rbgsl_mGamma, "taylorcoeff_e", Gamma_taylorcoeff_e, 2);

  rb_define_module_function(rbgsl_mGamma, "fact", Gamma_fact, 1);
  rb_define_module_function(rbgsl_mGamma, "fact_e", Gamma_fact_e, 1);
  rb_define_module_function(rbgsl_mGamma, "doublefact", Gamma_doublefact, 1);
  rb_define_module_function(rbgsl_mGamma, "doublefact_e", Gamma_doublefact_e, 1);
  rb_define_module_function(rbgsl_mGamma, "lnfact", Gamma_lnfact, 1);
  rb_define_module_function(rbgsl_mGamma, "lnfact_e", Gamma_lnfact_e, 1);
  rb_define_module_function(rbgsl_mGamma, "lndoublefact", Gamma_lndoublefact, 1);
  rb_define_module_function(rbgsl_mGamma, "lndoublefact_e", Gamma_lndoublefact_e, 1);
  rb_define_module_function(rbgsl_mGamma, "lnchoose", Gamma_lnchoose, 2);
  rb_define_module_function(rbgsl_mGamma, "lnchoose_e", Gamma_lnchoose_e, 2);
  rb_define_module_function(rbgsl_mGamma, "choose", Gamma_choose, 2);
  rb_define_module_function(rbgsl_mGamma, "choose_e", Gamma_choose_e, 2);

  rb_define_module_function(rbgsl_mGamma, "lnpoch", Gamma_lnpoch, 2);
  rb_define_module_function(rbgsl_mGamma, "lnpoch_e", Gamma_lnpoch_e, 2);
  rb_define_module_function(rbgsl_mGamma, "lnpoch_sgn_e", Gamma_lnpoch_sgn_e, 2);
  rb_define_module_function(rbgsl_mGamma, "poch", Gamma_poch, 2);
  rb_define_module_function(rbgsl_mGamma, "poch_e", Gamma_poch_e, 2);
  rb_define_module_function(rbgsl_mGamma, "pochrel", Gamma_pochrel, 2);
  rb_define_module_function(rbgsl_mGamma, "pochrel_e", Gamma_pochrel_e, 2);

  rb_define_module_function(rbgsl_mGamma, "gamma_inc_Q", Gamma_gamma_inc_Q, 2);
  rb_define_module_function(rbgsl_mGamma, "gamma_inc_Q_e", Gamma_gamma_inc_Q_e, 2);
  rb_define_module_function(rbgsl_mGamma, "gamma_inc_P", Gamma_gamma_inc_P, 2);
  rb_define_module_function(rbgsl_mGamma, "gamma_inc_P_e", Gamma_gamma_inc_P_e, 2);
  rb_define_module_function(rbgsl_mGamma, "lnbeta", Gamma_lnbeta, 2);
  rb_define_module_function(rbgsl_mGamma, "lnbeta_e", Gamma_lnbeta_e, 2);

  rb_define_module_function(rbgsl_mGamma, "beta", Gamma_beta, 2);
  rb_define_module_function(rbgsl_mGamma, "beta_e", Gamma_beta_e, 2);
  rb_define_module_function(rbgsl_mGamma, "beta_inc", Gamma_beta_inc, 3);
  rb_define_module_function(rbgsl_mGamma, "beta_inc_e", Gamma_beta_inc_e, 3);
}

/* vim: set ts=4: */
