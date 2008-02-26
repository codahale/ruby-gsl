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


/* $Id: Multiroot.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_multiroots.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* constants for Multiroot */
#define MSOLVER_BISECTION 0
#define MSOLVER_BRENT 1
#define MSOLVER_FALSEPOS 2
#define MSOLVER_DNEWTON 3
#define MSOLVER_BROYDEN 4
#define MSOLVER_HYBRID 5
#define MSOLVER_HYBRIDS 6
#define MSOLVER_NEWTON 7
#define MSOLVER_GNEWTON 8
#define MSOLVER_HYBRIDJ 9
#define MSOLVER_HYBRIDSJ 10

/* Multidimensional Root-Finding */

/* Search Stopping Parameters */

static VALUE MSolver_test_residual(VALUE self, VALUE f, VALUE epsabs) {
  gsl_vector * myf;
  Data_Get_Struct(f, gsl_vector, myf);
  return INT2FIX(gsl_multiroot_test_residual(myf, NUM2DBL(epsabs)));
}

static VALUE MSolver_test_delta(VALUE self, VALUE dx, VALUE x, VALUE epsabs, 
  VALUE epsrel) {
  gsl_vector * mydx, * myx;
  Data_Get_Struct(dx, gsl_vector, mydx);
  Data_Get_Struct(x, gsl_vector, myx);
  return INT2FIX(gsl_multiroot_test_delta(mydx, myx, NUM2DBL(epsabs), NUM2DBL(epsrel)));
}

/* Initializing the Solver */

static VALUE MFSolver_initialize(VALUE self, VALUE type) {
  return self;
}

static void MFSolver_free(void * p) {
  gsl_multiroot_fsolver_free(p);
}

VALUE MFSolver_new(VALUE class, VALUE type, VALUE n) {
  VALUE argv[2];
  VALUE solver;
  gsl_multiroot_fsolver * ptr;
  switch (NUM2INT(type)) {
    case MSOLVER_DNEWTON:
      ptr = gsl_multiroot_fsolver_alloc (gsl_multiroot_fsolver_dnewton, NUM2INT(n));
      break;
	case MSOLVER_BROYDEN:
      ptr = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_broyden, NUM2INT(n));
      break;
	case MSOLVER_HYBRID:
      ptr = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrid, NUM2INT(n));
      break;
	case MSOLVER_HYBRIDS:
      ptr = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrids, NUM2INT(n));
      break;
    default:
      rb_raise(rb_eArgError, "Illegal multiroot solver type!");
  }
  solver = Data_Wrap_Struct(class, 0, MFSolver_free, ptr);
  argv[0] = type;
  argv[1] = n;
  rb_obj_call_init(solver, 2, argv);
  return solver;
}

static VALUE MFSolver_name(VALUE self) {
  gsl_multiroot_fsolver * ptr;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  return rb_str_new2(gsl_multiroot_fsolver_name(ptr));
}

static VALUE MFSolver_set(VALUE self, VALUE f, VALUE x) {
  rb_raise(rb_eStandardError, "Not implemented yet.");
  /*
  gsl_multiroot_fsolver * ptr;
  gsl_multiroot_function * func;
  gsl_vector * my;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  Data_Get_Struct(f, gsl_multiroot_function, func);
  Data_Get_Struct(x, gsl_vector, myx);
  return INT2FIX(gsl_multiroot_fsolver_set(ptr, func, myx));
  */
  return INT2FIX(1);
}

/* Iteration */

static VALUE MFSolver_iterate(VALUE self) {
  gsl_multiroot_fsolver * ptr;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  return INT2FIX(gsl_multiroot_fsolver_iterate(ptr));
}

