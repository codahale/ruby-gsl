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

/* $Id: Stats.c,v 1.4 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_statistics.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Statistics */

/* Internal use only */

typedef double (* fun_t)(const double *, const size_t, const size_t);
typedef double (* fun_w_t)(const double *, const size_t, const double *, const size_t, const size_t);
typedef double (* fun_m_t)(const double *, const size_t, const size_t, double);
typedef double (* fun_w_m_t)(const double *, const size_t, const double *, const size_t, const size_t, double);
typedef double (* fun_m_sd_t)(const double *, const size_t, const size_t, double, double);
typedef double (* fun_w_m_sd_t)(const double *, const size_t, const double *, const size_t, const size_t, double, double);

static VALUE call(fun_t fun, VALUE data, VALUE vstride) {
  double * my_data;
  size_t size, stride;
  COPYRUBYARRAY(data, my_data);
  stride = NUM2INT(vstride);
  size = (RARRAY(data)->len)/stride;
  return rb_float_new((*fun)(my_data, stride, size));
}

static VALUE call_w(fun_w_t fun, VALUE w, VALUE vwstride, VALUE data, 
  VALUE vstride) {
  double * my_data, * my_w;
  size_t size, stride, wstride;
  COPYRUBYARRAY(data, my_data);
  COPYRUBYARRAY(w, my_w);
  stride = NUM2INT(vstride);
  wstride = NUM2INT(vwstride);
  size = (RARRAY(data)->len)/stride;
  return rb_float_new((*fun)(my_w, wstride, my_data, stride, size));
}

static VALUE call_m(fun_m_t fun, VALUE data, VALUE vstride, VALUE mean) {
  double * my_data;
  size_t size, stride;
  COPYRUBYARRAY(data, my_data);
  stride = NUM2INT(vstride);
  size = (RARRAY(data)->len)/stride;
  return rb_float_new((*fun)(my_data, stride, size, NUM2DBL(mean)));
}

static VALUE call_w_m(fun_w_m_t fun, VALUE w, VALUE vwstride, VALUE data, 
  VALUE vstride, VALUE mean) {
  double * my_data, * my_w;
  size_t size, stride, wstride;
  COPYRUBYARRAY(data, my_data);
  COPYRUBYARRAY(w, my_w);
  stride = NUM2INT(vstride);
  wstride = NUM2INT(vwstride);
  size = (RARRAY(data)->len)/stride;
  return rb_float_new((*fun)(my_w, wstride, my_data, stride, 
    size, NUM2DBL(mean)));
}

static VALUE call_m_sd(fun_m_sd_t fun, VALUE data, VALUE vstride, VALUE mean, 
  VALUE sd) {
  double * my_data;
  size_t size, stride;
  COPYRUBYARRAY(data, my_data);
  stride = NUM2INT(vstride);
  size = (RARRAY(data)->len)/stride;
  return rb_float_new((*fun)(my_data, stride, size,
    NUM2DBL(mean), NUM2DBL(sd)));
}

static VALUE call_w_m_sd(fun_w_m_sd_t fun, VALUE w, VALUE vwstride, VALUE data, 
  VALUE vstride, VALUE mean, VALUE sd) {
  double * my_data, * my_w;
  size_t size, stride, wstride;
  COPYRUBYARRAY(data, my_data);
  COPYRUBYARRAY(w, my_w);
  stride = NUM2INT(vstride);
  wstride = NUM2INT(vwstride);
  size = (RARRAY(data)->len)/stride;
  return rb_float_new((*fun)(my_w, wstride, my_data,
    stride, size, NUM2DBL(mean), NUM2DBL(sd)));
}

/* Mean, Standard Deviation, and Variance */

static VALUE Stats_mean(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_mean, data, stride);
}

static VALUE Stats_mean1(VALUE self, VALUE data) {
  return call(gsl_stats_mean, data, INT2FIX(1));
}

static VALUE Stats_variance(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_variance, data, stride);
}

