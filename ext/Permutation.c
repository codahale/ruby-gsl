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

/* $Id: Permutation.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <stdio.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_permutation.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Permutations */

VALUE rbgsl_cPermutation;
VALUE rbgsl_cCanPermutation;

/* internal use only */

static int permutation_valid(gsl_permutation * p) {
   const size_t size = p->size;
   size_t i, j ;
   for (i = 0; i < size; i++) {
     if (p->data[i] >= size)
       rb_raise(rb_eRangeError, "Index is outside of allowed range.");
     for (j = 0; j < i; j++) {
       if (p->data[i] == p->data[j])
         rb_raise(rb_eStandardError, "Duplicate permutation index.");
     }
   }
   return 1;
}

/* Permutation Allocation */

static VALUE Permutation_init(VALUE self, VALUE n) {
  return self;
}

static void Permutation_free(void * p) {
  gsl_permutation_free(p);
}

VALUE Permutation_new(VALUE class, VALUE n) {
  VALUE argv[1];
  VALUE perm;
  gsl_permutation * ptr;
  ptr = gsl_permutation_calloc(NUM2INT(n));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for permutation allocation.");
  perm = Data_Wrap_Struct(class, 0, Permutation_free, ptr);
  argv[0] = n;
  rb_obj_call_init(perm, 1, argv);
  return perm;
}

VALUE CanPermutation_new(VALUE class, VALUE n) {
  VALUE argv[1];
  VALUE perm;
  int i, num = NUM2INT(n);
  gsl_permutation * ptr;
  ptr = gsl_permutation_alloc(num);
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for permutation allocation.");
  for (i = 0; i < num; i++)
    ptr->data[i] = num - 1 - i;
  perm = Data_Wrap_Struct(class, 0, Permutation_free, ptr);
  argv[0] = n;
  rb_obj_call_init(perm, 1, argv);
  return perm;
}

VALUE Permutation_new2(VALUE class, VALUE n) {
  VALUE argv[1];
  VALUE perm;
  gsl_permutation * ptr;
  ptr = gsl_permutation_alloc(NUM2INT(n));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for permutation allocation.");
  perm = Data_Wrap_Struct(class, 0, Permutation_free, ptr);
  argv[0] = n;
  rb_obj_call_init(perm, 1, argv);
  return perm;
}

VALUE Permutation_new_ary(VALUE class, VALUE x) {
 gsl_permutation * ptr;
 VALUE perm;
 size_t xlen;
 size_t i;
 Check_Type(x, T_ARRAY);
 xlen = RARRAY(x)->len;
 perm =  Permutation_new2(rbgsl_cPermutation, INT2FIX(xlen));
 Data_Get_Struct(perm, gsl_permutation, ptr);
 for (i = 0; i < xlen ; i++)
   ptr->data[i] = (RARRAY(x)->ptr[i])>>1;
 if (!permutation_valid(ptr))
   rb_raise(rb_eStandardError, "ArgumentError in Permutation_new_ary.");
 return perm;
}

static VALUE Permutation_reset(VALUE self) {
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  gsl_permutation_init(ptr);
  return self;
}

static VALUE CanPermutation_reset(VALUE self) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr, * invptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  invptr = gsl_permutation_alloc(ptr->size);
  gsl_permutation_canonical_to_linear(ptr, invptr);
  gsl_permutation_init(invptr);
  gsl_permutation_linear_to_canonical(ptr, invptr);
  gsl_permutation_free(invptr);
  return self;
}

/* Accessing Permutation Elements */

static VALUE Permutation_get(VALUE self, VALUE i) {
  size_t idx = NUM2INT(i);
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  if (idx < 0 || idx >= ptr->size)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  return INT2FIX(gsl_permutation_get(ptr, idx));
}

