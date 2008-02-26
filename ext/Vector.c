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

/* $Id: Vector.c,v 1.14 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <math.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_linalg.h"
#include "ruby-gsl.h"

/* Vectors */

VALUE rbgsl_cVector;

/* Vector allocation */

static VALUE Vector_new(VALUE klass, VALUE arg) {
  gsl_vector * v = NULL;
  size_t i, n;

  switch (TYPE(arg)) {
    case T_FIXNUM:
    case T_BIGNUM:
      n = NUM2INT(arg);
      v = gsl_vector_alloc(n);
      break;
    case T_ARRAY:
      n = RARRAY(arg)->len;
      v = gsl_vector_alloc(n);
      for (i = 0; i < n; i++)
        gsl_vector_set(v, i, NUM2DBL(rb_ary_entry(arg, i)));
      break;
    default:
      rb_raise(rb_eArgError, "Illegal argument for constructor");
  }

  return Data_Wrap_Struct(klass, 0, gsl_vector_free, v);;
}

static VALUE Vector_calloc(VALUE klass, VALUE n) {
  VALUE obj;
  gsl_vector * v;

  v = gsl_vector_calloc(NUM2INT(n));

  obj = Data_Wrap_Struct(klass, 0, gsl_vector_free, v);

  return obj;
}

static VALUE Vector_size(VALUE obj) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  return INT2NUM(v->size);
}

static VALUE Vector_stride(VALUE obj) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  return INT2FIX(v->stride);
}

static VALUE Vector_owner(VALUE obj) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  return INT2FIX(v->owner);
}

/* Accessing vector elements */

static VALUE Vector_get(VALUE obj, VALUE i) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  return rb_float_new(gsl_vector_get(v, NUM2INT(i)));
}

static VALUE Vector_set(int argc, VALUE * argv, VALUE obj) {
  gsl_vector * v;
  size_t i, size, size0;
  double x;
  VALUE ary;

  switch (TYPE(argv[0])) {
    case T_FIXNUM:
    case T_BIGNUM:
      i = NUM2INT(argv[0]);
      x = NUM2DBL(argv[1]);

      Data_Get_Struct(obj, gsl_vector, v);

      gsl_vector_set(v, i, x);
      break;
    case T_ARRAY:
      ary = argv[0];
      Data_Get_Struct(obj, gsl_vector, v);
      size0 = v->size;
      size = RARRAY(ary)->len;
      if (size > size0)
        rb_raise(rb_eArgError, "wrong number of elements");
      for (i = 0; i < size; i++)
        gsl_vector_set(v, i, NUM2DBL(rb_ary_entry(ary, i)));
      break;
  }

  return obj;
}

static VALUE Vector_set1(VALUE obj, VALUE vi, VALUE vval)
{
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  gsl_vector_set(v, NUM2INT(vi), NUM2DBL(vval));
  return obj;
}

/* Initializing vector elements */

static VALUE Vector_set_all(VALUE obj, VALUE xx) {
  gsl_vector * v;
  double x;

  x = NUM2DBL(xx);
  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_set_all(v, x);

  return obj;
}

static VALUE Vector_set_zero(VALUE obj) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_set_zero(v);

  return obj;
}

static VALUE Vector_set_basis(VALUE obj, VALUE ii) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_set_basis(v, NUM2INT(ii));

  return obj;
}

static VALUE Vector_set_basis2(VALUE obj, VALUE ii) {
  gsl_vector * v, * w;

  Data_Get_Struct(obj, gsl_vector, v);
  w = gsl_vector_alloc(v->size);

  gsl_vector_set_basis(w, NUM2INT(ii));

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, w);
}

/* Vector views */

static VALUE Vector_subvector(VALUE obj, VALUE o, VALUE n) {
  gsl_vector * v, * w;
  gsl_vector_view vtmp;

  Data_Get_Struct(obj, gsl_vector, v);

  w = gsl_vector_alloc(NUM2INT(n));

  vtmp = gsl_vector_subvector(v, NUM2INT(o), NUM2INT(n));
  *w = vtmp.vector;

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, w);
}

