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

/* $Id: Hist2D.c,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include <stdio.h>
#include "ruby.h"
#include "rubyio.h"
#include "gsl/gsl_histogram2d.h"
#include "gsl/gsl_errno.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* 2D Histograms */

VALUE rbgsl_cHist2D;
VALUE rbgsl_cHist2D_PDF;

/* Internal use only */

typedef double (* fun_d_t) (const gsl_histogram2d *);
typedef int (* fun_h_t) (gsl_histogram2d *, const gsl_histogram2d *);
typedef int (* fun_h_d_t) (gsl_histogram2d *, double);

static VALUE call_d(fun_d_t fun, VALUE v) {
  gsl_histogram2d * ptr;
  Data_Get_Struct(v, gsl_histogram2d, ptr);
  return rb_float_new((*fun)(ptr));
}

static VALUE call_h(fun_h_t fun, VALUE h1, VALUE h2) {
  gsl_histogram2d * ptr1, * ptr2;
  int ret;
  Data_Get_Struct(h1, gsl_histogram2d, ptr1);
  Data_Get_Struct(h2, gsl_histogram2d, ptr2);
  ret = (*fun)(ptr1, ptr2);
  if (ret)
    rb_raise(rb_eArgError, "Operation failed.");
  return h1;
}

static VALUE call_h_d(fun_h_d_t fun, VALUE h, VALUE d) {
  gsl_histogram2d * ptr;
  int ret;
  Data_Get_Struct(h, gsl_histogram2d, ptr);
  ret = (*fun)(ptr, NUM2DBL(d));
  if (ret)
    rb_raise(rb_eArgError, "Operation failed.");
  return h;
}

/* 2D Histogram Allocation */

static VALUE Hist2D_initialize(VALUE self) {
  return self;
}

static void Hist2D_free(void * p) {
  gsl_histogram2d_free(p);
}

VALUE Hist2D_new(VALUE class, VALUE nx, VALUE ny) {
  VALUE h;
  gsl_histogram2d * ptr;
  ptr = gsl_histogram2d_calloc(NUM2INT(nx), NUM2INT(ny));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  h = Data_Wrap_Struct(class, 0, Hist2D_free, ptr);
  return h;
}

VALUE Hist2D_new2(VALUE class, VALUE nx, VALUE ny, VALUE xmin, VALUE xmax,
  VALUE ymin, VALUE ymax) {
  VALUE h;
  gsl_histogram2d * ptr;
  if (NUM2INT(nx) < 0 || NUM2INT(ny) < 0)
    rb_raise(rb_eArgError, "Histogram size must be positive.");
  ptr = gsl_histogram2d_calloc_uniform(NUM2INT(nx), NUM2INT(ny), NUM2DBL(xmin),
    NUM2DBL(xmax), NUM2DBL(ymin), NUM2DBL(ymax));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  h = Data_Wrap_Struct(class, 0, Hist2D_free, ptr);
  return h;
}

static VALUE Hist2D_set_xrange(VALUE self, VALUE n, VALUE d) {
  gsl_histogram2d * ptr;
  size_t i = NUM2INT(n);
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  if (i < 0 || i > ptr->nx)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  ptr->xrange[i] = NUM2DBL(d);
  return self;
}

static VALUE Hist2D_set_yrange(VALUE self, VALUE n, VALUE d) {
  gsl_histogram2d * ptr;
  size_t i = NUM2INT(n);
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  if (i < 0 || i > ptr->ny)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  ptr->yrange[i] = NUM2DBL(d);
  return self;
}

static VALUE Hist2D_set_ranges_uniform(VALUE self, VALUE xmin, VALUE xmax,
  VALUE ymin, VALUE ymax) {
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);

  return INT2FIX(gsl_histogram2d_set_ranges_uniform(ptr, NUM2DBL(xmin),
    NUM2DBL(xmax), NUM2DBL(ymin), NUM2DBL(ymax)));
}
  
