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

/* $Id: Linalg_complex.c,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_vector_complex.h"
#include "gsl/gsl_min.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Linear Algebra */

/* LU Decomposition */

/* LU decomposition: original matrix is changed. */
static VALUE Linalg_complex_LU_decomp(VALUE obj)
{
  gsl_matrix_complex *m;
  gsl_permutation *p;
  int signum;
  size_t size;
  VALUE obj2;
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  size = m->size1;

  p = gsl_permutation_alloc(size);
  
  gsl_linalg_complex_LU_decomp(m, p, &signum);

  obj2 = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, p);

  return rb_ary_new3(3, obj, obj2, INT2FIX(signum));
}

/* LU decomposition: original matrix_complex is not changed. 
 */
static VALUE Linalg_complex_LU_decomp2(VALUE obj)
{
  gsl_matrix_complex *m, *m2;
  gsl_permutation *p;
  int signum;
  size_t size;
  VALUE obj2, obj3;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  size = m->size1;
  m2 = gsl_matrix_complex_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_complex_memcpy(m2, m);

  gsl_linalg_complex_LU_decomp(m2, p, &signum);

  obj2 = Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, m2);
  obj3 = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, p);

  return rb_ary_new3(3, obj2, obj3, INT2FIX(signum));
}

static VALUE Linalg_complex_LU_solve(VALUE obj, VALUE pp, VALUE bb)
{
  gsl_matrix_complex *m;
  gsl_permutation *p;
  gsl_vector_complex *x, *b;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  size = m->size1;
  x = gsl_vector_complex_alloc(size);
  Data_Get_Struct(pp, gsl_permutation, p);

  gsl_linalg_complex_LU_solve(m, p, b, x);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, x);
}

static VALUE Linalg_complex_LU_decomp_solve(VALUE obj, VALUE bb)
{
  gsl_matrix_complex *m;
  gsl_permutation *p;
  gsl_vector_complex *b, *x;
  int signum;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  size = m->size1;
  p = gsl_permutation_alloc(size);
  x = gsl_vector_complex_alloc(size);

  gsl_linalg_complex_LU_decomp(m, p, &signum);
  gsl_linalg_complex_LU_solve(m, p, b, x);

  gsl_permutation_free(p);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, x);
}

static VALUE Linalg_complex_LUD_decomp_solve(VALUE obj, VALUE bb)
{
  gsl_matrix_complex *m, *m2;
  gsl_permutation *p;
  gsl_vector_complex *b, *x;
  int signum;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  size = m->size1;
  m2 = gsl_matrix_complex_alloc(size, size);
  gsl_matrix_complex_memcpy(m2, m);
  p = gsl_permutation_alloc(size);
  x = gsl_vector_complex_alloc(size);

  gsl_linalg_complex_LU_decomp(m2, p, &signum);
  gsl_linalg_complex_LU_solve(m2, p, b, x);

  gsl_permutation_free(p);
  gsl_matrix_complex_free(m2);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, x);
}


static VALUE Linalg_complex_LU_svx(VALUE obj, VALUE vm, VALUE pp)
{
  gsl_matrix_complex *m;
  gsl_permutation *p;
  gsl_vector_complex * b, * new;

  Data_Get_Struct(vm, gsl_matrix_complex, m);
  Data_Get_Struct(obj, gsl_vector_complex, b);
  Data_Get_Struct(pp, gsl_permutation, p);
  new = gsl_vector_complex_alloc(b->size);
  gsl_vector_complex_memcpy(new, b);
  
  gsl_linalg_complex_LU_svx(m, p, new);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
}

static VALUE Linalg_complex_LU_svx_bang(VALUE obj, VALUE vm, VALUE pp)
{
  gsl_matrix_complex *m;
  gsl_permutation *p;
  gsl_vector_complex * b;

  Data_Get_Struct(vm, gsl_matrix_complex, m);
  Data_Get_Struct(obj, gsl_vector_complex, b);
  Data_Get_Struct(pp, gsl_permutation, p);
  
  gsl_linalg_complex_LU_svx(m, p, b);

  return obj;
}

