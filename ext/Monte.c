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

#include <stdio.h>
#include "ruby.h"
#include "gsl/gsl_monte.h"
#include "gsl/gsl_monte_plain.h"
#include "gsl/gsl_monte_miser.h"
#include "gsl/gsl_monte_vegas.h"
#include "gsl/gsl_rng.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Monte Carlo Integration */

static ID ID_call, ID_arity;

/* Interface */

/* keeps reference to Proc object passed in the constructor */
typedef struct mystruct {
  VALUE proc;
  VALUE params;
  gsl_monte_function * func;
} mystruct_t;

/* Mark the hidden references so that the GC does not delete it. */
static void MFunction_mark(mystruct_t * ptr) {
  /* fprintf(stderr, "MFunction_mark\n"); */
  /* fprintf(stderr, "MFunction_mark: class = %s\n", rb_class2name(rb_class_of(ptr->proc))); */
  rb_gc_mark(ptr->proc);
  rb_gc_mark(ptr->params);
}

static void MFunction_free(mystruct_t * ptr) {
  /* fprintf(stderr, "MFunction_free\n"); */
  if (ptr->func)
    free(ptr->func);
  if (ptr)
    free(ptr);
}

static VALUE MFunction_init(VALUE self, VALUE dim, VALUE param, VALUE proc) {
  return self;
}

/* Call the passed Proc object.
 * This function gets called from the integrate functions. */
static double MFunction_function(double * x, size_t dim, void * p) {
  VALUE result;
  VALUE my_x;
  mystruct_t * ptr = p;

  /* fprintf(stderr, "function: dim = %d, class = %s\n", dim, 
   *  rb_class2name(rb_class_of(ptr->proc))); */

  COPYCARRAY(x, my_x, dim);

  /* fprintf(stderr, "function: before funcall\n"); */
  result = rb_funcall(ptr->proc, ID_call, 2, my_x, ptr->params);
  /* fprintf(stderr, "function: after funcall\n"); */

  return NUM2DBL(result);
}

static VALUE MFunction_new(VALUE klass, VALUE dim, VALUE param, VALUE proc) {
  mystruct_t * ptr;
  VALUE res;

  res = Data_Make_Struct(klass, mystruct_t, MFunction_mark, MFunction_free, ptr);
  ptr->proc = proc;
  ptr->params = param;
  /* fprintf(stderr, "MFunction_new: class = %s\n", rb_class2name(rb_class_of(proc))); */
  {
     VALUE tmp[3];
     tmp[0] = dim;
     tmp[1] = param;
     tmp[2] = proc;
     rb_obj_call_init(res, 3, tmp);
  }

  ptr->func = (gsl_monte_function *) malloc(sizeof(gsl_monte_function));
  ptr->func->params = ptr; /* we abuse the params to point to the Proc object */
  ptr->func->f = &MFunction_function;
  ptr->func->dim = NUM2INT(dim);

  return res;
}			

static VALUE MFunction_eval(int argc, VALUE * argv, VALUE self) {
  mystruct_t * ptr;
  VALUE res;
  /* fprintf(stderr, "MFunction_eval\n"); */
  Data_Get_Struct(self, mystruct_t, ptr);
  res = rb_funcall2(ptr->proc, ID_call, argc, argv);
  return res;
}

/*
static VALUE MFunction_eval2(VALUE self, VALUE x) {
  gsl_monte_function * ptr;
  // int dim = RARRAY(x)->len;
  Data_Get_Struct(self, gsl_monte_function, ptr);
  return rb_funcall2((VALUE) ptr->params, ID_call, 1, &x);
}

static VALUE MFunction_eval3(int argc, VALUE * argv, VALUE obj) {
  gsl_monte_function * ptr;
  Data_Get_Struct(obj, gsl_monte_function, ptr);
  return rb_funcall2((VALUE) ptr->params, ID_call, argc, argv);
}
*/

/* PLAIN Monte Carlo */

static VALUE MPlain_init(VALUE self, VALUE dim) {
  gsl_monte_plain_state * ptr;
  Data_Get_Struct(self, gsl_monte_plain_state, ptr);
  return gsl_monte_plain_init(ptr);
}

