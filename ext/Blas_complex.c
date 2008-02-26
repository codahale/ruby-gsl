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

/* $Id: Blas_complex.c,v 1.3 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_vector_complex.h"
#include "gsl/gsl_min.h"
#include "gsl/gsl_blas.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

#define Blas_complex_Trans	Qtrue
#define Blas_complex_NoTrans	Qfalse
#define Blas_complex_ConjTrans	Qnil
#define Blas_complex_Upper	Qtrue
#define Blas_complex_Lower	Qfalse
#define Blas_complex_Unit	Qtrue
#define Blas_complex_NonUnit	Qfalse
#define Blas_complex_Left	Qtrue
#define Blas_complex_Right	Qfalse

/* BLAS Support */

/* Level 1 */


/* x^T y */
static VALUE Blas_complex_zdotu(VALUE obj, VALUE bb) {
  gsl_vector_complex * a, * b;
  gsl_complex z;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  gsl_blas_zdotu(a, b, &z);
  return Complex_new_intern(rbgsl_cComplex, &z);
}

/* x^H y */
static VALUE Blas_complex_zdotc(VALUE obj, VALUE bb) {
  gsl_vector_complex * a, * b;
  gsl_complex z;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  gsl_blas_zdotc(a, b, &z);
  return Complex_new_intern(rbgsl_cComplex, &z);
}



/* Euclidean norm ||x||_2 = \sqrt {\sum (\Re(x_i)^2 + \Im(x_i)^2)} */
static VALUE Blas_complex_dznrm2(VALUE obj) {
  gsl_vector_complex * a;
  double x;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  x = gsl_blas_dznrm2(a);
  return(rb_float_new(x));
}



static VALUE Blas_complex_dzasum(VALUE obj) {
  gsl_vector_complex * a;
  double x;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  x = gsl_blas_dzasum(a);
  return(rb_float_new(x));
}

/* largest element of the vector x by absolute magnitude */

static VALUE Blas_complex_izamax(VALUE obj) {
  gsl_vector_complex * a;
  size_t i;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  i = gsl_blas_izamax(a);
  return(INT2FIX(i));
}


static VALUE Blas_complex_zswap(VALUE obj, VALUE va, VALUE vb) {
  gsl_vector_complex * a, * b;
  Data_Get_Struct(va, gsl_vector_complex, a);
  Data_Get_Struct(vb, gsl_vector_complex, b);
  gsl_blas_zswap(a, b);
  return obj;
} 


static VALUE Blas_complex_zcopy(VALUE obj, VALUE va) {
  gsl_vector_complex * a, * v;
  Data_Get_Struct(va, gsl_vector_complex, a);
  Data_Get_Struct(obj, gsl_vector_complex, v);
  gsl_blas_zcopy(v, a);
  return obj;
} 



/* y' = \alpha x + y */
static VALUE Blas_complex_zaxpy(VALUE obj, VALUE xx, VALUE va) {
  gsl_vector_complex * x, * y, * new;
  gsl_complex * zp;
  Data_Get_Struct(obj, gsl_vector_complex, y);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  new = gsl_vector_complex_alloc(x->size);
  gsl_vector_complex_memcpy(new, y);
  Data_Get_Struct(va, gsl_complex, zp);
  gsl_blas_zaxpy(*zp, x, new);
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_free, new);
}

/* y = \alpha x + y */
static VALUE Blas_complex_zaxpy_bang(VALUE obj, VALUE xx, VALUE va) {
  gsl_vector_complex * x, * y;
  gsl_complex * zp;
  Data_Get_Struct(obj, gsl_vector_complex, y);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(va, gsl_complex, zp);
  gsl_blas_zaxpy(*zp, x, y);
  return  obj;
}

	  


/* Level 2 */
/* y' = alpha op(A) x + beta y */

