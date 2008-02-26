class Array
  
  # Returns the array's correlation with +y+, another array of equal size.
  def correlation(y)
    if size == y.size
      return GSL::Stats.correlation1(self, y)
    else
      raise ArgumentError, "cannot correlated two differently-sized arrays"
    end
  end
  
  def mean
    return GSL::Stats.mean1(self)
  end
  
  def variance
    return GSL::Stats.variance1(self)
  end
  
  def sd
    return GSL::Stats::sd1(self)
  end
end