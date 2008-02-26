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

/* $Id: Synchrotron.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_synchrotron.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Synchrotron Functions */

static VALUE Synchrotron_S1(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_synchrotron_1(NUM2DBL(x)));
}

static VALUE Synchrotron_S1_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_synchrotron_1_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Synchrotron_S2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_synchrotron_2(NUM2DBL(x)));
}

static VALUE Synchrotron_S2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_synchrotron_2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mSynchrotron;

void Init_Synchrotron() {
  rbgsl_mSynchrotron = rb_define_module_under(rbgsl_mSpecial, "Synchrotron");

  rb_define_module_function(rbgsl_mSynchrotron, "S1", Synchrotron_S1, 1);
  rb_define_module_function(rbgsl_mSynchrotron, "S1_e", Synchrotron_S1_e, 1);
  rb_define_module_function(rbgsl_mSynchrotron, "S2", Synchrotron_S2, 1);
  rb_define_module_function(rbgsl_mSynchrotron, "S2_e", Synchrotron_S2_e, 1);
}

/* vim: set ts=4: */
