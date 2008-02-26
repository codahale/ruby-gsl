#!/bin/env ruby

# $Id: linalg.rb,v 1.6 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
include Math

require 'gsl_util'
require 'gsl_linalg'
require 'assert'

STDERR.puts "Running tests for Linalg..."

puts "======= LU Decomposition ========="

puts "LU_decomp"
a = [ 0.18, 0.60, 0.57, 0.96,
      0.41, 0.24, 0.99, 0.58,
      0.14, 0.30, 0.97, 0.66,
      0.51, 0.13, 0.19, 0.85 ]
b = [ 1.0, 2.0, 3.0, 4.0 ]

m = Matrix.new(a, 4, 4)
v = Vector.new(b)
ma = m.clone
ma, p, s = ma.LU_decomp!
assert Matrix.perm(p) * m =~ ma.L * ma.U

puts "LU_solve"
x = ma.LU_solve p, v
assert m * x =~ v

puts "LUD_solve"
x1 = m.LUD_solve(v)
assert x1 == x

puts "LU_svx!"
v_dup = v.clone
v_dup.LU_svx!(ma, p)
assert v_dup == x

puts "LU_invert"
m1 = ma.LU_invert(p)
assert m1 =~ m.inv

puts "LU_det"
assert ma.LU_det(s) == m.det

puts "LU_lndet"
assert ma.LU_lndet =~ log(m.det.abs)

puts "LU_sgndet"
assert ma.LU_sgndet(s) == -1

puts "======= QR Decomposition ========="
ma = Matrix.new([1,1], [2,3], [2,1])
qr, tau = ma.QR_decomp

puts "householder matrices"
Q = qr.house_m(0) * qr.house_m(1)
assert ma =~ Q * qr.U

puts "QR_solve"
a = Matrix.new([2,0,1],[6,2,0],[-3,-1,-1])
b = Vector.new([1,0,1])
qr, tau = a.QR_decomp
x = qr.QR_solve(tau, b)
assert a*x =~ b

puts "QR_lssolve"
a3 = Matrix.new([1,1],[2,3],[2,1])
b3 = Vector.new([1,1,1])
qr3, tau3 = a3.QR_decomp
x3, res = qr3.QR_lssolve(tau3, b3)
assert x3 =~ Vector.new([5.0/9, 0])
assert res =~ b3 - a3 * x3

puts "QR_QTvec"
qr, tau = Matrix.new([1,1], [2,3], [2,1]).QR_decomp
Q1 = qr.house_m(0) * qr.house_m(1)
b = Vector.new([1,2,3])
assert qr.QR_QTvec(tau, b) =~ Q1.t * b

puts "QR_Qvec"
assert qr.QR_Qvec(tau, b) =~ Q1 * b

puts "QR_unpack"
Q2, R1 = qr.QR_unpack(tau)
assert Q2 =~ Q1
assert ma =~ Q1 * R1

puts "QR_QRsolve"
m3 = Matrix.new([1,1], [2,3])
qr3, tau3 = m3.QR_decomp
Q3, R3 = qr3.QR_unpack(tau3)
Q3a = qr3.house_m(0)
assert Q3a =~ Q3 && m3 =~ Q3a * R3
v = Vector.new([1,0])
b = m3 * v
x = Linalg::QR_QRsolve(Q3, R3, b)
assert R3 * x =~ Q3.t * b

puts "QR Decomposition with Column Pivoting"

puts "QRPT_decomp"
ma = Matrix.new([1,2,3], [1,5,6],[1,8,9],[1,11,12])
vq, vr, vtau, vp, signum, vnorm = ma.QRPT_decomp
assert ma =~ [vq * vr * Matrix::perm(vp), 1.0e-14]

puts "QRPT_solve"
a = Matrix.new([2,0,1],[6,2,0],[-3,-1,-1])

a1 = a.clone
b = Vector.new([1,0,1])
vtau, vp, signum, vnorm = a1.QRPT_decomp!
x = a1.QRPT_solve(vtau, vp, b)
assert a*x =~ b

puts "QRPT_QRsolve"
vq, vr, vtau, vp, signum, vnorm  = a.QRPT_decomp
x = Linalg::QRPT_QRsolve(vq.transpose, vr, vp, b) # this fix works
# in linalg/qrpt.c 
#    /* compute b' = Q^T b */
#     gsl_blas_dgemv (CblasNoTrans, 1.0, Q, b, 0.0, x);
# becomes 
#     gsl_blas_dgemv (CblasTrans, 1.0, Q, b, 0.0, x);
#          
# This function solves the system R P^T x = Q^T b for x: 
assert vr * Matrix.perm(vp) * x =~ vq.transpose * b
assert a * x =~ b
assert a =~ vq * vr * Matrix.perm(vp)