static VALUE Stats_variance1(VALUE self, VALUE data) {
  return call(gsl_stats_variance, data, INT2FIX(1));
}

static VALUE Stats_sd(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_sd, data, stride);
}

static VALUE Stats_sd1(VALUE self, VALUE data) {
  return call(gsl_stats_sd, data, INT2FIX(1));
}

static VALUE Stats_variance_m(VALUE self, VALUE data, VALUE stride, VALUE mean) {
  return call_m(gsl_stats_variance_m, data, stride, mean);
}

static VALUE Stats_variance1_m(VALUE self, VALUE data, VALUE mean) {
  return call_m(gsl_stats_variance_m, data, INT2FIX(1), mean);
}

static VALUE Stats_variance_with_fixed_mean(VALUE self, VALUE data, 
  VALUE stride, VALUE mean) {
  return call_m(gsl_stats_variance_with_fixed_mean, data, stride, mean);
}

static VALUE Stats_variance1_with_fixed_mean(VALUE self, VALUE data, 
  VALUE mean) {
  return call_m(gsl_stats_variance_with_fixed_mean, data, INT2FIX(1), mean);
}

static VALUE Stats_sd_m(VALUE self, VALUE data, VALUE stride, VALUE mean) {
  return call_m(gsl_stats_sd_m, data, stride, mean);
}

static VALUE Stats_sd1_m(VALUE self, VALUE data, VALUE mean) {
  return call_m(gsl_stats_sd_m, data, INT2FIX(1), mean);
}

static VALUE Stats_sd_with_fixed_mean(VALUE self, VALUE data, 
  VALUE stride, VALUE mean) {
  return call_m(gsl_stats_sd_with_fixed_mean, data, stride, mean);
}

static VALUE Stats_sd1_with_fixed_mean(VALUE self, VALUE data, 
  VALUE mean) {
  return call_m(gsl_stats_sd_with_fixed_mean, data, INT2FIX(1), mean);
}

/* Absolute Deviation */

static VALUE Stats_absdev(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_absdev, data, stride);
}

static VALUE Stats_absdev_m(VALUE self, VALUE data, VALUE stride, VALUE mean) {
  return call_m(gsl_stats_absdev_m, data, stride, mean);
}

/* Higher Moments */

static VALUE Stats_skew(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_skew, data, stride);
}

static VALUE Stats_kurtosis(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_kurtosis, data, stride);
}

static VALUE Stats_skew_m_sd(VALUE self, VALUE data, VALUE stride, 
  VALUE mean, VALUE sd) {
  return call_m_sd(gsl_stats_skew_m_sd, data, stride, mean, sd);
}

static VALUE Stats_kurtosis_m_sd(VALUE self, VALUE data, VALUE stride, 
  VALUE mean, VALUE sd) {
  return call_m_sd(gsl_stats_kurtosis_m_sd, data, stride, mean, sd);
}

/* Autocorrelation */

static VALUE Stats_lag1_autocorrelation(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_lag1_autocorrelation, data, stride);
}

static VALUE Stats_lag1_autocorrelation_m(VALUE self, VALUE data, VALUE stride,
  VALUE m) {
  return call_m(gsl_stats_lag1_autocorrelation_m, data, stride, m);
}

/* Covariance */

