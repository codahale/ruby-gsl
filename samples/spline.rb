#!/bin/env ruby

# $Id: spline.rb,v 1.2 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL

# Test module Interpolation
# generate output suitable for graph(1) from GNU plotutils:
# spline.rb | graph -Tps > spline.ps

STDERR.puts "Running interpolation tests (spline)..."

x = []
y = []
puts "#m=0,S=2"
0.upto(9) do |i|
  x[i] = i + 0.5 * Special::Trig::sin(i)
  y[i] = i + Special::Trig::cos(i * i)
  printf("%g %g\n", x[i], y[i])
end

spline = Interpolation::Spline.new(Interpolation::CSPLINE, 10)
#STDERR.print "\nname    : ", i.name, "\n"
#STDERR.print "min_size: ", i.min_size, "\n"
acc = Interpolation::Accel.new
ret = spline.init(x, y)
STDERR.puts strerror(ret) if ret != GSL_SUCCESS

puts "#m=1,S=0"
xi = x[0]
d = Result.new
while (xi < x[9]) do
  yi = spline.eval(xi, acc)
  # ret = i.eval_e(xi, acc, d)
  # STDERR.puts strerror(ret) if ret != GSL_SUCCESS
  printf("%g %g\n", xi, yi)
  xi += 0.01
end
ret = spline.eval_integ_e(2.0, 8.0, acc, d)
STDERR.puts strerror(ret) if ret != GSL_SUCCESS
#STDERR.print "integral: ", d.val, "\n"

STDERR.puts "\ndone."
