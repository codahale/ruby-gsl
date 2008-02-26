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

/* $Id: Blas.c,v 1.8 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_min.h"
#include "gsl/gsl_blas.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

#define Blas_Trans	Qtrue
#define Blas_NoTrans	Qfalse
#define Blas_ConjTrans	Qnil
#define Blas_Upper	Qtrue
#define Blas_Lower	Qfalse
#define Blas_Unit	Qtrue
#define Blas_NonUnit	Qfalse
#define Blas_Left	Qtrue
#define Blas_Right	Qfalse

/* BLAS Support */

/* Level 1 */

/* scalar product x^T y */
static VALUE Blas_ddot(VALUE obj, VALUE bb) {
  gsl_vector * a, * b;
  double x;
  Data_Get_Struct(obj, gsl_vector, a);
  Data_Get_Struct(bb, gsl_vector, b);
  gsl_blas_ddot(a, b, &x);
  return(rb_float_new(x));
}



/* Euclidean norm ||x||_2 = \sqrt {\sum x_i^2} */
static VALUE Blas_dnrm2(VALUE obj) {
  gsl_vector * a;
  double x;
  Data_Get_Struct(obj, gsl_vector, a);
  x = gsl_blas_dnrm2(a);
  return(rb_float_new(x));
}

static VALUE Blas_dasum(VALUE obj) {
  gsl_vector * a;
  double x;
  Data_Get_Struct(obj, gsl_vector, a);
  x = gsl_blas_dasum(a);
  return(rb_float_new(x));
}


/* largest element of the vector x by absolute magnitude */
static VALUE Blas_idamax(VALUE obj) {
  gsl_vector * a;
  size_t i;
  Data_Get_Struct(obj, gsl_vector, a);
  i = gsl_blas_idamax(a);
  return(INT2FIX(i));
}


static VALUE Blas_dswap(VALUE obj, VALUE va, VALUE vb) {
  gsl_vector * a, * b;
  Data_Get_Struct(va, gsl_vector, a);
  Data_Get_Struct(vb, gsl_vector, b);
  gsl_blas_dswap(a, b);
  return obj;
} 


static VALUE Blas_dcopy(VALUE obj, VALUE va) {
  gsl_vector * a, * v;
  Data_Get_Struct(va, gsl_vector, a);
  Data_Get_Struct(obj, gsl_vector, v);
  gsl_blas_dcopy(v, a);
  return obj;
} 



/* y = \alpha x + y */
static VALUE Blas_daxpy_bang(VALUE obj, VALUE xx, VALUE va) {
  gsl_vector * x, * y;
  Data_Get_Struct(obj, gsl_vector, y);
  Data_Get_Struct(xx, gsl_vector, x);
  gsl_blas_daxpy(NUM2DBL(va), x, y);
  return  obj;
}

/* y' = \alpha x + y */
static VALUE Blas_daxpy(VALUE obj, VALUE xx, VALUE va) {
  gsl_vector * x, * y, * new;
  Data_Get_Struct(obj, gsl_vector, y);
  Data_Get_Struct(xx, gsl_vector, x);
  new = gsl_vector_alloc(x->size);
  gsl_vector_memcpy(new, y);
  gsl_blas_daxpy(NUM2DBL(va), x, new);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
}


/* This function computes a Givens rotation (c,s) which zeroes the vector (a,b),
 * <pre>
 *     [  c  s ] [ a ] = [ r ]
 *     [ -s  c ] [ b ]   [ 0 ]
 *     c = a/r; s = b/r; r = sqrt(a^2 + b^2)
 */    
static VALUE Blas_drotg(VALUE obj, VALUE aa, VALUE bb) {
  double a, b, c, s;
  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  gsl_blas_drotg(&a, &b, &c, &s);
  return rb_ary_new3(3, rb_float_new(c), rb_float_new(s), rb_float_new(a));
}


