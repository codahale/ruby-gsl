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

/* $Id: QRNG.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_qrng.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* constants for QRNG */
#define QRNG_NIEDERREITER_2 0
#define QRNG_SOBOL 1

/* Quasi-Random Sequences */

VALUE rbgsl_cQRNG;

/* QRNG Initialization */

static VALUE QRNG_init(VALUE self, VALUE type, VALUE dim) {
  return self;
}

static void QRNG_free(void *p) {
  gsl_qrng_free(p);
}

VALUE QRNG_new(VALUE class, VALUE type, VALUE dim) {
  VALUE argv[2];
  VALUE qrng;
  gsl_qrng * ptr;
  switch (NUM2INT(type)) {
    case QRNG_NIEDERREITER_2:
      ptr = gsl_qrng_alloc(gsl_qrng_niederreiter_2, NUM2UINT(dim));
      break;
    case QRNG_SOBOL:
      ptr = gsl_qrng_alloc(gsl_qrng_sobol, NUM2UINT(dim));
      break;
    default:
      rb_raise(rb_eArgError, "Illegal QRNG type!");
  }
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  qrng = Data_Wrap_Struct(class, 0, QRNG_free, ptr);
  argv[0] = type;
  argv[1] = dim;
  rb_obj_call_init(qrng, 2, argv);
  return qrng;
}

static VALUE QRNG_reset(VALUE self) {
  gsl_qrng * ptr;
  Data_Get_Struct(self, gsl_qrng, ptr);
  gsl_qrng_init(ptr);
  return self;
}

/* Sampling from a QRNG */

static VALUE QRNG_get(VALUE self) {
  gsl_qrng * ptr;
  double * x;
  VALUE ary;
  int ret, dim;
  Data_Get_Struct(self, gsl_qrng, ptr);
  dim = ptr->dimension;
  x = (double *)ALLOCA_N(double, dim);
  ret = gsl_qrng_get(ptr, x);
  if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  COPYCARRAY(x, ary, dim);
  return ary;
}

/* Auxiliary Functions for QRNG */

static VALUE QRNG_name(VALUE self) {
  gsl_qrng * ptr;
  Data_Get_Struct(self, gsl_qrng, ptr);
  return rb_str_new2(gsl_qrng_name(ptr));
}

static VALUE QRNG_dim(VALUE self) {
  gsl_qrng * ptr;
  Data_Get_Struct(self, gsl_qrng, ptr);
  return INT2NUM(ptr->dimension);
}

static VALUE QRNG_size(VALUE self) {
  gsl_qrng * ptr;
  Data_Get_Struct(self, gsl_qrng, ptr);
  return INT2NUM(gsl_qrng_size(ptr));
}

/* TODO: qsl_qrng_state */

/* Saving and Restoring QRNG state */

static VALUE QRNG_clone(VALUE self) {
  VALUE clone;
  gsl_qrng * ptr, * cptr;
  Data_Get_Struct(self, gsl_qrng, ptr);
  cptr = gsl_qrng_clone(ptr);
  clone = Data_Wrap_Struct(rbgsl_cQRNG, 0, QRNG_free, cptr);
  return clone;
}

static VALUE QRNG_memcpy(VALUE self, VALUE dest, VALUE src) {
  int ret;
  gsl_qrng * pdest, * psrc;
  Data_Get_Struct(dest, gsl_qrng, pdest);
  Data_Get_Struct(src, gsl_qrng, psrc);
  ret = gsl_qrng_memcpy(pdest, psrc);
  return INT2FIX(ret);
}

/* Class definition */

void Init_QRNG() {
  rbgsl_cQRNG = rb_define_class_under(rbgsl_mRandom, "QRNG", rb_cObject);

  rb_define_singleton_method(rbgsl_cQRNG, "new", QRNG_new, 2);
  rb_define_method(rbgsl_cQRNG, "initialize", QRNG_init, 2);
  rb_define_method(rbgsl_cQRNG, "reset", QRNG_reset, 0);

  rb_define_method(rbgsl_cQRNG, "get", QRNG_get, 0);
  rb_define_method(rbgsl_cQRNG, "name", QRNG_name, 0);
  rb_define_alias(rbgsl_cQRNG, "to_s", "name");
  rb_define_method(rbgsl_cQRNG, "dim", QRNG_dim, 0);
  rb_define_method(rbgsl_cQRNG, "size", QRNG_size, 0);

  rb_define_module_function(rbgsl_cQRNG, "memcpy", QRNG_memcpy, 2);
  rb_define_method(rbgsl_cQRNG, "clone", QRNG_clone, 0);

  rb_define_const(rbgsl_cQRNG, "NIEDERREITER_2", INT2FIX(QRNG_NIEDERREITER_2));
  rb_define_const(rbgsl_cQRNG, "SOBOL", INT2FIX(QRNG_SOBOL));

}

/* vim: set ts=4: */