static VALUE Hist2D_set_ranges(VALUE self, VALUE xrange, VALUE yrange) {
  gsl_histogram2d * ptr;
  double * x, * y;

  Data_Get_Struct(self, gsl_histogram2d, ptr);
  COPYRUBYARRAY(xrange, x);
  COPYRUBYARRAY(yrange, y);
  return INT2FIX(gsl_histogram2d_set_ranges(ptr, x, RARRAY(xrange)->len, y, RARRAY(yrange)->len));
}

/* Copying Histograms */

static VALUE Hist2D_clone(VALUE self) {
  VALUE clone;
  gsl_histogram2d * ptr, * cptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  cptr = gsl_histogram2d_clone(ptr);
  clone = Data_Wrap_Struct(rbgsl_cHist2D, 0, Hist2D_free, cptr);
  return clone;
}

static VALUE Hist2D_memcpy(VALUE self, VALUE dest, VALUE src) {
  int ret;
  gsl_histogram2d * pdest, * psrc;
  Data_Get_Struct(dest, gsl_histogram2d, pdest);
  Data_Get_Struct(src, gsl_histogram2d, psrc);
  ret = gsl_histogram2d_memcpy(pdest, psrc);
  return INT2FIX(ret);
}

/* 2D Histogram Probability Distribution */

static VALUE Hist2D_PDF_initialize(VALUE self) {
  return self;
}

static void Hist2D_PDF_free(void * p) {
  gsl_histogram2d_pdf_free(p);
}

VALUE Hist2D_PDF_new(VALUE class, VALUE h) {
  VALUE pdf;
  gsl_histogram2d_pdf * ptr;
  gsl_histogram2d * hptr;
  Data_Get_Struct(h, gsl_histogram2d, hptr);
  ptr = gsl_histogram2d_pdf_alloc(hptr->nx, hptr->ny);
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  pdf = Data_Wrap_Struct(class, 0, Hist2D_PDF_free, ptr);
  gsl_histogram2d_pdf_init(ptr, hptr);
  return pdf;
}

static VALUE Hist2D_PDF_sample(VALUE self, VALUE r1, VALUE r2) {
  VALUE ary;
  double x, y;
  int ret;
  gsl_histogram2d_pdf * ptr;
  Data_Get_Struct(self, gsl_histogram2d_pdf, ptr);
  ret = gsl_histogram2d_pdf_sample(ptr, NUM2DBL(r1), NUM2DBL(r2), &x, &y);
  if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  ary = rb_ary_new3(2, rb_float_new(x), rb_float_new(y));
  return ary;
}


/* Updating and Accessing Histogram Elements */

static VALUE Hist2D_increment(VALUE self, VALUE x, VALUE y) {
  gsl_histogram2d * ptr;
  int ret;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  ret = gsl_histogram2d_increment(ptr, NUM2DBL(x), NUM2DBL(y));
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Value does not belong to any bin.");
  return self;
}

static VALUE Hist2D_accumulate(VALUE self, VALUE x, VALUE y, VALUE w) {
  gsl_histogram2d * ptr;
  int ret;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  ret = gsl_histogram2d_accumulate(ptr, NUM2DBL(x), NUM2DBL(y), NUM2DBL(w));
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Value does not belong to any bin.");
  return self;
}

static VALUE Hist2D_get(VALUE self, VALUE i, VALUE j) {
  gsl_histogram2d * ptr;
  double val;
  int ni = NUM2INT(i), nj = NUM2INT(j);
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  if (ni < 0 || ni > ptr->nx)
    rb_raise(rb_eRangeError, "Index i is outside of allowed range.");
  if (nj < 0 || nj > ptr->nx)
    rb_raise(rb_eRangeError, "Index j is outside of allowed range.");
  val = gsl_histogram2d_get(ptr, ni, nj);
  return rb_float_new(val);
}

static VALUE Hist2D_get_xrange(VALUE self, VALUE i) {
  gsl_histogram2d * ptr;
  int ret;
  double lower, upper;
  VALUE ary;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  ret = gsl_histogram2d_get_xrange(ptr, NUM2INT(i), &lower, &upper);
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  ary = rb_ary_new3(2, rb_float_new(lower), rb_float_new(upper));
  return ary;
}

