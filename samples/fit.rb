#!/bin/env ruby

# $Id: fit.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL::Fit

# Test Least-Squares Fitting
# Usage: 
# $ ./fit.rb > tmp
# $ for n in data fit hi lo;  do grep "^$n" tmp | cut -d ':' -f 2 > $n; done
# $ graph -TX -X x -Y y -y 0 20 -m 0 -S 2 -Ie data -S 0 -I a -m 1 fit -m 2 hi lo

STDERR.puts "Running tests for Least-Squares Fitting..."

x = [1970, 1980, 1990, 2000]
y = [  12,   11,   14,   13]
w = [ 0.1,  0.2,  0.3,  0.4]

res = wlinear(x, w, y)
c0 = res[0]
c1 = res[1]
cov00 = res[2]
cov01 = res[3]
cov11 = res[4]
chisq = res[5]

puts "Best fit: Y = %g + %g X" % [c0, c1]
puts "Covariance matrix:"
puts "  [ %g, %g\n    %g, %g ]" % [cov00, cov01, cov01, cov11]
puts "Chi-squared: #{chisq}"

x.each_index do |i|
  puts "data: #{x[i]} #{y[i]} #{1/Math::sqrt(w[i])}"
end

(-30).upto(130) do |i|
  xf = x[0] + (i/100.0) * (x[3]-x[0])
  yf, yf_err = linear_est(xf, c0, c1, cov00, cov01, cov11)
  puts "fit: #{xf} #{yf}"
  puts "hi: #{xf} #{yf+yf_err}"
  puts "lo: #{xf} #{yf-yf_err}"
end

STDERR.puts "\ndone."
