#!/bin/env ruby

# $Id: special.rb,v 1.3 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL
include GSL::Special

# This is just a simple test if some of the special functions work.

def show(str, x, res)
  print "  ", str, "("
  if x.class == Array
	print x.join(", ")
  else
	printf "%g", x
  end
  print ") = "
  if res.class == Result
    printf "%g (err = %g)\n", res.val, res.err
  else
    printf "%g\n", res
  end
end

STDERR.puts "Running special tests..."

puts "\nAiry:"
x = 3.0
r = Airy::Ai(x, PREC_DOUBLE)
show "Ai", x, r
r = Airy::Ai_e(x, PREC_DOUBLE)
show "Ai_e", x, r

puts "\nBessel:"
x = 5.0
r = Bessel::J0(x)
show "J0", x, r
r = Bessel::J0_e(x)
show "J0_e", x, r
a = Bessel::Kn_array(0, 20, 3.333)
print "  Kn_array: ", a.join(" "), "\n"

puts "\nClausen:"
x = 1.7
r = Clausen::clausen(x)
show "clausen", x, r
r = Clausen::clausen_e(x)
show "clausen_e", x, r

puts "\nConical:"
x = [0.1, 3.03]
r = Conical::P_mhalf(*x)
show "P_mhalf", x, r
r = Conical::P_mhalf_e(*x)
show "P_mhalf_e", x, r

puts "\nCoulomb:"
x = [2.2, 0.4]
r = Coulomb::hydrogenicR_1(*x)
show "hydrogenicR_1", x, r
r = Coulomb::hydrogenicR_1_e(*x)
show "hydrogenicR_1_e", x, r

x = [2, 0.02]
r = Coulomb::CL_e(*x)
show "CL_e", x, r
a = Coulomb::CL_array(2, 5, 0.02)  
puts a.join(" ")

puts "\nCoupling:"
x = [2,4,6,3,5,7]
r = Coupling::wigner_6j(*x)
show "6j", x, r
r = Coupling::wigner_6j_e(*x)
show "6j_e", x, r

puts "\nDawson:"
x = 0.1
r = Dawson::dawson(x)
show "dawson", x, r
r = Dawson::dawson_e(x)
show "dawson_e", x, r

puts "\nDebye"
x = 0.5
r = Debye::D1(x)
show "D1", x, r
r1 = Debye::D1_e(x)
show "D1_e", x, r

puts "\nDilog:"
x = 0.5
r = Dilog::dilog(x)
show "dilog", x, r
r2 = Dilog::dilog_e(x)
show "dilog_e", x, r2
re, im = Dilog::complex_dilog_e(0.5, Math::M_PI_2)
printf "  complex_dilog_e: (%g, %g) (%g, %g)\n", re.val, im.val, re.err, im.err

puts "\nElementary:"
r = Elementary::multiply_err_e(r1.val, r1.err, r2.val, r2.err)
print "  multiply_err: ", r.val, r.err, "\n"
r = r1 * r2
print "  r1 * r2     : ", r.val, r.err, "\n"

puts "\nEllInt:"
x = 0.3
r = EllInt::Ecomp(x, PREC_DOUBLE)
show "Ecomp", x, r
r = EllInt::Ecomp_e(x, PREC_DOUBLE)
show "Ecomp_e", x, r

puts "\nEllJac:"
sn, cn, dn = EllJac::e(10.2, 0.2)
print "  e: ", sn, " ", cn, " ", dn, "\n"

puts "\nError:"
x = 0.5
r = Error::erf(x)
show "erf", x, r
r = Error::erf_e(x)
show "erf_e", x, r

puts "\nExp:"
x = 4.5
r = Exp::expm1(x)
show "expm1", x, r
r = Exp::expm1_e(x)
show "expm1_e", x, r

puts "\nExpInt:"
x = 0.5
r = ExpInt::E1(x)
show "E1", x, r
r = ExpInt::E1_e(x)
show "E1_e", x, r

puts "\nFermiDirac:"
x = 0.7
r = FermiDirac::F1(x)
show "F1", x, r
r = FermiDirac::F1_e(x)
show "F1_e", x, r

puts "\nGamma:"
x = 5.7
r = Gamma::lngamma(x)
show "lngamma", x, r
r = Gamma::lngamma_e(x)
show "lngamma_e", x, r

puts "\nGegenbauer:"
x = [6, 1.3, 2.19]
r = Gegenbauer::poly_n(*x)
show "poly_n", x, r
r = Gegenbauer::poly_n_e(*x)
show "poly_n_e", x, r

puts "\nHyperg:"
x = [2, 3, 6.9]
r = Hyperg::F11_int(*x)
show "F11_int", x, r
r = Hyperg::F11_int_e(*x)
show "F11_int_e", x, r

puts "\nLaguerre:"
x = [1.6, -2.65]
r = Laguerre::L3(*x)
show "L3", x, r
r = Laguerre::L3_e(*x)
show "L3_e", x, r

puts "\nLambert:"
x = 8.888
r = Lambert::W0(x)
show "W0", x, r
r = Lambert::W0_e(x)
show "W0_e", x, r

puts "\nLegendre:"
x = 7.001
r = Legendre::Q1(x)
show "Q1", x, r
r = Legendre::Q1_e(x)
show "Q1_e", x, r

puts "\nLog:"
x = 7.7
r = Log::log_1px(x)
show "log_1px", x, r
r = Log::log_1px_e(x)
show "log_1px_e ", x, r

puts "\nPsi"
x = 69.2
r = Psi::psi(x)
show "psi", x, r
r = Psi::psi_e(x)
show "psi_e", x, r

puts "\nSynchrotron:"
x = 42.0
r = Synchrotron::S1(x)
show "S1", x, r
r = Synchrotron::S1_e(x)
show "S1_e", x, r

puts "\nTransport:"
x = 23.0
r = Transport::J4(x)
show "J4", x, r
r = Transport::J4_e(x)
show "J4_e", x, r

puts "\nTrig:"
x = 0.987
r = Trig::lncosh(x)
show "lncosh ", x, r
r = Trig::lncosh_e(x)
show "lncosh_e", x, r

puts "\nZeta:"
x = [2.8, 3.4]
r = Zeta::hzeta(*x)
show "hzeta", x, r
r = Zeta::hzeta_e(*x)
show "hzeta_e", x, r

STDERR.puts "\ndone."