static VALUE Hist2D_get_yrange(VALUE self, VALUE i) {
  gsl_histogram2d * ptr;
  int ret;
  double lower, upper;
  VALUE ary;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  ret = gsl_histogram2d_get_yrange(ptr, NUM2INT(i), &lower, &upper);
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Index is outside of allowed range.");
  ary = rb_ary_new3(2, rb_float_new(lower), rb_float_new(upper));
  return ary;
}

static VALUE Hist2D_xmax(VALUE self) {
  return call_d(gsl_histogram2d_xmax, self);
}

static VALUE Hist2D_xmin(VALUE self) {
  return call_d(gsl_histogram2d_xmin, self);
}

static VALUE Hist2D_nx(VALUE self) {
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  return INT2FIX(gsl_histogram2d_nx(ptr));
}

static VALUE Hist2D_ymax(VALUE self) {
  return call_d(gsl_histogram2d_ymax, self);
}

static VALUE Hist2D_ymin(VALUE self) {
  return call_d(gsl_histogram2d_ymin, self);
}

static VALUE Hist2D_ny(VALUE self) {
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  return INT2FIX(gsl_histogram2d_ny(ptr));
}

static VALUE Hist2D_reset(VALUE self) {
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  gsl_histogram2d_reset(ptr);
  return self;
}

/* Searching Histograms */

static VALUE Hist2D_find(VALUE self, VALUE x, VALUE y) {
  VALUE ary;
  gsl_histogram2d * ptr;
  int ret;
  size_t i, j;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  ret = gsl_histogram2d_find(ptr, NUM2DBL(x), NUM2DBL(y), &i, &j);
  if (ret == GSL_EDOM)
    rb_raise(rb_eRangeError, "Value does not belong to any bin.");
  ary = rb_ary_new3(2, INT2NUM(i), INT2NUM(j));
  return ary;
}

/* Histogram Statistics */

static VALUE Hist2D_max_val(VALUE self) {
  return call_d(gsl_histogram2d_max_val, self);
}

static VALUE Hist2D_min_val(VALUE self) {
  return call_d(gsl_histogram2d_min_val, self);
}

static VALUE Hist2D_xmean(VALUE self) {
  return call_d(gsl_histogram2d_xmean, self);
}

static VALUE Hist2D_ymean(VALUE self) {
  return call_d(gsl_histogram2d_ymean, self);
}

static VALUE Hist2D_xsigma(VALUE self) {
  return call_d(gsl_histogram2d_xsigma, self);
}

static VALUE Hist2D_ysigma(VALUE self) {
  return call_d(gsl_histogram2d_ysigma, self);
}

static VALUE Hist2D_cov(VALUE self) {
  return call_d(gsl_histogram2d_cov, self);
}

static VALUE Hist2D_sum(VALUE self) {
  return call_d(gsl_histogram2d_sum, self);
}

static VALUE Hist2D_max_bin(VALUE self) {
  VALUE ary;
  size_t i, j;
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  gsl_histogram2d_max_bin(ptr, &i, &j);
  ary = rb_ary_new3(2, INT2NUM(i), INT2NUM(j));
  return ary;
}

static VALUE Hist2D_min_bin(VALUE self) {
  VALUE ary;
  size_t i, j;
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  gsl_histogram2d_min_bin(ptr, &i, &j);
  ary = rb_ary_new3(2, INT2NUM(i), INT2NUM(j));
  return ary;
}

/* Histogram Operations */

