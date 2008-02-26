#!/bin/env ruby

# $Id: vector.rb,v 1.3 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL

DUMP = "vector.dump"
File.delete(DUMP) if File.exists?(DUMP)

STDERR.puts "Running tests for Vector..."

puts "the C way"
v = Vector.new 4
0.upto(3) do |i|
  v[i]= 1.23 + i
end
0.upto(3) do |i|
  printf "v[%d] = %g\n", i, v[i]
end

puts "the Ruby way"
a = [1,2,3,4]
i = 0
v = Vector.new a
v.each do |x|
  printf "v[%d] = %g\n", i, v[i]
  i += 1
end

v = Vector.new((0..99).to_a)
f = File.open DUMP, "w"
v.fprintf f, "%.5g"
f.close

v = Vector.new [ 1.0, 2/3.0, 3.5 ]

marv = Marshal.dump(v)
newv = Marshal.load(marv)

v.size.times { |i|
  if v[i] != newv[i]
    puts "Marshalling FAILED"
    exit 1
  end
}

puts "Marshalling works"

STDERR.puts "\ndone."

File.delete(DUMP) if File.exists?(DUMP)