static VALUE Vector_subvector_with_stride(VALUE obj, VALUE o, VALUE s, VALUE n) {
  gsl_vector * v, * w;
  gsl_vector_view vtmp;

  Data_Get_Struct(obj, gsl_vector, v);

  w = gsl_vector_alloc(NUM2INT(n));

  vtmp = gsl_vector_subvector_with_stride(v, NUM2INT(o), NUM2INT(s), NUM2INT(n));
  *w = vtmp.vector;

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, w);
}

/* Copying vectors */

/* module function */
static VALUE Vector_memcpy(VALUE obj, VALUE dest, VALUE src) {
  gsl_vector * vdest, * vsrc;

  Data_Get_Struct(dest, gsl_vector, vdest);
  Data_Get_Struct(src, gsl_vector, vsrc);

  gsl_vector_memcpy(vdest, vsrc);

  return obj;
}

/* method */
static VALUE Vector_clone(VALUE obj) {
  gsl_vector * v, * new;

  Data_Get_Struct(obj, gsl_vector, v);
  new = gsl_vector_alloc(v->size);

  gsl_vector_memcpy(new, v);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, new);
}

/* module function */
static VALUE Vector_swap(VALUE obj, VALUE vv, VALUE ww) {
  gsl_vector * v, * w;

  Data_Get_Struct(vv, gsl_vector, v);
  Data_Get_Struct(ww, gsl_vector, w);

  gsl_vector_swap(w, v);

  return obj;
}

/* Exchanging elements */

static VALUE Vector_swap_elements(VALUE obj, VALUE i, VALUE j) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_swap_elements(v, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Vector_reverse(VALUE obj) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_reverse(v);

  return obj;
}

/* Vector operations */

static VALUE Vector_add(VALUE obj, VALUE b) {
  gsl_vector * v, * w, * tmp;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);
  tmp = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(tmp, v);

  gsl_vector_add(tmp, w);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tmp);
}

static VALUE Vector_add2(VALUE obj, VALUE b) {
  gsl_vector * v, * w;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);

  gsl_vector_add(v, w);

  return obj;
}

static VALUE Vector_sub(VALUE obj, VALUE b) {
  gsl_vector * v, * w, * tmp;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);
  tmp = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(tmp, v);

  gsl_vector_sub(tmp, w);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tmp);
}

static VALUE Vector_sub2(VALUE obj, VALUE b) {
  gsl_vector * v, * w;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);

  gsl_vector_sub(v, w);

  return obj;
}

static VALUE Vector_mul(VALUE obj, VALUE b) {
  gsl_vector * v, * w, * tmp;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);
  tmp = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(tmp, v);

  gsl_vector_mul(tmp, w);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tmp);
}

static VALUE Vector_mul2(VALUE obj, VALUE b) {
  gsl_vector * v, * w;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);

  gsl_vector_mul(v, w);

  return obj;
}

static VALUE Vector_div(VALUE obj, VALUE b) {
  gsl_vector * v, * w, * tmp;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);
  tmp = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(tmp, v);

  gsl_vector_div(tmp, w);

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tmp);
}

static VALUE Vector_div2(VALUE obj, VALUE b) {
  gsl_vector * v, * w;

  Data_Get_Struct(obj, gsl_vector, v);
  Data_Get_Struct(b, gsl_vector, w);

  gsl_vector_div(v, w);

  return obj;
}

static VALUE Vector_scale(VALUE obj, VALUE x) {
  gsl_vector * v, * tmp;

  Data_Get_Struct(obj, gsl_vector, v);
  tmp = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(tmp, v);

  gsl_vector_scale(tmp, NUM2DBL(x));

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tmp);
}

static VALUE Vector_scale2(VALUE obj, VALUE x) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_scale(v, NUM2DBL(x));

  return obj;
}

static VALUE Vector_add_const(VALUE obj, VALUE x) {
  gsl_vector * v, * tmp;

  Data_Get_Struct(obj, gsl_vector, v);
  tmp = gsl_vector_alloc(v->size);
  gsl_vector_memcpy(tmp, v);

  gsl_vector_add_constant(tmp, NUM2DBL(x));

  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, tmp);
}

static VALUE Vector_add_const2(VALUE obj, VALUE x) {
  gsl_vector * v;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_add_constant(v, NUM2DBL(x));

  return obj;
}