static VALUE Hist2D_equal_bins_p(VALUE self, VALUE other) {
  gsl_histogram2d * ptr1, * ptr2;
  int ret;
  Data_Get_Struct(self, gsl_histogram2d, ptr1);
  Data_Get_Struct(other, gsl_histogram2d, ptr2);
  ret = gsl_histogram2d_equal_bins_p(ptr1, ptr2);
  if (ret)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE Hist2D_add(VALUE self, VALUE other) {
  return call_h(gsl_histogram2d_add, self, other);
}

static VALUE Hist2D_sub(VALUE self, VALUE other) {
  return call_h(gsl_histogram2d_sub, self, other);
}

static VALUE Hist2D_mul(VALUE self, VALUE other) {
  return call_h(gsl_histogram2d_mul, self, other);
}

static VALUE Hist2D_div(VALUE self, VALUE other) {
  return call_h(gsl_histogram2d_div, self, other);
}

static VALUE Hist2D_scale(VALUE self, VALUE d) {
  return call_h_d(gsl_histogram2d_scale, self, d);
}

static VALUE Hist2D_shift(VALUE self, VALUE d) {
  return call_h_d(gsl_histogram2d_shift, self, d);
}

/* Reading and Writing Histograms */

static VALUE Hist2D_fwrite(VALUE self, VALUE stream) {
  int ret;
  gsl_histogram2d * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_histogram2d_fwrite(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist2D_fread(VALUE self, VALUE stream) {
  int ret;
  gsl_histogram2d * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_histogram2d_fread(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist2D_fprintf(VALUE self, VALUE stream, VALUE range_format,
  VALUE bin_format) {
  int ret;
  gsl_histogram2d * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_WRITABLE))
    rb_raise(rb_eIOError, "File is not opened for writing");
  ret = gsl_histogram2d_fprintf(fptr->f, ptr, STR2CSTR(range_format), 
    STR2CSTR(bin_format));
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist2D_dump(VALUE self) {
  int ret;
  gsl_histogram2d * ptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  ret = gsl_histogram2d_fprintf(stdout, ptr, "%g", "%g");
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem writing to file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

static VALUE Hist2D_fscanf(VALUE self, VALUE stream) {
  int ret;
  gsl_histogram2d * ptr;
  OpenFile * fptr;
  Data_Get_Struct(self, gsl_histogram2d, ptr);
  Check_Type(stream, T_FILE);
  GetOpenFile(stream, fptr);
  if (!(fptr->mode & FMODE_READABLE))
    rb_raise(rb_eIOError, "File is not opened for reading");
  ret = gsl_histogram2d_fscanf(fptr->f, ptr);
  if (ret == GSL_EFAILED)
    rb_raise(rb_eIOError, "Encountered problem reading from file.");
  else if (ret)
    rb_raise(rb_eStandardError, "Unknown error.");
  return self;
}

/* Class definition */

void Init_Hist2D() {
  rbgsl_cHist2D = rb_define_class_under(rbgsl_mHistogram, "Hist2D", rb_cObject);

  rb_define_singleton_method(rbgsl_cHist2D, "new", Hist2D_new, 2);
  rb_define_singleton_method(rbgsl_cHist2D, "new2", Hist2D_new2, 6);
  rb_define_method(rbgsl_cHist2D, "initialize", Hist2D_initialize, 0);
 
  rb_define_method(rbgsl_cHist2D, "set_xrange", Hist2D_set_xrange, 2);
  rb_define_method(rbgsl_cHist2D, "set_yrange", Hist2D_set_yrange, 2);
  rb_define_method(rbgsl_cHist2D, "set_ranges", Hist2D_set_ranges, 2);
  rb_define_method(rbgsl_cHist2D, "set_ranges_uniform", Hist2D_set_ranges_uniform, 4);

  rb_define_module_function(rbgsl_cHist2D, "memcpy", Hist2D_memcpy, 2);
  rb_define_method(rbgsl_cHist2D, "clone", Hist2D_clone, 0);

  rb_define_method(rbgsl_cHist2D, "increment", Hist2D_increment, 2);
  rb_define_alias(rbgsl_cHist2D, "inc", "increment");
  rb_define_method(rbgsl_cHist2D, "accumulate", Hist2D_accumulate, 3);
  rb_define_alias(rbgsl_cHist2D, "acc", "accumulate");
  rb_define_method(rbgsl_cHist2D, "get", Hist2D_get, 2);
  rb_define_method(rbgsl_cHist2D, "get_xrange", Hist2D_get_xrange, 1);
  rb_define_method(rbgsl_cHist2D, "get_yrange", Hist2D_get_yrange, 1);
  rb_define_method(rbgsl_cHist2D, "xmax", Hist2D_xmax, 0);
  rb_define_method(rbgsl_cHist2D, "xmin", Hist2D_xmin, 0);
  rb_define_method(rbgsl_cHist2D, "nx", Hist2D_nx, 0);
  rb_define_method(rbgsl_cHist2D, "ymax", Hist2D_ymax, 0);
  rb_define_method(rbgsl_cHist2D, "ymin", Hist2D_ymin, 0);
  rb_define_method(rbgsl_cHist2D, "ny", Hist2D_ny, 0);
  rb_define_method(rbgsl_cHist2D, "reset", Hist2D_reset, 0);

  rb_define_method(rbgsl_cHist2D, "find", Hist2D_find, 2);

  rb_define_method(rbgsl_cHist2D, "max_val", Hist2D_max_val, 0);
  rb_define_method(rbgsl_cHist2D, "min_val", Hist2D_min_val, 0);
  rb_define_method(rbgsl_cHist2D, "max_bin", Hist2D_max_bin, 0);
  rb_define_method(rbgsl_cHist2D, "min_bin", Hist2D_min_bin, 0);

  rb_define_method(rbgsl_cHist2D, "xmean", Hist2D_xmean, 0);
  rb_define_method(rbgsl_cHist2D, "ymean", Hist2D_ymean, 0);
  rb_define_method(rbgsl_cHist2D, "xsigma", Hist2D_xsigma, 0);
  rb_define_method(rbgsl_cHist2D, "ysigma", Hist2D_ysigma, 0);
  rb_define_method(rbgsl_cHist2D, "cov", Hist2D_cov, 0);
  rb_define_method(rbgsl_cHist2D, "sum", Hist2D_sum, 0);

  rb_define_method(rbgsl_cHist2D, "equal_bins_p", Hist2D_equal_bins_p, 1);
  rb_define_alias(rbgsl_cHist2D, "equal_bins?", "equal_bins_p");
  rb_define_method(rbgsl_cHist2D, "add", Hist2D_add, 1);
  rb_define_alias(rbgsl_cHist2D, "+", "add");
  rb_define_method(rbgsl_cHist2D, "sub", Hist2D_sub, 1);
  rb_define_alias(rbgsl_cHist2D, "-", "sub");
  rb_define_method(rbgsl_cHist2D, "mul", Hist2D_mul, 1);
  rb_define_alias(rbgsl_cHist2D, "*", "mul");
  rb_define_method(rbgsl_cHist2D, "div", Hist2D_div, 1);
  rb_define_alias(rbgsl_cHist2D, "/", "div");
  rb_define_method(rbgsl_cHist2D, "scale", Hist2D_scale, 1);
  rb_define_method(rbgsl_cHist2D, "shift", Hist2D_shift, 1);

  rb_define_method(rbgsl_cHist2D, "fwrite", Hist2D_fwrite, 1);
  rb_define_method(rbgsl_cHist2D, "fread", Hist2D_fread, 1);
  rb_define_method(rbgsl_cHist2D, "fprintf", Hist2D_fprintf, 3);
  rb_define_method(rbgsl_cHist2D, "dump", Hist2D_dump, 0);
  rb_define_method(rbgsl_cHist2D, "fscanf", Hist2D_fscanf, 1);

  rbgsl_cHist2D_PDF = rb_define_class_under(rbgsl_mHistogram, "Hist2D_PDF", rb_cObject);
  rb_define_singleton_method(rbgsl_cHist2D_PDF, "new", Hist2D_PDF_new, 1);
  rb_define_method(rbgsl_cHist2D_PDF, "initialize", Hist2D_PDF_initialize, 0);
  rb_define_method(rbgsl_cHist2D_PDF, "sample", Hist2D_PDF_sample, 2);
}

/* vim: set ts=4: */
