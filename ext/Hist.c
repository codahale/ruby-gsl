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

/* $Id: Hist.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <stdio.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_histogram.h"
#include "gsl/gsl_errno.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Histograms */

VALUE rbgsl_cHist;
VALUE rbgsl_cHist_PDF;

/* Internal use only */

typedef size_t (* fun_t) (const gsl_histogram *);
typedef double (* fun_d_t) (const gsl_histogram *);
typedef int (* fun_h_t) (gsl_histogram *, const gsl_histogram *);
typedef int (* fun_h_d_t) (gsl_histogram *, double);

static VALUE call(fun_t fun, VALUE v) {
  gsl_histogram * ptr;
  Data_Get_Struct(v, gsl_histogram, ptr);
  return INT2FIX((*fun)(ptr));
}

static VALUE call_d(fun_d_t fun, VALUE v) {
  gsl_histogram * ptr;
  Data_Get_Struct(v, gsl_histogram, ptr);
  return rb_float_new((*fun)(ptr));
}

static VALUE call_h(fun_h_t fun, VALUE h1, VALUE h2) {
  gsl_histogram * ptr1, * ptr2;
  int ret;
  Data_Get_Struct(h1, gsl_histogram, ptr1);
  Data_Get_Struct(h2, gsl_histogram, ptr2);
  ret = (*fun)(ptr1, ptr2);
  if (ret)
    rb_raise(rb_eArgError, "Operation failed.");
  return h1;
}

static VALUE call_h_d(fun_h_d_t fun, VALUE h, VALUE d) {
  gsl_histogram * ptr;
  int ret;
  Data_Get_Struct(h, gsl_histogram, ptr);
  ret = (*fun)(ptr, NUM2DBL(d));
  if (ret)
    rb_raise(rb_eArgError, "Operation failed.");
  return h;
}

/* Histogram Allocation */

static VALUE Hist_init(VALUE self) {
  return self;
}

static void Hist_free(void * p) {
  gsl_histogram_free(p);
}

VALUE Hist_new(VALUE class, VALUE n) {
  VALUE h;
  gsl_histogram * ptr;
  ptr = gsl_histogram_calloc(NUM2INT(n));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  h = Data_Wrap_Struct(class, 0, Hist_free, ptr);
  return h;
}

VALUE Hist_new2(VALUE class, VALUE n, VALUE xmin, VALUE xmax) {
  VALUE h;
  gsl_histogram * ptr;
  if (NUM2INT(n) < 0)
    rb_raise(rb_eArgError, "Histogram size must be positive.");
  ptr = gsl_histogram_calloc_uniform(NUM2INT(n), NUM2DBL(xmin), NUM2DBL(xmax));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  h = Data_Wrap_Struct(class, 0, Hist_free, ptr);
  return h;
}

static VALUE Hist_set_range(VALUE self, VALUE n, VALUE d) {
  gsl_histogram * ptr;
  size_t i = NUM2INT(n);
  Data_Get_Struct(self, gsl_histogram, ptr);
  if (i < 0 || i > ptr->n)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  ptr->range[i] = NUM2DBL(d);
  return self;
}

static VALUE Hist_set_ranges_uniform(VALUE self, VALUE xmin, VALUE xmax) {
  gsl_histogram * ptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  return INT2FIX(gsl_histogram_set_ranges_uniform(ptr, NUM2DBL(xmin),
    NUM2DBL(xmax)));
}

static VALUE Hist_set_ranges(VALUE self, VALUE xrange) {
  gsl_histogram * ptr;
  double * x;
  Data_Get_Struct(self, gsl_histogram, ptr);
  COPYRUBYARRAY(xrange, x);
  return INT2FIX(gsl_histogram_set_ranges(ptr, x, RARRAY(xrange)->len));
}

/* Copying Histograms */

static VALUE Hist_clone(VALUE self) {
  VALUE clone;
  gsl_histogram * ptr, * cptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  cptr = gsl_histogram_clone(ptr);
  clone = Data_Wrap_Struct(rbgsl_cHist, 0, Hist_free, cptr);
  return clone;
}