static VALUE Linalg_complex_LU_refine(VALUE obj, VALUE lu, VALUE pp, VALUE bb)
{
  gsl_matrix_complex *m, *mlu;
  gsl_permutation *p;
  gsl_vector_complex *b, *x, *r;
  size_t size;
  VALUE vx, vr;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  Data_Get_Struct(lu, gsl_matrix_complex, mlu);
  size = m->size1;

  x = gsl_vector_complex_alloc(size);
  r = gsl_vector_complex_alloc(size);
  Data_Get_Struct(pp, gsl_permutation, p);

  gsl_linalg_complex_LU_refine(m, mlu, p, b, x, r);
	
  vx = Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, x);
  vr = Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, r);

  return rb_ary_new3(2, vx, vr);
}

static VALUE Linalg_complex_LU_invert(VALUE obj, VALUE pp)
{
  gsl_matrix_complex *m, *inverse;
  gsl_permutation *p;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  Data_Get_Struct(pp, gsl_permutation, p);
  size = m->size1;
  inverse = gsl_matrix_complex_alloc(size, size);

  gsl_linalg_complex_LU_invert(m, p, inverse);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, inverse);
}


static VALUE Linalg_complex_invert(VALUE obj)
{
  gsl_matrix_complex *m, *m2, *inverse;
  gsl_permutation *p;
  int signum;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  size = m->size1;
  m2 = gsl_matrix_complex_alloc(size, size);
  p = gsl_permutation_alloc(size);
  inverse = gsl_matrix_complex_alloc(size, size);

  gsl_matrix_complex_memcpy(m2, m);

  gsl_linalg_complex_LU_decomp(m2, p, &signum);
  gsl_linalg_complex_LU_invert(m2, p, inverse);

  gsl_matrix_complex_free(m2);
  gsl_permutation_free(p);

  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, inverse);
}


static VALUE Linalg_complex_LU_det(VALUE obj, VALUE s)
{
  gsl_matrix_complex *lu;
  int signum;
  gsl_complex z;

  Data_Get_Struct(obj, gsl_matrix_complex, lu);
  signum = NUM2INT(s);
  z = gsl_linalg_complex_LU_det(lu, signum);
  return Complex_new_intern(rbgsl_cComplex, &z);
}

static VALUE Linalg_complex_det(VALUE obj)
{
  gsl_matrix_complex *m, *lu;
  gsl_permutation *p;
  int signum;
  size_t size;
  gsl_complex det;

  Data_Get_Struct(obj, gsl_matrix_complex, m);
  size = m->size1;
  lu = gsl_matrix_complex_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_complex_memcpy(lu, m);

  gsl_linalg_complex_LU_decomp(lu, p, &signum);

  det = gsl_linalg_complex_LU_det(lu, signum);

  gsl_matrix_complex_free(lu);
  gsl_permutation_free(p);
  return Complex_new_intern(rbgsl_cComplex, &det);
}

static VALUE Linalg_complex_LU_lndet(VALUE obj)
{
  gsl_matrix_complex *lu;
  double lndet;
  
  Data_Get_Struct(obj, gsl_matrix_complex, lu);
  lndet = gsl_linalg_complex_LU_lndet(lu);
  return rb_float_new(lndet);
}

static VALUE Linalg_complex_lndet(VALUE obj)
{
  gsl_matrix_complex *m, *lu;
  gsl_permutation *p;
  int signum;
  size_t size;
  double lndet;
  
  Data_Get_Struct(obj, gsl_matrix_complex, m);
  size = m->size1;
  lu = gsl_matrix_complex_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_complex_memcpy(lu, m);
  gsl_linalg_complex_LU_decomp(lu, p, &signum);
  lndet = gsl_linalg_complex_LU_lndet(lu);

  gsl_matrix_complex_free(lu);
  gsl_permutation_free(p);
  return rb_float_new(lndet);
}

static VALUE Linalg_complex_LU_sgndet(VALUE obj, VALUE s)
{
  gsl_matrix_complex *lu;
  int signum;
  gsl_complex z;

  Data_Get_Struct(obj, gsl_matrix_complex, lu);
  signum = NUM2INT(s);
  z = gsl_linalg_complex_LU_sgndet(lu, signum);
  return Complex_new_intern(rbgsl_cComplex, &z);
}





/* Tridiagonal Decomposition of Hermitian Matrices */
static VALUE Linalg_complex_hermtd_decomp_bang(VALUE obj)
{
  gsl_matrix_complex *A;
  gsl_vector_complex *tau;

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  tau = gsl_vector_complex_alloc(A->size1 - 1);
  gsl_linalg_hermtd_decomp(A, tau);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, tau);
}

