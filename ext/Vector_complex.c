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

/* $Id: Vector_complex.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <math.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_vector_complex.h"
#include "gsl/gsl_linalg.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Complex Vectors */

VALUE rbgsl_cVector_complex;

/* Internal use only */

/* Given a variable-length argument list consisting of 1 or more
 * complex numbers, create a vector with size = argc
 */
static gsl_vector_complex * alloc_vector_from_complex(int argc, VALUE * argv)
{
  gsl_vector_complex * v = NULL;
  gsl_complex * pz;
  int n, i;
  n = argc;
  v = gsl_vector_complex_alloc(n);
  for (i = 0; i < n; i++) {
    Data_Get_Struct(argv[i], gsl_complex, pz);
    gsl_vector_complex_set(v, i, *pz);
  }
  return v;
}

/* Vector allocation */

static VALUE Vector_complex_new(VALUE klass, VALUE arg) {
  gsl_vector_complex * v = NULL;
  size_t i, n, dim;
  switch (TYPE(arg)) {
    case T_FIXNUM:
    case T_BIGNUM:
      dim = NUM2INT(arg);
      v = gsl_vector_complex_calloc(dim);
      break;
    case T_ARRAY:
      n = RARRAY(arg)->len;
      dim = n/2;
      v = gsl_vector_complex_alloc(dim);
      for (i = 0; i < dim; i++){
        double d0, d1;
        gsl_complex z;
        d0 = NUM2DBL(rb_ary_entry(arg, 2*i));
        d1 = NUM2DBL(rb_ary_entry(arg, 2*i+1));
        GSL_SET_COMPLEX(&z, d0, d1);
        gsl_vector_complex_set(v, i, z);
      }
      break;
    default:
      rb_raise(rb_eArgError, "Illegal argument for constructor");
  }

  return Data_Wrap_Struct(klass, 0, gsl_vector_complex_free, v);
}

static VALUE Vector_complex_new2(int argc, VALUE * argv, VALUE klass)
{
  gsl_vector_complex * v = NULL;
  int type0;
  type0 = TYPE(argv[0]);

  if (type0 == T_FIXNUM || type0 == T_BIGNUM) {
    gsl_complex * pz;
    VALUE vz;
    int i, n;
    n = NUM2INT(argv[0]);
    Data_Get_Struct(argv[1], gsl_complex, pz);
    vz = Complex_new_intern(rbgsl_cComplex, pz);
    v = gsl_vector_complex_alloc(n);
    for (i = 0; i < n; i++)
      gsl_vector_complex_set(v, i, *pz);
    return Data_Wrap_Struct(klass, 0, gsl_vector_complex_free, v);
    }
  v = alloc_vector_from_complex(argc, argv);
  return Data_Wrap_Struct(klass, 0, gsl_vector_complex_free, v);
}

static VALUE Vector_complex_calloc(VALUE klass, VALUE n) {
  VALUE obj;
  gsl_vector_complex * v;

  v = gsl_vector_complex_calloc(NUM2INT(n));

  obj = Data_Wrap_Struct(klass, 0, gsl_vector_complex_free, v);

  return obj;
}

/* Accessing vector elements */

static VALUE Vector_complex_get(VALUE obj, VALUE i) {
  gsl_vector_complex * v;
  gsl_complex z;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  z = gsl_vector_complex_get(v, NUM2INT(i));
  return Complex_new_intern(rbgsl_cComplex, &z);
}

static VALUE Vector_complex_set1(VALUE obj, VALUE vi, VALUE va)
{
  gsl_vector_complex * v;
  gsl_complex * pz;
  Data_Get_Struct(va, gsl_complex, pz);
  Data_Get_Struct(obj, gsl_vector_complex, v);
  gsl_vector_complex_set(v, NUM2INT(vi), *pz);
  return obj;
}

/* Initializing vector elements */

static VALUE Vector_complex_set_all(VALUE obj, VALUE va) {
  gsl_vector_complex * v;
  gsl_complex z;
  double d0, d1;
  d0 = NUM2DBL(rb_ary_entry(va,0));
  d1 = NUM2DBL(rb_ary_entry(va,1));
  GSL_SET_COMPLEX(&z, d0, d1);
  Data_Get_Struct(obj, gsl_vector_complex, v);

  gsl_vector_complex_set_all(v, z);

  return obj;
}

static VALUE Vector_complex_set_zero(VALUE obj) {
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  gsl_vector_complex_set_zero(v);

  return obj;
}

static VALUE Vector_complex_set_basis(VALUE obj, VALUE ii) {
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  gsl_vector_complex_set_basis(v, NUM2INT(ii));

  return obj;
}

