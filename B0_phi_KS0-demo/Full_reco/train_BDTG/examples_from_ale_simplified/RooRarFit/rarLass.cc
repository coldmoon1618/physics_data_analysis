/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooRarFit
 *    File: $Id: rarLass.cc,v 1.1 2010/06/15 13:09:58 fwilson Exp $
 * Authors: F. Wilson
 * History:
 * 
 * Copyright (C) 2006 RAL
 *****************************************************************************/

// -- CLASS DESCRIPTION [RooRarFit] --
// This class provides K-Matrix parameterisation of LASS lineshape Pdf class for RooRarFit

#include "RooRarFit/rarVersion.hh"

#include <iostream>
#include <fstream>
using namespace std;

#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooProdPdf.h"
#include "RooRealVar.h"
#include "RooStringVar.h"

// Include the RooFit Pdf
#include "RooRarFit/RooLass.hh"

// Include this classes header
#include "RooRarFit/rarLass.hh"

/// \brief Trivial ctor
///
/// Usually the objects should be created using other ctors.
rarLass::rarLass()
  : rarBasePdf(),
    _x(0), _mean(0), _width(0), _effRange(0), _scatlen(0), _turnOffVal(0)
{
  init();
}

/// \brief Default ctor
///
/// \param configFile The config file
/// \param configSec The config section
/// \param configStr The config string
/// \param theDatasets Available datasets
/// \param theData Default dataset for this PDF
/// \param name The name
/// \param title The title
///
/// The default ctor first initializes data members,
/// and then calls #init.
rarLass::rarLass(const char *configFile, const char *configSec,
		     const char *configStr,
		     rarDatasets *theDatasets, RooDataSet *theData,
		     const char *name, const char *title)
  : rarBasePdf(configFile, configSec, configStr,
	       theDatasets, theData, name, title),
    _x(0), _mean(0), _width(0), _effRange(0), _scatlen(0), _turnOffVal(0)
{
  init();
}

rarLass::~rarLass()
{
}

/// \brief Initial function called by ctor
///
/// \p init is called by the ctor.
/// It first creates the parameters by calling #createAbsReal,
/// and finally it builds the LASS K-matrix PDF.
void rarLass::init()
{
  cout<<"init of rarLass for "<<GetName()<<":"<<endl;
  
  _x=createAbsReal("x", "observable"); assert(_x);
  RooRealVar *x=(RooRealVar *)RooArgList(_obsSet).at(0); assert(x);
 
  _mean=createAbsReal("mean", "#mu", (x->rar_getMin()+x->rar_getMax())/2,
                      x->rar_getMin(), x->rar_getMax(), _x->getUnit());
  // initialise with LASS measured values, Nucl Phys B296, 493 (1988)
  _width      = createAbsReal("width", "#Lambda", 0.25, 0, 1000);
  _effRange   = createAbsReal("effRange", "R", 3.32, 0, 1000);
  _scatlen    = createAbsReal("scatlen", "s", 2.07, 0, 1000);
  // LASS not tested beyond about 1.65 GeV
  _turnOffVal = createAbsReal("turnOffVal", "t", 1.65, 0, 1000);
  _params.Print("v");

  _thePdf = new RooLass(Form("the_%s", GetName()),_pdfType+" "+GetTitle(),
			       *_x, *_mean, *_width, *_effRange, *_scatlen, *_turnOffVal);

}