static VALUE Permutation_swap(VALUE self, VALUE i, VALUE j) {
  size_t ii = NUM2INT(i), ij = NUM2INT(j);
  gsl_permutation * ptr;
  int ret;
  Data_Get_Struct(self, gsl_permutation, ptr);
  if (ii < 0 || ii >= ptr->size)
    rb_raise(rb_eRangeError, "Index i is outside of allowed range.");
  if (ij < 0 || ij >= ptr->size)
    rb_raise(rb_eRangeError, "Index j is outside of allowed range.");
  ret = gsl_permutation_swap(ptr, ii, ij);
  if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_memcpy(VALUE self, VALUE dest, VALUE src) {
  int ret;
  gsl_permutation * pdest, * psrc;
  Data_Get_Struct(dest, gsl_permutation, pdest);
  Data_Get_Struct(src, gsl_permutation, psrc);
  ret = gsl_permutation_memcpy(pdest, psrc);
  return INT2FIX(ret);
}

/* Permutation Properties */

static VALUE Permutation_size(VALUE self) {
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  return INT2FIX(gsl_permutation_size(ptr));
}

static VALUE Permutation_valid(VALUE self) {
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  return (gsl_permutation_valid(ptr) ? Qfalse : Qtrue);
}

static VALUE Permutation_cycles(VALUE self) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  return INT2FIX(gsl_permutation_linear_cycles(ptr));
  return self;
}

static VALUE CanPermutation_cycles(VALUE self) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  return INT2FIX(gsl_permutation_canonical_cycles(ptr));
  return self;
}

VALUE Permutation_data(VALUE self) {
  gsl_permutation * ptr;
  size_t i;
  VALUE ary;
  Data_Get_Struct(self, gsl_permutation, ptr);
  ary = rb_ary_new2(ptr->size);
  for (i = 0; i < ptr->size; i++)
    RARRAY(ary)->ptr[i] = INT2FIX(ptr->data[i]);
  RARRAY(ary)->len = ptr->size;
  return ary;
}

/* Permutation Functions */

static VALUE Permutation_next(VALUE self) {
  int ret;
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  ret = gsl_permutation_next(ptr);
  if (ret == GSL_FAILURE)
    rb_raise(rb_eStandardError, "No next permutation.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_prev(VALUE self) {
  int ret;
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  ret = gsl_permutation_prev(ptr);
  if (ret == GSL_FAILURE)
    rb_raise(rb_eStandardError, "No previous permutation.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_each(VALUE self) {
  int ret = GSL_SUCCESS;
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  while (!ret) {
    rb_yield(self);
    ret = gsl_permutation_next(ptr);
  }
  return self;
}

/* non-mutating each seems to work; UNDERSTAND WHY */
static VALUE Permutation_nonmutating_each(VALUE self) {
  int ret = GSL_SUCCESS;
  VALUE inv;
  gsl_permutation * ptr, * invptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  inv = Permutation_new(rbgsl_cPermutation, INT2FIX(ptr->size));
  Data_Get_Struct(inv, gsl_permutation, invptr);
  gsl_permutation_memcpy(invptr, ptr);
  while (!ret) {
    rb_yield(inv);
    ret = gsl_permutation_next(invptr);
  }
  return inv;
}

static VALUE Permutation_reverse(VALUE self) {
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  gsl_permutation_reverse(ptr);
  return self;
}

static VALUE CanPermutation_reverse(VALUE self) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr, * invptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  invptr = gsl_permutation_alloc(ptr->size);
  gsl_permutation_canonical_to_linear(ptr, invptr);
  gsl_permutation_reverse(invptr);
  gsl_permutation_linear_to_canonical(ptr, invptr);
  gsl_permutation_free(invptr);
  return self;
}

/* FIXME: this seems to be broken */
static VALUE Permutation_inverse(VALUE self) {
  gsl_permutation * ptr, * invptr;
  VALUE inv;
  int ret;
  Data_Get_Struct(self, gsl_permutation, ptr);
  inv = Permutation_new(rbgsl_cPermutation, INT2FIX(ptr->size));
  Data_Get_Struct(inv, gsl_permutation, invptr);
  ret = gsl_permutation_inverse(invptr, ptr);
  return inv;
}

static VALUE Permutation_to_canonical(VALUE self) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr, * invptr;
  VALUE inv;
  Data_Get_Struct(self, gsl_permutation, ptr);
  inv = Permutation_new(rbgsl_cCanPermutation, INT2FIX(ptr->size));
  Data_Get_Struct(inv, gsl_permutation, invptr);
  gsl_permutation_linear_to_canonical(invptr, ptr);
  return inv;
}

static VALUE CanPermutation_to_canonical(VALUE self) {
 return self;
}

static VALUE Permutation_to_linear(VALUE self) {
   return self;
}

