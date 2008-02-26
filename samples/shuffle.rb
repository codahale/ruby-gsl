#!/bin/env ruby

# $Id: shuffle.rb,v 1.3 2004/11/13 22:50:13 aerpenbeck Exp $

require "GSL"
include GSL::Random

# shuffling and sampling
# shuffle, choose, and sample are module functions and methods
# for the Array class

STDERR.puts "Running tests for RND..."

r = RNG.new
puts "Shuffling as a module function"
data = [[1,2,3,4,5], [1.1,2.2,3.3,4.4,5.5], ["a","b","c","d","e"]]
data.each do |a|
  puts "\nShuffling #{a[0].class}..."
  puts a.join(",")
  10.times do
    RND.shuffle r, a
    puts a.join(",")
  end
end

puts "\nShuffling as a method of Array"
data = [[1,2,3,4,5], [1.1,2.2,3.3,4.4,5.5], ["a","b","c","d","e"]]
data.each do |a|
  puts "\nShuffling #{a[0].class}..."
  puts a.join(",")
  10.times do
    a.shuffle r
    puts a.join(",")
  end
end

lotto = Array.new 49
lotto.each_index { |i| lotto[i] = i+1 }

puts "\nChoosing elements as a module function"
10.times do
  a = RND.choose r, 6, lotto
  puts a.join(",")
end

puts "\nChoosing elements as a method of Array"
10.times do
  a = lotto.choose r, 6
  puts a.join(",")
end

dice = [1,2,3,4,5,6]
puts "\nSampling elements as a module function"
10.times do
  a = RND.sample r, 10, dice
  puts a.join(",")
end

puts "\nSampling elements as a method of Array"
10.times do
  a = dice.sample r, 10
  puts a.join(",")
end

STDERR.puts "\ndone."
