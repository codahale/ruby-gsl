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

/* $Id: Linalg.c,v 1.12 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_min.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Linear Algebra */

/* LU Decomposition */

/* LU decomposition: original matrix is changed. */
static VALUE Linalg_LU_decomp(VALUE obj)
{
  gsl_matrix *m;
  gsl_permutation *p;
  int signum;
  size_t size;
  VALUE obj2;

  Data_Get_Struct(obj, gsl_matrix, m);
  size = m->size1;

  p = gsl_permutation_alloc(size);
  
  gsl_linalg_LU_decomp(m, p, &signum);

  obj2 = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, p);

  return rb_ary_new3(3, obj, obj2, INT2FIX(signum));
}

/* LU decomposition: original matrix is not changed. */
static VALUE Linalg_LU_decomp2(VALUE obj)
{
  gsl_matrix *m, *m2;
  gsl_permutation *p;
  int signum;
  size_t size;
  VALUE obj2, obj3;

  Data_Get_Struct(obj, gsl_matrix, m);
  size = m->size1;
  m2 = gsl_matrix_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_memcpy(m2, m);

  gsl_linalg_LU_decomp(m2, p, &signum);

  obj2 = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, m2);
  obj3 = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, p);

  return rb_ary_new3(3, obj2, obj3, INT2FIX(signum));
}

static VALUE Linalg_LU_solve(VALUE obj, VALUE pp, VALUE bb)
{
  gsl_matrix *m;
  gsl_permutation *p;
  gsl_vector *x, *b;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_vector, b);
  size = m->size1;
  x = gsl_vector_alloc(size);
  Data_Get_Struct(pp, gsl_permutation, p);

  gsl_linalg_LU_solve(m, p, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_LU_decomp_solve(VALUE obj, VALUE bb)
{
  gsl_matrix *m;
  gsl_permutation *p;
  gsl_vector *b, *x;
  int signum;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_vector, b);
  size = m->size1;
  p = gsl_permutation_alloc(size);
  x = gsl_vector_alloc(size);

  gsl_linalg_LU_decomp(m, p, &signum);
  gsl_linalg_LU_solve(m, p, b, x);

  gsl_permutation_free(p);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_LUD_decomp_solve(VALUE obj, VALUE bb)
{
  gsl_matrix *m, *m2;
  gsl_permutation *p;
  gsl_vector *b, *x;
  int signum;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_vector, b);
  size = m->size1;
  m2 = gsl_matrix_alloc(size, size);
  gsl_matrix_memcpy(m2, m);
  p = gsl_permutation_alloc(size);
  x = gsl_vector_alloc(size);

  gsl_linalg_LU_decomp(m2, p, &signum);
  gsl_linalg_LU_solve(m2, p, b, x);

  gsl_permutation_free(p);
  gsl_matrix_free(m2);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}


static VALUE Linalg_LU_svx(VALUE obj, VALUE vm, VALUE pp)
{
  gsl_matrix *m;
  gsl_permutation *p;
  gsl_vector * b, * new;

  Data_Get_Struct(vm, gsl_matrix, m);
  Data_Get_Struct(obj, gsl_vector, b);
  Data_Get_Struct(pp, gsl_permutation, p);
  new = gsl_vector_alloc(b->size);
  gsl_vector_memcpy(new, b);
  
  gsl_linalg_LU_svx(m, p, new);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
}

static VALUE Linalg_LU_svx_bang(VALUE obj, VALUE vm, VALUE pp)
{
  gsl_matrix *m;
  gsl_permutation *p;
  gsl_vector * b;

  Data_Get_Struct(vm, gsl_matrix, m);
  Data_Get_Struct(obj, gsl_vector, b);
  Data_Get_Struct(pp, gsl_permutation, p);
  
  gsl_linalg_LU_svx(m, p, b);

  return obj;
}

static VALUE Linalg_LU_refine(VALUE obj, VALUE lu, VALUE pp, VALUE bb)
{
  gsl_matrix *m, *mlu;
  gsl_permutation *p;
  gsl_vector *b, *x, *r;
  size_t size;
  VALUE vx, vr;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(bb, gsl_vector, b);
  Data_Get_Struct(lu, gsl_matrix, mlu);
  size = m->size1;

  x = gsl_vector_alloc(size);
  r = gsl_vector_alloc(size);
  Data_Get_Struct(pp, gsl_permutation, p);

  gsl_linalg_LU_refine(m, mlu, p, b, x, r);
	
  vx = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
  vr = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, r);

  return rb_ary_new3(2, vx, vr);
}

static VALUE Linalg_LU_invert(VALUE obj, VALUE pp)
{
  gsl_matrix *m, *inverse;
  gsl_permutation *p;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix, m);
  Data_Get_Struct(pp, gsl_permutation, p);
  size = m->size1;
  inverse = gsl_matrix_alloc(size, size);

  gsl_linalg_LU_invert(m, p, inverse);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, inverse);
}


static VALUE Linalg_invert(VALUE obj)
{
  gsl_matrix *m, *m2, *inverse;
  gsl_permutation *p;
  int signum;
  size_t size;

  Data_Get_Struct(obj, gsl_matrix, m);
  size = m->size1;
  m2 = gsl_matrix_alloc(size, size);
  p = gsl_permutation_alloc(size);
  inverse = gsl_matrix_alloc(size, size);

  gsl_matrix_memcpy(m2, m);

  gsl_linalg_LU_decomp(m2, p, &signum);
  gsl_linalg_LU_invert(m2, p, inverse);

  gsl_matrix_free(m2);
  gsl_permutation_free(p);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, inverse);
}






static VALUE Linalg_LU_det(VALUE obj, VALUE s)
{
  gsl_matrix *lu;
  int signum;

  Data_Get_Struct(obj, gsl_matrix, lu);
  signum = NUM2INT(s);

  return rb_float_new(gsl_linalg_LU_det(lu, signum));
}

static VALUE Linalg_det(VALUE obj)
{
  gsl_matrix *m, *lu;
  gsl_permutation *p;
  int signum;
  size_t size;
  double det;

  Data_Get_Struct(obj, gsl_matrix, m);
  size = m->size1;
  lu = gsl_matrix_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_memcpy(lu, m);

  gsl_linalg_LU_decomp(lu, p, &signum);

  det = gsl_linalg_LU_det(lu, signum);

  gsl_matrix_free(lu);
  gsl_permutation_free(p);

  return rb_float_new(det);
}

