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

/* $Id: Matrix.c,v 1.13 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <math.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_matrix.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Internal use only */

static gsl_matrix * alloc_matrix_from_array_sizes(VALUE ary, VALUE nn1,
  VALUE nn2);
static gsl_matrix * alloc_matrix_from_arrays(int argc, VALUE * argv);


/* Given a variable-length argument list consisting of 1 or more
 * single-dimensional arrays (not nested), create a matrix with
 * size1 == argc and size2 == RARRAY(argv[0])->len
 */
static gsl_matrix * alloc_matrix_from_arrays(int argc, VALUE * argv) {
  int n1, n2, i, j;
  gsl_matrix * matrix;
  VALUE ary;

  n1 = argc;
  n2 = RARRAY(argv[0])->len;
  matrix = gsl_matrix_alloc(n1, n2);
  for (i = 0; i < n1; i++) {
    ary = argv[i];
    for (j = 0; j < n2; j++)
      gsl_matrix_set(matrix, i, j, NUM2DBL(rb_ary_entry(ary, j)));
  }
  return matrix;
}

/* Given a 2-dimensional array and an explicit width and height,
 * create a matrix using elements ary[0...nn1][0...nn2]
 */
static gsl_matrix * alloc_matrix_from_array_sizes(VALUE ary, VALUE nn1,
  VALUE nn2) {
  size_t n1, n2;
  gsl_matrix * matrix;
  size_t i, j, k;

  n1 = NUM2INT(nn1);
  n2 = NUM2INT(nn2);
  matrix = gsl_matrix_alloc(n1, n2);
  k = 0;
  for (i = 0; i < n1; i++) {
    for (j = 0; j < n2; j++, k++)
      gsl_matrix_set(matrix, i, j, NUM2DBL(rb_ary_entry(ary, k)));
  }
  return matrix;
}

/* Matrices */

static VALUE Matrix_new(int argc, VALUE * argv, VALUE klass)
{
  VALUE ary;
  gsl_matrix * matrix = NULL;
  int type0, type1;

  type0 = TYPE(argv[0]);

  if (type0 == T_FIXNUM || type0 == T_BIGNUM) {
    matrix = gsl_matrix_alloc(NUM2INT(argv[0]), NUM2INT(argv[1]));
  } else if (type0 == T_ARRAY) {
    ary = argv[0];
    type1 = TYPE(argv[1]);
    if (type1 == T_ARRAY) {
      matrix = alloc_matrix_from_arrays(argc, argv);
    } else if (type1 == T_FIXNUM || type1 == T_BIGNUM) {
      matrix = alloc_matrix_from_array_sizes(argv[0], argv[1], argv[2]);
    }
    
  }
  
  return Data_Wrap_Struct(klass, 0, gsl_matrix_free, matrix);
}

/* Matrix allocation */

static VALUE Matrix_calloc(VALUE klass, VALUE n1, VALUE n2)
{
  VALUE obj;
  gsl_matrix * matrix;

  matrix = gsl_matrix_calloc(NUM2INT(n1), NUM2INT(n2));

  obj = Data_Wrap_Struct(klass, 0, gsl_matrix_free, matrix);

  return obj;
}

/* Matrix attributes */

static VALUE Matrix_size1(VALUE obj)
{
  gsl_matrix * matrix;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  return INT2NUM(matrix->size1);
}

static VALUE Matrix_size2(VALUE obj)
{
  gsl_matrix * matrix;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  return INT2NUM(matrix->size2);
}

/* Accessing matrix elements */

static VALUE Matrix_get(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * matrix;

  Data_Get_Struct(obj, gsl_matrix, matrix);

  return rb_float_new(gsl_matrix_get(matrix, NUM2INT(i), NUM2INT(j)));
}

