#!/bin/env ruby

# $Id: min.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test class Minimizer

STDERR.puts "Running minimization tests (minimizer)..."

func = Function.new { |x| Special::Trig::cos(x) + 1.0 }

[Minimizer::GOLDENSECTION, Minimizer::BRENT].each do |alg|

  status = GSL_CONTINUE
  iter = 0
  max_iter = 100
  m = 2.0
  m_exp = Math::M_PI
  a = 0.0
  b = 6.0
  y = func.eval m

  min = Minimizer.new alg
  min.set func, m, a, b

  puts "\nUsing #{min} method"

  printf "%5s [%9s, %9s] %9s %10s %9s\n", "iter", "lower", "upper", 
    "min", "y", "err", "err(est)"

  printf "%5d [%.7f, %.7f] %.7f %.7f% +.7f %.7f\n",
    iter, a, b, m, y, m - m_exp, b - a

  while status == GSL_CONTINUE && iter < max_iter
    iter += 1
    status = min.iterate
    m = min.x_minimum
    a = min.x_lower
    b = min.x_upper
    y = min.f_minimum
    status = Minimizer::test_interval(a, b, 0.0, 0.001)

    puts "Converged:" if status == GSL_SUCCESS

    printf "%5d [%.7f, %.7f] %.7f %.7f %+.7f %.7f\n",
      iter, a, b, m, y, m - m_exp, b - a
  end

end

STDERR.puts "\ndone."
