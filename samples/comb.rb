#!/bin/env ruby

# $Id: comb.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test combinations

STDERR.puts "Running tests for combinations..."

n = 4
k = 3
0.upto(4) do |k|
  c = Combination.new n, k
  c.init_first

  printf "\nn = %d, k = %d\n", c.n, c.k
  puts "The valid is #{c.is_valid?}"
  puts "first combination: " +  c.to_a.join(" ")

  0.upto(k-1) do |i|
    puts c.get(i)
  end

  puts "  each: "
  c.each do |cc|
    puts "    " + cc.to_a.join(" ")
  end

  puts "  reverse_each: "
  c.reverse_each do |cc|
    puts "    " + cc.to_a.join(" ")
  end
end

c2 = Combination.new_ary [0,1,3], 4
puts "\nnew_ary: "
c2.each do |k|
  puts "  " + k.to_a.join(" ")
end

# generate all "Lotto" numbers :-)
#c = Combination.new(49, 6)
#c.init_first
#File.open("lotto.dat", "w") do |f|
  #c.each do |k|
    #k.fprintf(f, "%Zu\t") 
    #f.puts
  #end
#end
6.times { |k|
  c = Combination.new(5, k)
  c.each { |c2|
  mc2 = Marshal.dump(c2)
  nc2 = Marshal.load(mc2)
  unless c2 == nc2
    puts "PROBLEM: Marshalling failed! with n=#{n}, k=#{k}"
    exit(0)
  end
  }
}

puts "Marshalling works"
STDERR.puts "\ndone."