/* Finding maximum and minimum elements of vectors */

static VALUE Vector_max(VALUE obj) {
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  return rb_float_new(gsl_vector_max(v));
}

static VALUE Vector_min(VALUE obj) {
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  return rb_float_new(gsl_vector_min(v));
}

static VALUE Vector_minmax(VALUE obj) {
  gsl_vector * v;
  double min, max;

  Data_Get_Struct(obj, gsl_vector, v);

  gsl_vector_minmax(v, &min, &max);

  return rb_ary_new3(2, rb_float_new(min), rb_float_new(max));
}

static VALUE Vector_max_index(VALUE obj) {
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  return INT2NUM(gsl_vector_max_index(v));
}

static VALUE Vector_min_index(VALUE obj) {
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  return INT2NUM(gsl_vector_min_index(v));
}

static VALUE Vector_minmax_index(VALUE obj) {
  gsl_vector * v;
  size_t imin, imax;

  Data_Get_Struct(obj, gsl_vector, v);
  gsl_vector_minmax_index(v, &imin, &imax);
  return rb_ary_new3(2, INT2NUM(imin), INT2NUM(imax));
}

/* Vector properties */

static VALUE Vector_isnull(VALUE obj) {
  gsl_vector * v;
  Data_Get_Struct(obj, gsl_vector, v);
  return INT2FIX(gsl_vector_isnull(v));
}

/* Reading and writing vectors */

static VALUE Vector_fwrite(VALUE obj, VALUE io) {
  gsl_vector * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector, v);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_vector_fwrite(f, v);
  return INT2FIX(status);
}

static VALUE Vector_fread(VALUE obj, VALUE io) {
  gsl_vector * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector, v);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_vector_fread(f, v);
  return INT2FIX(status);
}

static VALUE Vector_fprintf(VALUE obj, VALUE io, VALUE format) {
  gsl_vector * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector, v);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_vector_fprintf(f, v, STR2CSTR(format));
  return INT2FIX(status);
}

static VALUE Vector_printf(VALUE obj, VALUE format) {
  gsl_vector * v;
  int status;

  Data_Get_Struct(obj, gsl_vector, v);
  status = gsl_vector_fprintf(stdout, v, STR2CSTR(format));
  return INT2FIX(status);
}

static VALUE Vector_fscanf(VALUE obj, VALUE io) {
  gsl_vector * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector, v);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_vector_fscanf(f, v);
  return INT2FIX(status);
}

/*  Ruby convenience methods */

/* Warning: This function converts to a ruby Array of ruby Floats.
 * These work fine as long as they stay in memory, but they accrue
 * rounding errors when dumped or loaded by the Marshal, as it uses
 * a platform-independent ASCII-decimal representation that cannot exactly
 * represent certain bit patterns like 4.0/3.0
 */

static VALUE Vector_to_a(VALUE obj) {
  gsl_vector * v;
  VALUE ary;
  size_t i;

  Data_Get_Struct(obj, gsl_vector, v);

  ary = rb_ary_new();
  for (i = 0; i < v->size; i++)
    rb_ary_push(ary, rb_float_new(gsl_vector_get(v, i)));

  return ary;
}

VALUE rb_ary_to_v(VALUE ary) {
  gsl_vector * v;
  size_t i, size;

  size = RARRAY(ary)->len;
  v = gsl_vector_alloc(size);

  for (i = 0; i < size; i++)
    gsl_vector_set(v, i, NUM2DBL(rb_ary_entry(ary, i)));


  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, v);
}

/* Must allocate a new generic instance without any parameters
 * Since we don't know how big the object will be later, just allocate
 * a minimum-sized placeholder
 */
#ifdef HAVE_DEFINE_ALLOC_FUNCTION
static VALUE Vector_allocate(VALUE kl) {
  return Vector_calloc(kl, INT2NUM(1));
}
#endif

/* Must return a Ruby object that contains this instance's state */
static VALUE Vector_dump_data(VALUE obj) {
  return Vector_to_a(obj);
}

/* Must overwrite the instance object state with the interpretation
 * represented in the arg object by a corresponding _dump_data call.
 * This is called after the allocate function by Marshal.load
 */
