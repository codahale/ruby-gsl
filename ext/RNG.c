/*
* ruby-gsl - using GSL from Ruby
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Id: RNG.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_rng.h"
#include "ruby-gsl.h"

/* constants for RNG */

#define RNG_BOROSH13 0
#define RNG_COVEYOU 1
#define RNG_CMRG 2
#define RNG_FISHMAN18 3
#define RNG_FISHMAN20 4
#define RNG_FISHMAN2X 5
#define RNG_GFSR4 6
#define RNG_KNUTHRAN 7
#define RNG_KNUTHRAN2 8
#define RNG_LECUYER21 9
#define RNG_MINSTD 10
#define RNG_MRG 11
#define RNG_MT19937 12
#define RNG_R250 13
#define RNG_RAN0 14
#define RNG_RAN1 15
#define RNG_RAN2 16
#define RNG_RAN3 17
#define RNG_RAND 18
#define RNG_RAND48 19
#define RNG_RANDOM128_BSD 20
#define RNG_RANDOM128_GLIBC2 21
#define RNG_RANDOM128_LIBC5 22
#define RNG_RANDOM256_BSD 23
#define RNG_RANDOM256_GLIBC2 24
#define RNG_RANDOM256_LIBC5 25
#define RNG_RANDOM32_BSD 26
#define RNG_RANDOM32_GLIBC2 27
#define RNG_RANDOM32_LIBC5 28
#define RNG_RANDOM64_BSD 29
#define RNG_RANDOM64_GLIBC2 30
#define RNG_RANDOM64_LIBC5 31
#define RNG_RANDOM8_BSD 32
#define RNG_RANDOM8_GLIBC2 33
#define RNG_RANDOM8_LIBC5 34
#define RNG_RANDOM_BSD 35
#define RNG_RANDOM_GLIBC2 36
#define RNG_RANDOM_LIBC5 37
#define RNG_RANDU 38
#define RNG_RANF 39
#define RNG_RANLUX 40
#define RNG_RANLUX389 41
#define RNG_RANLXD1 42
#define RNG_RANLXD2 43
#define RNG_RANLXS0 44
#define RNG_RANLXS1 45
#define RNG_RANLXS2 46
#define RNG_RANMAR 47
#define RNG_SLATEC 48
#define RNG_TAUS 49
#define RNG_TRANSPUTER 50
#define RNG_TT800 51
#define RNG_UNI 52
#define RNG_UNI32 53
#define RNG_VAX 54
#define RNG_WATERMAN14 55
#define RNG_ZUF 56
#define RNG_MT19937_1999 57
#define RNG_MT19937_1998 58
#define RNG_TAUS2 59
#define RNG_TAUS113 60

/* Random Number Generation */

VALUE rbgsl_cRNG;

/* Internal use only */

typedef unsigned long int (* fun_t)(const gsl_rng *);

static VALUE call(fun_t fun, VALUE class) {
  gsl_rng * ptr;
  Data_Get_Struct(class, gsl_rng, ptr);
  return INT2NUM((*fun)(ptr));
}

/* RNG Initialization */

static VALUE RNG_init(VALUE self, VALUE type) {
  return self;
}

static void RNG_free(void *p) {
  gsl_rng_free(p);
}

/* Initialize to default RNG type if no environment variable set */
VALUE RNG_new(VALUE class) {
  VALUE rng;
  const gsl_rng_type * type;
  gsl_rng * ptr;
  type = gsl_rng_env_setup();
  ptr = gsl_rng_alloc(type);
  rng = Data_Wrap_Struct(class, 0, RNG_free, ptr);
  return rng;
}

