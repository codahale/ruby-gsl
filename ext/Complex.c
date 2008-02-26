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

/* $Id: Complex.c,v 1.5 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include <math.h>
#include "ruby.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_sf_trig.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Internal use only */

typedef double (* fun_t) (gsl_complex);
typedef gsl_complex (* fun_c_t) (gsl_complex);
typedef gsl_complex (* fun_d_t) (double);
typedef gsl_complex (* fun_c_c_t) (gsl_complex, gsl_complex);
typedef gsl_complex (* fun_c_d_t) (gsl_complex, double);
typedef int (* fun_c_r_t) (const double, const double, gsl_sf_result *, gsl_sf_result *);

static VALUE call(fun_t fun, VALUE v) {
  gsl_complex * ptr;
  Data_Get_Struct(v, gsl_complex, ptr);
  return rb_float_new((* fun)(*ptr));
}

static VALUE call_c(fun_c_t fun, VALUE v) {
  gsl_complex *ptr, c;
  Data_Get_Struct(v, gsl_complex, ptr);
  c = (*fun)(*ptr);
  return Complex_new_intern(rbgsl_cComplex, &c);
}

static VALUE call_d(fun_d_t fun, VALUE d) {
  gsl_complex c;
  c = (*fun)(NUM2DBL(d));
  return Complex_new_intern(rbgsl_cComplex, &c);
}

static VALUE call_c_c(fun_c_c_t fun, VALUE a, VALUE b) {
  gsl_complex * a_ptr, * b_ptr, c;
  Data_Get_Struct(a, gsl_complex, a_ptr);
  Data_Get_Struct(b, gsl_complex, b_ptr);
  c = (*fun)(*a_ptr, *b_ptr);
  return Complex_new_intern(rbgsl_cComplex, &c);
}

static VALUE call_c_d(fun_c_d_t fun, VALUE a, double d) {
  gsl_complex * ptr, c;
  Data_Get_Struct(a, gsl_complex, ptr);
  c = (*fun)(*ptr, d);
  return Complex_new_intern(rbgsl_cComplex, &c);
}

static VALUE call_c_r(fun_c_r_t fun, VALUE c) {
  int ret;
  gsl_sf_result p1, p2;
  gsl_complex * ptr;
  Data_Get_Struct(c, gsl_complex, ptr);
  ret = (*fun)(ptr->dat[0], ptr->dat[1], &p1, &p2);
  return rb_ary_new3(2, RESULT(&p1), RESULT(&p2));
}

/* Complex Numbers */

static VALUE Complex_init(VALUE self, VALUE x, VALUE y) {
  gsl_complex * ptr;
  Data_Get_Struct(self, gsl_complex, ptr);
  ptr->dat[0] = NUM2DBL(x);
  ptr->dat[1] = NUM2DBL(y);
  return self;
}

/* initialization from gsl_complex */
VALUE Complex_new_intern(VALUE class, gsl_complex * z) {
  gsl_complex * ptr;
  VALUE c = Data_Make_Struct(class, gsl_complex, 0, free, ptr);
  ptr->dat[0] = z->dat[0];
  ptr->dat[1] = z->dat[1];
  return c;
}

/* initialization from Ruby Complex object */
VALUE Complex_new(VALUE class, VALUE z) {
  gsl_complex * ptr;
  VALUE c = Data_Make_Struct(class, gsl_complex, 0, free, ptr);
  ptr->dat[0] = NUM2DBL(rb_funcall(z, rb_intern("real"), 0, 0));
  ptr->dat[1] = NUM2DBL(rb_funcall(z, rb_intern("image"), 0, 0));
  return c;
}

/* initialization from rectangular coordinates */
VALUE Complex_new2(VALUE class, VALUE x, VALUE y) {
  gsl_complex * ptr;
  VALUE c = Data_Make_Struct(class, gsl_complex, 0, free, ptr);
  ptr->dat[0] = NUM2DBL(x);
  ptr->dat[1] = NUM2DBL(y);
  return c;
}

