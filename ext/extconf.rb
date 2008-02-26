require "mkmf"

# $Id: extconf.rb,v 1.3 2004/11/13 22:50:12 aerpenbeck Exp $

$CFLAGS = "-Wall"

have_library "m", "exp"
have_library "gslcblas"
have_library "gsl", "gsl_vector_free"

if have_library("gsl", "gsl_linalg_complex_householder_hv")
  $CFLAGS << " -DHAVE_COMPLEX_HV"
end
if CONFIG["MAJOR"].to_i >= 1 && CONFIG["MINOR"].to_i >= 8
  $CFLAGS << " -DHAVE_DEFINE_ALLOC_FUNCTION"
end
if CONFIG["host_os"] == "cygwin"
  $CFLAGS << " -DCYGWIN"
end

create_makefile "gsl_functions"