/* Initialize to given RNG type */
VALUE RNG_new2(VALUE class, VALUE type) {
  VALUE argv[1];
  VALUE rng;
  gsl_rng * ptr;
  /* TODO: this is ugly */
  switch (NUM2INT(type)) {
    case RNG_BOROSH13:
      ptr = gsl_rng_alloc(gsl_rng_borosh13);
      break;
    case RNG_COVEYOU:
      ptr = gsl_rng_alloc(gsl_rng_coveyou);
      break;
    case RNG_CMRG:
      ptr = gsl_rng_alloc(gsl_rng_cmrg);
      break;
    case RNG_FISHMAN18:
      ptr = gsl_rng_alloc(gsl_rng_fishman18);
      break;
    case RNG_FISHMAN20:
      ptr = gsl_rng_alloc(gsl_rng_fishman20);
      break;
    case RNG_FISHMAN2X:
      ptr = gsl_rng_alloc(gsl_rng_fishman2x);
      break;
    case RNG_GFSR4:
      ptr = gsl_rng_alloc(gsl_rng_gfsr4);
      break;
    case RNG_KNUTHRAN:
      ptr = gsl_rng_alloc(gsl_rng_knuthran);
      break;
    case RNG_KNUTHRAN2:
      ptr = gsl_rng_alloc(gsl_rng_knuthran2);
      break;
    case RNG_LECUYER21:
      ptr = gsl_rng_alloc(gsl_rng_lecuyer21);
      break;
    case RNG_MINSTD:
      ptr = gsl_rng_alloc(gsl_rng_minstd);
      break;
    case RNG_MRG:
      ptr = gsl_rng_alloc(gsl_rng_mrg);
      break;
    case RNG_MT19937:
      ptr = gsl_rng_alloc(gsl_rng_mt19937);
      break;
    case RNG_MT19937_1998:
      ptr = gsl_rng_alloc(gsl_rng_mt19937_1998);
      break;
    case RNG_MT19937_1999:
      ptr = gsl_rng_alloc(gsl_rng_mt19937_1999);
      break;
    case RNG_R250:
      ptr = gsl_rng_alloc(gsl_rng_r250);
      break;
    case RNG_RAN0:
      ptr = gsl_rng_alloc(gsl_rng_ran0);
      break;
    case RNG_RAN1:
      ptr = gsl_rng_alloc(gsl_rng_ran1);
      break;
    case RNG_RAN2:
      ptr = gsl_rng_alloc(gsl_rng_ran2);
      break;
    case RNG_RAN3:
      ptr = gsl_rng_alloc(gsl_rng_ran3);
      break;
    case RNG_RAND48:
      ptr = gsl_rng_alloc(gsl_rng_rand48);
      break;
    case RNG_RAND:
      ptr = gsl_rng_alloc(gsl_rng_rand);
      break;
    case RNG_RANDOM128_BSD:
      ptr = gsl_rng_alloc(gsl_rng_random128_bsd);
      break;
    case RNG_RANDOM128_GLIBC2:
      ptr = gsl_rng_alloc(gsl_rng_random128_glibc2);
      break;
    case RNG_RANDOM128_LIBC5:
      ptr = gsl_rng_alloc(gsl_rng_random128_libc5);
      break;
    case RNG_RANDOM256_BSD:
      ptr = gsl_rng_alloc(gsl_rng_random256_bsd);
      break;
    case RNG_RANDOM256_GLIBC2:
      ptr = gsl_rng_alloc(gsl_rng_random256_glibc2);
      break;
    case RNG_RANDOM256_LIBC5:
      ptr = gsl_rng_alloc(gsl_rng_random256_libc5);
      break;
    case RNG_RANDOM32_BSD:
      ptr = gsl_rng_alloc(gsl_rng_random32_bsd);
      break;
    case RNG_RANDOM32_GLIBC2:
      ptr = gsl_rng_alloc(gsl_rng_random32_glibc2);
      break;
    case RNG_RANDOM32_LIBC5:
      ptr = gsl_rng_alloc(gsl_rng_random32_libc5);
      break;
    case RNG_RANDOM64_BSD:
      ptr = gsl_rng_alloc(gsl_rng_random64_bsd);
      break;
    case RNG_RANDOM64_GLIBC2:
      ptr = gsl_rng_alloc(gsl_rng_random64_glibc2);
      break;
    case RNG_RANDOM64_LIBC5:
      ptr = gsl_rng_alloc(gsl_rng_random64_libc5);
      break;
    case RNG_RANDOM8_BSD:
      ptr = gsl_rng_alloc(gsl_rng_random8_bsd);
      break;
    case RNG_RANDOM8_GLIBC2:
      ptr = gsl_rng_alloc(gsl_rng_random8_glibc2);
      break;
    case RNG_RANDOM8_LIBC5:
      ptr = gsl_rng_alloc(gsl_rng_random8_libc5);
      break;
    case RNG_RANDOM_BSD:
      ptr = gsl_rng_alloc(gsl_rng_random_bsd);
      break;
    case RNG_RANDOM_GLIBC2:
      ptr = gsl_rng_alloc(gsl_rng_random_glibc2);
      break;
    case RNG_RANDOM_LIBC5:
      ptr = gsl_rng_alloc(gsl_rng_random_libc5);
      break;
    case RNG_RANDU:
      ptr = gsl_rng_alloc(gsl_rng_randu);
      break;
    case RNG_RANF:
      ptr = gsl_rng_alloc(gsl_rng_ranf);
      break;
    case RNG_RANLUX389:
      ptr = gsl_rng_alloc(gsl_rng_ranlux389);
      break;
    case RNG_RANLUX:
      ptr = gsl_rng_alloc(gsl_rng_ranlux);
      break;
    case RNG_RANLXD1:
      ptr = gsl_rng_alloc(gsl_rng_ranlxd1);
      break;
    case RNG_RANLXD2:
      ptr = gsl_rng_alloc(gsl_rng_ranlxd2);
      break;
    case RNG_RANLXS0:
      ptr = gsl_rng_alloc(gsl_rng_ranlxs0);
      break;
    case RNG_RANLXS1:
      ptr = gsl_rng_alloc(gsl_rng_ranlxs1);
      break;
    case RNG_RANLXS2:
      ptr = gsl_rng_alloc(gsl_rng_ranlxs2);
      break;
    case RNG_RANMAR:
      ptr = gsl_rng_alloc(gsl_rng_ranmar);
      break;
    case RNG_SLATEC:
      ptr = gsl_rng_alloc(gsl_rng_slatec);
      break;
    case RNG_TAUS:
      ptr = gsl_rng_alloc(gsl_rng_taus);
      break;
    case RNG_TAUS2:
      ptr = gsl_rng_alloc(gsl_rng_taus2);
      break;
    case RNG_TAUS113:
      ptr = gsl_rng_alloc(gsl_rng_taus113);
      break;
    case RNG_TRANSPUTER:
      ptr = gsl_rng_alloc(gsl_rng_transputer);
      break;
    case RNG_TT800:
      ptr = gsl_rng_alloc(gsl_rng_tt800);
      break;
    case RNG_UNI32:
      ptr = gsl_rng_alloc(gsl_rng_uni32);
      break;
    case RNG_UNI:
      ptr = gsl_rng_alloc(gsl_rng_uni);
      break;
    case RNG_VAX:
      ptr = gsl_rng_alloc(gsl_rng_vax);
      break;
    case RNG_ZUF:
      ptr = gsl_rng_alloc(gsl_rng_zuf);
      break;
    case RNG_WATERMAN14:
      ptr = gsl_rng_alloc(gsl_rng_waterman14);
      break;
    default:
      rb_raise(rb_eArgError, "Illegal RNG type!");
  }
  if (!ptr)
    rb_raise(rb_eNoMemError, "Insufficient memory for allocation.");
  rng = Data_Wrap_Struct(class, 0, RNG_free, ptr);
  argv[0] = type;
  rb_obj_call_init(rng, 1, argv);
  return rng;
}

