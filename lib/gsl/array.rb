class Array
  
  # Returns the array's correlation with +y+, another array of equal size.
  def correlation(y)
    if size == y.size
      return GSL::Stats.correlation1(self, y)
    else
      raise ArgumentError, "cannot correlate two differently-sized arrays"
    end
  end
  
  # Returns the arithmetic mean of the elements of the array.
  def mean
    return GSL::Stats.mean1(self)
  end
  
  # Returns the variance of the elements of the array.
  def variance
    return GSL::Stats.variance1(self)
  end
  
  # Returns the standard distribution of the elements of the array.
  def sd
    return GSL::Stats.sd1(self)
  end
  
  # Returns the Tanimoto coefficient of the array and +y+.
  def tanimoto_coefficient(y)
    return GSL::Similarity.tanimoto_coefficient(self, y)
  end
end