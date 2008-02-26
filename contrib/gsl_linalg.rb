# $Id: gsl_linalg.rb,v 1.6 2003/07/04 19:45:38 aerpenbeck Exp $

module GSL
  class Vector
    def outer(v)
      n = self.size
      raise("Error: vectors not same size") if n != v.size
      vw = self*v
      a = Matrix.new(n,n)
      (0...n).each{|i|
        (0...n).each{|j| a.set(i,j, self[i] * v[j])}
      }
      a
    end

    def house
      Matrix.identity(self.size) - self.outer(self) * (2/(self*self))
    end

  end

 class Vector_complex
    # self v^T
    def outer(v)
      n = self.size
      raise("Error: complex vectors not same size") if n != v.size
      #vw = self*v
      a = Matrix_complex.new(n,n)
      (0...n).each{|i|
        (0...n).each{|j| z =  self[i] * v[j]; a[i,j] = z}
      }
      a
    end
    
    # self v^H
    def outerc(v)
      n = self.size
      raise("Error: complex vectors not same size") if n != v.size
      #vw = self*v
      a = Matrix_complex.new(n,n)
      (0...n).each{|i|
        (0...n).each{|j| z =  self[i] * v[j].conjugate; a[i,j] = z}
      }
      a
    end

    def house
      Matrix_complex.identity(self.size) - 
        self.outerc(self) * (Complex.new2(2,0) * (self.zdotc(self).inverse))
    end
  
    def house_(tau)
      Matrix_complex.identity(self.size) - self.outerc(self) * tau 
    end
  end
 
  def Matrix::identity(n)
    a = Matrix.new(n,n)
    a.set_identity
    a
  end

  def Matrix_complex::identity(n)
    a = Matrix_complex.new(n,n)
    a.set_identity
    a
  end

  class Matrix
    def house_v(n)
      a = self.col(n)
      (0..n-1).each{|i| a.set(i, 0)}
      a.set(n, 1)
      a
    end

    def house_m(n)
      house_v(n).house
    end
  end
  
end
