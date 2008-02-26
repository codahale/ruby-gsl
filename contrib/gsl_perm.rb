require "GSL"
include GSL

# contributed by Mario Pernici

# $Id: gsl_perm.rb,v 1.1 2003/07/04 20:51:46 aerpenbeck Exp $

module GSL
 class Permutation
  def permute(vect)
    p = self.to_linear
    res = []
    p.to_a.each{|e| res << vect[e]}
    res
  end
  def permute_inverse(vect)
   p = self.to_linear
   res = []
   p.inverse.to_a.each{|e| res << vect[e]}
   res
  end
 end

 class CanPermutation < Permutation
  def inverse
    p = self.to_linear.inverse.to_canonical
  end
  def CanPermutation.new_ary(a) raise "Undefined method: CanPermutation.new_ary"; end
  def CanPermutation.new2(a)  raise "Undefined method: CanPermutation.new2"; end
  def swap(a,b) raise "Undefined method: CanPermutation#swap"; end
  def next()  raise "Undefined method: CanPermutation#next"; end
  def prev()  raise "Undefined method: CanPermutation#prev"; end
  def each()  raise "Undefined method: CanPermutation#each"; end
  def undump() raise "Undefined method: CanPermutation#undump"; end
 end

end