static VALUE Linalg_complex_hermtd_decomp(VALUE obj)
{
  gsl_matrix_complex *A, *Anew;
  gsl_vector_complex *tau;
  VALUE va, vtau;

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  Anew = gsl_matrix_complex_alloc(A->size1, A->size2);
  gsl_matrix_complex_memcpy(Anew, A);
  tau = gsl_vector_complex_alloc(A->size1 - 1);
  gsl_linalg_hermtd_decomp(Anew, tau);
  vtau = Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, tau);
  va = Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, Anew);
    
  return rb_ary_new3(2, va, vtau);
}

static VALUE Linalg_complex_hermtd_unpack(VALUE obj, VALUE tt)
{
  gsl_matrix_complex *A, *Q;
  gsl_vector_complex *tau;
  gsl_vector * d, * sd;
  VALUE vq, vd, vsd;

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  Data_Get_Struct(tt, gsl_vector_complex, tau);
  Q = gsl_matrix_complex_alloc(A->size1, A->size2);
  d = gsl_vector_alloc(A->size1);
  sd = gsl_vector_alloc(tau->size);
  gsl_linalg_hermtd_unpack(A, tau, Q, d, sd);
  vq = Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, Q);
  vd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, d);
  vsd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, sd);

  return rb_ary_new3(3, vq, vd, vsd);
}


static VALUE Linalg_complex_hermtd_unpack_T(VALUE obj)
{
  gsl_matrix_complex *A;
  gsl_vector *d, *sd;
  VALUE vd, vsd;

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  d = gsl_vector_alloc(A->size1);
  sd = gsl_vector_alloc(A->size1-1);
  gsl_linalg_hermtd_unpack_T(A, d, sd);

  vd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, d);
  vsd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, sd);

  return rb_ary_new3(2, vd, vsd);
}



/* Householder Transformations */
static VALUE Linalg_complex_householder_transform(VALUE obj) {
  gsl_vector_complex * v, * new;
  gsl_complex z;
  VALUE vf, vnew;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  new = gsl_vector_complex_alloc(v->size);
  gsl_vector_complex_memcpy(new, v);
  z = gsl_linalg_complex_householder_transform(new);
  vf = Complex_new_intern(rbgsl_cComplex, &z);
  vnew =  Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
  return rb_ary_new3(2, vf, vnew); 
}

/*
static VALUE Linalg_complex_householder_transform_bang(VALUE obj) {
  gsl_vector_complex * v;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  return rb_float_new(gsl_linalg_complex_householder_transform(v));
}
*/

/* TODO: should these take a matrix_complex as argument or return one? */
/*
static VALUE Linalg_complex_householder_hm(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector_complex * v;
  gsl_matrix_complex * A;

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_linalg_complex_householder_hm(NUM2DBL(tau), v, A);
  return obj;
}

static VALUE Linalg_complex_householder_mh(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector_complex * v;
  gsl_matrix_complex * A;

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_linalg_complex_householder_mh(NUM2DBL(tau), v, A);
  return obj;
}

static VALUE Linalg_complex_householder_hv_bang(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector_complex * v, * w;

  Data_Get_Struct(obj, gsl_vector_complex, w);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_linalg_complex_householder_hv(NUM2DBL(tau), v, w);
  return obj;
}

static VALUE Linalg_complex_householder_hv(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector_complex * v, * w, * new;

  Data_Get_Struct(obj, gsl_vector_complex, w);
  Data_Get_Struct(vv, gsl_vector_complex, v);
  new = gsl_vector_complex_alloc(v->size);
  gsl_vector_complex_memcpy(new, w);
  gsl_linalg_complex_householder_hv(NUM2DBL(tau), v, new);
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
}

static VALUE Linalg_complex_householder_hm1(VALUE obj, VALUE tau) {
  gsl_matrix_complex * A;

  Data_Get_Struct(obj, gsl_matrix_complex, A);

  gsl_linalg_complex_householder_hm1(NUM2DBL(tau), A);
  return obj;
}
*/

/* Upper part of a matrix_complex, including the diagonal */
static VALUE Matrix_complex_U(VALUE obj)
{
  int i,j,n,n2;
  gsl_matrix_complex * a, * b;
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
    b = gsl_matrix_complex_calloc(a->size1, b->size2);
    n = (a->size1 <= a->size2)? a->size1: a->size2;
	n2 = b->size2;
  for (i = 0; i < n; ++i) {
	for(j = i; j < n2; j++) {
	  gsl_matrix_complex_set(b,i,j, gsl_matrix_complex_get(a,i,j));
	}
  }
  
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, b);
}