static VALUE RNG_set(VALUE self, VALUE seed) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  gsl_rng_set(ptr, NUM2ULONG(seed));
  return self;
}

/* Sampling from a RNG */

static VALUE RNG_get(VALUE self) {
  return call(gsl_rng_get, self);
}

static VALUE RNG_uniform(VALUE self) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  return rb_float_new(gsl_rng_uniform(ptr));
}

static VALUE RNG_uniform_pos(VALUE self) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  return rb_float_new(gsl_rng_uniform_pos(ptr));
}

static VALUE RNG_uniform_int(VALUE self, VALUE n) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  return INT2FIX(gsl_rng_uniform_int(ptr, NUM2ULONG(n)));
}

/* Auxiliary Functions for RNG */

static VALUE RNG_max(VALUE self) {
  return call(gsl_rng_max, self);
}

static VALUE RNG_min(VALUE self) {
  return call(gsl_rng_min, self);
}

static VALUE RNG_name(VALUE self) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  return rb_str_new2(gsl_rng_name(ptr));
}

/* TODO: gsl_rng_state, gsl_rng_types_setup */

static VALUE RNG_size(VALUE self) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  return INT2NUM(gsl_rng_size(ptr));
}

/* Saving and Restoring a RNG state */

static VALUE RNG_clone(VALUE self) {
  VALUE clone;
  gsl_rng * ptr, * cptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  cptr = gsl_rng_clone(ptr);
  clone = Data_Wrap_Struct(rbgsl_cRNG, 0, RNG_free, cptr);
  return clone;
}

static VALUE RNG_print_state(VALUE self) {
  gsl_rng * ptr;
  Data_Get_Struct(self, gsl_rng, ptr);
  gsl_rng_print_state(ptr);
  return self;
}

static VALUE RNG_memcpy(VALUE self, VALUE dest, VALUE src) {
  int ret;
  gsl_rng * pdest, * psrc;
  Data_Get_Struct(dest, gsl_rng, pdest);
  Data_Get_Struct(src, gsl_rng, psrc);
  ret = gsl_rng_memcpy(pdest, psrc);
  return INT2FIX(ret);
}

/* Class definition */

