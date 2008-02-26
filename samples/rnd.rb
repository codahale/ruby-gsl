#!/bin/env ruby

# $Id: rnd.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test histogramms and random number distributions
# generate output suitable for graph(1) from GNU plotutils:
# rnd.rb | awk '{print $1, $3; print $2, $3}' | graph -Tps > g.ps

STDERR.puts "Running tests for histograms and RND..."

h = Histogram::Hist.new2(100, -4, 4)
r = Random::RNG.new
puts "#m=1,S=0"
10000.times do
  begin
    h.increment(Random::RND::gamma(r, 1, 1))
  rescue RangeError
	next
  end
end
h.dump
h.reset
puts "#m=2,S=0"
10000.times do
  begin
    h.increment(Random::RND::gamma(r, 3, 1))
  rescue RangeError
	next
  end
end
h.dump

STDERR.puts "\ndone."
