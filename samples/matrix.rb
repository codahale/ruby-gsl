#!/bin/env ruby

# $Id: matrix.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

DUMP = "matrix.dump"
File.delete(DUMP) if File.exists?(DUMP)

STDERR.puts "Running tests for Matrix..."

m = Matrix.new 10, 10
0.upto(9) do |i|
  0.upto(9) do |j|
    m.set i, j, Math::sin(i) + Math::cos(j)
  end
end

puts "writing matrix"
f = File.open DUMP, "w"
m.fwrite f
f.close

puts "\nreading matrix"
n = Matrix.new 10, 10
f = File.open DUMP, "r"
n.fread f
f.close

k = 0
0.upto(9) do |i|
  0.upto(9) do |j|
    d1 = m.get i, j
    d2 = n.get i, j
    k += 1 if d1 != d2
  end
end

puts "\ndifferences = #{k} (should be zero)"

0.upto(9) do |j|
  col = m.column j
  d = 0.0
  col.each do |i|
    d += i*i
  end
  printf "matrix column %d, norm = %g\n", j, Math::sqrt(d)
end

puts "\neigenvalues and eigenvectors"

n = Matrix.new [1.0, 1/2.0, 1/3.0, 1/4.0], [1/2.0, 1/3.0, 1/4.0, 1/5.0],
               [1/3.0, 1/4.0, 1/5.0, 1/6.0], [1/4.0, 1/5.0, 1/6.0, 1/7.0]
vals, vecs = n.eigen_symmv
Eigen.symmv_sort vals, vecs, Eigen::SORT_VAL_ASC
vals.to_a.each_index do |i|
  printf "l = %.3f\n", vals.get(i)
  vecs.get_col(i).printf "%.3f"
  puts
end

# had to use powers of 2 to prevent rounding errors in equality check below,
# because Ruby marshalling uses decimal representation that loses last bit(s)
n = Matrix.new [1.0, 1/2.0, 1/4.0, 1/8.0], [2/1.0, 2/2.0, 2/2.0, 2/8.0],
               [4/1.0, 4/2.0, 4/4.0, 4/8.0]

marn = Marshal.dump(n)
newn = Marshal.load(marn)

newn.size1.times { |x|
  newn.size2.times { |y|
    if (n.get(x,y) != newn.get(x,y))
      puts "Marshalling FAILED"
      puts "For #{x} and #{y} and #{n.get(x,y)} and #{newn.get(x,y)}"
      exit(1)
    end
  }
}
puts "Marshalling works"

STDERR.puts "\ndone."

File.delete(DUMP) if File.exists?(DUMP)


