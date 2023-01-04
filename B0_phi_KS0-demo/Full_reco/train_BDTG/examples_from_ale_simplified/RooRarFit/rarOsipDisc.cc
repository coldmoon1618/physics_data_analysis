
#include "RooRarFit/rarVersion.hh"

#include <iostream>
#include <fstream>
using namespace std;

#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooProdPdf.h"
#include "RooRealVar.h"
#include "RooStringVar.h"


#include "RooRarFit/RooOsipDisc.hh"


#include "RooRarFit/rarOsipDisc.hh"

/// \brief Trivial ctor
///
/// Usually the objects should be created using other ctors.
rarOsipDisc::rarOsipDisc()
  : rarBasePdf(),
    _x(0), _r(0), _b(0), _a(0), _h(0), _s(0)
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
rarOsipDisc::rarOsipDisc(const char *configFile, const char *configSec,
			 const char *configStr,
			 rarDatasets *theDatasets, RooDataSet *theData,
			 const char *name, const char *title)
  : rarBasePdf(configFile, configSec, configStr,
	       theDatasets, theData, name, title),
    _x(0), _r(0), _b(0), _a(0), _h(0), _s(0)
{
  init();
}

rarOsipDisc::~rarOsipDisc()
{
}

/// \brief Initial function called by ctor
///
/// \p init is called by the ctor.
/// It first creates the parameters by calling #createAbsReal,
/// and finally it builds RooGaussian/RooBreitWigner PDF
/// with #_pdfType being Gaussian/BreitWigner, respectively.
void rarOsipDisc::init()
{
  cout<<"init of rarOsipDisc for "<<GetName()<<":"<<endl;
  
  // first get its dependent/observable
  _x=createAbsReal("x", "observable"); assert(_x);
  RooRealVar *x=(RooRealVar *)RooArgList(_obsSet).at(0); assert(x);
  // Config pdf params
  
  _r=createAbsReal("r", "#r", .1, 0., 1., _x->getUnit());
  _b=createAbsReal("b", "#b", .1, 0., 1., _x->getUnit());
  _a=createAbsReal("a", "#a", .1, 0., 1., _x->getUnit());
  _h=createAbsReal("h", "#h", 1.0, -10, 10, _x->getUnit());
  _s=createAbsReal("s", "#s", .1, 0., 1., _x->getUnit());

  _params.Print("v");
  
  // create pdf
  if("OsipDisc"==_pdfType) {
    _thePdf=new RooOsipDisc(Form("the_%s", GetName()),
			       _pdfType+" "+GetTitle(), *_x, *_r, *_b, *_a, *_h, *_s);
  }
}
