// *
// * Project: BaBar detector at the SLAC PEP-II B-factory
// * Package: RooRarFit
// *    File: $Id: combine_acp_etaKst0.C,v 1.2 2007/06/29 08:37:47 zhanglei Exp $
// * Authors: Lei Zhang
// * History:
//
// This sample macro is adopted from combine.cc of Stephen Bailey
//----------------------------------------------------------------------
// Copy this macro to your workdir, edit it for your mode,
// and then run the macro like:
// bbrroot -l combine_acp_etaKstp.C

// This script combines Frac_nSig_chgCat_Plot NLL curves, no Acp NLL curves.  Significance is not handled correctly.
// For converting to Acp: If Frac = Frac +/- dFrac, then Acp = 1-2*Frac  -/+ 2*dFrac.

{
  gROOT->LoadMacro("../RooRarFit/doc/Sample_scripts/combine.cc");
  
  Int_t nModes=2;
  Char_t* fileNames[nModes] = {"results/etakstar_ggkpim_892_r15.mlFitter_Config.scanPlot.AcpScan.root",
			       "results/etakstar_3pikpim_892_r15.mlFitter_Config.scanPlot.AcpScan.root"};
  Char_t* plotNames[nModes] = {"NLLScanPlot_Frac_nSig_chgCat_Plus","NLLScanPlot_Frac_nSig_chgCat_Plus"};
  Double_t shift[nModes] = {0.,0.};
  Double_t addSystErrLo[nModes] = {0.0, 0.0};
  Double_t addSystErrHi[nModes] = {0.0, 0.0};
  Double_t uncorrSystErrLo[nModes]= {0., 0.};
  Double_t uncorrSystErrHi[nModes]= {0., 0.};
  Double_t corrSystErr[nModes]= {0.0, 0.0};
  // Give x axis a name
  Char_t* xAxisTitle= "Charge fraction(B->eta K*0(892))";
  // Calculate signf
  Bool_t doSignf=kTRUE;
  // Calculate UL
  Bool_t doUL=kTRUE;
  // Specify confident level
  Double_t CL=0.90;
  // combine the curves, draw them, calcuate combined mean, errors,
  // signf., and UL based on the final curve
  RooPlot *thePlot=combine(nModes, fileNames, plotNames, shift,
                           addSystErrLo, addSystErrHi,
                           uncorrSystErrLo, uncorrSystErrHi,
                           corrSystErr,
			   xAxisTitle, doSignf, doUL, CL);
}






