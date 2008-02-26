
# $Id: assert.rb,v 1.1 2004/11/13 22:50:11 aerpenbeck Exp $

# simple assert

def assert(x)
   caller(x) if (!x)
end

