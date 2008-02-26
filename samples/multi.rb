#!/bin/env ruby

# $Id: multi.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test module MSolver

STDERR.puts "Running multiroot-finding tests (fsolver)..."

s = Multiroot::MFSolver.new Multiroot::MFSolver::BROYDEN, 3
puts s

a = [2, 4]
v = Vector.new a
r = Vector.new 2
f = MFunction.new { |x, y| puts("Ruby"); nil }
puts f
puts f.eval(v, r)
puts f.arity
puts f.proc

STDERR.puts "\ndone."