static VALUE Matrix_set(int argc, VALUE * argv, VALUE obj)
{
  gsl_matrix * m;
  size_t size1, size2;
  size_t i, j, k;
  VALUE ary;
  int type0, type1;

  Data_Get_Struct(obj, gsl_matrix, m);
  size1 = m->size1;
  size2 = m->size2;

  type0 = TYPE(argv[0]);
  if (type0 == T_ARRAY) {
    type1 = TYPE(argv[1]);
    if (type1 == T_ARRAY) {
      for (i = 0; i < size1; i++) {
        ary = argv[i];
        for (j = 0; j < size2; j++)
          gsl_matrix_set(m, i, j, NUM2DBL(rb_ary_entry(ary, j)));
      }
    } else if (type1 == T_FIXNUM || type1 == T_BIGNUM) {
      ary = argv[0];
      size1 = NUM2INT(argv[1]);
      size2 = NUM2INT(argv[2]);
      k = 0;
      for (i = 0; i < size1; i++) {
        for (j = 0; j < size2; j++, k++)
          gsl_matrix_set(m, i, j, NUM2DBL(rb_ary_entry(ary, k)));
      }
    }
  } else if (type0 == T_FIXNUM || type0 == T_BIGNUM) {
    if (argc != 3)
      rb_raise(rb_eArgError, "wrong number of arguments");
    gsl_matrix_set(m, NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2DBL(argv[2]));
  }

  return obj;
}

/* Initializing matrix elements */

static VALUE Matrix_set_all(VALUE obj, VALUE x)
{
  gsl_matrix * matrix;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  gsl_matrix_set_all(matrix, NUM2DBL(x));

  return obj;
}

static VALUE Matrix_set_zero(VALUE obj)
{
  gsl_matrix * matrix;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  gsl_matrix_set_zero(matrix);

  return obj;
}

static VALUE Matrix_set_identity(VALUE obj)
{
  gsl_matrix * matrix;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  gsl_matrix_set_identity(matrix);

  return obj;
}

/* Matrix views */

static VALUE Matrix_submatrix(VALUE obj, VALUE i, VALUE j, VALUE n1, VALUE n2)
{
  gsl_matrix * matrix, * m;
  gsl_matrix_view mv;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  m = gsl_matrix_alloc(NUM2INT(n1), NUM2INT(n2));

  mv = gsl_matrix_submatrix(matrix, NUM2INT(i), NUM2INT(j), NUM2INT(n1), NUM2INT(n2));
  * m = mv.matrix;

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, m);
}

/* Creating row and column views */

