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

/* $Id: Coupling.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_coupling.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Coupling Coefficients */

/* Wigner 3-j Symbols */

static VALUE Coupling_wigner_3j(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_ma, VALUE two_mb, VALUE two_mc) {
  return rb_float_new(gsl_sf_coupling_3j(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_ma), NUM2INT(two_mb), NUM2INT(two_mc)));
}

static VALUE Coupling_wigner_3j_e(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_ma, VALUE two_mb, VALUE two_mc) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_coupling_3j_e(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_ma), NUM2INT(two_mb), 
    NUM2INT(two_mc), &r);
  return RESULT(&r);
}

/* Wigner 6-j Symbols */

static VALUE Coupling_wigner_6j(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_jd, VALUE two_je, VALUE two_jf) {
  return rb_float_new(gsl_sf_coupling_6j(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_jd), NUM2INT(two_je), NUM2INT(two_jf)));
}

static VALUE Coupling_wigner_6j_e(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_jd, VALUE two_je, VALUE two_jf) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_coupling_6j_e(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_jd), NUM2INT(two_je), 
    NUM2INT(two_jf), &r);
  return RESULT(&r);
}

/* Wigner 9-j Symbols */

static VALUE Coupling_wigner_9j(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_jd, VALUE two_je, VALUE two_jf, 
  VALUE two_jg, VALUE two_jh, VALUE two_ji) {
  return rb_float_new(gsl_sf_coupling_9j(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_jd), NUM2INT(two_je), NUM2INT(two_jf),
    NUM2INT(two_jg), NUM2INT(two_jh), NUM2INT(two_ji)));
}

static VALUE Coupling_wigner_9j_e(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_jd, VALUE two_je, VALUE two_jf, 
  VALUE two_jg, VALUE two_jh, VALUE two_ji) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_coupling_9j_e(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_jd), NUM2INT(two_je), NUM2INT(two_jf), 
    NUM2INT(two_jg), NUM2INT(two_jh), NUM2INT(two_ji), &r);
  return RESULT(&r);
}

/* Racah W coefficients */

static VALUE Coupling_racahW(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_jd, VALUE two_je, VALUE two_jf) {
  return rb_float_new(gsl_sf_coupling_RacahW(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_jd), NUM2INT(two_je), NUM2INT(two_jf)));
}

static VALUE Coupling_racahW_e(VALUE self, 
  VALUE two_ja, VALUE two_jb, VALUE two_jc,
  VALUE two_jd, VALUE two_je, VALUE two_jf) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_coupling_RacahW_e(NUM2INT(two_ja), NUM2INT(two_jb),
    NUM2INT(two_jc), NUM2INT(two_jd), NUM2INT(two_je), 
    NUM2INT(two_jf), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mCoupling;

void Init_Coupling() {
  rbgsl_mCoupling = rb_define_module_under(rbgsl_mSpecial, "Coupling");

  rb_define_module_function(rbgsl_mCoupling, "wigner_3j", Coupling_wigner_3j, 6);
  rb_define_module_function(rbgsl_mCoupling, "wigner_3j_e", Coupling_wigner_3j_e, 6);
  rb_define_module_function(rbgsl_mCoupling, "wigner_6j", Coupling_wigner_6j, 6);
  rb_define_module_function(rbgsl_mCoupling, "wigner_6j_e", Coupling_wigner_6j_e, 6);
  rb_define_module_function(rbgsl_mCoupling, "wigner_9j", Coupling_wigner_9j, 9);
  rb_define_module_function(rbgsl_mCoupling, "wigner_9j_e", Coupling_wigner_9j_e, 9);
  rb_define_module_function(rbgsl_mCoupling, "racahW", Coupling_racahW, 6);
  rb_define_module_function(rbgsl_mCoupling, "racahW_e", Coupling_racahW_e, 6);
}

/* vim: set ts=4: */