puts "QRPT_solve"
ma = a.clone
vtau2, vp2, signum2, vnorm2 = ma.QRPT_decomp!
b1 = vq.t * b
x1 = ma.QRPT_Rsolve(vp, b1)
assert a * x1 =~ b && vr * Matrix.perm(vp) * x1 =~ b1
assert vq * vr * Matrix.perm(vp) =~ a

puts "QRPT_Rsvx!"
x = b1.clone
x.QRPT_Rsvx!(vr, vp)
assert Matrix.perm(vp).inv * vr.upper(true).inv * b1 =~ x

puts "======= Singular Value Decomposition ============="
a = Matrix.new([1,1],
               [0,1],
	       [1,0])

puts "SV_decomp!"	       
a_dup = a.clone
v, s = a_dup.SV_decomp!

u1 = Matrix.new([-sqrt(6)/3,	    0   	], 
                [-sqrt(6)/6,	-sqrt(2)/2	],
		[-sqrt(6)/6,	sqrt(2)/2 	])

d1 = Matrix.new([sqrt(3),	0],
                [      0,       1])
		
v1 = Matrix.new([-sqrt(2)/2,	sqrt(2)/2 ],
                [-sqrt(2)/2,	-sqrt(2)/2])
		
assert a_dup =~ u1 && d1[0][0] == s[0] && v =~ v1 && a =~ u1 * d1 * v1.t
assert d1[1][1] =~ s[1]

puts "SV_decomp"
u2, v2, s2 = a.SV_decomp
assert u2 =~ u1 && v2 =~ v1 && s2 =~ s

puts "SV_decomp_mod!"
a_dup3 = a.clone
v3, s3 = a_dup3.SV_decomp_mod!
assert a_dup3 =~ u1 && v3 =~ v && s3 =~ s

puts "SV_decomp_mod"
u4, v4, s4 = a.SV_decomp_mod
assert u4 =~ u1 && v4 =~ v1 && s4 =~ s

puts "SV_decomp_jacobi!"
a_dup5 = a.clone
v5, s5 = a_dup5.SV_decomp_jacobi!
assert v5 =~ -v1 && s5 =~ s && a_dup5 =~ -a_dup3 && a =~ a_dup5 * d1 * v5.t

puts "SV_decomp_jacobi"
u6, v6, s6 = a.SV_decomp_jacobi
assert u6 =~ a_dup5 && v6 =~ v5 && s6 =~ s5

puts "SV_solve"
b = Vector.new([3, 4, 5])
x = Linalg::SV_solve(u2, v2, s2, b)
assert a * x =~ Vector.new([5, 2 ,3])
# check that x is the least squares solution to the overdetermined system a x = b
qr6, tau6 = a.QR_decomp
x6, res6 = qr6.QR_lssolve(tau6, b)
assert x6 =~ x && res6 =~ b - a * x

puts "======= Cholesky Decomposition =========="

puts "cholesky_decomp!"
a = Matrix.new([2, -2], [-2, 5])
a_dup = a.clone
a_dup.cholesky_decomp!
aL = a_dup.lower(true)
aLT = a_dup.upper(true)
assert aL == Matrix.new([sqrt(2), 0],[-sqrt(2), sqrt(3)])
assert a =~ aL * aL.t

puts "cholesky_solve"
x1 = Vector.new([1,2])
b = a * x1
x = a_dup.cholesky_solve(b)
assert x1 =~ x

puts "cholesky_svx!"
b_dup = b.clone
b_dup.cholesky_svx!(a_dup)
assert b_dup == x


puts "Tridiagonal Decomposition of Real Symmetric Matrices"

puts "symmtd_decomp"
a = Matrix.new([1,3,4,5],
               [3,2,8,9],
	       [4,8,3,7],
	       [5,9,7,6])
tm, tau = a.symmtd_decomp
v_0 = Vector.new([0,1,tm.get(2,0), tm.get(3,0)])
v_1 = Vector.new([0,0,1, tm.get(3,1)])
h_0 = v_0.house
h_1 = v_1.house
q = h_0 * h_1
# Get the tridiagonal matrix
a1 = tm.clone
a1.set(2,0,0)
a1.set(3,0,0); a1.set(3,1,0)
t1 = a1.lower(true) + a1.lower(false).t
assert t1 =~ [q.t * a * q, 1.0e-14]
assert tau[0] =~ 2/(v_0*v_0)
assert tau[1] =~ 2/(v_1*v_1)

