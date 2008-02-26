#!/bin/env ruby

# $Id: blas.rb,v 1.8 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL
require 'gsl_util'
require 'gsl_linalg'
require 'assert'

STDERR.puts "Running tests for BLAS support..."

eps = 1.0e-15
# comparison are done with =~ (precision 1.0e-15);
# when I find in an example that exact equality holds, I use == ,
# but such a precision is never warranted for operations with doubles
 
a1 = 2
a2 = 3
c1 = Complex.new2(1,2)
c2 = Complex.new2(2,3)
v1 = Vector.new([1,2,3])
v2 = Vector.new([4,6,5])
cv1 = Vector_complex.new([7,1, 3,2, 4,5])
cv2 = Vector_complex.new([1,2, 3,1, 5,6])
m1 = Matrix.new([6,5,4],[3,6,4],[3,4,3])
m2 = Matrix.new([3,2,1],[2,4,1],[1,7,8])
m3 = Matrix.new([9,8,7],[9,2,8],[8,1,4])
cm1 = Matrix_complex.new([1,2, 2,3, 3,4],[5,6, 6,7, 7,8],[9,8, 8,7, 7,6])
cm2 = Matrix_complex.new([5,1, 1,4, 2,6],[1,3, 2,4, 3,5],[6,4, 5,3, 2,1])
cm3 = Matrix_complex.new([1,4, 2,5, 3,6],[4,1, 5,2, 6,3],[5,1, 4,1, 3,1])
csym1 = Matrix_complex.new([1,0, 3,4, 5,6], [3,4, 6,0, 6,5], [5,6, 6,5, 2,0])


# symmetric matrix
sm1 = Matrix.new([1,2,3],[2,1,1],[3,1,2])
# hermitian matrix
csm1 = Matrix_complex.new([1,0,  3,4, 5,6], [3,-4, 6,0, 6,5], [5,-6, 6,-5,2,0])


# shorthands:
assert Blas::NoTrans == false && Blas::Trans == true && Blas::ConjTrans == nil
assert Blas::Upper == true && Blas::Lower == false
assert Blas::Unit == true && Blas::NonUnit == false
assert Blas::Left == true && Blas::Right == false
assert Blas_complex::NoTrans == false # etc

puts "Level 1"

puts "scalar product x^T y and norm"
assert v1.norm * v1.norm == v1*v1
assert cv1.zdotu(cv2) == Complex.new2(2, 73)

puts "hermitian product x^H y and norm"
assert cv1 * cv2 == Complex.new2(70, 9)
cv1_sq = cv1*cv1
assert Math::float_equal(cv1.dznrm2 * cv1.dznrm2, cv1_sq.real, 1.0e-13)
assert cv1.dznrm2 * cv1.dznrm2 =~ [cv1_sq.real, 1.0e-13]

puts "dasum"
assert v1.dasum == 6

puts "dzasum"
assert cv1.dzasum == 22

puts "idamax, izamax"
assert v2.idamax == 1
assert cv1.izamax == 2

puts "dswap and zswap"
v1d = v1.clone; v2d = v2.clone
Blas::dswap(v1d, v2d)
assert v1d == v2 && v2d == v1

cv1d = cv1.clone; cv2d = cv2.clone
Blas_complex::zswap(cv1d, cv2d)
assert cv1d == cv2 && cv2d == cv1

puts "daxpy"
assert v1.daxpy(v2, 2) == v1 + v2 * 2

puts "zaxpy"
assert cv1.zaxpy(cv2, c1) == Vector_complex.new([4,5, 4,9, -3,21])

puts "drotg"
v = Vector.new([a1,a2])
c, s, r = Blas::drotg(a1,a2)
assert Matrix.new([c, s], [-s, c]) * v =~ Vector.new([r,0]) && r == Vector.new([a1, a2]).norm

puts "drot"
va = Vector.new([a1,a1,a1,a1])
vb = Vector.new([a2,a2,a2,a2])
w1, w2 = va.drot(vb, c, s)
assert w1 =~ Vector.new([r,r,r,r]) && w2 == Vector.new([0,0,0,0])