static void MPlain_free(void * p) {
  gsl_monte_plain_free(p);
}

static VALUE MPlain_new(VALUE klass, VALUE dim) {
  VALUE plain;
  gsl_monte_plain_state * ptr;

  ptr = gsl_monte_plain_alloc(NUM2INT(dim));
  plain = Data_Wrap_Struct(klass, 0, MPlain_free, ptr);
  {
    VALUE tmp[1];
    tmp[0] = dim;
    rb_obj_call_init(plain, 1, tmp);
  }
  return plain;
}

static VALUE MPlain_integrate(VALUE self, VALUE f, VALUE xl, VALUE xu,
  VALUE calls, VALUE r) {
  gsl_monte_plain_state * sptr;
  gsl_monte_function * fptr;
  gsl_rng * rptr;
  mystruct_t * ptr;
  double * my_xl, * my_xu;
  double result, abserr;
  int ret;

  if (RARRAY(xl)->len != RARRAY(xu)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size.");

  Data_Get_Struct(self, gsl_monte_plain_state, sptr);
  Data_Get_Struct(f, mystruct_t, ptr);
  fptr = ptr->func;
  Data_Get_Struct(r, gsl_rng, rptr);

  COPYRUBYARRAY(xl, my_xl);
  COPYRUBYARRAY(xu, my_xu);
  
  ret = gsl_monte_plain_integrate(fptr, my_xl, my_xu, RARRAY(xl)->len,
    NUM2INT(calls), rptr, sptr, &result, &abserr);

  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

/* MISER */

static VALUE MMiser_init(VALUE self, VALUE dim) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return gsl_monte_miser_init(ptr);
}

static void MMiser_free(void * p) {
  gsl_monte_miser_free(p);
}

static VALUE MMiser_new(VALUE klass, VALUE dim) {
  VALUE miser;
  gsl_monte_miser_state * ptr;

  ptr = gsl_monte_miser_alloc(NUM2INT(dim));
  miser = Data_Wrap_Struct(klass, 0, MMiser_free, ptr);
  {
    VALUE tmp[1];
    tmp[0] = dim;
    rb_obj_call_init(miser, 1, tmp);
  }
  return miser;
}

static VALUE MMiser_integrate(VALUE self, VALUE f, VALUE xl, VALUE xu,
  VALUE calls, VALUE r) {
  gsl_monte_miser_state * sptr;
  gsl_monte_function * fptr;
  gsl_rng * rptr;
  mystruct_t * ptr;
  double * my_xl, * my_xu;
  double result, abserr;
  int ret;

  if (RARRAY(xl)->len != RARRAY(xu)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size.");

  Data_Get_Struct(self, gsl_monte_miser_state, sptr);
  Data_Get_Struct(f, mystruct_t, ptr);
  fptr = ptr->func;
  Data_Get_Struct(r, gsl_rng, rptr);

  COPYRUBYARRAY(xl, my_xl);
  COPYRUBYARRAY(xu, my_xu);
  
  ret = gsl_monte_miser_integrate(fptr, my_xl, my_xu, RARRAY(xl)->len,
    NUM2INT(calls), rptr, sptr, &result, &abserr);

  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

static VALUE MMiser_get_estimate_frac(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return rb_float_new(ptr->estimate_frac);
}

static VALUE MMiser_set_estimate_frac(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->estimate_frac = NUM2DBL(v);
  return self;
}

static VALUE MMiser_get_min_calls(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return INT2FIX(ptr->min_calls);
}

static VALUE MMiser_set_min_calls(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->min_calls = NUM2INT(v);
  return self;
}

static VALUE MMiser_get_min_calls_per_bisection(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return INT2FIX(ptr->min_calls_per_bisection);
}

static VALUE MMiser_set_min_calls_per_bisection(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->min_calls_per_bisection = NUM2INT(v);
  return self;
}

static VALUE MMiser_get_alpha(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return rb_float_new(ptr->alpha);
}

static VALUE MMiser_set_alpha(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->alpha = NUM2DBL(v);
  return self;
}

static VALUE MMiser_get_dither(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return rb_float_new(ptr->dither);
}

static VALUE MMiser_set_dither(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->dither = NUM2DBL(v);
  return self;
}

/* VEGAS */

static VALUE MVegas_init(VALUE self, VALUE dim) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return gsl_monte_vegas_init(ptr);
}

static void MVegas_free(void * p) {
  gsl_monte_vegas_free(p);
}

static VALUE MVegas_new(VALUE klass, VALUE dim) {
  VALUE vegas;
  gsl_monte_vegas_state * ptr;

  ptr = gsl_monte_vegas_alloc(NUM2INT(dim));
  vegas = Data_Wrap_Struct(klass, 0, MVegas_free, ptr);
  {
    VALUE tmp[1];
    tmp[0] = dim;
    rb_obj_call_init(vegas, 1, tmp);
  }
  return vegas;
}

static VALUE MVegas_integrate(VALUE self, VALUE f, VALUE xl, VALUE xu,
  VALUE calls, VALUE r) {
  gsl_monte_vegas_state * sptr;
  gsl_monte_function * fptr;
  gsl_rng * rptr;
  mystruct_t * ptr;
  double * my_xl, * my_xu;
  double result, abserr;
  int ret;

  if (RARRAY(xl)->len != RARRAY(xu)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size.");

  Data_Get_Struct(self, gsl_monte_vegas_state, sptr);
  Data_Get_Struct(f, mystruct_t, ptr);
  fptr = ptr->func;
  Data_Get_Struct(r, gsl_rng, rptr);

  COPYRUBYARRAY(xl, my_xl);
  COPYRUBYARRAY(xu, my_xu);
  
  ret = gsl_monte_vegas_integrate(fptr, my_xl, my_xu, RARRAY(xl)->len,
    NUM2INT(calls), rptr, sptr, &result, &abserr);

  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

static VALUE MVegas_get_sigma(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->sigma);
}

static VALUE MVegas_get_result(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->result);
}

static VALUE MVegas_get_iterations(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return INT2FIX(ptr->iterations);
}

static VALUE MVegas_set_iterations(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->iterations = NUM2INT(v);
  return self;
}

static VALUE MVegas_get_stage(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return INT2FIX(ptr->stage);
}

static VALUE MVegas_set_stage(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->stage = NUM2INT(v);
  return self;
}

static VALUE MVegas_get_alpha(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->alpha);
}

static VALUE MVegas_set_alpha(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->alpha = NUM2DBL(v);
  return self;
}

static VALUE MVegas_get_chisq(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->chisq);
}

