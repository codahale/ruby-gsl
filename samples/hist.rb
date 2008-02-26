#!/bin/env ruby

# $Id: hist.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL::Histogram

# Test histogramms

DUMP = "hist.dump"
File.delete(DUMP) if File.exists?(DUMP)

STDERR.puts "Running tests for histograms..."

puts "\nA simple histogram:"
h = Hist.new 10
v = [7, 7, 5, 1, 3, 5, 2, 4, 4, 4, 7, 7, 1, 9]
v.each do |x|
  h.increment x
end
h.dump
puts h.get(3)
puts h[3]

puts "\nDeep copy:"
h2 = Hist.new 10
Hist.memcpy h2, h
h2.dump

puts "\nmax    : #{h.max}"
puts "min    : #{h.min}"
puts "max_val: #{h.max_val}"
puts "max_bin: #{h.max_bin}"
puts "min_val: #{h.min_val}"
puts "min_bin: #{h.min_bin}"
puts "mean   : #{h.mean}"
puts "sigma  : #{h.sigma}"
a = h.get_range 3
puts "range  : #{a[0]}, #{a[1]}"
puts "find   : #{h.find(6.2)}"
begin
  print "\nwriting to file..."
  f = File.new DUMP, "w"
  h.fwrite f
  f.close
  puts "ok"
  print "reading from file..."
  f = File.new DUMP, "r"
  h.reset
  h.fread f
  f.close
  puts "ok"
  h.dump
  puts "\nread/write: ok"
rescue IOError
  STDERR.print "Caught exception: #{$!}\n"
end

puts "\nadd:"
h2 = Hist.new 10
w = [9, 2, 2, 3, 1, 9, 7]
w.each do |x|
  h2.increment x
end
h.add h2
h.dump

p = Hist_PDF.new h
puts "\nsample: #{p.sample(0.7)}"

File.delete(DUMP) if File.exists?(DUMP)

STDERR.puts "\ndone."
