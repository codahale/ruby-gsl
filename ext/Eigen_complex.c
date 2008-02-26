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

/*
  Ruby/GSL: Ruby extension library for GSL (GNU Scientific Library)
    (C) Copyright 2001 by Yoshiki Tsunesada

  Ruby/GSL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.
*/

/* $Id: Eigen_complex.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */


#include "ruby.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_eigen.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_vector_complex.h"
#include "gsl/gsl_min.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* constants for Eigen_complex */
#define EIGEN_COMPLEX_SORT_VAL_ASC 0
#define EIGEN_COMPLEX_SORT_VAL_DESC 1
#define EIGEN_COMPLEX_SORT_ABS_ASC 2
#define EIGEN_COMPLEX_SORT_ABS_DESC 3

/* Eigen_complex systems */

/* Complex Hermitian Matrices */
static VALUE Matrix_eigen_herm(VALUE obj) {
  gsl_matrix_complex *m;
  gsl_vector *v;
  gsl_eigen_herm_workspace *w;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  w = gsl_eigen_herm_alloc(m->size1);
  v = gsl_vector_alloc(m->size1);

  gsl_eigen_herm(m, v, w);

  gsl_eigen_herm_free(w);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}


static VALUE Matrix_eigen_herm2(VALUE obj) {
  gsl_matrix_complex *m, *m2;
  gsl_vector *v;
  gsl_eigen_herm_workspace *w;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  m2 = gsl_matrix_complex_alloc(m->size1, m->size1);
  gsl_matrix_complex_memcpy(m2, m);
  w = gsl_eigen_herm_alloc(m->size1);
  v = gsl_vector_alloc(m->size1);

  gsl_eigen_herm(m2, v, w);

  gsl_eigen_herm_free(w);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE Matrix_eigen_hermv(VALUE obj) {
  gsl_matrix_complex *m, *evec;
  gsl_vector *v;
  gsl_eigen_hermv_workspace *w;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  w = gsl_eigen_hermv_alloc(m->size1);
  v = gsl_vector_alloc(m->size1);
  evec = gsl_matrix_complex_alloc(m->size1, m->size1);

  gsl_eigen_hermv(m, v, evec, w);

  gsl_eigen_hermv_free(w);

  return rb_ary_new3(2, Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v),
     Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, evec));
}


static VALUE Matrix_eigen_hermv2(VALUE obj) {
  gsl_matrix_complex *m, *m2, *evec;
  gsl_vector *v;
  gsl_eigen_hermv_workspace *w;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  m2 = gsl_matrix_complex_alloc(m->size1, m->size1);
  gsl_matrix_complex_memcpy(m2, m);
  w = gsl_eigen_hermv_alloc(m->size1);
  v = gsl_vector_alloc(m->size1);
  evec = gsl_matrix_complex_alloc(m->size1, m->size1);

  gsl_eigen_hermv(m2, v, evec, w);

  gsl_eigen_hermv_free(w);

  return rb_ary_new3(2, Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v),
     Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, evec));
}

/* Sorting Eigen_complexvalues and Eigen_complexvectors */

static VALUE Eigen_complex_hermv_sort(VALUE self, VALUE vec, VALUE mat, VALUE type) {
  gsl_matrix_complex *m;
  gsl_vector *v;
  Data_Get_Struct(mat, gsl_matrix_complex, m);
  Data_Get_Struct(vec, gsl_vector, v);
  switch (NUM2INT(type)) {
    case EIGEN_COMPLEX_SORT_VAL_ASC:
      gsl_eigen_hermv_sort(v, m, GSL_EIGEN_SORT_VAL_ASC);
      break;
    case EIGEN_COMPLEX_SORT_VAL_DESC:
      gsl_eigen_hermv_sort(v, m, GSL_EIGEN_SORT_VAL_DESC);
      break;
    case EIGEN_COMPLEX_SORT_ABS_ASC:
      gsl_eigen_hermv_sort(v, m, GSL_EIGEN_SORT_ABS_ASC);
      break;
    case EIGEN_COMPLEX_SORT_ABS_DESC:
      gsl_eigen_hermv_sort(v, m, GSL_EIGEN_SORT_ABS_DESC);
      break;
    default:
        rb_raise(rb_eArgError, "Illegal sort type!");
  }
  return self;
}

/* Module definition */

VALUE rbgsl_mEigen_complex;

void Init_Eigen_complex() {
  rb_define_method(rbgsl_cMatrix_complex, "eigen_herm!", Matrix_eigen_herm, 0);
  rb_define_method(rbgsl_cMatrix_complex, "eigen_herm", Matrix_eigen_herm2, 0);
  rb_define_method(rbgsl_cMatrix_complex, "eigen_hermv!", Matrix_eigen_hermv, 0);
  rb_define_method(rbgsl_cMatrix_complex, "eigen_hermv", Matrix_eigen_hermv2, 0);

  rbgsl_mEigen_complex = rb_define_module_under(rbgsl_mGSL, "Eigen_complex");
  rb_define_const(rbgsl_mEigen_complex, "SORT_VAL_ASC", INT2FIX(EIGEN_COMPLEX_SORT_VAL_ASC));
  rb_define_const(rbgsl_mEigen_complex, "SORT_VAL_DESC", INT2FIX(EIGEN_COMPLEX_SORT_VAL_DESC));
  rb_define_const(rbgsl_mEigen_complex, "SORT_ABS_ASC", INT2FIX(EIGEN_COMPLEX_SORT_ABS_ASC));
  rb_define_const(rbgsl_mEigen_complex, "SORT_ABS_DESC", INT2FIX(EIGEN_COMPLEX_SORT_ABS_DESC));
  rb_define_module_function(rbgsl_mEigen_complex, "hermv_sort", Eigen_complex_hermv_sort, 3);
}

/* vim: set ts=4: */
