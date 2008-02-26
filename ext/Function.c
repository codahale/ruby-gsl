/*
* ruby-gsl - using GSL from Ruby
* Copyright (C) 2001-2003 Arno Erpenbeck
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

/*
  rb_gsl_function.c
  Ruby/GSL: Ruby extension library for GSL (GNU Scientific Library)
    (C) Copyright 2001 by Yoshiki Tsunesada

  Ruby/GSL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.
*/

/* $Id: Function.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "ruby-gsl.h"
#include "version.h"

static ID ID_call, ID_arity;

static void Function_free(gsl_function * ptr) {
  free((gsl_function *) ptr);
}

static double Function_function(double x, void * p) {
  VALUE result;
  result = rb_funcall((VALUE) p, ID_call, 1, rb_float_new(x));
  return NUM2DBL(result);
}

static VALUE Function_new(VALUE class) {
  gsl_function * ptr;

  ptr = (gsl_function *) xmalloc(sizeof(gsl_function));
#if (RUBY_VERSION_MINOR < 8)
  ptr->params = (void *) rb_f_lambda();
#else
  ptr->params = (void *) rb_block_proc();
#endif
  ptr->function = &Function_function;
  (VALUE) ptr->params = rb_f_lambda();
   ptr->function = &Function_function;

  return Data_Wrap_Struct(class, 0, Function_free, ptr);
}			

// ?
static VALUE Function_new2(VALUE class, VALUE func) {
  gsl_function * ptr;

  ptr = (gsl_function *) xmalloc(sizeof(gsl_function));
#if (RUBY_VERSION_MINOR < 8)
  ptr->params = (void *) rb_f_lambda();
#else
  ptr->params = (void *) rb_block_proc();
#endif
  ptr->function = &Function_function;

  return Data_Wrap_Struct(class, 0, Function_free, ptr);
}			
	
static VALUE Function_eval(VALUE obj, VALUE x) {
  gsl_function * ptr;
  Data_Get_Struct(obj, gsl_function, ptr);
  return rb_funcall((VALUE) ptr->params, ID_call, 1, x);
}

static VALUE Function_arity(VALUE obj) {
  gsl_function * ptr;
  Data_Get_Struct(obj, gsl_function, ptr);
  return rb_funcall((VALUE) ptr->params, ID_arity, 0);
}

static VALUE Function_proc(VALUE obj) {
  gsl_function * ptr;
  Data_Get_Struct(obj, gsl_function, ptr);
  return (VALUE) ptr->params;
}

static VALUE Function_set(VALUE obj) {
  gsl_function * ptr;
  Data_Get_Struct(obj, gsl_function, ptr);
#if (RUBY_VERSION_MINOR < 8)
  ptr->params = (void *) rb_f_lambda();
#else
  ptr->params = (void *)rb_block_proc();
#endif
  return obj;
}

/* Class definition */

VALUE rbgsl_cFunction;

void Init_Function() {
  rbgsl_cFunction = rb_define_class_under(rbgsl_mGSL, "Function", rb_cObject);

  rb_define_singleton_method(rbgsl_cFunction, "new", Function_new, 0);
  rb_define_singleton_method(rbgsl_cFunction, "new2", Function_new2, 1);
  rb_define_singleton_method(rbgsl_cFunction, "alloc", Function_new, 0);

  rb_define_method(rbgsl_cFunction, "eval", Function_eval, 1);
  rb_define_alias(rbgsl_cFunction, "call", "eval");
  rb_define_alias(rbgsl_cFunction, "[]", "eval");
  rb_define_method(rbgsl_cFunction, "arity", Function_arity, 0);
  rb_define_method(rbgsl_cFunction, "proc", Function_proc, 0);
  rb_define_method(rbgsl_cFunction, "set", Function_set, 0);

  ID_call = rb_intern("call");
  ID_arity = rb_intern("arity");
}

/* vim: set ts=4: */
