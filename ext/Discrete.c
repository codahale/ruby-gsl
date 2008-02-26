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

/* $Id: Discrete.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_randist.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* General Discrete Distributions */

static void Discrete_free(void * p) {
  gsl_ran_discrete_free(p);
}

static VALUE Discrete_initialize(VALUE self, VALUE p) {
  return self;
}

VALUE Discrete_new(VALUE class, VALUE p) {
  double * my_p;
  VALUE argv[1];
  VALUE discrete;
  gsl_ran_discrete_t * ptr;
  COPYRUBYARRAY(p, my_p);
  ptr = gsl_ran_discrete_preproc(RARRAY(p)->len, my_p);
  discrete = Data_Wrap_Struct(class, 0, Discrete_free, ptr);
  argv[0] = p;
  rb_obj_call_init(discrete, 1, argv);
  return discrete;
}

static VALUE Discrete_get(VALUE self, VALUE rng) {
  gsl_ran_discrete_t * dptr;
  gsl_rng * rptr;
  Data_Get_Struct(self, gsl_ran_discrete_t, dptr);
  Data_Get_Struct(rng, gsl_rng, rptr);
  return INT2FIX(gsl_ran_discrete(rptr, dptr)); 
}

static VALUE Discrete_get_pdf(VALUE self, VALUE k) {
  gsl_ran_discrete_t * dptr;
  Data_Get_Struct(self, gsl_ran_discrete_t, dptr);
  return rb_float_new(gsl_ran_discrete_pdf(NUM2INT(k), dptr)); 
}

/* Class definition */

VALUE rbgsl_cDiscrete;

void Init_Discrete() {
  rbgsl_cDiscrete = rb_define_class_under(rbgsl_mRandom, "Discrete", rb_cObject);
  rb_define_singleton_method(rbgsl_cDiscrete, "new", Discrete_new, 1);
  rb_define_method(rbgsl_cDiscrete, "initialize", Discrete_initialize, 1);

  rb_define_method(rbgsl_cDiscrete, "get", Discrete_get, 1);
  rb_define_method(rbgsl_cDiscrete, "get_pdf", Discrete_get_pdf, 1);
}

/* vim: set ts=4: */
