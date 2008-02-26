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


/* $Id: Solver.c,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_roots.h"
#include "ruby-gsl.h"

/* constants for root-finding */
#define SOLVER_BISECTION 0
#define SOLVER_BRENT 1
#define SOLVER_FALSEPOS 2
#define SOLVER_NEWTON 3
#define SOLVER_SECANT 4
#define SOLVER_STEFFENSON 5

/* One dimensional Root-Finding */

/* Search Stopping Parameters */

static VALUE Solver_test_interval(VALUE self, VALUE x_lower, VALUE x_upper,
  VALUE epsabs, VALUE epsrel) {
  return INT2FIX(gsl_root_test_interval(NUM2DBL(x_lower), NUM2DBL(x_upper), NUM2DBL(epsabs), NUM2DBL(epsrel)));
}

static VALUE Solver_test_residual(VALUE self, VALUE f, VALUE epsabs) {
  return INT2FIX(gsl_root_test_residual(NUM2DBL(f), NUM2DBL(epsabs)));
}

static VALUE Solver_test_delta(VALUE self, VALUE x1, VALUE x0, VALUE epsabs, 
  VALUE epsrel) {
  return INT2FIX(gsl_root_test_delta(NUM2DBL(x1), NUM2DBL(x0), NUM2DBL(epsabs), NUM2DBL(epsrel)));
}

/* bracketing-based solver */

static VALUE FSolver_initialize(VALUE self, VALUE type) {
  return self;
}

static void FSolver_free(void * p) {
  gsl_root_fsolver_free(p);
}

VALUE FSolver_new(VALUE class, VALUE type) {
  VALUE argv[1];
  VALUE solver;
  gsl_root_fsolver * ptr;
  switch (NUM2INT(type)) {
    case SOLVER_BISECTION:
      ptr = gsl_root_fsolver_alloc(gsl_root_fsolver_bisection);
      break;
	case SOLVER_BRENT:
      ptr = gsl_root_fsolver_alloc(gsl_root_fsolver_brent);
      break;
	case SOLVER_FALSEPOS:
      ptr = gsl_root_fsolver_alloc(gsl_root_fsolver_falsepos);
      break;
    default:
      rb_raise(rb_eArgError, "Illegal root solver type!");
  }
  solver = Data_Wrap_Struct(class, 0, FSolver_free, ptr);
  argv[0] = type;
  rb_obj_call_init(solver, 1, argv);
  return solver;
}

static VALUE FSolver_name(VALUE self) {
  gsl_root_fsolver * ptr;
  Data_Get_Struct(self, gsl_root_fsolver, ptr);
  return rb_str_new2(gsl_root_fsolver_name(ptr));
}

static VALUE FSolver_set(VALUE self, VALUE f, VALUE x_lower, VALUE x_upper) {
  gsl_root_fsolver * ptr;
  gsl_function * func;
  Data_Get_Struct(self, gsl_root_fsolver, ptr);
  Data_Get_Struct(f, gsl_function, func);
  return INT2FIX(gsl_root_fsolver_set(ptr, func, NUM2DBL(x_lower), NUM2DBL(x_upper)));
}

/* Iteration */

static VALUE FSolver_iterate(VALUE self) {
  gsl_root_fsolver * ptr;
  Data_Get_Struct(self, gsl_root_fsolver, ptr);
  return INT2FIX(gsl_root_fsolver_iterate(ptr));
}

static VALUE FSolver_x_lower(VALUE self) {
  gsl_root_fsolver * ptr;
  Data_Get_Struct(self, gsl_root_fsolver, ptr);
  return rb_float_new(gsl_root_fsolver_x_lower(ptr));
}

static VALUE FSolver_x_upper(VALUE self) {
  gsl_root_fsolver * ptr;
  Data_Get_Struct(self, gsl_root_fsolver, ptr);
  return rb_float_new(gsl_root_fsolver_x_upper(ptr));
}

static VALUE FSolver_root(VALUE self) {
  gsl_root_fsolver * ptr;
  Data_Get_Struct(self, gsl_root_fsolver, ptr);
  return rb_float_new(gsl_root_fsolver_root(ptr));
}

/* derivative-based solver */

static VALUE FDFSolver_initialize(VALUE self, VALUE type) {
  return self;
}

static void FDFSolver_free(void * p) {
  gsl_root_fdfsolver_free(p);
}

