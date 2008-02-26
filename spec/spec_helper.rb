begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

$:.unshift(File.expand_path(File.join(File.dirname(__FILE__), "..", "lib")))

require "gsl"