static VALUE Blas_drot(VALUE obj, VALUE yy, VALUE cc, VALUE ss) {
  gsl_vector * x, * y, * nx, * ny;
  double c, s;
  VALUE vx, vy;
  c = NUM2DBL(cc);
  s = NUM2DBL(ss);
  Data_Get_Struct(obj, gsl_vector, x);
  Data_Get_Struct(yy, gsl_vector, y);
  nx = gsl_vector_alloc(x->size);
  ny = gsl_vector_alloc(x->size);
  gsl_blas_drot(x, y, c, s);
  gsl_vector_memcpy(nx, x);
  gsl_vector_memcpy(ny, y);
  vx = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, nx);
  vy = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, ny);
  return rb_ary_new3(2, vx, vy);
}

/*  modified Given's transformation 
 *  http://www.unet.univie.ac.at/aix/libs/basetrf2/SROTMG.htm
 *  working ?
 */
static VALUE Blas_drotmg(VALUE obj, VALUE dd1, VALUE dd2, VALUE aa, VALUE bb) {
  double a, b, d1, d2, p[5];
  gsl_matrix * m;
  m = gsl_matrix_calloc(2,2);    
  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  d1 = NUM2DBL(dd1);
  d2 = NUM2DBL(dd2);
  gsl_blas_drotmg(&d1, &d2, &a, b, p);
  /* Case 1: PARAM(1) = 1.0
   * H(1,2) = H(2,1) = -1.0;  H(1,1) = PARAM(2);  H(2,2) = PARAM(5)
   */
  if (p[0] == 1) {
    gsl_matrix_set(m, 0,0, p[1]);	gsl_matrix_set(m, 0,1, -1);
    gsl_matrix_set(m, 1,0, -1);		gsl_matrix_set(m, 1,1, p[4]);
  }
  /* Case 2: PARAM(1) = 0.0 
   * H(1,1) = H(2,2) = 1.0; H(2,1) = PARAM(3); H(1,2) = PARAM(4)
   */
  if (p[0] == 0) {
    gsl_matrix_set(m, 0,0, 1);		gsl_matrix_set(m, 0,1, p[3]);
    gsl_matrix_set(m, 1,0, p[2]);	gsl_matrix_set(m, 1,1, 1);
  }
  /* Case 3: PARAM(1) = -1.0
   * H(1,1) = PARAM(2); H(2,1) = PARAM(3); H(1,2) = PARAM(4); H(2,2) = PARAM(5)
   */
  if (p[0] == -1) {
     gsl_matrix_set(m,0,0, p[1]);	gsl_matrix_set(m,0,1, p[3]);
     gsl_matrix_set(m,1,0, p[2]);	gsl_matrix_set(m,1,1, p[4]);
     }
/* Case 4: PARAM(1) = -2.0 */
  if (p[0] == -2) {
    gsl_matrix_set_identity(m);
  }
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, m);
}


