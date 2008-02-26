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

/* $Id: Matrix_complex.c,v 1.7 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <math.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_blas.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_matrix_complex_double.h"
#include "gsl/gsl_permutation.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Internal use only */

static gsl_matrix_complex * alloc_matrix_complex_from_array_sizes(VALUE ary, VALUE nn1,
  VALUE nn2);
static gsl_matrix_complex * alloc_matrix_complex_from_arrays(int argc, VALUE * argv);

/* Given a variable-length argument list consisting of 1 or more
 * single-dimensional arrays (not nested), create a matrix with
 * size1 == argc and size2 == RARRAY(argv[0])->len
 */
static gsl_matrix_complex * alloc_matrix_complex_from_arrays(int argc, VALUE * argv) {
  int n1, n2, dim, i, j;
  gsl_matrix_complex * matrix;
  VALUE ary;

  n1 = argc;
  n2 = RARRAY(argv[0])->len;
  dim = n2/2;
  matrix = gsl_matrix_complex_alloc(n1, dim);
  for (i = 0; i < n1; i++) {
    ary = argv[i];
    for (j = 0; j < dim; j++) {
      double d0, d1;
      gsl_complex z;
      d0 = NUM2DBL(rb_ary_entry(ary, 2*j));
      d1 = NUM2DBL(rb_ary_entry(ary, 2*j+1));
      GSL_SET_COMPLEX(&z, d0, d1);
      gsl_matrix_complex_set(matrix, i, j, z);
    }
  }
  return matrix;
}

/* Given a 2-dimensional array and an explicit width and height,
 * create a matrix using elements ary[0...nn1][0...nn2]
 */
static gsl_matrix_complex * alloc_matrix_complex_from_array_sizes(VALUE ary, VALUE nn1,
  VALUE nn2) {
  size_t n1, n2, dim;
  gsl_matrix_complex * matrix;
  size_t i, j, k;

  n1 = NUM2INT(nn1);
  n2 = NUM2INT(nn2);
  dim = n2/2;
  matrix = gsl_matrix_complex_alloc(n1, dim);
  k = 0;
  for (i = 0; i < n1; i++) {
    for (j = 0; j < dim; j++, k++) {
      double d0, d1;
      gsl_complex z;
      d0 = NUM2DBL(rb_ary_entry(ary, 2*j));
      d1 = NUM2DBL(rb_ary_entry(ary, 2*j+1));
      GSL_SET_COMPLEX(&z, d0, d1);
      gsl_matrix_complex_set(matrix, i, j, z);
    }
  }
  return matrix;
}

/* Matrices */

static VALUE Matrix_complex_new(int argc, VALUE * argv, VALUE klass)
{
  VALUE ary;
  gsl_matrix_complex * matrix = NULL;
  int type0, type1;

  type0 = TYPE(argv[0]);

  if (type0 == T_FIXNUM || type0 == T_BIGNUM) {
    matrix = gsl_matrix_complex_alloc(NUM2INT(argv[0]), NUM2INT(argv[1]));
  } else if (type0 == T_ARRAY) {
    ary = argv[0];
    type1 = TYPE(argv[1]);
    if (type1 == T_ARRAY) {
      matrix = alloc_matrix_complex_from_arrays(argc, argv);
    } else if (type1 == T_FIXNUM || type1 == T_BIGNUM) {
      matrix = alloc_matrix_complex_from_array_sizes(argv[0], argv[1], argv[2]);
    }
  }

  return Data_Wrap_Struct(klass, 0, gsl_matrix_complex_free, matrix);
}

/* Matrix_complex allocation */

static VALUE Matrix_complex_calloc(VALUE klass, VALUE n1, VALUE n2)
{
  VALUE obj;
  gsl_matrix_complex * matrix;

  matrix = gsl_matrix_complex_calloc(NUM2INT(n1), NUM2INT(n2));

  obj = Data_Wrap_Struct(klass, 0, gsl_matrix_complex_free, matrix);

  return obj;
}

/* Matrix_complex attributes */

static VALUE Matrix_complex_size1(VALUE obj)
{
  gsl_matrix_complex * matrix;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  return INT2NUM(matrix->size1);
}

static VALUE Matrix_complex_size2(VALUE obj)
{
  gsl_matrix_complex * matrix;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  return INT2NUM(matrix->size2);
}

/* Accessing matrix elements */

