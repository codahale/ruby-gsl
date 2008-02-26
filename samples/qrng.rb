#!/bin/env ruby

# $Id: qrng.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL::Random

# Test quasi-random sequences
# generate output suitable for graph(1) from GNU plotutils:
# qrng.rb | graph -Tps > qrng.ps

STDERR.puts "Running tests for quasi-random sequences..."

q = QRNG.new QRNG::SOBOL, 2

#puts "\nname : #{q.name}"
#puts "dim  : #{q.dim}"
#puts "size : #{q.size}"
a = [0, 0]
puts "#m=0,S=2"
1024.times do |i|
  a = q.get
  puts a.join(" ")
  #print a.join(" "), "\n"
end

#q.reset

STDERR.puts "\ndone."
