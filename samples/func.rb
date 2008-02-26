#!/bin/env ruby

# $Id: func.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

STDERR.puts "Running tests for Function class..."

#p = Proc.new { |x| x*x }
#f = GSL::Function.new2 p
f = Function.new { |x| x*x }

puts f.eval(2)
puts f.call(4)

puts f.proc

f.set { |x| x*x*x }

puts f.eval(2)
puts f[4]

f2 = Function.alloc { |x| Math::sin(x) - Math::log(x)*Math::sqrt(x) }

puts f2.eval(2.5)
puts f2.arity

STDERR.puts "\ndone."