static VALUE Matrix_complex_get(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * matrix;
  gsl_complex z;
  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  z = gsl_matrix_complex_get(matrix, NUM2INT(i), NUM2INT(j));
  return Complex_new_intern(rbgsl_cComplex, &z);
}

/*
static VALUE Matrix_complex_set(int argc, VALUE * argv, VALUE obj)
{
  gsl_matrix_complex * m;
  size_t size1, size2;
  size_t i, j, k;
  VALUE ary;
  int type0, type1;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  size1 = m->size1;
  size2 = m->size2;

  type0 = TYPE(argv[0]);
  if (type0 == T_ARRAY) {
    type1 = TYPE(argv[1]);
    if (type1 == T_ARRAY) {
      for (i = 0; i < size1; i++) {
        ary = argv[i];
        for (j = 0; j < size2; j++)
          gsl_matrix_complex_set(m, i, j, NUM2DBL(rb_ary_entry(ary, j)));
      }
    } else if (type1 == T_FIXNUM || type1 == T_BIGNUM) {
      ary = argv[0];
      size1 = NUM2INT(argv[1]);
      size2 = NUM2INT(argv[2]);
      k = 0;
      for (i = 0; i < size1; i++) {
        for (j = 0; j < size2; j++, k++)
          gsl_matrix_complex_set(m, i, j, NUM2DBL(rb_ary_entry(ary, k)));
      }
    }
  } else if (type0 == T_FIXNUM || type0 == T_BIGNUM) {
    if (argc != 3)
      rb_raise(rb_eArgError, "wrong number of arguments");
    gsl_matrix_complex_set(m, NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2DBL(argv[2]));
  }

  return obj;
}
*/

static VALUE Matrix_complex_set1(VALUE obj, VALUE vi, VALUE vj, VALUE va)
{
  gsl_matrix_complex * m;
  gsl_complex * pz;
  Data_Get_Struct(va, gsl_complex, pz);
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  gsl_matrix_complex_set(m, NUM2INT(vi), NUM2INT(vj), *pz);
  return obj;
}

/* Initializing matrix elements */
static VALUE Matrix_complex_set_all(VALUE obj, VALUE va)
{
  gsl_matrix_complex * matrix;
  gsl_complex * pz;
  Data_Get_Struct(va, gsl_complex, pz);
  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  gsl_matrix_complex_set_all(matrix, *pz);

  return obj;
}

static VALUE Matrix_complex_set_zero(VALUE obj)
{
  gsl_matrix_complex * matrix;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  gsl_matrix_complex_set_zero(matrix);

  return obj;
}

static VALUE Matrix_complex_set_identity(VALUE obj)
{
  gsl_matrix_complex * matrix;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  gsl_matrix_complex_set_identity(matrix);

  return obj;
}

/* Matrix_complex views */
static VALUE Matrix_complex_submatrix(VALUE obj, VALUE i, VALUE j, VALUE n1, VALUE n2)
{
  gsl_matrix_complex * matrix, * m;
  gsl_matrix_complex_view mv;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  m = gsl_matrix_complex_alloc(NUM2INT(n1), NUM2INT(n2));

  mv = gsl_matrix_complex_submatrix(matrix, NUM2INT(i), NUM2INT(j), NUM2INT(n1), NUM2INT(n2));
  * m = mv.matrix;

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, m);
}

/* Creating row and column views */
static VALUE Matrix_complex_row(VALUE obj, VALUE i)
{
  gsl_matrix_complex * m;
  gsl_vector_complex * v;
  gsl_vector_complex_view vv;

  Data_Get_Struct(obj, gsl_matrix_complex, m);

  vv = gsl_matrix_complex_row(m, NUM2INT(i));
  v = gsl_vector_complex_alloc(m->size2);
  gsl_vector_complex_memcpy(v, &vv.vector);
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, v);
}

static VALUE Matrix_complex_column(VALUE obj, VALUE j)
{
  gsl_matrix_complex * m;
  gsl_vector_complex * v;
  gsl_vector_complex_view vv;

  Data_Get_Struct(obj, gsl_matrix_complex, m);

  vv = gsl_matrix_complex_column(m, NUM2INT(j));
  v = gsl_vector_complex_alloc(m->size1);
  gsl_vector_complex_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, v);
}

