/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooRarFit
 *    File: $Id: combine.cc,v 1.11 2007/06/29 08:37:47 zhanglei Exp $
 * Authors: Lei Zhang
 * History:
 * 
 * Copyright (C) 2005 University of California, Riverside
 *****************************************************************************/

// This macro provides functions for NLL scan plots/curves
// using RooRarFit facilities
// Sample macro to call these functions is combine.C

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "RooFitCore/RooPlot.hh"
#include "RooRarFit/rarMLFitter.hh"
#include "RooRarFit/rarNLL.hh"
#endif


// Wrappers of RooRarFit functions
// `Add' Error onto NLL curve
// maxNLL is the pointer to store max NLL value for plotting
void addErrToCurve(RooCurve *curve, Double_t errLo, Double_t errHi,
		   Double_t *maxNLL=0) {
  rarMLFitter::addErrToCurve(curve, errLo, errHi, maxNLL);
}
void addErrToCurve(RooCurve *curve, Double_t err, Double_t *maxNLL=0) {
  addErrToCurve(curve, err, err, maxNLL);
}
// Shift NLL curve with dx and dy
RooCurve *shiftNLLCurve(RooCurve *curve, Double_t dx, Double_t dy) {
  return rarMLFitter::shiftNLLCurve(curve, dx, dy);
}
// Combine NLL curves and shift the new min to zero (if shiftToZero)
RooCurve *combineNLLCurves(TList &curves, Bool_t shiftToZero=kTRUE,
			   Double_t *maxNLL=0) {
  return rarMLFitter::combineNLLCurves(curves, shiftToZero, maxNLL);
}
// Combine NLL curves and their CORRELATED errors
// maxNLL is pointer to store maxNLL value for plotting
RooCurve *combineNLLCurves(TList &curves,Double_t errs[], Double_t *maxNLL=0) {
  return rarMLFitter::combineNLLCurves(curves, errs, maxNLL);
}
// Get significance wrt refVal on NLL curve
Double_t getSignf(RooCurve *curve, Double_t refVal=0.) {
  return rarMLFitter::getSignf(curve, refVal);
}
// Get upper limit (default CL set to 90%)
Double_t getUL(RooCurve *curve, Double_t CL=0.90) {
  return rarMLFitter::getUL(curve, CL);
}
// Get mean and errors from NLL curve
Double_t getMeanErrs(RooCurve *curve, Double_t *errLo=0, Double_t *errHi=0) {
  return rarMLFitter::getMeanErrs(curve, errLo, errHi);
}