static VALUE Vector_complex_set_basis2(VALUE obj, VALUE ii) {
  gsl_vector_complex * v, * w;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  w = gsl_vector_complex_alloc(v->size);

  gsl_vector_complex_set_basis(w, NUM2INT(ii));

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, w);
}

/* Copying vectors */

/* module function */
static VALUE Vector_complex_memcpy(VALUE obj, VALUE dest, VALUE src) {
  gsl_vector_complex * vdest, * vsrc;

  Data_Get_Struct(dest, gsl_vector_complex, vdest);
  Data_Get_Struct(src, gsl_vector_complex, vsrc);

  gsl_vector_complex_memcpy(vdest, vsrc);

  return obj;
}

/* method */
static VALUE Vector_complex_clone(VALUE obj) {
  gsl_vector_complex * v, * new;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  new = gsl_vector_complex_alloc(v->size);

  gsl_vector_complex_memcpy(new, v);

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
}

/* module function */
static VALUE Vector_complex_swap(VALUE obj, VALUE vv, VALUE ww) {
  gsl_vector_complex * v, * w;

  Data_Get_Struct(vv, gsl_vector_complex, v);
  Data_Get_Struct(ww, gsl_vector_complex, w);

  gsl_vector_complex_swap(w, v);

  return obj;
}

static VALUE Vector_complex_swap_elements(VALUE obj, VALUE i, VALUE j) {
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  gsl_vector_complex_swap_elements(v, NUM2INT(i), NUM2INT(j));

  return obj;
}

static VALUE Vector_complex_reverse(VALUE obj) {
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  gsl_vector_complex_reverse(v);

  return obj;
}

/* Vector_complex operations */

static VALUE Vector_complex_add(VALUE obj, VALUE b) {
  gsl_vector_complex * v, * w, * tmp;
  gsl_vector_view rw, iw, rtmp, itmp;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  Data_Get_Struct(b, gsl_vector_complex, w);
  tmp = gsl_vector_complex_alloc(v->size);
  gsl_vector_complex_memcpy(tmp, v);
  rw = gsl_vector_complex_real(w);
  iw = gsl_vector_complex_imag(w);
  rtmp = gsl_vector_complex_real(tmp);
  itmp = gsl_vector_complex_imag(tmp);
  gsl_vector_add(&rtmp.vector, &rw.vector);
  gsl_vector_add(&itmp.vector, &iw.vector);
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, tmp);
}

static VALUE Vector_complex_add2(VALUE obj, VALUE b) {
  gsl_vector_complex * v, * w;
  gsl_vector_view rw, iw, rv, iv;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  Data_Get_Struct(b, gsl_vector_complex, w);
  rw = gsl_vector_complex_real(w);
  iw = gsl_vector_complex_imag(w);
  rv = gsl_vector_complex_real(v);
  iv = gsl_vector_complex_imag(v);
  gsl_vector_add(&rv.vector, &rw.vector);
  gsl_vector_add(&iv.vector, &iw.vector);
  return obj;
}

static VALUE Vector_complex_sub(VALUE obj, VALUE b) {
  gsl_vector_complex * v, * w, * tmp;
  gsl_vector_view rw, iw, rtmp, itmp;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  Data_Get_Struct(b, gsl_vector_complex, w);
  tmp = gsl_vector_complex_alloc(v->size);
  gsl_vector_complex_memcpy(tmp, v);
  rw = gsl_vector_complex_real(w);
  iw = gsl_vector_complex_imag(w);
  rtmp = gsl_vector_complex_real(tmp);
  itmp = gsl_vector_complex_imag(tmp);
  gsl_vector_sub(&rtmp.vector, &rw.vector);
  gsl_vector_sub(&itmp.vector, &iw.vector);
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, tmp);
}

static VALUE Vector_complex_sub2(VALUE obj, VALUE b) {
  gsl_vector_complex * v, * w;
  gsl_vector_view rw, iw, rv, iv;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  Data_Get_Struct(b, gsl_vector_complex, w);
  rw = gsl_vector_complex_real(w);
  iw = gsl_vector_complex_imag(w);
  rv = gsl_vector_complex_real(v);
  iv = gsl_vector_complex_imag(v);
  gsl_vector_sub(&rv.vector, &rw.vector);
  gsl_vector_sub(&iv.vector, &iw.vector);
  return obj;
}

static VALUE Vector_complex_scale(VALUE obj, VALUE vz)
{
  gsl_vector_complex * v, * new;
  gsl_complex * pz;
  gsl_complex w;
  int n, i;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  Data_Get_Struct(vz, gsl_complex, pz);
  n = v->size;
  new = gsl_vector_complex_alloc(n);
  for(i = 0; i < n; i++) {
    w = gsl_complex_mul(gsl_vector_complex_get(v,i), *pz);
    gsl_vector_complex_set(new, i, w);
  }
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, new);
}