static VALUE Linalg_LU_lndet(VALUE obj)
{
  gsl_matrix *lu;
  
  Data_Get_Struct(obj, gsl_matrix, lu);

  return rb_float_new(gsl_linalg_LU_lndet(lu));
}

static VALUE Linalg_lndet(VALUE obj)
{
  gsl_matrix *m, *lu;
  gsl_permutation *p;
  int signum;
  size_t size;
  double lndet;

  Data_Get_Struct(obj, gsl_matrix, m);
  size = m->size1;
  lu = gsl_matrix_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_memcpy(lu, m);
  gsl_linalg_LU_decomp(lu, p, &signum);
  lndet = gsl_linalg_LU_lndet(lu);

  gsl_matrix_free(lu);
  gsl_permutation_free(p);

  return rb_float_new(lndet);
}

static VALUE Linalg_LU_sgndet(VALUE obj, VALUE s)
{
  gsl_matrix *lu;
  int signum;

  Data_Get_Struct(obj, gsl_matrix, lu);
  signum = NUM2INT(s);

  return INT2FIX(gsl_linalg_LU_sgndet(lu, signum));
}

static VALUE Linalg_sgndet(VALUE obj)
{
  gsl_matrix *m, *lu;
  gsl_permutation *p;
  int signum;
  size_t size;
  int sgndet;

  Data_Get_Struct(obj, gsl_matrix, m);
  size = m->size1;
  lu = gsl_matrix_alloc(size, size);
  p = gsl_permutation_alloc(size);
  
  gsl_matrix_memcpy(lu, m);

  gsl_linalg_LU_decomp(lu, p, &signum);

  sgndet = gsl_linalg_LU_sgndet(lu, signum);

  gsl_matrix_free(lu);
  gsl_permutation_free(p);

  return INT2FIX(sgndet);
}

/* QR Decomposition */

/* QR decomposition: the original matrix is changed. */
static VALUE Linalg_QR_decomp(VALUE obj)
{
  gsl_matrix *m;
  gsl_vector *v;
  size_t size0;

  Data_Get_Struct(obj, gsl_matrix, m);
  size0 = GSL_MIN(m->size1, m->size2);
  v = gsl_vector_alloc(size0);
  gsl_linalg_QR_decomp(m, v);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

/* QR decomposition: the original matrix is not changed. */
static VALUE Linalg_QR_decomp2(VALUE obj)
{
  gsl_matrix *m, *new;
  gsl_vector *v;
  VALUE tau, qr;
  size_t size0;

  Data_Get_Struct(obj, gsl_matrix, m);
  size0 = GSL_MIN(m->size1, m->size2);
  new = gsl_matrix_alloc(m->size1, m->size2);
  v = gsl_vector_alloc(size0);
  gsl_matrix_memcpy(new, m);
  gsl_linalg_QR_decomp(new, v);

  qr = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, new);
  tau = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);

  return rb_ary_new3(2, qr, tau);
}

static VALUE Linalg_QR_solve(VALUE obj, VALUE tt, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *tau, *b, *x;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);
  gsl_linalg_QR_solve(QR, tau, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_QR_svx(VALUE obj, VALUE qr, VALUE tt)
{
  gsl_matrix * QR;
  gsl_vector *tau, *x, * new;

  Data_Get_Struct(qr, gsl_matrix, QR);
  Data_Get_Struct(obj, gsl_vector, x);
  Data_Get_Struct(tt, gsl_vector, tau);
  new = gsl_vector_alloc(x->size);
  gsl_vector_memcpy(new, x);
  
  gsl_linalg_QR_svx(QR, tau, new);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
}

static VALUE Linalg_QR_svx_bang(VALUE obj, VALUE qr, VALUE tt)
{
  gsl_matrix * QR;
  gsl_vector *tau, *x;

  Data_Get_Struct(qr, gsl_matrix, QR);
  Data_Get_Struct(obj, gsl_vector, x);
  Data_Get_Struct(tt, gsl_vector, tau);
  
  gsl_linalg_QR_svx(QR, tau, x);

  return obj;
}

static VALUE Linalg_QR_lssolve(VALUE obj, VALUE tt, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *tau, *b, *x, *res;
  VALUE vx, vres;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(QR->size2);
  res = gsl_vector_alloc(b->size);
  gsl_linalg_QR_lssolve(QR, tau, b, x, res);
  vx = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
  vres = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, res);

  return rb_ary_new3(2, vx, vres);
}


static VALUE Linalg_QR_QTvec(VALUE obj, VALUE tt, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *tau, *v, *b;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Data_Get_Struct(bb, gsl_vector, b);
  v = gsl_vector_alloc(b->size);
  gsl_vector_memcpy(v, b);
  gsl_linalg_QR_QTvec(QR, tau, v);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

static VALUE Linalg_QR_Qvec(VALUE obj, VALUE tt, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *tau, *v, *b;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Data_Get_Struct(bb, gsl_vector, b);
  v = gsl_vector_alloc(b->size);
  gsl_vector_memcpy(v, b);
  gsl_linalg_QR_Qvec(QR, tau, v);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}


static VALUE Linalg_QR_Rsolve(VALUE obj, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *b, *x;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);
  gsl_linalg_QR_Rsolve(QR, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}


static VALUE Linalg_QR_Rsvx(VALUE obj, VALUE xx)
{
  gsl_matrix *QR;
  gsl_vector *x;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(xx, gsl_vector, x);
  gsl_linalg_QR_Rsvx(QR, x);

  return obj;
}

static VALUE Linalg_QR_unpack(VALUE obj, VALUE tt)
{
  gsl_matrix *QR, *Q, *R;
  gsl_vector *tau;
  VALUE vq, vr;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Q = gsl_matrix_alloc(QR->size1, QR->size1);
  R = gsl_matrix_alloc(QR->size1, QR->size2);
  gsl_linalg_QR_unpack(QR, tau, Q, R);
  vq = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Q);
  vr = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, R);

  return rb_ary_new3(2, vq, vr);
}

