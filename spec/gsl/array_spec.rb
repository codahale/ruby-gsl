require File.join(File.dirname(__FILE__), "..", "spec_helper")

require "gsl/array"

describe Array do
  describe "calculating the correlation coefficient" do
    it "should return a real number" do
      [1, 2, 3].correlation([1, 2, 3]).should be_close(1.0, 0.001)
    end
    
    it "should raise an ArgumentError if the two arrays are of different sizes" do
      lambda { [1, 2, 3].correlation([1, 2]) }.should raise_error(ArgumentError)
    end
  end
  
  describe "calculating the mean" do
    it "should return a real number" do
      [1, 2, 3].mean.should be_close(2.0, 0.0001)
    end
  end
  
  describe "calculating the variance" do
    it "should return a real number" do
      [1, 2, 3].variance.should be_close(1.0, 0.0001)
    end
  end
  
  describe "calculating the standard deviation" do
    it "should return a real number" do
      [2, 2, 2].sd.should be_close(0.0, 0.0001)
    end
  end
  
  describe "calculating the Tanimoto coefficient" do
    it "should return a real number" do
      [1, 2, 3].tanimoto_coefficient([1, 2, 3]).should be_close(1.0, 0.0001)
      [1, 2, 3].tanimoto_coefficient([4, 5, 6]).should be_close(0.0, 0.0001)
    end
  end
  
  describe "calculating Dice's coefficient" do
    it "should return a real number" do
      [1, 2, 3].dice_coefficient([1, 2, 3]).should be_close(1.0, 0.0001)
      [1, 2, 3].dice_coefficient([4, 5, 6]).should be_close(0.0, 0.0001)
    end
  end
end