static VALUE Vector_complex_scale2(VALUE obj, VALUE vz)
{
  gsl_vector_complex * v;
  gsl_complex * pz;
  gsl_complex w;
  int n, i;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  Data_Get_Struct(vz, gsl_complex, pz);
  n = v->size;
  for(i = 0; i < n; i++) {
    w = gsl_complex_mul(gsl_vector_complex_get(v,i), *pz);
    gsl_vector_complex_set(v, i, w);
  }
  return obj;
}
/* Vector properties */

static VALUE Vector_complex_isnull(VALUE obj) {
  gsl_vector_complex * v;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  return INT2FIX(gsl_vector_complex_isnull(v));
}

/* Reading and writing vectors */
static VALUE Vector_complex_fwrite(VALUE obj, VALUE io) {
  gsl_vector_complex * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_vector_complex_fwrite(f, v);
  return INT2FIX(status);
}

static VALUE Vector_complex_fread(VALUE obj, VALUE io) {
  gsl_vector_complex * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_vector_complex_fread(f, v);
  return INT2FIX(status);
}

static VALUE Vector_complex_fprintf(VALUE obj, VALUE io, VALUE format) {
  gsl_vector_complex * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  f = GetWriteFile(fptr);
  status = gsl_vector_complex_fprintf(f, v, STR2CSTR(format));
  return INT2FIX(status);
}

static VALUE Vector_complex_fscanf(VALUE obj, VALUE io) {
  gsl_vector_complex * v;
  OpenFile * fptr;
  FILE * f;
  int status;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = fptr->f;
  status = gsl_vector_complex_fscanf(f, v);
  return INT2FIX(status);
}

static VALUE Vector_complex_printf(VALUE obj, VALUE format) {
  gsl_vector_complex * v;
  int status;

  Data_Get_Struct(obj, gsl_vector_complex, v);
  status = gsl_vector_complex_fprintf(stdout, v, STR2CSTR(format));
  return INT2FIX(status);
}

static VALUE Vector_complex_print(VALUE obj) {
  gsl_vector_complex * v;
  size_t i, n;
  Data_Get_Struct(obj, gsl_vector_complex, v);
  n = v->size;
  printf("[");
  for (i = 0; i < n; i += 1){
   gsl_complex z;
   z = gsl_vector_complex_get(v, i);
   printf("(%g, %g)  ", GSL_REAL(z),GSL_IMAG(z));
  }
  printf("]\n");
  return obj;
}

static VALUE Vector_complex_each(VALUE obj) {
  gsl_vector_complex * v;
  size_t i;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  for (i = 0; i < v->size; i++) {
    gsl_complex z;
    z = gsl_vector_complex_get(v, i);
    rb_yield(Complex_new_intern(rbgsl_cComplex, &z));
  }

  return obj;
}

static VALUE Vector_complex_size(VALUE obj) {
  gsl_vector_complex * v;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  return INT2NUM(v->size);
}

static VALUE Vector_complex_inspect(VALUE obj) {
  Vector_complex_print(obj);
  return obj;
}

static VALUE Vector_complex_equal(VALUE self, VALUE bb) {
  gsl_vector_complex * a, * b;
  gsl_vector_view ra, ia, rb, ib;

  size_t i, na, nb;
  Data_Get_Struct(self, gsl_vector_complex, a);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  na = a->size;
  nb = b->size;
  if (na != nb)
    return Qfalse;

  ra = gsl_vector_complex_real(a);
  ia = gsl_vector_complex_imag(a);
  rb = gsl_vector_complex_real(b);
  ib = gsl_vector_complex_imag(b);
  for(i = 0; i < na; i++) {
    if(gsl_vector_get(&ra.vector,i) != gsl_vector_get(&rb.vector,i))
      return Qfalse;
    if(gsl_vector_get(&ia.vector,i) != gsl_vector_get(&ib.vector,i))
      return Qfalse;
  }
  return Qtrue;
}

