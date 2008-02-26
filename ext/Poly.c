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

/* $Id: Poly.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_poly.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Roots of Polynomials */

/* Polynomial evaluation */

static VALUE Poly_evaluate(VALUE self, VALUE c, VALUE x) {
  double * my_c;
  COPYRUBYARRAY(c, my_c);
  return rb_float_new(gsl_poly_eval(my_c, RARRAY(c)->len, NUM2DBL(x)));
}

/* Quadratic equations */

static VALUE Poly_solve_quadratic(VALUE self, VALUE a, VALUE b, VALUE c) {
  double x0, x1;
  int ret;
  VALUE ary;
  ret = gsl_poly_solve_quadratic(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), 
    &x0, &x1);
  if (ret == 0)
    ary = rb_ary_new2(0);
  else if (ret == 2)
    ary = rb_ary_new3(2, rb_float_new(x0), rb_float_new(x1));
  else
    rb_raise(rb_eStandardError, "Unknown error.");
  return ary;
}

static VALUE Poly_complex_solve_quadratic(VALUE self, VALUE a, VALUE b, 
  VALUE c) {
  gsl_complex z0, z1;
  int ret;
  VALUE ary;
  ret = gsl_poly_complex_solve_quadratic(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), 
    &z0, &z1);
  ary = rb_ary_new3(2, COMPLEX(&z0), COMPLEX(&z1));
  return ary;
}

/* Cubic equations */

static VALUE Poly_solve_cubic(VALUE self, VALUE a, VALUE b, VALUE c) {
  double x0, x1, x2;
  int ret;
  VALUE ary;
  ret = gsl_poly_solve_cubic(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), 
    &x0, &x1, &x2);
  if (ret == 1)
    ary = rb_ary_new3(1, rb_float_new(x0));
  else if (ret == 3)
    ary = rb_ary_new3(3, rb_float_new(x0), rb_float_new(x1), rb_float_new(x2));
  else
    rb_raise(rb_eStandardError, "Unknown error.");
  return ary;
}

static VALUE Poly_complex_solve_cubic(VALUE self, VALUE a, VALUE b, 
  VALUE c) {
  gsl_complex z0, z1, z2;
  int ret;
  VALUE ary;
  ret = gsl_poly_complex_solve_cubic(NUM2DBL(a), NUM2DBL(b), NUM2DBL(c), 
    &z0, &z1, &z2);
  ary = rb_ary_new3(3, COMPLEX(&z0), COMPLEX(&z1), COMPLEX(&z2));
  return ary;
}

/* General polynomial equations */

static VALUE Workspace_init(VALUE self, VALUE size) {
  return self;
}

static void Workspace_free(void * p) {
  gsl_poly_complex_workspace_free(p);
}

VALUE Workspace_new(VALUE class, VALUE size) {
  VALUE argv[1];
  VALUE work;
  gsl_poly_complex_workspace * ptr;
  ptr = gsl_poly_complex_workspace_alloc(NUM2INT(size));
  work = Data_Wrap_Struct(class, 0, Workspace_free, ptr);
  argv[0] = size;
  rb_obj_call_init(work, 1, argv);
  return work;
}

static VALUE Workspace_solve(VALUE self, VALUE a) {
  int ret;
  long dim = RARRAY(a)->len;
  double * z, * my_a;
  VALUE ary;
  gsl_poly_complex_workspace * ptr;
  z = ALLOCA_N(double, 2 * (dim - 1));
  COPYRUBYARRAY(a, my_a);
  Data_Get_Struct(self, gsl_poly_complex_workspace, ptr);
  ret = gsl_poly_complex_solve(my_a, dim, ptr, z);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eStandardError, "QR reduction does not converge.");
  COPYCARRAY(z, ary, 2 * (dim - 1));
  return ary; 
}

/* Divided Difference Representation of Polynomials */

typedef struct {
  double * dd;
  double * xa;
  int size;
} poly_dd_struct;

static VALUE PolyDD_init(VALUE self, VALUE xa, VALUE ya) {
  return self;
}

