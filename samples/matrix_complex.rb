#!/bin/env ruby

# $Id: matrix_complex.rb,v 1.5 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

require 'gsl_util'
require 'assert'

STDERR.puts "Running tests for complex matrices..."

eps = 1.0e-15

m0 = Matrix_complex.new([1,0, 0,0], [0,0, 1,0])
m1 = Matrix_complex.new([0,0, 1,0], [1,0, 0,0])
m2 = Matrix_complex.new([0,0, 0,-1],[0,1, 0,0])
m3 = Matrix_complex.new([1,0, 0,0], [0,0, -1,0])

I = Complex.new2(0,1)

assert m0.get(0,0) == Complex.new2(1, 0)
assert m0.size1 == 2 && m0.size2 == 2

m4 = Matrix_complex.new([7,8, 9,10], [11,12, 13,14])
assert m4.row(0) == Vector_complex.new([7,8, 9,10])
assert m4.col(1) == Vector_complex.new([9,10, 13,14])
assert m4.t == Matrix_complex.new([7,8, 11,12], [9,10, 13,14])
#m5 = m4.dup
#m5[0,0] = Matrix_complex.new(7, 8+eps/2)
#assert m5 =~ m4

assert m0 + m2 == Matrix_complex.new([1,0, 0,-1], [0,1, 1,0]) 

v = Vector_complex.new([1,2, 3,4])
assert m1 * v == Vector_complex.new([3,4, 1,2])
assert m1 * m2 == m3 * I

STDERR.puts "\ndone."