static VALUE CanPermutation_to_linear(VALUE self) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr, * invptr;
  VALUE inv;
  Data_Get_Struct(self, gsl_permutation, ptr);
  inv = Permutation_new(rbgsl_cPermutation, INT2FIX(ptr->size));
  Data_Get_Struct(inv, gsl_permutation, invptr);
  gsl_permutation_canonical_to_linear(ptr, invptr);
  return inv;
}

static VALUE Permutation_equal(VALUE self, VALUE p2) {
  gsl_permutation * ptr1, * ptr2;
  size_t self_len, i;
  if (rb_obj_class(self) != rb_obj_class(p2))
    p2 = CanPermutation_to_linear(p2);
  Data_Get_Struct(self, gsl_permutation, ptr1);
  Data_Get_Struct(p2, gsl_permutation, ptr2);
  if (self == p2)
    return Qtrue;
  self_len = ptr1->size;
  if (INT2FIX(ptr1->size) != INT2FIX(ptr2->size))
    return Qfalse;
  for (i = 0; i < self_len; i++)
   if (ptr1->data[i] != ptr2->data[i])
     return Qfalse;
  return Qtrue;
}

static VALUE CanPermutation_equal(VALUE self, VALUE p2) {
  gsl_permutation * ptr1, * ptr2;
  size_t self_len, i;
  if (rb_obj_class(self) != rb_obj_class(p2))
   self = CanPermutation_to_linear(self);
  Data_Get_Struct(self, gsl_permutation, ptr1);
  Data_Get_Struct(p2, gsl_permutation, ptr2);
  if (self == p2)
    return Qtrue;
  self_len = ptr1->size;
  if (INT2FIX(ptr1->size) != INT2FIX(ptr2->size))
    return Qfalse;
  for (i = 0; i < self_len; i++)
    if (ptr1->data[i] != ptr2->data[i])
      return Qfalse;
   return Qtrue;
}

/* TODO: Applying Permutations */

static VALUE Permutation_mul(VALUE self, VALUE p1) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr, * ptr1, * p_mul;
  VALUE mul;
  if (rb_obj_class(self) != rb_obj_class(p1))
    p1 = CanPermutation_to_linear(p1);
  Data_Get_Struct(self, gsl_permutation, ptr);
  Data_Get_Struct(p1, gsl_permutation, ptr1);
  mul = Permutation_new(rbgsl_cPermutation, INT2FIX(ptr->size));
  Data_Get_Struct(mul, gsl_permutation, p_mul);
  gsl_permutation_mul(p_mul, ptr, ptr1);
  return mul;
}

static VALUE CanPermutation_mul(VALUE self, VALUE p1) {
  /* until missing functions implemented in GSL */
  /* rb_notimplement(); */
  gsl_permutation * ptr, * ptr1, * p_mul;
  VALUE mul;
  /* convert cycle to perm */
  if (rb_obj_class(self) == rb_obj_class(p1))
    p1 = CanPermutation_to_linear(p1);
  self = CanPermutation_to_linear(self);
  Data_Get_Struct(self, gsl_permutation, ptr);
  Data_Get_Struct(p1, gsl_permutation, ptr1);
  mul = Permutation_new(rbgsl_cPermutation, INT2FIX(ptr->size));
  Data_Get_Struct(mul, gsl_permutation, p_mul);
  gsl_permutation_mul(p_mul, ptr, ptr1);
  return mul;
}

/* Reading and Writing Permutations */

