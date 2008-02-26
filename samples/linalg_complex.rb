#!/bin/env ruby

# $Id: linalg_complex.rb,v 1.6 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
include Math

require 'gsl_util'
require 'gsl_linalg'
require 'assert'

STDERR.puts "Running tests for Linalg_complex..."

zero = Complex.new2(0,0)
one = Complex.new2(1,0)

puts "======= LU Decomposition ========="

puts "LU_decomp"
m2 = Matrix_complex.new([1,2, 1,3], [2,1, 2,2])
m_2, p2, s2 = m2.LU_decomp
# P A = L U
assert Matrix.perm(p2).to_complex * m2 =~ m_2.L * m_2.U
v1 = Vector_complex.new([1,3, 2,4])
puts "LU_solve"
x1 = m_2.LU_solve(p2, v1)
assert v1 =~ [m2 * x1, 1.0e-14]

puts "LUD_solve"
x2 = m2.LUD_solve(v1)
assert x2 == x1

puts "LU_decomp_solve!"
m3 = m2.clone
x3 = m3.LU_decomp_solve!(v1)
assert m3 == m_2 && x3 == x1

puts "LU_svx"
x4 = v1.LU_svx(m_2, p2)
assert x4 == x1

puts "LU_svx!"
v1_dup = v1.clone
v1_dup.LU_svx!(m_2, p2)
assert v1_dup == x1

puts "LU_det"
det = m2.get(0,0) * m2.get(1,1) - m2.get(1,0) * m2.get(0,1)
assert m_2.LU_det(s2) =~ det

puts "det"
assert m2.det =~ det

puts "LU_lndeti and lndet"
assert m_2.LU_lndet =~ Math::log(det.abs)
assert m2.lndet =~ Math::log(det.abs)

puts "LU_sgndet (alias sgndet)"
assert m_2.sgndet(s2) =~ det.div_real(det.abs)

puts "trace"
assert m2.trace == m2.get(0,0) + m2.get(1,1)


puts "========= householder transformations ==========="
v = Vector_complex.new([3,2, 4,-1, 6,3])
tau_0, vv_0 = v.householder_transform
puts "tau_0, vv_0"
p tau_0, vv_0
assert vv_0[0] =~ [Complex.new2(-v.dznrm2, 0),1.0e-14]

# save the non-vanishing component (-norm) and construct the
# Householder vector
ei = vv_0[0]
vv_0[0] = one

# Compute the hoiseholder transformation P v
res = vv_0.house_(tau_0.conjugate) * v

assert res =~ [Vector_complex.new2(ei, zero, zero),  1.0e-14]

if Vector_complex.instance_methods.include?("householder_hv")
  puts "householder_hv"
  assert res =~ [v.householder_hv(tau_0.conjugate, vv_0), 1.0e-14]
end

puts "householder_hm!"
m = Matrix_complex.new([3,2, 1,2, 3,4], [4,-1, 4,3, 2,1], [6,3, 5,4, 3,2])
m_dup = m.clone
m_dup.householder_hm!(tau_0, vv_0)
p m_dup
assert vv_0.house_(tau_0) * m =~ m_dup

puts "======= Hermitian to symmetric tridiagonal decomposition ===="
puts "hermtd_decomp"
a = Matrix_complex.new([2,0,  3,2, 4,1],
                       [3,-2, 2,0, 8,1],
                       [4,-1, 8,-1,3,0])

puts "hermtd_decomp"       
a1, tau = a.hermtd_decomp
# Householder vector
v_0 = Vector_complex.new2(zero, one, a1.get(2,0))
h_0 = v_0.house_(tau[0])
v_1 = Vector_complex.new2(zero,zero,one)
h_1 = v_1.house_(tau[1])
# Q
q = h_0 * h_1
# Get the tridiagonal matrix
a1_dup = a1.clone
a1_dup[2,0] = Complex.new2(0,0)
t1 = a1_dup.lower(true) + a1_dup.lower(false).h
assert t1 =~ [q.h * a * q, 1.0e-14]

puts "hermtd_unpack"
q_un, d_un, sd_un = a1.hermtd_unpack(tau)
assert q_un =~ q 
assert d_un =~ t1.subdiagonal(0).real
assert sd_un =~ t1.subdiagonal(1).real

puts "hermtd_unpack_T"
d, sd = a1.hermtd_unpack_T
assert d == d_un &&  sd == sd_un

STDERR.puts "\ndone."