/* Level 2 */
/* y' = alpha op(A) x + beta y */
static VALUE Blas_dgemv(VALUE obj, VALUE tta, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix * a;
  gsl_vector * x, * y, * new;
  double alpha, beta;
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(obj, gsl_vector, y);
  new = gsl_vector_alloc(a->size1);
  gsl_vector_memcpy(new, y);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (tta == Qfalse) {
    gsl_blas_dgemv(CblasNoTrans,alpha, a, x, beta, new);
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  if (tta == Qtrue) {
    gsl_blas_dgemv(CblasTrans,alpha, a, x, beta, new);
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  return Qnil;
}



/* y = alpha op(A) x + beta y */
static VALUE Blas_dgemv_bang(VALUE obj, VALUE tta, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix * a;
  gsl_vector * x, * y;
  double alpha, beta;
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(obj, gsl_vector, y);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (tta == Qfalse) {
    gsl_blas_dgemv(CblasNoTrans,alpha, a, x, beta, y);
    return obj;
  }
  if (tta == Qtrue) {
    gsl_blas_dgemv(CblasTrans,alpha, a, x, beta, y);
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
static VALUE Blas_dtrmv(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix * a;
  gsl_vector * x, * new;
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_vector, x);
  new = gsl_vector_alloc(x->size);
  gsl_vector_memcpy(new, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
	  if (vd == Qtrue)
        gsl_blas_dtrmv (CblasUpper, CblasTrans, CblasUnit, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrmv (CblasUpper, CblasTrans, CblasNonUnit, a, new);
	}
	  if (tta == Qfalse) { 
	    if (vd == Qtrue)
          gsl_blas_dtrmv (CblasUpper, CblasNoTrans, CblasUnit, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrmv (CblasUpper, CblasNoTrans, CblasNonUnit, a, new);
	  }
     return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrmv (CblasLower, CblasTrans, CblasUnit, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrmv (CblasLower, CblasTrans, CblasNonUnit, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrmv (CblasLower, CblasNoTrans, CblasUnit, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrmv (CblasLower, CblasNoTrans, CblasNonUnit, a, new);
	  }
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  return Qnil;
}


/* This method computes x = op(A) x ,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */

static VALUE Blas_dtrmv_bang(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix * a;
  gsl_vector * x;
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_vector, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
	  if (vd == Qtrue)
        gsl_blas_dtrmv (CblasUpper, CblasTrans, CblasUnit, a, x);
	  if (vd == Qfalse)
        gsl_blas_dtrmv (CblasUpper, CblasTrans, CblasNonUnit, a, x);
	}
	  if (tta == Qfalse) { 
	    if (vd == Qtrue)
          gsl_blas_dtrmv (CblasUpper, CblasNoTrans, CblasUnit, a, x);
	    if (vd == Qfalse)
          gsl_blas_dtrmv (CblasUpper, CblasNoTrans, CblasNonUnit, a, x);
	  }
     return obj;
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrmv (CblasLower, CblasTrans, CblasUnit, a, x);
	  if (vd == Qfalse)
        gsl_blas_dtrmv (CblasLower, CblasTrans, CblasNonUnit, a, x);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrmv (CblasLower, CblasNoTrans, CblasUnit, a, x);
	    if (vd == Qfalse)
          gsl_blas_dtrmv (CblasLower, CblasNoTrans, CblasNonUnit, a, x);
	  }
    return obj;
  }
  return Qnil;
}

/* This method computes x' = inv(op(A)) x ,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_dtrsv(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix * a;
  gsl_vector * x, * new;
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_vector, x);
  new = gsl_vector_alloc(x->size);
  gsl_vector_memcpy(new, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
	  if (vd == Qtrue)
        gsl_blas_dtrsv (CblasUpper, CblasTrans, CblasUnit, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrsv (CblasUpper, CblasTrans, CblasNonUnit, a, new);
	}
	  if (tta == Qfalse) { 
	    if (vd == Qtrue)
          gsl_blas_dtrsv (CblasUpper, CblasNoTrans, CblasUnit, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrsv (CblasUpper, CblasNoTrans, CblasNonUnit, a, new);
	  }
     return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrsv (CblasLower, CblasTrans, CblasUnit, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrsv (CblasLower, CblasTrans, CblasNonUnit, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrsv (CblasLower, CblasNoTrans, CblasUnit, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrsv (CblasLower, CblasNoTrans, CblasNonUnit, a, new);
	  }
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  return Qnil;
}




/* This method computes x = inv(op(A)) x ,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_dtrsv_bang(VALUE obj, VALUE vup, VALUE tta, VALUE vd, VALUE aa)
{
  gsl_matrix * a;
  gsl_vector * x;
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_vector, x);
  if (vup == Qtrue) {
    if (tta == Qtrue) {
	  if (vd == Qtrue)
        gsl_blas_dtrsv (CblasUpper, CblasTrans, CblasUnit, a, x);
	  if (vd == Qfalse)
        gsl_blas_dtrsv (CblasUpper, CblasTrans, CblasNonUnit, a, x);
	}
	  if (tta == Qfalse) { 
	    if (vd == Qtrue)
          gsl_blas_dtrsv (CblasUpper, CblasNoTrans, CblasUnit, a, x);
	    if (vd == Qfalse)
          gsl_blas_dtrsv (CblasUpper, CblasNoTrans, CblasNonUnit, a, x);
	  }
     return obj;
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrsv (CblasLower, CblasTrans, CblasUnit, a, x);
	  if (vd == Qfalse)
        gsl_blas_dtrsv (CblasLower, CblasTrans, CblasNonUnit, a, x);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrsv (CblasLower, CblasNoTrans, CblasUnit, a, x);
	    if (vd == Qfalse)
          gsl_blas_dtrsv (CblasLower, CblasNoTrans, CblasNonUnit, a, x);
	  }
    return obj;
  }
  return Qnil;
}

/* y' = alpha op(A) x + beta y 
 * for symmetric matrix A
 */
static VALUE Blas_dsymv(VALUE obj, VALUE vup, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix * a;
  gsl_vector * x, * y, * new;
  double alpha, beta;
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(obj, gsl_vector, y);
  new = gsl_vector_alloc(a->size1);
  gsl_vector_memcpy(new, y);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (vup == Qfalse) {
    gsl_blas_dsymv(CblasLower,alpha, a, x, beta, new);
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  if (vup == Qtrue) {
    gsl_blas_dsymv(CblasUpper,alpha, a, x, beta, new);
    return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  }
  return Qnil;
}


/* y = alpha op(A) x + beta y 
 * for symmetric matrix A
 */
static VALUE Blas_dsymv_bang(VALUE obj, VALUE vup, VALUE aalpha,
                       VALUE va, VALUE xx, VALUE bbeta)
{
  gsl_matrix * a;
  gsl_vector * x, * y;
  double alpha, beta;
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(obj, gsl_vector, y);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (vup == Qfalse) {
    gsl_blas_dsymv(CblasLower,alpha, a, x, beta, y);
    return obj;
  }
  if (vup == Qtrue) {
    gsl_blas_dsymv(CblasUpper,alpha, a, x, beta, y);
    return obj;
  }
  return Qnil;
}

static VALUE Blas_dger(VALUE obj, VALUE aalpha, VALUE xx, VALUE yy) {
  gsl_matrix * a, * new;
  gsl_vector * x, * y;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(yy, gsl_vector, y);
  new = gsl_matrix_alloc(a->size1, a->size2);
  gsl_matrix_memcpy(new, a);
  gsl_blas_dger(NUM2DBL(aalpha), x, y, new);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
}


/* 
 * update A' = \alpha x x^T + A for symmetric matrix A
 */
static VALUE Blas_dsyr(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx)
{
  gsl_matrix * a, * new;
  gsl_vector * x;
  double alpha;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  new = gsl_matrix_alloc(a->size1, a->size2);
  gsl_matrix_memcpy(new, a);
  alpha = NUM2DBL(aalpha);
  if (vup == Qfalse) {
    gsl_blas_dsyr(CblasLower, alpha, x, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  if (vup == Qtrue) {
    gsl_blas_dsyr(CblasUpper,alpha, x, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  return Qnil;
}


/* 
 * update A = \alpha x x^T + A for symmetric matrix A
 */
static VALUE Blas_dsyr_bang(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx)
{
  gsl_matrix * a;
  gsl_vector * x;
  double alpha;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  alpha = NUM2DBL(aalpha);
  if (vup == Qfalse) {
    gsl_blas_dsyr(CblasLower, alpha, x, a);
    return obj;
  }
  if (vup == Qtrue) {
    gsl_blas_dsyr(CblasUpper,alpha, x, a);
    return obj;
  }
  return Qnil;
}


/* 
 * update A' = alpha x y^T + alpha y x^T + A for symmetric matrix A
 */
static VALUE Blas_dsyr2(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx, VALUE yy)
{
  gsl_matrix * a, * new;
  gsl_vector * x, * y;
  double alpha;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(yy, gsl_vector, y);
  new = gsl_matrix_alloc(a->size1, a->size2);
  gsl_matrix_memcpy(new, a);
  alpha = NUM2DBL(aalpha);
  if (vup == Qfalse) {
    gsl_blas_dsyr2(CblasLower, alpha, x, y, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  if (vup == Qtrue) {
    gsl_blas_dsyr2(CblasUpper,alpha, x, y, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  return Qnil;
}


/* 
 * update A = alpha x y^T + alpha y x^T + A for symmetric matrix A
 */
static VALUE Blas_dsyr2_bang(VALUE obj, VALUE vup, VALUE aalpha, VALUE xx, VALUE yy)
{
  gsl_matrix * a;
  gsl_vector * x, * y;
  double alpha;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(xx, gsl_vector, x);
  Data_Get_Struct(yy, gsl_vector, y);
  alpha = NUM2DBL(aalpha);
  if (vup == Qfalse) {
    gsl_blas_dsyr2(CblasLower, alpha, x, y, a);
    return obj;
  }
  if (vup == Qtrue) {
    gsl_blas_dsyr2(CblasUpper,alpha, x, y, a);
    return  obj;
  }
  return Qnil;
}


/* Level 3 */

/*  matrix-matrix product and sum
 *  C' = \alpha op(A) op(B) + \beta C
 *  where op(A) = A, A^T for TransA = false, true respectively;
 *  similarly for the parameter TransB.
 */

static VALUE Blas_dgemm(VALUE obj, VALUE tta, VALUE ttb, VALUE aalpha, 
			VALUE aa, VALUE bb, VALUE bbeta)  
{
  gsl_matrix * a, * b, * c, * new;
  double alpha, beta;
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(bb, gsl_matrix, b);
  Data_Get_Struct(obj, gsl_matrix, c);
  new = gsl_matrix_alloc(a->size1, b->size2);
  gsl_matrix_memcpy(new, c);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (tta == Qfalse) {
	if (ttb == Qfalse) {
      gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,alpha, a, b, beta, new);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
	}
	if (ttb == Qtrue) {
      gsl_blas_dgemm (CblasNoTrans, CblasTrans,alpha, a, b, beta, new);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
	}
  }
  if (tta == Qtrue) {
	if (ttb == Qfalse) {
      gsl_blas_dgemm (CblasTrans, CblasNoTrans,alpha, a, b, beta, new);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
	}
	if (ttb == Qtrue) {
      gsl_blas_dgemm (CblasTrans, CblasTrans,alpha, a, b, beta, new);
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
	}
  }
  return Qnil; 
}

/* This method computes the matrix-matrix product and sum
 * C = \alpha op(A) op(B) + \beta C
 * where op(A) = A, A^T for TransA = false, true respectively;
 * similarly for the parameter TransB.
 */
static VALUE Blas_dgemm_bang(VALUE obj, VALUE tta, VALUE ttb, VALUE aalpha, 
			VALUE aa, VALUE bb, VALUE bbeta)  
{
  gsl_matrix * a, * b, * c;
  double alpha, beta;
  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(bb, gsl_matrix, b);
  Data_Get_Struct(obj, gsl_matrix, c);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (tta == Qfalse) {
	if (ttb == Qfalse) {
      gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,alpha, a, b, beta, c);
      return obj;
	}
	if (ttb == Qtrue) {
      gsl_blas_dgemm (CblasNoTrans, CblasTrans,alpha, a, b, beta, c);
      return obj;
	}
  }
  if (tta == Qtrue) {
	if (ttb == Qfalse) {
      gsl_blas_dgemm (CblasTrans, CblasNoTrans,alpha, a, b, beta, c);
      return obj;
	}
	if (ttb == Qtrue) {
      gsl_blas_dgemm (CblasTrans, CblasTrans,alpha, a, b, beta, c);
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
static VALUE Blas_dsymm(VALUE obj, VALUE vSideLeft, VALUE vUplo, VALUE aalpha,
                       VALUE va, VALUE vb, VALUE bbeta)
{
  gsl_matrix * a, * b, * c, * new;
  double alpha, beta;
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(vb, gsl_matrix, b);
  Data_Get_Struct(obj, gsl_matrix, c);
  new = gsl_matrix_alloc(c->size1, c->size2);
  gsl_matrix_memcpy(new, c);
  alpha = NUM2DBL(aalpha);
  beta = NUM2DBL(bbeta);
  if (vSideLeft == Qtrue) {
    if (vUplo == Qtrue)
      gsl_blas_dsymm(CblasLeft, CblasUpper, alpha, a, b, beta, new);
     if (vUplo == Qfalse)
       gsl_blas_dsymm(CblasLeft, CblasLower, alpha, a, b, beta, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  if (vSideLeft == Qfalse) {
    if (vUplo == Qtrue)
      gsl_blas_dsymm(CblasRight, CblasUpper, alpha, a, b, beta, new);
     if (vUplo == Qfalse)
       gsl_blas_dsymm(CblasRight, CblasLower, alpha, a, b, beta, new);
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  return Qnil;
}


/* B.dtrmm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'
 * This method computes B' = op(A) B when SideLeft = true,
 * B' = B op(A) when SideLeft = false,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_dtrmm(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
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
        gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
	gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
        gsl_blas_dtrmm (CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrmm (CblasLeft, CblasLower, CblasTrans, CblasUnit, alpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrmm (CblasLeft, CblasLower, CblasTrans, CblasNonUnit, alpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrmm (CblasLeft, CblasLower, CblasNoTrans, CblasUnit, alpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrmm (CblasLeft, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
    return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  if (vs == Qfalse) {
  if (vup == Qtrue) {
    if (tta == Qtrue) {
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasRight, CblasUpper, CblasTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
	gsl_blas_dtrmm (CblasRight, CblasUpper, CblasTrans, CblasNonUnit, alpha, a, new);
	}
    if (tta == Qfalse) { 
      if (vd == Qtrue)
        gsl_blas_dtrmm (CblasRight, CblasUpper, CblasNoTrans, CblasUnit, alpha, a, new);
      if (vd == Qfalse)
        gsl_blas_dtrmm (CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
  if (vup == Qfalse) { 
    if (tta == Qtrue) { 
	  if (vd == Qtrue)
        gsl_blas_dtrmm (CblasRight, CblasLower, CblasTrans, CblasUnit, alpha, a, new);
	  if (vd == Qfalse)
        gsl_blas_dtrmm (CblasRight, CblasLower, CblasTrans, CblasNonUnit, alpha, a, new);
	}
      if (tta == Qfalse) {
	    if (vd == Qtrue)
          gsl_blas_dtrmm (CblasRight, CblasLower, CblasNoTrans, CblasUnit, alpha, a, new);
	    if (vd == Qfalse)
          gsl_blas_dtrmm (CblasRight, CblasLower, CblasNoTrans, CblasNonUnit, alpha, a, new);
	  }
  }
      return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
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
 */
static VALUE Blas_dtrmm_bang(VALUE obj, VALUE vs, VALUE vup, VALUE tta, 
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



/* B.dtrsm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'
 * This method computes B' = op(inv(A)) B when SideLeft = true,
 * B' = B op(inv(A)) when SideLeft = false,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_dtrsm(VALUE obj, VALUE vs, VALUE vup, VALUE tta, VALUE vd, 
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


/* B.dtrsm!(SideLeft, Uplo, TransA, DiagUnit, A) -> B
 * This method computes B = op(inv(A)) B when SideLeft = true,
 * B = B op(inv(A)) when SideLeft = false,
 * where op(A) = A, A^T for TransA = true of false.
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 * If DiagUnit is false then the diagonal of
 * the matrix is used, but if DiagUnit is true then the diagonal elements
 * of the matrix A are taken as unity.
 */
static VALUE Blas_dtrsm_bang(VALUE obj, VALUE vs, VALUE vup, VALUE tta, 
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


/* This method computes 
 * C.dsyr2k(Uplo, Trans, alpha, A, B, beta) -> C'
 * C' = alpha A^T B + alpha B^T A + beta C when Trans = true and
 * C' = alpha A B^T + alpha B A^T + beta C when Trans = false
 * where C is a symmetric matrix
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 */
static VALUE Blas_dsyr2k(VALUE obj, VALUE uplo, VALUE tta, VALUE valpha,
                         VALUE va, VALUE vb, VALUE vbeta)
{
  gsl_matrix * a, * b, * c, * new;
  double alpha = NUM2DBL(valpha);
  double beta = NUM2DBL(vbeta);
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(vb, gsl_matrix, b);
  Data_Get_Struct(obj, gsl_matrix, c);
  new = gsl_matrix_alloc(c->size1, c->size2);
  gsl_matrix_memcpy(new, c);
  if (uplo == Qtrue) {
    if (tta == Qtrue)
        gsl_blas_dsyr2k (CblasUpper, CblasTrans, alpha, a, b, beta, new);
    if (tta == Qfalse) 
        gsl_blas_dsyr2k (CblasUpper, CblasNoTrans, alpha, a, b, beta, new);
     return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  if (uplo == Qfalse) {
    if (tta == Qtrue)
        gsl_blas_dsyr2k (CblasLower, CblasTrans, alpha, a, b, beta, new);
    if (tta == Qfalse) 
        gsl_blas_dsyr2k (CblasLower, CblasNoTrans, alpha, a, b, beta, new);
     return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  }
  return Qnil;
}


/* This method computes 
 * C.dsyr2k(Uplo, Trans, alpha, A, B, beta) -> C
 * C = alpha A^T B + alpha B^T A + beta C when Trans = true and
 * C = alpha A B^T + alpha B A^T + beta C when Trans = false
 * where C is a symmetric matrix
 * When Uplo is true (false) then the upper (lower) triangle of A is used.
 */
static VALUE Blas_dsyr2k_bang(VALUE obj, VALUE uplo, VALUE tta, VALUE valpha,
                         VALUE va, VALUE vb, VALUE vbeta)
{
  gsl_matrix * a, * b, * c;
  double alpha = NUM2DBL(valpha);
  double beta = NUM2DBL(vbeta);
  Data_Get_Struct(va, gsl_matrix, a);
  Data_Get_Struct(vb, gsl_matrix, b);
  Data_Get_Struct(obj, gsl_matrix, c);
  if (uplo == Qtrue) {
    if (tta == Qtrue)
        gsl_blas_dsyr2k (CblasUpper, CblasTrans, alpha, a, b, beta, c);
    if (tta == Qfalse) 
        gsl_blas_dsyr2k (CblasUpper, CblasNoTrans, alpha, a, b, beta, c);
     return obj;
  }
  if (uplo == Qfalse) {
    if (tta == Qtrue)
        gsl_blas_dsyr2k (CblasLower, CblasTrans, alpha, a, b, beta, c);
    if (tta == Qfalse) 
        gsl_blas_dsyr2k (CblasLower, CblasNoTrans, alpha, a, b, beta, c);
     return obj;
  }
  return Qnil;
}

/* Module definition */
VALUE rbgsl_mBlas;

void Init_Blas() {
   rbgsl_mBlas = rb_define_module_under(rbgsl_mGSL, "Blas");
   
  /* level 1 */ 
  rb_define_method(rbgsl_cVector, "ddot", Blas_ddot, 1);
  rb_define_alias(rbgsl_cVector, "dot", "ddot");
  rb_define_method(rbgsl_cVector, "dnrm2", Blas_dnrm2, 0);
  rb_define_alias(rbgsl_cVector, "norm", "dnrm2");
  rb_define_method(rbgsl_cVector, "dasum", Blas_dasum, 0);

  rb_define_method(rbgsl_cVector, "idamax", Blas_idamax, 0);
  rb_define_module_function(rbgsl_mBlas, "dswap", Blas_dswap, 2);
  rb_define_method(rbgsl_cVector, "dcopy", Blas_dcopy, 1);
  rb_define_method(rbgsl_cVector, "daxpy!", Blas_daxpy_bang, 2);
  rb_define_method(rbgsl_cVector, "daxpy", Blas_daxpy, 2);
  
  rb_define_module_function(rbgsl_mBlas, "drotg", Blas_drotg, 2);
  rb_define_method(rbgsl_cVector, "drot",  Blas_drot, 3);
  rb_define_module_function(rbgsl_mBlas, "drotmg", Blas_drotmg, 4);

  /* level 2 */
  rb_define_method(rbgsl_cVector, "dgemv", Blas_dgemv, 5);
  rb_define_method(rbgsl_cVector, "dgemv!", Blas_dgemv_bang, 5);
  rb_define_method(rbgsl_cVector, "dtrmv", Blas_dtrmv, 4);
  rb_define_method(rbgsl_cVector, "dtrmv!", Blas_dtrmv_bang, 4);
  rb_define_method(rbgsl_cVector, "dtrsv", Blas_dtrsv, 4);
  rb_define_method(rbgsl_cVector, "dtrsv!", Blas_dtrsv_bang, 4);
  rb_define_method(rbgsl_cVector, "dsymv", Blas_dsymv, 5);
  rb_define_method(rbgsl_cVector, "dsymv!", Blas_dsymv_bang, 5);
  rb_define_method(rbgsl_cMatrix, "dger", Blas_dger, 3);
  rb_define_method(rbgsl_cMatrix, "dsyr", Blas_dsyr, 3);
  rb_define_method(rbgsl_cMatrix, "dsyr!", Blas_dsyr_bang, 3);
  rb_define_method(rbgsl_cMatrix, "dsyr2", Blas_dsyr2, 4);
  rb_define_method(rbgsl_cMatrix, "dsyr2!", Blas_dsyr2_bang, 4);
  
  /* level 3 */
  rb_define_method(rbgsl_cMatrix, "dgemm", Blas_dgemm, 6);
  rb_define_method(rbgsl_cMatrix, "dgemm!", Blas_dgemm_bang, 6);
  rb_define_method(rbgsl_cMatrix, "dsymm", Blas_dsymm, 6);
  rb_define_method(rbgsl_cMatrix, "dtrmm", Blas_dtrmm, 6);
  rb_define_method(rbgsl_cMatrix, "dtrmm!", Blas_dtrmm_bang, 6);
  rb_define_method(rbgsl_cMatrix, "dtrsm", Blas_dtrsm, 6);
  rb_define_method(rbgsl_cMatrix, "dtrsm!", Blas_dtrsm_bang, 6);
  rb_define_method(rbgsl_cMatrix, "dsyr2k", Blas_dsyr2k, 6);
  rb_define_method(rbgsl_cMatrix, "dsyr2k!", Blas_dsyr2k_bang, 6);

  rb_define_const(rbgsl_mBlas, "Trans", Blas_Trans);
  rb_define_const(rbgsl_mBlas, "NoTrans", Blas_NoTrans);
  rb_define_const(rbgsl_mBlas, "ConjTrans", Blas_ConjTrans);
  rb_define_const(rbgsl_mBlas, "Upper", Blas_Upper);
  rb_define_const(rbgsl_mBlas, "Lower", Blas_Lower);
  rb_define_const(rbgsl_mBlas, "Unit", Blas_Unit);
  rb_define_const(rbgsl_mBlas, "NonUnit", Blas_NonUnit);
  rb_define_const(rbgsl_mBlas, "Left", Blas_Left);
  rb_define_const(rbgsl_mBlas, "Right", Blas_Right);
}      
