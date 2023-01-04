/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooRarFit
 *    File: $Id: rarHistPdf.cc,v 1.2 2007/06/29 08:37:36 zhanglei Exp $
 * Authors: Lei Zhang
 * History:
 * 
 * Copyright (C) 2005-2006 University of California, Riverside
 *****************************************************************************/

// -- CLASS DESCRIPTION [RooRarFit] --
// This class provides HistPdf class for RooRarFit

#include "RooRarFit/rarVersion.hh"

#include <iostream>
#include <fstream>
using namespace std;

#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooProdPdf.h"
#include "RooRealVar.h"
#include "RooStringVar.h"

#include "RooHistPdf.h"
#include "RooRarFit/rarHistPdf.hh"

/// \brief Trivial ctor
///
/// Usually the objects should be created using other ctors.
rarHistPdf::rarHistPdf()
  : rarBasePdf(),
    _theHist(0)
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
rarHistPdf::rarHistPdf(const char *configFile, const char *configSec,
		       const char *configStr,
		       rarDatasets *theDatasets, RooDataSet *theData,
		       const char *name, const char *title)
  : rarBasePdf(configFile, configSec, configStr,
	       theDatasets, theData, name, title),
    _theHist(0)
{
  init();
}

rarHistPdf::~rarHistPdf()
{
}

/// \brief Initial function called by ctor
///
/// \p init is called by the ctor.
/// It first reads in observable info from config item \p obs,
/// and use #getFormulaArgs to get ArgList of the PDF,
/// and finally it builds RooHistPdf.
void rarHistPdf::init()
{
  cout<<"init of rarHistPdf for "<<GetName()<<":"<<endl;
  
  // no need for pdf fit
  setControlBits("noPdfFit");
  
  // read the obs string
  rarStrParser obsStrParser=readConfStr("obs", "", getVarSec());
  // get obs list
  getFormulaArgs(obsStrParser);
  
  cout<<" Obs in pdf:"<<endl;
  _obsSet.Print("v");
  
  // it should always have fitData
  if (!_theData) {
    cout<<" No dataset for HistPdf"<<endl;
    exit(-1);
  }
  // create the RooDataHist
  _theHist=new RooDataHist(Form("the_%s_Hist", GetName()),
			   "pdf histogram", _obsSet, *_theData);
  // create the generic pdf
  _thePdf=new RooHistPdf(Form("the_%s", GetName()), _pdfType+" "+GetTitle(),
			 _obsSet, *_theHist);
}
