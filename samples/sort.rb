#!/bin/env ruby

# $Id: sort.rb,v 1.3 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL
# the following files can be found in the contrib folder
require 'gsl_util'
require 'assert'

v1 = Vector.new([3,2,6,4,1,2])
v1_dup = v1.clone
v1_dup.sort!
assert v1_dup.to_a == [1.0, 2.0, 2.0, 3.0, 4.0, 6.0]
perm1 = v1.sort_index
assert perm1.to_a == [4, 1, 5, 0, 3, 2]
assert v1_dup == Matrix.perm(perm1) * v1

assert v1.sort_smallest(4) == [1.0, 2.0, 2.0, 3.0]
assert v1.sort_largest(4) == [6.0, 4.0, 3.0, 2.0]

a1 = v1.smallest_index(4)
assert a1 == [4.0, 5.0, 1.0, 0.0]
# a1 != perm1.to_a[0,4] (unstable sorting), but:
a2 = []
a1.size.times{|i| a2 << v1[a1[i]]}
a3 = []
a1.size.times{|i| a3 << v1[perm1.to_a[i]]}
assert a2 == a3 && a2 == [1.0, 2.0, 2.0, 3.0]

a4 = v1.largest_index(4)
assert a4 == [2.0, 3.0, 0.0, 1.0]