/* initialization from polar coordinates */
VALUE Complex_new3(VALUE class, VALUE r, VALUE theta) {
  gsl_complex * ptr;
  double r1 = NUM2DBL(r);
  double theta1 = NUM2DBL(theta);
  VALUE c = Data_Make_Struct(class, gsl_complex, 0, free, ptr);
  ptr->dat[0] = r1 * gsl_sf_cos(theta1);
  ptr->dat[1] = r1 * gsl_sf_sin(theta1);
  return c;
}

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
VALUE Complex_allocate(VALUE kl) {
  return Complex_new2(kl, INT2NUM(0), INT2NUM(0));
}
#endif

/* Properties of Complex Numbers */

static VALUE Complex_eql(VALUE self, VALUE other) {
  gsl_complex * ptr, * optr;
  Data_Get_Struct(self, gsl_complex, ptr);
  Data_Get_Struct(other, gsl_complex, optr);
  if (ptr->dat[0] == optr->dat[0] && ptr->dat[1] == optr->dat[1])
    return Qtrue;
  else
    return Qfalse;
}

static VALUE Complex_float_equal(VALUE self, VALUE x, VALUE y, VALUE eps) {
  gsl_complex * ptr, * optr;
  double d1 = NUM2DBL(eps);
  Data_Get_Struct(x, gsl_complex, ptr);
  Data_Get_Struct(y, gsl_complex, optr);
  if ((fabs(ptr->dat[0] - optr->dat[0]) < d1) &&
	   (fabs(ptr->dat[1] - optr->dat[1]) < d1))
    return Qtrue;
  else
    return Qfalse;
}

/* TODO: make this better */
static VALUE Complex_to_s(VALUE self) {
  char str[30];
  gsl_complex * ptr;
  Data_Get_Struct(self, gsl_complex, ptr);
  sprintf(str, "(%f, %f)", ptr->dat[0], ptr->dat[1]);
  return rb_str_new2(str);
}

static VALUE Complex_to_a(VALUE self) {
  gsl_complex * ptr;
  VALUE ary;
  int i;
  Data_Get_Struct(self, gsl_complex, ptr);
  ary = rb_ary_new();
  for (i = 0; i < 2; i++)
    rb_ary_push(ary, rb_float_new(ptr->dat[i]));
  return ary;
}

static VALUE Complex_dump_data(VALUE self) {
  return Complex_to_a(self);
}

static VALUE Complex_load_data(VALUE self, VALUE ary) {
  gsl_complex *ptr;
  int i;
  Data_Get_Struct(self, gsl_complex, ptr);
  for (i = 0; i < 2; i++)
    ptr->dat[i] = NUM2DBL(rb_ary_entry(ary, i));
  return Qtrue;
}

static VALUE Complex_real(VALUE self) {
  gsl_complex * ptr;
  Data_Get_Struct(self, gsl_complex, ptr);
  return rb_float_new(ptr->dat[0]);
}

static VALUE Complex_image(VALUE self) {
  gsl_complex * ptr;
  Data_Get_Struct(self, gsl_complex, ptr);
  return rb_float_new(ptr->dat[1]);
}

static VALUE Complex_arg(VALUE self) {
  return call(gsl_complex_arg, self);
}

static VALUE Complex_abs(VALUE self) {
  return call(gsl_complex_abs, self);
}

static VALUE Complex_abs2(VALUE self) {
  return call(gsl_complex_abs2, self);
}

static VALUE Complex_logabs(VALUE self) {
  return call(gsl_complex_logabs, self);
}

/* Complex Arithmetic Operaters */

/*
static VALUE Complex_add(VALUE self, VALUE z) {
  if (FIXNUM_P(z))
    return call_c_d(gsl_complex_add_real, self, NUM2DBL(z));
  else
    return call_c_c(gsl_complex_add, self, z);
}
*/

static VALUE Complex_add(VALUE self, VALUE z) {
  return call_c_c(gsl_complex_add, self, z);
}

static VALUE Complex_add_real(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_add_real, self, NUM2DBL(z));
}

static VALUE Complex_add_imag(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_add_imag, self, NUM2DBL(z));
}

static VALUE Complex_mul(VALUE self, VALUE z) {
  return call_c_c(gsl_complex_mul, self, z);
}

static VALUE Complex_mul_real(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_mul_real, self, NUM2DBL(z));
}

static VALUE Complex_mul_imag(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_mul_imag, self, NUM2DBL(z));
}

static VALUE Complex_sub(VALUE self, VALUE z) {
  return call_c_c(gsl_complex_sub, self, z);
}