static VALUE Linalg_QR_QRsolve(VALUE obj, VALUE qq, VALUE rr, VALUE bb)
{
  gsl_matrix *Q, *R;
  gsl_vector *b, *x;

  Data_Get_Struct(qq, gsl_matrix, Q);
  Data_Get_Struct(rr, gsl_matrix, R);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);
  gsl_linalg_QR_QRsolve(Q, R, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

/* FIX ME 
 * int gsl_linalg_QR_update (gsl_matrix * Q, gsl_matrix * R, gsl_vector * w, const gsl_vector * v)
 This function performs a rank-1 update w v^T of the QR decomposition (Q, R). The update is given by Q'R' = Q R + w v^T where the output matrices Q' and R' are also orthogonal and right triangular. Note that w is destroyed by the update.
 
 */
static VALUE Linalg_QR_update(VALUE obj, VALUE qq, VALUE rr, VALUE vw, VALUE vv)
{
  gsl_matrix *Q, *R;
  gsl_vector *w, *v, *new;
  
  Data_Get_Struct(qq, gsl_matrix, Q);
  Data_Get_Struct(rr, gsl_matrix, R);
  Data_Get_Struct(vw, gsl_vector, w);
  Data_Get_Struct(vv, gsl_vector, v);
  new = gsl_vector_alloc(w->size);
  gsl_vector_memcpy(new, w);
  gsl_linalg_QR_update(Q, R, new, v);
  
  return rb_ary_new3(2, qq, rr);
}



static VALUE Linalg_R_solve(VALUE obj, VALUE bb)
{
  gsl_matrix *R;
  gsl_vector *b, *x;

  Data_Get_Struct(obj, gsl_matrix, R);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);
  gsl_linalg_R_solve(R, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_R_svx(VALUE obj, VALUE xx)
{
  gsl_matrix *R;
  gsl_vector *x;

  Data_Get_Struct(obj, gsl_matrix, R);
  x = gsl_vector_alloc(R->size1);
  gsl_linalg_R_svx(R, x);

  return obj;
}

/* QR Decomposition with Column Pivoting */

static VALUE Linalg_QRPT_decomp(VALUE obj)
{
  gsl_matrix *A;
  gsl_vector *tau, *norm;
  gsl_permutation *p;
  int signum;
  size_t size0;
  VALUE vtau, vp, vnorm;

  Data_Get_Struct(obj, gsl_matrix, A);
  size0 = GSL_MIN(A->size1, A->size2);
  tau = gsl_vector_alloc(size0);
  p = gsl_permutation_alloc(size0);
  norm = gsl_vector_alloc(size0);
  gsl_linalg_QRPT_decomp(A, tau, p, &signum, norm);

  vtau = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau);
  vnorm = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, norm);
  vp = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, p);

  return rb_ary_new3(4, vtau, vp, INT2FIX(signum), vnorm);
}

static VALUE Linalg_QRPT_decomp2(VALUE obj)
{
  gsl_matrix *A, *q, *r;
  gsl_vector *tau, *norm;
  gsl_permutation *p;
  int signum;
  size_t size0;
  VALUE vtau, vp, vnorm, vr, vq;

  Data_Get_Struct(obj, gsl_matrix, A);
  size0 = GSL_MIN(A->size1, A->size2);
  q = gsl_matrix_alloc(A->size1, A->size1);
  r = gsl_matrix_alloc(A->size1, A->size2);
  tau = gsl_vector_alloc(size0);
  p = gsl_permutation_alloc(size0);
  norm = gsl_vector_alloc(size0);
  gsl_linalg_QRPT_decomp2(A, q, r, tau, p, &signum, norm);

  vq = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, q);
  vr = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, r);
  vtau = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau);
  vnorm = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, norm);
  vp = Data_Wrap_Struct(rbgsl_cPermutation, 0, gsl_permutation_free, p);

  return rb_ary_new3(6, vq, vr, vtau, vp, INT2FIX(signum), vnorm);
}

