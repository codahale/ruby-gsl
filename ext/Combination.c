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

/* $Id: Combination.c,v 1.6 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_combination.h"
#include "ruby-gsl.h"

/* Combinations */

static VALUE Combination_initialize(VALUE self, VALUE n, VALUE k) {
  return self;
}

static void Combination_free(void * p) {
  gsl_combination_free(p);
}

VALUE Combination_new(VALUE class, VALUE n, VALUE k) {
  VALUE argv[2];
  VALUE comb;
  gsl_combination * ptr;
  ptr = gsl_combination_calloc(NUM2INT(n), NUM2INT(k));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Not enough memory for combination allocation");
  comb = Data_Wrap_Struct(class, 0, Combination_free, ptr);
  argv[0] = n;
  argv[1] = k;
  rb_obj_call_init(comb, 2, argv);
  return comb;
}

VALUE Combination_new_ary(VALUE class, VALUE x, VALUE y) {
  gsl_combination * invptr;
  VALUE inv;
  VALUE goodx;
  long xlen;
  long i;
  Check_Type(x, T_ARRAY);
  Check_Type(y, T_FIXNUM);
  xlen = RARRAY(x)->len;
  inv = Combination_new(rbgsl_cCombination, y, INT2FIX(xlen));
  goodx = rb_ary_sort(x);
  Data_Get_Struct(inv, gsl_combination, invptr);
  for (i = 0; i < xlen ; i++)
    invptr->data[i] = NUM2INT(rb_ary_entry(goodx, i));
  /* commented out until gsl_combination_valid is fixed */
  /*
   * if (gsl_combination_valid(invptr)) // GSL_SUCCESS == 0
   *  rb_raise(rb_eStandardError, "ArgumentError in Combination_new_ary");
   */
  return inv;
}

/* Combination allocation */

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
static VALUE Combination_allocate(VALUE kl) {
  return Combination_new(kl, INT2NUM(1), INT2NUM(1));
}
#endif

static VALUE Combination_init_first(VALUE self) {
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  gsl_combination_init_first(ptr);
  return self;
}

static VALUE Combination_init_last(VALUE self) {
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  gsl_combination_init_last(ptr);
  return self;
}

/* Accessing combination elements */

static VALUE Combination_get(VALUE self, VALUE i) {
  size_t ret;
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  ret = gsl_combination_get(ptr, NUM2INT(i));
  return INT2FIX(ret);
}

/* Combination properties */

static VALUE Combination_n(VALUE self) {
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  return INT2FIX(ptr->n);
}

static VALUE Combination_k(VALUE self) {
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  return INT2FIX(ptr->k);
}

static VALUE Combination_data(VALUE self) {
  gsl_combination * ptr;
  size_t i;
  VALUE ary;
  Data_Get_Struct(self, gsl_combination, ptr);
  ary = rb_ary_new2(ptr->k);
  for (i = 0; i < ptr->k; i++)
    RARRAY(ary)->ptr[i] = INT2FIX(ptr->data[i]);
  RARRAY(ary)->len = ptr->k;
  return ary;
}

/* To fix this, change near line 88 of combination/combination.c in GSL 1.3
 * the if (c->data[i] > c->data[j]) should be if (c->data[i] < c->data[j]) 
 */
static VALUE Combination_valid(VALUE self) {
  gsl_combination * ptr;
  int gretval;
  VALUE retval;
  Data_Get_Struct(self, gsl_combination, ptr);
  gretval = gsl_combination_valid(ptr);
  retval = (gretval == GSL_SUCCESS) ? Qtrue : Qfalse;
  return retval;
}

static VALUE Combination_load_data(VALUE self, VALUE ary) {
  gsl_combination * ptr, * newguy, tempspace;
  int i, k, n;
  n = NUM2INT(rb_ary_entry(ary, 0));
  k = NUM2INT(rb_ary_entry(ary, 1));
  Data_Get_Struct(self, gsl_combination, ptr);
  newguy = gsl_combination_alloc(n, k);
  for (i = 0; i < k; ++i)
    newguy->data[i] = NUM2INT(rb_ary_entry(ary, 2+i));
  memcpy(&tempspace, newguy, sizeof(gsl_combination));
  memcpy(newguy, ptr, sizeof(gsl_combination));
  memcpy(ptr, &tempspace, sizeof(gsl_combination));
  gsl_combination_free(newguy);
  return Qtrue;
}

static VALUE Combination_dump_data(VALUE self) {
  VALUE ary = rb_ary_new();
  size_t i;
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  rb_ary_push(ary, INT2NUM(ptr->n));
  rb_ary_push(ary, INT2NUM(ptr->k));
  for (i = 0; i < ptr->k; ++i)
    rb_ary_push(ary, INT2NUM(ptr->data[i]));
  return ary;
}

