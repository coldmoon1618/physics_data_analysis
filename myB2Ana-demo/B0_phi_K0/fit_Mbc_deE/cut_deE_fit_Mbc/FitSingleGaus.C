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

//Fits Mbc of all isBphiK events with single Gaussian
void FitSingleGaus(){
    // ------------------------------------------------
    //observables
    RooRealVar Mbc("Mbc","Mbc",5.23,5.3);
    RooRealVar isBphiK("isBphiK","",0,1);
    Mbc.Print();
    //Mbc_data sets
    RooDataSet Mbc_data("Mbc_data","MC Mbc_data", RooArgSet(Mbc,isBphiK), ImportFromFile("FullMC_cand.root","B0_cand"),Cut("isBphiK>0.5")); 
    Mbc.setBins(35);
    RooPlot *Mbc_frame = Mbc.frame(Title("M_{bc}"));
    Mbc_data.plotOn(Mbc_frame);

    // D e f i n e   s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------

    // Create a Gaussian PDF g(M,mean,sigma) 
    RooRealVar Mbc_mean("Mbc_mean", "mean of Mbc sig", 5.28,5.277,5.283);
    RooRealVar Mbc_sigma1("Mbc_sigma1", "core width of Mbc sig", 0.0016,0.,0.005);
    RooGaussian Mbc_sig1("Mbc_sig1", "Signal", Mbc, Mbc_mean, Mbc_sigma1);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar Mbc_nsig1("Mbc_nsig1", "",  5e4,  0,  5e7);
    //RooAddPdf model("model","sig",RooArgList(Mbc_sig1),RooArgList(Mbc_nsig1));

    //signal region
    Mbc.setRange("signal", 5.27, 5.285);

    RooFitResult *r_Mbc_sig = Mbc_sig1.fitTo(Mbc_data, Save(kTRUE),Range("signal"));
    Mbc_sig1.plotOn(Mbc_frame, Range("Full"));

    Mbc_sig1.paramOn(Mbc_frame, Layout(0.1,0.4,0.9));
    TCanvas *c1 = new TCanvas("c1", "c1", 500, 400);
    Mbc_frame -> Draw();

    return;

}//End File
