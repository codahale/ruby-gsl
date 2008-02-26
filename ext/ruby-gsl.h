/*
* ruby-gsl - using GSL from Ruby
* Copyright (C) 2001-2003 Arno Erpenbeck
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

/* $Id: ruby-gsl.h,v 1.10 2004/11/13 22:50:12 aerpenbeck Exp $ */

#ifndef _RUBY_GSL_
#define _RUBY_GSL_

#include "ruby.h"

/* exceptions */
extern VALUE rbgsl_eException;
extern VALUE rbgsl_eFactorization;
extern VALUE rbgsl_eRunaway;
extern VALUE rbgsl_eMaxIter;
extern VALUE rbgsl_eTol;
extern VALUE rbgsl_ePrecisionLoss;
extern VALUE rbgsl_eRoundoff;
extern VALUE rbgsl_eSingularity;
extern VALUE rbgsl_eDivergent;
extern VALUE rbgsl_eCache;
extern VALUE rbgsl_eTable;
extern VALUE rbgsl_eNoProgress;
extern VALUE rbgsl_eNoProgressJac;
extern VALUE rbgsl_eToleranceF;
extern VALUE rbgsl_eToleranceX;
extern VALUE rbgsl_eToleranceG;

/* classes */
extern VALUE rbgsl_cAccel;
extern VALUE rbgsl_cBlock;
extern VALUE rbgsl_cCheb;
extern VALUE rbgsl_cComplex;
extern VALUE rbgsl_cCombination;
extern VALUE rbgsl_cDHT;
extern VALUE rbgsl_cFDFSolver;
extern VALUE rbgsl_cFSolver;
extern VALUE rbgsl_cFunction;
extern VALUE rbgsl_cHist;
extern VALUE rbgsl_cHist_PDF;
extern VALUE rbgsl_cHist2D;
extern VALUE rbgsl_cHist2D_PDF;
extern VALUE rbgsl_cInterp;
extern VALUE rbgsl_cLevin;
extern VALUE rbgsl_cLevinTrunc;
extern VALUE rbgsl_cMatrix;
extern VALUE rbgsl_cMatrix_complex;
extern VALUE rbgsl_cMinimizer;
extern VALUE rbgsl_cMultiFunction;
extern VALUE rbgsl_cMonteFunction;
extern VALUE rbgsl_cMPlain;
extern VALUE rbgsl_cMFSolver;
extern VALUE rbgsl_cPermutation;
extern VALUE rbgsl_cQRNG;
extern VALUE rbgsl_cResult;
extern VALUE rbgsl_cResult10;
extern VALUE rbgsl_cRNG;
extern VALUE rbgsl_mSort;
extern VALUE rbgsl_cSiman;
extern VALUE rbgsl_cSpline;
extern VALUE rbgsl_cVector;
extern VALUE rbgsl_cVector_complex;

/* modules */
extern VALUE rbgsl_mGSL;
extern VALUE rbgsl_mAiry;
extern VALUE rbgsl_mBessel;
extern VALUE rbgsl_mBlas;
extern VALUE rbgsl_mBlas_complex;
extern VALUE rbgsl_mClausen;
extern VALUE rbgsl_mConical;
extern VALUE rbgsl_mConst;
extern VALUE rbgsl_mConstNum;
extern VALUE rbgsl_mConstCGSM;
extern VALUE rbgsl_mConstMKSA;
extern VALUE rbgsl_mCoulomb;
extern VALUE rbgsl_mCoupling;
extern VALUE rbgsl_mDawson;
extern VALUE rbgsl_mDebye;
extern VALUE rbgsl_mDiff;
extern VALUE rbgsl_mDilog;
extern VALUE rbgsl_mEigen;
extern VALUE rbgsl_mEigen_complex;
extern VALUE rbgsl_mElementary;
extern VALUE rbgsl_mEllInt;
extern VALUE rbgsl_mEllJac;
extern VALUE rbgsl_mError;
extern VALUE rbgsl_mExp;
extern VALUE rbgsl_mExpInt;
extern VALUE rbgsl_mFermiDirac;
extern VALUE rbgsl_mFit;
extern VALUE rbgsl_mGamma;
extern VALUE rbgsl_mGegenbauer;
extern VALUE rbgsl_mHistogram;
extern VALUE rbgsl_mHyperg;
extern VALUE rbgsl_mInterp;
extern VALUE rbgsl_mIntegration;
extern VALUE rbgsl_mLaguerre;
extern VALUE rbgsl_mLambert;
extern VALUE rbgsl_mLegendre;
extern VALUE rbgsl_mLinalg;
extern VALUE rbgsl_mLinalg_complex;
extern VALUE rbgsl_mLog;
extern VALUE rbgsl_mMonteCarlo;
extern VALUE rbgsl_mMSolver;
extern VALUE rbgsl_mPoly;
extern VALUE rbgsl_mPsi;
extern VALUE rbgsl_mRandom;
extern VALUE rbgsl_mRND;
extern VALUE rbgsl_mSolver;
extern VALUE rbgsl_mSpecial;
extern VALUE rbgsl_mStats;
extern VALUE rbgsl_mSum;
extern VALUE rbgsl_mSynchrotron;
extern VALUE rbgsl_mTransport;
extern VALUE rbgsl_mTrig;
extern VALUE rbgsl_mZeta;

extern void Init_GSL(void);

#endif