static VALUE Complex_sub_real(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_sub_real, self, NUM2DBL(z));
}

static VALUE Complex_sub_imag(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_sub_imag, self, NUM2DBL(z));
}

static VALUE Complex_div(VALUE self, VALUE z) {
  return call_c_c(gsl_complex_div, self, z);
}

static VALUE Complex_div_real(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_div_real, self, NUM2DBL(z));
}

static VALUE Complex_div_imag(VALUE self, VALUE z) {
  return call_c_d(gsl_complex_div_imag, self, NUM2DBL(z));
}

static VALUE Complex_inverse(VALUE self) {
  return call_c(gsl_complex_inverse, self);
}

static VALUE Complex_negative(VALUE self) {
  return call_c(gsl_complex_negative, self);
}

static VALUE Complex_conjugate(VALUE self) {
  return call_c(gsl_complex_conjugate, self);
}

/* Elementary Complex Functions */

static VALUE Complex_sqrt(VALUE self, VALUE c) {
  return call_c(gsl_complex_sqrt, c);
}

static VALUE Complex_sqrt_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_sqrt_real, d);
}

static VALUE Complex_pow(VALUE self, VALUE a, VALUE b) {
  return call_c_c(gsl_complex_pow, a, b);
}

static VALUE Complex_pow_real(VALUE self, VALUE a, VALUE b) {
  return call_c_d(gsl_complex_pow_real, a, NUM2DBL(b));
}

static VALUE Complex_exp(VALUE self, VALUE c) {
  return call_c(gsl_complex_exp, c);
}

static VALUE Complex_log(VALUE self, VALUE c) {
  return call_c(gsl_complex_log, c);
}

static VALUE Complex_log10(VALUE self, VALUE c) {
  return call_c(gsl_complex_log10, c);
}

static VALUE Complex_logb(VALUE self, VALUE a, VALUE b) {
  return call_c_c(gsl_complex_log_b, a, b);
}

/* Complex Trigonometric Functions */

static VALUE Complex_sin(VALUE self, VALUE c) {
  return call_c(gsl_complex_sin, c);
}

static VALUE Complex_sin_e(VALUE self, VALUE c) {
  return call_c_r(gsl_sf_complex_sin_e, c);
}

static VALUE Complex_cos(VALUE self, VALUE c) {
  return call_c(gsl_complex_cos, c);
}

static VALUE Complex_cos_e(VALUE self, VALUE c) {
  return call_c_r(gsl_sf_complex_cos_e, c);
}

static VALUE Complex_tan(VALUE self, VALUE c) {
  return call_c(gsl_complex_tan, c);
}

static VALUE Complex_sec(VALUE self, VALUE c) {
  return call_c(gsl_complex_sec, c);
}

static VALUE Complex_csc(VALUE self, VALUE c) {
  return call_c(gsl_complex_csc, c);
}

static VALUE Complex_cot(VALUE self, VALUE c) {
  return call_c(gsl_complex_cot, c);
}

static VALUE Complex_logsin_e(VALUE self, VALUE c) {
  return call_c_r(gsl_sf_complex_logsin_e, c);
}

/* Inverse Complex Trigonometric Functions */

static VALUE Complex_arcsin(VALUE self, VALUE c) {
  return call_c(gsl_complex_arcsin, c);
}

static VALUE Complex_arcsin_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_arcsin_real, d);
}

static VALUE Complex_arccos(VALUE self, VALUE c) {
  return call_c(gsl_complex_arccos, c);
}

static VALUE Complex_arccos_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_arccos_real, d);
}

static VALUE Complex_arctan(VALUE self, VALUE c) {
  return call_c(gsl_complex_arctan, c);
}

static VALUE Complex_arcsec(VALUE self, VALUE c) {
  return call_c(gsl_complex_arcsec, c);
}

static VALUE Complex_arcsec_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_arcsec_real, d);
}

static VALUE Complex_arccsc(VALUE self, VALUE c) {
  return call_c(gsl_complex_arccsc, c);
}

static VALUE Complex_arccsc_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_arccsc_real, d);
}

static VALUE Complex_arccot(VALUE self, VALUE c) {
  return call_c(gsl_complex_arccot, c);
}

/* Complex Hyperbolic Functions */

