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

/* $Id: Special.c,v 1.2 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_mode.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Module definition */

VALUE rbgsl_mSpecial;

void Init_Special() {
  rbgsl_mSpecial = rb_define_module_under(rbgsl_mGSL, "Special");

  /* Modes */
  rb_define_const(rbgsl_mSpecial, "PREC_DOUBLE", INT2FIX(GSL_PREC_DOUBLE));
  rb_define_const(rbgsl_mSpecial, "PREC_SINGLE", INT2FIX(GSL_PREC_SINGLE));
  rb_define_const(rbgsl_mSpecial, "PREC_APPROX", INT2FIX(GSL_PREC_APPROX));

  Init_Airy();
  Init_Bessel();
  Init_Clausen();
  Init_Conical();
  Init_Coulomb();
  Init_Coupling();
  Init_Dawson();
  Init_Debye();
  Init_Dilog();
  Init_Elementary();
  Init_EllInt();
  Init_EllJac();
  Init_Error();
  Init_Exp();
  Init_ExpInt();
  Init_FermiDirac();
  Init_Gamma();
  Init_Gegenbauer();
  Init_Hyperg();
  Init_Laguerre();
  Init_Lambert();
  Init_Legendre();
  Init_Log();
  Init_Psi();
  Init_Synchrotron();
  Init_Transport();
  Init_Trig();
  Init_Zeta();
}

/* vim: set ts=4: */
