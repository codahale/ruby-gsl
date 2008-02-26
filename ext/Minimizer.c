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

/* $Id: Minimizer.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_min.h"
#include "ruby-gsl.h"

/* constants for minimization */
#define MIN_GOLDENSECTION 0
#define MIN_BRENT 1

/* One dimensional Minimization */

static VALUE Minimizer_initialize(VALUE self, VALUE type) {
  return self;
}

static void Minimizer_free(void *p) {
  gsl_min_fminimizer_free(p);
}

VALUE Minimizer_new(VALUE class, VALUE type) {
  VALUE argv[1];
  VALUE minimizer;
  gsl_min_fminimizer * ptr;
  switch (NUM2INT(type)) {
    case MIN_GOLDENSECTION:
      ptr = gsl_min_fminimizer_alloc(gsl_min_fminimizer_goldensection);
      break;
	case MIN_BRENT:
      ptr = gsl_min_fminimizer_alloc(gsl_min_fminimizer_brent);
      break;
    default:
      rb_raise(rb_eArgError, "Illegal minimizer type!");
  }
  minimizer = Data_Wrap_Struct(class, 0, Minimizer_free, ptr);
  argv[0] = type;
  rb_obj_call_init(minimizer, 1, argv);
  return minimizer;
}

static VALUE Minimizer_name(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_str_new2(gsl_min_fminimizer_name(ptr));
}

static VALUE Minimizer_set(VALUE self, VALUE f, VALUE min, VALUE x_lower, 
  VALUE x_upper) {
  gsl_min_fminimizer *ptr;
  gsl_function *func;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  Data_Get_Struct(f, gsl_function, func);
  return INT2FIX(gsl_min_fminimizer_set(ptr, func, NUM2DBL(min), 
    NUM2DBL(x_lower), NUM2DBL(x_upper)));
}

static VALUE Minimizer_set_with_values(VALUE self, VALUE f, VALUE min, 
  VALUE f_min, VALUE x_lower, VALUE f_lower, VALUE x_upper, VALUE f_upper) {
  gsl_min_fminimizer *ptr;
  gsl_function *func;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  Data_Get_Struct(f, gsl_function, func);
  return INT2FIX(gsl_min_fminimizer_set_with_values(ptr, func, NUM2DBL(min), 
    NUM2DBL(f_min), NUM2DBL(x_lower), NUM2DBL(f_lower), 
	NUM2DBL(x_upper), NUM2DBL(f_upper)));
}

/* Iteration */

static VALUE Minimizer_iterate(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return INT2FIX(gsl_min_fminimizer_iterate(ptr));
}

static VALUE Minimizer_x_lower(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_float_new(gsl_min_fminimizer_x_lower(ptr));
}

static VALUE Minimizer_x_upper(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_float_new(gsl_min_fminimizer_x_upper(ptr));
}

static VALUE Minimizer_x_minimum(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_float_new(gsl_min_fminimizer_x_minimum(ptr));
}

static VALUE Minimizer_f_lower(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_float_new(gsl_min_fminimizer_f_lower(ptr));
}

static VALUE Minimizer_f_upper(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_float_new(gsl_min_fminimizer_f_upper(ptr));
}

static VALUE Minimizer_f_minimum(VALUE self) {
  gsl_min_fminimizer *ptr;
  Data_Get_Struct(self, gsl_min_fminimizer, ptr);
  return rb_float_new(gsl_min_fminimizer_f_minimum(ptr));
}

/* Search Stopping Parameters */

static VALUE Minimizer_test_interval(VALUE self, VALUE x_lower, VALUE x_upper,
  VALUE epsabs, VALUE epsrel) {
  return INT2FIX(gsl_min_test_interval(NUM2DBL(x_lower), NUM2DBL(x_upper), NUM2DBL(epsabs), NUM2DBL(epsrel)));
}


/* Class definition */

VALUE rbgsl_cMinimizer;

void Init_Minimizer() {

  rbgsl_cMinimizer = rb_define_class_under(rbgsl_mGSL, "Minimizer", rb_cObject);

  rb_define_singleton_method(rbgsl_cMinimizer, "new", Minimizer_new, 1);
  rb_define_method(rbgsl_cMinimizer, "initialize", Minimizer_initialize, 1);

  rb_define_module_function(rbgsl_cMinimizer, "test_interval", Minimizer_test_interval, 4);

  rb_define_method(rbgsl_cMinimizer, "set", Minimizer_set, 4);
  rb_define_method(rbgsl_cMinimizer, "set_with_values", Minimizer_set_with_values, 7);
  rb_define_alias(rbgsl_cMinimizer, "set2", "set_with_values");
  rb_define_method(rbgsl_cMinimizer, "name", Minimizer_name, 0);
  rb_define_alias(rbgsl_cMinimizer, "to_s", "name");
 
  rb_define_method(rbgsl_cMinimizer, "iterate", Minimizer_iterate, 0);

  rb_define_method(rbgsl_cMinimizer, "x_minimum", Minimizer_x_minimum, 0);
  rb_define_alias(rbgsl_cMinimizer, "min", "x_minimum");
  rb_define_method(rbgsl_cMinimizer, "x_lower", Minimizer_x_lower, 0);
  rb_define_method(rbgsl_cMinimizer, "x_upper", Minimizer_x_upper, 0);

  rb_define_method(rbgsl_cMinimizer, "f_minimum", Minimizer_f_minimum, 0);
  rb_define_method(rbgsl_cMinimizer, "f_lower", Minimizer_f_lower, 0);
  rb_define_method(rbgsl_cMinimizer, "f_upper", Minimizer_f_upper, 0);

  rb_define_const(rbgsl_cMinimizer, "GOLDENSECTION", INT2FIX(MIN_GOLDENSECTION));
  rb_define_const(rbgsl_cMinimizer, "BRENT", INT2FIX(MIN_BRENT));

}

/* vim: set ts=4: */
