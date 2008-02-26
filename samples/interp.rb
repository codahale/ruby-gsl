#!/bin/env ruby

# $Id: interp.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test module Interpolation
# generate output suitable for graph(1) from GNU plotutils:
# interp.rb | graph -Tps > interp.ps

STDERR.puts "Running interpolation tests (interp)..."

x = []
y = []
puts "#m=0,S=2"
0.upto(9) do |i|
  x[i] = i + 0.5 * Special::Trig::sin(i)
  y[i] = i + Special::Trig::cos(i * i)
  printf "%g %g\n", x[i], y[i]
end

acc = Interpolation::Accel.new
type = [Interpolation::LINEAR, Interpolation::CSPLINE, Interpolation::AKIMA]
type.each_index do |k|
  i = Interpolation::Interp.new type[k], 10
  #STDERR.print "#{i.name}\n"
  ret = i.init x, y
  STDERR.puts strerror(ret) if ret != GSL_SUCCESS

  puts "#m=#{k},S=0"
  xi = x[0]
  d = Result.new
  while (xi < x[9]) do
    yi = i.eval x, y, xi, acc
    printf "%g %g\n", xi, yi
    xi += 0.01
  end

  acc.reset
end

STDERR.puts "\ndone."