/*
static VALUE Matrix_complex_column(VALUE obj, VALUE vj)
{
  gsl_matrix_complex * m;
  gsl_vector_complex * v;
  int i, j, n2;
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  n2 = m->size2;
  j = NUM2INT(vj);
  v = gsl_vector_complex_alloc(m->size1);
  for(i=0; i < n2; i++)
    gsl_vector_complex_set(v, i, gsl_matrix_complex_get(m, i,j));
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, v);
}
*/
static VALUE Matrix_complex_diagonal(VALUE obj)
{
  gsl_matrix_complex * m;
  gsl_vector_complex * v;
  gsl_vector_complex_view vv;

  Data_Get_Struct(obj, gsl_matrix_complex, m);

  vv = gsl_matrix_complex_diagonal(m);
  v = gsl_vector_complex_alloc(vv.vector.size);
  gsl_vector_complex_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, v);
}

static VALUE Matrix_complex_subdiagonal(VALUE obj, VALUE vk)
{
  gsl_matrix_complex * matrix;
  gsl_vector_complex * v;
  gsl_vector_complex_view vv;
  size_t k = NUM2INT(vk);

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);

  vv = gsl_matrix_complex_subdiagonal(matrix, k);
  v = gsl_vector_complex_alloc(vv.vector.size);
  gsl_vector_complex_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, v);
}

static VALUE Matrix_complex_superdiagonal(VALUE obj, VALUE vk)
{
  gsl_matrix_complex * matrix;
  gsl_vector_complex * v;
  gsl_vector_complex_view vv;
  size_t k = NUM2INT(vk);

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);

  vv = gsl_matrix_complex_superdiagonal(matrix, k);
  v = gsl_vector_complex_alloc(vv.vector.size);
  gsl_vector_complex_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, v);
}

