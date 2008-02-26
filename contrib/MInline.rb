require "rbconfig"
require "ftools"

module MInline

   def MInline::compile(a, foo)
##############################################
#  Configuration: rgsldir is the path of the ruby-gsl sources
       rgsldir = "../ext"
       tmpdir = "."  
       mod_name = "MonteCarlo_" + foo  
       src_name = "#{tmpdir}/#{mod_name}.c"
       so_name = "#{tmpdir}/#{mod_name}.#{Config::CONFIG["DLEXT"]}" 

# src = str1 + a + str2 + str3	 
# str1 header files
# a source to be compiled
# str2 functions
# str3 methods: insert the name foo
#
############################################################
# str1 header files
str1 = <<EOS1
#include "ruby.h"
#include "gsl/gsl_monte.h"
#include "gsl/gsl_monte_plain.h"
#include "gsl/gsl_monte_miser.h"
#include "gsl/gsl_monte_vegas.h"
#include "gsl/gsl_rng.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"
EOS1

############################################################
# str2 functions
str2 = <<EOS2

/* Monte Carlo Integration */

/* PLAIN Monte Carlo */

static VALUE MPlain_init(VALUE self, VALUE dim) {
  gsl_monte_plain_state * ptr;
  Data_Get_Struct(self, gsl_monte_plain_state, ptr);
  return gsl_monte_plain_init(ptr);
}

static void MPlain_free(void * p) {
  gsl_monte_plain_free(p);
}

static VALUE MPlain_new(VALUE klass, VALUE dim) {
  VALUE plain;
  gsl_monte_plain_state * ptr;

  ptr = gsl_monte_plain_alloc(NUM2INT(dim));
  plain = Data_Wrap_Struct(klass, 0, MPlain_free, ptr);
  {
    VALUE tmp[1];
    tmp[0] = dim;
    rb_obj_call_init(plain, 1, tmp);
  }
  return plain;
}

static VALUE MPlain_integrate(VALUE self, VALUE xl, VALUE xu,
  VALUE calls, VALUE r) {
  gsl_monte_plain_state * sptr;
  gsl_monte_function * fptr;
  gsl_rng * rptr;
  double * my_xl, * my_xu;
  double result, abserr;
  int ret;

  if (RARRAY(xl)->len != RARRAY(xu)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size."); // end

  Data_Get_Struct(self, gsl_monte_plain_state, sptr);
  fptr = &MC_FNCT;
  Data_Get_Struct(r, gsl_rng, rptr);

  COPYRUBYARRAY(xl, my_xl);
  COPYRUBYARRAY(xu, my_xu);
  
  ret = gsl_monte_plain_integrate(fptr, my_xl, my_xu, RARRAY(xl)->len,
    NUM2INT(calls), rptr, sptr, &result, &abserr);

  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

/* MISER */

static VALUE MMiser_init(VALUE self, VALUE dim) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return gsl_monte_miser_init(ptr);
}

static void MMiser_free(void * p) {
  gsl_monte_miser_free(p);
}

static VALUE MMiser_new(VALUE klass, VALUE dim) {
  VALUE miser;
  gsl_monte_miser_state * ptr;

  ptr = gsl_monte_miser_alloc(NUM2INT(dim));
  miser = Data_Wrap_Struct(klass, 0, MMiser_free, ptr);
  {
    VALUE tmp[1];
    tmp[0] = dim;
    rb_obj_call_init(miser, 1, tmp);
  }
  return miser;
}

static VALUE MMiser_integrate(VALUE self, VALUE xl, VALUE xu,
  VALUE calls, VALUE r) {
  gsl_monte_miser_state * sptr;
  gsl_monte_function * fptr;
  gsl_rng * rptr;
  double * my_xl, * my_xu;
  double result, abserr;
  int ret;

  if (RARRAY(xl)->len != RARRAY(xu)->len)
	rb_raise(rb_eArgError, "Arrays must be of same size."); // end

  Data_Get_Struct(self, gsl_monte_miser_state, sptr);
  fptr = &MC_FNCT;
  Data_Get_Struct(r, gsl_rng, rptr);

  COPYRUBYARRAY(xl, my_xl);
  COPYRUBYARRAY(xu, my_xu);
  
  ret = gsl_monte_miser_integrate(fptr, my_xl, my_xu, RARRAY(xl)->len,
    NUM2INT(calls), rptr, sptr, &result, &abserr);

  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

static VALUE MMiser_get_estimate_frac(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return rb_float_new(ptr->estimate_frac);
}

static VALUE MMiser_set_estimate_frac(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->estimate_frac = NUM2DBL(v);
  return self;
}

static VALUE MMiser_get_min_calls(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return INT2FIX(ptr->min_calls);
}

static VALUE MMiser_set_min_calls(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->min_calls = NUM2INT(v);
  return self;
}

static VALUE MMiser_get_min_calls_per_bisection(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return INT2FIX(ptr->min_calls_per_bisection);
}

static VALUE MMiser_set_min_calls_per_bisection(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->min_calls_per_bisection = NUM2INT(v);
  return self;
}

static VALUE MMiser_get_alpha(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return rb_float_new(ptr->alpha);
}

static VALUE MMiser_set_alpha(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->alpha = NUM2DBL(v);
  return self;
}

static VALUE MMiser_get_dither(VALUE self) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  return rb_float_new(ptr->dither);
}

static VALUE MMiser_set_dither(VALUE self, VALUE v) {
  gsl_monte_miser_state * ptr;
  Data_Get_Struct(self, gsl_monte_miser_state, ptr);
  ptr->dither = NUM2DBL(v);
  return self;
}

/* VEGAS */

static VALUE MVegas_init(VALUE self, VALUE dim) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return gsl_monte_vegas_init(ptr);
}