static VALUE Complex_sinh(VALUE self, VALUE c) {
  return call_c(gsl_complex_sinh, c);
}

static VALUE Complex_cosh(VALUE self, VALUE c) {
  return call_c(gsl_complex_cosh, c);
}

static VALUE Complex_tanh(VALUE self, VALUE c) {
  return call_c(gsl_complex_tanh, c);
}

static VALUE Complex_sech(VALUE self, VALUE c) {
  return call_c(gsl_complex_sech, c);
}

static VALUE Complex_csch(VALUE self, VALUE c) {
  return call_c(gsl_complex_csch, c);
}

static VALUE Complex_coth(VALUE self, VALUE c) {
  return call_c(gsl_complex_coth, c);
}

/* Inverse Complex Hyperbolic Functions */

static VALUE Complex_arcsinh(VALUE self, VALUE c) {
  return call_c(gsl_complex_arcsinh, c);
}

static VALUE Complex_arccosh(VALUE self, VALUE c) {
  return call_c(gsl_complex_arccosh, c);
}

static VALUE Complex_arccosh_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_arccosh_real, d);
}

static VALUE Complex_arctanh(VALUE self, VALUE c) {
  return call_c(gsl_complex_arctanh, c);
}

static VALUE Complex_arctanh_real(VALUE self, VALUE d) {
  return call_d(gsl_complex_arctanh_real, d);
}

static VALUE Complex_arcsech(VALUE self, VALUE c) {
  return call_c(gsl_complex_arcsech, c);
}

static VALUE Complex_arccsch(VALUE self, VALUE c) {
  return call_c(gsl_complex_arccsch, c);
}

static VALUE Complex_arccoth(VALUE self, VALUE c) {
  return call_c(gsl_complex_arccoth, c);
}

/* Class definition */

VALUE rbgsl_cComplex;