static VALUE Stats_covariance(VALUE self, VALUE data1, VALUE stride1, 
  VALUE data2, VALUE stride2) {
  double * my_data1, * my_data2;
  if (RARRAY(data1)->len != RARRAY(data2)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  COPYRUBYARRAY(data1, my_data1);
  COPYRUBYARRAY(data2, my_data2);
  return rb_float_new(gsl_stats_covariance(my_data1, NUM2INT(stride1),
    my_data2, NUM2INT(stride2), RARRAY(data1)->len));
}

static VALUE Stats_covariance_m(VALUE self, VALUE data1, VALUE stride1, 
  VALUE data2, VALUE stride2, VALUE mean1, VALUE mean2) {
  double * my_data1, * my_data2;
  if (RARRAY(data1)->len != RARRAY(data2)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size!");
  COPYRUBYARRAY(data1, my_data1);
  COPYRUBYARRAY(data2, my_data2);
  return rb_float_new(gsl_stats_covariance_m(my_data1, NUM2INT(stride1), 
    my_data2, NUM2INT(stride2), RARRAY(data1)->len, NUM2DBL(mean1),
    NUM2DBL(mean2)));
}

/* Weighted Samples */

static VALUE Stats_wmean(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride) {
  return call_w(gsl_stats_wmean, w, wstride, data, stride);
}

static VALUE Stats_wvariance(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride) {
  return call_w(gsl_stats_wvariance, w, wstride, data, stride);
}

static VALUE Stats_wsd(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride) {
  return call_w(gsl_stats_wsd, w, wstride, data, stride);
}

static VALUE Stats_wabsdev(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride) {
  return call_w(gsl_stats_wabsdev, w, wstride, data, stride);
}

static VALUE Stats_wskew(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride) {
  return call_w(gsl_stats_wskew, w, wstride, data, stride);
}

static VALUE Stats_wkurtosis(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride) {
  return call_w(gsl_stats_wkurtosis, w, wstride, data, stride);
}

static VALUE Stats_wvariance_with_fixed_mean(VALUE self, VALUE w, 
  VALUE wstride, VALUE data, VALUE stride, VALUE mean) {
  return call_w_m(gsl_stats_wvariance_with_fixed_mean, w, wstride, data, stride, mean);
}

static VALUE Stats_wsd_with_fixed_mean(VALUE self, VALUE w, 
  VALUE wstride, VALUE data, VALUE stride, VALUE mean) {
  return call_w_m(gsl_stats_wsd_with_fixed_mean, w, wstride, data, stride, mean);
}

static VALUE Stats_wvariance_m(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride, VALUE wmean) {
  return call_w_m(gsl_stats_wvariance_m, w, wstride, data, stride, wmean);
}

static VALUE Stats_wsd_m(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride, VALUE wmean) {
  return call_w_m(gsl_stats_wsd_m, w, wstride, data, stride, wmean);
}

static VALUE Stats_wabsdev_m(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride, VALUE wmean) {
  return call_w_m(gsl_stats_wabsdev_m, w, wstride, data, stride, wmean);
}

static VALUE Stats_wskew_m_sd(VALUE self, VALUE w, VALUE wstride, VALUE data, 
  VALUE stride, VALUE wmean, VALUE sd) {
  return call_w_m_sd(gsl_stats_wskew_m_sd, w, wstride, data, stride, wmean, sd);
}

static VALUE Stats_wkurtosis_m_sd(VALUE self, VALUE w, VALUE wstride, 
  VALUE data, VALUE stride, VALUE wmean, VALUE sd) {
  return call_w_m_sd(gsl_stats_wkurtosis_m_sd, w, wstride, data, stride, wmean, sd);
}

/* Maximum and Minimum Values */

static VALUE Stats_max(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_max, data, stride);
}

static VALUE Stats_min(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_min, data, stride);
}

static VALUE Stats_minmax(VALUE self, VALUE data, VALUE stride) {
  double * my_data;
  double max, min;
  VALUE ary;
  COPYRUBYARRAY(data, my_data);
  gsl_stats_minmax(&min, &max, my_data, NUM2INT(stride), RARRAY(data)->len);
  ary = rb_ary_new3(2, rb_float_new(min), rb_float_new(max));
  return ary;  
}

static VALUE Stats_max_index(VALUE self, VALUE data, VALUE stride) {
  double * my_data;
  COPYRUBYARRAY(data, my_data);
  return INT2FIX(gsl_stats_max_index(my_data, NUM2INT(stride), RARRAY(data)->len));
}