puts "symmtd_unpack"
q_un, d_un, sd_un = tm.symmtd_unpack(tau)
assert q_un =~ q && d_un =~ t1.subdiagonal(0) && sd_un =~ t1.subdiagonal(1)

puts "symmtd_unpack_T"
d_un, sd_un = tm.symmtd_unpack_T
assert d_un =~ t1.subdiagonal(0) && sd_un =~ t1.subdiagonal(1)

puts "========= Bidiagonalization =============="

# M x N matrix A with M = 4, N = 3 
a = Matrix.new([1,3,4],
               [3,2,8],
	       [4,8,3],
               [5,9,7])
m = a.size1
n = a.size2
puts "bidiag_decomp"
tm, tu, tv = a.bidiag_decomp

# householder vectors U_i, i = 0,.., N-1
vu0 = tm.col(0); vu0[0] = 1
vu1 = tm.col(1); vu1[0] = 0; vu1[1] = 1
vu2 = tm.col(2); vu2[0] = 0; vu2[1] = 0; vu2[2] = 1

# U = U_0 U_1 .. U_(N-1)
vu1 = vu0.house * vu1.house * vu2.house

# householder vectors V_i, i = 0,.., N-3
vv0 = tm.row(0); vv0[0] = 0; vv0[1] = 1 

# V = V_0 .. V_(N-3)
vv1 = vv0.house

# B = U^T A V
bd1 = vu1.t * a * vv1

# check that B is bidiagonal within precision 1.0e-14
(2..n-1).each{|i| assert bd1.superdiagonal(i) =~ [Vector::zero(n-i), 1.0e-14]}
(1..m-1).each{|i| assert bd1.subdiagonal(i) =~ [Vector::zero(m-i), 1.0e-14]}

puts "bidiag_unpack"
vu, vv, vd, vs = tm.bidiag_unpack(tu, tv)
bd = vu.t * a * vv
assert bd.superdiagonal(0) =~ [vd, 1.0e-14]
assert bd.superdiagonal(1) =~ [vs, 1.0e-14]

assert vu.size1 == 4 && vu.size2 == 3 && vu1.size1 == 4 && vu1.size2 == 4
3.times {|i| assert vu.col(i) =~ vu1.col(i)}

assert vv1 =~ vv

puts "bidiag_unpack2"
tm_dup = tm.clone
vv2 = tm_dup.bidiag_unpack2(tu, tv) 
assert tm_dup =~ vu
assert vv2 =~ vv
assert tu =~ vd && tv =~ vs

puts "bidiag_unpack_B"
vd3, vs3 = bd.bidiag_unpack_B
assert vd3 =~ [vd, 1.0e-14] && vs3 =~ vs

puts "======== Householder solver for linear systems ======="
puts "HH_solve"
a = Matrix.new([1,3,4],
               [3,2,6],
	       [2,8,3])

x = Vector.new([1,2,3])

b = a*x
a_dup = a.clone
x1 = a_dup.HH_solve(b)
assert x1 =~ [x, 1.0e-14]
assert a_dup == a
    

puts "HH_svx!"
b_dup = b.clone
a_dup = a.clone
a_dup.HH_svx!(b_dup)
assert b_dup =~ [x, 1.0e-14] && a_dup !~ a

puts "=========== Tridiagonal Systems ==============="
puts "solve_symm_tridiag"
a = Matrix.new([1,3,4,5],
               [3,2,8,9],
	       [4,8,3,7],
	       [5,9,7,6])
tm, tau = a.symmtd_decomp
q, d, sd = tm.symmtd_unpack(tau)
t = q.t * a * q
b = Vector.new([1,2,3,4])
x = Linalg::solve_symm_tridiag(d,sd,b)
assert t * x =~ [b, 1.0e-14]

puts "solve_symm_cyc_tridiag"

t2 = Matrix.new([1,3,0,5],
                [3,2,8,0],
        	[0,8,3,7],
	        [5,0,7,6])

d2 = t2.subdiagonal(0)
e2 = Vector.new([3,8,7,5])
x2 = Linalg::solve_symm_cyc_tridiag(d2,e2,b)
assert t2 * x2 =~ b

STDERR.puts "\ndone."