static VALUE Linalg_QRPT_solve(VALUE obj, VALUE tt, VALUE pp, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *tau, *b, *x;
  gsl_permutation *p;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Data_Get_Struct(pp, gsl_permutation, p);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_QRPT_solve(QR, tau, p, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_QRPT_svx(VALUE obj, VALUE qr, VALUE tt, VALUE pp)
{
  gsl_matrix *QR;
  gsl_vector *tau, *x;
  gsl_permutation *p;

  Data_Get_Struct(qr, gsl_matrix, QR);
  Data_Get_Struct(tt, gsl_vector, tau);
  Data_Get_Struct(pp, gsl_permutation, p);
  Data_Get_Struct(obj, gsl_vector, x);

  gsl_linalg_QRPT_svx(QR, tau, p, x);

  return obj;
}

static VALUE Linalg_QRPT_QRsolve(VALUE obj, VALUE qq, VALUE rr, VALUE pp, VALUE bb)
{
  gsl_matrix *Q, *R;
  gsl_vector *b, *x;
  gsl_permutation *p;

  Data_Get_Struct(qq, gsl_matrix, Q);
  Data_Get_Struct(rr, gsl_matrix, R);
  Data_Get_Struct(bb, gsl_vector, b);
  Data_Get_Struct(pp, gsl_permutation, p);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_QRPT_QRsolve(Q, R, p, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_QRPT_update(VALUE obj, VALUE qq, VALUE rr, VALUE pp)
{
  gsl_matrix *Q, *R;
  gsl_vector *w, *v;
  gsl_permutation *p;
  VALUE vw, vv;

  Data_Get_Struct(qq, gsl_matrix, Q);
  Data_Get_Struct(rr, gsl_matrix, R);
  Data_Get_Struct(pp, gsl_permutation, p);
  w = gsl_vector_alloc(Q->size1);
  v = gsl_vector_alloc(Q->size1);
  gsl_linalg_QRPT_update(Q, R, p, w, v);
  vw = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, w);
  vv = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);

  return rb_ary_new3(2, vw, vv);
}

static VALUE Linalg_QRPT_Rsolve(VALUE obj, VALUE pp, VALUE bb)
{
  gsl_matrix *QR;
  gsl_vector *b, *x;
  gsl_permutation *p;

  Data_Get_Struct(obj, gsl_matrix, QR);
  Data_Get_Struct(pp, gsl_permutation, p);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_QRPT_Rsolve(QR, p, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_QRPT_Rsvx(VALUE obj, VALUE vr, VALUE pp)
{
  gsl_matrix * R;
  gsl_vector * x;
  gsl_permutation * p;

  Data_Get_Struct(vr, gsl_matrix, R);
  Data_Get_Struct(pp, gsl_permutation, p);
  Data_Get_Struct(obj, gsl_vector, x);

  gsl_linalg_QRPT_Rsvx(R, p, x);

  return obj;
}

/* Singular Value Decomposition */

/* Factorise a general M x N matrix A into A = U D V^T
 * where U is a column-orthogonal M x N matrix (U^T U = I), 
 * D is a diagonal N x N matrix,
 * and V is an N x N orthogonal matrix (V^T V = V V^T = I)
 * U is stored in the original matrix A.
 * The diagonal matrix D is stored in the vector S,  D_ii = S_i
 */
static VALUE Linalg_SV_decomp(VALUE obj)
{
  gsl_matrix *A, *V;
  gsl_vector *w, *S;
  VALUE vs, vv;

  Data_Get_Struct(obj, gsl_matrix, A);
  S = gsl_vector_alloc(A->size2);  
  V = gsl_matrix_alloc(A->size2, A->size2); 
  w = gsl_vector_alloc(A->size2);

  gsl_linalg_SV_decomp(A, V, S, w);
  gsl_vector_free(w);

  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, S);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);

  return rb_ary_new3(2, vv, vs);
}


static VALUE Linalg_SV_decomp2(VALUE obj)
{
  gsl_matrix *A, *Atmp, *V;
  gsl_vector *w, *S;
  VALUE vs, vv, va;

  Data_Get_Struct(obj, gsl_matrix, A);
  Atmp = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Atmp, A);
  S = gsl_vector_alloc(A->size2);
  V = gsl_matrix_alloc(A->size2, A->size2);
  w = gsl_vector_alloc(A->size2);

  gsl_linalg_SV_decomp(Atmp, V, S, w);
  gsl_vector_free(w);

  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, S);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);
  va = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Atmp);

  return rb_ary_new3(3, va, vv, vs);
}

static VALUE Linalg_SV_decomp_mod(VALUE obj)
{
  gsl_matrix *A, *V, *X;
  gsl_vector *w, *S;
  VALUE vs, vv;

  Data_Get_Struct(obj, gsl_matrix, A);
  S = gsl_vector_alloc(A->size2);   /* see manual p 123 */
  V = gsl_matrix_alloc(A->size2, A->size2);
  X = gsl_matrix_alloc(A->size2, A->size2);
  w = gsl_vector_alloc(A->size2);

  gsl_linalg_SV_decomp_mod(A, X, V, S, w);
  gsl_vector_free(w);
  gsl_matrix_free(X);

  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, S);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);

  return rb_ary_new3(2, vv, vs);
}

static VALUE Linalg_SV_decomp_mod2(VALUE obj)
{
  gsl_matrix *A, *Atmp, *V, *X;
  gsl_vector *w, *S;
  VALUE vs, vv, va;

  Data_Get_Struct(obj, gsl_matrix, A);
  Atmp = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Atmp, A);
  S = gsl_vector_alloc(A->size2);   /* see manual p 123 */
  V = gsl_matrix_alloc(A->size2, A->size2);
  X = gsl_matrix_alloc(A->size2, A->size2);
  w = gsl_vector_alloc(A->size2);

  gsl_linalg_SV_decomp_mod(Atmp, X, V, S, w);
  gsl_vector_free(w);
  gsl_matrix_free(X);

  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, S);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);
  va = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Atmp);

  return rb_ary_new3(3, va, vv, vs);
}

static VALUE Linalg_SV_decomp_jacobi(VALUE obj)
{
  gsl_matrix *A, *V;
  gsl_vector *S;
  VALUE vs, vv;

  Data_Get_Struct(obj, gsl_matrix, A);
  S = gsl_vector_alloc(A->size2);   /* see manual p 123 */
  V = gsl_matrix_alloc(A->size2, A->size2);

  gsl_linalg_SV_decomp_jacobi(A, V, S);

  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, S);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);

  return rb_ary_new3(2, vv, vs);
}

static VALUE Linalg_SV_decomp_jacobi2(VALUE obj)
{
  gsl_matrix *A, *Atmp, *V;
  gsl_vector *S;
  VALUE vs, vv, va;

  Data_Get_Struct(obj, gsl_matrix, A);
  Atmp = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Atmp, A);
  S = gsl_vector_alloc(A->size2);   /* see manual p 123 */
  V = gsl_matrix_alloc(A->size2, A->size2);

  gsl_linalg_SV_decomp_jacobi(Atmp, V, S);

  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, S);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);
  va = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Atmp);

  return rb_ary_new3(3, va, vv, vs);
}