static VALUE Stats_min_index(VALUE self, VALUE data, VALUE stride) {
  double * my_data;
  COPYRUBYARRAY(data, my_data);
  return INT2FIX(gsl_stats_min_index(my_data, NUM2INT(stride), RARRAY(data)->len));
}

static VALUE Stats_minmax_index(VALUE self, VALUE data, VALUE stride) {
  double * my_data;
  size_t min, max;
  VALUE ary;
  COPYRUBYARRAY(data, my_data);
  gsl_stats_minmax_index(&min, &max, my_data, NUM2INT(stride), RARRAY(data)->len);
  ary = rb_ary_new3(2, INT2FIX(min), INT2FIX(max));
  return ary;
}

/* Median and Percentiles */

static VALUE Stats_median(VALUE self, VALUE data, VALUE stride) {
  return call(gsl_stats_median_from_sorted_data, 
    rb_funcall(data, rb_intern("sort"), 0, 0), stride);
}

static VALUE Stats_median_from_sorted_data(VALUE self, VALUE data, 
  VALUE stride) {
  return call(gsl_stats_median_from_sorted_data, data, stride);
}

static VALUE Stats_quantile(VALUE self, VALUE data, VALUE stride, VALUE f) {
  return call_m(gsl_stats_quantile_from_sorted_data, 
    rb_funcall(data, rb_intern("sort"), 0, 0), stride, f);
}

static VALUE Stats_quantile_from_sorted_data(VALUE self, VALUE data, 
  VALUE stride, VALUE f) {
  return call_m(gsl_stats_quantile_from_sorted_data, data, stride, f);
}

/* Miscellaneous */

static VALUE Stats_pvariance(VALUE self, VALUE data1, VALUE stride1, 
  VALUE data2, VALUE stride2) {
  double * my_data1, * my_data2;
  COPYRUBYARRAY(data1, my_data1);
  COPYRUBYARRAY(data2, my_data2);
  return rb_float_new(gsl_stats_pvariance(my_data1, NUM2INT(stride1),
    RARRAY(data1)->len, my_data2, NUM2INT(stride2), RARRAY(data2)->len));
}

static VALUE Stats_ttest(VALUE self, VALUE data1, VALUE stride1, VALUE data2, 
  VALUE stride2) {
  double * my_data1, * my_data2;
  COPYRUBYARRAY(data1, my_data1);
  COPYRUBYARRAY(data2, my_data2);
  return rb_float_new(gsl_stats_ttest(my_data1, NUM2INT(stride1), 
    RARRAY(data1)->len, my_data2, NUM2INT(stride2), RARRAY(data2)->len));
}

/* Module definition */

VALUE rbgsl_mStats;

