#!/bin/env ruby

# $Id: hist2d.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
include GSL::Histogram

# Test 2D histogramms
# generate output suitable for graph(1) from GNU plotutils:
# hist2d.rb | grep -v '#' | graph -Tps > g.ps

STDERR.puts "Running tests for 2D histograms..."

h = Hist2D.new 10, 10
h.set_ranges_uniform 0.0, 1.0, 0.0, 1.0
h.accumulate 0.3, 0.3, 1
h.accumulate 0.8, 0.1, 5
h.accumulate 0.7, 0.9, 0.5

#puts "\nA simple 2D Histogram:"
#h.fprintf(STDOUT, "%g", "%g")
#h2 = Hist2D.new(10, 10)
#Hist2D::memcpy(h2, h)
#puts "\nDeep copy:"
#h2.fprintf(STDOUT, "%g", "%g")

r = Random::RNG.new
p = Hist2D_PDF.new h
i = 0

while (i < 1000)
  i += 1
  u = r.uniform
  v = r.uniform

  x, y = p.sample(u, v)
  
  printf "%g %g\n", x, y
end

printf "#  xmean: %f\n", h.xmean
printf "#  ymean: %f\n", h.ymean
printf "#  xsigma: %f\n", h.xsigma
printf "#  ysigma: %f\n", h.ysigma
printf "#  cov: %f\n", h.cov
printf "#  sum: %f\n", h.sum

# h.fprintf(STDOUT, "%g", "%g")

STDERR.puts "\ndone."