void Init_Complex() {
  rbgsl_cComplex = rb_define_class_under(rbgsl_mGSL, "Complex",  rb_cObject);
  rb_define_singleton_method(rbgsl_cComplex, "new", Complex_new, 1);
  rb_define_singleton_method(rbgsl_cComplex, "new2", Complex_new2, 2);
  rb_define_singleton_method(rbgsl_cComplex, "new3", Complex_new3, 2);
  rb_define_method(rbgsl_cComplex, "initialize", Complex_init, 2);

  rb_define_method(rbgsl_cComplex, "==", Complex_eql, 1);
  rb_define_singleton_method(rbgsl_cComplex, "float_equal", Complex_float_equal, 3);
  rb_define_method(rbgsl_cComplex, "to_s", Complex_to_s, 0);
  rb_define_method(rbgsl_cComplex, "real", Complex_real, 0);
  rb_define_method(rbgsl_cComplex, "image", Complex_image, 0);
  rb_define_alias(rbgsl_cComplex, "imag", "image");
  rb_define_method(rbgsl_cComplex, "arg", Complex_arg, 0);
  rb_define_method(rbgsl_cComplex, "abs", Complex_abs, 0);
  rb_define_method(rbgsl_cComplex, "abs2", Complex_abs2, 0);
  rb_define_method(rbgsl_cComplex, "logabs", Complex_logabs, 0);

  rb_define_method(rbgsl_cComplex, "add", Complex_add, 1);
  rb_define_method(rbgsl_cComplex, "add_real", Complex_add_real, 1);
  rb_define_method(rbgsl_cComplex, "add_imag", Complex_add_imag, 1);
  rb_define_alias(rbgsl_cComplex, "+", "add");
  rb_define_method(rbgsl_cComplex, "sub", Complex_sub, 1);
  rb_define_method(rbgsl_cComplex, "sub_real", Complex_sub_real, 1);
  rb_define_method(rbgsl_cComplex, "sub_imag", Complex_sub_imag, 1);
  rb_define_alias(rbgsl_cComplex, "-", "sub");
  rb_define_method(rbgsl_cComplex, "mul", Complex_mul, 1);
  rb_define_method(rbgsl_cComplex, "mul_real", Complex_mul_real, 1);
  rb_define_method(rbgsl_cComplex, "mul_imag", Complex_mul_imag, 1);
  rb_define_alias(rbgsl_cComplex, "*", "mul");
  rb_define_method(rbgsl_cComplex, "div", Complex_div, 1);
  rb_define_method(rbgsl_cComplex, "div_real", Complex_div_real, 1);
  rb_define_method(rbgsl_cComplex, "div_imag", Complex_div_imag, 1);
  rb_define_alias(rbgsl_cComplex, "/", "div");
  rb_define_method(rbgsl_cComplex, "conjugate", Complex_conjugate, 0);
  rb_define_method(rbgsl_cComplex, "inverse", Complex_inverse, 0);
  rb_define_method(rbgsl_cComplex, "negative", Complex_negative, 0);
  rb_define_method(rbgsl_cComplex, "to_a", Complex_to_a, 0);
  rb_define_method(rbgsl_cComplex, "_dump_data", Complex_dump_data, 0);
  rb_define_method(rbgsl_cComplex, "_load_data", Complex_load_data, 1);
	  
  rb_define_alias(rbgsl_cComplex, "-@", "negative");

  rb_define_singleton_method(rbgsl_cComplex, "sqrt", Complex_sqrt, 1);
  rb_define_singleton_method(rbgsl_cComplex, "sqrt_real", Complex_sqrt_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "exp", Complex_exp, 1);
  rb_define_singleton_method(rbgsl_cComplex, "pow", Complex_pow, 2);
  rb_define_singleton_method(rbgsl_cComplex, "pow_real", Complex_pow_real, 2);
  rb_define_singleton_method(rbgsl_cComplex, "log", Complex_log, 1);
  rb_define_singleton_method(rbgsl_cComplex, "log10", Complex_log10, 1);
  rb_define_singleton_method(rbgsl_cComplex, "logb", Complex_logb, 2);

  rb_define_singleton_method(rbgsl_cComplex, "sin", Complex_sin, 1);
  rb_define_singleton_method(rbgsl_cComplex, "cos", Complex_cos, 1);
  rb_define_singleton_method(rbgsl_cComplex, "tan", Complex_tan, 1);
  rb_define_singleton_method(rbgsl_cComplex, "csc", Complex_csc, 1);
  rb_define_singleton_method(rbgsl_cComplex, "sec", Complex_sec, 1);
  rb_define_singleton_method(rbgsl_cComplex, "cot", Complex_cot, 1);

  rb_define_singleton_method(rbgsl_cComplex, "sin_e", Complex_sin_e, 1);
  rb_define_singleton_method(rbgsl_cComplex, "cos_e", Complex_cos_e, 1);
  rb_define_singleton_method(rbgsl_cComplex, "logsin_e", Complex_logsin_e, 1);

  rb_define_singleton_method(rbgsl_cComplex, "arcsin", Complex_arcsin, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arcsin_real", Complex_arcsin_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccos", Complex_arccos, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccos_real", Complex_arccos_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arctan", Complex_arctan, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccsc", Complex_arccsc, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccsc_real", Complex_arccsc_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arcsec", Complex_arcsec, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arcsec_real", Complex_arcsec_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccot", Complex_arccot, 1);

  rb_define_singleton_method(rbgsl_cComplex, "sinh", Complex_sinh, 1);
  rb_define_singleton_method(rbgsl_cComplex, "cosh", Complex_cosh, 1);
  rb_define_singleton_method(rbgsl_cComplex, "tanh", Complex_tanh, 1);
  rb_define_singleton_method(rbgsl_cComplex, "csch", Complex_csch, 1);
  rb_define_singleton_method(rbgsl_cComplex, "sech", Complex_sech, 1);
  rb_define_singleton_method(rbgsl_cComplex, "coth", Complex_coth, 1);

  rb_define_singleton_method(rbgsl_cComplex, "arcsinh", Complex_arcsinh, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccosh", Complex_arccosh, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccosh_real", Complex_arccosh_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arctanh", Complex_arctanh, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arctanh_real", Complex_arctanh_real, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccsch", Complex_arccsch, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arcsech", Complex_arcsech, 1);
  rb_define_singleton_method(rbgsl_cComplex, "arccoth", Complex_arccoth, 1);

#ifdef HAVE_DEFINE_ALLOC_FUNCTION
  rb_define_alloc_func(rbgsl_cComplex, Complex_allocate);
#endif
}

/* vim: set ts=4: */