static VALUE MVegas_set_chisq(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->chisq = NUM2DBL(v);
  return self;
}

static VALUE MVegas_get_mode(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return INT2FIX(ptr->mode);
}

static VALUE MVegas_set_mode(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->mode = NUM2INT(v);
  return self;
}

/* Class definition */

VALUE rbgsl_mMonteCarlo;
VALUE rbgsl_cMonteFunction;
VALUE rbgsl_cMPlain;
VALUE rbgsl_cMMiser;
VALUE rbgsl_cMVegas;

void Init_MonteCarlo() {
  rbgsl_mMonteCarlo = rb_define_module_under(rbgsl_mGSL, "MonteCarlo");
  rbgsl_cMonteFunction = rb_define_class_under(rbgsl_mMonteCarlo, "Function", rb_cObject);

  rb_define_singleton_method(rbgsl_cMonteFunction, "new", MFunction_new, 3);
  rb_define_method(rbgsl_cMonteFunction, "initialize", MFunction_init, 3);
  rb_define_method(rbgsl_cMonteFunction, "eval", MFunction_eval, -1);
/*
  rb_define_method(rbgsl_cMonteFunction, "eval2", MFunction_eval2, 1);
  rb_define_method(rbgsl_cMonteFunction, "eval3", MFunction_eval3, -1);
*/

  rbgsl_cMPlain = rb_define_class_under(rbgsl_mMonteCarlo, "Plain", rb_cObject);
  rb_define_singleton_method(rbgsl_cMPlain, "new", MPlain_new, 1);
  rb_define_method(rbgsl_cMPlain, "initialize", MPlain_init, 1);
  rb_define_method(rbgsl_cMPlain, "integrate", MPlain_integrate, 5);

  rbgsl_cMMiser = rb_define_class_under(rbgsl_mMonteCarlo, "Miser", rb_cObject);
  rb_define_singleton_method(rbgsl_cMMiser, "new", MMiser_new, 1);
  rb_define_method(rbgsl_cMMiser, "initialize", MMiser_init, 1);
  rb_define_method(rbgsl_cMMiser, "integrate", MMiser_integrate, 5);

  rb_define_method(rbgsl_cMMiser, "estimate_frac", MMiser_get_estimate_frac, 0);
  rb_define_method(rbgsl_cMMiser, "estimate_frac=", MMiser_set_estimate_frac, 1);
  rb_define_method(rbgsl_cMMiser, "min_calls", MMiser_get_min_calls, 0);
  rb_define_method(rbgsl_cMMiser, "min_calls=", MMiser_set_min_calls, 1);
  rb_define_method(rbgsl_cMMiser, "min_calls_per_bisection", MMiser_get_min_calls_per_bisection, 0);
  rb_define_method(rbgsl_cMMiser, "min_calls_per_bisection=", MMiser_set_min_calls_per_bisection, 1);
  rb_define_method(rbgsl_cMMiser, "alpha", MMiser_get_alpha, 0);
  rb_define_method(rbgsl_cMMiser, "alpha=", MMiser_set_alpha, 1);
  rb_define_method(rbgsl_cMMiser, "dither", MMiser_get_dither, 0);
  rb_define_method(rbgsl_cMMiser, "dither=", MMiser_set_dither, 1);

  rbgsl_cMVegas = rb_define_class_under(rbgsl_mMonteCarlo, "Vegas", rb_cObject);
  rb_define_singleton_method(rbgsl_cMVegas, "new", MVegas_new, 1);
  rb_define_method(rbgsl_cMVegas, "initialize", MVegas_init, 1);
  rb_define_method(rbgsl_cMVegas, "integrate", MVegas_integrate, 5);

  rb_define_method(rbgsl_cMVegas, "result", MVegas_get_result, 0);
  rb_define_method(rbgsl_cMVegas, "sigma", MVegas_get_sigma, 0);
  rb_define_method(rbgsl_cMVegas, "iterations", MVegas_get_iterations, 0);
  rb_define_method(rbgsl_cMVegas, "iterations=", MVegas_set_iterations, 1);
  rb_define_method(rbgsl_cMVegas, "alpha", MVegas_get_alpha, 0);
  rb_define_method(rbgsl_cMVegas, "alpha=", MVegas_set_alpha, 1);
  rb_define_method(rbgsl_cMVegas, "chisq", MVegas_get_chisq, 0);
  rb_define_method(rbgsl_cMVegas, "chisq=", MVegas_set_chisq, 1);
  rb_define_method(rbgsl_cMVegas, "stage", MVegas_get_stage, 0);
  rb_define_method(rbgsl_cMVegas, "stage=", MVegas_set_stage, 1);
  rb_define_method(rbgsl_cMVegas, "mode", MVegas_get_mode, 0);
  rb_define_method(rbgsl_cMVegas, "mode=", MVegas_set_mode, 1);

  rb_define_const(rbgsl_cMVegas, "MODE_IMPORTANCE", INT2FIX(GSL_VEGAS_MODE_IMPORTANCE));
  rb_define_const(rbgsl_cMVegas, "MODE_IMPORTANCE_ONLY", INT2FIX(GSL_VEGAS_MODE_IMPORTANCE_ONLY));
  rb_define_const(rbgsl_cMVegas, "MODE_STRATIFIED", INT2FIX(GSL_VEGAS_MODE_STRATIFIED));

  ID_call = rb_intern("call");
  ID_arity = rb_intern("arity");

}

/* vim: set ts=4: */
