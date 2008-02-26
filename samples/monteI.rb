#!/bin/env ruby
# $Id: monteI.rb,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
include Math
# the following files can be found in the contrib folder
require "assert"
require "MInline"

puts "First test"
MInline::compile(%Q{
#include <math.h>

struct my_f_params { double a; double b; double c; };

static double g1x (double * x, size_t dim, void * p) {
    struct my_f_params * fp = (struct my_f_params *)p;
    if (dim != 2) {
      fprintf(stderr, "error: dim != 2");
      abort();
    }

   return  fp->a * x[0]  + fp->b * x[0] * x[1];
}

   struct my_f_params params = { 1.0, 2.0, 3.0 };
   static gsl_monte_function MC_FNCT  = {&g1x, 2, &params};

 }, "qua")

include GSL::MonteCarlo_qua
dim2 = 2
 
plain2 = Plain.new(dim2)
xl2 = [0,0]; xu2 = [1,1];  calls2 = 100000
rng2 = Random::RNG.new
p plain2.integrate(xl2, xu2, calls2, rng2)

puts "Second test"
STDERR.puts "Running tests for Monte Carlo integration..."

def display(msg, r)
  result = r[0]
  abserr = r[1]
  exact = 1.3932039296856768591842462603255
  puts msg
  printf "result = % .6f\n", result
  printf "sigma  = % .6f\n", abserr
  printf "exact  = % .6f\n", exact
  error = result - exact
  printf "error  = % .6f = %.1g sigma\n", error, error.abs / abserr
end

MInline::compile("
#include <math.h>
static double
g1x (double *k, size_t dim, void *params)
{
  double A = 1.0 / (M_PI * M_PI * M_PI);
  return A / (1.0 - cos (k[0]) * cos (k[1]) * cos (k[2]));
}

static gsl_monte_function MC_FNCT = { &g1x, 3, 0 };
", "monte2")


include GSL::MonteCarlo_monte2
dim = 3

xl = [0.0, 0.0, 0.0]
xu = [ M_PI, M_PI, M_PI]
rng = Random::RNG.new

puts
plain = Plain.new dim
calls = 500000
puts "calls = #{calls}"
r = plain.integrate xl, xu, calls, rng
display "Plain", r

puts
puts "calls = #{calls}"
miser = Miser.new dim


r = miser.integrate xl, xu, calls, rng
display "Miser", r

puts
vegas = Vegas.new dim
puts "calls = 10000"
r = vegas.integrate xl, xu, 10000, rng
display "Vegas warmup", r
puts "converging..."

calls = calls / 5
puts "calls = #{calls}"
while ((vegas.chisq - 1.0).abs > 0.5) do
  r = vegas.integrate xl, xu, calls, rng
  printf "result = % .6f sigma = % .6f chisq/dof = %.1f\n",
    r[0], r[1], vegas.chisq
end
display "Vegas final", r

STDERR.puts "\ndone."