static VALUE Combination_eq(VALUE self, VALUE rother) {
  gsl_combination * ptr, * other;
  size_t i;
  Data_Get_Struct(self, gsl_combination, ptr);
  Data_Get_Struct(rother, gsl_combination, other);
  if (ptr->n != other->n)
    return Qnil;
  if (ptr->k != other->k)
    return Qnil;
  for (i = 0; i < ptr->k; ++i)
    if (ptr->data[i] != other->data[i])
      return Qnil;
  return Qtrue;
}

/* Combination functions */

static VALUE Combination_next(VALUE self) {
  gsl_combination * ptr;
  int ret;
  Data_Get_Struct(self, gsl_combination, ptr);
  ret = gsl_combination_next(ptr);
  if (ret == GSL_FAILURE)
    rb_raise(rb_eStandardError, "No next combination.");
  else if (ret != GSL_SUCCESS)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Combination_prev(VALUE self) {
  gsl_combination * ptr;
  int ret;
  Data_Get_Struct(self, gsl_combination, ptr);
  ret = gsl_combination_prev(ptr);
  if (ret == GSL_FAILURE)
    rb_raise(rb_eStandardError, "No previous combination.");
  else if (ret != GSL_SUCCESS)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Combination_each(VALUE self) {
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  do {
    rb_yield(self);
  } while (gsl_combination_next(ptr) == GSL_SUCCESS);
  return self;
}

static VALUE Combination_reverse_each(VALUE self) {
  gsl_combination * ptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  do {
    rb_yield(self);
  } while (gsl_combination_prev(ptr) == GSL_SUCCESS);
  return self;
}

/* Reading and Writing combinations */

static VALUE Combination_fwrite(VALUE self, VALUE stream) {
  int ret;
  gsl_combination * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_combination_fwrite(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Combination_fread(VALUE self, VALUE stream) {
  int ret;
  gsl_combination * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_combination_fread(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Combination_fprintf(VALUE self, VALUE stream, VALUE format) {
  int ret;
  gsl_combination * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_combination_fprintf(fptr->f, ptr, STR2CSTR(format));
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Combination_fscanf(VALUE self, VALUE stream) {
  int ret;
  gsl_combination * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_combination, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_combination_fscanf(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

/* Class definition */

VALUE rbgsl_cCombination;

void Init_Combination() {
  rbgsl_cCombination = rb_define_class_under(rbgsl_mGSL, "Combination", rb_cObject);

  rb_define_singleton_method(rbgsl_cCombination, "new", Combination_new, 2);
  rb_define_singleton_method(rbgsl_cCombination, "new_ary", Combination_new_ary, 2);
  rb_define_method(rbgsl_cCombination, "initialize", Combination_initialize, 2);

  rb_define_method(rbgsl_cCombination, "init_first", Combination_init_first, 0);
  rb_define_method(rbgsl_cCombination, "init_last", Combination_init_last, 0);

  rb_define_method(rbgsl_cCombination, "get", Combination_get, 1);
  rb_define_alias(rbgsl_cCombination, "[]", "get");
  rb_define_method(rbgsl_cCombination, "n", Combination_n, 0);
  rb_define_method(rbgsl_cCombination, "k", Combination_k, 0);
  rb_define_method(rbgsl_cCombination, "data", Combination_data, 0);
  rb_define_alias(rbgsl_cCombination, "to_a", "data");
  rb_define_method(rbgsl_cCombination, "valid", Combination_valid, 0);
  rb_define_alias(rbgsl_cCombination, "is_valid?", "valid");
  rb_define_method(rbgsl_cCombination, "==", Combination_eq, 1);
  rb_define_method(rbgsl_cCombination, "_dump_data", Combination_dump_data, 0);
  rb_define_method(rbgsl_cCombination, "_load_data", Combination_load_data, 1);
#ifdef HAVE_DEFINE_ALLOC_FUNCTION
  rb_define_alloc_func(rbgsl_cCombination, Combination_allocate);
#endif

  rb_define_method(rbgsl_cCombination, "next", Combination_next, 0);
  rb_define_method(rbgsl_cCombination, "prev", Combination_prev, 0);
  /* TODO: These should become each! and reverse_each! once we
   * have gsl_combination_memcpy available */
  rb_define_method(rbgsl_cCombination, "each", Combination_each, 0);
  rb_define_method(rbgsl_cCombination, "reverse_each", Combination_reverse_each, 0);

  rb_define_method(rbgsl_cCombination, "fwrite", Combination_fwrite, 1);
  rb_define_method(rbgsl_cCombination, "fread", Combination_fread, 1);
  rb_define_method(rbgsl_cCombination, "fprintf", Combination_fprintf, 2);
  rb_define_method(rbgsl_cCombination, "fscanf", Combination_fscanf, 1);
}

/* vim: set ts=4: */
