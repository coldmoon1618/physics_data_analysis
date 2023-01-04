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

void FitPhiSignal(){
    //observables
    RooRealVar M("M","M",0.98,1.06);

    M.Print();
    //data sets
    RooDataSet data("data","MC data",M, ImportFromFile("Combo_phi_signal.root","tMatched")); 
    M.setBins(80);
    RooPlot *phi_frame = M.frame(Title("m_#phi"));
    data.plotOn(phi_frame);

    // D e f i n e   s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------

    //Signal form: Breit-Wigner
    RooRealVar phi_mean("phi_mean", "mean of phi sig", 1.02,1.016,1.026);
    RooRealVar phi_sigma1("phi_sigma1", "core width of phi sig", 0.0025,0.,0.01);
    RooBreitWigner phi_sig1("phi_sig1","Signal", M, phi_mean, phi_sigma1);

    //Background form
    RooRealVar phi_A("phi_A","",20000,0,5e9);
    RooRealVar phi_B("phi_B","", 0.5,0., 100.);
    RooGenericPdf phi_bkg("phi_bkg","@1*sqrt(@0-0.987354)*exp(@2*(@0-0.987354))",RooArgList(M,phi_A,phi_B));

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar phi_nsig1("phi_nsig1", "",  50000,  0, 5000000);

    M.setRange("plotRange", 0.98, 1.05);
    M.setRange("fitRange", 1.01, 1.03);

    RooAddPdf phi_model("phi_model", "sig1", RooArgList(phi_sig1), RooArgList(phi_nsig1));
    RooFitResult *r_phi_sig = phi_model.fitTo(data, Save(kTRUE), Range("fitRange"));
    phi_model.plotOn(phi_frame, Range("plotRange"), Components(phi_bkg), LineStyle(kDashed), LineColor(kRed));
    phi_model.plotOn(phi_frame, Range("plotRange"));
    phi_model.plotOn(phi_frame, Range("plotRange"), Components("phi_sig1"), DrawOption("F"),FillColor(kOrange),MoveToBack());

    phi_nsig1.Print();

    // Draw the frame on the canvas
    phi_sig1.paramOn(phi_frame, Layout(0.1,0.45,0.9));
    phi_frame->getAttText()->SetTextSize(0.03);

    TCanvas *c1 = new TCanvas("c1", "c1", 500, 400);
    phi_frame -> Draw();
    c1 -> SaveAs("plots/FitPhiSignal.pdf");

    return;

}//End File
