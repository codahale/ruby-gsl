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

/* $Id: MultiFunction.c,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <stdio.h>
#include "ruby.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_multiroots.h"
#include "ruby-gsl.h"

static ID ID_call, ID_arity;

static void MFunction_free(gsl_function * ptr) {
  free((gsl_multiroot_function *) ptr);
}

static int MFunction_function(const gsl_vector * x, void * p, gsl_vector * f) {
  rb_raise(rb_eStandardError, "Not implemented yet.");
/*
static int MFunction_function(const gsl_vector * x, void * p) {
  VALUE result;
  result = rb_funcall((VALUE) p, ID_call, 2, rb_float_new(x), ...);
  
  return NUM2INT(result);
*/
  return 1;
}

static VALUE MFunction_new(VALUE class) {
  gsl_multiroot_function * ptr;
  ptr = (gsl_multiroot_function *) xmalloc(sizeof(gsl_multiroot_function));
#if (RUBY_VERSION_MINOR < 8)
  ptr->params = (void *) rb_f_lambda();
#else
  ptr->params = (void *) rb_block_proc();
#endif
  ptr->f = &MFunction_function;
  ID_call = rb_intern("call");
  ID_arity = rb_intern("arity");

  return Data_Wrap_Struct(class, 0, MFunction_free, ptr);
}			

static VALUE MFunction_new2(VALUE class, VALUE func, VALUE n) {
  gsl_multiroot_function * ptr;
  ptr = (gsl_multiroot_function *) xmalloc(sizeof(gsl_multiroot_function));
#if (RUBY_VERSION_MINOR < 8)
  ptr->params = (void *) rb_f_lambda();
#else
  ptr->params = (void *) rb_block_proc();
#endif
  ptr->f = &MFunction_function;
  ptr->n = NUM2INT(n);

  return Data_Wrap_Struct(class, 0, MFunction_free, ptr);
}			
	
static VALUE MFunction_eval(VALUE obj, VALUE x, VALUE f) {
  gsl_multiroot_function * ptr;
  gsl_vector * myx, * myf;
  VALUE result;
  Data_Get_Struct(obj, gsl_multiroot_function, ptr);
  Data_Get_Struct(x, gsl_vector, myx);
  Data_Get_Struct(f, gsl_vector, myf);
  result = rb_funcall((VALUE) ptr->params, ID_call, 2, myx, myf);
  return result;
}

static VALUE MFunction_arity(VALUE obj) {
  gsl_multiroot_function * ptr;
  Data_Get_Struct(obj, gsl_multiroot_function, ptr);
  return INT2FIX(rb_funcall((VALUE) ptr->params, ID_arity, 0));
}

static VALUE MFunction_proc(VALUE obj) {
  gsl_multiroot_function * ptr;
  Data_Get_Struct(obj, gsl_multiroot_function, ptr);
  return (VALUE) ptr->params;
}

static VALUE MFunction_set(VALUE obj) {
  gsl_multiroot_function * ptr;
  Data_Get_Struct(obj, gsl_multiroot_function, ptr);
#if (RUBY_VERSION_MINOR < 8)
  ptr->params = (void *) rb_f_lambda();
#else
  ptr->params = (void *) rb_block_proc();
#endif
  return obj;
}

/* Class definition */

VALUE rbgsl_cMultiFunction;

void Init_MFunction() {
  rbgsl_cMultiFunction = rb_define_class_under(rbgsl_mGSL, "MFunction", rb_cObject);

  rb_define_singleton_method(rbgsl_cMultiFunction, "new", MFunction_new, 0);
  rb_define_singleton_method(rbgsl_cMultiFunction, "new2", MFunction_new2, 1);
  rb_define_singleton_method(rbgsl_cMultiFunction, "alloc", MFunction_new, 0);

  rb_define_method(rbgsl_cMultiFunction, "eval", MFunction_eval, 2);
  rb_define_alias(rbgsl_cMultiFunction, "call", "eval");
  rb_define_alias(rbgsl_cMultiFunction, "[]", "eval");
  rb_define_method(rbgsl_cMultiFunction, "arity", MFunction_arity, 0);
  rb_define_method(rbgsl_cMultiFunction, "proc", MFunction_proc, 0);
  rb_define_method(rbgsl_cMultiFunction, "set", MFunction_set, 0);
}

/* vim: set ts=4: */
