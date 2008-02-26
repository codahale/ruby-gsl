require File.join(File.dirname(__FILE__), "..", "spec_helper")

require "gsl/array"

describe Array do
  describe "calculating the correlation coefficient" do
    it "should return a real number" do
      [1, 2, 3].correlation([1, 2, 3]).should be_close(1.0, 0.001)
    end
  end
end