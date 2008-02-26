/*
* ruby-gsl - using GSL from Ruby
* Copyright (C) 2001-2003 Arno Erpenbeck
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

/* $Id: ruby-gsl-intern.h,v 1.13 2004/11/13 22:50:12 aerpenbeck Exp $ */

#ifndef _RUBY_GSL_INTERN_
#define _RUBY_GSL_INTERN_

#include "ruby.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_sf_result.h"

/* macros for copying data */

#define COPYRESULT(x,y) { \
  gsl_sf_result *p; \
  Data_Get_Struct(x, gsl_sf_result, p); \
  p->val = (y).val; \
  p->err = (y).err; }

#define COPYRUBYHASHARRAY(from,to) { \
  long _l; \
  to = (long *) ALLOCA_N(long, RARRAY(from)->len); \
  for (_l = 0; _l < RARRAY(from)->len; _l++) \
    (to)[_l] = NUM2LONG(rb_funcall(RARRAY(from)->ptr[_l], rb_intern("hash"), 0)); }

#define COPYRUBYARRAY(from,to) { \
  long _l; \
  to = (double *) ALLOCA_N(double, RARRAY(from)->len); \
  for (_l = 0; _l < RARRAY(from)->len; _l++) \
    (to)[_l] = NUM2DBL(RARRAY(from)->ptr[_l]); }

#define COPYCARRAY(from,to,size) { \
  long _l;\
  to = rb_ary_new2(size); \
  for (_l = 0; _l < (size); _l++) \
    rb_ary_store(to, _l, rb_float_new((from)[_l])); \
  RARRAY(to)->len = (size); }
  
/* RARRAY(to)->ptr[_l] = rb_float_new((from)[_l]); */

/* constants for interpolation types */
#define INTERP_LINEAR 0
#define INTERP_POLYNOMIAL 1
#define INTERP_CSPLINE 2
#define INTERP_CSPLINE_PERIODIC 3
#define INTERP_AKIMA 4
#define INTERP_AKIMA_PERIODIC 5

/* functions */
VALUE Complex_new_intern(VALUE, gsl_complex *);
VALUE Result_new_intern(VALUE, gsl_sf_result *);
VALUE Result10_new_intern(VALUE, gsl_sf_result_e10 *);

#define RESULT(r) Result_new_intern(rbgsl_cResult,r)
#define RESULT10(r) Result10_new_intern(rbgsl_cResult10,r)
#define COMPLEX(c) Complex_new_intern(rbgsl_cComplex,c)

/* initialization functions */
extern void Init_Accel(void);
extern void Init_Airy(void);
extern void Init_Bessel(void);
extern void Init_Blas(void);
extern void Init_Blas_complex(void);
extern void Init_Block(void);
extern void Init_Cheb(void);
extern void Init_Clausen(void);
extern void Init_Combination(void);
extern void Init_Complex(void);
extern void Init_Conical(void);
extern void Init_Const(void);
extern void Init_Coulomb(void);
extern void Init_Coupling(void);
extern void Init_DHT(void);
extern void Init_Dawson(void);
extern void Init_Debye(void);
extern void Init_Diff(void);
extern void Init_Dilog(void);
extern void Init_Discrete(void);
extern void Init_Eigen(void);
extern void Init_Eigen_complex(void);
extern void Init_Elementary(void);
extern void Init_EllInt(void);
extern void Init_EllJac(void);
extern void Init_Error(void);
extern void Init_Exp(void);
extern void Init_ExpInt(void);
extern void Init_FermiDirac(void);
extern void Init_Fit(void);
extern void Init_Function(void);
extern void Init_Gamma(void);
extern void Init_Gegenbauer(void);
extern void Init_GSLMath(void);
extern void Init_GSLRandom(void);
extern void Init_Hist(void);
extern void Init_Hist2D(void);
extern void Init_Histogram(void);
extern void Init_Hyperg(void);
extern void Init_Integration(void);
extern void Init_Interp(void);
extern void Init_Laguerre(void);
extern void Init_Lambert(void);
extern void Init_Legendre(void);
extern void Init_Linalg(void);
extern void Init_Linalg_complex(void);
extern void Init_Log(void);
extern void Init_Matrix(void);
extern void Init_Matrix_complex(void);
extern void Init_Minimizer(void);
extern void Init_MonteCarlo(void);
extern void Init_MFunction(void);
extern void Init_MSolver(void);
extern void Init_Permutation(void);
extern void Init_Poly(void);
extern void Init_Psi(void);
extern void Init_QRNG(void);
extern void Init_Result(void);
extern void Init_RND(void);
extern void Init_RNG(void);
extern void Init_Siman(void);
extern void Init_Sum(void);
extern void Init_Solver(void);
extern void Init_Sort(void);
extern void Init_Special(void);
extern void Init_Spline(void);
extern void Init_Stats(void);
extern void Init_Synchrotron(void);
extern void Init_Transport(void);
extern void Init_Trig(void);
extern void Init_Vector(void);
extern void Init_Vector_complex(void);
extern void Init_Zeta(void);

#endif