static VALUE Hist_memcpy(VALUE self, VALUE dest, VALUE src) {
  int ret;
  gsl_histogram * pdest, * psrc;
  Data_Get_Struct(dest, gsl_histogram, pdest);
  Data_Get_Struct(src, gsl_histogram, psrc);
  ret = gsl_histogram_memcpy(pdest, psrc);
  return INT2FIX(ret);
}

/* Histogram Probability Distribution */

static VALUE Hist_PDF_initialize(VALUE self) {
  return self;
}

static void Hist_PDF_free(void * p) {
  gsl_histogram_pdf_free(p);
}

VALUE Hist_PDF_new(VALUE class, VALUE h) {
  VALUE pdf;
  gsl_histogram_pdf * ptr;
  gsl_histogram * hptr;
  Data_Get_Struct(h, gsl_histogram, hptr);
  ptr = gsl_histogram_pdf_alloc(hptr->n);
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  pdf = Data_Wrap_Struct(class, 0, Hist_PDF_free, ptr);
  gsl_histogram_pdf_init(ptr, hptr);
  return pdf;
}

static VALUE Hist_PDF_sample(VALUE self, VALUE r) {
  gsl_histogram_pdf * ptr;
  Data_Get_Struct(self, gsl_histogram_pdf, ptr);
  return rb_float_new(gsl_histogram_pdf_sample(ptr, NUM2DBL(r)));
}

/* Updating and Accessing Histogram Elements */

static VALUE Hist_increment(VALUE self, VALUE x) {
  gsl_histogram * ptr;
  int ret;
  Data_Get_Struct(self, gsl_histogram, ptr);
  ret = gsl_histogram_increment(ptr, NUM2DBL(x));
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Value does not belong to any bin.");
  return self;
}

static VALUE Hist_accumulate(VALUE self, VALUE x, VALUE w) {
  gsl_histogram * ptr;
  int ret;
  Data_Get_Struct(self, gsl_histogram, ptr);
  ret = gsl_histogram_accumulate(ptr, NUM2DBL(x), NUM2DBL(w));
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Value does not belong to any bin.");
  return self;
}

static VALUE Hist_get(VALUE self, VALUE i) {
  gsl_histogram * ptr;
  double val;
  size_t n = NUM2INT(i);
  Data_Get_Struct(self, gsl_histogram, ptr);
  if (n < 0 || n > ptr->n)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  val = gsl_histogram_get(ptr, n);
  return rb_float_new(val);
}

static VALUE Hist_each(VALUE self) {
  gsl_histogram * ptr;
  double val;
  Data_Get_Struct(self, gsl_histogram, ptr);
  if (rb_block_given_p()) {
    size_t n;
	for (n = 0; n < ptr->n; n++) {
      val = gsl_histogram_get(ptr, n);
      rb_yield(rb_ary_new3(2, INT2FIX(n), rb_float_new(val)));
    }
  }
  return self;
}

static VALUE Hist_each_bin(VALUE self) {
  gsl_histogram * ptr;
  double val;
  Data_Get_Struct(self, gsl_histogram, ptr);
  if (rb_block_given_p()) {
    size_t n;
	for (n = 0; n < ptr->n; n++) {
      val = gsl_histogram_get(ptr, n);
      rb_yield(rb_float_new(val));
    }
  }
  return self;
}

static VALUE Hist_get_range(VALUE self, VALUE i) {
  gsl_histogram * ptr;
  int ret;
  double lower, upper;
  VALUE ary;
  Data_Get_Struct(self, gsl_histogram, ptr);
  ret = gsl_histogram_get_range(ptr, NUM2INT(i), &lower, &upper);
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  ary = rb_ary_new3(2, rb_float_new(lower), rb_float_new(upper));
  return ary;
}

static VALUE Hist_max(VALUE self) {
  return call_d(gsl_histogram_max, self);
}

static VALUE Hist_min(VALUE self) {
  return call_d(gsl_histogram_min, self);
}