void Init_Stats() {
  rbgsl_mStats = rb_define_module_under(rbgsl_mGSL, "Stats");

  rb_define_module_function(rbgsl_mStats, "mean", Stats_mean, 2);
  rb_define_module_function(rbgsl_mStats, "mean1", Stats_mean1, 1);
  rb_define_module_function(rbgsl_mStats, "variance", Stats_variance, 2);
  rb_define_module_function(rbgsl_mStats, "variance1", Stats_variance1, 1);
  rb_define_module_function(rbgsl_mStats, "variance1_m", Stats_variance1_m, 2);
  rb_define_module_function(rbgsl_mStats, "variance_m", Stats_variance_m, 3);
  rb_define_module_function(rbgsl_mStats, "variance_with_fixed_mean", Stats_variance_with_fixed_mean, 3);
  rb_define_module_function(rbgsl_mStats, "variance1_with_fixed_mean", Stats_variance1_with_fixed_mean, 2);
  rb_define_module_function(rbgsl_mStats, "sd", Stats_sd, 2);
  rb_define_module_function(rbgsl_mStats, "sd1", Stats_sd1, 1);
  rb_define_module_function(rbgsl_mStats, "sd_m", Stats_sd_m, 3);
  rb_define_module_function(rbgsl_mStats, "sd1_m", Stats_sd1_m, 2);
  rb_define_module_function(rbgsl_mStats, "sd_with_fixed_mean", Stats_sd_with_fixed_mean, 3);
  rb_define_module_function(rbgsl_mStats, "sd1_with_fixed_mean", Stats_sd1_with_fixed_mean, 2);

  rb_define_module_function(rbgsl_mStats, "absdev", Stats_absdev, 2);
  rb_define_module_function(rbgsl_mStats, "absdev_m", Stats_absdev_m, 3);
  rb_define_module_function(rbgsl_mStats, "skew", Stats_skew, 2);
  rb_define_module_function(rbgsl_mStats, "skew_m_sd", Stats_skew_m_sd, 4);
  rb_define_module_function(rbgsl_mStats, "kurtosis", Stats_kurtosis, 2);
  rb_define_module_function(rbgsl_mStats, "kurtosis_m_sd", Stats_kurtosis_m_sd, 4);

  rb_define_module_function(rbgsl_mStats, "lag1_autocorrelation", Stats_lag1_autocorrelation, 2);
  rb_define_module_function(rbgsl_mStats, "lag1_autocorrelation_m", Stats_lag1_autocorrelation_m, 3);

  rb_define_module_function(rbgsl_mStats, "covariance", Stats_covariance, 4);
  rb_define_module_function(rbgsl_mStats, "covariance_m", Stats_covariance_m, 6);

  rb_define_module_function(rbgsl_mStats, "wmean", Stats_wmean, 4);
  rb_define_module_function(rbgsl_mStats, "wvariance", Stats_wvariance, 4);
  rb_define_module_function(rbgsl_mStats, "wvariance_with_fixed_mean", Stats_wvariance_with_fixed_mean, 5);
  rb_define_module_function(rbgsl_mStats, "wvariance_m", Stats_wvariance_m, 5);
  rb_define_module_function(rbgsl_mStats, "wsd", Stats_wsd, 4);
  rb_define_module_function(rbgsl_mStats, "wsd_m", Stats_wsd_m, 5);
  rb_define_module_function(rbgsl_mStats, "wsd_with_fixed_mean", Stats_wsd_with_fixed_mean, 5);
  rb_define_module_function(rbgsl_mStats, "wabsdev", Stats_wabsdev, 4);
  rb_define_module_function(rbgsl_mStats, "wabsdev_m", Stats_wabsdev_m, 5);
  rb_define_module_function(rbgsl_mStats, "wskew", Stats_wskew, 4);
  rb_define_module_function(rbgsl_mStats, "wskew_m_sd", Stats_wskew_m_sd, 6);
  rb_define_module_function(rbgsl_mStats, "wkurtosis", Stats_wkurtosis, 4);
  rb_define_module_function(rbgsl_mStats, "wkurtosis_m_sd", Stats_wkurtosis_m_sd, 6);

  rb_define_module_function(rbgsl_mStats, "max", Stats_max, 2);
  rb_define_module_function(rbgsl_mStats, "min", Stats_min, 2);
  rb_define_module_function(rbgsl_mStats, "minmax", Stats_minmax, 2);
  rb_define_module_function(rbgsl_mStats, "max_index", Stats_max_index, 2);
  rb_define_module_function(rbgsl_mStats, "min_index", Stats_min_index, 2);
  rb_define_module_function(rbgsl_mStats, "minmax_index", Stats_minmax_index, 2);

  rb_define_module_function(rbgsl_mStats, "median", Stats_median, 2);
  rb_define_module_function(rbgsl_mStats, "median_from_sorted_data", Stats_median_from_sorted_data, 2);
  rb_define_module_function(rbgsl_mStats, "quantile", Stats_quantile, 3);
  rb_define_module_function(rbgsl_mStats, "quantile_from_sorted_data", Stats_quantile_from_sorted_data, 3);

  rb_define_module_function(rbgsl_mStats, "pvariance", Stats_pvariance, 4);
  rb_define_module_function(rbgsl_mStats, "ttest", Stats_ttest, 4);
}

/* vim: set ts=4: */