static void PolyDD_free(void * p) {
  poly_dd_struct * ptr = (poly_dd_struct *) p;
  if (ptr->xa)
    free(ptr->xa);
  if (ptr->dd)
    free(ptr->dd);
  if (ptr)
    free(ptr);
}

VALUE PolyDD_new(VALUE class, VALUE xa, VALUE ya) {
  int i;
  long dim = RARRAY(xa)->len;
  double * my_ya;
  VALUE obj;
  poly_dd_struct * ptr = ALLOC(poly_dd_struct);
  COPYRUBYARRAY(ya, my_ya);
  ptr->xa = ALLOC_N(double, dim);
  for (i = 0; i < dim; i++)
    ptr->xa[i] = NUM2DBL(RARRAY(xa)->ptr[i]);
  ptr->dd = ALLOC_N(double, dim);
  ptr->size = dim;
  gsl_poly_dd_init(ptr->dd, ptr->xa, my_ya, dim);
  obj = Data_Wrap_Struct(class, 0, PolyDD_free, ptr);
  return obj;
}

static VALUE PolyDD_get(VALUE self) {
  poly_dd_struct * ptr;
  VALUE ary;
  Data_Get_Struct(self, poly_dd_struct, ptr);
  COPYCARRAY(ptr->dd, ary, ptr->size);
  return ary;
}

static VALUE PolyDD_evaluate(VALUE self, VALUE x) {
  poly_dd_struct * ptr;
  Data_Get_Struct(self, poly_dd_struct, ptr);
  return rb_float_new(gsl_poly_dd_eval(ptr->dd, ptr->xa, ptr->size, NUM2DBL(x)));
}

static VALUE PolyDD_taylor(VALUE self, VALUE xp) {
  poly_dd_struct * ptr;
  int ret;
  double * c, * w;
  VALUE ary;
  Data_Get_Struct(self, poly_dd_struct, ptr);
  w = ALLOCA_N(double, ptr->size);
  c = ALLOCA_N(double, ptr->size);
  ret = gsl_poly_dd_taylor(c, NUM2DBL(xp), ptr->dd, ptr->xa, ptr->size, w);
  if (ret != GSL_SUCCESS)
    rb_raise(rb_eStandardError, "Error while computing Taylor expansion");
  COPYCARRAY(c, ary, ptr->size);
  return ary;
}

/* Module definition */

VALUE rbgsl_mPoly;
VALUE rbgsl_cPolyDD;
VALUE rbgsl_cWorkspace;

void Init_Poly() {
  rbgsl_mPoly = rb_define_module_under(rbgsl_mGSL, "Poly");

  rb_define_module_function(rbgsl_mPoly, "evaluate", Poly_evaluate, 2);
  rb_define_module_function(rbgsl_mPoly, "solve_quadratic", Poly_solve_quadratic, 3);
  rb_define_module_function(rbgsl_mPoly, "complex_solve_quadratic", Poly_complex_solve_quadratic, 3);
  rb_define_module_function(rbgsl_mPoly, "solve_cubic", Poly_solve_cubic, 3);
  rb_define_module_function(rbgsl_mPoly, "complex_solve_cubic", Poly_complex_solve_cubic, 3);

  rbgsl_cWorkspace = rb_define_class_under(rbgsl_mPoly, "Workspace", rb_cObject);
  rb_define_singleton_method(rbgsl_cWorkspace, "new", Workspace_new, 1);
  rb_define_method(rbgsl_cWorkspace, "initialize", Workspace_init, 1);
  rb_define_method(rbgsl_cWorkspace, "solve", Workspace_solve, 1);

  rbgsl_cPolyDD = rb_define_class_under(rbgsl_mPoly, "DD", rb_cObject);
  rb_define_singleton_method(rbgsl_cPolyDD, "new", PolyDD_new, 2);
  rb_define_method(rbgsl_cPolyDD, "initialize", PolyDD_init, 2);
  rb_define_method(rbgsl_cPolyDD, "evaluate", PolyDD_evaluate, 1);
  rb_define_method(rbgsl_cPolyDD, "get", PolyDD_get, 0);
  rb_define_method(rbgsl_cPolyDD, "taylor", PolyDD_taylor, 1);
}

/* vim: set ts=4: */
