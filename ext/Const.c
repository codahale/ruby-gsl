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

/* $Id: Const.c,v 1.4 2004/11/13 22:50:12 aerpenbeck Exp $ */

#include "ruby.h"
#include "gsl/gsl_const.h"
#include "ruby-gsl.h"

/* Physical Constants */

/* Module definition */

VALUE rbgsl_mConst;
VALUE rbgsl_mConstNum;
VALUE rbgsl_mConstCGSM;
VALUE rbgsl_mConstMKSA;

void Init_Const() {
  rbgsl_mConst = rb_define_module_under(rbgsl_mGSL, "Const");
  rbgsl_mConstNum = rb_define_module_under(rbgsl_mConst, "Num");
  rbgsl_mConstCGSM = rb_define_module_under(rbgsl_mConst, "CGSM");
  rbgsl_mConstMKSA = rb_define_module_under(rbgsl_mConst, "MKSA");

  rb_define_const(rbgsl_mConstNum, "FINE_STRUCTURE", rb_float_new(GSL_CONST_NUM_FINE_STRUCTURE));
  rb_define_const(rbgsl_mConstNum, "AVOGADRO", rb_float_new(GSL_CONST_NUM_AVOGADRO));
  rb_define_const(rbgsl_mConstNum, "YOTTA", rb_float_new(GSL_CONST_NUM_YOTTA));
  rb_define_const(rbgsl_mConstNum, "ZETTA", rb_float_new(GSL_CONST_NUM_ZETTA));
  rb_define_const(rbgsl_mConstNum, "EXA", rb_float_new(GSL_CONST_NUM_EXA));
  rb_define_const(rbgsl_mConstNum, "PETA", rb_float_new(GSL_CONST_NUM_PETA));
  rb_define_const(rbgsl_mConstNum, "TERA", rb_float_new(GSL_CONST_NUM_TERA));
  rb_define_const(rbgsl_mConstNum, "GIGA", rb_float_new(GSL_CONST_NUM_GIGA));
  rb_define_const(rbgsl_mConstNum, "MEGA", rb_float_new(GSL_CONST_NUM_MEGA));
  rb_define_const(rbgsl_mConstNum, "KILO", rb_float_new(GSL_CONST_NUM_KILO));
  rb_define_const(rbgsl_mConstNum, "MILLI", rb_float_new(GSL_CONST_NUM_MILLI));
  rb_define_const(rbgsl_mConstNum, "MICRO", rb_float_new(GSL_CONST_NUM_MICRO));
  rb_define_const(rbgsl_mConstNum, "NANO", rb_float_new(GSL_CONST_NUM_NANO));
  rb_define_const(rbgsl_mConstNum, "PICO", rb_float_new(GSL_CONST_NUM_PICO));
  rb_define_const(rbgsl_mConstNum, "FEMTO", rb_float_new(GSL_CONST_NUM_FEMTO));
  rb_define_const(rbgsl_mConstNum, "ATTO", rb_float_new(GSL_CONST_NUM_ATTO));
  rb_define_const(rbgsl_mConstNum, "ZEPTO", rb_float_new(GSL_CONST_NUM_ZEPTO));
  rb_define_const(rbgsl_mConstNum, "YOCTO", rb_float_new(GSL_CONST_NUM_YOCTO));

  rb_define_const(rbgsl_mConstMKSA, "SPEED_OF_LIGHT", rb_float_new(GSL_CONST_MKSA_SPEED_OF_LIGHT)); 
  rb_define_const(rbgsl_mConstMKSA, "GRAVITATIONAL_CONSTANT", rb_float_new(GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT)); 
  rb_define_const(rbgsl_mConstMKSA, "PLANCKS_CONSTANT_H", rb_float_new(GSL_CONST_MKSA_PLANCKS_CONSTANT_H)); 
  rb_define_const(rbgsl_mConstMKSA, "PLANCKS_CONSTANT_HBAR", rb_float_new(GSL_CONST_MKSA_PLANCKS_CONSTANT_HBAR)); 
  rb_define_const(rbgsl_mConstMKSA, "ASTRONOMICAL_UNIT", rb_float_new(GSL_CONST_MKSA_ASTRONOMICAL_UNIT)); 
  rb_define_const(rbgsl_mConstMKSA, "LIGHT_YEAR", rb_float_new(GSL_CONST_MKSA_LIGHT_YEAR)); 
  rb_define_const(rbgsl_mConstMKSA, "PARSEC", rb_float_new(GSL_CONST_MKSA_PARSEC)); 
  rb_define_const(rbgsl_mConstMKSA, "GRAV_ACCEL", rb_float_new(GSL_CONST_MKSA_GRAV_ACCEL)); 
  rb_define_const(rbgsl_mConstMKSA, "ELECTRON_VOLT", rb_float_new(GSL_CONST_MKSA_ELECTRON_VOLT)); 
  rb_define_const(rbgsl_mConstMKSA, "MASS_ELECTRON", rb_float_new(GSL_CONST_MKSA_MASS_ELECTRON)); 
  rb_define_const(rbgsl_mConstMKSA, "MASS_MUON", rb_float_new(GSL_CONST_MKSA_MASS_MUON)); 
  rb_define_const(rbgsl_mConstMKSA, "MASS_PROTON", rb_float_new(GSL_CONST_MKSA_MASS_PROTON)); 
  rb_define_const(rbgsl_mConstMKSA, "MASS_NEUTRON", rb_float_new(GSL_CONST_MKSA_MASS_NEUTRON)); 
  rb_define_const(rbgsl_mConstMKSA, "RYDBERG", rb_float_new(GSL_CONST_MKSA_RYDBERG)); 
  rb_define_const(rbgsl_mConstMKSA, "BOLTZMANN", rb_float_new(GSL_CONST_MKSA_BOLTZMANN)); 
  rb_define_const(rbgsl_mConstMKSA, "BOHR_MAGNETON", rb_float_new(GSL_CONST_MKSA_BOHR_MAGNETON)); 
  rb_define_const(rbgsl_mConstMKSA, "NUCLEAR_MAGNETON", rb_float_new(GSL_CONST_MKSA_NUCLEAR_MAGNETON)); 
  rb_define_const(rbgsl_mConstMKSA, "ELECTRON_MAGNETIC_MOMENT", rb_float_new(GSL_CONST_MKSA_ELECTRON_MAGNETIC_MOMENT)); 
  rb_define_const(rbgsl_mConstMKSA, "PROTON_MAGNETIC_MOMENT", rb_float_new(GSL_CONST_MKSA_PROTON_MAGNETIC_MOMENT)); 
  rb_define_const(rbgsl_mConstMKSA, "MOLAR_GAS", rb_float_new(GSL_CONST_MKSA_MOLAR_GAS)); 
  rb_define_const(rbgsl_mConstMKSA, "STANDARD_GAS_VOLUME", rb_float_new(GSL_CONST_MKSA_STANDARD_GAS_VOLUME)); 
  rb_define_const(rbgsl_mConstMKSA, "MINUTE", rb_float_new(GSL_CONST_MKSA_MINUTE)); 
  rb_define_const(rbgsl_mConstMKSA, "HOUR", rb_float_new(GSL_CONST_MKSA_HOUR)); 
  rb_define_const(rbgsl_mConstMKSA, "DAY", rb_float_new(GSL_CONST_MKSA_DAY)); 
  rb_define_const(rbgsl_mConstMKSA, "WEEK", rb_float_new(GSL_CONST_MKSA_WEEK)); 
  rb_define_const(rbgsl_mConstMKSA, "INCH", rb_float_new(GSL_CONST_MKSA_INCH)); 
  rb_define_const(rbgsl_mConstMKSA, "FOOT", rb_float_new(GSL_CONST_MKSA_FOOT)); 
  rb_define_const(rbgsl_mConstMKSA, "YARD", rb_float_new(GSL_CONST_MKSA_YARD)); 
  rb_define_const(rbgsl_mConstMKSA, "MILE", rb_float_new(GSL_CONST_MKSA_MILE)); 
  rb_define_const(rbgsl_mConstMKSA, "NAUTICAL_MILE", rb_float_new(GSL_CONST_MKSA_NAUTICAL_MILE)); 
  rb_define_const(rbgsl_mConstMKSA, "FATHOM", rb_float_new(GSL_CONST_MKSA_FATHOM)); 
  rb_define_const(rbgsl_mConstMKSA, "MIL", rb_float_new(GSL_CONST_MKSA_MIL)); 
  rb_define_const(rbgsl_mConstMKSA, "POINT", rb_float_new(GSL_CONST_MKSA_POINT)); 
  rb_define_const(rbgsl_mConstMKSA, "TEXPOINT", rb_float_new(GSL_CONST_MKSA_TEXPOINT)); 
  rb_define_const(rbgsl_mConstMKSA, "MICRON", rb_float_new(GSL_CONST_MKSA_MICRON)); 
  rb_define_const(rbgsl_mConstMKSA, "ANGSTROM", rb_float_new(GSL_CONST_MKSA_ANGSTROM)); 
  rb_define_const(rbgsl_mConstMKSA, "HECTARE", rb_float_new(GSL_CONST_MKSA_HECTARE)); 
  rb_define_const(rbgsl_mConstMKSA, "ACRE", rb_float_new(GSL_CONST_MKSA_ACRE)); 
  rb_define_const(rbgsl_mConstMKSA, "BARN", rb_float_new(GSL_CONST_MKSA_BARN)); 
  rb_define_const(rbgsl_mConstMKSA, "LITER", rb_float_new(GSL_CONST_MKSA_LITER)); 
  rb_define_const(rbgsl_mConstMKSA, "US_GALLON", rb_float_new(GSL_CONST_MKSA_US_GALLON)); 
  rb_define_const(rbgsl_mConstMKSA, "QUART", rb_float_new(GSL_CONST_MKSA_QUART)); 
  rb_define_const(rbgsl_mConstMKSA, "PINT", rb_float_new(GSL_CONST_MKSA_PINT)); 
  rb_define_const(rbgsl_mConstMKSA, "CUP", rb_float_new(GSL_CONST_MKSA_CUP)); 
  rb_define_const(rbgsl_mConstMKSA, "FLUID_OUNCE", rb_float_new(GSL_CONST_MKSA_FLUID_OUNCE)); 
  rb_define_const(rbgsl_mConstMKSA, "TABLESPOON", rb_float_new(GSL_CONST_MKSA_TABLESPOON)); 
  rb_define_const(rbgsl_mConstMKSA, "TEASPOON", rb_float_new(GSL_CONST_MKSA_TEASPOON)); 
  rb_define_const(rbgsl_mConstMKSA, "CANADIAN_GALLON", rb_float_new(GSL_CONST_MKSA_CANADIAN_GALLON)); 
  rb_define_const(rbgsl_mConstMKSA, "UK_GALLON", rb_float_new(GSL_CONST_MKSA_UK_GALLON)); 
  rb_define_const(rbgsl_mConstMKSA, "MILES_PER_HOUR", rb_float_new(GSL_CONST_MKSA_MILES_PER_HOUR)); 
  rb_define_const(rbgsl_mConstMKSA, "KILOMETERS_PER_HOUR", rb_float_new(GSL_CONST_MKSA_KILOMETERS_PER_HOUR)); 
  rb_define_const(rbgsl_mConstMKSA, "KNOT", rb_float_new(GSL_CONST_MKSA_KNOT)); 
  rb_define_const(rbgsl_mConstMKSA, "POUND_MASS", rb_float_new(GSL_CONST_MKSA_POUND_MASS)); 
  rb_define_const(rbgsl_mConstMKSA, "OUNCE_MASS", rb_float_new(GSL_CONST_MKSA_OUNCE_MASS)); 
  rb_define_const(rbgsl_mConstMKSA, "TON", rb_float_new(GSL_CONST_MKSA_TON)); 
  rb_define_const(rbgsl_mConstMKSA, "METRIC_TON", rb_float_new(GSL_CONST_MKSA_METRIC_TON)); 
  rb_define_const(rbgsl_mConstMKSA, "UK_TON", rb_float_new(GSL_CONST_MKSA_UK_TON)); 
  rb_define_const(rbgsl_mConstMKSA, "TROY_OUNCE", rb_float_new(GSL_CONST_MKSA_TROY_OUNCE)); 
  rb_define_const(rbgsl_mConstMKSA, "CARAT", rb_float_new(GSL_CONST_MKSA_CARAT)); 
  rb_define_const(rbgsl_mConstMKSA, "UNIFIED_ATOMIC_MASS", rb_float_new(GSL_CONST_MKSA_UNIFIED_ATOMIC_MASS)); 
  rb_define_const(rbgsl_mConstMKSA, "GRAM_FORCE", rb_float_new(GSL_CONST_MKSA_GRAM_FORCE)); 
  rb_define_const(rbgsl_mConstMKSA, "POUND_FORCE", rb_float_new(GSL_CONST_MKSA_POUND_FORCE)); 
  rb_define_const(rbgsl_mConstMKSA, "KILOPOUND_FORCE", rb_float_new(GSL_CONST_MKSA_KILOPOUND_FORCE)); 
  rb_define_const(rbgsl_mConstMKSA, "POUNDAL", rb_float_new(GSL_CONST_MKSA_POUNDAL)); 
  rb_define_const(rbgsl_mConstMKSA, "CALORIE", rb_float_new(GSL_CONST_MKSA_CALORIE)); 
  rb_define_const(rbgsl_mConstMKSA, "BTU", rb_float_new(GSL_CONST_MKSA_BTU)); 
  rb_define_const(rbgsl_mConstMKSA, "THERM", rb_float_new(GSL_CONST_MKSA_THERM)); 
  rb_define_const(rbgsl_mConstMKSA, "HORSEPOWER", rb_float_new(GSL_CONST_MKSA_HORSEPOWER)); 
  rb_define_const(rbgsl_mConstMKSA, "BAR", rb_float_new(GSL_CONST_MKSA_BAR)); 
  rb_define_const(rbgsl_mConstMKSA, "STD_ATMOSPHERE", rb_float_new(GSL_CONST_MKSA_STD_ATMOSPHERE)); 
  rb_define_const(rbgsl_mConstMKSA, "TORR", rb_float_new(GSL_CONST_MKSA_TORR)); 
  rb_define_const(rbgsl_mConstMKSA, "METER_OF_MERCURY", rb_float_new(GSL_CONST_MKSA_METER_OF_MERCURY)); 
  rb_define_const(rbgsl_mConstMKSA, "INCH_OF_MERCURY", rb_float_new(GSL_CONST_MKSA_INCH_OF_MERCURY)); 
  rb_define_const(rbgsl_mConstMKSA, "INCH_OF_WATER", rb_float_new(GSL_CONST_MKSA_INCH_OF_WATER)); 
  rb_define_const(rbgsl_mConstMKSA, "PSI", rb_float_new(GSL_CONST_MKSA_PSI)); 
  rb_define_const(rbgsl_mConstMKSA, "POISE", rb_float_new(GSL_CONST_MKSA_POISE)); 
  rb_define_const(rbgsl_mConstMKSA, "STOKES", rb_float_new(GSL_CONST_MKSA_STOKES)); 
  rb_define_const(rbgsl_mConstMKSA, "FARADAY", rb_float_new(GSL_CONST_MKSA_FARADAY)); 
  rb_define_const(rbgsl_mConstMKSA, "ELECTRON_CHARGE", rb_float_new(GSL_CONST_MKSA_ELECTRON_CHARGE)); 
  rb_define_const(rbgsl_mConstMKSA, "GAUSS", rb_float_new(GSL_CONST_MKSA_GAUSS)); 
  rb_define_const(rbgsl_mConstMKSA, "STILB", rb_float_new(GSL_CONST_MKSA_STILB)); 
  rb_define_const(rbgsl_mConstMKSA, "LUMEN", rb_float_new(GSL_CONST_MKSA_LUMEN)); 
  rb_define_const(rbgsl_mConstMKSA, "LUX", rb_float_new(GSL_CONST_MKSA_LUX)); 
  rb_define_const(rbgsl_mConstMKSA, "PHOT", rb_float_new(GSL_CONST_MKSA_PHOT)); 
  rb_define_const(rbgsl_mConstMKSA, "FOOTCANDLE", rb_float_new(GSL_CONST_MKSA_FOOTCANDLE)); 
  rb_define_const(rbgsl_mConstMKSA, "LAMBERT", rb_float_new(GSL_CONST_MKSA_LAMBERT)); 
  rb_define_const(rbgsl_mConstMKSA, "FOOTLAMBERT", rb_float_new(GSL_CONST_MKSA_FOOTLAMBERT)); 
  rb_define_const(rbgsl_mConstMKSA, "CURIE", rb_float_new(GSL_CONST_MKSA_CURIE)); 
  rb_define_const(rbgsl_mConstMKSA, "ROENTGEN", rb_float_new(GSL_CONST_MKSA_ROENTGEN)); 
  rb_define_const(rbgsl_mConstMKSA, "RAD", rb_float_new(GSL_CONST_MKSA_RAD)); 
  rb_define_const(rbgsl_mConstMKSA, "SOLAR_MASS", rb_float_new(GSL_CONST_MKSA_SOLAR_MASS)); 
  rb_define_const(rbgsl_mConstMKSA, "BOHR_RADIUS", rb_float_new(GSL_CONST_MKSA_BOHR_RADIUS)); 
  rb_define_const(rbgsl_mConstMKSA, "NEWTON", rb_float_new(GSL_CONST_MKSA_NEWTON)); 
  rb_define_const(rbgsl_mConstMKSA, "DYNE", rb_float_new(GSL_CONST_MKSA_DYNE)); 
  rb_define_const(rbgsl_mConstMKSA, "JOULE", rb_float_new(GSL_CONST_MKSA_JOULE)); 
  rb_define_const(rbgsl_mConstMKSA, "ERG", rb_float_new(GSL_CONST_MKSA_ERG)); 
  rb_define_const(rbgsl_mConstMKSA, "STEFAN_BOLTZMANN_CONSTANT", rb_float_new(GSL_CONST_CGSM_STEFAN_BOLTZMANN_CONSTANT)); 
  rb_define_const(rbgsl_mConstMKSA, "THOMSON_CROSS_SECTION", rb_float_new(GSL_CONST_CGSM_THOMSON_CROSS_SECTION)); 
  rb_define_const(rbgsl_mConstMKSA, "VACUUM_PERMITTIVITY", rb_float_new(GSL_CONST_MKSA_VACUUM_PERMITTIVITY)); 
  rb_define_const(rbgsl_mConstMKSA, "VACUUM_PERMEABILITY", rb_float_new(GSL_CONST_MKSA_VACUUM_PERMEABILITY)); 

  rb_define_const(rbgsl_mConstCGSM, "SPEED_OF_LIGHT", rb_float_new(GSL_CONST_CGSM_SPEED_OF_LIGHT));
  rb_define_const(rbgsl_mConstCGSM, "GRAVITATIONAL_CONSTANT", rb_float_new(GSL_CONST_CGSM_GRAVITATIONAL_CONSTANT));
  rb_define_const(rbgsl_mConstCGSM, "PLANCKS_CONSTANT_H", rb_float_new(GSL_CONST_CGSM_PLANCKS_CONSTANT_H));
  rb_define_const(rbgsl_mConstCGSM, "PLANCKS_CONSTANT_HBAR", rb_float_new(GSL_CONST_CGSM_PLANCKS_CONSTANT_HBAR));
  rb_define_const(rbgsl_mConstCGSM, "ASTRONOMICAL_UNIT", rb_float_new(GSL_CONST_CGSM_ASTRONOMICAL_UNIT));
  rb_define_const(rbgsl_mConstCGSM, "LIGHT_YEAR", rb_float_new(GSL_CONST_CGSM_LIGHT_YEAR));
  rb_define_const(rbgsl_mConstCGSM, "PARSEC", rb_float_new(GSL_CONST_CGSM_PARSEC));
  rb_define_const(rbgsl_mConstCGSM, "GRAV_ACCEL", rb_float_new(GSL_CONST_CGSM_GRAV_ACCEL));
  rb_define_const(rbgsl_mConstCGSM, "ELECTRON_VOLT", rb_float_new(GSL_CONST_CGSM_ELECTRON_VOLT));
  rb_define_const(rbgsl_mConstCGSM, "MASS_ELECTRON", rb_float_new(GSL_CONST_CGSM_MASS_ELECTRON));
  rb_define_const(rbgsl_mConstCGSM, "MASS_MUON", rb_float_new(GSL_CONST_CGSM_MASS_MUON));
  rb_define_const(rbgsl_mConstCGSM, "MASS_PROTON", rb_float_new(GSL_CONST_CGSM_MASS_PROTON));
  rb_define_const(rbgsl_mConstCGSM, "MASS_NEUTRON", rb_float_new(GSL_CONST_CGSM_MASS_NEUTRON));
  rb_define_const(rbgsl_mConstCGSM, "RYDBERG", rb_float_new(GSL_CONST_CGSM_RYDBERG));
  rb_define_const(rbgsl_mConstCGSM, "BOLTZMANN", rb_float_new(GSL_CONST_CGSM_BOLTZMANN));
  rb_define_const(rbgsl_mConstCGSM, "BOHR_MAGNETON", rb_float_new(GSL_CONST_CGSM_BOHR_MAGNETON));
  rb_define_const(rbgsl_mConstCGSM, "NUCLEAR_MAGNETON", rb_float_new(GSL_CONST_CGSM_NUCLEAR_MAGNETON));
  rb_define_const(rbgsl_mConstCGSM, "ELECTRON_MAGNETIC_MOMENT", rb_float_new(GSL_CONST_CGSM_ELECTRON_MAGNETIC_MOMENT));
  rb_define_const(rbgsl_mConstCGSM, "PROTON_MAGNETIC_MOMENT", rb_float_new(GSL_CONST_CGSM_PROTON_MAGNETIC_MOMENT));
  rb_define_const(rbgsl_mConstCGSM, "MOLAR_GAS", rb_float_new(GSL_CONST_CGSM_MOLAR_GAS));
  rb_define_const(rbgsl_mConstCGSM, "STANDARD_GAS_VOLUME", rb_float_new(GSL_CONST_CGSM_STANDARD_GAS_VOLUME));
  rb_define_const(rbgsl_mConstCGSM, "MINUTE", rb_float_new(GSL_CONST_CGSM_MINUTE));
  rb_define_const(rbgsl_mConstCGSM, "HOUR", rb_float_new(GSL_CONST_CGSM_HOUR));
  rb_define_const(rbgsl_mConstCGSM, "DAY", rb_float_new(GSL_CONST_CGSM_DAY));
  rb_define_const(rbgsl_mConstCGSM, "WEEK", rb_float_new(GSL_CONST_CGSM_WEEK));
  rb_define_const(rbgsl_mConstCGSM, "INCH", rb_float_new(GSL_CONST_CGSM_INCH));
  rb_define_const(rbgsl_mConstCGSM, "FOOT", rb_float_new(GSL_CONST_CGSM_FOOT));
  rb_define_const(rbgsl_mConstCGSM, "YARD", rb_float_new(GSL_CONST_CGSM_YARD));
  rb_define_const(rbgsl_mConstCGSM, "MILE", rb_float_new(GSL_CONST_CGSM_MILE));
  rb_define_const(rbgsl_mConstCGSM, "NAUTICAL_MILE", rb_float_new(GSL_CONST_CGSM_NAUTICAL_MILE));
  rb_define_const(rbgsl_mConstCGSM, "FATHOM", rb_float_new(GSL_CONST_CGSM_FATHOM));
  rb_define_const(rbgsl_mConstCGSM, "MIL", rb_float_new(GSL_CONST_CGSM_MIL));
  rb_define_const(rbgsl_mConstCGSM, "POINT", rb_float_new(GSL_CONST_CGSM_POINT));
  rb_define_const(rbgsl_mConstCGSM, "TEXPOINT", rb_float_new(GSL_CONST_CGSM_TEXPOINT));
  rb_define_const(rbgsl_mConstCGSM, "MICRON", rb_float_new(GSL_CONST_CGSM_MICRON));
  rb_define_const(rbgsl_mConstCGSM, "ANGSTROM", rb_float_new(GSL_CONST_CGSM_ANGSTROM));
  rb_define_const(rbgsl_mConstCGSM, "HECTARE", rb_float_new(GSL_CONST_CGSM_HECTARE));
  rb_define_const(rbgsl_mConstCGSM, "ACRE", rb_float_new(GSL_CONST_CGSM_ACRE));
  rb_define_const(rbgsl_mConstCGSM, "BARN", rb_float_new(GSL_CONST_CGSM_BARN));
  rb_define_const(rbgsl_mConstCGSM, "LITER", rb_float_new(GSL_CONST_CGSM_LITER));
  rb_define_const(rbgsl_mConstCGSM, "US_GALLON", rb_float_new(GSL_CONST_CGSM_US_GALLON));
  rb_define_const(rbgsl_mConstCGSM, "QUART", rb_float_new(GSL_CONST_CGSM_QUART));
  rb_define_const(rbgsl_mConstCGSM, "PINT", rb_float_new(GSL_CONST_CGSM_PINT));
  rb_define_const(rbgsl_mConstCGSM, "CUP", rb_float_new(GSL_CONST_CGSM_CUP));
  rb_define_const(rbgsl_mConstCGSM, "FLUID_OUNCE", rb_float_new(GSL_CONST_CGSM_FLUID_OUNCE));
  rb_define_const(rbgsl_mConstCGSM, "TABLESPOON", rb_float_new(GSL_CONST_CGSM_TABLESPOON));
  rb_define_const(rbgsl_mConstCGSM, "TEASPOON", rb_float_new(GSL_CONST_CGSM_TEASPOON));
  rb_define_const(rbgsl_mConstCGSM, "CANADIAN_GALLON", rb_float_new(GSL_CONST_CGSM_CANADIAN_GALLON));
  rb_define_const(rbgsl_mConstCGSM, "UK_GALLON", rb_float_new(GSL_CONST_CGSM_UK_GALLON));
  rb_define_const(rbgsl_mConstCGSM, "MILES_PER_HOUR", rb_float_new(GSL_CONST_CGSM_MILES_PER_HOUR));
  rb_define_const(rbgsl_mConstCGSM, "KILOMETERS_PER_HOUR", rb_float_new(GSL_CONST_CGSM_KILOMETERS_PER_HOUR));
  rb_define_const(rbgsl_mConstCGSM, "KNOT", rb_float_new(GSL_CONST_CGSM_KNOT));
  rb_define_const(rbgsl_mConstCGSM, "POUND_MASS", rb_float_new(GSL_CONST_CGSM_POUND_MASS));
  rb_define_const(rbgsl_mConstCGSM, "OUNCE_MASS", rb_float_new(GSL_CONST_CGSM_OUNCE_MASS));
  rb_define_const(rbgsl_mConstCGSM, "TON", rb_float_new(GSL_CONST_CGSM_TON));
  rb_define_const(rbgsl_mConstCGSM, "METRIC_TON", rb_float_new(GSL_CONST_CGSM_METRIC_TON));
  rb_define_const(rbgsl_mConstCGSM, "UK_TON", rb_float_new(GSL_CONST_CGSM_UK_TON));
  rb_define_const(rbgsl_mConstCGSM, "TROY_OUNCE", rb_float_new(GSL_CONST_CGSM_TROY_OUNCE));
  rb_define_const(rbgsl_mConstCGSM, "CARAT", rb_float_new(GSL_CONST_CGSM_CARAT));
  rb_define_const(rbgsl_mConstCGSM, "UNIFIED_ATOMIC_MASS", rb_float_new(GSL_CONST_CGSM_UNIFIED_ATOMIC_MASS));
  rb_define_const(rbgsl_mConstCGSM, "GRAM_FORCE", rb_float_new(GSL_CONST_CGSM_GRAM_FORCE));
  rb_define_const(rbgsl_mConstCGSM, "POUND_FORCE", rb_float_new(GSL_CONST_CGSM_POUND_FORCE));
  rb_define_const(rbgsl_mConstCGSM, "KILOPOUND_FORCE", rb_float_new(GSL_CONST_CGSM_KILOPOUND_FORCE));
  rb_define_const(rbgsl_mConstCGSM, "POUNDAL", rb_float_new(GSL_CONST_CGSM_POUNDAL));
  rb_define_const(rbgsl_mConstCGSM, "CALORIE", rb_float_new(GSL_CONST_CGSM_CALORIE));
  rb_define_const(rbgsl_mConstCGSM, "BTU", rb_float_new(GSL_CONST_CGSM_BTU));
  rb_define_const(rbgsl_mConstCGSM, "THERM", rb_float_new(GSL_CONST_CGSM_THERM));
  rb_define_const(rbgsl_mConstCGSM, "HORSEPOWER", rb_float_new(GSL_CONST_CGSM_HORSEPOWER));
  rb_define_const(rbgsl_mConstCGSM, "BAR", rb_float_new(GSL_CONST_CGSM_BAR));
  rb_define_const(rbgsl_mConstCGSM, "STD_ATMOSPHERE", rb_float_new(GSL_CONST_CGSM_STD_ATMOSPHERE));
  rb_define_const(rbgsl_mConstCGSM, "TORR", rb_float_new(GSL_CONST_CGSM_TORR));
  rb_define_const(rbgsl_mConstCGSM, "METER_OF_MERCURY", rb_float_new(GSL_CONST_CGSM_METER_OF_MERCURY));
  rb_define_const(rbgsl_mConstCGSM, "INCH_OF_MERCURY", rb_float_new(GSL_CONST_CGSM_INCH_OF_MERCURY));
  rb_define_const(rbgsl_mConstCGSM, "INCH_OF_WATER", rb_float_new(GSL_CONST_CGSM_INCH_OF_WATER));
  rb_define_const(rbgsl_mConstCGSM, "PSI", rb_float_new(GSL_CONST_CGSM_PSI));
  rb_define_const(rbgsl_mConstCGSM, "POISE", rb_float_new(GSL_CONST_CGSM_POISE));
  rb_define_const(rbgsl_mConstCGSM, "STOKES", rb_float_new(GSL_CONST_CGSM_STOKES));
  rb_define_const(rbgsl_mConstCGSM, "FARADAY", rb_float_new(GSL_CONST_CGSM_FARADAY));
  rb_define_const(rbgsl_mConstCGSM, "ELECTRON_CHARGE", rb_float_new(GSL_CONST_CGSM_ELECTRON_CHARGE));
  rb_define_const(rbgsl_mConstCGSM, "GAUSS", rb_float_new(GSL_CONST_CGSM_GAUSS));
  rb_define_const(rbgsl_mConstCGSM, "STILB", rb_float_new(GSL_CONST_CGSM_STILB));
  rb_define_const(rbgsl_mConstCGSM, "LUMEN", rb_float_new(GSL_CONST_CGSM_LUMEN));
  rb_define_const(rbgsl_mConstCGSM, "LUX", rb_float_new(GSL_CONST_CGSM_LUX));
  rb_define_const(rbgsl_mConstCGSM, "PHOT", rb_float_new(GSL_CONST_CGSM_PHOT));
  rb_define_const(rbgsl_mConstCGSM, "FOOTCANDLE", rb_float_new(GSL_CONST_CGSM_FOOTCANDLE));
  rb_define_const(rbgsl_mConstCGSM, "LAMBERT", rb_float_new(GSL_CONST_CGSM_LAMBERT));
  rb_define_const(rbgsl_mConstCGSM, "FOOTLAMBERT", rb_float_new(GSL_CONST_CGSM_FOOTLAMBERT));
  rb_define_const(rbgsl_mConstCGSM, "CURIE", rb_float_new(GSL_CONST_CGSM_CURIE));
  rb_define_const(rbgsl_mConstCGSM, "ROENTGEN", rb_float_new(GSL_CONST_CGSM_ROENTGEN));
  rb_define_const(rbgsl_mConstCGSM, "RAD", rb_float_new(GSL_CONST_CGSM_RAD));
  rb_define_const(rbgsl_mConstCGSM, "SOLAR_MASS", rb_float_new(GSL_CONST_CGSM_SOLAR_MASS));
  rb_define_const(rbgsl_mConstCGSM, "BOHR_RADIUS", rb_float_new(GSL_CONST_CGSM_BOHR_RADIUS)); 
  rb_define_const(rbgsl_mConstCGSM, "NEWTON", rb_float_new(GSL_CONST_CGSM_NEWTON)); 
  rb_define_const(rbgsl_mConstCGSM, "DYNE", rb_float_new(GSL_CONST_CGSM_DYNE)); 
  rb_define_const(rbgsl_mConstCGSM, "JOULE", rb_float_new(GSL_CONST_CGSM_JOULE)); 
  rb_define_const(rbgsl_mConstCGSM, "ERG", rb_float_new(GSL_CONST_CGSM_ERG)); 
  rb_define_const(rbgsl_mConstCGSM, "STEFAN_BOLTZMANN_CONSTANT", rb_float_new(GSL_CONST_CGSM_STEFAN_BOLTZMANN_CONSTANT)); 
  rb_define_const(rbgsl_mConstCGSM, "THOMSON_CROSS_SECTION", rb_float_new(GSL_CONST_CGSM_THOMSON_CROSS_SECTION)); 
}

/* vim: set ts=4: */
