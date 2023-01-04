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

//Fits DeE of all isBphiK events with double Gaussian
void FitDoubleGaus(){
    // ------------------------------------------------
    //observables
    RooRealVar deltaE("deltaE","deltaE",-.1,.1);
    RooRealVar isBphiK("isBphiK","",0,1);
    RooDataSet deltaE_data("deltaE_data","MC deltaE_data", RooArgSet(deltaE,isBphiK), ImportFromFile("FullMC_cand.root","B0_cand"),Cut("isBphiK>0.5")); 

    deltaE.Print();
    deltaE.setBins(40);
    RooPlot *deltaE_frame = deltaE.frame(Title("#Delta E"));
    deltaE_data.plotOn(deltaE_frame);

    // Create a Gaussian PDF g(M,mean,sigma) 
    RooRealVar mean("mean", "mean of deltaE sig", 0,0,.01);
    RooRealVar sigma1("sigma1", "tail width of deltaE sig", 0.01,0.,0.05);
    RooRealVar sigma2("sigma2", "core width of deltaE sig", 0.005,0.,0.05);
    RooGaussian sig1("sig1", "Signal", deltaE, mean, sigma1);
    RooGaussian sig2("sig2", "Signal", deltaE, mean, sigma2);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar fsig1("fsig1", "", .5, 0, 1);

    //signal region
    deltaE.setRange("signal", -.05, .05);
    RooAddPdf model("model", "sig1+sig2", RooArgList(sig1, sig2), RooArgList(fsig1));
    RooFitResult *r_deltaE_sig = model.fitTo(deltaE_data, Save(kTRUE),Range("signal"));
    model.plotOn(deltaE_frame, Range("Full"));

    fsig1.Print();

    sig1.paramOn(deltaE_frame, Layout(0.1,0.4,0.9));
    sig2.paramOn(deltaE_frame, Layout(0.1,0.4,0.78));
    TCanvas *c2 = new TCanvas("c2", "c2", 500, 400);
    deltaE_frame -> Draw();

    return;

}//End File
