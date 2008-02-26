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

/* $Id: RND.c,v 1.6 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Random Number Distributions */

/* Gaussian */

static VALUE RND_gaussian(VALUE self, VALUE rng, VALUE sigma) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gaussian(ptr, NUM2DBL(sigma)));
}

static VALUE RND_gaussian_ratio_method(VALUE self, VALUE rng, VALUE sigma) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gaussian_ratio_method(ptr, NUM2DBL(sigma)));
}

static VALUE RND_gaussian_pdf(VALUE self, VALUE x, VALUE sigma) {
  return rb_float_new(gsl_ran_gaussian_pdf(NUM2DBL(x), NUM2DBL(sigma)));
}

static VALUE RND_ugaussian(VALUE self, VALUE rng) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_ugaussian(ptr));
}

static VALUE RND_ugaussian_ratio_method(VALUE self, VALUE rng) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_ugaussian_ratio_method(ptr));
}

static VALUE RND_ugaussian_pdf(VALUE self, VALUE x) {
  return rb_float_new(gsl_ran_ugaussian_pdf(NUM2DBL(x)));
}

/* Gaussian Tail */

static VALUE RND_gaussian_tail(VALUE self, VALUE rng, VALUE a, VALUE sigma) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gaussian_tail(ptr, NUM2DBL(a), NUM2DBL(sigma)));
}

static VALUE RND_gaussian_tail_pdf(VALUE self, VALUE x, VALUE a, VALUE sigma) {
  return rb_float_new(gsl_ran_gaussian_tail_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(sigma)));
}

static VALUE RND_ugaussian_tail(VALUE self, VALUE rng, VALUE a) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_ugaussian_tail(ptr, NUM2DBL(a)));
}

static VALUE RND_ugaussian_tail_pdf(VALUE self, VALUE x, VALUE a) {
  return rb_float_new(gsl_ran_ugaussian_tail_pdf(NUM2DBL(x), NUM2DBL(a)));
}

/* Bivariate Gaussian */

static VALUE RND_bivariate_gaussian(VALUE self, VALUE rng, VALUE sigma_x, 
  VALUE sigma_y, VALUE rho) {
  VALUE ary;
  double x, y;
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_bivariate_gaussian(ptr, NUM2DBL(sigma_x), NUM2DBL(sigma_y), 
    NUM2DBL(rho), &x, &y);
  ary = rb_ary_new3(2, rb_float_new(x), rb_float_new(y));
  return ary;
}

static VALUE RND_bivariate_gaussian_pdf(VALUE self, VALUE x, VALUE y, 
  VALUE sigma_x, VALUE sigma_y, VALUE rho) {
  return rb_float_new(gsl_ran_bivariate_gaussian_pdf(NUM2DBL(x), NUM2DBL(y), 
    NUM2DBL(sigma_x), NUM2DBL(sigma_y), NUM2DBL(rho)));
}

/* Exponential */

static VALUE RND_exponential(VALUE self, VALUE rng, VALUE mu) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_exponential(ptr, NUM2DBL(mu)));
}

static VALUE RND_exponential_pdf(VALUE self, VALUE x, VALUE mu) {
  return rb_float_new(gsl_ran_exponential_pdf(NUM2DBL(x), NUM2DBL(mu)));
}

/* Laplace */

static VALUE RND_laplace(VALUE self, VALUE rng, VALUE a) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_laplace(ptr, NUM2DBL(a)));
}

static VALUE RND_laplace_pdf(VALUE self, VALUE x, VALUE a) {
  return rb_float_new(gsl_ran_laplace_pdf(NUM2DBL(x), NUM2DBL(a)));
}

/* Exponential Power */

static VALUE RND_exppow(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_exppow(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_exppow_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_exppow_pdf(NUM2DBL(x), NUM2DBL(a), NUM2DBL(b)));
}

/* Cauchy */

static VALUE RND_cauchy(VALUE self, VALUE rng, VALUE a) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_cauchy(ptr, NUM2DBL(a)));
}

static VALUE RND_cauchy_pdf(VALUE self, VALUE x, VALUE a) {
  return rb_float_new(gsl_ran_cauchy_pdf(NUM2DBL(x), NUM2DBL(a)));
}

/* Rayleigh */

