#!/bin/env ruby

# $Id: array.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"

# how to use module functions from Stats as methods for Array

class Array
  def mean
    GSL::Stats::mean1 self
  end
  
# if you need stride:
# def mean(stride=1)
#   GSL::Stats::mean self, stride
# end
# etc.

  def variance
    GSL::Stats::variance1 self
  end
  def variance_m(m)
    GSL::Stats::variance1_m self, m
  end
  def variance_with_fixed_mean(m)
    GSL::Stats::variance1_with_fixed_mean self, m
  end
  def sd
    GSL::Stats::sd1 self
  end
  def sd_m(m)
    GSL::Stats::sd1_m self, m
  end
  def sd_with_fixed_mean(m)
    GSL::Stats::sd1_with_fixed_mean self, m
  end

  def absdev
    GSL::Stats::absdev self, 1
  end
  def absdev_m(m)
    GSL::Stats::absdev self, 1, m
  end

  def skew
    GSL::Stats::skew self, 1
  end
  def skew_m_sd(m, sd)
    GSL::Stats::skew self, 1, m, sd
  end
  def kurtosis
    GSL::Stats::kurtosis self, 1
  end
  def kurtosis_m_sd(m, sd)
    GSL::Stats::kurtosis_m_sd self, 1, m, sd
  end
  
  def autocorrelation
    GSL::Stats::autocorrelation self, 1
  end
  def autocorrelation_m(m)
    GSL::Stats::autocorrelation_m self, 1, m
  end

  def max_index
    GSL::Stats::max_index self, 1
  end
  def min_index
    GSL::Stats::min_index self, 1
  end
  def minmax
    GSL::Stats::minmax self, 1
  end
  def minmax_index
    GSL::Stats::minmax_index self, 1
  end

  # etc.
end

a = [1,2,3,4,5,6]
puts a.mean