// combine NLL curves (w sys errs), draw the plots,
// calculate signf. and UL based on the final curve
RooPlot *combine(Int_t nModes, Char_t* fileNames[], Char_t* plotNames[],
		 Double_t fitBias[],
                 Double_t addSystErrLo[], Double_t addSystErrHi[],
		 Double_t uncorrSystErrLo[], Double_t uncorrSystErrHi[],
		 Double_t corrSystErr[],
		 Char_t *xAxisTitle=0,
		 Bool_t doSignf=kTRUE,
		 Bool_t doUL=kFALSE, Double_t CL=.90)
{
  RooPlot *thePlot(0);
  // List of RooCurves
  TList curves; // original individual NLL curves
  TList curvesWadds; // individual NLL curves with additive errors
  TList curvesWerrs; // individual NLL curves with uncorrelated errors
  // RooPlot properties
  TString yAxisTitle="-2 ln (L/L_{0})";
  Double_t xMin(0), xMax(0), yMax(0);
  // read in all the NLL Curves
  for (Int_t i=0; i<nModes; i++) {
    TFile *f=new TFile(fileNames[i]);
    if (!f) {
      cout<<" Can not read from root file: "<<fileNames[i]<<endl;
      return thePlot;
    }
    RooPlot *scanPlot=(RooPlot*)f->Get(plotNames[i]);
    if (!scanPlot) {
      cout<<" Can not read in RooPlot "<<plotNames[i]
	  <<" from "<<fileNames[i]<<endl;
      return thePlot;
    }
    Double_t theYmax=scanPlot->GetMaximum();
    if (yMax<theYmax) yMax=theYmax;
    TAxis *a=scanPlot->GetXaxis();
    Double_t theXmin=a->GetXmin();
    if (xMin>theXmin) xMin=theXmin;
    Double_t theXmax=a->GetXmax();
    if (xMax<theXmax) xMax=theXmax;
    if (!xAxisTitle) xAxisTitle=(Char_t*)a->GetTitle();
    // Get curve
    RooCurve *nllCurve=scanPlot->getCurve("NLL_curve");
    if (!nllCurve) {
      cout<<" Can not read NLL curve NLL_curve from RooPlot "<<plotNames[i]
	  <<" in root file "<<fileNames[i]<<endl;
      return thePlot;
    }
    // rename it so one can tell which is which
    nllCurve->SetNameTitle(Form("NLL_curve_Mode%d", i),
                           Form("curve for sub-mode %d", i));
    { // now shift the curve to min=0
      Double_t mean(0), yMin(0);
      rarNLL nll(nllCurve);
      nll.getMin(mean, yMin);
      shiftNLLCurve(nllCurve, -fitBias[i], -yMin);
    }
    // Clone it
    RooCurve *nllCurveWadd=(RooCurve*)nllCurve->Clone();
    RooCurve *nllCurveWerr=(RooCurve*)nllCurve->Clone();
    // `add' errors onto the curve
    addErrToCurve(nllCurveWadd, addSystErrLo[i], addSystErrHi[i]);
    addErrToCurve(nllCurveWerr, uncorrSystErrLo[i], uncorrSystErrHi[i]);
    // add the curve into lists
    curves.Add(nllCurve);
    curvesWadds.Add(nllCurveWadd);
    curvesWerrs.Add(nllCurveWerr);
  }
  //curves.Print();
  //curvesWadds.Print();
  //curvesWerrs.Print();
  
  // combine the curve together
  RooCurve *tCurve=combineNLLCurves(curves, kTRUE, &yMax);
  tCurve->SetNameTitle("NLL_curve_total", "total curve w/o syst errors");
  // combine the curve (w additive errors) together
  RooCurve *aCurve=combineNLLCurves(curvesWadds, kTRUE, &yMax);
  aCurve->SetNameTitle("NLL_curve_additive", "total curve w/ additve errors");
  // combine the curve (w uncorrelated errors) together
  RooCurve *uCurve=combineNLLCurves(curvesWerrs, kTRUE, &yMax);
  uCurve->SetNameTitle("NLL_curve_unCorr", "total curve w/ unCorr. errors");
  // combine the curves and add correlated errors
  RooCurve *cCurve=combineNLLCurves(curvesWerrs, corrSystErr, &yMax);
  cCurve->SetNameTitle("NLL_curve_Corr", "total curve w/ ALL syst errors");
  // Draw the RooPlot
  thePlot=new RooPlot(xMin, xMax, 0, yMax);
  thePlot->SetNameTitle(plotNames[0], plotNames[0]);
  thePlot->SetYTitle(yAxisTitle);
  thePlot->SetXTitle(xAxisTitle);
  // total curve without syst. error
  thePlot->addPlotable(tCurve);
  tCurve->SetLineWidth(2);
  tCurve->SetLineStyle(2);
  thePlot->setInvisible(tCurve->GetName()); // invisible
  // total curve w/ additive errors
  thePlot->addPlotable(aCurve);
  aCurve->SetLineWidth(2);
  aCurve->SetLineStyle(4);
  aCurve->SetLineColor(kYellow);
  thePlot->setInvisible(aCurve->GetName()); // invisible
  // total curve w/ uncorrelated errors
  thePlot->addPlotable(uCurve);
  uCurve->SetLineWidth(2);
  uCurve->SetLineStyle(4);
  uCurve->SetLineColor(kGreen);
  thePlot->setInvisible(uCurve->GetName()); // invisible
  // total curve w/ all errors
  thePlot->addPlotable(cCurve);
  cCurve->SetLineWidth(2);
  // add individual NLL curves
  for (Int_t i=0; i<nModes; i++) {
    RooCurve *theCCurve=(RooCurve*)curves.At(i)->Clone();
    thePlot->addPlotable(theCCurve);
    theCCurve->SetLineWidth(1);
    theCCurve->SetLineColor(rarBasePdf::getColor(i));
    theCCurve->SetLineStyle(0==(i%2)?2:4);
  }
  // draw it
  thePlot->Draw();
  thePlot->Print("v");
  cout<<endl
      <<" To show hidden curves (those with Option \":I\"),"
      <<" type, for example"<<endl
      <<"   NLL_curve_total->SetDrawOption(\"\")"
      <<endl
      <<" To hide a curve, type, for example"<<endl
      <<"   NLL_curve_Mode0->SetDrawOption(\"I\")"
      <<endl
      <<" Then redraw the plot: gPad->Modified()"
      <<endl;
  // get new mean, stat. error, syst. errors
  Double_t theMean(0), systErrLo(0), systErrHi(0), statErrLo(0), statErrHi(0);
  // find stat. errors
  getMeanErrs(tCurve, &statErrLo, &statErrHi);
  // find final mean and syst. errors
  theMean=getMeanErrs(cCurve, &systErrLo, &systErrHi);
  // get pure syst. error
  systErrLo=(systErrLo*systErrLo-statErrLo*statErrLo);
  if (systErrLo>=0) systErrLo=-sqrt(systErrLo);
  else {
    cout<<" systErrLo^2="<<systErrLo<<endl
	<<" Can not be true!!!! Please check!!!"<<endl;
  }
  systErrHi=(systErrHi*systErrHi-statErrHi*statErrHi);
  if (systErrHi>=0) systErrHi=sqrt(systErrHi);
  else {
    cout<<" systErrHi^2="<<systErrHi<<endl
	<<" Can not be true!!!! Please check!!!"<<endl;
  }
  // signf and UL
  Double_t signfStat(0), ULStat(0), signf(0), UL(0);
  if (doSignf) {
    signfStat=getSignf(tCurve);
    signf=getSignf(aCurve);
  }
  if (doUL) {
    ULStat=getUL(tCurve, CL);
    UL=getUL(cCurve, CL);
  }
  
  // final output for combined results
  cout<<endl;
  cout<<"Combined results based on "<<cCurve->GetName()
      <<" (and "<<tCurve->GetName()<<" for stat.):"<<endl
      <<" mean="<<theMean
      <<" ("<<statErrLo<<", +"<<statErrHi<<")(stat)"
      <<" ("<<systErrLo<<", +"<<systErrHi<<")(syst)"
      <<endl;
  if (doSignf) cout<<" Signf = "<<signf<<" (wrt 0)"
		   <<" (stat only = "<<signfStat<<")"<<endl;
  if (doUL) cout<<" UL = "<<UL<<" (CL="<<CL<<")"
		<<" (stat only = "<<ULStat<<")"<<endl;
  
  return thePlot;
}

