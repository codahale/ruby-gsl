#!/bin/env ruby

# $Id: rnd2.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL::Random

# Test random number distributions
# generate output suitable for graph(1) from GNU plotutils:
# rnd2.rb | graph -Tps > g.ps

STDERR.puts "Running tests for RND(2)..."

a = []
r = RNG.new
puts "#m=0,S=2"
1000.times do
  a = RND::bivariate_gaussian(r, 1, 1, 0.9)
  printf "%g %g\n", a[0], a[1]
end

STDERR.puts "\ndone."
