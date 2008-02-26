#!/bin/env ruby

# $Id: monte.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
include Math
include GSL::MonteCarlo

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

# toy example, pass parameters as a Struct

#struct = Struct.new("Params", :a, :b, :c)
#params = struct.new(1, 2, 3)

#p1 = Proc.new {|x, p|
#  p.a * x[0] * x[0] + p.b * x[0] * x[1] + p.c * x[1] * x[1]
#}

#x = [1, 2, 1]
#dim = 3

#f = Function.new dim, params, p1
#y = f.eval x, params

# now a real example (used to crash sometimes)

p2 = Proc.new {|k, p|
  p / (1.0 - cos(k[0]) * cos(k[1]) * cos(k[2]))
}

dim = 3
A = 1.0 / (M_PI * M_PI * M_PI)
f = Function.new dim, A, p2

xl = [0.0, 0.0, 0.0]
xu = [M_PI, M_PI, M_PI]
rng = Random::RNG.new

puts
plain = Plain.new dim
calls = 500000
r = plain.integrate f, xl, xu, calls, rng
display "Plain", r

puts
miser = Miser.new dim

#puts miser.estimate_frac
#puts miser.min_calls
#puts miser.min_calls_per_bisection
#puts miser.alpha
#puts miser.dither

calls = 500000
r = miser.integrate f, xl, xu, calls, rng
display "Miser", r

puts
vegas = Vegas.new dim
calls = 10000
r = vegas.integrate f, xl, xu, calls, rng
display "Vegas warmup", r
puts "converging..."
calls = 100000
while (vegas.chisq - 1.0).abs > 0.5 do
  r = vegas.integrate f, xl, xu, calls, rng
  printf "result = % .6f sigma = % .6f chisq/dof = %.1f\n",
    r[0], r[1], vegas.chisq
end
display "Vegas final", r

STDERR.puts "\ndone."
