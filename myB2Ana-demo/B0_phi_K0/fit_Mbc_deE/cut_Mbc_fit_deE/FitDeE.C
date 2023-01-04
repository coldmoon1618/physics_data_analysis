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

void FitDeE(){

    // ------------------------------------------------
    //observables
    RooRealVar deltaE("deltaE","deltaE",-.2,.2);
    RooRealVar Mbc("Mbc","",5.2,5.3);
    RooRealVar isBphiK("isBphiK","",0,1);
    //RooDataSet deltaE_data("deltaE_data","MC deltaE_data", RooArgSet(deltaE,Mbc,isBphiK), ImportFromFile("FullMC_cand.root","B0_cand"),Cut("Mbc>5.27&&isBphiK<0.5")); 
    RooDataSet deltaE_data("deltaE_data","MC deltaE_data", RooArgSet(deltaE,Mbc), ImportFromFile("FullMC_cand.root","B0_cand"),Cut("Mbc>5.27")); 

    deltaE.Print();
    deltaE.setBins(80);
    RooPlot *deltaE_frame = deltaE.frame(Title("#Delta E"));
    deltaE_data.plotOn(deltaE_frame);

    // Create a Gaussian PDF g(M,mean,sigma) 
    RooRealVar mean("mean", "mean of deltaE sig", .00385);
    RooRealVar sigma1("sigma1", "tail width of deltaE sig", 0.0281);
    RooRealVar sigma2("sigma2", "core width of deltaE sig", 0.00793);
    RooGaussian sig1("sig1", "Signal", deltaE, mean, sigma1);
    RooGaussian sig2("sig2", "Signal", deltaE, mean, sigma2);
    RooRealVar f_core("f_core","",.1888);
    //Fix PDF shape to isBphiK event fit
    //Float mean value
    //TODO: float horizontal scaling factor too

    RooAddPdf sig("sig", "sig1+sig2", RooArgList(sig1, sig2), RooArgList(f_core));

    RooRealVar c_exp("c_exp","",-5,-10,-.01); 
    RooExponential bkg("bkg","",deltaE,c_exp);

    // Composite model
    RooRealVar nsig("nsig", "number of signal events", 53, 0, 1e5);
    RooRealVar nbkg("nbkg", "number of background events", 103, 0, 5e5);
    RooAddPdf model("model", "model", RooArgList(sig, bkg), RooArgList(nsig, nbkg));

    //sideband region
    deltaE.setRange("sband1",-.2,-.05);
    deltaE.setRange("sband2",.05,.2);
    deltaE.setRange("signal",-.06,.06);

    bkg.fitTo(deltaE_data,Range("sband1","sband2"));
    model.fitTo(deltaE_data);
    model.plotOn(deltaE_frame, Range("plotRange"), Components(bkg), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
    model.plotOn(deltaE_frame, Range("plotRange"));
    model.plotOn(deltaE_frame, Range("plotRange"), Components(sig), DrawOption("F"), FillColor(kOrange), MoveToBack());

    nsig.Print();
    c_exp.Print();

    sig1.paramOn(deltaE_frame, Layout(0.1,0.4,0.9));
    bkg.paramOn(deltaE_frame, Layout(0.1,0.4,0.78));
    TCanvas *c2 = new TCanvas("c2", "c2", 500, 400);
    deltaE_frame -> Draw();
    c2 -> SaveAs("CutMbc_deE.pdf");

    return;

}//End File