puts "Level 2"

puts "y.dgemv(TransA, alpha, A, x, beta) -> y'"
assert v2.dgemv(false, a1, m1, v1, a2) == m1 * v1 * a1 + v2 * a2
assert v2.dgemv(true, a1, m1, v1, a2) == m1.t * v1 * a1 + v2 * a2

puts "y.zgemv(TransA, alpha, A, x, beta) -> y'"
assert cv2.zgemv(false, c1, cm1, cv1, c2) == cm1 * cv1 * c1 + cv2 * c2
assert cv2.zgemv(true, c1, cm1, cv1, c2) == cm1.t * cv1 * c1 + cv2 * c2
assert cv2.zgemv(nil, c1, cm1, cv1, c2) == cm1.herm * cv1 * c1 + cv2 * c2

puts "x.dtrmv(Uplo, TransA, DiagUnit, A) -> x'"
n1 = m1.upper(false) + Matrix.identity(m1.size1)
assert v1.dtrmv(true, true, true, m1) == n1.t * v1

puts "x.ztrmv(Uplo, TransA, DiagUnit, A) -> x'"
cn1 = cm1.upper(false) + Matrix_complex.identity(cm1.size1)
assert cv1.ztrmv(true, true, true, cm1) == cn1.t * cv1
assert cv1.ztrmv(true, false, true, cm1) == cn1 * cv1

puts "x.dtrsv(Uplo, TransA, DiagUnit, A) -> x'"
assert v1.dtrsv(true, true, true, m1) == n1.inv.t * v1

puts "x.ztrsv(Uplo, TransA, DiagUnit, A) -> x'"
cm1UD =  Matrix_complex.new([1,0, 2,3, 3,4],[0,0, 1,0, 7,8],[0,0, 0,0, 1,0])
cm1UD_inv = Matrix_complex.new([1,0, -2,-3, -13, 33], [0,0, 1,0, -7,-8],[0,0, 0,0, 1,0])
assert cm1UD * cm1UD_inv =~ Matrix_complex::identity(3)
assert cm1UD_inv.t * cv1 =~ cv1.ztrsv(true, true, true, cm1)

puts "y.dsymv(Uplo, alpha, A, x, beta) -> y'"
v2.dsymv(true, a1, sm1.upper(true), v1, a2) == sm1 * v1 * a1 + v2 * a2


assert cv2.zhemv(true, c1, csm1, cv1, c2) == csm1 * cv1 * c1 + cv2 * c2

puts "A.dger(alpha, x, y) -> A'"
assert m1.dger(a1, v1, v2) == m1 + v1.outer(v2) * a1

puts "A.zgeru(alpha, x, y) -> A'"
assert cm1.zgeru(c1, cv1, cv2) == cm1 + cv1.outer(cv2) * c1

puts "A.zgerc(alpha, x, y) -> A'"
assert cm1.zgerc(c1, cv1, cv2) == cm1 + cv1.outerc(cv2) * c1

puts "A.dsyr(Uplo, alpha, x) -> A'"
smvv = v1.outer(v1) * a1 + sm1
assert sm1.upper(true).dsyr(true, a1, v1) == smvv.upper(true)

puts "A.zher(Uplo, alpha, x) -> A'"
alpha = 2; c = Complex.new2(alpha,0)
w2 = cv1.outerc(cv2) * c + cv2.outerc(cv1) * c + csm1
assert csm1.upper(true).zher2(true, c, cv1, cv2) == w2.upper(true)


puts "A.dsyr2(Uplo, alpha, x) -> A'"
smvv = v1.outer(v2) * a1 + v2.outer(v1) * a1 + sm1
assert sm1.upper(true).dsyr2(true, a1, v1, v2) == smvv.upper(true)

puts "A.zher2(Uplo, alpha, x,y) -> A'"
w2 = cv1.outerc(cv2) * c1 + cv2.outerc(cv1) * c1.conjugate + csm1
assert csm1.upper(true).zher2(true, c1, cv1, cv2) == w2.upper(true)