static VALUE Vector_complex_float_equal(VALUE self, VALUE aa, VALUE bb, VALUE eps) {
  gsl_vector_complex * a, * b;
  gsl_vector_view ra, ia, rb, ib;
  double d1 = NUM2DBL(eps);
  size_t i, na, nb;
  Data_Get_Struct(aa, gsl_vector_complex, a);
  Data_Get_Struct(bb, gsl_vector_complex, b);
  na = a->size;
  nb = b->size;
  if (na != nb)
    return Qfalse;

  ra = gsl_vector_complex_real(a);
  ia = gsl_vector_complex_imag(a);
  rb = gsl_vector_complex_real(b);
  ib = gsl_vector_complex_imag(b);
  for(i = 0; i < na; i++) {
    if(fabs(gsl_vector_get(&ra.vector,i) - gsl_vector_get(&rb.vector,i)) >= d1)
      return Qfalse;
    if(fabs(gsl_vector_get(&ia.vector,i) - gsl_vector_get(&ib.vector,i)) >= d1)
      return Qfalse;
  }
  return Qtrue;
}

static VALUE Vector_to_complex(VALUE obj)
{
  int i, n;
  gsl_vector * a;
  gsl_vector_complex * b;
  Data_Get_Struct(obj, gsl_vector, a);
  n = a->size;
  b = gsl_vector_complex_alloc(a->size);
  for(i=0; i < n; i++) {
      gsl_complex z;
      GSL_SET_COMPLEX (&z, gsl_vector_get(a,i), 0);
      gsl_vector_complex_set(b,i,z);
    }
  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, b);
}

/* Vector views */

static VALUE Vector_complex_subvector(VALUE obj, VALUE o, VALUE n) {
  gsl_vector_complex * v, * w;
  gsl_vector_complex_view vtmp;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  w = gsl_vector_complex_alloc(NUM2INT(n));

  vtmp = gsl_vector_complex_subvector(v, NUM2INT(o), NUM2INT(n));
  *w = vtmp.vector;

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, w);
}

static VALUE Vector_complex_subvector_with_stride(VALUE obj, VALUE o, VALUE s, VALUE n) {
  gsl_vector_complex * v, * w;
  gsl_vector_complex_view vtmp;

  Data_Get_Struct(obj, gsl_vector_complex, v);

  w = gsl_vector_complex_alloc(NUM2INT(n));

  vtmp = gsl_vector_complex_subvector_with_stride(v, NUM2INT(o), NUM2INT(s), NUM2INT(n));
  *w = vtmp.vector;

  return Data_Wrap_Struct(rbgsl_cVector_complex, 0, gsl_vector_complex_free, w);
}

static VALUE Vector_complex_real(VALUE obj)
{
  gsl_vector_complex * a;
  gsl_vector * b;
  gsl_vector_view w;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  b = gsl_vector_alloc(a->size);
  w = gsl_vector_complex_real(a);
  gsl_vector_memcpy(b, &w.vector);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, b);
}

static VALUE Vector_complex_imag(VALUE obj)
{
  gsl_vector_complex * a;
  gsl_vector * b;
  gsl_vector_view w;
  Data_Get_Struct(obj, gsl_vector_complex, a);
  b = gsl_vector_alloc(a->size);
  w = gsl_vector_complex_imag(a);
  gsl_vector_memcpy(b, &w.vector);
  return Data_Wrap_Struct(rbgsl_cVector, 0, gsl_vector_free, b);
}

/* Must allocate a new generic instance without any parameters
 * Since we don't know how big the object will be later, just allocate
 * a minimum-sized placeholder
 */
#ifdef HAVE_DEFINE_ALLOC_FUNCTION
static VALUE Vector_complex_allocate(VALUE kl) {
  return Vector_complex_calloc(kl, INT2NUM(1));
}
#endif

/* Class definition */

