class Array
  
  # Returns the array's correlation with +y+, another array of equal size.
  def correlation(y)
    if size == y.size
      return GSL::Stats.correlation1(self, y)
    else
      raise ArgumentError, "cannot correlated two differently-sized arrays"
    end
  end
  
end