puts "Level 3"

puts "C.dgemm(TransA, TransB, alpha, A, B, beta) -> C'"
assert m3.dgemm(true, true, a1, m1, m2, a2) == m1.t * m2.t * a1 + m3 * a2 

puts "C.zgemm(TransA, TransB, alpha, A, B, beta) -> C'"
assert cm3.zgemm(true, true, c1, cm1, cm2, c2) == cm1.t * cm2.t * c1 + cm3 * c2
assert cm3.zgemm(false, true, c1, cm1, cm2, c2) == cm1 * cm2.t * c1 + cm3 * c2
assert cm3.zgemm(nil, true, c1, cm1, cm2, c2) == cm1.h * cm2.t * c1 + cm3 * c2

puts "C.dsymm(SideLeft, Uplo, alpha, A, B, beta) -> C'"
assert m3.dsymm(true, true, a1, sm1.upper(true), m2, a2) == sm1 * m2 * a1 + m3 * a2

puts "C.zsymm(SideLeft, Uplo, alpha, A, B, beta) -> C'"
assert cm2.zsymm(true, true, c1,csym1.upper(true), cm1, c2) == 
        csym1 * cm1 * c1 + cm2 * c2

puts "B.dtrmm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'"
m1.upper(true).t * m2 * a1 == m2.dtrmm(true, true, true, false, a1, m1)

puts "B.ztrmm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'"
m_U = cm1.upper(true); m_L = cm1.lower(true)
m_UD = cm1.upper(false) + Matrix_complex::identity(3)
m_LD = cm1.lower(false) + Matrix_complex::identity(3)
assert m_U.t * cm2 * c1 == cm2.ztrmm(true, true, true, false, c1, cm1)
assert m_U * cm2 * c1 == cm2.ztrmm(true, true, false, false, c1, cm1)
assert m_U.h * cm2 * c1 == cm2.ztrmm(true, true, nil, false, c1, cm1)
assert m_UD.t * cm2 * c1 == cm2.ztrmm(true, true, true, true, c1, cm1)

puts "B.dtrsm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'"
assert m1.upper(true).inv.t * m2 * a1 =~ m2.dtrsm(true, true, true, false, a1, m1)

puts "B.ztrsm(SideLeft, Uplo, TransA, DiagUnit, A) -> B'"
m_UD = cm1.upper(false) + Matrix_complex::identity(3)
m_UD_inv = Matrix_complex.new([1,0, -2,-3, -13, 33], [0,0, 1,0, -7,-8],[0,0, 0,0, 1,0])
assert m_UD * m_UD_inv == Matrix_complex::identity(3)
assert m_UD_inv.t * cm2 * c1 =~ cm2.ztrsm(true, true, true, true, c1, cm1)

puts "C.dsyr2k(Uplo, Trans, alpha, A, B, beta) -> C'"
res =  m1.t * m2 * a1 + m2.t * m1 * a1 + sm1 * a2
assert sm1.upper(true).dsyr2k(true, true, a1, m1, m2, a2) =~ res.upper(true)

puts "C.zsyr2k(Uplo, Trans, alpha, A, B, beta) -> C'"
m_U = csym1.upper(true)
m_L = csym1.lower(true)
w1 = cm1.t * cm2 * c1 + cm2.t * cm1 * c1 + csym1 * c2
w2 = cm1 * cm2.t * c1 + cm2 * cm1.t * c1 + csym1 * c2
assert m_U.zsyr2k(true, true, c1, cm1, cm2, c2) == w1.upper(true)
assert m_U.zsyr2k(true, false, c1, cm1, cm2, c2) == w2.upper(true)
assert m_L.zsyr2k(false, true, c1, cm1, cm2, c2) == w1.lower(true)
assert m_L.zsyr2k(false, false, c1, cm1, cm2, c2) == w2.lower(true)

STDERR.puts "\ndone."
