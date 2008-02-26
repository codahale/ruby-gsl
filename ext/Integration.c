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

/* $Id: Integration.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

/*
  Ruby/GSL: Ruby extension library for GSL (GNU Scientific Library)
    (C) Copyright 2001 by Yoshiki Tsunesada

  Ruby/GSL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.
*/

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_integration.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Numerical Integration */

static VALUE rbgsl_cQAWSTable;
static VALUE rbgsl_cQAWOTable;

/* QNG non-adaptive Gauss-Kronrod integration */

static VALUE Integration_qng(VALUE obj, VALUE aa, VALUE bb, VALUE ea, VALUE er)
{
  double a, b, epsabs, epsrel;
  double result, abserr;
  size_t neval;
  gsl_function * F;
  int status;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);

  status = gsl_integration_qng(F, a, b, epsabs, epsrel, &result, &abserr,
    &neval);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2NUM(neval), INT2FIX(status));
}          

/* QAG adaptive integration */

static VALUE Integration_qag(VALUE obj, VALUE aa, VALUE bb, VALUE ea, VALUE er,
  VALUE ll, VALUE kk)
{
  double a, b, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  int key, status, intervals;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  key = NUM2INT(kk);

  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qag(F, a, b, epsabs, epsrel, limit, key, w,
    &result, &abserr);
  intervals = w->size;

  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr), 
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAGS adaptive integration with singularities */