// combine NLL curves (w sys errs), draw the plots,
// calculate signf. and UL based on the final curve
RooPlot *combine(Int_t nModes, Char_t* fileNames[], Char_t* plotNames[],
		 Double_t fitBias[],
		 Double_t uncorrSystErrLo[], Double_t uncorrSystErrHi[],
		 Double_t corrSystErr[],
		 Char_t *xAxisTitle=0,
		 Bool_t doSignf=kTRUE,
		 Bool_t doUL=kFALSE, Double_t CL=.90)
{
  RooPlot *thePlot=combine(nModes, fileNames, plotNames, fitBias,
                           uncorrSystErrLo, uncorrSystErrHi,
                           uncorrSystErrLo, uncorrSystErrHi,
                           corrSystErr,
                           xAxisTitle, doSignf, doUL, CL);
  cout<<endl<<" W A R N I N G ! !"<<endl
      <<" You are using obsolete combine.C script!!"<<endl
      <<" Please copy RooRarFit/doc/Sample_scripts/combine.C "<<endl
      <<" And merge your current script into it."<<endl<<endl
      <<" Your current signf value might be LOWER than it should be!"<<endl
      <<endl;
  return thePlot;
}

// combine NLL curves (w sys errs), draw the plots,
// calculate signf. and UL based on the final curve
RooPlot *combine(Int_t nModes, Char_t* fileNames[], Char_t* plotNames[],
		 Double_t uncorrSystErrLo[], Double_t uncorrSystErrHi[],
		 Double_t corrSystErr[],
		 Char_t *xAxisTitle=0,
		 Bool_t doSignf=kTRUE,
		 Bool_t doUL=kFALSE, Double_t CL=.90)
{
  TArrayD fitBias(nModes);
  fitBias.Reset();
  return combine(nModes, fileNames, plotNames, fitBias.GetArray(),
		 uncorrSystErrLo, uncorrSystErrHi, corrSystErr,
		 xAxisTitle, doSignf, doUL, CL);
}