static VALUE RND_rayleigh(VALUE self, VALUE rng, VALUE sigma) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_rayleigh(ptr, NUM2DBL(sigma)));
}

static VALUE RND_rayleigh_pdf(VALUE self, VALUE x, VALUE sigma) {
  return rb_float_new(gsl_ran_rayleigh_pdf(NUM2DBL(x), NUM2DBL(sigma)));
}

/* Rayleigh Tail */

static VALUE RND_rayleigh_tail(VALUE self, VALUE rng, VALUE a, VALUE sigma) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_rayleigh_tail(ptr, NUM2DBL(a), NUM2DBL(sigma)));
}

static VALUE RND_rayleigh_tail_pdf(VALUE self, VALUE x, VALUE a, VALUE sigma) {
  return rb_float_new(gsl_ran_rayleigh_tail_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(sigma)));
}

/* Landau */

static VALUE RND_landau(VALUE self, VALUE rng) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_landau(ptr));
}

static VALUE RND_landau_pdf(VALUE self, VALUE x) {
  return rb_float_new(gsl_ran_landau_pdf(NUM2DBL(x)));
}

/* Levy alpha-Stable */

static VALUE RND_levy(VALUE self, VALUE rng, VALUE c, VALUE alpha) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_levy(ptr, NUM2DBL(c), NUM2DBL(alpha)));
}

/* Levy skew alpha-Stable */

static VALUE RND_levy_skew(VALUE self, VALUE rng, VALUE c, VALUE alpha, 
    VALUE beta) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_levy_skew(ptr, NUM2DBL(c), NUM2DBL(alpha), 
    NUM2DBL(beta)));
}

/* Gamma */

static VALUE RND_gamma(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gamma(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_gamma_int(VALUE self, VALUE rng, VALUE a) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gamma_int(ptr, NUM2DBL(a)));
}

static VALUE RND_gamma_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_gamma_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}

/* Flat (Uniform) */

static VALUE RND_flat(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_flat(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_flat_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_flat_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}

/* Lognormal */

static VALUE RND_lognormal(VALUE self, VALUE rng, VALUE zeta, VALUE sigma) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_lognormal(ptr, NUM2DBL(zeta), NUM2DBL(sigma)));
}

static VALUE RND_lognormal_pdf(VALUE self, VALUE x, VALUE zeta, VALUE sigma) {
  return rb_float_new(gsl_ran_lognormal_pdf(NUM2DBL(x), NUM2DBL(zeta), 
    NUM2DBL(sigma)));
}

/* Chi-squared */

static VALUE RND_chisq(VALUE self, VALUE rng, VALUE nu) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_chisq(ptr, NUM2DBL(nu)));
}

static VALUE RND_chisq_pdf(VALUE self, VALUE x, VALUE nu) {
  return rb_float_new(gsl_ran_chisq_pdf(NUM2DBL(x), NUM2DBL(nu)));
}

/* F */

static VALUE RND_fdist(VALUE self, VALUE rng, VALUE nu1, VALUE nu2) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_fdist(ptr, NUM2DBL(nu1), NUM2DBL(nu2)));
}

static VALUE RND_fdist_pdf(VALUE self, VALUE x, VALUE nu1, VALUE nu2) {
  return rb_float_new(gsl_ran_fdist_pdf(NUM2DBL(x), NUM2DBL(nu1), 
    NUM2DBL(nu2)));
}

/* t */

static VALUE RND_tdist(VALUE self, VALUE rng, VALUE nu) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_tdist(ptr, NUM2DBL(nu)));
}

static VALUE RND_tdist_pdf(VALUE self, VALUE x, VALUE nu) {
  return rb_float_new(gsl_ran_tdist_pdf(NUM2DBL(x), NUM2DBL(nu)));
}

/* Beta */

static VALUE RND_beta(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_beta(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_beta_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_beta_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}

/* Logistic */

static VALUE RND_logistic(VALUE self, VALUE rng, VALUE a) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_logistic(ptr, NUM2DBL(a)));
}

static VALUE RND_logistic_pdf(VALUE self, VALUE x, VALUE a) {
  return rb_float_new(gsl_ran_logistic_pdf(NUM2DBL(x), NUM2DBL(a)));
}

/* Pareto */

