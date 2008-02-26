require File.join(File.dirname(__FILE__), "..", "spec_helper")

describe "Similarity" do
  
  describe "calculating the Tanimoto difference" do
    it "should return 0.0 if either of the sets are empty" do
      GSL::Similarity.tanimoto_coefficient(%w{ shirt shoes pants socks}, []).should be_close(0.0, 0.0001)
      GSL::Similarity.tanimoto_coefficient([], %w{ shirt skirt shoes }).should be_close(0.0, 0.0001)
      GSL::Similarity.tanimoto_coefficient([], []).should be_close(0.0, 0.0001)
    end
    
    it "should return a float" do
      GSL::Similarity.tanimoto_coefficient(%w{ shirt shoes pants socks}, %w{ shirt skirt shoes }).should be_close(0.4, 0.0001)
    end
  end
end