static VALUE Hist_bins(VALUE self) {
  return call(gsl_histogram_bins, self);
}

static VALUE Hist_reset(VALUE self) {
  gsl_histogram * ptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  gsl_histogram_reset(ptr);
  return self;
}

/* Searching Histograms */

static VALUE Hist_find(VALUE self, VALUE x) {
  gsl_histogram * ptr;
  int ret;
  size_t i;
  Data_Get_Struct(self, gsl_histogram, ptr);
  ret = gsl_histogram_find(ptr, NUM2DBL(x), &i);
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Value does not belong to any bin.");
  return INT2FIX(i);
}

/* Histogram Statistics */

static VALUE Hist_max_val(VALUE self) {
  return call_d(gsl_histogram_max_val, self);
}

static VALUE Hist_max_bin(VALUE self) {
  return call(gsl_histogram_max_bin, self);
}

static VALUE Hist_min_val(VALUE self) {
  return call_d(gsl_histogram_min_val, self);
}

static VALUE Hist_min_bin(VALUE self) {
  return call(gsl_histogram_min_bin, self);
}

static VALUE Hist_mean(VALUE self) {
  return call_d(gsl_histogram_mean, self);
}

static VALUE Hist_sigma(VALUE self) {
  return call_d(gsl_histogram_sigma, self);
}

static VALUE Hist_sum(VALUE self) {
  return call_d(gsl_histogram_sum, self);
}

/* Histogram Operations */

