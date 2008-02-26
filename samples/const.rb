#!/bin/env ruby

# $Id: const.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL::Const

# Test physical constants

STDERR.puts "Running tests for physical constants..."

c = MKSA::SPEED_OF_LIGHT
au = MKSA::ASTRONOMICAL_UNIT
minutes = MKSA::MINUTE

r_earth = 1.00 * au
r_mars = 1.52 * au

t_min = (r_mars - r_earth) / c
t_max = (r_mars + r_earth) / c

puts "\nlight travel time from Earth to Mars:"
printf "min = %.1f minutes\n", t_min / minutes
printf "max = %.1f minutes\n", t_max / minutes

STDERR.puts "\ndone."