static void MVegas_free(void * p) {
  gsl_monte_vegas_free(p);
}

static VALUE MVegas_new(VALUE klass, VALUE dim) {
  VALUE vegas;
  gsl_monte_vegas_state * ptr;

  ptr = gsl_monte_vegas_alloc(NUM2INT(dim));
  vegas = Data_Wrap_Struct(klass, 0, MVegas_free, ptr);
  {
    VALUE tmp[1];
    tmp[0] = dim;
    rb_obj_call_init(vegas, 1, tmp);
  }
  return vegas;
}

static VALUE MVegas_integrate(VALUE self, VALUE xl, VALUE xu,
  VALUE calls, VALUE r) {
  gsl_monte_vegas_state * sptr;
  gsl_monte_function * fptr;
  gsl_rng * rptr;
  double * my_xl, * my_xu;
  double result, abserr;
  int ret;

  if (RARRAY(xl)->len != RARRAY(xu)->len)
    rb_raise(rb_eArgError, "Arrays must be of same size."); // end

  Data_Get_Struct(self, gsl_monte_vegas_state, sptr);
  fptr = &MC_FNCT;
  Data_Get_Struct(r, gsl_rng, rptr);

  COPYRUBYARRAY(xl, my_xl);
  COPYRUBYARRAY(xu, my_xu);
  
  ret = gsl_monte_vegas_integrate(fptr, my_xl, my_xu, RARRAY(xl)->len,
    NUM2INT(calls), rptr, sptr, &result, &abserr);

  return rb_ary_new3(2, rb_float_new(result), rb_float_new(abserr));
}

static VALUE MVegas_get_sigma(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->sigma);
}

static VALUE MVegas_get_result(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->result);
}

static VALUE MVegas_get_iterations(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return INT2FIX(ptr->iterations);
}

static VALUE MVegas_set_iterations(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->iterations = NUM2INT(v);
  return self;
}

static VALUE MVegas_get_stage(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return INT2FIX(ptr->stage);
}

static VALUE MVegas_set_stage(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->stage = NUM2INT(v);
  return self;
}

static VALUE MVegas_get_alpha(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->alpha);
}

static VALUE MVegas_set_alpha(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->alpha = NUM2DBL(v);
  return self;
}

static VALUE MVegas_get_chisq(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return rb_float_new(ptr->chisq);
}

static VALUE MVegas_set_chisq(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->chisq = NUM2DBL(v);
  return self;
}

static VALUE MVegas_get_mode(VALUE self) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  return INT2FIX(ptr->mode);
}

static VALUE MVegas_set_mode(VALUE self, VALUE v) {
  gsl_monte_vegas_state * ptr;
  Data_Get_Struct(self, gsl_monte_vegas_state, ptr);
  ptr->mode = NUM2INT(v);
  return self;
}
/* Class definition */
EOS2

#####################################################
# str3 methods: insert the name foo
#define variables
monteCarlo_foo = "MonteCarlo_" + foo
rbgsl_cMPlain_foo = "rbgsl_cMPlain_" + foo
rbgsl_cMMiser_foo = "rbgsl_cMMiser_" + foo
rbgsl_cMVegas_foo = "rbgsl_cMVegas_" + foo

str3 = <<EOS3
VALUE rbgsl_m#{monteCarlo_foo};
VALUE #{rbgsl_cMPlain_foo};
VALUE #{rbgsl_cMMiser_foo};
VALUE #{rbgsl_cMVegas_foo};