static VALUE Blas_complex_zgemv(VALUE obj, VALUE tta, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y, * new;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(obj, gsl_vector_complex, y);
  new = gsl_vector_complex_alloc(a->size1);
  gsl_vector_complex_memcpy(new, y);
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);
  if (tta == Qfalse) {
    gsl_blas_zgemv(CblasNoTrans, *palpha, a, x, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  if (tta == Qtrue) {
    gsl_blas_zgemv(CblasTrans, *palpha, a, x, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  if (tta == Qnil) {
    gsl_blas_zgemv(CblasConjTrans, *palpha, a, x, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  return Qnil;
}


/* y = alpha op(A) x + beta y */

static VALUE Blas_complex_zgemv_bang(VALUE obj, VALUE tta, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(obj, gsl_vector_complex, y);
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);
  if (tta == Qfalse) {
    gsl_blas_zgemv(CblasNoTrans, *palpha, a, x, *pbeta, y);
    return obj;
  }
  if (tta == Qtrue) {
    gsl_blas_zgemv(CblasTrans, *palpha, a, x, *pbeta, y);
    return obj;
  }
  if (tta == Qnil) {
    gsl_blas_zgemv(CblasConjTrans, *palpha, a, x, *pbeta, y);
    return obj;
  }
  return Qnil;
}


/* This method computes x' = op(A) x ,
 * where op(A) = A, A^T, A^H for TransA = true of false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */

static VALUE Blas_complex_ztrmv(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * new;
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_vector_complex, x);
  new = gsl_vector_complex_alloc(x->size);
  gsl_vector_complex_memcpy(new, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
         gsl_blas_ztrmv (CblasUpper, CblasTrans, CblasUnit, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrmv (CblasUpper, CblasTrans, CblasNonUnit, a, new);
     }
     if (tta == Qfalse) { 
       if (vd == Qtrue)
          gsl_blas_ztrmv (CblasUpper, CblasNoTrans, CblasUnit, a, new);
       if (vd == Qfalse)
          gsl_blas_ztrmv (CblasUpper, CblasNoTrans, CblasNonUnit, a, new);
      }
     if (tta == Qnil) { 
       if (vd == Qtrue)
          gsl_blas_ztrmv (CblasUpper, CblasConjTrans, CblasUnit, a, new);
       if (vd == Qfalse)
          gsl_blas_ztrmv (CblasUpper, CblasConjTrans, CblasNonUnit, a, new);
      }

     return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
      if (vd == Qtrue)
        gsl_blas_ztrmv (CblasLower, CblasTrans, CblasUnit, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrmv (CblasLower, CblasTrans, CblasNonUnit, a, new);
     }
      if (tta == Qfalse) {
        if (vd == Qtrue)
          gsl_blas_ztrmv (CblasLower, CblasNoTrans, CblasUnit, a, new);
        if (vd == Qfalse)
          gsl_blas_ztrmv (CblasLower, CblasNoTrans, CblasNonUnit, a, new);
      }

      if (tta == Qnil) {
        if (vd == Qtrue)
          gsl_blas_ztrmv (CblasLower, CblasConjTrans, CblasUnit, a, new);
        if (vd == Qfalse)
          gsl_blas_ztrmv (CblasLower, CblasConjTrans, CblasNonUnit, a, new);
      }
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  return Qnil;
}


/* This method computes x = op(A) x ,
 * where op(A) = A, A^T, A^H for TransA = true of false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */

static VALUE Blas_complex_ztrmv_bang(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x;
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_vector_complex, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
         gsl_blas_ztrmv (CblasUpper, CblasTrans, CblasUnit, a, x);
      if (vd == Qfalse)
        gsl_blas_ztrmv (CblasUpper, CblasTrans, CblasNonUnit, a, x);
     }
     if (tta == Qfalse) { 
       if (vd == Qtrue)
          gsl_blas_ztrmv (CblasUpper, CblasNoTrans, CblasUnit, a, x);
       if (vd == Qfalse)
          gsl_blas_ztrmv (CblasUpper, CblasNoTrans, CblasNonUnit, a, x);
      }
     if (tta == Qnil) { 
       if (vd == Qtrue)
          gsl_blas_ztrmv (CblasUpper, CblasConjTrans, CblasUnit, a, x);
       if (vd == Qfalse)
          gsl_blas_ztrmv (CblasUpper, CblasConjTrans, CblasNonUnit, a, x);
      }

     return obj;
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
      if (vd == Qtrue)
        gsl_blas_ztrmv (CblasLower, CblasTrans, CblasUnit, a, x);
      if (vd == Qfalse)
        gsl_blas_ztrmv (CblasLower, CblasTrans, CblasNonUnit, a, x);
     }
      if (tta == Qfalse) {
        if (vd == Qtrue)
          gsl_blas_ztrmv (CblasLower, CblasNoTrans, CblasUnit, a, x);
        if (vd == Qfalse)
          gsl_blas_ztrmv (CblasLower, CblasNoTrans, CblasNonUnit, a, x);
      }

      if (tta == Qnil) {
        if (vd == Qtrue)
          gsl_blas_ztrmv (CblasLower, CblasConjTrans, CblasUnit, a, x);
        if (vd == Qfalse)
          gsl_blas_ztrmv (CblasLower, CblasConjTrans, CblasNonUnit, a, x);
      }
      return obj;
  }
  return Qnil;
}

/* This method computes x' = inv(op(A)) x ,
 * where op(A) = A, A^T, A^H for TransA = true, false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */

static VALUE Blas_complex_ztrsv(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * new;
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_vector_complex, x);
  new = gsl_vector_complex_alloc(x->size);
  gsl_vector_complex_memcpy(new, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
         gsl_blas_ztrsv (CblasUpper, CblasTrans, CblasUnit, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrsv (CblasUpper, CblasTrans, CblasNonUnit, a, new);
     }
     if (tta == Qfalse) { 
       if (vd == Qtrue)
          gsl_blas_ztrsv (CblasUpper, CblasNoTrans, CblasUnit, a, new);
       if (vd == Qfalse)
          gsl_blas_ztrsv (CblasUpper, CblasNoTrans, CblasNonUnit, a, new);
      }
     if (tta == Qnil) { 
       if (vd == Qtrue)
          gsl_blas_ztrsv (CblasUpper, CblasConjTrans, CblasUnit, a, new);
       if (vd == Qfalse)
          gsl_blas_ztrsv (CblasUpper, CblasConjTrans, CblasNonUnit, a, new);
      }

     return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
      if (vd == Qtrue)
        gsl_blas_ztrsv (CblasLower, CblasTrans, CblasUnit, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrsv (CblasLower, CblasTrans, CblasNonUnit, a, new);
     }
      if (tta == Qfalse) {
        if (vd == Qtrue)
          gsl_blas_ztrsv (CblasLower, CblasNoTrans, CblasUnit, a, new);
        if (vd == Qfalse)
          gsl_blas_ztrsv (CblasLower, CblasNoTrans, CblasNonUnit, a, new);
      }

      if (tta == Qnil) {
        if (vd == Qtrue)
          gsl_blas_ztrsv (CblasLower, CblasConjTrans, CblasUnit, a, new);
        if (vd == Qfalse)
          gsl_blas_ztrsv (CblasLower, CblasConjTrans, CblasNonUnit, a, new);
      }
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  return Qnil;
}


/* This method computes x = inv(op(A)) x ,
 * where op(A) = A, A^T, A^H for TransA = true, false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */

static VALUE Blas_complex_ztrsv_bang(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x;
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_vector_complex, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
         gsl_blas_ztrsv (CblasUpper, CblasTrans, CblasUnit, a, x);
      if (vd == Qfalse)
        gsl_blas_ztrsv (CblasUpper, CblasTrans, CblasNonUnit, a, x);
     }
     if (tta == Qfalse) { 
       if (vd == Qtrue)
          gsl_blas_ztrsv (CblasUpper, CblasNoTrans, CblasUnit, a, x);
       if (vd == Qfalse)
          gsl_blas_ztrsv (CblasUpper, CblasNoTrans, CblasNonUnit, a, x);
      }
     if (tta == Qnil) { 
       if (vd == Qtrue)
          gsl_blas_ztrsv (CblasUpper, CblasConjTrans, CblasUnit, a, x);
       if (vd == Qfalse)
          gsl_blas_ztrsv (CblasUpper, CblasConjTrans, CblasNonUnit, a, x);
      }
     return obj;
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
      if (vd == Qtrue)
        gsl_blas_ztrsv (CblasLower, CblasTrans, CblasUnit, a, x);
      if (vd == Qfalse)
        gsl_blas_ztrsv (CblasLower, CblasTrans, CblasNonUnit, a, x);
     }
      if (tta == Qfalse) {
        if (vd == Qtrue)
          gsl_blas_ztrsv (CblasLower, CblasNoTrans, CblasUnit, a, x);
        if (vd == Qfalse)
          gsl_blas_ztrsv (CblasLower, CblasNoTrans, CblasNonUnit, a, x);
      }

      if (tta == Qnil) {
        if (vd == Qtrue)
          gsl_blas_ztrsv (CblasLower, CblasConjTrans, CblasUnit, a, x);
        if (vd == Qfalse)
          gsl_blas_ztrsv (CblasLower, CblasConjTrans, CblasNonUnit, a, x);
      }
      return obj;
  }
  return Qnil;
}