static VALUE Vector_load_data(VALUE obj, VALUE arg) {
  int i;
  gsl_vector *vector;
  Data_Get_Struct(obj, gsl_vector, vector);
  /* If this vector has a block all its own, it must be freed first */
  if (vector->owner) {
    gsl_block_free(vector->block);
  }
  vector->block = gsl_block_alloc(RARRAY(arg)->len);
  vector->data = vector->block->data;
  vector->size = RARRAY(arg)->len;
  vector->stride = 1;
  vector->owner = 1; /* this does indeed own the memory allocated above */
  for (i = 0; i < RARRAY(arg)->len; ++i)
    gsl_vector_set(vector, i, NUM2DBL(rb_ary_entry(arg, i)));
  return Qtrue;
}

static VALUE Vector_each(VALUE obj) {
  gsl_vector * v;
  size_t i;

  Data_Get_Struct(obj, gsl_vector, v);

  for (i = 0; i < v->size; i++)
    rb_yield(rb_float_new(gsl_vector_get(v, i)));

  return obj;
}

static VALUE Vector_print(VALUE obj) {
  gsl_vector * v;
  size_t i;

  Data_Get_Struct(obj, gsl_vector, v);
  printf("[ ");
  for (i = 0; i < v->size; i++)
    printf("%4.3e ", v->data[i * v->stride]);
  printf("]\n");

  return obj;
}

static VALUE Vector_inspect(VALUE obj) {
  Vector_print(obj);
  return obj;
}

static VALUE Vector_equal(VALUE self, VALUE bb) {
  gsl_vector * a, * b;
  size_t na, nb;
  size_t i;
  Data_Get_Struct(self, gsl_vector, a);
  Data_Get_Struct(bb, gsl_vector, b);
  na = a->size;
  nb = b->size;
  if (na != nb)
    return Qfalse;

  for(i = 0; i < na; i++) {
    if(gsl_vector_get(a,i) != gsl_vector_get(b,i))
      return Qfalse;
  }
  return Qtrue;
}
  
static VALUE Vector_float_equal(VALUE self, VALUE aa, VALUE bb, VALUE eps) {
  gsl_vector * a, * b;
  size_t na, nb;
  size_t i;
  double d1 = NUM2DBL(eps);
  Data_Get_Struct(aa, gsl_vector, a);
  Data_Get_Struct(bb, gsl_vector, b);
  na = a->size;
  nb = b->size;
  if (na != nb)
    return Qfalse;

  for(i = 0; i < na; i++) {
    if(fabs(gsl_vector_get(a,i) - gsl_vector_get(b,i)) >= d1) 
      return Qfalse;
  }
  return Qtrue;
}


/* Class definition */