static VALUE Matrix_row(VALUE obj, VALUE i)
{
  gsl_matrix * matrix;
  gsl_vector * v;
  gsl_vector_view vv;

  Data_Get_Struct(obj, gsl_matrix, matrix);

  vv = gsl_matrix_row(matrix, NUM2INT(i));
  v = gsl_vector_alloc(matrix->size2);
  gsl_vector_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE Matrix_column(VALUE obj, VALUE j)
{
  gsl_matrix * matrix;
  gsl_vector * v;
  gsl_vector_view vv;

  Data_Get_Struct(obj, gsl_matrix, matrix);

  vv = gsl_matrix_column(matrix, NUM2INT(j));
  v = gsl_vector_alloc(matrix->size1);
  gsl_vector_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE Matrix_diagonal(VALUE obj)
{
  gsl_matrix * matrix;
  gsl_vector * v;
  gsl_vector_view vv;
  
  Data_Get_Struct(obj, gsl_matrix, matrix);
  
  vv = gsl_matrix_diagonal(matrix);
  v = gsl_vector_alloc(vv.vector.size);
  gsl_vector_memcpy(v, &vv.vector);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

  
static VALUE Matrix_subdiagonal(VALUE obj, VALUE vk)
{
  gsl_matrix * matrix;
  gsl_vector * v;
  gsl_vector_view vv;
  size_t k = NUM2INT(vk);
  
  Data_Get_Struct(obj, gsl_matrix, matrix);
  
  vv = gsl_matrix_subdiagonal(matrix, k);
  v = gsl_vector_alloc(vv.vector.size);
  gsl_vector_memcpy(v, &vv.vector);
  
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}


static VALUE Matrix_superdiagonal(VALUE obj, VALUE vk)
{
  gsl_matrix * matrix;
  gsl_vector * v;
  gsl_vector_view vv;
  size_t k = NUM2INT(vk);
  
  Data_Get_Struct(obj, gsl_matrix, matrix);
  
  vv = gsl_matrix_superdiagonal(matrix, k);
  v = gsl_vector_alloc(vv.vector.size);
  gsl_vector_memcpy(v, &vv.vector);
  
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}
	
/* Copying matrices */

static VALUE Matrix_clone(VALUE obj)
{
  gsl_matrix * matrix, * new;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  new = gsl_matrix_alloc(matrix->size1, matrix->size2);

  gsl_matrix_memcpy(new, matrix);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_swap(VALUE obj, VALUE mm2)
{
  gsl_matrix * matrix, * m2;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  Data_Get_Struct(mm2, gsl_matrix, m2);

  gsl_matrix_swap(m2, matrix);

  return obj;
}

/* Copying rows and columns */

static VALUE Matrix_set_row(VALUE obj, VALUE i, VALUE vv)
{
  gsl_matrix * m;
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(vv, gsl_vector, v);

  gsl_matrix_set_row(m, NUM2INT(i), v);

  return obj;
}

static VALUE Matrix_set_col(VALUE obj, VALUE j, VALUE vv)
{
  gsl_matrix * m;
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(vv, gsl_vector, v);

  gsl_matrix_set_col(m, NUM2INT(j), v);

  return obj;
}

/* Exchanging rows and columns */

static VALUE Matrix_swap_rows(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * m, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_swap_rows(new, NUM2INT(i), NUM2INT(j));

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_swap_rows2(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * m;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_swap_rows(m, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Matrix_swap_columns(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * m, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_swap_columns(new, NUM2INT(i), NUM2INT(j));

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_swap_columns2(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * m;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_swap_columns(m, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Matrix_swap_rowcol(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * m, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_swap_rowcol(new, NUM2INT(i), NUM2INT(j));

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_swap_rowcol2(VALUE obj, VALUE i, VALUE j)
{
  gsl_matrix * m;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_swap_rowcol(m, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Matrix_transpose_memcpy(VALUE obj)
{
  gsl_matrix * m, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size2, m->size1);

  gsl_matrix_transpose_memcpy(new, m);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_transpose(VALUE obj)
{
  gsl_matrix * m;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_transpose(m);

  return obj;
}

/* Matrix properties */

static VALUE Matrix_isnull(VALUE obj)
{
  gsl_matrix * m;
  Data_Get_Struct(obj, gsl_matrix, m);
  return INT2FIX(gsl_matrix_isnull(m));
}

/* Matrix operations */

static VALUE Matrix_add(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b, * new;
  int type;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  type = TYPE(bb);
  if (type == T_FIXNUM || type == T_BIGNUM) {
    gsl_matrix_add_constant(new, NUM2DBL(bb));
  } else {
    Data_Get_Struct(bb, gsl_matrix, b);
    gsl_matrix_add(new, b);
  }

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_equal(VALUE obj, VALUE bb)
{
  gsl_matrix *m, *b;
  size_t na1, na2, nb1, nb2;
  size_t i, j;
  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_matrix, b);
  na1 = m->size1;
  na2 = m->size2;
  nb1 = b->size1;
  nb2 = b->size2;
  if(na1 != nb1 || na2 != nb2)
	return Qfalse;
 
  for (i = 0; i < na1; i++) {
    for (j = 0; j < na2; j++){
	  if(gsl_matrix_get(m, i, j) != gsl_matrix_get(b, i, j))
		return Qfalse;
     } 
  }
  return Qtrue;
}

  
static VALUE Matrix_float_equal(VALUE self, VALUE aa, VALUE bb, VALUE eps) {
  gsl_matrix *m, *b;
  size_t na1, na2, nb1, nb2;
  size_t i, j;
  double d1 = NUM2DBL(eps);
  Data_Get_Struct(aa, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_matrix, b);
  na1 = m->size1;
  na2 = m->size2;
  nb1 = b->size1;
  nb2 = b->size2;
  if(na1 != nb1 || na2 != nb2)
	return Qfalse;
 
  for (i = 0; i < na1; i++) {
    for (j = 0; j < na2; j++){
	  if(fabs(gsl_matrix_get(m, i, j) - gsl_matrix_get(b, i, j)) >= d1)
		return Qfalse;
     } 
  }
  return Qtrue;
}

static VALUE Matrix_add2(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b;
  int type;

  Data_Get_Struct(obj, gsl_matrix, m);
  type = TYPE(bb);
  if (type == T_FIXNUM || type == T_BIGNUM) {
    gsl_matrix_add_constant(m, NUM2DBL(bb));
  } else {
    Data_Get_Struct(bb, gsl_matrix, b);
    gsl_matrix_add(m, b);
  }
  return obj;
}

static VALUE Matrix_sub(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b, * new;
  int type;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  type = TYPE(bb);
  if (type == T_FIXNUM || type == T_BIGNUM) {
    gsl_matrix_add_constant(new, -NUM2DBL(bb));
  } else {
    Data_Get_Struct(bb, gsl_matrix, b);
    gsl_matrix_sub(new, b);
  }

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_sub2(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b;
  int type;

  Data_Get_Struct(obj, gsl_matrix, m);
  type = TYPE(bb);
  if (type == T_FIXNUM || type == T_BIGNUM) {
    gsl_matrix_add_constant(m, -NUM2DBL(bb));
  } else {
    Data_Get_Struct(bb, gsl_matrix, b);
    gsl_matrix_sub(m, b);
  }
  return obj;
}

static VALUE Matrix_mul_elements(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_matrix, b);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_mul_elements(new, b);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_mul_elements2(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b;
  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_matrix, b);
  gsl_matrix_mul_elements(m, b);
  return obj;
}

  
  
static VALUE Matrix_div_elements(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_matrix, b);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_div_elements(new, b);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_div_elements2(VALUE obj, VALUE bb)
{
  gsl_matrix * m, * b;
  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_matrix, b);
  gsl_matrix_div_elements(m, b);
  return obj;
}

static VALUE Matrix_scale(VALUE obj, VALUE x)
{
  gsl_matrix * m, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_scale(new, NUM2DBL(x));

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_scale2(VALUE obj, VALUE x)
{
  gsl_matrix * m;
  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_scale(m, NUM2DBL(x));
  return obj;
}

static VALUE Matrix_add_constant(VALUE obj, VALUE x)
{
  gsl_matrix * m, * new;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  gsl_matrix_add_constant(new, NUM2DBL(x));

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_add_constant2(VALUE obj, VALUE x)
{
  gsl_matrix * m;
  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_add_constant(m, NUM2DBL(x));
  return obj;
}

static VALUE Matrix_mul(VALUE obj, VALUE x)
{
  gsl_matrix * m, * b, * new;
  int type;

  Data_Get_Struct(obj, gsl_matrix, m);
  new = gsl_matrix_alloc(m->size1, m->size2);
  gsl_matrix_memcpy(new, m);

  type = TYPE(x);
  if (type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT)
    gsl_matrix_scale(new, NUM2DBL(x));
  else {
    Data_Get_Struct(x, gsl_matrix, b);
    gsl_matrix_mul_elements(new, b);
  }
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

static VALUE Matrix_mul2(VALUE obj, VALUE x)
{
  gsl_matrix * m, * b;
  int type;

  Data_Get_Struct(obj, gsl_matrix, m);
  type = TYPE(x);
  if (type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT)
    gsl_matrix_scale(m, NUM2DBL(x));
  else {
    Data_Get_Struct(x, gsl_matrix, b);
    gsl_matrix_mul_elements(m, b);
  }
  return obj;
}

/* Finding maximum and minimum elements of matrices */

static VALUE Matrix_max(VALUE obj)
{
  gsl_matrix * m;
  Data_Get_Struct(obj, gsl_matrix, m);
  return rb_float_new(gsl_matrix_max(m));
}

static VALUE Matrix_min(VALUE obj)
{
  gsl_matrix * m;
  Data_Get_Struct(obj, gsl_matrix, m);
  return rb_float_new(gsl_matrix_min(m));
}

static VALUE Matrix_minmax(VALUE obj)
{
  gsl_matrix * m;
  double min, max;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_minmax(m, &min, &max);
  return rb_ary_new3(2, rb_float_new(min), rb_float_new(max));
}

static VALUE Matrix_max_index(VALUE obj)
{
  gsl_matrix * m;
  size_t imax, jmax;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_max_index(m, &imax, &jmax);
  return rb_ary_new3(2, INT2NUM(imax), INT2NUM(jmax));
}

static VALUE Matrix_min_index(VALUE obj)
{
  gsl_matrix * m;
  size_t imin, jmin;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_min_index(m, &imin, &jmin);
  return rb_ary_new3(2, INT2NUM(imin), INT2NUM(jmin));
}

static VALUE Matrix_minmax_index(VALUE obj)
{
  gsl_matrix * m;
  size_t imin, jmin, imax, jmax;

  Data_Get_Struct(obj, gsl_matrix, m);
  gsl_matrix_minmax_index(m, &imin, &jmin, &imax, &jmax);
  return rb_ary_new3(4, INT2NUM(imin), INT2NUM(jmin), INT2NUM(imax), INT2NUM(jmax));
}

/* Reading and writing matrices */

static VALUE Matrix_fwrite(VALUE obj, VALUE io)
{
  gsl_matrix * h;
  OpenFile * fptr;
  FILE * f;
  int status;
  Data_Get_Struct(obj, gsl_matrix, h);
  Check_Type(io, T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_matrix_fwrite(f, h);
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_fread(VALUE obj, VALUE io)
{
  gsl_matrix * h;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_matrix, h);
  Check_Type(io, T_FILE);
  GetOpenFile(io, fptr);     
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_matrix_fread(f, h);
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_fprintf(VALUE obj, VALUE io, VALUE format)
{
  gsl_matrix * h;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_matrix, h);
  Check_Type(io, T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_matrix_fprintf(f, h, STR2CSTR(format));
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_printf(VALUE obj, VALUE format)
{
  gsl_matrix * h;
  int status;

  Data_Get_Struct(obj, gsl_matrix, h);
  status = gsl_matrix_fprintf(stdout, h, STR2CSTR(format));
  if (status == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to stdout.");
  else if (status)
    rb_raise(rb_eStandardError, "Unknown error.");
  return obj;
}

static VALUE Matrix_fscanf(VALUE obj, VALUE io)
{
  gsl_matrix * h;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_matrix, h);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_matrix_fscanf(f, h);
  return INT2FIX(status);
}

/* Ruby convenience methods */

static VALUE Matrix_print(VALUE obj) {
  gsl_matrix * matrix;
  size_t n1, n2;
  size_t i, j;

  Data_Get_Struct(obj, gsl_matrix, matrix);
  n1 = matrix->size1;
  n2 = matrix->size2;
  printf("[ ");
  for (i = 0; i < n1; i++) {
    if (i != 0)
      printf("  ");
    for (j = 0; j < n2; j++)
      printf("%4.3e ", gsl_matrix_get(matrix, i, j));
    if (i == n1 - 1)
      printf("]\n");
    else
      printf("\n");
  }

  return obj;
}

static VALUE Matrix_to_a(VALUE obj) {
  VALUE ary;
  gsl_matrix * matrix;
  size_t i;

  Data_Get_Struct(obj, gsl_matrix, matrix);

  ary = rb_ary_new();
  for (i = 0; i < matrix->size1; i++)
    rb_ary_push(ary, Matrix_row(obj, INT2NUM(i)));

  return ary;
}

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
static VALUE Matrix_allocate(VALUE kl) {
  return Matrix_calloc(kl, INT2NUM(1), INT2NUM(1));
}
#endif

static VALUE Matrix_dump_data(VALUE obj) {
  return Matrix_to_a(obj);
}

static VALUE Matrix_load_data(VALUE obj, VALUE ary) {
  gsl_matrix *matrix;
  gsl_vector *vector;
  VALUE firstVec;
  size_t i;
  Data_Get_Struct(obj, gsl_matrix, matrix);
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
    Matrix_set_row(obj, INT2NUM(i), rb_ary_entry(ary, i));
  }
  return Qtrue;
}
static VALUE Matrix_inspect(VALUE obj) {
  Matrix_print(obj);
  return obj;
}

/* BLAS Support */
/* gsl_matrix.rb in the contrib directory makes easier to use a few
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
static VALUE Matrix_mult(VALUE obj, VALUE bb)
{
  gsl_matrix * a, * b, * c;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(bb, gsl_matrix, b);
  if (a->size2 != b->size1)
	    rb_raise(rb_eIOError, "Matrix product not allowed");
  c = gsl_matrix_alloc(a->size1, b->size2);
  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, a, b, 0.0, c);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, c);
}

   
static VALUE Matrix_mult_vect(VALUE obj, VALUE bb)
{
  gsl_matrix * a;
  gsl_vector * b, * c;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(bb, gsl_vector, b);
  if (a->size2 != b->size)
    rb_raise(rb_eIOError, "mult_vect: Matrix product not allowed");
  c = gsl_vector_alloc(a->size1);
  gsl_blas_dgemv(CblasNoTrans,1.0, a, b, 0.0, c);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, c);
}

static VALUE Matrix_negative(VALUE obj)
{
  int i, j, n1, n2;
  gsl_matrix * a, * new;
  Data_Get_Struct(obj, gsl_matrix, a);
  n1 = a->size1;
  n2 = a->size2;
  new = gsl_matrix_alloc(n1, n2);
  gsl_matrix_memcpy(new, a);
  for(i = 0; i < n1; i++)
	for(j = 0; j < n2; j++)
	  gsl_matrix_set(new,i,j,-gsl_matrix_get(a, i,j));
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}

/* Permutation matrix */
#include "gsl/gsl_permutation.h"
/* Golub, Loan p109
 * Let p(k) be the column index of the sole "1" in the k-th row
 * P(k,p(k)) = 1; P(k,j) = 0 for j != p(k)  
 */
static VALUE Matrix_perm(VALUE self, VALUE p1)
{
  int i,n;
  gsl_permutation *p;
  gsl_matrix * a;
  Data_Get_Struct(p1, gsl_permutation, p);
  n = p->size;
  a = gsl_matrix_calloc(n, n);
  for(i = 0; i < n; i++)
	   gsl_matrix_set(a,i,p->data[i],1);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, a);
}



/* Class definition */

VALUE rbgsl_cMatrix;

void Init_Matrix() {
  rbgsl_cMatrix = rb_define_class_under(rbgsl_mGSL, "Matrix", rb_cObject);

  rb_define_singleton_method(rbgsl_cMatrix, "new", Matrix_new, -1);
  rb_define_singleton_method(rbgsl_cMatrix, "alloc", Matrix_new, -1);
  rb_define_singleton_method(rbgsl_cMatrix, "calloc", Matrix_calloc, 2);

  rb_define_method(rbgsl_cMatrix, "size1", Matrix_size1, 0);
  rb_define_method(rbgsl_cMatrix, "size2", Matrix_size2, 0);
  rb_define_alias(rbgsl_cMatrix, "size", "size1");
  
  rb_define_method(rbgsl_cMatrix, "get", Matrix_get, 2);
  rb_define_method(rbgsl_cMatrix, "set", Matrix_set, -1);
  rb_define_method(rbgsl_cMatrix, "set_all", Matrix_set_all, 1);
  rb_define_method(rbgsl_cMatrix, "set_zero", Matrix_set_zero, 0);
  rb_define_method(rbgsl_cMatrix, "set_identity", Matrix_set_identity, 0);
  rb_define_method(rbgsl_cMatrix, "submatrix", Matrix_submatrix, 4);
  rb_define_method(rbgsl_cMatrix, "row", Matrix_row, 1);
  rb_define_alias(rbgsl_cMatrix, "get_row", "row");
  rb_define_alias(rbgsl_cMatrix, "[]", "row");
  rb_define_method(rbgsl_cMatrix, "column", Matrix_column, 1);
  rb_define_alias(rbgsl_cMatrix, "col", "column");
  rb_define_alias(rbgsl_cMatrix, "get_column", "column");
  rb_define_alias(rbgsl_cMatrix, "get_col", "column");
  
  rb_define_method(rbgsl_cMatrix, "diagonal", Matrix_diagonal, 0);
  rb_define_method(rbgsl_cMatrix, "subdiagonal", Matrix_subdiagonal, 1);
  rb_define_method(rbgsl_cMatrix, "superdiagonal", Matrix_superdiagonal, 1);
  rb_define_method(rbgsl_cMatrix, "clone", Matrix_clone, 0);
  rb_define_method(rbgsl_cMatrix, "swap", Matrix_swap, 1);
  rb_define_method(rbgsl_cMatrix, "set_row", Matrix_set_row, 2);
  rb_define_method(rbgsl_cMatrix, "set_col", Matrix_set_col, 2);
  rb_define_method(rbgsl_cMatrix, "swap_rows", Matrix_swap_rows, 2);
  rb_define_method(rbgsl_cMatrix, "swap_rows!", Matrix_swap_rows2, 2);
  rb_define_method(rbgsl_cMatrix, "swap_columns", Matrix_swap_columns, 2);
  rb_define_alias(rbgsl_cMatrix, "swap_cols", "swap_columns");
  rb_define_method(rbgsl_cMatrix, "swap_columns!", Matrix_swap_columns2, 2);
  rb_define_alias(rbgsl_cMatrix, "swap_cols!", "swap_columns!");
  rb_define_method(rbgsl_cMatrix, "swap_rowcol", Matrix_swap_rowcol, 2);
  rb_define_method(rbgsl_cMatrix, "swap_rowcol!", Matrix_swap_rowcol2, 2);
  rb_define_method(rbgsl_cMatrix, "transpose_memcpy", Matrix_transpose_memcpy, 0);
  rb_define_alias(rbgsl_cMatrix, "transpose", "transpose_memcpy");
  rb_define_alias(rbgsl_cMatrix, "t", "transpose_memcpy");
  rb_define_method(rbgsl_cMatrix, "transpose!", Matrix_transpose, 0);
  rb_define_method(rbgsl_cMatrix, "isnull", Matrix_isnull, 0);
  rb_define_method(rbgsl_cMatrix, "add", Matrix_add, 1);
  rb_define_alias(rbgsl_cMatrix, "+", "add");
  rb_define_method(rbgsl_cMatrix, "add!", Matrix_add2, 1);
  rb_define_method(rbgsl_cMatrix, "sub", Matrix_sub, 1);
  rb_define_alias(rbgsl_cMatrix, "-", "sub");
  rb_define_method(rbgsl_cMatrix, "sub!", Matrix_sub2, 1);
  rb_define_method(rbgsl_cMatrix, "mul_elements", Matrix_mul_elements, 1);
  rb_define_method(rbgsl_cMatrix, "mul_elements!", Matrix_mul_elements2, 1);
  rb_define_method(rbgsl_cMatrix, "mul", Matrix_mul, 1);
  rb_define_alias(rbgsl_cMatrix, "*", "mul");
  rb_define_method(rbgsl_cMatrix, "mul!", Matrix_mul2, 1);
  rb_define_method(rbgsl_cMatrix, "div_elements", Matrix_div_elements, 1);
  
  
  rb_define_alias(rbgsl_cMatrix, "/", "div_elements");
  rb_define_method(rbgsl_cMatrix, "div_elements!", Matrix_div_elements2, 1);
  rb_define_method(rbgsl_cMatrix, "scale", Matrix_scale, 1);
  rb_define_method(rbgsl_cMatrix, "scale!", Matrix_scale2, 1);
  rb_define_method(rbgsl_cMatrix, "add_constant", Matrix_add_constant, 1);
  rb_define_method(rbgsl_cMatrix, "add_constant!", Matrix_add_constant2, 1);
  rb_define_method(rbgsl_cMatrix, "max", Matrix_max, 0);
  rb_define_method(rbgsl_cMatrix, "min", Matrix_min, 0);
  rb_define_method(rbgsl_cMatrix, "minmax", Matrix_minmax, 0);
  rb_define_method(rbgsl_cMatrix, "max_index", Matrix_max_index, 0);
  rb_define_method(rbgsl_cMatrix, "min_index", Matrix_min_index, 0);
  rb_define_method(rbgsl_cMatrix, "minmax_index", Matrix_minmax_index, 0);

  rb_define_method(rbgsl_cMatrix, "==", Matrix_equal, 1);
  rb_define_singleton_method(rbgsl_cMatrix, "float_equal", Matrix_float_equal, 3);
  
  rb_define_method(rbgsl_cMatrix, "fwrite", Matrix_fwrite, 1);
  rb_define_method(rbgsl_cMatrix, "fread", Matrix_fread, 1);
  rb_define_method(rbgsl_cMatrix, "fprintf", Matrix_fprintf, 2);
  rb_define_method(rbgsl_cMatrix, "printf", Matrix_printf, 1);
  rb_define_method(rbgsl_cMatrix, "fscanf", Matrix_fscanf, 1);

  rb_define_method(rbgsl_cMatrix, "print", Matrix_print, 0);
  rb_define_method(rbgsl_cMatrix, "inspect", Matrix_inspect, 0);
  rb_define_method(rbgsl_cMatrix, "to_a", Matrix_to_a, 0);

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
  rb_define_alloc_func(rbgsl_cMatrix, Matrix_allocate);
#endif

  rb_define_method(rbgsl_cMatrix, "_load_data", Matrix_load_data, 1);
  rb_define_method(rbgsl_cMatrix, "_dump_data", Matrix_dump_data, 0);

  /* blas */
  rb_define_method(rbgsl_cMatrix, "mult", Matrix_mult, 1);
  rb_define_method(rbgsl_cMatrix, "mult_vect", Matrix_mult_vect, 1);
  rb_define_method(rbgsl_cMatrix, "negative", Matrix_negative, 0);
  rb_define_alias(rbgsl_cMatrix, "-@", "negative");
  
  rb_define_singleton_method(rbgsl_cMatrix, "perm", Matrix_perm, 1);
  Init_Blas();
  Init_Eigen();
  Init_Linalg();
}

/* vim: set ts=4: */