void Init_RNG() {
  rbgsl_cRNG = rb_define_class_under(rbgsl_mRandom, "RNG", rb_cObject);

  rb_define_singleton_method(rbgsl_cRNG, "new", RNG_new, 0);
  rb_define_singleton_method(rbgsl_cRNG, "new2", RNG_new2, 1);
  rb_define_method(rbgsl_cRNG, "initialize", RNG_init, 1);
  rb_define_method(rbgsl_cRNG, "set", RNG_set, 1);

  rb_define_method(rbgsl_cRNG, "get", RNG_get, 0);
  rb_define_method(rbgsl_cRNG, "uniform", RNG_uniform, 0);
  rb_define_method(rbgsl_cRNG, "uniform_pos", RNG_uniform_pos, 0);
  rb_define_method(rbgsl_cRNG, "uniform_int", RNG_uniform_int, 1);

  rb_define_method(rbgsl_cRNG, "max", RNG_max, 0);
  rb_define_method(rbgsl_cRNG, "min", RNG_min, 0);
  rb_define_method(rbgsl_cRNG, "name", RNG_name, 0);
  rb_define_alias(rbgsl_cRNG, "to_s", "name");
  rb_define_method(rbgsl_cRNG, "size", RNG_size, 0);

  rb_define_module_function(rbgsl_cRNG, "memcpy", RNG_memcpy, 2);
  rb_define_method(rbgsl_cRNG, "clone", RNG_clone, 0);
  rb_define_method(rbgsl_cRNG, "print_state", RNG_print_state, 0);
  rb_define_alias(rbgsl_cRNG, "dump", "print_state");

  rb_define_const(rbgsl_cRNG, "BOROSH13", INT2FIX(RNG_BOROSH13)); 
  rb_define_const(rbgsl_cRNG, "COVEYOU", INT2FIX(RNG_COVEYOU)); 
  rb_define_const(rbgsl_cRNG, "CMRG", INT2FIX(RNG_CMRG)); 
  rb_define_const(rbgsl_cRNG, "FISHMAN18", INT2FIX(RNG_FISHMAN18)); 
  rb_define_const(rbgsl_cRNG, "FISHMAN20", INT2FIX(RNG_FISHMAN20)); 
  rb_define_const(rbgsl_cRNG, "FISHMAN2X", INT2FIX(RNG_FISHMAN2X)); 
  rb_define_const(rbgsl_cRNG, "GFSR4", INT2FIX(RNG_GFSR4)); 
  rb_define_const(rbgsl_cRNG, "KNUTHRAN", INT2FIX(RNG_KNUTHRAN)); 
  rb_define_const(rbgsl_cRNG, "KNUTHRAN2", INT2FIX(RNG_KNUTHRAN2)); 
  rb_define_const(rbgsl_cRNG, "LECUYER21", INT2FIX(RNG_LECUYER21)); 
  rb_define_const(rbgsl_cRNG, "MINSTD", INT2FIX(RNG_MINSTD)); 
  rb_define_const(rbgsl_cRNG, "MRG", INT2FIX(RNG_MRG)); 
  rb_define_const(rbgsl_cRNG, "MT19937", INT2FIX(RNG_MT19937)); 
  rb_define_const(rbgsl_cRNG, "MT19937_1999", INT2FIX(RNG_MT19937_1999)); 
  rb_define_const(rbgsl_cRNG, "MT19937_1998", INT2FIX(RNG_MT19937_1998)); 
  rb_define_const(rbgsl_cRNG, "R250", INT2FIX(RNG_R250)); 
  rb_define_const(rbgsl_cRNG, "RAN0", INT2FIX(RNG_RAN0)); 
  rb_define_const(rbgsl_cRNG, "RAN1", INT2FIX(RNG_RAN1)); 
  rb_define_const(rbgsl_cRNG, "RAN2", INT2FIX(RNG_RAN2)); 
  rb_define_const(rbgsl_cRNG, "RAN3", INT2FIX(RNG_RAN3)); 
  rb_define_const(rbgsl_cRNG, "RAND", INT2FIX(RNG_RAND)); 
  rb_define_const(rbgsl_cRNG, "RAND48", INT2FIX(RNG_RAND48)); 
  rb_define_const(rbgsl_cRNG, "RANDOM128_BSD", INT2FIX(RNG_RANDOM128_BSD)); 
  rb_define_const(rbgsl_cRNG, "RANDOM128_GLIBC2", INT2FIX(RNG_RANDOM128_GLIBC2)); 
  rb_define_const(rbgsl_cRNG, "RANDOM128_LIBC5", INT2FIX(RNG_RANDOM128_LIBC5)); 
  rb_define_const(rbgsl_cRNG, "RANDOM256_BSD", INT2FIX(RNG_RANDOM256_BSD)); 
  rb_define_const(rbgsl_cRNG, "RANDOM256_GLIBC2", INT2FIX(RNG_RANDOM256_GLIBC2)); 
  rb_define_const(rbgsl_cRNG, "RANDOM256_LIBC5", INT2FIX(RNG_RANDOM256_LIBC5)); 
  rb_define_const(rbgsl_cRNG, "RANDOM32_BSD", INT2FIX(RNG_RANDOM32_BSD)); 
  rb_define_const(rbgsl_cRNG, "RANDOM32_GLIBC2", INT2FIX(RNG_RANDOM32_GLIBC2)); 
  rb_define_const(rbgsl_cRNG, "RANDOM32_LIBC5", INT2FIX(RNG_RANDOM32_LIBC5)); 
  rb_define_const(rbgsl_cRNG, "RANDOM64_BSD", INT2FIX(RNG_RANDOM64_BSD)); 
  rb_define_const(rbgsl_cRNG, "RANDOM64_GLIBC2", INT2FIX(RNG_RANDOM64_GLIBC2)); 
  rb_define_const(rbgsl_cRNG, "RANDOM64_LIBC5", INT2FIX(RNG_RANDOM64_LIBC5)); 
  rb_define_const(rbgsl_cRNG, "RANDOM8_BSD", INT2FIX(RNG_RANDOM8_BSD)); 
  rb_define_const(rbgsl_cRNG, "RANDOM8_GLIBC2", INT2FIX(RNG_RANDOM8_GLIBC2)); 
  rb_define_const(rbgsl_cRNG, "RANDOM8_LIBC5", INT2FIX(RNG_RANDOM8_LIBC5)); 
  rb_define_const(rbgsl_cRNG, "RANDOM_BSD", INT2FIX(RNG_RANDOM_BSD)); 
  rb_define_const(rbgsl_cRNG, "RANDOM_GLIBC2", INT2FIX(RNG_RANDOM_GLIBC2)); 
  rb_define_const(rbgsl_cRNG, "RANDOM_LIBC5", INT2FIX(RNG_RANDOM_LIBC5)); 
  rb_define_const(rbgsl_cRNG, "RANDU", INT2FIX(RNG_RANDU)); 
  rb_define_const(rbgsl_cRNG, "RANF", INT2FIX(RNG_RANF)); 
  rb_define_const(rbgsl_cRNG, "RANLUX", INT2FIX(RNG_RANLUX)); 
  rb_define_const(rbgsl_cRNG, "RANLUX389", INT2FIX(RNG_RANLUX389)); 
  rb_define_const(rbgsl_cRNG, "RANLXD1", INT2FIX(RNG_RANLXD1)); 
  rb_define_const(rbgsl_cRNG, "RANLXD2", INT2FIX(RNG_RANLXD2)); 
  rb_define_const(rbgsl_cRNG, "RANLXS0", INT2FIX(RNG_RANLXS0)); 
  rb_define_const(rbgsl_cRNG, "RANLXS1", INT2FIX(RNG_RANLXS1)); 
  rb_define_const(rbgsl_cRNG, "RANLXS2", INT2FIX(RNG_RANLXS2)); 
  rb_define_const(rbgsl_cRNG, "RANMAR", INT2FIX(RNG_RANMAR)); 
  rb_define_const(rbgsl_cRNG, "SLATEC", INT2FIX(RNG_SLATEC)); 
  rb_define_const(rbgsl_cRNG, "TAUS", INT2FIX(RNG_TAUS)); 
  rb_define_const(rbgsl_cRNG, "TAUS2", INT2FIX(RNG_TAUS2)); 
  rb_define_const(rbgsl_cRNG, "TAUS113", INT2FIX(RNG_TAUS113)); 
  rb_define_const(rbgsl_cRNG, "TRANSPUTER", INT2FIX(RNG_TRANSPUTER)); 
  rb_define_const(rbgsl_cRNG, "TT800", INT2FIX(RNG_TT800)); 
  rb_define_const(rbgsl_cRNG, "UNI", INT2FIX(RNG_UNI)); 
  rb_define_const(rbgsl_cRNG, "UNI32", INT2FIX(RNG_UNI32)); 
  rb_define_const(rbgsl_cRNG, "VAX", INT2FIX(RNG_VAX)); 
  rb_define_const(rbgsl_cRNG, "WATERMAN14", INT2FIX(RNG_WATERMAN14)); 
  rb_define_const(rbgsl_cRNG, "ZUF", INT2FIX(RNG_ZUF)); 
}

/* vim: set ts=4: */
