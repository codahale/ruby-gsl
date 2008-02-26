#!/bin/env ruby

# $Id: diff.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

STDERR.puts "Running tests for Numerical Differentiation..."

f = Function.new { |x| Math::pow(x, 1.5) }

puts "f(x) = x^(3/2)"

res, err = Diff::central(f, 2.0)
puts "x = 2.0"
printf "f'(x) = %.10f +/- %.5f\n", res, err
printf "exact = %.10f\n\n", 1.5 * Math::sqrt(2.0)

res, err = Diff::forward(f, 0.0)
puts "x = 0.0"
printf "f'(x) = %.10f +/- %.5f\n", res, err
printf "exact = %.10f\n\n", 0.0

STDERR.puts "\ndone."