static VALUE Linalg_SV_solve(VALUE obj, VALUE uu, VALUE vv, VALUE ss, VALUE bb)
{
  gsl_matrix *U, *V;
  gsl_vector *S, *b, *x;

  Data_Get_Struct(uu, gsl_matrix, U);
  Data_Get_Struct(vv, gsl_matrix, V);
  Data_Get_Struct(ss, gsl_vector, S);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(V->size2);

  gsl_linalg_SV_solve(U, V, S, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

/* original function: SVD + solve */
static VALUE Linalg_SVD_solve(VALUE obj, VALUE bb)
{
  gsl_matrix *A, *U, *V;
  gsl_vector *S, *x, *b;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(bb, gsl_vector, b);
  U = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(U, A);
  S = gsl_vector_alloc(A->size2);  
  V = gsl_matrix_alloc(A->size1, A->size1);
  x = gsl_vector_alloc(V->size2);

  gsl_linalg_SV_decomp_jacobi(U, V, S);
  gsl_linalg_SV_solve(U, V, S, b, x);

  gsl_matrix_free(U);
  gsl_matrix_free(V);
  gsl_vector_free(S);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

/* Cholesky Decomposition */

static VALUE Linalg_cholesky_decomp(VALUE obj)
{
  gsl_matrix *A;

  Data_Get_Struct(obj, gsl_matrix, A);
  gsl_linalg_cholesky_decomp(A);

  return obj;
}

static VALUE Linalg_cholesky_decomp2(VALUE obj)
{
  gsl_matrix *A, *Atmp;

  Data_Get_Struct(obj, gsl_matrix, A);
  Atmp = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Atmp, A);
  gsl_linalg_cholesky_decomp(Atmp);

  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Atmp);
}

static VALUE Linalg_cholesky_solve(VALUE obj, VALUE bb)
{
  gsl_matrix *c;
  gsl_vector *b, *x;

  Data_Get_Struct(obj, gsl_matrix, c);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_cholesky_solve(c, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_cholesky_svx(VALUE obj, VALUE vLLT)
{
  gsl_matrix * LLT;
  gsl_vector *x;

  Data_Get_Struct(vLLT, gsl_matrix, LLT);
  Data_Get_Struct(obj, gsl_vector, x);

  gsl_linalg_cholesky_svx(LLT, x);

  return obj;
}

/* Tridiagonal Decomposition of Real Symmetric Matrices */

static VALUE Linalg_symmtd_decomp(VALUE obj)
{
  gsl_matrix *A;
  gsl_vector *tau;

  Data_Get_Struct(obj, gsl_matrix, A);
  tau = gsl_vector_alloc(A->size1 - 1);
  gsl_linalg_symmtd_decomp(A, tau);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau);
}

static VALUE Linalg_symmtd_decomp2(VALUE obj)
{
  gsl_matrix *A, *Anew;
  gsl_vector *tau;
  VALUE va, vtau;

  Data_Get_Struct(obj, gsl_matrix, A);
  Anew = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Anew, A);
  tau = gsl_vector_alloc(A->size1 - 1);
  gsl_linalg_symmtd_decomp(Anew, tau);
  vtau = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau);
  va = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Anew);
    
  return rb_ary_new3(2, va, vtau);
}

static VALUE Linalg_symmtd_unpack(VALUE obj, VALUE tt)
{
  gsl_matrix *A, *Q;
  gsl_vector *tau, *d, *sd;
  VALUE vq, vd, vsd;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(tt, gsl_vector, tau);
  Q = gsl_matrix_alloc(A->size1, A->size2);
  d = gsl_vector_alloc(A->size1);
  sd = gsl_vector_alloc(tau->size);
  gsl_linalg_symmtd_unpack(A, tau, Q, d, sd);

  vq = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Q);
  vd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, d);
  vsd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, sd);

  return rb_ary_new3(3, vq, vd, vsd);
}

static VALUE Linalg_symmtd_unpack_T(VALUE obj)
{
  gsl_matrix *A;
  gsl_vector *d, *sd;
  VALUE vd, vsd;

  Data_Get_Struct(obj, gsl_matrix, A);
  d = gsl_vector_alloc(A->size1);
  sd = gsl_vector_alloc(A->size1-1);
  gsl_linalg_symmtd_unpack_T(A, d, sd);

  vd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, d);
  vsd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, sd);

  return rb_ary_new3(2, vd, vsd);
}

/* TODO: Tridiagonal Decomposition of Hermitian Matrices */

/* Bidiagonalization */

static VALUE Linalg_bidiag_decomp(VALUE obj)
{
  gsl_matrix *A;
  gsl_vector *tau_U, *tau_V;
  size_t size0;
  VALUE vu, vv;

  Data_Get_Struct(obj, gsl_matrix, A);
  size0 = GSL_MIN(A->size1, A->size2);
  tau_U = gsl_vector_alloc(size0);
  tau_V = gsl_vector_alloc(size0-1);
  gsl_linalg_bidiag_decomp(A, tau_U, tau_V);
  vu = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau_U);
  vv = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau_V);

  return rb_ary_new3(2, vu, vv);
}

static VALUE Linalg_bidiag_decomp2(VALUE obj)
{
  gsl_matrix *A, *Anew;
  gsl_vector *tau_U, *tau_V;
  size_t size0;
  VALUE vu, vv, va;

  Data_Get_Struct(obj, gsl_matrix, A);
  Anew = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Anew, A);
  size0 = GSL_MIN(A->size1, A->size2);
  tau_U = gsl_vector_alloc(size0);
  tau_V = gsl_vector_alloc(size0-1);
  gsl_linalg_bidiag_decomp(Anew, tau_U, tau_V);
  vu = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau_U);
  vv = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tau_V);
  va = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, Anew);

  return rb_ary_new3(3, va, vu, vv);
}

static VALUE Linalg_bidiag_unpack(VALUE obj, VALUE tu, VALUE tv)
{
  gsl_matrix *A, *U, *V;
  gsl_vector *tau_U, *tau_V, *d, *s;
  size_t size0;
  VALUE vu, vv, vd, vs;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(tu, gsl_vector, tau_U);
  Data_Get_Struct(tv, gsl_vector, tau_V);
  U = gsl_matrix_alloc(A->size1, A->size2);
  V = gsl_matrix_alloc(A->size2, A->size2);
  size0 = GSL_MIN(A->size1, A->size2);
  d = gsl_vector_alloc(size0);
  s = gsl_vector_alloc(size0-1);
  gsl_linalg_bidiag_unpack(A, tau_U, U, tau_V, V, d, s);
  vu = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, U);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);
  vd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, d);
  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, s);

  return rb_ary_new3(4, vu, vv, vd, vs);
}

static VALUE Linalg_bidiag_unpack2(VALUE obj, VALUE tu, VALUE tv)
{
  gsl_matrix *A, *V;
  gsl_vector *tau_V, *tau_U;
  VALUE vv;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(tu, gsl_vector, tau_U);
  Data_Get_Struct(tv, gsl_vector, tau_V);
  V = gsl_matrix_alloc(A->size2, A->size2);
  gsl_linalg_bidiag_unpack2(A, tau_U, tau_V, V);
  vv = Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, V);

  return vv;
}

