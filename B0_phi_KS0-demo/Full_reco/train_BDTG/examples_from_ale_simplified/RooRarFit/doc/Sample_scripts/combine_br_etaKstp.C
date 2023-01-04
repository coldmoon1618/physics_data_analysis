// *
// * Project: BaBar detector at the SLAC PEP-II B-factory
// * Package: RooRarFit
// *    File: $Id: combine_br_etaKstp.C,v 1.2 2007/06/29 08:37:47 zhanglei Exp $
// * Authors: Lei Zhang
// * History:
//
// This sample macro is adopted from combine.cc of Stephen Bailey
//----------------------------------------------------------------------
// Copy this macro to your workdir, edit it for your mode,
// and then run the macro like:
// bbrroot -l combine_892_etaKstp.C

{
  gROOT->LoadMacro("../RooRarFit/doc/Sample_scripts/combine.cc");
  // comment out the above line and
  // uncomment the following two lines if you want to load combine.cc compiled
  //gSystem->SetIncludePath(" -IRELEASE -IRELEASE/tmp/$BFARCH ");
  //gROOT->ProcessLine(".L ../RooRarFit/doc/Sample_scripts/combine.cc++");
  
  // change number of modes to what you have
  Int_t nModes=4;
  // specify all the root files w/ NLL RooPlots
  Char_t* fileNames[nModes] = {
    "results/etakstar_ggkpi0_892_r15.BRFit.scanPlot.BrScan.root",
    "results/etakstar_3pikpi0_892_r15.BRFit.scanPlot.BrScan.root",
    "results/etakstar_ggkspip_892_r15.BRFit.scanPlot.BrScan.root",
    "results/etakstar_3pikspip_892_r15.BRFit.scanPlot.BrScan.root",
  };
  // specify RooPlot name per root file
  Char_t* plotNames[nModes] = {
    "NLLScanPlot_measBR",
    "NLLScanPlot_measBR",
    "NLLScanPlot_measBR",
    "NLLScanPlot_measBR",
  };
  // specify fit bias for each mode, if the curve has been corrected, set to 0s
  Double_t fitBias[nModes] = {
    0.,
    0.,
    0.,
    0.,
  };
  
  // See App G of BAD521 for more info on additive, uncorrelated,
  // and correlated errors
  // All errors are absolute errors
  
  // Specify low additive syst. error per mode, if not known, set to 0s,
  // if avg errors are used, set to avg errors.
  Double_t addSystErrLo[nModes] = {
    1.2, // the first mode low
    1.8, // the second mode low
    1.1, // the first mode low
    1.1, // the second mode low
  };
  // Specify high additive syst. error per mode, if not known, set to 0s,
  // if avg errors are used, set to avg errors.
  Double_t addSystErrHi[nModes] = {
    1.2, // the first mode high
    1.8, // the second mode high
    1.1, // the first mode high
    1.1, // the second mode high
  };
  // Specify low uncorrelated syst. error per mode, if not known, set to 0s,
  // if avg errors are used, set to avg errors.
  // N.B. The uncorrelated errors here include additive errors listed above,
  // so you should always have uncorrelated>=additive
  Double_t uncorrSystErrLo[nModes]= {
    0.9, // the first mode low
    1.4, // the second mode low
    1.1, // the first mode low
    1.1, // the second mode low
  };
  // Specify high uncorrelated syst. error per mode, if not known, set to 0s,
  // if avg errors are used, set to avg errors.
  // N.B. The uncorrelated errors here include additive errors listed above,
  // so you should always have uncorrelated>=additive
  Double_t uncorrSystErrHi[nModes]= {
    0.9, // the first mode high
    1.4, // the second mode high
    1.1, // the first mode high
    1.1, // the second mode high
  };
  // Specify correlated syst. error per mode, if not known, set to 0s
  Double_t corrSystErr[nModes]= {
    1.4, // the first mode
    2.0, // the second mode
    0.8, // the first mode
    0.5, // the second mode
  };
  // Give x axis a name
  Char_t* xAxisTitle= "BR(B#rightarrow#etaK^{*+})(#times10^{-6})";
  // Calculate signf
  Bool_t doSignf=kTRUE;
  // Calculate UL
  Bool_t doUL=kTRUE;
  // Specify confident level
  Double_t CL=0.90;
  // combine the curves, draw them, calcuate combined mean, errors,
  // signf., and UL based on the final curve
  RooPlot *thePlot=combine(nModes, fileNames, plotNames, fitBias,
                           addSystErrLo, addSystErrHi,
                           uncorrSystErrLo, uncorrSystErrHi,
                           corrSystErr,
			   xAxisTitle, doSignf, doUL, CL);
}