/* y' = alpha op(A) x + beta y  for hermitian matrix A
 */

static VALUE Blas_complex_zhemv(VALUE obj, VALUE vup, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y, * new;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(obj, gsl_vector_complex, y);
  new = gsl_vector_complex_alloc(a->size1);
  gsl_vector_complex_memcpy(new, y);
  if (vup == Qfalse) {
    gsl_blas_zhemv(CblasLower,*palpha, a, x, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_complex_free, new);
  }
  if (vup == Qtrue) {
    gsl_blas_zhemv(CblasUpper,*palpha, a, x, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  if (vup == Qfalse) {
    gsl_blas_zhemv(CblasLower,*palpha, a, x, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  }
  return Qnil;
}


/* y = alpha op(A) x + beta y  for hermitian matrix A
 */

static VALUE Blas_complex_zhemv_bang(VALUE obj, VALUE vup, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(obj, gsl_vector_complex, y);
  if (vup == Qfalse) {
    gsl_blas_zhemv(CblasLower,*palpha, a, x, *pbeta, y);
    return obj;
  }
  if (vup == Qtrue) {
    gsl_blas_zhemv(CblasUpper,*palpha, a, x, *pbeta, y);
    return obj;
  }
  if (vup == Qfalse) {
    gsl_blas_zhemv(CblasLower,*palpha, a, x, *pbeta, y);
    return obj;
  }
  return Qnil;
}

/* A' = \alpha x y^T + A of the matrix A 
 */ 
static VALUE Blas_complex_zgeru(VALUE obj, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix_complex * a, * new;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(yy, gsl_vector_complex, y);
  new = gsl_matrix_complex_alloc(a->size1, a->size2);
  gsl_matrix_complex_memcpy(new, a);
  gsl_blas_zgeru(*palpha, x, y, new);
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

/* A = \alpha x y^T + A of the matrix A 
 */ 
static VALUE Blas_complex_zgeru_bang(VALUE obj, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(yy, gsl_vector_complex, y);
  gsl_blas_zgeru(*palpha, x, y, a);
  return obj;
}

/* A' = \alpha x y^H + A of the matrix A 
 */ 
static VALUE Blas_complex_zgerc(VALUE obj, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix_complex * a, * new;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(yy, gsl_vector_complex, y);
  new = gsl_matrix_complex_alloc(a->size1, a->size2);
  gsl_matrix_complex_memcpy(new, a);
  gsl_blas_zgerc(*palpha, x, y, new);
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
}

/* A = \alpha x y^H + A of the matrix A 
 */ 
static VALUE Blas_complex_zgerc_bang(VALUE obj, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(yy, gsl_vector_complex, y);
  gsl_blas_zgerc(*palpha, x, y, a);
  return obj;
}

/* update A' = \alpha x x^H + A for hermitian matrix A
 */
static VALUE Blas_complex_zher(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx) {
  gsl_matrix_complex * a, * new;
  gsl_vector_complex * x;
  double alpha;
  alpha = NUM2DBL(aalpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  new = gsl_matrix_complex_alloc(a->size1, a->size2);
  gsl_matrix_complex_memcpy(new, a);
  
  if (vup == Qfalse) {
    gsl_blas_zher(CblasLower, alpha, x, new);
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  if (vup == Qtrue) {
    gsl_blas_zher(CblasUpper, alpha, x, new);
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}

/* update A = \alpha x x^H + A for hermitian matrix A
 */
static VALUE Blas_complex_zher_bang(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx) {
  gsl_matrix_complex * a;
  gsl_vector_complex * x;
  double alpha;
  alpha = NUM2DBL(aalpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  
  if (vup == Qfalse) {
    gsl_blas_zher(CblasLower, alpha, x, a);
    return obj;
  }
  if (vup == Qtrue) {
    gsl_blas_zher(CblasUpper, alpha, x, a);
    return obj;
  }
  return Qnil;
}



/* 
 * update A' = alpha x y^H + alpha^* y x^H + A for hermitian matrix A
 */
static VALUE Blas_complex_zher2(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix_complex * a, * new;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(yy, gsl_vector_complex, y);
  new = gsl_matrix_complex_alloc(a->size1, a->size2);
  gsl_matrix_complex_memcpy(new, a);
  
  if (vup == Qfalse) {
    gsl_blas_zher2(CblasLower, *palpha, x, y, new);
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  if (vup == Qtrue) {
    gsl_blas_zher2(CblasUpper, *palpha, x, y, new);
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}

/* 
 * update A = alpha x y^H + alpha^* y x^H + A for hermitian matrix A
 */
static VALUE Blas_complex_zher2_bang(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix_complex * a;
  gsl_vector_complex * x, * y;
  gsl_complex * palpha;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(xx, gsl_vector_complex, x);
  Data_Get_Struct(yy, gsl_vector_complex, y);
  
  if (vup == Qfalse) {
    gsl_blas_zher2(CblasLower, *palpha, x, y, a);
    return obj;
  }
  if (vup == Qtrue) {
    gsl_blas_zher2(CblasUpper, *palpha, x, y, a);
    return obj;
  }
  return Qnil;
}


/* Level 3 */

/*  matrix-matrix product and sum
 *  C' = \alpha op(A) op(B) + \beta C
 *  where op(A) = A, A^T, A^H for TransA = false, true, nil respectively;
 *  similarly for the parameter TransB.
 */

static VALUE Blas_complex_zgemm(VALUE obj, VALUE tta, VALUE ttb, VALUE aalpha, 
			VALUE aa, VALUE bb, VALUE bbeta)  
{
  gsl_matrix_complex * a, * b, * c, * new;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);
       
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  new = gsl_matrix_complex_alloc(a->size1, b->size2);
  gsl_matrix_complex_memcpy(new, c);
  if (tta == Qfalse) {
    if (ttb == Qfalse) {
      gsl_blas_zgemm (CblasNoTrans, CblasNoTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
	}
    if (ttb == Qtrue) {
      gsl_blas_zgemm (CblasNoTrans, CblasTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
	}

    if (ttb == Qnil) {
      gsl_blas_zgemm (CblasNoTrans, CblasConjTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
	}
  }
  if (tta == Qtrue) {
    if (ttb == Qfalse) {
      gsl_blas_zgemm (CblasTrans, CblasNoTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
	}
    if (ttb == Qtrue) {
      gsl_blas_zgemm (CblasTrans, CblasTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
    }

    if (ttb == Qnil) {
      gsl_blas_zgemm (CblasTrans, CblasConjTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
    }
  }

  if (tta == Qnil) {
    if (ttb == Qfalse) {
      gsl_blas_zgemm (CblasConjTrans, CblasNoTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
	}
    if (ttb == Qtrue) {
      gsl_blas_zgemm (CblasConjTrans, CblasTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
    }

    if (ttb == Qnil) {
      gsl_blas_zgemm (CblasConjTrans, CblasConjTrans,*palpha, a, b, *pbeta, new);
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
    }
  }
  return Qnil; 
}

/*  matrix-matrix product and sum
 *  C = \alpha op(A) op(B) + \beta C
 *  where op(A) = A, A^T, A^H for TransA = false, true, nil respectively;
 *  similarly for the parameter TransB.
 */

static VALUE Blas_complex_zgemm_bang(VALUE obj, VALUE tta, VALUE ttb, VALUE aalpha, 
			VALUE aa, VALUE bb, VALUE bbeta)  
{
  gsl_matrix_complex * a, * b, * c;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);
       
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(bb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  if (tta == Qfalse) {
    if (ttb == Qfalse) {
      gsl_blas_zgemm (CblasNoTrans, CblasNoTrans,*palpha, a, b, *pbeta, c);
      return obj;
	}
    if (ttb == Qtrue) {
      gsl_blas_zgemm (CblasNoTrans, CblasTrans,*palpha, a, b, *pbeta, c);
      return obj;
	}

    if (ttb == Qnil) {
      gsl_blas_zgemm (CblasNoTrans, CblasConjTrans,*palpha, a, b, *pbeta, c);
      return obj;
	}
  }
  if (tta == Qtrue) {
    if (ttb == Qfalse) {
      gsl_blas_zgemm (CblasTrans, CblasNoTrans,*palpha, a, b, *pbeta, c);
      return obj;
	}
    if (ttb == Qtrue) {
      gsl_blas_zgemm (CblasTrans, CblasTrans,*palpha, a, b, *pbeta, c);
      return obj;
    }

    if (ttb == Qnil) {
      gsl_blas_zgemm (CblasTrans, CblasConjTrans,*palpha, a, b, *pbeta, c);
      return obj;
    }
  }

  if (tta == Qnil) {
    if (ttb == Qfalse) {
      gsl_blas_zgemm (CblasConjTrans, CblasNoTrans,*palpha, a, b, *pbeta, c);
      return obj;
	}
    if (ttb == Qtrue) {
      gsl_blas_zgemm (CblasConjTrans, CblasTrans,*palpha, a, b, *pbeta, c);
      return obj;
    }

    if (ttb == Qnil) {
      gsl_blas_zgemm (CblasConjTrans, CblasConjTrans,*palpha, a, b, *pbeta, c);
      return obj;
    }
  }
  return Qnil; 
}

 
/* C' = alpha A B + beta C when SideLeft = true
 * C' = alpha B A + beta C when SideLeft = false
 *  where A is symmetric. 
 *  When Uplo = true the upper triangle and diagonal of A are used;
 *  when Uplo is false then the lower triangle and diagonal of A are used.
 */
static VALUE Blas_complex_zsymm(VALUE obj, VALUE vSideLeft, VALUE vUplo, VALUE aalpha,
                       VALUE va, VALUE vb, VALUE bbeta)
{
  gsl_matrix_complex * a, * b, * c, * new;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);  
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  new = gsl_matrix_complex_alloc(c->size1, c->size2);
  gsl_matrix_complex_memcpy(new, c);
  if (vSideLeft == Qtrue) {
    if (vUplo == Qtrue)
      gsl_blas_zsymm(CblasLeft, CblasUpper, *palpha, a, b, *pbeta, new);
     if (vUplo == Qfalse)
       gsl_blas_zsymm(CblasLeft, CblasLower, *palpha, a, b, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_complex_free, new);
  }
  if (vSideLeft == Qfalse) {
    if (vUplo == Qtrue)
      gsl_blas_zsymm(CblasRight, CblasUpper, *palpha, a, b, *pbeta, new);
     if (vUplo == Qfalse)
       gsl_blas_zsymm(CblasRight, CblasLower, *palpha, a, b, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}


/* C = alpha A B + beta C when SideLeft = true
 * C = alpha B A + beta C when SideLeft = false
 *  where A is symmetric. 
 *  When Uplo = true the upper triangle and diagonal of A are used;
 *  when Uplo is false then the lower triangle and diagonal of A are used.
 */
static VALUE Blas_complex_zsymm_bang(VALUE obj, VALUE vSideLeft, VALUE vUplo, VALUE aalpha,
                       VALUE va, VALUE vb, VALUE bbeta)
{
  gsl_matrix_complex * a, * b, * c;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);  
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  if (vSideLeft == Qtrue) {
    if (vUplo == Qtrue)
      gsl_blas_zsymm(CblasLeft, CblasUpper, *palpha, a, b, *pbeta, c);
     if (vUplo == Qfalse)
       gsl_blas_zsymm(CblasLeft, CblasLower, *palpha, a, b, *pbeta, c);
    return obj;
  }
  if (vSideLeft == Qfalse) {
    if (vUplo == Qtrue)
      gsl_blas_zsymm(CblasRight, CblasUpper, *palpha, a, b, *pbeta, c);
     if (vUplo == Qfalse)
       gsl_blas_zsymm(CblasRight, CblasLower, *palpha, a, b, *pbeta, c);
    return obj;
  }
  return Qnil;
}

/* C' = alpha A B + beta C when SideLeft = true
 * C' = alpha B A + beta C when SideLeft = false
 *  where A is hermitian. 
 *  When Uplo = true the upper triangle and diagonal of A are used;
 *  when Uplo is false then the lower triangle and diagonal of A are used.
 */
static VALUE Blas_complex_zhemm(VALUE obj, VALUE vSideLeft, VALUE vUplo, VALUE aalpha,
                       VALUE va, VALUE vb, VALUE bbeta)
{
  gsl_matrix_complex * a, * b, * c, * new;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);  
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  new = gsl_matrix_complex_alloc(c->size1, c->size2);
  gsl_matrix_complex_memcpy(new, c);
  if (vSideLeft == Qtrue) {
    if (vUplo == Qtrue)
      gsl_blas_zhemm(CblasLeft, CblasUpper, *palpha, a, b, *pbeta, new);
     if (vUplo == Qfalse)
       gsl_blas_zhemm(CblasLeft, CblasLower, *palpha, a, b, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_complex_free, new);
  }
  if (vSideLeft == Qfalse) {
    if (vUplo == Qtrue)
      gsl_blas_zhemm(CblasRight, CblasUpper, *palpha, a, b, *pbeta, new);
     if (vUplo == Qfalse)
       gsl_blas_zhemm(CblasRight, CblasLower, *palpha, a, b, *pbeta, new);
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}


/* C = alpha A B + beta C when SideLeft = true
 * C = alpha B A + beta C when SideLeft = false
 *  where A is hermitian. 
 *  When Uplo = true the upper triangle and diagonal of A are used;
 *  when Uplo is false then the lower triangle and diagonal of A are used.
 */
static VALUE Blas_complex_zhemm_bang(VALUE obj, VALUE vSideLeft, VALUE vUplo, VALUE aalpha,
                       VALUE va, VALUE vb, VALUE bbeta)
{
  gsl_matrix_complex * a, * b, * c;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(aalpha, gsl_complex, palpha);
  Data_Get_Struct(bbeta, gsl_complex, pbeta);  
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  if (vSideLeft == Qtrue) {
    if (vUplo == Qtrue)
      gsl_blas_zhemm(CblasLeft, CblasUpper, *palpha, a, b, *pbeta, c);
     if (vUplo == Qfalse)
       gsl_blas_zhemm(CblasLeft, CblasLower, *palpha, a, b, *pbeta, c);
    return obj;
  }
  if (vSideLeft == Qfalse) {
    if (vUplo == Qtrue)
      gsl_blas_zhemm(CblasRight, CblasUpper, *palpha, a, b, *pbeta, c);
     if (vUplo == Qfalse)
       gsl_blas_zhemm(CblasRight, CblasLower, *palpha, a, b, *pbeta, c);
    return obj;
  }
  return Qnil;
}

/* B.ztrmm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'
 * The matrix A is triangular.
 * This method computes B' = op(A) B when SideLeft = true,
 * B' = B op(A) when SideLeft = false,
 * where op(A) = A, A^T, A^H for TransA = true, false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_complex_ztrmm(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
                        VALUE valpha, VALUE aa)
{
  gsl_matrix_complex * a, * b, * new;
  gsl_complex * palpha;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
  new = gsl_matrix_complex_alloc(b->size1, b->size2);
  gsl_matrix_complex_memcpy(new, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
	gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasLower, CblasTrans, CblasUnit, *palpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_ztrmm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }
  }
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
	gsl_blas_ztrmm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }

  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasLower, CblasTrans, CblasUnit, *palpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_ztrmm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }
  }
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}


/* B.ztrmm!(SideLeft, Uplo, TransA, DiagUnit, A) -> B
 * The matrix A is triangular.
 * This method computes B' = op(A) B when SideLeft = true,
 * B' = B op(A) when SideLeft = false,
 * where op(A) = A, A^T, A^H for TransA = true, false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_complex_ztrmm_bang(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
                        VALUE valpha, VALUE aa)
{
  gsl_matrix_complex * a, * b;
  gsl_complex * palpha;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qfalse)
	gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasLeft, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrmm (CblasLeft, CblasLower, CblasTrans, CblasUnit, *palpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_ztrmm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasLeft, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }
  }
    return obj;
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
	gsl_blas_ztrmm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrmm (CblasRight, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }

  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrmm (CblasRight, CblasLower, CblasTrans, CblasUnit, *palpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_ztrmm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrmm (CblasRight, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }
  }
      return obj;
  }
  return Qnil;
}


/* >>>> */
/* B.ztrsm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'
 * The matrix A is triangular.
 * This method computes B' = op(inv(A)) B when SideLeft = true,
 * B' = B op(inv(A)) when SideLeft = false,
 * where op(A) = A, A^T, A^H for TransA = true, false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_complex_ztrsm(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
                        VALUE valpha, VALUE aa)
{
  gsl_matrix_complex * a, * b, * new;
  gsl_complex * palpha;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
  new = gsl_matrix_complex_alloc(b->size1, b->size2);
  gsl_matrix_complex_memcpy(new, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
	gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasLower, CblasTrans, CblasUnit, *palpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_ztrsm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }
  }
    return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
	gsl_blas_ztrsm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, new);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }

  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasLower, CblasTrans, CblasUnit, *palpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_ztrsm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, new);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, new);
	  }
  }
      return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}


/* B.ztrsm!(SideLeft, Uplo, TransA, DiagUnit, A) -> B
 * The matrix A is triangular.
 * This method computes B' = op(inv(A)) B when SideLeft = true,
 * B' = B op(inv(A)) when SideLeft = false,
 * where op(A) = A, A^T, A^H for TransA = true, false or nil.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_complex_ztrsm_bang(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
                        VALUE valpha, VALUE aa)
{
  gsl_matrix_complex * a, * b;
  gsl_complex * palpha;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  Data_Get_Struct(aa, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qfalse)
	gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasLeft, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrsm (CblasLeft, CblasLower, CblasTrans, CblasUnit, *palpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_ztrsm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasLeft, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }
  }
    return obj;
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
	gsl_blas_ztrsm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
    if (tta == Qnil) { 
      if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasConjTrans, CblasUnit, *palpha, a, b);
      if (vd == Qfalse)
        gsl_blas_ztrsm (CblasRight, CblasUpper, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }

  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_ztrsm (CblasRight, CblasLower, CblasTrans, CblasUnit, *palpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_ztrsm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, *palpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, *palpha, a, b);
	  }
      if (tta == Qnil) {
	    if (vd == Qtrue)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasConjTrans, CblasUnit, *palpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_ztrsm (CblasRight, CblasLower, CblasConjTrans, CblasNonUnit, *palpha, a, b);
	  }
  }
      return obj;
  }
  return Qnil;
}

/* B.dtrmm!(SideLeft, Uplo, TransA, DiagUnit, A) -> B
 * This method computes B = op(A) B when SideLeft = true,
 * B = B op(A) when SideLeft = false,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
static VALUE Blas_complex_dtrmm_bang(VALUE obj, VALUE vs, VALUE vup, VALUE tta, 
                             VALUE vd, VALUE valpha, VALUE aa)
{
  gsl_matrix * a, * b;
  double alpha = NUM2DBL(valpha);
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
	gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
        gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrmm (CblasLeft, CblasLower, CblasTrans, CblasUnit, alpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_dtrmm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, alpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrmm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, alpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_dtrmm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
    return obj;
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasRight, CblasUpper, CblasTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
	gsl_blas_dtrmm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
        gsl_blas_dtrmm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrmm (CblasRight, CblasLower, CblasTrans, CblasUnit, alpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_dtrmm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, alpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrmm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, alpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_dtrmm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
      return obj;
  }
  return Qnil;
}

 */


/* B.dtrsm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'
 * This method computes B' = op(inv(A)) B when SideLeft = true,
 * B' = B op(inv(A)) when SideLeft = false,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
static VALUE Blas_complex_dtrsm(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
                        VALUE valpha, VALUE aa)
{
  gsl_matrix * a, * b, * new;
  double alpha = NUM2DBL(valpha);
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
  new = gsl_matrix_alloc(b->size1, b->size2);
  gsl_matrix_memcpy(new, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
	gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
        gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrsm (CblasLeft, CblasLower, CblasTrans, CblasUnit, alpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrsm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, alpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrsm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, alpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrsm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasRight, CblasUpper, CblasTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
	gsl_blas_dtrsm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
        gsl_blas_dtrsm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrsm (CblasRight, CblasLower, CblasTrans, CblasUnit, alpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrsm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, alpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrsm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, alpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrsm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
      return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  return Qnil;
}

 */

/* B.dtrsm!(SideLeft, Uplo, TransA, DiagUnit, A) -> B
 * This method computes B = op(inv(A)) B when SideLeft = true,
 * B = B op(inv(A)) when SideLeft = false,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
static VALUE Blas_complex_dtrsm_bang(VALUE obj, VALUE vs, VALUE vup, VALUE tta, 
                             VALUE vd, VALUE valpha, VALUE aa)
{
  gsl_matrix * a, * b;
  double alpha = NUM2DBL(valpha);
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
  if (vs == Qtrue) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
	gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
        gsl_blas_dtrsm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrsm (CblasLeft, CblasLower, CblasTrans, CblasUnit, alpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_dtrsm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, alpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrsm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, alpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_dtrsm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
    return obj;
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasRight, CblasUpper, CblasTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
	gsl_blas_dtrsm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, b);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrsm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, b);
      if (vd == Qfalse)
        gsl_blas_dtrsm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrsm (CblasRight, CblasLower, CblasTrans, CblasUnit, alpha, a, b);
	  if (vd == Qfalse)
        gsl_blas_dtrsm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, alpha, a, b);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrsm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, alpha, a, b);
	    if (vd == Qfalse)
          gsl_blas_dtrsm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, b);
	  }
  }
      return obj;
  }
  return Qnil;
}

 */

/* This method computes 
 * C.zsyr2k(Uplo, Trans, alpha, A, B, beta) -> C'
 * C' = alpha A^T B + alpha B^T A + beta C when Trans = true and
 * C' = alpha A B^T + alpha B A^T + beta C when Trans = false
 * where C is a symmetric matrix
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 */
static VALUE Blas_complex_zsyr2k(VALUE obj, VALUE uplo, VALUE tta, VALUE valpha,
                         VALUE va, VALUE vb, VALUE vbeta)
{
  gsl_matrix_complex * a, * b, * c, * new;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  Data_Get_Struct(vbeta, gsl_complex, pbeta);
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  new = gsl_matrix_complex_alloc(c->size1, c->size2);
  gsl_matrix_complex_memcpy(new, c);
  if (uplo == Qtrue) {
    if (tta == Qtrue)
        gsl_blas_zsyr2k (CblasUpper, CblasTrans, *palpha, a, b, *pbeta, new);
    if (tta == Qfalse) 
        gsl_blas_zsyr2k (CblasUpper, CblasNoTrans, *palpha, a, b, *pbeta, new);
     return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  if (uplo == Qfalse) {
    if (tta == Qtrue)
        gsl_blas_zsyr2k (CblasLower, CblasTrans, *palpha, a, b, *pbeta, new);
    if (tta == Qfalse) 
        gsl_blas_zsyr2k (CblasLower, CblasNoTrans, *palpha, a, b, *pbeta, new);
     return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}


/* This method computes 
 * C.zsyr2k(Uplo, Trans, alpha, A, B, beta) -> C
 * C = alpha A^T B + alpha B^T A + beta C when Trans = true and
 * C = alpha A B^T + alpha B A^T + beta C when Trans = false
 * where C is a symmetric matrix
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 */
static VALUE Blas_complex_zsyr2k_bang(VALUE obj, VALUE uplo, VALUE tta, VALUE valpha,
                         VALUE va, VALUE vb, VALUE vbeta)
{
  gsl_matrix_complex * a, * b, * c;
  gsl_complex * palpha, * pbeta;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  Data_Get_Struct(vbeta, gsl_complex, pbeta);
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  if (uplo == Qtrue) {
    if (tta == Qtrue)
        gsl_blas_zsyr2k (CblasUpper, CblasTrans, *palpha, a, b, *pbeta, c);
    if (tta == Qfalse) 
        gsl_blas_zsyr2k (CblasUpper, CblasNoTrans, *palpha, a, b, *pbeta, c);
     return obj;
  }
  if (uplo == Qfalse) {
    if (tta == Qtrue)
        gsl_blas_zsyr2k (CblasLower, CblasTrans, *palpha, a, b, *pbeta, c);
    if (tta == Qfalse) 
        gsl_blas_zsyr2k (CblasLower, CblasNoTrans, *palpha, a, b, *pbeta, c);
     return obj;
  }
  return Qnil;
}

/* This method computes 
 * C.zher2k(Uplo, Trans, alpha, A, B, beta) -> C'
 * C' = alpha A^H B + alpha^* B^H A + beta C when Trans = nil and
 * C' = alpha A B^H + alpha^* B A^H + beta C when Trans = false
 * where C is a hermitian matrix
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 */
static VALUE Blas_complex_zher2k(VALUE obj, VALUE uplo, VALUE tta, VALUE valpha,
                         VALUE va, VALUE vb, VALUE vbeta)
{
  gsl_matrix_complex * a, * b, * c, * new;
  gsl_complex * palpha;
  double beta;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  beta = NUM2DBL(vbeta);
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  new = gsl_matrix_complex_alloc(c->size1, c->size2);
  gsl_matrix_complex_memcpy(new, c);
  if (uplo == Qtrue) {
    if (tta == Qnil){
        gsl_blas_zher2k (CblasUpper, CblasConjTrans, *palpha, a, b, beta, new);
    }
    if (tta == Qfalse) 
        gsl_blas_zher2k (CblasUpper, CblasNoTrans, *palpha, a, b, beta, new);
     return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  if (uplo == Qfalse) {
    if (tta == Qnil)
        gsl_blas_zher2k (CblasLower, CblasConjTrans, *palpha, a, b, beta, new);
    if (tta == Qfalse) 
        gsl_blas_zher2k (CblasLower, CblasNoTrans, *palpha, a, b, beta, new);
     return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, new);
  }
  return Qnil;
}

/* This method computes 
 * C.zher2k(Uplo, Trans, alpha, A, B, beta) -> C
 * C = alpha A^H B + alpha^* B^H A + beta C when Trans = nil and
 * C = alpha A B^H + alpha^* B A^H + beta C when Trans = false
 * where C is a hermitian matrix
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 */
static VALUE Blas_complex_zher2k_bang(VALUE obj, VALUE uplo, VALUE tta, VALUE valpha,
                         VALUE va, VALUE vb, VALUE vbeta)
{
  gsl_matrix_complex * a, * b, * c;
  gsl_complex * palpha;
  double beta;
  Data_Get_Struct(valpha, gsl_complex, palpha);
  beta = NUM2DBL(vbeta);
  Data_Get_Struct(va, gsl_matrix_complex, a);
  Data_Get_Struct(vb, gsl_matrix_complex, b);
  Data_Get_Struct(obj, gsl_matrix_complex, c);
  if (uplo == Qtrue) {
    if (tta == Qnil){
        gsl_blas_zher2k (CblasUpper, CblasConjTrans, *palpha, a, b, beta, c);
    }
    if (tta == Qfalse) 
        gsl_blas_zher2k (CblasUpper, CblasNoTrans, *palpha, a, b, beta, c);
     return obj;
  }
  if (uplo == Qfalse) {
    if (tta == Qnil)
        gsl_blas_zher2k (CblasLower, CblasConjTrans, *palpha, a, b, beta, c);
    if (tta == Qfalse) 
        gsl_blas_zher2k (CblasLower, CblasNoTrans, *palpha, a, b, beta, c);
     return obj;
  }
  return Qnil;
}


/* Module definition */
VALUE rbgsl_mBlas_complex;

void Init_Blas_complex() {
   rbgsl_mBlas_complex = rb_define_module_under(rbgsl_mGSL, "Blas_complex");
   
  /* level 1 */ 
  rb_define_method(rbgsl_cVector_complex, "zdotu", Blas_complex_zdotu, 1);
  rb_define_method(rbgsl_cVector_complex, "zdotc", Blas_complex_zdotc, 1);
  rb_define_method(rbgsl_cVector_complex, "dznrm2", Blas_complex_dznrm2, 0);
  rb_define_method(rbgsl_cVector_complex, "dzasum", Blas_complex_dzasum, 0);

  rb_define_method(rbgsl_cVector_complex, "izamax", Blas_complex_izamax, 0);
  rb_define_module_function(rbgsl_mBlas_complex, "zswap", Blas_complex_zswap, 2);
  rb_define_method(rbgsl_cVector_complex, "zcopy", Blas_complex_zcopy, 1);
  rb_define_method(rbgsl_cVector_complex, "zaxpy!", Blas_complex_zaxpy_bang, 2);
  rb_define_method(rbgsl_cVector_complex, "zaxpy", Blas_complex_zaxpy, 2);
  

  /* level 2 */
  rb_define_method(rbgsl_cVector_complex, "zgemv", Blas_complex_zgemv, 5);
  rb_define_method(rbgsl_cVector_complex, "zgemv!", Blas_complex_zgemv_bang, 5);
  rb_define_method(rbgsl_cVector_complex, "ztrmv", Blas_complex_ztrmv, 4);
  rb_define_method(rbgsl_cVector_complex, "ztrmv!", Blas_complex_ztrmv_bang, 4);
  rb_define_method(rbgsl_cVector_complex, "ztrsv", Blas_complex_ztrsv, 4);
  rb_define_method(rbgsl_cVector_complex, "ztrsv!", Blas_complex_ztrsv_bang, 4);
  rb_define_method(rbgsl_cVector_complex, "zhemv", Blas_complex_zhemv, 5);
  rb_define_method(rbgsl_cVector_complex, "zhemv!", Blas_complex_zhemv_bang, 5);
  rb_define_method(rbgsl_cMatrix_complex, "zgeru", Blas_complex_zgeru, 3);
  rb_define_method(rbgsl_cMatrix_complex, "zgeru!", Blas_complex_zgeru_bang, 3);
  rb_define_method(rbgsl_cMatrix_complex, "zgerc", Blas_complex_zgerc, 3);
  rb_define_method(rbgsl_cMatrix_complex, "zgerc!", Blas_complex_zgerc_bang, 3);
  rb_define_method(rbgsl_cMatrix_complex, "zher", Blas_complex_zher, 3);
  rb_define_method(rbgsl_cMatrix_complex, "zher!", Blas_complex_zher_bang, 3);
  
  rb_define_method(rbgsl_cMatrix_complex, "zher2", Blas_complex_zher2, 4);
  rb_define_method(rbgsl_cMatrix_complex, "zher2!", Blas_complex_zher2_bang, 4);
  
  
  /* level 3 */
  rb_define_method(rbgsl_cMatrix_complex, "zgemm", Blas_complex_zgemm, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zgemm!", Blas_complex_zgemm_bang, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zsymm", Blas_complex_zsymm, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zsymm!", Blas_complex_zsymm_bang, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zhemm", Blas_complex_zhemm, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zhemm!", Blas_complex_zhemm_bang, 6);
  rb_define_method(rbgsl_cMatrix_complex, "ztrmm", Blas_complex_ztrmm, 6);
  rb_define_method(rbgsl_cMatrix_complex, "ztrmm!", Blas_complex_ztrmm_bang, 6);
  rb_define_method(rbgsl_cMatrix_complex, "ztrsm", Blas_complex_ztrsm, 6);
  rb_define_method(rbgsl_cMatrix_complex, "ztrsm!", Blas_complex_ztrsm_bang, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zsyr2k", Blas_complex_zsyr2k, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zsyr2k!", Blas_complex_zsyr2k_bang, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zher2k", Blas_complex_zher2k, 6);
  rb_define_method(rbgsl_cMatrix_complex, "zher2k!", Blas_complex_zher2k_bang, 6);

  rb_define_const(rbgsl_mBlas_complex, "Trans", Blas_complex_Trans);
  rb_define_const(rbgsl_mBlas_complex, "NoTrans", Blas_complex_NoTrans);
  rb_define_const(rbgsl_mBlas_complex, "ConjTrans", Blas_complex_ConjTrans);
  rb_define_const(rbgsl_mBlas_complex, "Upper", Blas_complex_Upper);
  rb_define_const(rbgsl_mBlas_complex, "Lower", Blas_complex_Lower);
  rb_define_const(rbgsl_mBlas_complex, "Unit", Blas_complex_Unit);
  rb_define_const(rbgsl_mBlas_complex, "NonUnit", Blas_complex_NonUnit);
  rb_define_const(rbgsl_mBlas_complex, "Left", Blas_complex_Left);
  rb_define_const(rbgsl_mBlas_complex, "Right", Blas_complex_Right);
}      
