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

/* $Id: Random.c,v 1.3 2003/07/04 20:58:19 aerpenbeck Exp $ */

#include "ruby.h"
#include "ruby-gsl.h"
#include "ruby-gsl-intern.h"

/* Random Numbers top-level module */

/* Module definition */

VALUE rbgsl_mRandom;

void Init_GSLRandom() {
  rbgsl_mRandom = rb_define_module_under(rbgsl_mGSL, "Random");

  Init_QRNG();         /* Quasi Random Sequences */
  Init_RND();          /* Random Number Distributions */
  Init_RNG();          /* Random Number Generation */
  Init_Discrete();     /* Discrete Distributions */
}

/* vim: set ts=4: */
