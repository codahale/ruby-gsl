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

/* $Id: EllJac.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_sf_elljac.h"
#include "ruby-gsl.h"

/* Elliptic Functions (Jacobi) */

static VALUE EllJac_e(VALUE self, VALUE u, VALUE m) {
  int ret;
  double sn, cn, dn;
  ret = gsl_sf_elljac_e(NUM2DBL(u), NUM2DBL(m), &sn, &cn, &dn);
  return rb_ary_new3(3, rb_float_new(sn),rb_float_new(cn), rb_float_new(dn)); 
}

/* Module definition */

VALUE rbgsl_mEllJac;

void Init_EllJac() {
  rbgsl_mEllJac = rb_define_module_under(rbgsl_mSpecial, "EllJac");

  rb_define_module_function(rbgsl_mEllJac, "e", EllJac_e, 2);
}

/* vim: set ts=4: */
