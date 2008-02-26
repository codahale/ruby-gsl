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

/* $Id: Accel.c,v 1.3 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_interp.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Index Look-up and Acceleration */

static VALUE Accel_init(VALUE self) {
  return self;
}

static void Accel_free(void * p) {
  gsl_interp_accel_free(p);
}

VALUE Accel_new(VALUE class) {
  VALUE accel;
  gsl_interp_accel * ptr;
  ptr = gsl_interp_accel_alloc();
  accel = Data_Wrap_Struct(class, 0, Accel_free, ptr);
  rb_obj_call_init(accel, 0, 0);
  return accel;
}

static VALUE Accel_reset(VALUE self) {
  gsl_interp_accel * ptr;
  Data_Get_Struct(self, gsl_interp_accel, ptr);
  return INT2FIX(gsl_interp_accel_reset(ptr));
}

static VALUE Accel_bsearch(VALUE self, VALUE x_array, VALUE x, VALUE index_lo, 
  VALUE index_hi) {
  double * my_x_array;
  COPYRUBYARRAY(x_array, my_x_array);
  return INT2FIX(gsl_interp_bsearch(my_x_array, NUM2DBL(x), 
    NUM2INT(index_lo), NUM2INT(index_hi)));
}

static VALUE Accel_find(VALUE self, VALUE x_array, VALUE x) {
  long my_size = RARRAY(x_array)->len;
  double * my_x_array;
  gsl_interp_accel * ptr;
  Data_Get_Struct(self, gsl_interp_accel, ptr);
  COPYRUBYARRAY(x_array, my_x_array);
  return INT2FIX(gsl_interp_accel_find(ptr, my_x_array, my_size, NUM2DBL(x)));
}

/* Class definition */

VALUE rbgsl_cAccel;

void Init_Accel() {
  rbgsl_cAccel = rb_define_class_under(rbgsl_mInterp, "Accel", rb_cObject);

  rb_define_singleton_method(rbgsl_cAccel, "new", Accel_new, 0);
  rb_define_method(rbgsl_cAccel, "initialize", Accel_init, 0);
  rb_define_method(rbgsl_cAccel, "reset", Accel_reset, 0);
  rb_define_method(rbgsl_cAccel, "bsearch", Accel_bsearch, 4);
  rb_define_method(rbgsl_cAccel, "find", Accel_find, 2);

}

/* vim: set ts=4: */
