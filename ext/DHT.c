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

/* $Id: DHT.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_dht.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Discrete Hankel Transforms */

static VALUE DHT_initialize(VALUE self, VALUE nu, VALUE xmax) {
  return self;
}

static void DHT_free(void * p) {
  gsl_dht_free(p);
}

VALUE DHT_new(VALUE class, VALUE size, VALUE nu, VALUE xmax) {
  gsl_dht * ptr;
  VALUE dht;
  ptr = gsl_dht_new(NUM2INT(size), NUM2DBL(nu), NUM2DBL(xmax));
  dht = Data_Wrap_Struct(class, 0, DHT_free, ptr);
  return dht;
}

static VALUE DHT_k_sample(VALUE self, VALUE n) {
  gsl_dht * ptr;
  Data_Get_Struct(self, gsl_dht, ptr);
  return rb_float_new(gsl_dht_k_sample(ptr, NUM2INT(n)));
}

static VALUE DHT_x_sample(VALUE self, VALUE n) {
  gsl_dht * ptr;
  Data_Get_Struct(self, gsl_dht, ptr);
  return rb_float_new(gsl_dht_x_sample(ptr, NUM2INT(n)));
}

static VALUE DHT_apply(VALUE self, VALUE f_in, VALUE f_out) {
  double * in, * out;
  gsl_dht * ptr;
  
  Data_Get_Struct(self, gsl_dht, ptr);
  if (RARRAY(f_in)->len != ptr->size)
    rb_raise(rb_eArgError, "Array must be of same size as transform");
  if (RARRAY(f_in)->len != RARRAY(f_out)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size");
  COPYRUBYARRAY(f_in, in);
  COPYRUBYARRAY(f_out, out);
  return INT2FIX(gsl_dht_apply(ptr, in, out));
}

/* Class definition */

VALUE rbgsl_cDHT;

void Init_DHT() {
  rbgsl_cDHT = rb_define_class_under(rbgsl_mGSL, "DHT", rb_cObject);

  rb_define_singleton_method(rbgsl_cDHT, "new", DHT_new, 3);
  rb_define_method(rbgsl_cDHT, "initialize", DHT_initialize, 3);

  rb_define_method(rbgsl_cDHT, "k_sample", DHT_k_sample, 1);
  rb_define_method(rbgsl_cDHT, "x_sample", DHT_x_sample, 1);
  rb_define_method(rbgsl_cDHT, "apply", DHT_apply, 2);
}

/* vim: set ts=4: */
