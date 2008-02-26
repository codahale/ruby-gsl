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

/* $Id: GSL.c,v 1.8 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <stdio.h>
#include <string.h>

#include "ruby.h"
#include "gsl/gsl_errno.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* constants for error handling */
#define ERR_INTERN 0
#define ERR_RUBY 1
#define ERR_OFF 2

gsl_error_handler_t * handler = NULL;

void my_handler(const char *, const char *, int , int);

/* Error Handling */

static VALUE GSL_strerror(VALUE self, VALUE gsl_errno) {
  return rb_str_new2(gsl_strerror(NUM2INT(gsl_errno)));
}

void my_handler(const char * reason, const char * file, int line, int gsl_errno) {
  const char * msg = gsl_strerror(gsl_errno);
  int l = strlen(reason) + strlen(file) + strlen(msg) + 29;
  char * buf = ALLOCA_N(char, l);
  snprintf(buf, l, "GSL: %s (file: %s, line: %d): %s", reason, file, line, msg);
  switch(gsl_errno)
    {
    case GSL_EDOM:
      rb_raise(rb_eFloatDomainError, buf);
      break;
    case GSL_ERANGE:
      rb_raise(rb_eFloatDomainError, buf);
      break;
    case GSL_EFAULT:
      rb_raise(rb_eArgError, buf);
      break;
    case GSL_EINVAL:
      rb_raise(rb_eArgError, buf);
      break;
    case GSL_EFAILED:
      rb_raise(rbgsl_eException, buf);
      break;
    case GSL_EFACTOR:
      rb_raise(rbgsl_eFactorization, buf);
      break;
    case GSL_ESANITY:
      rb_raise(rbgsl_eException, buf);
      break;
    case GSL_ENOMEM:
      rb_raise(rbgsl_eException, buf);
      break;
    case GSL_EBADFUNC:
      rb_raise(rb_eArgError, buf);
      break;
    case GSL_ERUNAWAY:
      rb_raise(rbgsl_eRunaway, buf);
      break;
    case GSL_EMAXITER:
      rb_raise(rbgsl_eMaxIter, buf);
      break;
    case GSL_EZERODIV:
      rb_raise(rb_eZeroDivError, buf);
      break;
    case GSL_EBADTOL:
      rb_raise(rb_eArgError, buf);
      break;
    case GSL_ETOL:
      rb_raise(rbgsl_eTol, buf);
      break;
    case GSL_EUNDRFLW:
      rb_raise(rb_eFloatDomainError, buf);
      break;
    case GSL_EOVRFLW:
      rb_raise(rb_eFloatDomainError, buf);
      break;
    case GSL_ELOSS:
      rb_raise(rbgsl_ePrecisionLoss, buf);
      break;
    case GSL_EROUND:
      rb_raise(rbgsl_eRoundoff, buf);
      break;
    case GSL_EBADLEN:
      rb_raise(rb_eArgError, buf);
      break;
    case GSL_ENOTSQR:
      rb_raise(rb_eArgError, buf);
      break;
    case GSL_ESING:
      rb_raise(rbgsl_eSingularity, buf);
      break;
    case GSL_EDIVERGE:
      rb_raise(rbgsl_eDivergent, buf);
      break;
    case GSL_EUNSUP:
      rb_notimplement();
      break;
    case GSL_EUNIMPL:
      rb_notimplement();
      break;
    case GSL_ECACHE:
      rb_raise(rbgsl_eCache, buf);
      break;
    case GSL_ETABLE:
      rb_raise(rbgsl_eTable, buf);
      break;
    case GSL_ENOPROG:
      rb_raise(rbgsl_eNoProgress, buf);
      break;
    case GSL_ENOPROGJ:
      rb_raise(rbgsl_eNoProgressJac, buf);
      break;
    case GSL_ETOLF:
      rb_raise(rbgsl_eToleranceF, buf);
      break;
    case GSL_ETOLX:
      rb_raise(rbgsl_eToleranceX, buf);
      break;
    case GSL_ETOLG:
      rb_raise(rbgsl_eToleranceG, buf);
      break;
    case GSL_EOF:
      rb_raise(rb_eEOFError, buf);
      break;
    default:
      rb_raise(rbgsl_eException, buf);
      break;
    }
}

static VALUE GSL_set_handler(VALUE self, VALUE type) {
  switch (NUM2INT(type)) {
    case ERR_INTERN:
      handler = gsl_set_error_handler(NULL);
      break;
    case ERR_OFF:
      handler = gsl_set_error_handler_off();
      break;
    case ERR_RUBY:
      handler = gsl_set_error_handler(&my_handler);
      break;
    default:
      rb_raise(rb_eArgError, "Illegal error handler type!");
  }
  return self;
}

/* Module definition */

