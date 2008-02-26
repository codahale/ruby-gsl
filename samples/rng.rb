#!/bin/env ruby

# $Id: rng.rb,v 1.2 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL::Random

# Test random number generation

STDERR.puts "Running tests for random number generation..."

r = RNG.new2(RNG::CMRG)
c = r.clone
puts "\nname   : #{r.name}"
puts "min    : #{r.min}"
puts "max    : #{r.max}"
puts "size   : #{r.size}"
print "get    : "
10.times do |k| print r.get, " " end
print "\nclone  : "
10.times do |k| print c.get, " " end
print "\nuniform: "
10.times do |k| print r.uniform, " " end
puts

# c.dump

STDERR.puts "\ndone."
