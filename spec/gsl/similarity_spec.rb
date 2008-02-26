require File.join(File.dirname(__FILE__), "..", "spec_helper")

describe "Similarity" do
  
  describe "calculating the Tanimoto coefficient" do
    it "should return 0.0 if either of the sets are empty" do
      GSL::Similarity.tanimoto_coefficient(%w{ shirt shoes pants socks}, []).should be_close(0.0, 0.0001)
      GSL::Similarity.tanimoto_coefficient([], %w{ shirt skirt shoes }).should be_close(0.0, 0.0001)
      GSL::Similarity.tanimoto_coefficient([], []).should be_close(0.0, 0.0001)
    end
    
    it "should return a float" do
      GSL::Similarity.tanimoto_coefficient(%w{ shirt shoes pants socks }, %w{ shirt skirt shoes }).should be_close(0.4, 0.0001)
    end
  end
  
  describe "calculating Dice's coefficient" do
    it "should return 0.0 if either of the sets are empty" do
      GSL::Similarity.dice_coefficient(%w{ shirt shoes pants socks}, []).should be_close(0.0, 0.0001)
      GSL::Similarity.dice_coefficient([], %w{ shirt skirt shoes }).should be_close(0.0, 0.0001)
      GSL::Similarity.dice_coefficient([], []).should be_close(0.0, 0.0001)
    end
    
    it "should return a float" do
      GSL::Similarity.dice_coefficient(%w{ ni ig gh ht }, %w{ na ac ch ht }).should be_close(0.25, 0.0001)
    end
  end
end