static VALUE RND_pareto(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_pareto(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_pareto_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_pareto_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}

/* Spherical Vector */

static VALUE RND_dir_2d(VALUE self, VALUE rng) {
  VALUE ary;
  double x, y;
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_dir_2d(ptr, &x, &y);
  ary = rb_ary_new3(2, rb_float_new(x), rb_float_new(y));
  return ary;
}

static VALUE RND_dir_2d_trig_method(VALUE self, VALUE rng) {
  VALUE ary;
  double x, y;
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_dir_2d_trig_method(ptr, &x, &y);
  ary = rb_ary_new3(2, rb_float_new(x), rb_float_new(y));
  return ary;
}

static VALUE RND_dir_3d(VALUE self, VALUE rng) {
  VALUE ary;
  double x, y, z;
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_dir_3d(ptr, &x, &y, &z);
  ary = rb_ary_new3(3, rb_float_new(x), rb_float_new(y), rb_float_new(z));
  return ary;
}

static VALUE RND_dir_nd(VALUE self, VALUE rng, VALUE n) {
  VALUE ary;
  double * x;
  int dim = NUM2INT(n);
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  x = (double *) ALLOCA_N(double, dim);
  gsl_ran_dir_nd(ptr, dim, x);
  COPYCARRAY(x, ary, dim);
  return ary;
}

/* Weibull */

static VALUE RND_weibull(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_weibull(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_weibull_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_weibull_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}

/* Type-1 Gumbel */

static VALUE RND_gumbel1(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gumbel1(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_gumbel1_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_gumbel1_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}


/* Type-2 Gumbel */

static VALUE RND_gumbel2(VALUE self, VALUE rng, VALUE a, VALUE b) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_gumbel2(ptr, NUM2DBL(a), NUM2DBL(b)));
}

static VALUE RND_gumbel2_pdf(VALUE self, VALUE x, VALUE a, VALUE b) {
  return rb_float_new(gsl_ran_gumbel2_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(b)));
}

/* Erlang */

static VALUE RND_erlang(VALUE self, VALUE rng, VALUE a, VALUE n) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return rb_float_new(gsl_ran_erlang(ptr, NUM2DBL(a), NUM2DBL(n)));
}

static VALUE RND_erlang_pdf(VALUE self, VALUE x, VALUE a, VALUE n) {
  return rb_float_new(gsl_ran_erlang_pdf(NUM2DBL(x), NUM2DBL(a), 
    NUM2DBL(n)));
}

/* Poisson */

static VALUE RND_poisson(VALUE self, VALUE rng, VALUE mu) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_poisson(ptr, NUM2DBL(mu)));
}

static VALUE RND_poisson_pdf(VALUE self, VALUE k, VALUE mu) {
  return rb_float_new(gsl_ran_poisson_pdf(NUM2UINT(k), NUM2DBL(mu)));
}

static VALUE RND_poisson_array(VALUE self, VALUE rng, VALUE n, VALUE mu) {
  gsl_rng * ptr;
  unsigned int * array;
  VALUE to;
  size_t i;
  size_t size;
  
  size = NUM2INT(n);
  array = (unsigned int *) ALLOCA_N(unsigned int, size);
  to = rb_ary_new2(size);
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_poisson_array(ptr, size, array, NUM2DBL(mu));
  for (i = 0; i < size; i++)
    RARRAY(to)->ptr[i] = UINT2NUM(array[i]);
  RARRAY(to)->len = size;
  return to;
}

/* Bernoulli */

static VALUE RND_bernoulli(VALUE self, VALUE rng, VALUE p) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_bernoulli(ptr, NUM2DBL(p)));
}

static VALUE RND_bernoulli_pdf(VALUE self, VALUE k, VALUE p) {
  return rb_float_new(gsl_ran_bernoulli_pdf(NUM2UINT(k), NUM2DBL(p)));
}

/* Binomial */

static VALUE RND_binomial(VALUE self, VALUE rng, VALUE p, VALUE n) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_binomial(ptr, NUM2DBL(p), NUM2UINT(n)));
}

static VALUE RND_binomial_pdf(VALUE self, VALUE k, VALUE p, VALUE n) {
  return rb_float_new(gsl_ran_binomial_pdf(NUM2UINT(k), NUM2DBL(p), NUM2UINT(n)));
}

/* Negative Binomial */

