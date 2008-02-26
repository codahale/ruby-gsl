#!/bin/env ruby

# $Id: eigen.rb,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
include Math

require 'gsl_util'
require 'gsl_linalg'
require 'assert'

STDERR.puts "Running tests for Eigenvalues..."

eps = 1.0e-15

puts "eigen_symm"
a_diag = Matrix.new([1,0,0],[0,2,0],[0,0,3])
h = Matrix.new([0.6, -0.8,0],[-0.8, -0.6,0],[0,0,1])
a = Matrix.new([41.0/25, 12.0/25, 0], [12.0/25, 34.0/25, 0], [0, 0, 3])
assert h.t * a_diag * h =~ a
eval = a.eigen_symm
assert eval =~ Vector.new([2,1,3])

puts "eigen_symmv"
eval1, evec1 = a.eigen_symmv
3.times{|i| assert a * evec1.col(i) == evec1.col(i) * eval[i]}

puts "eigen_symmv!"
a_dup = a.dup
eval2, evec2 = a_dup.eigen_symmv!
assert eval2 == eval1 &&  evec2 == evec1
assert a_dup == a  # ?

puts "symmv_sort"
eval1_dup, evec1_dup =  eval1, evec1
Eigen::symmv_sort(eval1_dup, evec1_dup,Eigen::SORT_VAL_ASC)
assert eval1_dup == Vector.new([1,2,3])
assert evec1_dup.col(0) == evec2.col(1) && evec1_dup.col(1) == evec2.col(0)

eval1_dup, evec1_dup =  eval1, evec1
Eigen::symmv_sort(eval1_dup, evec1_dup,Eigen::SORT_VAL_DESC)
assert eval1_dup == Vector.new([3,2,1])
3.times{|i| assert a * evec1_dup.col(i) == evec1_dup.col(i) * eval1_dup[i]}

eval1_dup, evec1_dup =  eval1, evec1
Eigen::symmv_sort(eval1_dup, evec1_dup,Eigen::SORT_ABS_ASC)
assert eval1_dup == Vector.new([1,2,3])
3.times{|i| assert a * evec1_dup.col(i) == evec1_dup.col(i) * eval1_dup[i]}

eval1_dup, evec1_dup =  eval1, evec1
Eigen::symmv_sort(eval1_dup, evec1_dup,Eigen::SORT_ABS_DESC)
assert eval1_dup == Vector.new([3,2,1])
3.times{|i| assert a * evec1_dup.col(i) == evec1_dup.col(i) * eval1_dup[i]}

puts "eigen_herm"
a_diag = Matrix.new([1,0,0],[0,2,0],[0,0,3]).to_complex
h = Matrix.new([0.6, -0.8,0],[-0.8, -0.6,0],[0,0,1]).to_complex
a0 = Matrix.new([41.0/25, 12.0/25, 0], [12.0/25, 34.0/25, 0], [0, 0, 3]).to_complex
assert h.t * a_diag * h =~ a0
v_0 = Vector_complex.new([1,1, 1,2, 1,3])
U = v_0.house
assert U * U.h =~ Matrix_complex::identity(3)
a = U * a0 * U.h

a_dup = a.dup
eval = a_dup.eigen_herm!
assert eval =~ Vector.new([3,1,2])
eval2 = a.eigen_herm
assert eval == eval2

puts "eigen_hermv!"
a_dup = a.dup
eval1, evec1 = a_dup.eigen_hermv!

3.times{|i| assert a * evec1.col(i) =~ evec1.col(i) * eval[i]}
assert a_dup !~ [a, 1.0e-14]  

puts "eigen_hermv"
eval2, evec2 = a.eigen_hermv
assert eval2 == eval1 &&  evec2 == evec1

puts "hermv_sort"
eval1_dup, evec1_dup =  eval1, evec1
Eigen_complex::hermv_sort(eval1_dup, evec1_dup,Eigen_complex::SORT_VAL_ASC)
assert eval1_dup =~ Vector.new([1,2,3])
#assert evec1_dup.col(0) == evec2.col(1) && evec1_dup.col(1) == evec2.col(0)
assert evec1_dup.col(0) == evec2.col(1)
assert evec1_dup.col(1) =~ evec2.col(2)
assert evec1_dup.col(2) =~ evec2.col(0)

eval1_dup, evec1_dup =  eval2, evec2
Eigen_complex::hermv_sort(eval1_dup, evec1_dup,Eigen_complex::SORT_VAL_DESC)
assert eval1_dup =~ Vector.new([3,2,1])
3.times{|i| assert a * evec1_dup.col(i) =~ evec1_dup.col(i) * eval1_dup[i]}
eval1_dup, evec1_dup =  eval2, evec2
Eigen_complex::hermv_sort(eval1_dup, evec1_dup,Eigen_complex::SORT_ABS_ASC)
assert eval1_dup =~ Vector.new([1,2,3])
3.times{|i| assert a * evec1_dup.col(i) =~ evec1_dup.col(i) * eval1_dup[i]}

eval1_dup, evec1_dup =  eval2, evec2
Eigen_complex::hermv_sort(eval1_dup, evec1_dup,Eigen_complex::SORT_ABS_DESC)
assert eval1_dup =~ Vector.new([3,2,1])
3.times{|i| assert a * evec1_dup.col(i) =~ evec1_dup.col(i) * eval1_dup[i]}

STDERR.puts "\ndone."
