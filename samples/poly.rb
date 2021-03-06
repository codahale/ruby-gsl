#!/bin/env ruby

# $Id: poly.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test for polynomial functions

STDERR.puts "Running polynomial tests..."

puts "\nPoly:"
c = [1, 2]
print "  evaluate: ", Poly::evaluate(c, 3), "\n"

print "  solve_quadratic: "
a = Poly::solve_quadratic 1, -2, 1
if a.length == 0
  puts "no solutions"
else
  puts a.join(" ")
end

print "  complex_solve_quadratic: "
a = Poly::complex_solve_quadratic 1, 0, 1
puts a.join(" ")

a = Poly::solve_cubic -6, 12, -8
print "  solve_cubic: "
if a.length == 1
  puts a[0]
else
  puts a.join(" ")
end

puts "  complex_solve: "
w = Poly::Workspace.new 6
a = [-1, 0, 0, 0, 0, 1]
z = w.solve a
for i in 0..4
  printf "    z%d = (%+.18f, %+.18f)\n", i, z[2*i], z[2*i+1]
end

print "  divided difference representation: "
xa = [0, 2, 4, 6]
ya = [0, 10, 0, -10]
p = Poly::DD.new xa, ya
puts "[" + p.get.join(",") + "]"

print "  Taylor expansion: "
c = p.taylor 0
puts "[" + c.join(",") + "]"

STDERR.puts "\ndone."