/* m' = m.upper(true); m' is the upper part of m, including the diagonal */
/*
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
*/

/* Lower part of a matrix_complex, with unit diagonal */
static VALUE Matrix_complex_L(VALUE obj)
{
  size_t i,j;
  gsl_matrix_complex * a, * b;
  gsl_complex z1;
  GSL_SET_COMPLEX(&z1, 1, 0);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  Data_Get_Struct(obj, gsl_matrix_complex, b);
  if (a->size2 != a->size1)
	 rb_raise(rb_eIOError, "Only square matrix_complex allowed for the moment");
  b = gsl_matrix_complex_calloc(a->size1, b->size2);
  for (i = 0; i < a->size1; ++i) {
	for(j = 0; j < i; j++) {
	  gsl_matrix_complex_set(b,i,j, gsl_matrix_complex_get(a,i,j));
	}
	if (j == i)
	  gsl_matrix_complex_set(b,i,j,z1);
  }
  return Data_Wrap_Struct(rbgsl_cMatrix_complex, 0, gsl_matrix_complex_free, b);
}

/* m' = m.lower(true); m' is the lower part of m, including the diagonal */
/*
static VALUE Matrix_complex_lower(VALUE obj, VALUE vdiag)
{
  int i,j,n;
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
*/

static VALUE Linalg_complex_trace(VALUE obj)
{
  gsl_matrix_complex * a;
  size_t i, n;
  gsl_complex z;
  GSL_SET_COMPLEX(&z,0,0);
  Data_Get_Struct(obj, gsl_matrix_complex, a);
  n = a->size1;
  for (i=0; i < n; i++)
	z = gsl_complex_add(z, gsl_matrix_complex_get(a,i,i)); 
  return Complex_new_intern(rbgsl_cComplex, &z);
}

/* Utilities */

/* w.householder_hv!(tau, v) -> w
 * w = w - tau * (v^H w) * v
 */
#ifdef COMPLEX_HV
static VALUE Linalg_complex_householder_hv_bang(VALUE obj, VALUE vtau, VALUE vv) {
  gsl_vector_complex * v, * w;
  gsl_complex * ptau;
  Data_Get_Struct(vtau, gsl_complex, ptau);

  Data_Get_Struct(obj, gsl_vector_complex, w);
  Data_Get_Struct(vv, gsl_vector_complex, v);
  gsl_linalg_complex_householder_hv(*ptau, v, w);
  return obj;
}

static VALUE Linalg_complex_householder_hv(VALUE obj, VALUE vtau, VALUE vv) 
{
  gsl_vector_complex * v, * w, * new;
  gsl_complex * ptau;
  Data_Get_Struct(vtau, gsl_complex, ptau);

  Data_Get_Struct(obj, gsl_vector_complex, w);
  Data_Get_Struct(vv, gsl_vector_complex, v);
  new = gsl_vector_complex_alloc(v->size);
  gsl_vector_complex_memcpy(new, w);
  gsl_linalg_complex_householder_hv(*ptau, v, new);
  
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
}
#endif

/* P A = (1 - tau v v^H) A = A - v w^H,
 * where w = tau A^H v (tau being real)
 * A.householder_hm(tau, v) -> v
 */
static VALUE Linalg_complex_householder_hm_bang(VALUE obj, VALUE vtau, VALUE vv) {
  gsl_vector_complex * v;
  gsl_matrix_complex * A;
  gsl_complex * ptau;
  Data_Get_Struct(vtau, gsl_complex, ptau);

  Data_Get_Struct(obj, gsl_matrix_complex, A);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_linalg_complex_householder_hm(*ptau, v, A);
  return obj;
}

/* A P = A (1 - tau v v^H) 
 * gsl_linalg_complex_householder_mh absent
static VALUE Linalg_complex_householder_mh_bang(VALUE obj, VALUE vtau, VALUE vv) {
  gsl_vector_complex * v;
  gsl_matrix_complex * A;
  gsl_complex * ptau;
  Data_Get_Struct(vtau, gsl_complex, ptau);
  Data_Get_Struct(obj, gsl_matrix_complex, A);
  Data_Get_Struct(vv, gsl_vector_complex, v);

  gsl_linalg_complex_householder_mh(*ptau, v, A);
  return obj;
}
 */

