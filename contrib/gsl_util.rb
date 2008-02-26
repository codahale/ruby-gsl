# This module allows * for matrix multiplication and =~ for approximate
# equality; for instance
# 
# require "GSL"; include GSL
# require 'gsl_util'
#
# puts "QR_solve"
# a = Matrix.new([2,0,1],[6,2,0],[-3,-1,-1])
# b = Vector.new([1,0,1])
# qr, tau = a.QR_decomp
# x = qr.QR_solve(tau, b)
# a*x =~ b   #=> true 
# The method =~ is defined for the classes
#    Matrix, Matrix_complex, Vector, Vector_complex, Float
# 
module GSL
  class Matrix
    def mltp(a)
      if a.class == GSL::Matrix
	m = self.mult(a)
      elsif a.class == GSL::Vector
	m = self.mult_vect(a)
      elsif (a.class == Fixnum || a.class == Bignum || a.class == Float)
	m = self.scale(a)
      end
      m
    end
    alias * mltp
    
    # very inefficient implementation
    def pow(n)
      if (n < 1)
	raise("Error: only positive matrix powers allowed")
      elsif (n == 1)
	return self
      else
	p = self
	(2..n).each{ p *= self}
	return p
      end
    end
  # end class Matrix 
    def fl_eq(a)
      if a.class == GSL::Matrix
        return Matrix::float_equal(self,a, 1.0e-15)
      elsif a.class == Array
	if a[0].class == GSL::Matrix
	return Matrix::float_equal(self,a[0], a[1])
      else raise("Error: not a comparison between matrices")
      end
      else raise("Error: not a comparison between matrices")
      end
    end
    alias =~ fl_eq
  end 

  class Matrix_complex
    def mltp(a)
      if a.class == GSL::Matrix_complex
	m = self.mult(a)
      elsif a.class == GSL::Vector_complex
	m = self.mult_vect(a)
      elsif a.class == GSL::Complex
	m = self.scale(a)
      elsif (a.class == Float || a.class == Fixnum)
	m = self.scale(Complex.new2(a,0))
      end
      m
    end
    alias * mltp
    
    # very inefficient implementation
    def pow(n)
      if (n < 1)
	raise("Error: only positive matrix powers allowed")
      elsif (n == 1)
	return self
      else
	p = self
	(2..n).each{ p *= self}
	return p
      end
    end
  # end class Matrix_complex 
    def fl_eq(a)
      if a.class == GSL::Matrix_complex
        return Matrix_complex::float_equal(self,a, 1.0e-15)
      elsif a.class == Array
	if a[0].class == GSL::Matrix_complex
	  return Matrix_complex::float_equal(self,a[0], a[1])
	else raise("Error: not a comparison between matrices")
	end
      else raise("Error: not a comparison between matrices")
      end
    end
    alias =~ fl_eq
  end 

end

module GSL
  class Vector
    def Vector::zero(n)
      a = Vector.new(n)
      a.set_zero
      a
    end
    
    def mltp(a)
      if a.class == GSL::Vector
	v = self.dot(a)
      elsif a.class == GSL::Matrix
	v = a.transpose * self
      elsif (a.class == Fixnum || a.class == Bignum || a.class == Float)
	v = self.scale(a)
      end
      v
    end
    alias * mltp

    def fl_eq(a)
      if a.class == GSL::Vector
        return Vector::float_equal(self,a, 1.0e-15)
      elsif a.class == Array
	if a[0].class == GSL::Vector
	  return Vector::float_equal(self,a[0], a[1])
	else raise("Error: not a comparison between vectors")
	end  
      else raise("Error: not a comparison between vectors")
      end
    end

 alias =~ fl_eq
  end

  
  class Vector_complex
    def mltp(a)
      if a.class == GSL::Vector_complex
	v = self.zdotc(a)	
      elsif a.class == GSL::Complex
	v = self.scale(a)
      elsif (a.class == Float || a.class == Fixnum)
	v = self.scale(Complex.new2(a,0))
      end
      v
    end
    alias * mltp

    def fl_eq(a)
      if a.class == GSL::Vector_complex
        return Vector_complex::float_equal(self,a, 1.0e-15)
      elsif a.class == Array
	if a[0].class == GSL::Vector_complex
	  return Vector_complex::float_equal(self,a[0], a[1])
	else raise("Error: not a comparison between complex vectors")
	end  
      else raise("Error: not a comparison between complex vectors")
      end
    end

 alias =~ fl_eq
    
  end


  
end

module GSL

  class Complex
    def fl_eq(a)
      if (a.class == Complex)
	return Complex::float_equal(self,a,1.0e-15)
      elsif (a.class == Float || a.class == Fixnum)
	if (self.imag =~ 0)
	  return (self.real =~ a)
	else
	  return false
	end
      end
    end
    alias =~ fl_eq
  end
  
end

class Float
  def fl_eq(a)
    if (a.class == Float || a.class == Fixnum)
      return Math::float_equal(self,a,1.0e-15)
    elsif a.class == Array
      if (a[0].class == Float || a[0].class == Fixnum) 
	return Math::float_equal(self,a[0], a[1])
      else 
	raise("Error: not a comparison between floats")
      end
    end
  end
  alias =~ fl_eq
end


