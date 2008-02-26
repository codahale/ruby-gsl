#!/bin/env ruby

# $Id: siman.rb,v 1.2 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL

r = Random::RNG.new

energy = Proc.new {|x| Math::exp(-Math::pow_2(x[0]-1.0)) * Math::sin(8*x[0])}
metric = Proc.new {|x, y| (x[0] - y[0]).abs}
step = Proc.new {|x, u, ss|
  x[0] = 2 * u * ss - ss + x[0]
}
show = Proc.new {|x| printf("12%g\n", x[0])}

f = Siman.new

f.energy = energy 
f.metric = metric
f.step = step
f.print = show

conf = [15.5]

f.search r, conf