static VALUE Linalg_bidiag_unpack_B(VALUE obj)
{
  gsl_matrix *A;
  gsl_vector *d, *s;
  size_t size0;
  VALUE vd, vs;

  Data_Get_Struct(obj, gsl_matrix, A);
  size0 = GSL_MIN(A->size1, A->size2);
  d = gsl_vector_alloc(size0);
  s = gsl_vector_alloc(size0-1);
  gsl_linalg_bidiag_unpack_B(A, d, s);
  vd = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, d);
  vs = Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, s);

  return rb_ary_new3(2, vd, vs);
}

/* Householder solver for linear systems */

static VALUE Linalg_HH_solve(VALUE obj, VALUE bb)
{
  gsl_matrix *A;
  gsl_vector *b, *x;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_HH_solve(A, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_HH_solve2(VALUE obj, VALUE bb)
{
  gsl_matrix *A, *Anew;
  gsl_vector *b, *x;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(bb, gsl_vector, b);
  Anew = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Anew, A);
  x = gsl_vector_alloc(b->size);
  gsl_linalg_HH_solve(Anew, b, x);
  gsl_matrix_free(Anew);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_HH_svx(VALUE obj, VALUE xx)
{
  gsl_matrix *A;
  gsl_vector *x;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(xx, gsl_vector, x);

  gsl_linalg_HH_svx(A, x);

  return obj;
}

static VALUE Linalg_HH_svx2(VALUE obj, VALUE xx)
{
  gsl_matrix *A, *Anew;
  gsl_vector *x;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(xx, gsl_vector, x);
  Anew = gsl_matrix_alloc(A->size1, A->size2);
  gsl_matrix_memcpy(Anew, A);
  gsl_linalg_HH_svx(Anew, x);
  gsl_matrix_free(Anew);

  return obj;
}

/* Tridiagonal Systems */

static VALUE Linalg_solve_symm_tridiag(VALUE obj, VALUE dd, VALUE ee, VALUE bb)
{
  gsl_vector *b, *x, *d, *e;

  Data_Get_Struct(dd, gsl_vector, d);
  Data_Get_Struct(ee, gsl_vector, e);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_solve_symm_tridiag(d, e, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_solve_tridiag(VALUE obj, VALUE dd, VALUE above, VALUE below, VALUE bb)
{
  gsl_vector *a, *b, *d, *x, *z;

  Data_Get_Struct(above, gsl_vector, a);
  Data_Get_Struct(below, gsl_vector, z);
  Data_Get_Struct(bb, gsl_vector, b);
  Data_Get_Struct(dd, gsl_vector, d);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_solve_tridiag(d, a, z, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_solve_symm_cyc_tridiag(VALUE obj, VALUE dd, VALUE ee, VALUE bb)
{
  gsl_vector *b, *x, *d, *e;

  Data_Get_Struct(dd, gsl_vector, d);
  Data_Get_Struct(ee, gsl_vector, e);
  Data_Get_Struct(bb, gsl_vector, b);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_solve_symm_cyc_tridiag(d, e, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

static VALUE Linalg_solve_cyc_tridiag(VALUE obj, VALUE dd, VALUE above, VALUE below, VALUE bb)
{
  gsl_vector *a, *b, *d, *x, *z;

  Data_Get_Struct(above, gsl_vector, a);
  Data_Get_Struct(below, gsl_vector, z);
  Data_Get_Struct(bb, gsl_vector, b);
  Data_Get_Struct(dd, gsl_vector, d);
  x = gsl_vector_alloc(b->size);

  gsl_linalg_solve_cyc_tridiag(d, a, z, b, x);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, x);
}

/* Balancing */

static VALUE Linalg_balance_columns(VALUE obj, VALUE aa, VALUE dd) {
  gsl_matrix * a;
  gsl_vector * d;

  Data_Get_Struct(aa, gsl_matrix, a);
  Data_Get_Struct(dd, gsl_vector, d);

  return INT2NUM(gsl_linalg_balance_columns(a, d));
}

/* Householder Transformations */

static VALUE Linalg_householder_transform(VALUE obj) {
  gsl_vector * v, * new;
  VALUE vf, vnew;
  Data_Get_Struct(obj, gsl_vector, v);
  new = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(new, v);
  vf = rb_float_new(gsl_linalg_householder_transform(new));
  vnew =  Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
  return rb_ary_new3(2, vf, vnew); 
}

static VALUE Linalg_householder_transform_bang(VALUE obj) {
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  return rb_float_new(gsl_linalg_householder_transform(v));
}

/* P A = (1 - tau v v^T) A = A - v w^T,
 * where w = tau A^T v
 * A.householder_hm(tau, v) -> v
 */
static VALUE Linalg_householder_hm_bang(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector * v;
  gsl_matrix * A;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(vv, gsl_vector, v);

  gsl_linalg_householder_hm(NUM2DBL(tau), v, A);
  return obj;
}

static VALUE Linalg_householder_mh_bang(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector * v;
  gsl_matrix * A;

  Data_Get_Struct(obj, gsl_matrix, A);
  Data_Get_Struct(vv, gsl_vector, v);

  gsl_linalg_householder_mh(NUM2DBL(tau), v, A);
  return obj;
}

static VALUE Linalg_householder_hv_bang(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector * v, * w;

  Data_Get_Struct(obj, gsl_vector, w);
  Data_Get_Struct(vv, gsl_vector, v);

  gsl_linalg_householder_hv(NUM2DBL(tau), v, w);
  return obj;
}

static VALUE Linalg_householder_hv(VALUE obj, VALUE tau, VALUE vv) {
  gsl_vector * v, * w, * new;

  Data_Get_Struct(obj, gsl_vector, w);
  Data_Get_Struct(vv, gsl_vector, v);
  new = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(new, w);
  gsl_linalg_householder_hv(NUM2DBL(tau), v, new);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
}

static VALUE Linalg_householder_hm1(VALUE obj, VALUE tau) {
  gsl_matrix * A;

  Data_Get_Struct(obj, gsl_matrix, A);

  gsl_linalg_householder_hm1(NUM2DBL(tau), A);
  return obj;
}

/* Upper part of a matrix, including the diagonal */
  
static VALUE Matrix_U(VALUE obj)
{
  int i,j,n,n2;
  gsl_matrix * a, * b;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
    b = gsl_matrix_calloc(a->size1, b->size2);
    n = (a->size1 <= a->size2)? a->size1: a->size2;
	n2 = b->size2;
  for (i = 0; i < n; ++i) {
	for(j = i; j < n2; j++) {
	  gsl_matrix_set(b,i,j, gsl_matrix_get(a,i,j));
	}
  }
  
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, b);
}

/* m' = m.upper(true); m' is the upper part of m, including the diagonal */
static VALUE Matrix_upper(VALUE obj, VALUE vdiag)
{
  int i,j,n,n2;
  gsl_matrix * a, * b;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
    b = gsl_matrix_calloc(a->size1, b->size2);
    n = (a->size1 <= a->size2)? a->size1: a->size2;
	n2 = b->size2;
  for (i = 0; i < n; ++i) {
	for(j = i+1; j < n2; j++) {
	  gsl_matrix_set(b,i,j, gsl_matrix_get(a,i,j));
	}
  }
  if (vdiag == Qtrue)
	for (i = 0; i < n; ++i) 
	  gsl_matrix_set(b,i,i, gsl_matrix_get(a,i,i));
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, b);
}


/* Lower part of a matrix, with unit diagonal */

static VALUE Matrix_L(VALUE obj)
{
  size_t i,j;
  gsl_matrix * a, * b;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
  if (a->size2 != a->size1)
	 rb_raise(rb_eIOError, "Only square matrix allowed for the moment");
  b = gsl_matrix_calloc(a->size1, b->size2);
  for (i = 0; i < a->size1; ++i) {
	for(j = 0; j < i; j++) {
	  gsl_matrix_set(b,i,j, gsl_matrix_get(a,i,j));
	}
	if (j == i)
	  gsl_matrix_set(b,i,j,1);
  }
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, b);
}
	

/* m' = m.lower(true); m' is the lower part of m, including the diagonal */
static VALUE Matrix_lower(VALUE obj, VALUE vdiag)
{
  size_t i,j,n;
  gsl_matrix * a, * b;
  Data_Get_Struct(obj, gsl_matrix, a);
  Data_Get_Struct(obj, gsl_matrix, b);
    b = gsl_matrix_calloc(a->size1, b->size2);
    n = (a->size1 <= a->size2)? a->size1: a->size2;
  for (i = 0; i < a->size1; ++i) {
	for(j = 0; j < i; j++) {
	  gsl_matrix_set(b,i,j, gsl_matrix_get(a,i,j));
	}
  }
  if (vdiag == Qtrue)
	for (i = 0; i < n; ++i) 
	  gsl_matrix_set(b,i,i, gsl_matrix_get(a,i,i));
  return Data_Wrap_Struct(rbgsl_cMatrix, 0, gsl_matrix_free, b);
}

static VALUE Linalg_trace(VALUE obj)
{
  gsl_matrix * a;
  size_t i, n;
  double sum = 0.0;
  Data_Get_Struct(obj, gsl_matrix, a);
  n = a->size1;
  for (i=0; i < n; i++)
    sum +=  gsl_matrix_get(a,i,i);
  return rb_float_new(sum);
}

/* Module definition */

VALUE rbgsl_mLinalg;

void Init_Linalg() {

  rb_define_method(rbgsl_cMatrix, "LU_decomp!", Linalg_LU_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "LU_decomp", Linalg_LU_decomp2, 0);
  rb_define_method(rbgsl_cMatrix, "LU_solve", Linalg_LU_solve, 2);
  rb_define_method(rbgsl_cMatrix, "LU_decomp_solve!", Linalg_LU_decomp_solve, 1);
  rb_define_method(rbgsl_cMatrix, "LUD_solve", Linalg_LUD_decomp_solve, 1);
  rb_define_method(rbgsl_cVector, "LU_svx", Linalg_LU_svx, 2);
  rb_define_method(rbgsl_cVector, "LU_svx!", Linalg_LU_svx_bang, 2);
  rb_define_method(rbgsl_cMatrix, "LU_refine", Linalg_LU_refine, 3);
  rb_define_method(rbgsl_cMatrix, "LU_invert", Linalg_LU_invert, 1);
  rb_define_method(rbgsl_cMatrix, "invert", Linalg_invert, 0);
  rb_define_alias(rbgsl_cMatrix,  "inv", "invert");
  rb_define_method(rbgsl_cMatrix, "LU_det", Linalg_LU_det, 1);
  rb_define_method(rbgsl_cMatrix, "det", Linalg_det, 0);
  rb_define_method(rbgsl_cMatrix, "LU_lndet", Linalg_LU_lndet, 0);
  rb_define_method(rbgsl_cMatrix, "lndet", Linalg_lndet, 0);
  rb_define_method(rbgsl_cMatrix, "LU_sgndet", Linalg_LU_sgndet, 1);
  rb_define_method(rbgsl_cMatrix, "sgndet", Linalg_sgndet, 0);

  rb_define_method(rbgsl_cMatrix, "QR_decomp!", Linalg_QR_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "QR_decomp", Linalg_QR_decomp2, 0);
  rb_define_method(rbgsl_cMatrix, "QR_solve", Linalg_QR_solve, 2);
  rb_define_method(rbgsl_cVector, "QR_svx", Linalg_QR_svx, 2);
  rb_define_method(rbgsl_cVector, "QR_svx!", Linalg_QR_svx_bang, 2);
  rb_define_method(rbgsl_cMatrix, "QR_lssolve", Linalg_QR_lssolve, 2);
  rb_define_method(rbgsl_cMatrix, "QR_QTvec", Linalg_QR_QTvec, 2);
  rb_define_method(rbgsl_cMatrix, "QR_Qvec", Linalg_QR_Qvec, 2);
  rb_define_method(rbgsl_cMatrix, "QR_Rsolve", Linalg_QR_Rsolve, 1);
  rb_define_method(rbgsl_cMatrix, "QR_Rsvx", Linalg_QR_Rsvx, 1);
  rb_define_method(rbgsl_cMatrix, "QR_unpack", Linalg_QR_unpack, 1);

  rb_define_method(rbgsl_cMatrix, "R_solve", Linalg_R_solve, 1);
  rb_define_method(rbgsl_cMatrix, "R_svx", Linalg_R_svx, 1);

  rb_define_method(rbgsl_cMatrix, "QRPT_decomp!", Linalg_QRPT_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "QRPT_decomp", Linalg_QRPT_decomp2, 0);
  rb_define_method(rbgsl_cMatrix, "QRPT_solve", Linalg_QRPT_solve, 3);
  rb_define_method(rbgsl_cVector, "QRPT_svx!", Linalg_QRPT_svx, 3);
  rb_define_method(rbgsl_cMatrix, "QRPT_Rsolve", Linalg_QRPT_Rsolve, 2);
  rb_define_method(rbgsl_cVector, "QRPT_Rsvx!", Linalg_QRPT_Rsvx, 2);

  rb_define_method(rbgsl_cMatrix, "SV_decomp!", Linalg_SV_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "SV_decomp", Linalg_SV_decomp2, 0);

  rb_define_method(rbgsl_cMatrix, "SV_decomp_mod!", Linalg_SV_decomp_mod, 0);
  rb_define_method(rbgsl_cMatrix, "SV_decomp_mod", Linalg_SV_decomp_mod2, 0);

  rb_define_method(rbgsl_cMatrix, "SV_decomp_jacobi!", Linalg_SV_decomp_jacobi, 0);
  rb_define_method(rbgsl_cMatrix, "SV_decomp_jacobi", Linalg_SV_decomp_jacobi2, 0);

  rb_define_method(rbgsl_cMatrix, "SVD_solve", Linalg_SVD_solve, 1);

  rb_define_method(rbgsl_cMatrix, "cholesky_decomp!", Linalg_cholesky_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "cholesky_decomp", Linalg_cholesky_decomp2, 0);
  rb_define_method(rbgsl_cMatrix, "cholesky_solve", Linalg_cholesky_solve, 1);
  rb_define_method(rbgsl_cVector, "cholesky_svx!", Linalg_cholesky_svx, 1);

  rb_define_method(rbgsl_cMatrix, "symmtd_decomp!", Linalg_symmtd_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "symmtd_decomp", Linalg_symmtd_decomp2, 0);
  rb_define_method(rbgsl_cMatrix, "symmtd_unpack", Linalg_symmtd_unpack, 1);
  rb_define_method(rbgsl_cMatrix, "symmtd_unpack_T", Linalg_symmtd_unpack_T, 0);

  rb_define_method(rbgsl_cMatrix, "bidiag_decomp!", Linalg_bidiag_decomp, 0);
  rb_define_method(rbgsl_cMatrix, "bidiag_decomp", Linalg_bidiag_decomp2, 0);
  rb_define_method(rbgsl_cMatrix, "bidiag_unpack", Linalg_bidiag_unpack, 2);
  rb_define_method(rbgsl_cMatrix, "bidiag_unpack2", Linalg_bidiag_unpack2, 2);
  rb_define_method(rbgsl_cMatrix, "bidiag_unpack_B", Linalg_bidiag_unpack_B, 0);

  rb_define_method(rbgsl_cMatrix, "HH_solve!", Linalg_HH_solve, 1);
  rb_define_method(rbgsl_cMatrix, "HH_svx!", Linalg_HH_svx, 1);

  rb_define_method(rbgsl_cMatrix, "HH_solve", Linalg_HH_solve2, 1);
  rb_define_method(rbgsl_cMatrix, "HH_svx", Linalg_HH_svx2, 1);

  rbgsl_mLinalg = rb_define_module_under(rbgsl_mGSL, "Linalg");

  rb_define_module_function(rbgsl_mLinalg, "QR_QRsolve", Linalg_QR_QRsolve, 3);

  rb_define_module_function(rbgsl_mLinalg, "QR_update", Linalg_QR_update, 4);
/*
  rb_define_module_function(rbgsl_mLinalg, "QR_update", Linalg_QR_update, 4);
*/
  rb_define_module_function(rbgsl_mLinalg, "QRPT_QRsolve", Linalg_QRPT_QRsolve, 4);
  rb_define_module_function(rbgsl_mLinalg, "QRPT_update", Linalg_QRPT_update, 3);
  
  rb_define_module_function(rbgsl_mLinalg, "SV_solve", Linalg_SV_solve, 4);

  rb_define_module_function(rbgsl_mLinalg, "solve_symm_tridiag", Linalg_solve_symm_tridiag, 3);
  rb_define_module_function(rbgsl_mLinalg, "solve_tridiag", Linalg_solve_tridiag, 4);
  rb_define_module_function(rbgsl_mLinalg, "solve_symm_cyc_tridiag", Linalg_solve_symm_cyc_tridiag, 3);
  rb_define_module_function(rbgsl_mLinalg, "solve_cyc_tridiag", Linalg_solve_cyc_tridiag, 4);

  rb_define_module_function(rbgsl_mLinalg, "balance_columns", Linalg_balance_columns, 2);

  rb_define_method(rbgsl_cVector, "householder_transform", Linalg_householder_transform, 0);
  rb_define_method(rbgsl_cVector, "householder_transform!", Linalg_householder_transform_bang, 0);
  rb_define_method(rbgsl_cVector, "householder_hv!", Linalg_householder_hv_bang, 2);
  rb_define_method(rbgsl_cVector, "householder_hv", Linalg_householder_hv, 2);
  rb_define_method(rbgsl_cMatrix, "householder_hm!", Linalg_householder_hm_bang, 2);
  rb_define_method(rbgsl_cMatrix, "householder_mh!", Linalg_householder_mh_bang, 2);
  rb_define_method(rbgsl_cMatrix, "householder_hm1", Linalg_householder_hm1, 1);

  rb_define_method(rbgsl_cMatrix, "U", Matrix_U, 0);
  rb_define_method(rbgsl_cMatrix, "L", Matrix_L, 0);
  rb_define_method(rbgsl_cMatrix, "upper", Matrix_upper, 1);
  rb_define_method(rbgsl_cMatrix, "lower", Matrix_lower, 1);
  rb_define_method(rbgsl_cMatrix, "trace", Linalg_trace, 0);
}

/* vim: set ts=4: */
