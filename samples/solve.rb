#!/bin/env ruby

# $Id: solve.rb,v 1.2 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL

# Test module Solver

STDERR.puts "Running root-finding tests (fsolver)..."

a = 1
b = 0
c = -5
f = Function.new { |x| (a*x + b) * x + c }

[Solver::FSolver::BISECTION, Solver::FSolver::BRENT, Solver::FSolver::FALSEPOS].each do |m|
  
  status = GSL_CONTINUE
  iter = 0
  max_iter = 100

  x_lo = 0.0
  x_hi = 5.0
  r = 0.0
  r_exp = Math::sqrt(5.0)

  s = Solver::FSolver.new m
  s.set f, x_lo, x_hi

  puts "\nUsing #{s} method"

  printf "%5s [%9s, %9s] %9s %10s %9s\n", "iter", "lower", "upper", 
    "root", "err", "err(est)"

  while status == GSL_CONTINUE && iter < max_iter
    iter += 1
    status = s.iterate
    r = s.root
    x_lo = s.x_lower
    x_hi = s.x_upper
    status = Solver::test_interval x_lo, x_hi, 0, 0.001

    puts "Converged:" if status == GSL_SUCCESS

    printf "%5d [%.7f, %.7f] %.7f %+.7f %.7f\n", iter,  x_lo, x_hi,
      r, r - r_exp,  x_hi - x_lo
  end

end

STDERR.puts "\ndone."