void Init_#{monteCarlo_foo}() {
  rbgsl_m#{monteCarlo_foo} = rb_define_module_under(rbgsl_mGSL, "#{monteCarlo_foo}");

  #{rbgsl_cMPlain_foo} = rb_define_class_under(rbgsl_m#{monteCarlo_foo}, "Plain", rb_cObject);
  rb_define_singleton_method(#{rbgsl_cMPlain_foo}, "new", MPlain_new, 1);
  rb_define_method(#{rbgsl_cMPlain_foo}, "initialize", MPlain_init, 1);
  rb_define_method(#{rbgsl_cMPlain_foo}, "integrate", MPlain_integrate, 4);
  
  #{rbgsl_cMMiser_foo} = rb_define_class_under(rbgsl_m#{monteCarlo_foo}, "Miser", rb_cObject);
  rb_define_singleton_method(#{rbgsl_cMMiser_foo}, "new", MMiser_new, 1);
  rb_define_method(#{rbgsl_cMMiser_foo}, "initialize", MMiser_init, 1);
  rb_define_method(#{rbgsl_cMMiser_foo}, "integrate", MMiser_integrate, 4);

  rb_define_method(#{rbgsl_cMMiser_foo}, "estimate_frac", MMiser_get_estimate_frac, 0);
  rb_define_method(#{rbgsl_cMMiser_foo}, "estimate_frac=", MMiser_set_estimate_frac, 1);
  rb_define_method(#{rbgsl_cMMiser_foo}, "min_calls", MMiser_get_min_calls, 0);
  rb_define_method(#{rbgsl_cMMiser_foo}, "min_calls=", MMiser_set_min_calls, 1);
  rb_define_method(#{rbgsl_cMMiser_foo}, "min_calls_per_bisection", MMiser_get_min_calls_per_bisection, 0);
  rb_define_method(#{rbgsl_cMMiser_foo}, "min_calls_per_bisection=", MMiser_set_min_calls_per_bisection, 1);
  rb_define_method(#{rbgsl_cMMiser_foo}, "alpha", MMiser_get_alpha, 0);
  rb_define_method(#{rbgsl_cMMiser_foo}, "alpha=", MMiser_set_alpha, 1);
  rb_define_method(#{rbgsl_cMMiser_foo}, "dither", MMiser_get_dither, 0);
  rb_define_method(#{rbgsl_cMMiser_foo}, "dither=", MMiser_set_dither, 1);

  #{rbgsl_cMVegas_foo} = rb_define_class_under(rbgsl_m#{monteCarlo_foo}, "Vegas", rb_cObject);
  rb_define_singleton_method(#{rbgsl_cMVegas_foo}, "new", MVegas_new, 1);
  rb_define_method(#{rbgsl_cMVegas_foo}, "initialize", MVegas_init, 1);
  rb_define_method(#{rbgsl_cMVegas_foo}, "integrate", MVegas_integrate, 4);

  rb_define_method(#{rbgsl_cMVegas_foo}, "result", MVegas_get_result, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "sigma", MVegas_get_sigma, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "iterations", MVegas_get_iterations, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "iterations=", MVegas_set_iterations, 1);
  rb_define_method(#{rbgsl_cMVegas_foo}, "alpha", MVegas_get_alpha, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "alpha=", MVegas_set_alpha, 1);
  rb_define_method(#{rbgsl_cMVegas_foo}, "chisq", MVegas_get_chisq, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "chisq=", MVegas_set_chisq, 1);
  rb_define_method(#{rbgsl_cMVegas_foo}, "stage", MVegas_get_stage, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "stage=", MVegas_set_stage, 1);
  rb_define_method(#{rbgsl_cMVegas_foo}, "mode", MVegas_get_mode, 0);
  rb_define_method(#{rbgsl_cMVegas_foo}, "mode=", MVegas_set_mode, 1);

  rb_define_const(#{rbgsl_cMVegas_foo}, "MODE_IMPORTANCE", INT2FIX(GSL_VEGAS_MODE_IMPORTANCE));
  rb_define_const(#{rbgsl_cMVegas_foo}, "MODE_IMPORTANCE_ONLY", INT2FIX(GSL_VEGAS_MODE_IMPORTANCE_ONLY));
  rb_define_const(#{rbgsl_cMVegas_foo}, "MODE_STRATIFIED", INT2FIX(GSL_VEGAS_MODE_STRATIFIED));


}
EOS3


  src = str1 + a + str2 + str3

# Generating code  
  File.open(src_name, "w"){ |f| f.print src }

#############################  
# compilation
#
  srcdir  = Config::CONFIG["srcdir"]
  archdir = Config::CONFIG["archdir"]
  if File.exist? archdir + "/ruby.h"
    hdrdir = archdir
  elsif File.exist? srcdir + "/ruby.h"
    hdrdir = srcdir
  else
    $stderr.puts "ERROR: Can't find header files for ruby. Exiting..."
  exit 1
end

  cmd = "#{Config::CONFIG['LDSHARED']} #{Config::CONFIG['CFLAGS']} -I #{hdrdir} -I #{rgsldir} -o #{so_name} #{src_name}"
  
  if /mswin32/ =~ RUBY_PLATFORM then
    cmd += " -link /INCREMENTAL:no /EXPORT:Init_#{mod_name}"
  end
	
  $stderr.puts "Building #{so_name} with '#{cmd}'" if $DEBUG
  `#{cmd}`
  raise "error executing #{cmd}: #{$?}" if $? != 0

  # Loading the monteCarlo_foo module
  require monteCarlo_foo or raise "require #{monteCarlo_foo} failed"

# remove compiled files at_exit
  at_exit { File.unlink(so_name) }
  at_exit { File.unlink(src_name) }
  
 end  # definition of compile
end # end of MInline



