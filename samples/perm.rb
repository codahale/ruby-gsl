#!/bin/env ruby

# $Id: perm.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test permutations

DUMP = "perm.dump"
File.delete(DUMP) if File.exists?(DUMP)

STDERR.puts "Running permutation tests..."

perm = Permutation.new 10
puts "\n  size  = #{perm.size}"
print "  perm  = "
perm.fprintf STDOUT, " %u"
perm.swap(2, 4).swap(3, 5)
print "\n  swap  = "
#puts perm.to_a.join(" ")
perm.dump
perm.next
print "\n  next  = "
perm.dump
perm.prev
print "\n  prev  = "
perm.dump
print "\n  valid: ", perm.is_valid? ? "yes" : "no", "\n"
begin
  print "\nwriting to file..."
  f = File.new DUMP, "w"
  perm.fwrite f
  f.close
  puts "ok"
  print "reading from file..."
  f = File.new DUMP, "r"
  perm = Permutation.new 10
  perm.fread f
  f.close
  puts "ok"
  perm.dump
  puts "\nread/write: ok"
rescue IOError
  STDERR.print "Caught exception: #{$!}\n"
end

puts "\ntesting each:"
perm = Permutation.new 4
perm.each {|p| p.dump; printf "\n"}

File.delete(DUMP) if File.exists?(DUMP)

STDERR.puts "\ndone."
