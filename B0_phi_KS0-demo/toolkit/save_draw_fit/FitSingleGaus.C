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

void FitSingleGaus(){
#define MBC
#ifdef MBC
    // ------------------------------------------------
    //observables
    RooRealVar Mbc("Mbc","Mbc",5.25,5.29);
    Mbc.Print();
    //Mbc_data sets
    //RooDataSet Mbc_data("Mbc_data","MC Mbc_data",Mbc, ImportFromFile("SigMC_signal.root","tMatched")); 
    RooDataSet Mbc_data("Mbc_data","MC Mbc_data",Mbc, ImportFromFile("SigMC_signal_reduced.root","tMatched")); 
    Mbc.setBins(55);
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

    //signal region
    //Mbc.setRange("signal", 5.275, 5.285);
    Mbc.setRange("signal", 5.27, 5.285);

    RooFitResult *r_Mbc_sig = Mbc_sig1.fitTo(Mbc_data, Save(kTRUE),Range("signal"));
    Mbc_sig1.plotOn(Mbc_frame, Range("Full"));

    Mbc_nsig1.Print();

    Mbc_sig1.paramOn(Mbc_frame, Layout(0.1,0.4,0.9));
    Mbc_frame->getAttText()->SetTextSize(0.025);
    TCanvas *c1 = new TCanvas("c1", "c1", 900, 500);
    Mbc_frame -> Draw();

    cout<<"chi2 value"<<endl;
    cout<<Mbc_frame->chiSquare(2)<<endl;
#endif

//#define DELE
#ifdef DELE
    // ------------------------------------------------
    //observables
    RooRealVar deltaE("deltaE","deltaE",-.1,.1);
    RooDataSet deltaE_data("deltaE_data","MC deltaE_data",deltaE, ImportFromFile("SigMC_signal_reduced.root","tMatched")); 
    //RooDataSet deltaE_data("deltaE_data","MC deltaE_data",deltaE, ImportFromFile("SigMC_signal.root","tMatched")); 

    deltaE.Print();
    deltaE.setBins(85);
    RooPlot *deltaE_frame = deltaE.frame(Title("#Delta E"));
    deltaE_data.plotOn(deltaE_frame);

    // Create a Gaussian PDF g(M,mean,sigma) 
    RooRealVar deltaE_mean("deltaE_mean", "mean of deltaE sig", 0,0,.01);
    RooRealVar deltaE_sigma1("deltaE_sigma1", "core width of deltaE sig", 0.01,0.,0.1);
    RooGaussian deltaE_sig1("deltaE_sig1", "Signal", deltaE, deltaE_mean, deltaE_sigma1);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar deltaE_nsig1("deltaE_nsig1", "",  5e4,  0,  5e7);

    //signal region
    deltaE.setRange("signal", -.05, .05);
    RooFitResult *r_deltaE_sig = deltaE_sig1.fitTo(deltaE_data, Save(kTRUE),Range("signal"));
    deltaE_sig1.plotOn(deltaE_frame, Range("Full"));

    deltaE_nsig1.Print();

    deltaE_sig1.paramOn(deltaE_frame, Layout(0.1,0.4,0.9));
    deltaE_frame->getAttText()->SetTextSize(0.025);
    TCanvas *c2 = new TCanvas("c2", "c2", 900, 500);
    deltaE_frame -> Draw();

    cout<<"chi2 value"<<endl;
    cout<<deltaE_frame->chiSquare(2)<<endl;
#endif

    return;

}//End File