void Init_Vector_complex() {
  rbgsl_cVector_complex = rb_define_class_under(rbgsl_mGSL, "Vector_complex", rb_cObject);

  rb_define_singleton_method(rbgsl_cVector_complex, "new", Vector_complex_new, 1);
  rb_define_singleton_method(rbgsl_cVector_complex, "new2", Vector_complex_new2, -1);

  rb_define_singleton_method(rbgsl_cVector_complex, "alloc", Vector_complex_new, 1);
  rb_define_method(rbgsl_cVector_complex, "size", Vector_complex_size, 0);
/*
  rb_define_method(rbgsl_cVector_complex, "stride", Vector_complex_stride, 0);
  rb_define_method(rbgsl_cVector_complex, "owner", Vector_complex_owner, 0);
  */
  rb_define_method(rbgsl_cVector_complex, "get", Vector_complex_get, 1);
  /*
  rb_define_method(rbgsl_cVector_complex, "set", Vector_complex_set, -1);
  */
  rb_define_method(rbgsl_cVector_complex, "real", Vector_complex_real, 0);
  rb_define_method(rbgsl_cVector_complex, "imag", Vector_complex_imag, 0);

  rb_define_method(rbgsl_cVector_complex, "set1", Vector_complex_set1, 2);
  rb_define_alias(rbgsl_cVector_complex, "[]=", "set1");
  rb_define_alias(rbgsl_cVector_complex, "[]", "get");
  rb_define_method(rbgsl_cVector_complex, "set_all", Vector_complex_set_all, 1);
  rb_define_method(rbgsl_cVector_complex, "set_zero", Vector_complex_set_zero, 0);
  rb_define_method(rbgsl_cVector_complex, "set_basis!", Vector_complex_set_basis, 1);
  rb_define_method(rbgsl_cVector_complex, "set_basis", Vector_complex_set_basis2, 1);
  rb_define_method(rbgsl_cVector, "to_complex", Vector_to_complex, 0);
  rb_define_method(rbgsl_cVector_complex, "subvector", Vector_complex_subvector, 2);
  rb_define_method(rbgsl_cVector_complex, "subvector_with_stride", Vector_complex_subvector_with_stride, 3);

  rb_define_module_function(rbgsl_cVector_complex, "memcpy", Vector_complex_memcpy, 2);
  rb_define_module_function(rbgsl_cVector_complex, "swap", Vector_complex_swap, 2);
  rb_define_method(rbgsl_cVector_complex, "clone", Vector_complex_clone, 0);
  rb_define_method(rbgsl_cVector_complex, "swap_elements", Vector_complex_swap_elements, 2);
  rb_define_method(rbgsl_cVector_complex, "reverse!", Vector_complex_reverse, 0);
  rb_define_method(rbgsl_cVector_complex, "add", Vector_complex_add, 1);
  rb_define_alias(rbgsl_cVector_complex, "+", "add");
  rb_define_method(rbgsl_cVector_complex, "add!", Vector_complex_add2, 1);
  rb_define_method(rbgsl_cVector_complex, "sub", Vector_complex_sub, 1);
  rb_define_alias(rbgsl_cVector_complex, "-", "sub");
  rb_define_method(rbgsl_cVector_complex, "sub!", Vector_complex_sub2, 1);
  rb_define_method(rbgsl_cVector_complex, "scale", Vector_complex_scale, 1);
  rb_define_method(rbgsl_cVector_complex, "scale!", Vector_complex_scale2, 1);
  /*
  rb_define_method(rbgsl_cVector_complex, "add_const", Vector_complex_add_const, 1);
  rb_define_method(rbgsl_cVector_complex, "add_const!", Vector_complex_add_const2, 1);
  */
  rb_define_method(rbgsl_cVector_complex, "isnull", Vector_complex_isnull, 0);

  rb_define_method(rbgsl_cVector_complex, "fwrite", Vector_complex_fwrite, 1);
  rb_define_method(rbgsl_cVector_complex, "fread", Vector_complex_fread, 1);
  rb_define_method(rbgsl_cVector_complex, "fprintf", Vector_complex_fprintf, 2);

  rb_define_method(rbgsl_cVector_complex, "printf", Vector_complex_printf, 1);
  rb_define_method(rbgsl_cVector_complex, "fscanf", Vector_complex_fscanf, 1);
  /*
  rb_define_method(rbgsl_cVector_complex, "to_a", Vector_complex_to_a, 0);
  */
  rb_define_method(rbgsl_cVector_complex, "each", Vector_complex_each, 0);

  /*
  rb_define_method(rb_cArray, "to_v", rb_ary_to_v, 0);
  rb_define_alias(rb_cArray, "to_gv", "to_v");
  */
  rb_define_method(rbgsl_cVector_complex, "print", Vector_complex_print, 0);
  rb_define_method(rbgsl_cVector_complex, "inspect", Vector_complex_inspect, 0);
  rb_define_method(rbgsl_cVector_complex, "equal", Vector_complex_equal, 1);
  rb_define_alias(rbgsl_cVector_complex, "==", "equal");
  rb_define_singleton_method(rbgsl_cVector_complex, "float_equal", Vector_complex_float_equal, 3);
#ifdef HAVE_DEFINE_ALLOC_FUNCTION
  rb_define_alloc_func(rbgsl_cVector_complex, Vector_complex_allocate);
#endif

  /* These must be called _dump_data to signify that they are the special
   * functions to be called implicitly from the Marshal engine.
   * The word data refers to the fact that they are really C struct types
   * created using the alloc func and Data_Get_Struct
   */
  /*
  rb_define_method(rbgsl_cVector_complex, "_dump_data", Vector_complex_dump_data, 0);
  rb_define_method(rbgsl_cVector_complex, "_load_data", Vector_complex_load_data, 1);
*/
}
/* vim: set ts=4: */
