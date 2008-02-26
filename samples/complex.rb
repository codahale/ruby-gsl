#!/bin/env ruby

# $Id: complex.rb,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
require "benchmark"
require "complex"
include Benchmark

# Test complex numbers/functions
# This requires benchmark.rb (available from the RAA)

STDERR.puts "Running tests for complex numbers..."

puts "Ruby Complex:"
c = Complex(1.0, 2.0)
puts "real:  #{c.real}"
puts "image: #{c.image}"
puts "arg:   #{c.arg}"

puts "\nGSL::Complex:"
d = GSL::Complex.new c
puts "real:  #{d.real}"
puts "image: #{d.image}"
puts "arg:   #{d.arg}"
e = GSL::Complex.new2 2.0, -1.0
f = d + e
puts "d + e\nreal:  #{f.real}"
puts "image: #{f.image}"
f = d.add_real 4
puts "d + 4\nreal:  #{f.real}"
puts "image: #{f.image}"

n = 10000
puts "\nBenchmarking: (n=#{n})\nsin"
zr = d.real
zi = d.imag
bm do |x|
  x.report("Ruby  : ") {for i in 1..n; z = Math::sin(c); end}
  x.report("GSL_1 : ") {for i in 1..n; z = GSL::Complex::sin(d); end}
  x.report("GSL_2 : ") {for i in 1..n; szr, szi = GSL::Special::Trig::complex_sin_e(zr, zi); end}
  x.report("GSL_3 : ") {for i in 1..n; szr, szi = GSL::Complex::sin_e(d); end}
end

puts "\ntan"
bm do |x|
  x.report("Ruby : ") {for i in 1..n; z = Math::tan(c); end}
  x.report("GSL  : ") {for i in 1..n; z = GSL::Complex::tan(d); end}
end

md = Marshal.dump(d)
nd = Marshal.load(md)
if (d == nd)
  puts "Marshalling works"
else
  puts "Marshalling problem: #{d}, #{md.dump}, #{nd}"
end
STDERR.puts "\ndone."