VALUE rbgsl_mGSL;
VALUE rbgsl_eException;
VALUE rbgsl_eFactorization;
VALUE rbgsl_eRunaway;
VALUE rbgsl_eMaxIter;
VALUE rbgsl_eTol;
VALUE rbgsl_ePrecisionLoss;
VALUE rbgsl_eRoundoff;
VALUE rbgsl_eSingularity;
VALUE rbgsl_eDivergent;
VALUE rbgsl_eCache;
VALUE rbgsl_eTable;
VALUE rbgsl_eNoProgress;
VALUE rbgsl_eNoProgressJac;
VALUE rbgsl_eToleranceF;
VALUE rbgsl_eToleranceX;
VALUE rbgsl_eToleranceG;

void Init_gsl_functions() {
  rbgsl_mGSL = rb_define_module("GSL");

  rbgsl_eException = rb_define_class_under(rbgsl_mGSL, "GSLException", rb_eStandardError);
  rbgsl_eFactorization = rb_define_class_under(rbgsl_mGSL, "FactorizationError", rbgsl_eException);
  rbgsl_eRunaway = rb_define_class_under(rbgsl_mGSL, "RunawayError", rbgsl_eException);
  rbgsl_eMaxIter = rb_define_class_under(rbgsl_mGSL, "MaxIterError", rbgsl_eException);
  rbgsl_eTol = rb_define_class_under(rbgsl_mGSL, "ToleranceError", rbgsl_eException);
  rbgsl_ePrecisionLoss = rb_define_class_under(rbgsl_mGSL, "PrecisionError", rbgsl_eException);
  rbgsl_eRoundoff = rb_define_class_under(rbgsl_mGSL, "RoundoffError", rbgsl_eException);
  rbgsl_eSingularity = rb_define_class_under(rbgsl_mGSL, "SingularityError", rbgsl_eException);
  rbgsl_eDivergent = rb_define_class_under(rbgsl_mGSL, "DivergentError", rbgsl_eException);
  rbgsl_eCache = rb_define_class_under(rbgsl_mGSL, "CacheError", rbgsl_eException);
  rbgsl_eTable = rb_define_class_under(rbgsl_mGSL, "TableError", rbgsl_eException);
  rbgsl_eNoProgress = rb_define_class_under(rbgsl_mGSL, "NoProgressError", rbgsl_eException);
  rbgsl_eNoProgressJac = rb_define_class_under(rbgsl_mGSL, "NoProgressJacError", rbgsl_eException);
  rbgsl_eToleranceF = rb_define_class_under(rbgsl_mGSL, "ToleranceFError", rbgsl_eException);
  rbgsl_eToleranceX = rb_define_class_under(rbgsl_mGSL, "ToleranceXError", rbgsl_eException);
  rbgsl_eToleranceG = rb_define_class_under(rbgsl_mGSL, "ToleranceGError", rbgsl_eException);

  rb_define_module_function(rbgsl_mGSL, "strerror",  GSL_strerror, 1);
  rb_define_module_function(rbgsl_mGSL, "set_handler",  GSL_set_handler, 1);

  rb_define_const(rbgsl_mGSL, "GSL_SUCCESS",  INT2FIX(GSL_SUCCESS));
  rb_define_const(rbgsl_mGSL, "GSL_FAILURE",  INT2FIX(GSL_FAILURE));
  rb_define_const(rbgsl_mGSL, "GSL_CONTINUE", INT2FIX(GSL_CONTINUE));
  rb_define_const(rbgsl_mGSL, "GSL_EDOM",     INT2FIX(GSL_EDOM));
  rb_define_const(rbgsl_mGSL, "GSL_ERANGE",   INT2FIX(GSL_ERANGE));
  rb_define_const(rbgsl_mGSL, "GSL_EFAULT",   INT2FIX(GSL_EFAULT));
  rb_define_const(rbgsl_mGSL, "GSL_EINVAL",   INT2FIX(GSL_EINVAL));
  rb_define_const(rbgsl_mGSL, "GSL_EFAILED",  INT2FIX(GSL_EFAILED));
  rb_define_const(rbgsl_mGSL, "GSL_EFACTOR",  INT2FIX(GSL_EFACTOR));
  rb_define_const(rbgsl_mGSL, "GSL_ESANITY",  INT2FIX(GSL_ESANITY));
  rb_define_const(rbgsl_mGSL, "GSL_ENOMEM",   INT2FIX(GSL_ENOMEM));
  rb_define_const(rbgsl_mGSL, "GSL_EBADFUNC", INT2FIX(GSL_EBADFUNC));
  rb_define_const(rbgsl_mGSL, "GSL_ERUNAWAY", INT2FIX(GSL_ERUNAWAY));
  rb_define_const(rbgsl_mGSL, "GSL_EMAXITER", INT2FIX(GSL_EMAXITER));
  rb_define_const(rbgsl_mGSL, "GSL_EZERODIV", INT2FIX(GSL_EZERODIV));
  rb_define_const(rbgsl_mGSL, "GSL_EBADTOL",  INT2FIX(GSL_EBADTOL));
  rb_define_const(rbgsl_mGSL, "GSL_ETOL",     INT2FIX(GSL_ETOL));
  rb_define_const(rbgsl_mGSL, "GSL_EUNDRFLW", INT2FIX(GSL_EUNDRFLW));
  rb_define_const(rbgsl_mGSL, "GSL_EOVRFLW",  INT2FIX(GSL_EOVRFLW));
  rb_define_const(rbgsl_mGSL, "GSL_ELOSS",    INT2FIX(GSL_ELOSS));
  rb_define_const(rbgsl_mGSL, "GSL_EROUND",   INT2FIX(GSL_EROUND));
  rb_define_const(rbgsl_mGSL, "GSL_EBADLEN",  INT2FIX(GSL_EBADLEN));
  rb_define_const(rbgsl_mGSL, "GSL_ENOTSQR",  INT2FIX(GSL_ENOTSQR));
  rb_define_const(rbgsl_mGSL, "GSL_ESING",    INT2FIX(GSL_ESING));
  rb_define_const(rbgsl_mGSL, "GSL_EDIVERGE", INT2FIX(GSL_EDIVERGE));
  rb_define_const(rbgsl_mGSL, "GSL_EUNSUP",   INT2FIX(GSL_EUNSUP));
  rb_define_const(rbgsl_mGSL, "GSL_EUNIMPL",  INT2FIX(GSL_EUNIMPL));
  rb_define_const(rbgsl_mGSL, "GSL_ECACHE",   INT2FIX(GSL_ECACHE));
  rb_define_const(rbgsl_mGSL, "GSL_ETABLE",   INT2FIX(GSL_ETABLE));
  rb_define_const(rbgsl_mGSL, "GSL_ENOPROG",  INT2FIX(GSL_ENOPROG));
  rb_define_const(rbgsl_mGSL, "GSL_ENOPROGJ", INT2FIX(GSL_ENOPROGJ));
  rb_define_const(rbgsl_mGSL, "GSL_ETOLF",    INT2FIX(GSL_ETOLF));
  rb_define_const(rbgsl_mGSL, "GSL_ETOLX",    INT2FIX(GSL_ETOLX));
  rb_define_const(rbgsl_mGSL, "GSL_ETOLG",    INT2FIX(GSL_ETOLG));
  rb_define_const(rbgsl_mGSL, "GSL_EOF",      INT2FIX(GSL_EOF));

  rb_define_const(rbgsl_mGSL, "ERR_INTERN",   INT2FIX(ERR_INTERN));
  rb_define_const(rbgsl_mGSL, "ERR_OFF",      INT2FIX(ERR_OFF));
  rb_define_const(rbgsl_mGSL, "ERR_RUBY",     INT2FIX(ERR_RUBY));

  handler = gsl_set_error_handler(&my_handler);

  Init_Result();
  Init_GSLMath();      /* don't confuse with Ruby's Math */
  Init_Block();        /* Blocks */
  Init_Cheb();         /* Chebyshev Approximations */
  Init_Combination();  /* Combinations */
  Init_Complex();      /* Complex Numbers */
  Init_Const();        /* Physical Constants */
  Init_DHT();          /* Discrete Henkel Transforms */
  Init_Diff();         /* Numerical Differentiation */
  Init_Fit();          /* Least-Squares Fitting */
  Init_Function();     /* Functions */
  Init_Histogram();    /* Histograms */
  Init_Interp();       /* Interpolation */
  Init_Integration();  /* Numerical Interpolation */
  Init_Vector();       /* Vectors (has to be before Matrix) */
  Init_Vector_complex();
  Init_Matrix();       /* Matrices */
  Init_Matrix_complex();
  Init_Minimizer();    /* Minimization */
  Init_MonteCarlo();   /* Monte Carlo Integration */
  Init_MFunction();    /* Multidimensional Functions */
  Init_MSolver();      /* Multidimensional Root-Finding */
  Init_Permutation();  /* Permutations */
  Init_Poly();         /* Polynomials */
  Init_GSLRandom();    /* everything related to random numbers */
  Init_Siman();        /* Simulated Annealing */
  Init_Solver();       /* Root-Finding */
  Init_Sort();         /* Sorting */
  Init_Special();      /* Special Functions */
  Init_Stats();        /* Statistical Functions */
  Init_Sum();          /* Series Acceleration */
}

/* vim: set ts=4: */
