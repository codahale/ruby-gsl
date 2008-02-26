#!/bin/env ruby

# $Id: sum.rb,v 1.2 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL

STDERR.puts "Running tests for Series acceleration..."

N = 20
w= Sum::Levin.new N
zeta_2 = Math::M_PI * Math::M_PI / 6.0
n = 0
t = Array.new N
sum = 0

while (n < N)
  np = n + 1.0
  t[n] = 1.0 / (np * np)
  sum += t[n]
  n += 1
end

sum_accel, err = w.accel(t)

printf "\nterm-by-term sum = %.16f using %d terms\n", sum, N
printf "term-by-term sum = %.16f using %d terms\n", w.sum_plain, w.terms_used
printf "exact value      = %.16f\n", zeta_2
printf "accelerated sum  = %.16f using %d terms\n", sum_accel, w.terms_used
printf "estimated error  = %.16f\n", err
printf "actual error     = %.16f\n", sum_accel - zeta_2

STDERR.puts "\ndone."
