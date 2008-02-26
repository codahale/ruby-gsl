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


/* $Id: Clausen.c,v 1.2 2003/07/04 20:58:18 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_clausen.h"
#include "gsl/gsl_sf_result.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Clausen Functions */

static VALUE Clausen_clausen(VALUE self, VALUE x) {
  return rb_float_new(gsl_sf_clausen(NUM2DBL(x)));
}

static VALUE Clausen_clausen_e(VALUE self, VALUE x) {
  int ret;
  gsl_sf_result r;
  ret = gsl_sf_clausen_e(NUM2DBL(x), &r);
  return RESULT(&r);
}

/* Module definition */

VALUE rbgsl_mClausen;

void Init_Clausen() {
  rbgsl_mClausen = rb_define_module_under(rbgsl_mSpecial, "Clausen");

  rb_define_module_function(rbgsl_mClausen, "clausen", Clausen_clausen, 1);
  rb_define_module_function(rbgsl_mClausen, "clausen_e", Clausen_clausen_e, 1);
}

/* vim: set ts=4: */