static VALUE Integration_qags(VALUE obj, VALUE aa, VALUE bb, VALUE ea, VALUE er,
  VALUE ll)
{
  double a, b, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  int status, intervals;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qags(F, a, b, epsabs, epsrel, limit, w, &result, 
    &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr), 
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAGP adaptive integration with known singular points */

static VALUE Integration_qagp(VALUE obj, VALUE ary_ab, VALUE ea, VALUE er, 
  VALUE ll)
{
  double result, abserr;
  double * pts;
  gsl_function * F;
  gsl_integration_workspace * w;
  int status, intervals;
  
  Data_Get_Struct(obj, gsl_function, F);

  COPYRUBYARRAY(ary_ab, pts);
  w = gsl_integration_workspace_alloc(NUM2INT(ll));

  status = gsl_integration_qagp(F, pts, RARRAY(ary_ab)->len, NUM2DBL(ea),
    NUM2DBL(er), NUM2INT(ll), w, &result, &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAGI adaptive integration on infinite intervals */

/* (-infty -- infty) */
static VALUE Integration_qagi(VALUE obj, VALUE ea, VALUE er, VALUE ll)
{
  double epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  int status, intervals;
  
  Data_Get_Struct(obj, gsl_function, F);

  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qagi(F, epsabs, epsrel, limit, w, &result, &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* (a -- infty) */
static VALUE Integration_qagiu(VALUE obj, VALUE aa, VALUE ea, VALUE er, 
  VALUE ll)
{
  double a, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  int status, intervals;
  
  Data_Get_Struct(obj, gsl_function, F);
  a = NUM2DBL(aa);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qagiu(F, a, epsabs, epsrel, limit, w, 
        &result, &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* (-infty -- b) */
static VALUE Integration_qagil(VALUE obj, VALUE bb, VALUE ea, VALUE er, 
  VALUE ll)
{
  double b, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  int status, intervals;
  
  Data_Get_Struct(obj, gsl_function, F);
  b = NUM2DBL(bb);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qagil(F, b, epsabs, epsrel, limit, w, 
    &result, &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAWC adaptive integration for Cauchy principal values */

static VALUE Integration_qawc(VALUE obj, VALUE aa, VALUE bb, VALUE cc,
  VALUE ea, VALUE er, VALUE ll)
{
  double a, b, c, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  int status, intervals;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  c = NUM2DBL(cc);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qawc(F, a, b, c, epsabs, epsrel, limit, w, &result,
    &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAWS adaptive integration for singular functions */

static VALUE Integration_qaws_table_alloc(VALUE klass, VALUE alpha, VALUE beta,
  VALUE mu, VALUE nu)
{
  gsl_integration_qaws_table * t;

  t = gsl_integration_qaws_table_alloc(NUM2DBL(alpha), NUM2DBL(beta),
    NUM2INT(mu), NUM2INT(nu));
  return Data_Wrap_Struct(klass, 0, gsl_integration_qaws_table_free, t);
}

static VALUE Integration_qaws_table_set(int argc, VALUE * argv, VALUE obj)
{
  gsl_integration_qaws_table * t;
  double alpha, beta;
  int mu, nu, type;
  
  type = TYPE(argv[0]);
  Data_Get_Struct(obj, gsl_integration_qaws_table, t);

  if (type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT) {
    alpha = NUM2DBL(argv[0]);
    beta  = NUM2DBL(argv[1]);
    mu    = NUM2INT(argv[2]);
    nu    = NUM2INT(argv[3]);
  } else if (type == T_ARRAY) {
    alpha = NUM2DBL(rb_ary_entry(argv[0], 0));
    beta  = NUM2DBL(rb_ary_entry(argv[0], 1));
    mu    = NUM2INT(rb_ary_entry(argv[0], 2));
    nu    = NUM2INT(rb_ary_entry(argv[0], 3));
  } else {
    rb_raise(rb_eArgError, "Arguments of illegal type");
  }

  gsl_integration_qaws_table_set(t, alpha, beta, mu, nu);
  return obj;
}

static VALUE Integration_qaws_table_to_a(VALUE obj)
{
  gsl_integration_qaws_table * t;

  Data_Get_Struct(obj, gsl_integration_qaws_table, t);
  return rb_ary_new3(4, rb_float_new(t->alpha), rb_float_new(t->beta),
    INT2FIX(t->mu), INT2FIX(t->nu));
}

static VALUE Array_to_integration_qaws_table(VALUE ary)
{
  gsl_integration_qaws_table * t;
  double alpha, beta;
  int mu, nu;

  alpha = NUM2DBL(rb_ary_entry(ary, 0));
  beta  = NUM2DBL(rb_ary_entry(ary, 1));
  mu    = NUM2INT(rb_ary_entry(ary, 2));
  nu    = NUM2INT(rb_ary_entry(ary, 3));
  t = gsl_integration_qaws_table_alloc(alpha, beta, mu, nu);
  return Data_Wrap_Struct(rbgsl_cQAWSTable, 0, gsl_integration_qaws_table_free, t);
}

static VALUE Integration_qaws(VALUE obj, VALUE aa, VALUE bb, VALUE tt,
  VALUE ea, VALUE er, VALUE ll)
{
  double a, b, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  gsl_integration_qaws_table * t;
  int status, intervals;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  b = NUM2DBL(bb);
  Data_Get_Struct(tt, gsl_integration_qaws_table, t);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qaws(F, a, b, t, epsabs, epsrel, limit, w, &result,
    &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr), 
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAWO adaptive integration for oscillatory functions */

static VALUE Integration_qawo_table_alloc(VALUE klass, VALUE oo, VALUE LL,
  VALUE ss, VALUE nn)
{
  gsl_integration_qawo_table * t;
  double omega, L;
  enum gsl_integration_qawo_enum sine;
  size_t n;

  omega = NUM2DBL(oo);
  L = NUM2DBL(LL);
  sine = NUM2INT(ss);
  n = NUM2INT(nn);
  t = gsl_integration_qawo_table_alloc(omega, L, sine, n);
               
  return Data_Wrap_Struct(klass, 0, gsl_integration_qawo_table_free, t);
}

static VALUE Integration_qawo_table_to_a(VALUE obj)
{
  gsl_integration_qawo_table * t;

  Data_Get_Struct(obj, gsl_integration_qawo_table, t);
  return rb_ary_new3(4, rb_float_new(t->omega), rb_float_new(t->L), 
    INT2FIX(t->sine), INT2FIX(t->n));
}

static VALUE Array_to_integration_qawo_table(VALUE ary)
{
  gsl_integration_qawo_table * t;
  double omega, L;
  enum gsl_integration_qawo_enum sine;
  size_t n;

  omega = NUM2DBL(rb_ary_entry(ary, 0));
  L     = NUM2DBL(rb_ary_entry(ary, 1));
  sine  = NUM2INT(rb_ary_entry(ary, 2));
  n     = NUM2INT(rb_ary_entry(ary, 3));
  t = gsl_integration_qawo_table_alloc(omega, L, sine, n);
  return Data_Wrap_Struct(rbgsl_cQAWOTable, 0, gsl_integration_qawo_table_free, t);
}

static VALUE Integration_qawo_table_set(int argc, VALUE * argv, VALUE obj)
{
  gsl_integration_qawo_table * t;
  double omega, L;
  enum gsl_integration_qawo_enum sine;
  int type;
  
  type = TYPE(argv[0]);
  Data_Get_Struct(obj, gsl_integration_qawo_table, t);

  if (type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT) {
    omega = NUM2DBL(argv[0]);
    L     = NUM2DBL(argv[1]);
    sine  = NUM2INT(argv[2]);
  } else if (type == T_ARRAY) {
    omega = NUM2DBL(rb_ary_entry(argv[0], 0));
    L     = NUM2DBL(rb_ary_entry(argv[0], 1));
    sine  = NUM2INT(rb_ary_entry(argv[0], 2));
  } else {
    rb_raise(rb_eArgError, "Arguments of illegal type");
  }

  gsl_integration_qawo_table_set(t, omega, L, sine);
  return obj;
}

static VALUE Integration_qawo_table_set_length(VALUE obj, VALUE L)
{
  gsl_integration_qawo_table * t;
  Data_Get_Struct(obj, gsl_integration_qawo_table, t);
  gsl_integration_qawo_table_set_length(t, NUM2DBL(L));
  return obj;
}

static VALUE Integration_qawo(VALUE obj, VALUE aa, VALUE ea, VALUE er,
  VALUE ll, VALUE tt)
{
  double a, epsabs, epsrel;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w;
  gsl_integration_qawo_table * t;
  int status, intervals;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  epsabs = NUM2DBL(ea);
  epsrel = NUM2DBL(er);
  limit = NUM2INT(ll);
  Data_Get_Struct(tt, gsl_integration_qawo_table, t);
  w = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qawo(F, a, epsabs, epsrel, limit, w, t, &result,
    &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* QAWF adaptive integration for Fourier integrals */

static VALUE Integration_qawf(VALUE obj, VALUE aa, 
             VALUE ea, VALUE ll, VALUE tt)
{
  double a, epsabs;
  double result, abserr;
  size_t limit;
  gsl_function * F;
  gsl_integration_workspace * w, * cw;
  gsl_integration_qawo_table * t;
  int status, intervals;

  Data_Get_Struct(obj, gsl_function, F);

  a = NUM2DBL(aa);
  epsabs = NUM2DBL(ea);
  limit = NUM2INT(ll);
  Data_Get_Struct(tt, gsl_integration_qawo_table, t);
  w = gsl_integration_workspace_alloc(limit);
  cw = gsl_integration_workspace_alloc(limit);

  status = gsl_integration_qawf(F, a, epsabs, limit, w, cw, t, &result, &abserr);
  intervals = w->size;
  gsl_integration_workspace_free(w);
  gsl_integration_workspace_free(cw);

  return rb_ary_new3(4, rb_float_new(result), rb_float_new(abserr),
    INT2FIX(intervals), INT2FIX(status));
}          

/* Module definition */

VALUE rbgsl_mIntegration;

void Init_Integration() {
  rbgsl_mIntegration = rb_define_module_under(rbgsl_mGSL, "Integration");

  rb_define_const(rbgsl_mIntegration, "GAUSS15", INT2FIX(GSL_INTEG_GAUSS15));
  rb_define_const(rbgsl_mIntegration, "GAUSS21", INT2FIX(GSL_INTEG_GAUSS21));
  rb_define_const(rbgsl_mIntegration, "GAUSS31", INT2FIX(GSL_INTEG_GAUSS31));
  rb_define_const(rbgsl_mIntegration, "GAUSS41", INT2FIX(GSL_INTEG_GAUSS41));
  rb_define_const(rbgsl_mIntegration, "GAUSS51", INT2FIX(GSL_INTEG_GAUSS51));
  rb_define_const(rbgsl_mIntegration, "GAUSS61", INT2FIX(GSL_INTEG_GAUSS61));
  rb_define_const(rbgsl_mIntegration, "COSINE",  INT2FIX(GSL_INTEG_COSINE));
  rb_define_const(rbgsl_mIntegration, "SINE",    INT2FIX(GSL_INTEG_SINE));
       
  rb_define_method(rbgsl_cFunction, "integration_qng", Integration_qng, 4);
  rb_define_method(rbgsl_cFunction, "integration_qag", Integration_qag, 6);
  rb_define_method(rbgsl_cFunction, "integration_qags", Integration_qags, 5);
  rb_define_method(rbgsl_cFunction, "integration_qagp", Integration_qagp, 4);
  rb_define_method(rbgsl_cFunction, "integration_qagi", Integration_qagi, 3);
  rb_define_method(rbgsl_cFunction, "integration_qagiu", Integration_qagiu, 4);
  rb_define_method(rbgsl_cFunction, "integration_qagil", Integration_qagil, 4);
  rb_define_method(rbgsl_cFunction, "integration_qawc", Integration_qawc, 6);

  rb_define_method(rbgsl_cFunction, "integration_qaws", Integration_qaws, 6);
  rb_define_method(rbgsl_cFunction, "integration_qawo", Integration_qawo, 5);
  rb_define_method(rbgsl_cFunction, "integration_qawf", Integration_qawf, 4);

  rbgsl_cQAWSTable = rb_define_class_under(rbgsl_mIntegration, "QAWSTable", rb_cObject);
  rb_define_singleton_method(rbgsl_cQAWSTable, "alloc", Integration_qaws_table_alloc, 4);
  rb_define_singleton_method(rbgsl_cQAWSTable, "new", Integration_qaws_table_alloc, 4);
  rb_define_method(rbgsl_cQAWSTable, "to_a", Integration_qaws_table_to_a, 0);
  rb_define_method(rbgsl_cQAWSTable, "set", Integration_qaws_table_set, -1);
  rb_define_method(rb_cArray, "to_gsl_integration_qaws_table", Array_to_integration_qaws_table, 0);
  rb_define_alias(rb_cArray, "to_qaws_table", "to_gsl_integration_qaws_table");
  rb_define_alias(rb_cArray, "to_qaws_table", "to_gsl_integration_qaws_table");

  rbgsl_cQAWOTable = rb_define_class_under(rbgsl_mIntegration, "QAWOTable", rb_cObject);
  rb_define_singleton_method(rbgsl_cQAWOTable, "alloc", Integration_qawo_table_alloc, 4);
  rb_define_singleton_method(rbgsl_cQAWOTable, "new", Integration_qawo_table_alloc, 4);
  rb_define_method(rbgsl_cQAWOTable, "to_a", Integration_qawo_table_to_a, 0);
  rb_define_method(rb_cArray, "to_gsl_integration_qawo_table", Array_to_integration_qawo_table, 0);
  rb_define_method(rbgsl_cQAWOTable, "set", Integration_qawo_table_set, -1);
  rb_define_method(rbgsl_cQAWOTable, "set_length", Integration_qawo_table_set_length, 1);
}

/* vim: set ts=4: */