/* Module definition */

VALUE rbgsl_mLinalg_complex;

void Init_Linalg_complex() {

  rb_define_method(rbgsl_cMatrix_complex, "LU_decomp!", Linalg_complex_LU_decomp, 0);
  rb_define_method(rbgsl_cMatrix_complex, "LU_decomp", Linalg_complex_LU_decomp2, 0);
  rb_define_method(rbgsl_cMatrix_complex, "LU_solve", Linalg_complex_LU_solve, 2);
  rb_define_method(rbgsl_cMatrix_complex, "LU_decomp_solve!", Linalg_complex_LU_decomp_solve, 1);
  rb_define_method(rbgsl_cMatrix_complex, "LUD_solve", Linalg_complex_LUD_decomp_solve, 1);
  rb_define_method(rbgsl_cVector_complex, "LU_svx", Linalg_complex_LU_svx, 2);
  rb_define_method(rbgsl_cVector_complex, "LU_svx!", Linalg_complex_LU_svx_bang, 2);
  rb_define_method(rbgsl_cMatrix_complex, "LU_refine", Linalg_complex_LU_refine, 3);
  rb_define_method(rbgsl_cMatrix_complex, "LU_invert", Linalg_complex_LU_invert, 1);
  rb_define_method(rbgsl_cMatrix_complex, "invert", Linalg_complex_invert, 0);
  rb_define_alias(rbgsl_cMatrix_complex,  "inv", "invert");
  rb_define_method(rbgsl_cMatrix_complex, "LU_det", Linalg_complex_LU_det, 1);
  rb_define_method(rbgsl_cMatrix_complex, "det", Linalg_complex_det, 0);
  rb_define_method(rbgsl_cMatrix_complex, "LU_lndet", Linalg_complex_LU_lndet, 0);
  rb_define_method(rbgsl_cMatrix_complex, "lndet", Linalg_complex_lndet, 0);
  rb_define_method(rbgsl_cMatrix_complex, "LU_sgndet", Linalg_complex_LU_sgndet, 1);
  rb_define_alias(rbgsl_cMatrix_complex, "sgndet", "LU_sgndet");

  rb_define_method(rbgsl_cMatrix_complex, "hermtd_decomp!", Linalg_complex_hermtd_decomp_bang, 0);
  rb_define_method(rbgsl_cMatrix_complex, "hermtd_decomp", Linalg_complex_hermtd_decomp, 0);
  rb_define_method(rbgsl_cMatrix_complex, "hermtd_unpack", Linalg_complex_hermtd_unpack, 1);
  
  rb_define_method(rbgsl_cMatrix_complex, "hermtd_unpack_T", Linalg_complex_hermtd_unpack_T, 0);
  
  rb_define_method(rbgsl_cVector_complex, "householder_transform", Linalg_complex_householder_transform, 0);
  /*
  rb_define_method(rbgsl_cVector_complex, "householder_transform!", Linalg_complex_householder_transform_bang, 0);
  */
#ifdef COMPLEX_HV
  rb_define_method(rbgsl_cVector_complex, "householder_hv!", Linalg_complex_householder_hv_bang, 2);
  rb_define_method(rbgsl_cVector_complex, "householder_hv", Linalg_complex_householder_hv, 2);
#endif
  rb_define_method(rbgsl_cMatrix_complex, "householder_hm!", Linalg_complex_householder_hm_bang, 2);
  /*
  rb_define_method(rbgsl_cMatrix_complex, "householder_mh!", Linalg_complex_householder_mh_bang, 2);
  rb_define_method(rbgsl_cMatrix_complex, "householder_hm1", Linalg_complex_householder_hm1, 1);
*/
  
  rb_define_method(rbgsl_cMatrix_complex, "U", Matrix_complex_U, 0);
  rb_define_method(rbgsl_cMatrix_complex, "L", Matrix_complex_L, 0);
/*
  rb_define_method(rbgsl_cMatrix_complex, "upper", Matrix_complex_upper, 1);
  rb_define_method(rbgsl_cMatrix_complex, "lower", Matrix_complex_lower, 1);
  */
  rb_define_method(rbgsl_cMatrix_complex, "trace", Linalg_complex_trace, 0);
 
  
}

/* vim: set ts=4: */
