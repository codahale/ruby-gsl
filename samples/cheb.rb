#!/bin/env ruby

# $Id: cheb.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test Chebyshev approximations
# visualize output with graph(1) from GNU plotutils:
# ruby cheb.rb > cheb.dat
# awk '{print $1,$2}' cheb.dat > 1.dat
# awk '{print $1,$3}' cheb.dat > 2.dat
# awk '{print $1,$4}' cheb.dat > 3.dat
# graph -Tps 1.dat -m 2 2.dat -m 3 3.dat > plot.ps

STDERR.puts "Running tests for Chebyshev Approximations..." 

f = Function.new { |x|
  if x < 0.5
	0.25
  else
	0.75
  end
}

i = 0.0
n = 10000.0

cs = Chebyshev.new 40, f, 0.0, 1.0

while (i < n)
  i += 1.0
  x = i / n
  r10 = cs.eval_n 10, x
  #res = cs.eval_n_err(10, x)
  r40 = cs.eval x
  #res = cs.eval_err(x)

  y = f[x]

  printf "%g %g %g %g\n", x, y, r10, r40
  #printf "%g (err = %g)\n", res.val, res.err
end

STDERR.puts "\ndone."
