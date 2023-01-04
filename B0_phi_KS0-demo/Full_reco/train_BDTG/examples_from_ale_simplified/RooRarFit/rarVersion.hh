/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooRarFit
 *    File: $Id: rarVersion.hh,v 1.52 2010/06/15 13:42:17 fwilson Exp $
 * Authors: Lei Zhang
 * History:
 *
 * Copyright (C) 2005 University of California, Riverside
 *****************************************************************************/
#ifndef RAR_VERSION
#define RAR_VERSION

#define RARFIT_VERSION "V00-01-73"

#include "RVersion.h"

#if ROOT_VERSION_CODE < ROOT_VERSION(4,0,0)
  #error "Do not support ROOT<4.0.0, RooFit<v1.92, Babar<16.0.3-physics-1"
  #define RAR_USE_ROOT3
  #define rar_setRange setFitRange
  #define rar_inRange inFitRange
  #define rar_getMin getFitMin
  #define rar_getMax getFitMax
  #define rar_setBins setFitBins
  #define rar_getBins getFitBins
#endif

#define USEROOFIT230
#ifndef USEROOFIT230
/// For RooRealVar
  #define rar_setRange setRange
/// For RooRealVar
  #define rar_inRange(a,b) inRange(a)
/// For RooRealVar
  #define rar_getMin getMin
/// For RooRealVar
  #define rar_getMax getMax
/// For RooRealVar
  #define rar_setBins setBins
/// For RooRealVar
  #define rar_getBins getBins
/// For projection plot
  #define rar_createProjection createProjection
#else
/// RooFit/ROOT version control macro
  #define RAR_USE_ROOT5
/// For RooRealVar
  #define rar_setRange setRange
/// For RooRealVar
  #define rar_inRange(a,b) inRange(a,b)
/// For RooRealVar
  #define rar_getMin getMin
/// For RooRealVar
  #define rar_getMax getMax
/// For RooRealVar
  #define rar_setBins setBins
/// For RooRealVar
  #define rar_getBins getBins
/// For projection plot
  #define rar_createProjection createPlotProjection
#endif

// os-related
#if defined(__SUNPRO_CC) && defined(_XOPEN_SOURCE) && ( _XOPEN_SOURCE - 0 == 500 )
#ifndef _CLOCK_T
#define _CLOCK_T
typedef long            clock_t; /* relative time in a specified resolution */
#endif  /* ifndef _CLOCK_T */
#endif // SUN and XOPENSOURCE=500


#endif
