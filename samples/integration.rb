#!/bin/env ruby

# $Id: integration.rb,v 1.2 2004/11/13 22:50:12 aerpenbeck Exp $

require "GSL"
include GSL

# Test numerical integration

STDERR.puts "Running numerical integration tests..."

#f = Function.new{ |x| x*x }
#f2 = Function.new { |x| Math::exp(-x*x*0.5) }

#p f.integration_qng(0.0, 2.0, 0.0, 1.0e-7)
#p f.integration_qag(0.0, 2.0, 0.0, 1.0e-7, 1, 1000)
#p f.integration_qags(0.0, 2.0, 0.0, 1.0e-7, 1000)

#a, = f2.integration_qagi(0.0, 1.0e-7, 1000)

#t = Integration::QAWSTable.new(4, 5, 1, 0)
#t.set([7.0, 2.0, 0, 1])
#p t.to_a

#p [34, 5, 0, 1].to_gsl_integration_qaws_table
#p t.to_a

#p f3.integration_qags(0, 1, 0, 1e-7, 1000)

#aa = Integration::QAWOTable.new(3.0, 4.0, Integration::SINE, 9)
#aa.set_length(123)
#p aa.to_a

f3 = Function.new { |x| Math::log(x) / Math::sqrt(x) }
expected = -4.0
result, error, intervals, status = f3.integration_qags(0.0, 1.0, 0.0, 1.0e-7, 1000)

printf("\nresult          = % .18f\n", result);
printf("exact result    = % .18f\n", expected);
printf("estimated error = % .18f\n", error);
printf("actual error    = % .18f\n", result - expected);
printf("intervals       = %d\n", intervals);

STDERR.puts "\ndone."
