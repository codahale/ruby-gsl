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

/* $Id: Transport.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_transport.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Transport Functions */

static VALUE Transport_J2(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_transport_2(NUM2DBL(x)));
}

static VALUE Transport_J2_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_transport_2_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Transport_J3(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_transport_3(NUM2DBL(x)));
}

static VALUE Transport_J3_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_transport_3_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Transport_J4(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_transport_4(NUM2DBL(x)));
}

static VALUE Transport_J4_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_transport_4_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

static VALUE Transport_J5(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_transport_5(NUM2DBL(x)));
}

static VALUE Transport_J5_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_transport_5_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mTransport;

void Init_Transport() {
  rbgsl_mTransport = rb_define_module_under(rbgsl_mSpecial, "Transport");

  rb_define_module_function(rbgsl_mTransport, "J2", Transport_J2, 1);
  rb_define_module_function(rbgsl_mTransport, "J2_e", Transport_J2_e, 1);
  rb_define_module_function(rbgsl_mTransport, "J3", Transport_J3, 1);
  rb_define_module_function(rbgsl_mTransport, "J3_e", Transport_J3_e, 1);
  rb_define_module_function(rbgsl_mTransport, "J4", Transport_J4, 1);
  rb_define_module_function(rbgsl_mTransport, "J4_e", Transport_J4_e, 1);
  rb_define_module_function(rbgsl_mTransport, "J5", Transport_J5, 1);
  rb_define_module_function(rbgsl_mTransport, "J5_e", Transport_J5_e, 1);
}

/* vim: set ts=4: */