/* Copying matrices */
static VALUE Matrix_complex_clone(VALUE obj)
{
  gsl_matrix_complex * matrix, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  new = gsl_matrix_complex_alloc(matrix->size1, matrix->size2);

  gsl_matrix_complex_memcpy(new, matrix);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_swap(VALUE obj, VALUE mm2)
{
  gsl_matrix_complex * matrix, * m2;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  Data_Get_Struct(mm2, gsl_matrix_complex, m2);

  gsl_matrix_complex_swap(m2, matrix);

  return obj;
}

/* Copying rows and columns */
static VALUE Matrix_complex_set_row(VALUE obj, VALUE i, VALUE vv)
{
  gsl_matrix_complex * m;
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_matrix_complex_set_row(m, NUM2INT(i), v);

  return obj;
}

static VALUE Matrix_complex_set_col(VALUE obj, VALUE j, VALUE vv)
{
  gsl_matrix_complex * m;
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_matrix_complex_set_col(m, NUM2INT(j), v);

  return obj;
}

/* Exchanging rows and columns */
static VALUE Matrix_complex_swap_rows(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * m, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  gsl_matrix_complex_swap_rows(new, NUM2INT(i), NUM2INT(j));

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_swap_rows2(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * m;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  gsl_matrix_complex_swap_rows(m, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Matrix_complex_swap_columns(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * m, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  gsl_matrix_complex_swap_columns(new, NUM2INT(i), NUM2INT(j));

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_swap_columns2(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * m;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  gsl_matrix_complex_swap_columns(m, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Matrix_complex_swap_rowcol(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * m, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  gsl_matrix_complex_swap_rowcol(new, NUM2INT(i), NUM2INT(j));

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_swap_rowcol2(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix_complex * m;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  gsl_matrix_complex_swap_rowcol(m, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Matrix_complex_transpose_memcpy(VALUE obj)
{
  gsl_matrix_complex * m, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  new = gsl_matrix_complex_alloc(m->size2, m->size1);

  gsl_matrix_complex_transpose_memcpy(new, m);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_transpose(VALUE obj)
{
  gsl_matrix_complex * m;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  gsl_matrix_complex_transpose(m);

  return obj;
}

/* Hermitian conjugation; not in GSL */
static VALUE Matrix_complex_herm(VALUE obj)
{
  gsl_matrix_complex * m, * new;
  gsl_complex z;
  int n1, n2, i, j;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  n1 = m->size1;
  n2 = m->size2;
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  for(i = 0; i < n1; i++)
    for(j = 0; j < n2; j++) {
      z = gsl_matrix_complex_get(m, i, j);
      gsl_matrix_complex_set(new, j, i, gsl_complex_conjugate(z));
    }
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

/* Matrix_complex properties */
static VALUE Matrix_complex_isnull(VALUE obj)
{
  gsl_matrix_complex * m;
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  return INT2FIX(gsl_matrix_complex_isnull(m));
}

/* Matrix_complex operations */
static VALUE Matrix_complex_add(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * a, * b, * new;
  int n1, n2;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  n1 = a->size1;
  n2 = a->size2;
  new = gsl_matrix_complex_alloc(n1, n2);
  gsl_matrix_complex_memcpy(new, a);
  gsl_matrix_complex_add(new, b);
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_add_bang(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * a, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  gsl_matrix_complex_add(a, b);
  return obj;
}

static VALUE Matrix_complex_sub(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * a, * b, * new;
  int n1, n2;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  n1 = a->size1;
  n2 = a->size2;
  new = gsl_matrix_complex_alloc(n1, n2);
  gsl_matrix_complex_memcpy(new, a);
  gsl_matrix_complex_sub(new, b);
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_sub_bang(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * a, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  gsl_matrix_complex_sub(a, b);
  return obj;
}

static VALUE Matrix_complex_equal(VALUE obj, VALUE bb)
{
  gsl_matrix_complex *m, *b;
  size_t na1, na2, nb1, nb2;
  size_t i, j;
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  na1 = m->size1;
  na2 = m->size2;
  nb1 = b->size1;
  nb2 = b->size2;
  if(na1 != nb1 || na2 != nb2)
    return Qfalse;

  for (i = 0; i < na1; i++) {
    for (j = 0; j < na2; j++){
      gsl_complex za, zb;
      za = gsl_matrix_complex_get(m, i, j);
      zb = gsl_matrix_complex_get(b, i, j);

      if(GSL_REAL(za) != GSL_REAL(zb) || GSL_IMAG(za) != GSL_IMAG(zb))
        return Qfalse;
     }
  }
  return Qtrue;
}

static VALUE Matrix_complex_float_equal(VALUE self, VALUE aa, VALUE bb, VALUE eps) {
  gsl_matrix_complex *m, *b;
  gsl_complex z, z1, z2;
  size_t na1, na2, nb1, nb2;
  size_t i, j;
  double d1 = NUM2DBL(eps);
  Data_Get_Struct(aa, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  na1 = m->size1;
  na2 = m->size2;
  nb1 = b->size1;
  nb2 = b->size2;
  if(na1 != nb1 || na2 != nb2)
    return Qfalse;

  for (i = 0; i < na1; i++) {
    for (j = 0; j < na2; j++){
      z1 = gsl_matrix_complex_get(m, i, j);
      z2 = gsl_matrix_complex_get(b, i, j);
      z = gsl_complex_sub(z1, z2);
      if(GSL_REAL(z) >= d1 || GSL_IMAG(z) >= d1)
        return Qfalse;
     }
  }
  return Qtrue;
}

static VALUE Matrix_complex_mul_elements(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * m, * b, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  gsl_matrix_complex_mul_elements(new, b);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_mul_elements2(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * m, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  gsl_matrix_complex_mul_elements(m, b);
  return obj;
}

static VALUE Matrix_complex_div_elements(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * m, * b, * new;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  gsl_matrix_complex_div_elements(new, b);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_div_elements2(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * m, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  gsl_matrix_complex_div_elements(m, b);
  return obj;
}

static VALUE Matrix_complex_scale(VALUE obj, VALUE vz)
{
  gsl_matrix_complex * m, * new;
  gsl_complex * pz;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(vz, gsl_complex, pz);

  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  gsl_matrix_complex_scale(new, *pz);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_scale2(VALUE obj, VALUE vz)
{
  gsl_matrix_complex * m;
  gsl_complex * pz;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(vz, gsl_complex, pz);

  gsl_matrix_complex_scale(m, *pz);
  return obj;
}

/*
static VALUE Matrix_complex_mul(VALUE obj, VALUE x)
{
  gsl_matrix_complex * m, * b, * new;
  int type;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  new = gsl_matrix_complex_alloc(m->size1, m->size2);
  gsl_matrix_complex_memcpy(new, m);

  type = TYPE(x);
  if (type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT)
    gsl_matrix_complex_scale(new, NUM2DBL(x));
  else {
    Data_Get_Struct(x, gsl_matrix_complex, b);
    gsl_matrix_complex_mul_elements(new, b);
  }
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

static VALUE Matrix_complex_mul2(VALUE obj, VALUE x)
{
  gsl_matrix_complex * m, * b;
  int type;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  type = TYPE(x);
  if (type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT)
    gsl_matrix_complex_scale(m, NUM2DBL(x));
  else {
    Data_Get_Struct(x, gsl_matrix_complex, b);
    gsl_matrix_complex_mul_elements(m, b);
  }
  return obj;
}
*/

/* Reading and writing matrices */

static VALUE Matrix_complex_fwrite(VALUE obj, VALUE io)
{
  gsl_matrix_complex * h;
  OpenFile * fptr;
  FILE * f;
  int status;
  Data_Get_Struct(obj, gsl_matrix_complex, h);
  Check_Type(io, T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_matrix_complex_fwrite(f, h);
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_complex_fread(VALUE obj, VALUE io)
{
  gsl_matrix_complex * h;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_matrix_complex, h);
  Check_Type(io, T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_matrix_complex_fread(f, h);
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_complex_fprintf(VALUE obj, VALUE io, VALUE format)
{
  gsl_matrix_complex * h;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_matrix_complex, h);
  Check_Type(io, T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_matrix_complex_fprintf(f, h, STR2CSTR(format));
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_complex_printf(VALUE obj, VALUE format)
{
  gsl_matrix_complex * h;
  int status;

  Data_Get_Struct(obj, gsl_matrix_complex, h);
  status = gsl_matrix_complex_fprintf(stdout, h, STR2CSTR(format));
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to stdout.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_complex_fscanf(VALUE obj, VALUE io)
{
  gsl_matrix_complex * h;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_matrix_complex, h);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_matrix_complex_fscanf(f, h);
  return INT2FIX(status);
}

/* Ruby convenience methods */
static VALUE Matrix_complex_print(VALUE obj) {
  gsl_matrix_complex * matrix;
  size_t n1, n2;
  size_t i, j;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  n1 = matrix->size1;
  n2 = matrix->size2;
  printf("[ ");
  for (i = 0; i < n1; i++) {
    if (i != 0)
      printf("  ");
    for (j = 0; j < n2; j++) {
      gsl_complex z;
      z = gsl_matrix_complex_get(matrix, i, j);
      printf("(%g, %g)  ", GSL_REAL(z),GSL_IMAG(z));
    }
    if (i == n1 - 1)
      printf("]\n");
    else
      printf("\n");
  }

  return obj;
}

/*
static VALUE Matrix_complex_to_a(VALUE obj) {
  VALUE ary;
  gsl_matrix_complex * matrix;
  size_t i;

  Data_Get_Struct(obj, gsl_matrix_complex, matrix);

  ary = rb_ary_new();
  for (i = 0; i < matrix->size1; i++)
    rb_ary_push(ary, Matrix_complex_row(obj, INT2NUM(i)));

  return ary;
}

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
static VALUE Matrix_complex_allocate(VALUE kl) {
  return Matrix_complex_calloc(kl, INT2NUM(1), INT2NUM(1));
}
#endif

static VALUE Matrix_complex_dump_data(VALUE obj) {
  return Matrix_complex_to_a(obj);
}

static VALUE Matrix_complex_load_data(VALUE obj, VALUE ary) {
  gsl_matrix_complex *matrix;
  gsl_vector *vector;
  VALUE firstVec;
  int i;
  Data_Get_Struct(obj, gsl_matrix_complex, matrix);
  if (matrix->owner) {
    gsl_block_free(matrix->block);
  }
  matrix->size1 = RARRAY(ary)->len;
  firstVec = rb_ary_entry(ary, 0);
  Data_Get_Struct(firstVec, gsl_vector, vector);
  matrix->size2 = vector->size;
  matrix->tda = matrix->size2;
  matrix->block = gsl_block_alloc(matrix->size1 * matrix->size2);
  matrix->owner = 1;
  matrix->data = matrix->block->data;
  for (i = 0; i < matrix->size1; ++i) {
    Matrix_complex_set_row(obj, INT2NUM(i), rb_ary_entry(ary, i));
  }
  return Qtrue;
}
*/

static VALUE Matrix_complex_inspect(VALUE obj) {
  Matrix_complex_print(obj);
  return obj;
}

/* BLAS Support */
/* gsl_matrix_complex.rb in the contrib directory makes easier to use a few
 * matrix operations; for example
 * m1.mult(m2).scale(2) + m3.scale(3)
 * is equivalent to
 * m1 * m2 * 2 + m3 * 3
 * m1.mult_vect(v1).scale(2) + v2.scale(3)
 * is equivalent to
 * m1 * v1 * 2 + v2 * 3
 * The previous forms are slower than the corresponding Blas ones
 * m1.dgemm(false, false,2,m2,3,m3)
 * m1.dgemv(false, v1, 2, 3, v2)
 */

#include <gsl/gsl_blas.h>
static VALUE Matrix_complex_mult(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * a, * b, * c;
  gsl_complex z0, z1;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  if (a->size2 != b->size1)
        rb_raise(rb_eIOError, "Matrix_complex product not allowed");
  c = gsl_matrix_complex_alloc(a->size1, b->size2);
  GSL_SET_COMPLEX(&z0, 0, 0);
  GSL_SET_COMPLEX(&z1, 1, 0);
  gsl_blas_zgemm (CblasNoTrans, CblasNoTrans, z1, a, b, z0, c);
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, c);
}

static VALUE Matrix_complex_mult_vect(VALUE obj, VALUE bb)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * b, * c;
  gsl_complex z0, z1;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  if (a->size2 != b->size)
    rb_raise(rb_eIOError, "mult_vect: Matrix_complex product not allowed");
  c = gsl_vector_complex_alloc(a->size1);
  GSL_SET_COMPLEX(&z0, 0, 0);
  GSL_SET_COMPLEX(&z1, 1, 0);
  gsl_blas_zgemv(CblasNoTrans,z1, a, b, z0, c);
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, c);
}

static VALUE Matrix_complex_negative(VALUE obj)
{
  int i, j, n1, n2;
  gsl_matrix_complex * a, * new;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  n1 = a->size1;
  n2 = a->size2;
  new = gsl_matrix_complex_alloc(n1, n2);
  gsl_matrix_complex_memcpy(new, a);
  for(i = 0; i < n1; i++)
    for(j = 0; j < n2; j++) {
      gsl_complex z;
      z = gsl_matrix_complex_get(a, i,j);
      z = gsl_complex_negative(z);
      gsl_matrix_complex_set(new,i,j, z);
    }
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

/* Utilities */
static VALUE Matrix_complex_upper(VALUE obj, VALUE vdiag)
{
  int i,j,n,n2;
  gsl_matrix_complex * a, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
    b = gsl_matrix_complex_calloc(a->size1, b->size2);
    n = (a->size1 <= a->size2)? a->size1: a->size2;
    n2 = b->size2;
  for (i = 0; i < n; ++i) {
    for(j = i+1; j < n2; j++) {
      gsl_matrix_complex_set(b,i,j, gsl_matrix_complex_get(a,i,j));
    }
  }
  if (vdiag == Qtrue)
    for (i = 0; i < n; ++i)
      gsl_matrix_complex_set(b,i,i, gsl_matrix_complex_get(a,i,i));
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, b);
}

static VALUE Matrix_complex_lower(VALUE obj, VALUE vdiag)
{
  size_t i, j, n;
  gsl_matrix_complex * a, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
    b = gsl_matrix_complex_calloc(a->size1, b->size2);
    n = (a->size1 <= a->size2)? a->size1: a->size2;
  for (i = 0; i < a->size1; ++i) {
    for(j = 0; j < i; j++) {
      gsl_matrix_complex_set(b,i,j, gsl_matrix_complex_get(a,i,j));
    }
  }
  if (vdiag == Qtrue)
    for (i = 0; i < n; ++i)
      gsl_matrix_complex_set(b,i,i, gsl_matrix_complex_get(a,i,i));
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, b);
}

static VALUE Matrix_to_complex(VALUE obj)
{
  int i, j, n1, n2;
  gsl_matrix * a;
  gsl_matrix_complex * b;
  Data_Get_Struct(obj, gsl_matrix, a);
  n1 = a->size1;
  n2 = a->size2;
  b = gsl_matrix_complex_alloc(a->size1, a->size2);
  for(i=0; i < n1; i++)
    for(j=0; j < n2; j++) {
      gsl_complex z;
      GSL_SET_COMPLEX (&z, gsl_matrix_get(a,i,j), 0);
      gsl_matrix_complex_set(b,i,j,z);
    }
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, b);}

/* Permutation matrix */
/* Golub, Loan p109
 * Let p(k) be the column index of the sole "1" in the k-th row
 * P(k,p(k)) = 1; P(k,j) = 0 for j != p(k)
 */

/*
static VALUE Matrix_complex_perm(VALUE self, VALUE p1)
{
  int i,n;
  gsl_permutation *p;
  gsl_matrix_complex * a;
  Data_Get_Struct(p1, gsl_permutation, p);
  n = p->size;
  a = gsl_matrix_complex_calloc(n, n);
  for(i = 0; i < n; i++)
       gsl_matrix_complex_set(a,i,p->data[i],1);
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, a);
}
*/

/* Class definition */

VALUE rbgsl_cMatrix_complex;

void Init_Matrix_complex() {
  rbgsl_cMatrix_complex = rb_define_class_under(rbgsl_mGSL, "Matrix_complex", rb_cObject);

  rb_define_singleton_method(rbgsl_cMatrix_complex, "new", Matrix_complex_new, -1);
  rb_define_singleton_method(rbgsl_cMatrix_complex, "alloc", Matrix_complex_new, -1);
  rb_define_singleton_method(rbgsl_cMatrix_complex, "calloc", Matrix_complex_calloc, 2);

  rb_define_method(rbgsl_cMatrix_complex, "size1", Matrix_complex_size1, 0);
  rb_define_method(rbgsl_cMatrix_complex, "size2", Matrix_complex_size2, 0);
  rb_define_alias(rbgsl_cMatrix_complex, "size", "size1");

  rb_define_method(rbgsl_cMatrix_complex, "get", Matrix_complex_get, 2);
  rb_define_alias(rbgsl_cMatrix_complex, "[]", "get");
  rb_define_method(rbgsl_cMatrix_complex, "set1", Matrix_complex_set1, 3);
   rb_define_alias(rbgsl_cMatrix_complex, "[]=", "set1");
/*
  rb_define_method(rbgsl_cMatrix_complex, "set", Matrix_complex_set, -1);
*/
  rb_define_method(rbgsl_cMatrix_complex, "set_all", Matrix_complex_set_all, 1);
  rb_define_method(rbgsl_cMatrix_complex, "set_zero", Matrix_complex_set_zero, 0);
  rb_define_method(rbgsl_cMatrix_complex, "set_identity", Matrix_complex_set_identity, 0);
  rb_define_method(rbgsl_cMatrix_complex, "submatrix", Matrix_complex_submatrix, 4);
  rb_define_method(rbgsl_cMatrix_complex, "row", Matrix_complex_row, 1);
  rb_define_alias(rbgsl_cMatrix_complex, "get_row", "row");
  rb_define_alias(rbgsl_cMatrix_complex, "[]", "row");
  rb_define_method(rbgsl_cMatrix_complex, "column", Matrix_complex_column, 1);
  rb_define_alias(rbgsl_cMatrix_complex, "col", "column");
  rb_define_alias(rbgsl_cMatrix_complex, "get_column", "column");
  rb_define_alias(rbgsl_cMatrix_complex, "get_col", "column");

  rb_define_method(rbgsl_cMatrix_complex, "diagonal", Matrix_complex_diagonal, 0);
  rb_define_method(rbgsl_cMatrix_complex, "subdiagonal", Matrix_complex_subdiagonal, 1);
  rb_define_method(rbgsl_cMatrix_complex, "superdiagonal", Matrix_complex_superdiagonal, 1);
  rb_define_method(rbgsl_cMatrix_complex, "clone", Matrix_complex_clone, 0);
  rb_define_method(rbgsl_cMatrix_complex, "swap", Matrix_complex_swap, 1);
  rb_define_method(rbgsl_cMatrix_complex, "set_row", Matrix_complex_set_row, 2);
  rb_define_method(rbgsl_cMatrix_complex, "set_col", Matrix_complex_set_col, 2);
  rb_define_method(rbgsl_cMatrix_complex, "swap_rows", Matrix_complex_swap_rows, 2);
  rb_define_method(rbgsl_cMatrix_complex, "swap_rows!", Matrix_complex_swap_rows2, 2);
  rb_define_method(rbgsl_cMatrix_complex, "swap_columns", Matrix_complex_swap_columns, 2);
  rb_define_alias(rbgsl_cMatrix_complex, "swap_cols", "swap_columns");
  rb_define_method(rbgsl_cMatrix_complex, "swap_columns!", Matrix_complex_swap_columns2, 2);
  rb_define_alias(rbgsl_cMatrix_complex, "swap_cols!", "swap_columns!");
  rb_define_method(rbgsl_cMatrix_complex, "swap_rowcol", Matrix_complex_swap_rowcol, 2);
  rb_define_method(rbgsl_cMatrix_complex, "swap_rowcol!", Matrix_complex_swap_rowcol2, 2);
  rb_define_method(rbgsl_cMatrix_complex, "transpose_memcpy", Matrix_complex_transpose_memcpy, 0);
  rb_define_alias(rbgsl_cMatrix_complex, "transpose", "transpose_memcpy");
  rb_define_alias(rbgsl_cMatrix_complex, "t", "transpose_memcpy");
  rb_define_method(rbgsl_cMatrix_complex, "transpose!", Matrix_complex_transpose, 0);
  rb_define_method(rbgsl_cMatrix_complex, "herm", Matrix_complex_herm, 0);
  rb_define_alias(rbgsl_cMatrix_complex, "h", "herm");
  rb_define_method(rbgsl_cMatrix_complex, "isnull", Matrix_complex_isnull, 0);
  rb_define_method(rbgsl_cMatrix_complex, "add", Matrix_complex_add, 1);
  rb_define_alias(rbgsl_cMatrix_complex, "+", "add");
  rb_define_method(rbgsl_cMatrix_complex, "add!", Matrix_complex_add_bang, 1);
  rb_define_method(rbgsl_cMatrix_complex, "sub", Matrix_complex_sub, 1);
  rb_define_alias(rbgsl_cMatrix_complex, "-", "sub");
  rb_define_method(rbgsl_cMatrix_complex, "sub!", Matrix_complex_sub_bang, 1);
  rb_define_method(rbgsl_cMatrix_complex, "mul_elements", Matrix_complex_mul_elements, 1);
  rb_define_method(rbgsl_cMatrix_complex, "mul_elements!", Matrix_complex_mul_elements2, 1);
  /*
  rb_define_method(rbgsl_cMatrix_complex, "mul", Matrix_complex_mul, 1);
  rb_define_method(rbgsl_cMatrix_complex, "mul!", Matrix_complex_mul2, 1);
  */
  rb_define_method(rbgsl_cMatrix_complex, "div_elements", Matrix_complex_div_elements, 1);

  rb_define_method(rbgsl_cMatrix_complex, "div_elements!", Matrix_complex_div_elements2, 1);
  rb_define_method(rbgsl_cMatrix_complex, "scale", Matrix_complex_scale, 1);
  rb_define_method(rbgsl_cMatrix_complex, "scale!", Matrix_complex_scale2, 1);
  rb_define_method(rbgsl_cMatrix_complex, "==", Matrix_complex_equal, 1);
  rb_define_singleton_method(rbgsl_cMatrix_complex, "float_equal", Matrix_complex_float_equal, 3);

  rb_define_method(rbgsl_cMatrix_complex, "fwrite", Matrix_complex_fwrite, 1);
  rb_define_method(rbgsl_cMatrix_complex, "fread", Matrix_complex_fread, 1);
  rb_define_method(rbgsl_cMatrix_complex, "fprintf", Matrix_complex_fprintf, 2);
  rb_define_method(rbgsl_cMatrix_complex, "printf", Matrix_complex_printf, 1);

  rb_define_method(rbgsl_cMatrix_complex, "fscanf", Matrix_complex_fscanf, 1);
  rb_define_method(rbgsl_cMatrix_complex, "print", Matrix_complex_print, 0);
  rb_define_method(rbgsl_cMatrix_complex, "inspect", Matrix_complex_inspect, 0);
  /*
  rb_define_method(rbgsl_cMatrix_complex, "to_a", Matrix_complex_to_a, 0);

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
  rb_define_alloc_func(rbgsl_cMatrix_complex, Matrix_complex_allocate);
#endif

  rb_define_method(rbgsl_cMatrix_complex, "_load_data", Matrix_complex_load_data, 1);
  rb_define_method(rbgsl_cMatrix_complex, "_dump_data", Matrix_complex_dump_data, 0);
*/

   rb_define_method(rbgsl_cMatrix, "to_complex", Matrix_to_complex, 0);
  /* blas */
  rb_define_method(rbgsl_cMatrix_complex, "mult", Matrix_complex_mult, 1);
  rb_define_method(rbgsl_cMatrix_complex, "mult_vect", Matrix_complex_mult_vect, 1);
  rb_define_method(rbgsl_cMatrix_complex, "negative", Matrix_complex_negative, 0);
  rb_define_alias(rbgsl_cMatrix_complex, "-@", "negative");

  rb_define_method(rbgsl_cMatrix_complex, "upper", Matrix_complex_upper, 1);
  rb_define_method(rbgsl_cMatrix_complex, "lower", Matrix_complex_lower, 1);

  /*
  rb_define_singleton_method(rbgsl_cMatrix_complex, "perm", Matrix_complex_perm, 1);
  */
  Init_Blas_complex();
  Init_Linalg_complex();
  Init_Eigen_complex();

}

/* vim: set ts=4: */