static VALUE Hist_equal_bins_p(VALUE self, VALUE other) {
  gsl_histogram * ptr1, * ptr2;
  int ret;
  Data_Get_Struct(self, gsl_histogram, ptr1);
  Data_Get_Struct(other, gsl_histogram, ptr2);
  ret = gsl_histogram_equal_bins_p(ptr1, ptr2);
  if (ret)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE Hist_add(VALUE self, VALUE other) {
  return call_h(gsl_histogram_add, self, other);
}

static VALUE Hist_sub(VALUE self, VALUE other) {
  return call_h(gsl_histogram_sub, self, other);
}

static VALUE Hist_mul(VALUE self, VALUE other) {
  return call_h(gsl_histogram_mul, self, other);
}

static VALUE Hist_div(VALUE self, VALUE other) {
  return call_h(gsl_histogram_div, self, other);
}

static VALUE Hist_scale(VALUE self, VALUE d) {
  return call_h_d(gsl_histogram_scale, self, d);
}

static VALUE Hist_shift(VALUE self, VALUE d) {
  return call_h_d(gsl_histogram_shift, self, d);
}

/* Reading and Writing Histograms */

static VALUE Hist_fwrite(VALUE self, VALUE stream) {
  int ret;
  gsl_histogram * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_histogram_fwrite(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist_fread(VALUE self, VALUE stream) {
  int ret;
  gsl_histogram * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_histogram_fread(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist_fprintf(VALUE self, VALUE stream, VALUE range_format,
  VALUE bin_format) {
  int ret;
  gsl_histogram * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_histogram_fprintf(fptr->f, ptr, STR2CSTR(range_format),
    STR2CSTR(bin_format));
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist_dump(VALUE self) {
  int ret;
  gsl_histogram * ptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  ret = gsl_histogram_fprintf(stdout, ptr, "%g", "%g");
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist_fscanf(VALUE self, VALUE stream) {
  int ret;
  gsl_histogram * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_histogram_fscanf(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

/* Class definition */

void Init_Hist() {
  rbgsl_cHist = rb_define_class_under(rbgsl_mHistogram, "Hist", rb_cObject);
  rb_define_singleton_method(rbgsl_cHist, "new", Hist_new, 1);
  rb_define_singleton_method(rbgsl_cHist, "new2", Hist_new2, 3);
  rb_define_method(rbgsl_cHist, "initialize", Hist_init, 0);

  rb_define_method(rbgsl_cHist, "set_range", Hist_set_range, 2);
  rb_define_method(rbgsl_cHist, "set_ranges", Hist_set_ranges, 1);
  rb_define_method(rbgsl_cHist, "set_ranges_uniform", Hist_set_ranges_uniform, 2);

  rb_define_module_function(rbgsl_cHist, "memcpy", Hist_memcpy, 2);
  rb_define_method(rbgsl_cHist, "clone", Hist_clone, 0);

  rb_define_method(rbgsl_cHist, "increment", Hist_increment, 1);
  rb_define_alias(rbgsl_cHist, "inc", "increment");
  rb_define_method(rbgsl_cHist, "accumulate", Hist_accumulate, 2);
  rb_define_alias(rbgsl_cHist, "acc", "accumulate");
  rb_define_method(rbgsl_cHist, "get", Hist_get, 1);
  rb_define_alias(rbgsl_cHist, "[]", "get");
  rb_define_method(rbgsl_cHist, "get_range", Hist_get_range, 1);
  rb_define_method(rbgsl_cHist, "each", Hist_each, 0);
  rb_define_method(rbgsl_cHist, "each_bin", Hist_each_bin, 0);
  rb_define_method(rbgsl_cHist, "max", Hist_max, 0);
  rb_define_method(rbgsl_cHist, "min", Hist_min, 0);
  rb_define_method(rbgsl_cHist, "bins", Hist_bins, 0);
  rb_define_method(rbgsl_cHist, "reset", Hist_reset, 0);

  rb_define_method(rbgsl_cHist, "find", Hist_find, 1);

  rb_define_method(rbgsl_cHist, "max_val", Hist_max_val, 0);
  rb_define_method(rbgsl_cHist, "max_bin", Hist_max_bin, 0);
  rb_define_method(rbgsl_cHist, "min_val", Hist_min_val, 0);
  rb_define_method(rbgsl_cHist, "min_bin", Hist_min_bin, 0);
  rb_define_method(rbgsl_cHist, "mean", Hist_mean, 0);
  rb_define_method(rbgsl_cHist, "sigma", Hist_sigma, 0);
  rb_define_method(rbgsl_cHist, "sum", Hist_sum, 0);

  rb_define_method(rbgsl_cHist, "equal_bins_p", Hist_equal_bins_p, 1);
  rb_define_alias(rbgsl_cHist, "equal_bins?", "equal_bins_p");
  rb_define_method(rbgsl_cHist, "add", Hist_add, 1);
  rb_define_alias(rbgsl_cHist, "+", "add");
  rb_define_method(rbgsl_cHist, "sub", Hist_sub, 1);
  rb_define_alias(rbgsl_cHist, "-", "sub");
  rb_define_method(rbgsl_cHist, "mul", Hist_mul, 1);
  rb_define_alias(rbgsl_cHist, "*", "mul");
  rb_define_method(rbgsl_cHist, "div", Hist_div, 1);
  rb_define_alias(rbgsl_cHist, "/", "div");
  rb_define_method(rbgsl_cHist, "scale", Hist_scale, 1);
  rb_define_method(rbgsl_cHist, "shift", Hist_shift, 1);

  rb_define_method(rbgsl_cHist, "fwrite", Hist_fwrite, 1);
  rb_define_method(rbgsl_cHist, "fread", Hist_fread, 1);
  rb_define_method(rbgsl_cHist, "fprintf", Hist_fprintf, 3);
  rb_define_method(rbgsl_cHist, "dump", Hist_dump, 0);
  rb_define_method(rbgsl_cHist, "fscanf", Hist_fscanf, 1);

  rbgsl_cHist_PDF = rb_define_class_under(rbgsl_mHistogram, "Hist_PDF", rb_cObject);
  rb_define_singleton_method(rbgsl_cHist_PDF, "new", Hist_PDF_new, 1);
  rb_define_method(rbgsl_cHist_PDF, "initialize", Hist_PDF_initialize, 0);
  rb_define_method(rbgsl_cHist_PDF, "sample", Hist_PDF_sample, 1);
}

/* vim: set ts=4: */
