#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif


#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

#include <TChain.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>

//RooFit
#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit;

// 2D fits DeE&Mbc of all isBphiK events
void FitSignal2D(){
    // ------------------------------------------------
    //observables
    RooRealVar Mbc("Mbc","Mbc",5.23,5.3);
    RooRealVar deltaE("deltaE","deltaE",-.1,.1);
    RooRealVar isBphiK("isBphiK","",0,1);
    RooDataSet data("data","MC data", RooArgSet(Mbc,deltaE,isBphiK), ImportFromFile("FullMC_cand.root","B0_cand"),Cut("isBphiK>0.5")); 

    Mbc.Print();
    Mbc.setBins(35);
    RooPlot *Mbc_frame = Mbc.frame(Title("M_{bc}"));
    data.plotOn(Mbc_frame);
    deltaE.Print();
    deltaE.setBins(40);
    RooPlot *deE_frame = deltaE.frame(Title("#Delta E"));
    data.plotOn(deE_frame);
   
    TH1 *H_data = data.createHistogram("H_data", Mbc, Binning(35), YVar(deltaE, Binning(40)));

    // deltaE signal
    RooRealVar deE_mean("deE_mean", "mean of deltaE sig",0,0,.01);
    RooRealVar deE_sigma1("deE_sigma1", "tail width of deltaE sig", .01 ,0.,.05);
    RooRealVar deE_sigma2("deE_sigma2", "core width of deltaE sig", .005,0.,.05);
    RooGaussian deE_sig1("deE_sig1", "Signal", deltaE, deE_mean, deE_sigma1);
    RooGaussian deE_sig2("deE_sig2", "Signal", deltaE, deE_mean, deE_sigma2);
    RooRealVar f_tail("f_tail", "", .1888);
    RooAddPdf deE_sig("deE_sig", "sig1+sig2", RooArgList(deE_sig1, deE_sig2), RooArgList(f_tail));

    // Mbc signal
    RooRealVar Mbc_mean("Mbc_mean", "mean of Mbc sig", 5.28,5.277,5.283);
    RooRealVar Mbc_sigma("Mbc_sigma", "core width of Mbc sig", 0.0016,0.,0.005);
    RooGaussian Mbc_sig("Mbc_sig", "Signal", Mbc, Mbc_mean, Mbc_sigma);
    
    // 2D signal
    RooProdPdf signal("signal","",RooArgSet(deE_sig,Mbc_sig));
    signal.fitTo(data);
    TH1 *H_pdf = signal.createHistogram("H_pdf", Mbc, Binning(35), YVar(deltaE, Binning(40)));
    deE_sig.plotOn(deE_frame);
    deE_sig1.paramOn(deE_frame, Layout(0.1,0.4,0.9));
    deE_sig2.paramOn(deE_frame, Layout(0.1,0.4,0.78));
    Mbc_sig.plotOn(Mbc_frame);
    Mbc_sig.paramOn(Mbc_frame, Layout(0.1,0.4,0.9));

    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 400);
    c2 -> Divide(3,1);
    c2 -> cd(1);
    Mbc_frame -> Draw();
    c2 -> cd(2);
    deE_frame -> Draw();
    c2 -> cd(3);
    H_pdf -> Draw("lego");

    return;

}//End File