void Init_Vector() {
  rbgsl_cVector = rb_define_class_under(rbgsl_mGSL, "Vector", rb_cObject);

  rb_define_singleton_method(rbgsl_cVector, "new", Vector_new, 1);
  rb_define_singleton_method(rbgsl_cVector, "alloc", Vector_new, 1);
  rb_define_singleton_method(rbgsl_cVector, "calloc", Vector_calloc, 1);

  rb_define_method(rbgsl_cVector, "size", Vector_size, 0);
  rb_define_method(rbgsl_cVector, "stride", Vector_stride, 0);
  rb_define_method(rbgsl_cVector, "owner", Vector_owner, 0);
  rb_define_method(rbgsl_cVector, "get", Vector_get, 1);
  rb_define_method(rbgsl_cVector, "set", Vector_set, -1);
  rb_define_method(rbgsl_cVector, "set1", Vector_set1, 2);
  rb_define_alias(rbgsl_cVector, "[]=", "set1");
  rb_define_alias(rbgsl_cVector, "[]", "get");
  rb_define_method(rbgsl_cVector, "set_all", Vector_set_all, 1);
  rb_define_method(rbgsl_cVector, "set_zero", Vector_set_zero, 0);
  rb_define_method(rbgsl_cVector, "set_basis!", Vector_set_basis, 1);
  rb_define_method(rbgsl_cVector, "set_basis", Vector_set_basis2, 1);
  rb_define_method(rbgsl_cVector, "subvector", Vector_subvector, 2);
  rb_define_method(rbgsl_cVector, "subvector_with_stride", Vector_subvector_with_stride, 3);

  rb_define_module_function(rbgsl_cVector, "memcpy", Vector_memcpy, 2);
  rb_define_module_function(rbgsl_cVector, "swap", Vector_swap, 2);

  rb_define_method(rbgsl_cVector, "clone", Vector_clone, 0);
  rb_define_method(rbgsl_cVector, "swap_elements", Vector_swap_elements, 2);
  rb_define_method(rbgsl_cVector, "reverse!", Vector_reverse, 0);
  rb_define_method(rbgsl_cVector, "add", Vector_add, 1);
  rb_define_alias(rbgsl_cVector, "+", "add");
  rb_define_method(rbgsl_cVector, "add!", Vector_add2, 1);
  rb_define_method(rbgsl_cVector, "sub", Vector_sub, 1);
  rb_define_alias(rbgsl_cVector, "-", "sub");
  rb_define_method(rbgsl_cVector, "sub!", Vector_sub2, 1);
  rb_define_method(rbgsl_cVector, "mul", Vector_mul, 1);
  rb_define_method(rbgsl_cVector, "mul!", Vector_mul2, 1);
  rb_define_method(rbgsl_cVector, "div", Vector_div, 1);
  rb_define_alias(rbgsl_cVector, "/", "div");
  rb_define_method(rbgsl_cVector, "div!", Vector_div2, 1);
  rb_define_method(rbgsl_cVector, "scale", Vector_scale, 1);
  rb_define_method(rbgsl_cVector, "scale!", Vector_scale2, 1);
  rb_define_method(rbgsl_cVector, "add_const", Vector_add_const, 1);
  rb_define_method(rbgsl_cVector, "add_const!", Vector_add_const2, 1);
  rb_define_method(rbgsl_cVector, "max", Vector_max, 0);
  rb_define_method(rbgsl_cVector, "min", Vector_min, 0);
  rb_define_method(rbgsl_cVector, "minmax", Vector_minmax, 0);
  rb_define_method(rbgsl_cVector, "max_index", Vector_max_index, 0);
  rb_define_method(rbgsl_cVector, "min_index", Vector_min_index, 0);
  rb_define_method(rbgsl_cVector, "minmax_index", Vector_minmax_index, 0);
  rb_define_method(rbgsl_cVector, "isnull", Vector_isnull, 0);

  rb_define_method(rbgsl_cVector, "fwrite", Vector_fwrite, 1);
  rb_define_method(rbgsl_cVector, "fread", Vector_fread, 1);
  rb_define_method(rbgsl_cVector, "fprintf", Vector_fprintf, 2);
  rb_define_method(rbgsl_cVector, "printf", Vector_printf, 1);
  rb_define_method(rbgsl_cVector, "fscanf", Vector_fscanf, 1);

  rb_define_method(rbgsl_cVector, "to_a", Vector_to_a, 0);
  rb_define_method(rbgsl_cVector, "each", Vector_each, 0);
  rb_define_method(rb_cArray, "to_v", rb_ary_to_v, 0);
  rb_define_alias(rb_cArray, "to_gv", "to_v");
  rb_define_method(rbgsl_cVector, "print", Vector_print, 0);
  rb_define_method(rbgsl_cVector, "inspect", Vector_inspect, 0);
  
  rb_define_method(rbgsl_cVector, "equal", Vector_equal, 1);
  rb_define_alias(rbgsl_cVector, "==", "equal");
  rb_define_singleton_method(rbgsl_cVector, "float_equal", Vector_float_equal, 3);

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
  rb_define_alloc_func(rbgsl_cVector, Vector_allocate);
#endif

  /* These must be called _dump_data to signify that they are the special
   * functions to be called implicitly from the Marshal engine.
   * The word data refers to the fact that they are really C struct types
   * created using the alloc func and Data_Get_Struct
   */
  rb_define_method(rbgsl_cVector, "_dump_data", Vector_dump_data, 0);
  rb_define_method(rbgsl_cVector, "_load_data", Vector_load_data, 1);

}
/* vim: set ts=4: */