// combine NLL curves (w sys errs), draw the plots,
// calculate signf. and UL based on the final curve
RooPlot *combine(Int_t nModes, Char_t* fileNames[], Char_t* plotNames[],
		 Double_t uncorrSystErr[], Double_t corrSystErr[],
		 Char_t *xAxisTitle=0,
		 Bool_t doSignf=kTRUE,
		 Bool_t doUL=kFALSE, Double_t CL=.90)
{
  return combine(nModes, fileNames, plotNames,
		 uncorrSystErr, uncorrSystErr, corrSystErr,
		 xAxisTitle, doSignf, doUL, CL);
}

// combine RooCurves together
RooCurve *combCurves(RooCurve* crv1, RooCurve* crv2,
		     const char* formula, const char* valid="1",
		     const char* crvName=0, Int_t lineWidth=2,
		     Int_t lineStyle=2, Int_t lineColor=kBlue) {
  return rarMLFitter::combCurves(crv1, crv2, formula, valid,
				 crvName, lineWidth, lineStyle,lineColor);
}

Char_t *combCurves(RooPlot *plot,
		   const char *crvName1, const char *crvName2)
{
  RooCurve *retCurve(0);
  RooCurve *crv(0);
  Int_t lineWidth=2;
  Int_t lineStyle=2;
  Int_t lineColor=kBlue;
  if (crvName1&&(crv=plot->getCurve(crvName1))) {
    lineWidth=crv->GetLineWidth();
    lineStyle=crv->GetLineStyle();
    lineColor=crv->GetLineColor();
    retCurve=crv;
  }
  if (crvName2&&(crv=plot->getCurve(crvName2))) {
    if (retCurve) {
      plot->setInvisible(retCurve->GetName());
      plot->setInvisible(crv->GetName());
      retCurve=combCurves(retCurve, crv, "@0+@1", "1",
                          Form("%s_%s", retCurve->GetName(), crv->GetName()),
                          lineWidth, lineStyle, lineColor);
      plot->addPlotable(retCurve);
    } else {
      retCurve=crv;
    }
  }
  
  if (retCurve) return (Char_t*) retCurve->GetName();
  return 0;
}

Char_t *combCurves(RooPlot *plot,
		   const char *crvName1, const char *crvName2,
		   const char *crvName3, const char *crvName4=0,
		   const char *crvName5=0, const char *crvName6=0,
		   const char *crvName7=0, const char *crvName8=0,
		   const char *crvName9=0, const char *crvName10=0)
{
  Char_t *retCurve=combCurves(plot, crvName1, crvName2);
  retCurve=combCurves(plot, retCurve, crvName3);
  retCurve=combCurves(plot, retCurve, crvName4);
  retCurve=combCurves(plot, retCurve, crvName5);
  retCurve=combCurves(plot, retCurve, crvName6);
  retCurve=combCurves(plot, retCurve, crvName7);
  retCurve=combCurves(plot, retCurve, crvName8);
  retCurve=combCurves(plot, retCurve, crvName9);
  retCurve=combCurves(plot, retCurve, crvName10);
  
  return retCurve;
}
