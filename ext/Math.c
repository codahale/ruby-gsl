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

/* $Id: Math.c,v 1.6 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_math.h"
#include "ruby-gsl.h"

/* TODO: Infinities and Not-a-number */

/* Elementary Functions */

static VALUE Math_log1p(VALUE self, VALUE x) {
  return rb_float_new(gsl_log1p(NUM2DBL(x)));
}

static VALUE Math_expm1(VALUE self, VALUE x) {
  return rb_float_new(gsl_expm1(NUM2DBL(x)));
}

static VALUE Math_acosh(VALUE self, VALUE x) {
  return rb_float_new(gsl_acosh(NUM2DBL(x)));
}

static VALUE Math_asinh(VALUE self, VALUE x) {
  return rb_float_new(gsl_asinh(NUM2DBL(x)));
}

static VALUE Math_atanh(VALUE self, VALUE x) {
  return rb_float_new(gsl_atanh(NUM2DBL(x)));
}

static VALUE Math_hypot(VALUE self, VALUE x, VALUE y) {
  return rb_float_new(gsl_hypot(NUM2DBL(x), NUM2DBL(y)));
}




/* Small integer powers */

static VALUE Math_pow_int(VALUE self, VALUE x, VALUE n) {
  return rb_float_new(gsl_pow_int(NUM2DBL(x), NUM2INT(n)));
}

static VALUE Math_pow_2(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_2(NUM2DBL(x)));
}

static VALUE Math_pow_3(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_3(NUM2DBL(x)));
}

static VALUE Math_pow_4(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_4(NUM2DBL(x)));
}

static VALUE Math_pow_5(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_5(NUM2DBL(x)));
}

static VALUE Math_pow_6(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_6(NUM2DBL(x)));
}

static VALUE Math_pow_7(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_7(NUM2DBL(x)));
}

static VALUE Math_pow_8(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_8(NUM2DBL(x)));
}

static VALUE Math_pow_9(VALUE self, VALUE x) {
  return rb_float_new(gsl_pow_9(NUM2DBL(x)));
}

static VALUE Math_pow(VALUE self, VALUE x, VALUE y) {
  return rb_float_new(pow(NUM2DBL(x), NUM2DBL(y)));
}

/* Testing the Sign of Numbers */

static VALUE Math_sign(VALUE self, VALUE x) {
  return INT2FIX(GSL_SIGN(NUM2DBL(x)));
}

/* Testing for Odd and Even Numbers */

static VALUE Math_is_odd(VALUE self, VALUE x) {
  int type;
  type = TYPE(x);
  if (type == T_FIXNUM || type == T_BIGNUM)
    return INT2FIX(GSL_IS_ODD(FIX2INT(x)));
  else
	return Qnil;
}

static VALUE Math_is_even(VALUE self, VALUE x) {
  int type;
  type = TYPE(x);
  if (type == T_FIXNUM || type == T_BIGNUM)
    return INT2FIX(GSL_IS_EVEN(FIX2INT(x)));
  else
	return Qnil;
}

static VALUE Math_fcmp(VALUE self, VALUE x, VALUE y, VALUE eps) {
  double d1 = NUM2DBL(eps);
/*
 * Check_Type(x, T_FLOAT);
 * Check_Type(y, T_FLOAT); 
 * Check_Type(eps, T_FLOAT);
 */
  if (d1 <= 0)
    rb_raise(rb_eStandardError,"eps should be positive");
  return INT2FIX(gsl_fcmp(NUM2DBL(x),NUM2DBL(y), d1));
}
	  
static VALUE Math_float_equal(VALUE self, VALUE x, VALUE y, VALUE eps) {
  double d1 = NUM2DBL(eps);
  double diff = NUM2DBL(x) - NUM2DBL(y);
  if (fabs(diff) < d1)
    return Qtrue;
  else
    return Qfalse;
}

/* TODO: Maximum and Minimum Functions */


/* Module definition */

void Init_GSLMath() {

  rb_define_module_function(rb_mMath, "asinh", Math_asinh, 1);
  rb_define_module_function(rb_mMath, "acosh", Math_acosh, 1);
  rb_define_module_function(rb_mMath, "atanh", Math_atanh, 1);

  rb_define_module_function(rb_mMath, "log1p", Math_log1p, 1);
  rb_define_module_function(rb_mMath, "expm1", Math_expm1, 1);
  rb_define_module_function(rb_mMath, "hypot", Math_hypot, 2);

  rb_define_module_function(rb_mMath, "pow", Math_pow, 2);
  rb_define_module_function(rb_mMath, "pow_int", Math_pow_int, 2);
  rb_define_module_function(rb_mMath, "pow_2", Math_pow_2, 1);
  rb_define_module_function(rb_mMath, "pow_3", Math_pow_3, 1);
  rb_define_module_function(rb_mMath, "pow_4", Math_pow_4, 1);
  rb_define_module_function(rb_mMath, "pow_5", Math_pow_5, 1);
  rb_define_module_function(rb_mMath, "pow_6", Math_pow_6, 1);
  rb_define_module_function(rb_mMath, "pow_7", Math_pow_7, 1);
  rb_define_module_function(rb_mMath, "pow_8", Math_pow_8, 1);
  rb_define_module_function(rb_mMath, "pow_9", Math_pow_9, 1);

  rb_define_module_function(rb_mMath, "sign", Math_sign, 1);
  rb_define_module_function(rb_mMath, "is_odd", Math_is_odd, 1);
  rb_define_module_function(rb_mMath, "is_even", Math_is_even, 1);
  rb_define_module_function(rb_mMath, "fcmp", Math_fcmp, 3);
  rb_define_module_function(rb_mMath, "float_equal", Math_float_equal, 3);

  rb_define_const(rb_mMath, "M_E", rb_float_new(M_E));
  rb_define_const(rb_mMath, "M_LOG2E", rb_float_new(M_LOG2E));
  rb_define_const(rb_mMath, "M_LOG10E", rb_float_new(M_LOG10E));
  rb_define_const(rb_mMath, "M_SQRT2", rb_float_new(M_SQRT2));
  rb_define_const(rb_mMath, "M_SQRT1_2", rb_float_new(M_SQRT1_2));
  rb_define_const(rb_mMath, "M_SQRT3", rb_float_new(M_SQRT3));
  rb_define_const(rb_mMath, "M_PI", rb_float_new(M_PI));
  rb_define_const(rb_mMath, "M_PI_2", rb_float_new(M_PI_2));
  rb_define_const(rb_mMath, "M_PI_4", rb_float_new(M_PI_4));
  rb_define_const(rb_mMath, "M_SQRTPI", rb_float_new(M_SQRTPI));
  rb_define_const(rb_mMath, "M_2_SQRTPI", rb_float_new(M_2_SQRTPI));
  rb_define_const(rb_mMath, "M_2_PI", rb_float_new(M_1_PI));
  rb_define_const(rb_mMath, "M_1_PI", rb_float_new(M_2_PI));
  rb_define_const(rb_mMath, "M_LN10", rb_float_new(M_LN10));
  rb_define_const(rb_mMath, "M_LN2", rb_float_new(M_LN2));
  rb_define_const(rb_mMath, "M_LNPI", rb_float_new(M_LNPI));
  rb_define_const(rb_mMath, "M_EULER", rb_float_new(M_EULER));
}

/* vim: set ts=4: */