static VALUE RND_negative_binomial(VALUE self, VALUE rng, VALUE p, VALUE n) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_negative_binomial(ptr, NUM2DBL(p), NUM2DBL(n)));
}

static VALUE RND_negative_binomial_pdf(VALUE self, VALUE k, VALUE p, VALUE n) {
  return rb_float_new(gsl_ran_negative_binomial_pdf(NUM2UINT(k), NUM2DBL(p), NUM2DBL(n)));
}

/* Pascal */

static VALUE RND_pascal(VALUE self, VALUE rng, VALUE p, VALUE n) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_pascal(ptr, NUM2DBL(p), NUM2UINT(n)));
}

static VALUE RND_pascal_pdf(VALUE self, VALUE k, VALUE p, VALUE n) {
  return rb_float_new(gsl_ran_pascal_pdf(NUM2UINT(k), NUM2DBL(p), NUM2UINT(n)));
}

/* Geometric */

static VALUE RND_geometric(VALUE self, VALUE rng, VALUE p) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_geometric(ptr, NUM2DBL(p)));
}

static VALUE RND_geometric_pdf(VALUE self, VALUE k, VALUE p) {
  return rb_float_new(gsl_ran_geometric_pdf(NUM2UINT(k), NUM2DBL(p)));
}

/* Hypergeometric */

static VALUE RND_hypergeometric(VALUE self, VALUE rng, VALUE n1, VALUE n2,
  VALUE t) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_hypergeometric(ptr, NUM2UINT(n1), NUM2UINT(n2),
    NUM2UINT(t)));
}

static VALUE RND_hypergeometric_pdf(VALUE self, VALUE k, VALUE n1, VALUE n2,
  VALUE t) {
  return rb_float_new(gsl_ran_hypergeometric_pdf(NUM2UINT(k), NUM2UINT(n1), 
    NUM2UINT(n2), NUM2UINT(t)));
}

/* Logarithmic */

static VALUE RND_logarithmic(VALUE self, VALUE rng, VALUE p) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  return UINT2NUM(gsl_ran_logarithmic(ptr, NUM2DBL(p)));
}

static VALUE RND_logarithmic_pdf(VALUE self, VALUE k, VALUE p) {
  return rb_float_new(gsl_ran_logarithmic_pdf(NUM2UINT(k), NUM2DBL(p)));
}

/* Dirichlet */

static VALUE RND_dirichlet(VALUE self, VALUE rng, VALUE k, VALUE alpha, VALUE theta) {
  gsl_rng * ptr;
  double * my_alpha, * my_theta;
  size_t n, i;
  Data_Get_Struct(rng, gsl_rng, ptr);
  COPYRUBYARRAY(alpha, my_alpha);
  COPYRUBYARRAY(theta, my_theta);
  n = RARRAY(theta)->len;
  gsl_ran_dirichlet(ptr, NUM2INT(k), my_alpha, my_theta);
  for(i=0; i < n; i++)
    RARRAY(theta)->ptr[i] = rb_float_new(my_theta[i]);
  
  return self;
}

static VALUE RND_dirichlet_pdf(VALUE self, VALUE k, VALUE alpha, VALUE theta) {
  double * my_alpha, * my_theta;
  COPYRUBYARRAY(alpha, my_alpha);
  COPYRUBYARRAY(theta, my_theta);
  return rb_float_new(gsl_ran_dirichlet_pdf(NUM2INT(k), my_alpha, my_theta));
}

static VALUE RND_dirichlet_lnpdf(VALUE self, VALUE k, VALUE alpha, VALUE theta) {
  double * my_alpha, * my_theta;
  COPYRUBYARRAY(alpha, my_alpha);
  COPYRUBYARRAY(theta, my_theta);
  return rb_float_new(gsl_ran_dirichlet_lnpdf(NUM2INT(k), my_alpha, my_theta));
}

/* Multinomial */

