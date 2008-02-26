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

/* $Id: Siman.c,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <stdio.h>
#include <ruby.h>
#include <gsl/gsl_siman.h>
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Internal use only */

typedef void (*func_t) (void *);

extern void callme(void *, func_t);

/* Simulated Annealing */

typedef struct {
  VALUE conf;
  VALUE energy;
  VALUE step;
  VALUE metric;
  VALUE print;
  gsl_siman_params_t params;
} mystruct_t;

static ID ID_call;

double energy_callback(void * xp) {
  VALUE result;
  mystruct_t * ptr = (mystruct_t *) xp;
  /* fprintf(stderr, "energy_callback: "); */
  result = rb_funcall(ptr->energy, ID_call, 1, ptr->conf);
  /* fprintf(stderr, "%g\n", NUM2DBL(result)); */
  return NUM2DBL(result);
}

void step_callback(const gsl_rng * r, void * xp, double step_size) {
  double u = gsl_rng_uniform(r);
  mystruct_t * ptr = (mystruct_t *) xp;
  /* VALUE ary = ptr->conf; */
  /* fprintf(stderr, "step_callback: x_old = %g, ", NUM2DBL(rb_ary_entry(ary, 0))); */
  rb_funcall(ptr->step, ID_call, 3, ptr->conf, rb_float_new(u), rb_float_new(step_size));
  /* fprintf(stderr, "x_new = %g\n", NUM2DBL(rb_ary_entry(ary, 0))); */
}

double metric_callback(void * xp, void * yp) {
  VALUE result;
  mystruct_t * xptr = (mystruct_t *) xp;
  mystruct_t * yptr = (mystruct_t *) yp;
  /* fprintf(stderr, "metric_callback: "); */
  result = rb_funcall(xptr->metric, ID_call, 2, xptr->conf, yptr->conf);
  /* fprintf(stderr, "%g\n", NUM2DBL(result)); */
  return NUM2DBL(result);
}

void print_callback(void * xp) {
  mystruct_t * ptr = (mystruct_t *) xp;
  /* fprintf(stderr, "print_callback\n"); */
  rb_funcall(ptr->print, ID_call, 1, ptr->conf);
}

static VALUE Siman_initialize(VALUE self) {
  return self;
}

static void Siman_mark(mystruct_t * ptr) {
  /* fprintf(stderr, "Siman#mark\n"); */
  if (ptr) {
    rb_gc_mark(ptr->energy);
    rb_gc_mark(ptr->step);
    rb_gc_mark(ptr->metric);
    rb_gc_mark(ptr->print);
  }
}

static void Siman_free(mystruct_t * ptr) {
  /* fprintf(stderr, "Siman#free\n"); */
  if (ptr)
    free(ptr);
}

VALUE Siman_new(int argc, VALUE * argv, VALUE class) {
  VALUE foo;
  mystruct_t * ptr;
  gsl_siman_params_t params;

  /* fprintf(stderr, "Siman#new\n"); */

  params.n_tries = 200;
  params.iters_fixed_T = 10;
  params.step_size = 10.0;
  params.k = 1.0;
  params.t_initial = 0.002;
  params.mu_t = 1.005;
  params.t_min = 2.0e-6;

  if (argc > 0)
    params.n_tries = NUM2INT(argv[0]);
  if (argc > 1)
    params.iters_fixed_T = NUM2INT(argv[1]);
  if (argc > 2)
    params.step_size = NUM2DBL(argv[2]);
  if (argc > 3)
    params.k = NUM2DBL(argv[3]);
  if (argc > 4)
    params.t_initial = NUM2DBL(argv[4]);
  if (argc > 5)
    params.mu_t = NUM2DBL(argv[5]);
  if (argc == 7)
    params.t_min = NUM2DBL(argv[6]);

  foo = Data_Make_Struct(class, mystruct_t, Siman_mark, Siman_free, ptr);
  ptr->params = params;

  return foo;
}
 
/* set the callback functions */

static VALUE Siman_set_energy(VALUE self, VALUE proc) {
  mystruct_t * ptr;
  /* fprintf(stderr, "Siman#set_energy: %s\n", rb_class2name(rb_class_of(proc))); */
  Data_Get_Struct(self, mystruct_t, ptr);
  ptr->energy = proc;
  return self;
}

static VALUE Siman_set_metric(VALUE self, VALUE proc) {
  mystruct_t * ptr;
  /* fprintf(stderr, "Siman#set_metric: %s\n", rb_class2name(rb_class_of(proc))); */
  Data_Get_Struct(self, mystruct_t, ptr);
  ptr->metric = proc;
  return self;
}

static VALUE Siman_set_step(VALUE self, VALUE proc) {
  mystruct_t * ptr;
  /* fprintf(stderr, "Siman#set_step: %s\n", rb_class2name(rb_class_of(proc))); */
  Data_Get_Struct(self, mystruct_t, ptr);
  ptr->step = proc;
  return self;
}

static VALUE Siman_set_print(VALUE self, VALUE proc) {
  mystruct_t * ptr;
  /* fprintf(stderr, "Siman#set_print: %s\n", rb_class2name(rb_class_of(proc))); */
  Data_Get_Struct(self, mystruct_t, ptr);
  ptr->print = proc;
  return self;
}

/* start the search */

static VALUE Siman_search(VALUE self, VALUE rng, VALUE conf) {
  gsl_rng * rptr;
  mystruct_t * ptr;
  
  /* fprintf(stderr, "Siman#search: %s\n", rb_class2name(rb_class_of(conf))); */
  
  Data_Get_Struct(self, mystruct_t, ptr);
  Data_Get_Struct(rng, gsl_rng, rptr);
  ptr->conf = conf;
  /* callme(ptr, &energy_callback); */
  
  gsl_siman_solve(rptr, ptr, energy_callback, step_callback, metric_callback, 
    print_callback, NULL, NULL, NULL, sizeof(mystruct_t), ptr->params);
  
  return self;
}

/* Class definition */

VALUE rbgsl_cSiman;

void Init_Siman() {
  rbgsl_cSiman = rb_define_class_under(rbgsl_mGSL, "Siman", rb_cObject);

  rb_define_singleton_method(rbgsl_cSiman, "new", Siman_new, -1);
  rb_define_method(rbgsl_cSiman, "initialize", Siman_initialize, -1);

  rb_define_method(rbgsl_cSiman, "search", Siman_search, 2);
  
  rb_define_method(rbgsl_cSiman, "set_energy", Siman_set_energy, 1);
  rb_define_method(rbgsl_cSiman, "set_metric", Siman_set_metric, 1);
  rb_define_method(rbgsl_cSiman, "set_step", Siman_set_step, 1);
  rb_define_method(rbgsl_cSiman, "set_print", Siman_set_print, 1);
  rb_define_alias(rbgsl_cSiman, "energy=", "set_energy");
  rb_define_alias(rbgsl_cSiman, "metric=", "set_metric");
  rb_define_alias(rbgsl_cSiman, "step=", "set_step");
  rb_define_alias(rbgsl_cSiman, "print=", "set_print");

  ID_call = rb_intern("call");
}

/* vim: set ts=4: */