static VALUE MFSolver_root(VALUE self) {
  gsl_multiroot_fsolver * ptr;
  gsl_vector * v;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  v = gsl_multiroot_fsolver_root(ptr);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE MFSolver_dx(VALUE self) {
  gsl_multiroot_fsolver * ptr;
  gsl_vector * v;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  v = gsl_multiroot_fsolver_dx(ptr);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE MFSolver_f(VALUE self) {
  gsl_multiroot_fsolver * ptr;
  gsl_vector * v;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  v = gsl_multiroot_fsolver_f(ptr);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

/* derivative-based solver */

static VALUE MFDFSolver_initialize(VALUE self, VALUE type) {
  return self;
}

static void MFDFSolver_free(void * p) {
  gsl_multiroot_fdfsolver_free(p);
}

VALUE MFDFSolver_new(VALUE class, VALUE type, VALUE n) {
  VALUE argv[2];
  VALUE solver;
  gsl_multiroot_fdfsolver * ptr;
  switch (NUM2INT(type)) {
    case MSOLVER_NEWTON:
      ptr = gsl_multiroot_fdfsolver_alloc(gsl_multiroot_fdfsolver_newton, NUM2INT(n));
      break;
	case MSOLVER_GNEWTON:
      ptr = gsl_multiroot_fdfsolver_alloc(gsl_multiroot_fdfsolver_gnewton, NUM2INT(n));
      break;
	case MSOLVER_HYBRIDJ:
      ptr = gsl_multiroot_fdfsolver_alloc(gsl_multiroot_fdfsolver_hybridj, NUM2INT(n));
      break;
	case MSOLVER_HYBRIDSJ:
      ptr = gsl_multiroot_fdfsolver_alloc(gsl_multiroot_fdfsolver_hybridsj, NUM2INT(n));
      break;
    default:
      rb_raise(rb_eArgError, "Illegal multiroot solver type!");
  }
  solver = Data_Wrap_Struct(class, 0, MFDFSolver_free, ptr);
  argv[0] = type;
  argv[1] = n;
  rb_obj_call_init(solver, 2, argv);
  return solver;
}

static VALUE MFDFSolver_name(VALUE self) {
  gsl_multiroot_fdfsolver * ptr;
  Data_Get_Struct(self, gsl_multiroot_fdfsolver, ptr);
  return rb_str_new2(gsl_multiroot_fdfsolver_name(ptr));
}

static VALUE MFDFSolver_set(VALUE self, VALUE fdf, VALUE root) {
  rb_raise(rb_eStandardError, "Not implemented yet.");
  /*
  gsl_multiroot_fsolver * ptr;
  gsl_multiroot_function * func;
  gsl_vector * my;
  Data_Get_Struct(self, gsl_multiroot_fsolver, ptr);
  Data_Get_Struct(f, gsl_multiroot_function, func);
  Data_Get_Struct(x, gsl_vector, myx);
  return INT2FIX(gsl_multiroot_fsolver_set(ptr, func, myx));
  */
  return INT2FIX(1);
}

/* Iteration */

static VALUE MFDFSolver_root(VALUE self) {
  gsl_multiroot_fdfsolver * ptr;
  gsl_vector * v;
  Data_Get_Struct(self, gsl_multiroot_fdfsolver, ptr);
  v = gsl_multiroot_fdfsolver_root(ptr);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE MFDFSolver_dx(VALUE self) {
  gsl_multiroot_fdfsolver * ptr;
  gsl_vector * v;
  Data_Get_Struct(self, gsl_multiroot_fdfsolver, ptr);
  v = gsl_multiroot_fdfsolver_dx(ptr);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE MFDFSolver_f(VALUE self) {
  gsl_multiroot_fdfsolver * ptr;
  gsl_vector * v;
  Data_Get_Struct(self, gsl_multiroot_fdfsolver, ptr);
  v = gsl_multiroot_fdfsolver_f(ptr);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE MFDFSolver_iterate(VALUE self) {
  gsl_multiroot_fdfsolver * ptr;
  Data_Get_Struct(self, gsl_multiroot_fdfsolver, ptr);
  return INT2FIX(gsl_multiroot_fdfsolver_iterate(ptr));
}

/* Class definition */

VALUE rbgsl_cMFSolver;
VALUE rbgsl_cMFDFSolver;
VALUE rbgsl_mMSolver;

void Init_MSolver() {
  rbgsl_mMSolver = rb_define_module_under(rbgsl_mGSL, "Multiroot");

  rb_define_module_function(rbgsl_mMSolver, "test_residual", MSolver_test_residual, 2);
  rb_define_module_function(rbgsl_mMSolver, "test_delta", MSolver_test_delta, 4);

  /* algorithms without derivatives */

  rbgsl_cMFSolver = rb_define_class_under(rbgsl_mMSolver, "MFSolver", rb_cObject);

  rb_define_singleton_method(rbgsl_cMFSolver, "new", MFSolver_new, 2);
  rb_define_method(rbgsl_cMFSolver, "initialize", MFSolver_initialize, 2);

  rb_define_method(rbgsl_cMFSolver, "set", MFSolver_set, 2);
  rb_define_method(rbgsl_cMFSolver, "name", MFSolver_name, 0);
  rb_define_alias(rbgsl_cMFSolver, "to_s", "name");
 
  rb_define_method(rbgsl_cMFSolver, "iterate", MFSolver_iterate, 0);
  rb_define_method(rbgsl_cMFSolver, "root", MFSolver_root, 0);
  rb_define_method(rbgsl_cMFSolver, "dx", MFSolver_dx, 0);
  rb_define_method(rbgsl_cMFSolver, "f", MFSolver_f, 0);

  rb_define_const(rbgsl_cMFSolver, "BISECTION", INT2FIX(MSOLVER_BISECTION));
  rb_define_const(rbgsl_cMFSolver, "BRENT", INT2FIX(MSOLVER_BRENT));
  rb_define_const(rbgsl_cMFSolver, "FALSEPOS", INT2FIX(MSOLVER_FALSEPOS));

  rb_define_const(rbgsl_cMFSolver, "DNEWTON", INT2FIX(MSOLVER_DNEWTON));
  rb_define_const(rbgsl_cMFSolver, "BROYDEN", INT2FIX(MSOLVER_BROYDEN));
  rb_define_const(rbgsl_cMFSolver, "HYBRID", INT2FIX(MSOLVER_HYBRID));
  rb_define_const(rbgsl_cMFSolver, "HYBRIDS", INT2FIX(MSOLVER_HYBRIDS));

  /* root finding algorithms using derivatives */

  rbgsl_cMFDFSolver = rb_define_class_under(rbgsl_mMSolver, "MFDFSolver", rb_cObject);

  rb_define_singleton_method(rbgsl_cMFDFSolver, "new", MFDFSolver_new, 2);
  rb_define_method(rbgsl_cMFDFSolver, "initialize", MFDFSolver_initialize, 2);

  rb_define_method(rbgsl_cMFDFSolver, "set", MFDFSolver_set, 2);
  rb_define_method(rbgsl_cMFDFSolver, "name", MFDFSolver_name, 0);
  rb_define_alias(rbgsl_cMFDFSolver, "to_s", "name");
 
  rb_define_method(rbgsl_cMFDFSolver, "iterate", MFDFSolver_iterate, 0);
  rb_define_method(rbgsl_cMFDFSolver, "root", MFDFSolver_root, 0);
  rb_define_method(rbgsl_cMFDFSolver, "dx", MFDFSolver_dx, 0);
  rb_define_method(rbgsl_cMFDFSolver, "f", MFDFSolver_f, 0);

  rb_define_const(rbgsl_cMFDFSolver, "NEWTON", INT2FIX(MSOLVER_NEWTON));
  rb_define_const(rbgsl_cMFDFSolver, "GNEWTON", INT2FIX(MSOLVER_GNEWTON));
  rb_define_const(rbgsl_cMFDFSolver, "HYBRIDJ", INT2FIX(MSOLVER_HYBRIDJ));
  rb_define_const(rbgsl_cMFDFSolver, "HYBRIDSJ", INT2FIX(MSOLVER_HYBRIDSJ));

}

/* vim: set ts=4: */