static VALUE RND_multinomial(VALUE self, VALUE rng, VALUE k, VALUE nn, VALUE p, VALUE n) {
  gsl_rng * ptr;
  double * my_p;
  unsigned int * my_n;
  long i;
  Data_Get_Struct(rng, gsl_rng, ptr);
  COPYRUBYARRAY(p, my_p);
  my_n = (unsigned int *) ALLOCA_N(unsigned int, RARRAY(n)->len);
  for (i = 0; i < RARRAY(n)->len; i++)
    my_n[i] = NUM2UINT(RARRAY(n)->ptr[i]);
  gsl_ran_multinomial(ptr, NUM2INT(k), NUM2UINT(nn), my_p, my_n);
  for (i = 0; i < RARRAY(n)->len; i++)
  	RARRAY(n)->ptr[i] = INT2FIX(my_n[i]);
  return self;
}

static VALUE RND_multinomial_pdf(VALUE self, VALUE k, VALUE p, VALUE n) {
  double * my_p;
  unsigned int * my_n;
  long i;
  COPYRUBYARRAY(p, my_p);
  my_n = (unsigned int *) ALLOCA_N(unsigned int, RARRAY(n)->len);
  for (i = 0; i < RARRAY(n)->len; i++)
    my_n[i] = NUM2UINT(RARRAY(n)->ptr[i]);
  return rb_float_new(gsl_ran_multinomial_pdf(NUM2INT(k), my_p, my_n));
}

static VALUE RND_multinomial_lnpdf(VALUE self, VALUE k, VALUE p, VALUE n) {
  double * my_p;
  unsigned int * my_n;
  long i;
  COPYRUBYARRAY(p, my_p);
  my_n = (unsigned int *) ALLOCA_N(unsigned int, RARRAY(n)->len);
  for (i = 0; i < RARRAY(n)->len; i++)
    my_n[i] = NUM2UINT(RARRAY(n)->ptr[i]);
  return rb_float_new(gsl_ran_multinomial_lnpdf(NUM2INT(k), my_p, my_n));
}

/* Shuffling and Sampling */

static VALUE RND_shuffle(VALUE self, VALUE rng, VALUE ary) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_shuffle(ptr, RARRAY(ary)->ptr, RARRAY(ary)->len, sizeof(VALUE));
  return self;
}

/* method for Array class */
static VALUE RND_shuffle2(VALUE self, VALUE rng) {
  gsl_rng * ptr;
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_shuffle(ptr, RARRAY(self)->ptr, RARRAY(self)->len, sizeof(VALUE));
  return self;
}

static VALUE RND_choose(VALUE self, VALUE rng, VALUE k, VALUE ary) {
  gsl_rng * ptr;
  int ret;
  size_t kk = NUM2INT(k);
  VALUE dest = rb_ary_new2(kk);
  Data_Get_Struct(rng, gsl_rng, ptr);
  ret = gsl_ran_choose(ptr, RARRAY(dest)->ptr, kk, RARRAY(ary)->ptr, 
    RARRAY(ary)->len, sizeof(VALUE));
  RARRAY(dest)->len = kk;
  if (ret != GSL_SUCCESS)
    rb_raise(rb_eStandardError, "Error choosing objects from array");
  return dest;
}

/* method for Array class */
static VALUE RND_choose2(VALUE self, VALUE rng, VALUE k) {
  gsl_rng * ptr;
  int ret;
  size_t kk = NUM2INT(k);
  VALUE dest = rb_ary_new2(kk);
  Data_Get_Struct(rng, gsl_rng, ptr);
  ret = gsl_ran_choose(ptr, RARRAY(dest)->ptr, kk, RARRAY(self)->ptr, 
    RARRAY(self)->len, sizeof(VALUE));
  RARRAY(dest)->len = kk;
  if (ret != GSL_SUCCESS)
    rb_raise(rb_eStandardError, "Error choosing objects from array");
  return dest;
}

static VALUE RND_sample(VALUE self, VALUE rng, VALUE k, VALUE ary) {
  gsl_rng * ptr;
  size_t kk = NUM2INT(k);
  VALUE dest = rb_ary_new2(kk);
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_sample(ptr, RARRAY(dest)->ptr, kk, RARRAY(ary)->ptr, 
    RARRAY(ary)->len, sizeof(VALUE));
  RARRAY(dest)->len = kk;
  return dest;
}

/* method for Array class */
static VALUE RND_sample2(VALUE self, VALUE rng, VALUE k) {
  gsl_rng * ptr;
  size_t kk = NUM2INT(k);
  VALUE dest = rb_ary_new2(kk);
  Data_Get_Struct(rng, gsl_rng, ptr);
  gsl_ran_sample(ptr, RARRAY(dest)->ptr, kk, RARRAY(self)->ptr, 
    RARRAY(self)->len, sizeof(VALUE));
  RARRAY(dest)->len = kk;
  return dest;
}