static VALUE Permutation_fwrite(VALUE self, VALUE stream) {
  int ret;
  gsl_permutation * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_permutation_fwrite(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_fread(VALUE self, VALUE stream) {
  int ret;
  gsl_permutation * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_permutation_fread(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_fprintf(VALUE self, VALUE stream, VALUE format) {
  int ret;
  gsl_permutation * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_permutation_fprintf(fptr->f, ptr, STR2CSTR(format));
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_fscanf(VALUE self, VALUE stream) {
  int ret;
  gsl_permutation * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_permutation_fscanf(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_dump(VALUE self) {
  int ret;
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  ret = gsl_permutation_fprintf(stdout, ptr, " %u");
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Permutation_undump(VALUE self) {
  int ret;
  gsl_permutation * ptr;
  Data_Get_Struct(self, gsl_permutation, ptr);
  ret = gsl_permutation_fscanf(stdin, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

/* Class definition */

void Init_Permutation() {
  rbgsl_cPermutation = rb_define_class_under(rbgsl_mGSL, "Permutation", rb_cObject);
  rbgsl_cCanPermutation = rb_define_class_under(rbgsl_mGSL, "CanPermutation", rbgsl_cPermutation);

  rb_define_singleton_method(rbgsl_cPermutation, "new", Permutation_new, 1);
  rb_define_singleton_method(rbgsl_cPermutation, "new2", Permutation_new2, 1);
  rb_define_singleton_method(rbgsl_cPermutation, "new_ary", Permutation_new_ary, 1);
  rb_define_method(rbgsl_cPermutation, "initialize", Permutation_init, 1);
  rb_define_method(rbgsl_cPermutation, "reset", Permutation_reset, 0);
  rb_define_module_function(rbgsl_cPermutation, "memcpy", Permutation_memcpy, 2);

  rb_define_method(rbgsl_cPermutation, "size", Permutation_size, 0);
  rb_define_method(rbgsl_cPermutation, "valid", Permutation_valid, 0);
  rb_define_alias(rbgsl_cPermutation, "is_valid?", "valid");
  rb_define_method(rbgsl_cPermutation, "cycles", Permutation_cycles, 0);

  rb_define_method(rbgsl_cPermutation, "equal", Permutation_equal, 1);
  rb_define_alias(rbgsl_cPermutation, "==", "equal");
  rb_define_method(rbgsl_cPermutation, "get", Permutation_get, 1);
  rb_define_alias(rbgsl_cPermutation, "[]", "get");
  rb_define_method(rbgsl_cPermutation, "data", Permutation_data, 0);
  rb_define_alias(rbgsl_cPermutation, "to_a", "data");
  rb_define_method(rbgsl_cPermutation, "swap", Permutation_swap, 2);

  rb_define_method(rbgsl_cPermutation, "reverse!", Permutation_reverse, 0);
  rb_define_method(rbgsl_cPermutation, "inverse", Permutation_inverse, 0);
  rb_define_method(rbgsl_cPermutation, "next", Permutation_next, 0);
  rb_define_method(rbgsl_cPermutation, "prev", Permutation_prev, 0);
  rb_define_method(rbgsl_cPermutation, "each!", Permutation_each, 0);
  rb_define_method(rbgsl_cPermutation, "each", Permutation_nonmutating_each, 0);

  rb_define_method(rbgsl_cPermutation, "mul", Permutation_mul, 1);
  rb_define_alias(rbgsl_cPermutation, "*", "mul");

  rb_define_method(rbgsl_cPermutation, "fwrite", Permutation_fwrite, 1);
  rb_define_method(rbgsl_cPermutation, "fread", Permutation_fread, 1);
  rb_define_method(rbgsl_cPermutation, "fprintf", Permutation_fprintf, 2);
  rb_define_method(rbgsl_cPermutation, "fscanf", Permutation_fscanf, 1);
  rb_define_method(rbgsl_cPermutation, "dump", Permutation_dump, 0);
  rb_define_method(rbgsl_cPermutation, "undump", Permutation_undump, 0);

  rb_define_singleton_method(rbgsl_cCanPermutation, "new", CanPermutation_new, 1);
  rb_define_method(rbgsl_cPermutation, "to_canonical", Permutation_to_canonical, 0);
  rb_define_method(rbgsl_cCanPermutation, "to_canonical", CanPermutation_to_canonical, 0);
  rb_define_method(rbgsl_cPermutation, "to_linear", Permutation_to_linear, 0);
  rb_define_method(rbgsl_cCanPermutation, "to_linear", CanPermutation_to_linear, 0);
  rb_define_method(rbgsl_cCanPermutation, "equal", CanPermutation_equal, 1);
  rb_define_alias(rbgsl_cCanPermutation, "==", "equal");
  rb_define_method(rbgsl_cCanPermutation, "mul", CanPermutation_mul, 1);
  rb_define_alias(rbgsl_cCanPermutation, "*", "mul");
  rb_define_method(rbgsl_cCanPermutation, "reverse!", CanPermutation_reverse, 0);
  rb_define_method(rbgsl_cCanPermutation, "cycles", CanPermutation_cycles, 0);
  rb_define_method(rbgsl_cCanPermutation, "reset", CanPermutation_reset, 0);
}

/* vim: set ts=4: */
