#!/bin/env ruby

# $Id: error.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# how to use error handling

def show(str, x, res)
  if res.class == Result
    printf "  %s(%g) = %g (err = %g)\n", str, x, res.val, res.err
  else
    printf "  %s(%g) = %g\n", str, x, res
  end
end

STDERR.puts "Error handling..."

puts "\nThis is ok:"
x = 7.001
r = Special::Legendre::Q1(x)
show "Q1", x, r

puts "\nFor Legendre::Q1(x) the x argument must be != 1"
x = 1

puts "\nTurn off error handling:"
GSL::set_handler(ERR_OFF)
r = Special::Legendre::Q1_e(x)
show "Q1_e", x, r

puts "\nCatch GSL exception:"
GSL::set_handler(ERR_RUBY)
begin
  r = Special::Legendre::Q1_e(x)
  show "Q1_e", x, r
rescue GSLException
  puts "Ouch: #{$!}"
end

puts "\nThis will abort the program:"
GSL::set_handler(ERR_INTERN)
r = Special::Legendre::Q1_e(x)
show "Q1_e", x, r

STDERR.puts "\ndone."