/* Module definition */

VALUE rbgsl_mRND;

/* TODO: Maybe all of this should be moved to module Random */

void Init_RND() {
  rbgsl_mRND = rb_define_module_under(rbgsl_mRandom, "RND");

  rb_define_module_function(rbgsl_mRND, "gaussian", RND_gaussian, 2);
  rb_define_module_function(rbgsl_mRND, "gaussian_ratio_method", RND_gaussian_ratio_method, 2);
  rb_define_module_function(rbgsl_mRND, "gaussian_pdf", RND_gaussian_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "ugaussian", RND_ugaussian, 1);
  rb_define_module_function(rbgsl_mRND, "ugaussian_ratio_method", RND_ugaussian_ratio_method, 1);
  rb_define_module_function(rbgsl_mRND, "ugaussian_pdf", RND_ugaussian_pdf, 1);

  rb_define_module_function(rbgsl_mRND, "gaussian_tail", RND_gaussian_tail, 3);
  rb_define_module_function(rbgsl_mRND, "gaussian_tail_pdf", RND_gaussian_tail_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "ugaussian_tail", RND_ugaussian_tail, 2);
  rb_define_module_function(rbgsl_mRND, "ugaussian_tail_pdf", RND_ugaussian_tail_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "bivariate_gaussian", RND_bivariate_gaussian, 4);
  rb_define_module_function(rbgsl_mRND, "bivariate_gaussian_pdf", RND_bivariate_gaussian_pdf, 5);

  rb_define_module_function(rbgsl_mRND, "exponential", RND_exponential, 2);
  rb_define_module_function(rbgsl_mRND, "exponential_pdf", RND_exponential_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "laplace", RND_laplace, 2);
  rb_define_module_function(rbgsl_mRND, "laplace_pdf", RND_laplace_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "exppow", RND_exppow, 3);
  rb_define_module_function(rbgsl_mRND, "exppow_pdf", RND_exppow_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "cauchy", RND_cauchy, 2);
  rb_define_module_function(rbgsl_mRND, "cauchy_pdf", RND_cauchy_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "rayleigh", RND_rayleigh, 2);
  rb_define_module_function(rbgsl_mRND, "rayleigh_pdf", RND_rayleigh_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "rayleigh_tail", RND_rayleigh_tail, 3);
  rb_define_module_function(rbgsl_mRND, "rayleigh_tail_pdf", RND_rayleigh_tail_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "landau", RND_landau, 1);
  rb_define_module_function(rbgsl_mRND, "landau_pdf", RND_landau_pdf, 1);

  rb_define_module_function(rbgsl_mRND, "levy", RND_levy, 3);
  rb_define_module_function(rbgsl_mRND, "levy_skew", RND_levy_skew, 4);

  rb_define_module_function(rbgsl_mRND, "gamma", RND_gamma, 3);
  rb_define_module_function(rbgsl_mRND, "gamma_int", RND_gamma_int, 2);
  rb_define_module_function(rbgsl_mRND, "gamma_pdf", RND_gamma_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "flat", RND_flat, 3);
  rb_define_module_function(rbgsl_mRND, "flat_pdf", RND_flat_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "lognormal", RND_lognormal, 3);
  rb_define_module_function(rbgsl_mRND, "lognormal_pdf", RND_lognormal_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "chisq", RND_chisq, 2);
  rb_define_module_function(rbgsl_mRND, "chisq_pdf", RND_chisq_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "fdist", RND_fdist, 3);
  rb_define_alias(rbgsl_mRND, "F", "fdist");
  rb_define_module_function(rbgsl_mRND, "fdist_pdf", RND_fdist_pdf, 3);
  rb_define_alias(rbgsl_mRND, "F_pdf", "fdist_pdf");

  rb_define_module_function(rbgsl_mRND, "tdist", RND_tdist, 2);
  rb_define_alias(rbgsl_mRND, "t", "tdist");
  rb_define_module_function(rbgsl_mRND, "tdist_pdf", RND_tdist_pdf, 2);
  rb_define_alias(rbgsl_mRND, "t_pdf", "tdist_pdf");

  rb_define_module_function(rbgsl_mRND, "beta", RND_beta, 3);
  rb_define_module_function(rbgsl_mRND, "beta_pdf", RND_beta_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "logistic", RND_logistic, 2);
  rb_define_module_function(rbgsl_mRND, "logistic_pdf", RND_logistic_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "pareto", RND_pareto, 3);
  rb_define_module_function(rbgsl_mRND, "pareto_pdf", RND_pareto_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "dir_2d", RND_dir_2d, 1);
  rb_define_module_function(rbgsl_mRND, "dir_2d_trig_method", RND_dir_2d_trig_method, 1);
  rb_define_module_function(rbgsl_mRND, "dir_3d", RND_dir_3d, 1);
  rb_define_module_function(rbgsl_mRND, "dir_nd", RND_dir_nd, 2);

  rb_define_module_function(rbgsl_mRND, "weibull", RND_weibull, 3);
  rb_define_module_function(rbgsl_mRND, "weibull_pdf", RND_weibull_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "gumbel1", RND_gumbel1, 3);
  rb_define_module_function(rbgsl_mRND, "gumbel1_pdf", RND_gumbel1_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "gumbel2", RND_gumbel2, 3);
  rb_define_module_function(rbgsl_mRND, "gumbel2_pdf", RND_gumbel2_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "erlang", RND_erlang, 3);
  rb_define_module_function(rbgsl_mRND, "erlang_pdf", RND_erlang_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "poisson", RND_poisson, 2);
  rb_define_module_function(rbgsl_mRND, "poisson_pdf", RND_poisson_pdf, 2);
  rb_define_module_function(rbgsl_mRND, "poisson_array", RND_poisson_array, 3);

  rb_define_module_function(rbgsl_mRND, "bernoulli", RND_bernoulli, 2);
  rb_define_module_function(rbgsl_mRND, "bernoulli_pdf", RND_bernoulli_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "binomial", RND_binomial, 3);
  rb_define_module_function(rbgsl_mRND, "binomial_pdf", RND_binomial_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "negative_binomial", RND_negative_binomial, 3);
  rb_define_module_function(rbgsl_mRND, "negative_binomial_pdf", RND_negative_binomial_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "pascal", RND_pascal, 3);
  rb_define_module_function(rbgsl_mRND, "pascal_pdf", RND_pascal_pdf, 3);

  rb_define_module_function(rbgsl_mRND, "geometric", RND_geometric, 2);
  rb_define_module_function(rbgsl_mRND, "geometric_pdf", RND_geometric_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "hypergeometric", RND_hypergeometric, 4);
  rb_define_module_function(rbgsl_mRND, "hypergeometric_pdf", RND_hypergeometric_pdf, 4);

  rb_define_module_function(rbgsl_mRND, "logarithmic", RND_logarithmic, 2);
  rb_define_module_function(rbgsl_mRND, "logarithmic_pdf", RND_logarithmic_pdf, 2);

  rb_define_module_function(rbgsl_mRND, "dirichlet", RND_dirichlet, 4);
  rb_define_module_function(rbgsl_mRND, "dirichlet_pdf", RND_dirichlet_pdf, 3);
  rb_define_module_function(rbgsl_mRND, "dirichlet_lnpdf", RND_dirichlet_lnpdf, 3);

  rb_define_module_function(rbgsl_mRND, "multinomial", RND_multinomial, 5);
  rb_define_module_function(rbgsl_mRND, "multinomial_pdf", RND_multinomial_pdf, 3);
  rb_define_module_function(rbgsl_mRND, "multinomial_lnpdf", RND_multinomial_lnpdf, 3);

  rb_define_module_function(rbgsl_mRND, "shuffle", RND_shuffle, 2);
  rb_define_method(rb_cArray, "shuffle", RND_shuffle2, 1);

  rb_define_module_function(rbgsl_mRND, "choose", RND_choose, 3);
  rb_define_method(rb_cArray, "choose", RND_choose2, 2);

  rb_define_module_function(rbgsl_mRND, "sample", RND_sample, 3);
  rb_define_method(rb_cArray, "sample", RND_sample2, 2);
}

/* vim: set ts=4: */
