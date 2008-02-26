#!/bin/env ruby

# $Id: rnd3.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL::Random

# Test random number distributions
# generate output suitable for graph(1) from GNU plotutils:
# rnd3.rb | graph -Tps > g.ps

STDERR.puts "Running tests for RND(3)..."

x = y = 0
a = []
r = RNG.new
puts "#m=1,S=2"
printf "%g %g\n", x, y
10.times do
  a = RND::dir_2d(r)
  x += a[0]
  y += a[1]
  printf "%g %g\n", x, y
end

STDERR.puts "\ndone."