VALUE FDFSolver_new(VALUE class, VALUE type) {
  VALUE argv[1];
  VALUE solver;
  gsl_root_fdfsolver * ptr;
  switch (NUM2INT(type)) {
    case SOLVER_NEWTON:
      ptr = gsl_root_fdfsolver_alloc(gsl_root_fdfsolver_newton);
      break;
	case SOLVER_SECANT:
      ptr = gsl_root_fdfsolver_alloc(gsl_root_fdfsolver_secant);
      break;
	case SOLVER_STEFFENSON:
      ptr = gsl_root_fdfsolver_alloc(gsl_root_fdfsolver_steffenson);
      break;
    default:
      rb_raise(rb_eArgError, "Illegal root solver type!");
  }
  solver = Data_Wrap_Struct(class, 0, FDFSolver_free, ptr);
  argv[0] = type;
  rb_obj_call_init(solver, 1, argv);
  return solver;
}

static VALUE FDFSolver_name(VALUE self) {
  gsl_root_fdfsolver * ptr;
  Data_Get_Struct(self, gsl_root_fdfsolver, ptr);
  return rb_str_new2(gsl_root_fdfsolver_name(ptr));
}

static VALUE FDFSolver_set(VALUE self, VALUE fdf, VALUE root) {
  rb_raise(rb_eStandardError, "Not implemented yet.");
  return INT2FIX(1);
}

/* Iteration */

static VALUE FDFSolver_root(VALUE self) {
  gsl_root_fdfsolver * ptr;
  Data_Get_Struct(self, gsl_root_fdfsolver, ptr);
  return rb_float_new(gsl_root_fdfsolver_root(ptr));
}

static VALUE FDFSolver_iterate(VALUE self) {
  gsl_root_fdfsolver * ptr;
  Data_Get_Struct(self, gsl_root_fdfsolver, ptr);
  return INT2FIX(gsl_root_fdfsolver_iterate(ptr));
}

/* Class definition */

VALUE rbgsl_cFSolver;
VALUE rbgsl_cFDFSolver;
VALUE rbgsl_mSolver;

void Init_Solver() {
  rbgsl_mSolver = rb_define_module_under(rbgsl_mGSL, "Solver");

  rb_define_module_function(rbgsl_mSolver, "test_interval", Solver_test_interval, 4);
  rb_define_module_function(rbgsl_mSolver, "test_residual", Solver_test_residual, 2);
  rb_define_module_function(rbgsl_mSolver, "test_delta", Solver_test_delta, 4);

  /* root bracketing algorithms */

  rbgsl_cFSolver = rb_define_class_under(rbgsl_mSolver, "FSolver", rb_cObject);

  rb_define_singleton_method(rbgsl_cFSolver, "new", FSolver_new, 1);
  rb_define_method(rbgsl_cFSolver, "initialize", FSolver_initialize, 1);

  rb_define_method(rbgsl_cFSolver, "set", FSolver_set, 3);
  rb_define_method(rbgsl_cFSolver, "name", FSolver_name, 0);
  rb_define_alias(rbgsl_cFSolver, "to_s", "name");
 
  rb_define_method(rbgsl_cFSolver, "iterate", FSolver_iterate, 0);
  rb_define_method(rbgsl_cFSolver, "root", FSolver_root, 0);
  rb_define_method(rbgsl_cFSolver, "x_lower", FSolver_x_lower, 0);
  rb_define_method(rbgsl_cFSolver, "x_upper", FSolver_x_upper, 0);

  rb_define_const(rbgsl_cFSolver, "BISECTION", INT2FIX(SOLVER_BISECTION));
  rb_define_const(rbgsl_cFSolver, "BRENT", INT2FIX(SOLVER_BRENT));
  rb_define_const(rbgsl_cFSolver, "FALSEPOS", INT2FIX(SOLVER_FALSEPOS));

  /* root finding algorithms using derivatives */

  rbgsl_cFDFSolver = rb_define_class_under(rbgsl_mSolver, "FDFSolver", rb_cObject);

  rb_define_singleton_method(rbgsl_cFDFSolver, "new", FDFSolver_new, 1);
  rb_define_method(rbgsl_cFDFSolver, "initialize", FDFSolver_initialize, 1);

  rb_define_method(rbgsl_cFDFSolver, "set", FDFSolver_set, 3);
  rb_define_method(rbgsl_cFDFSolver, "name", FDFSolver_name, 0);
  rb_define_alias(rbgsl_cFDFSolver, "to_s", "name");
 
  rb_define_method(rbgsl_cFDFSolver, "iterate", FDFSolver_iterate, 0);
  rb_define_method(rbgsl_cFDFSolver, "root", FDFSolver_root, 0);

  rb_define_const(rbgsl_cFDFSolver, "NEWTON", INT2FIX(SOLVER_NEWTON));
  rb_define_const(rbgsl_cFDFSolver, "SECANT", INT2FIX(SOLVER_SECANT));
  rb_define_const(rbgsl_cFDFSolver, "STEFFENSON", INT2FIX(SOLVER_STEFFENSON));

}

/* vim: set ts=4: */
