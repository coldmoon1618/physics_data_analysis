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

// 2D fits DeE&Mbc of all events
void FitFull2D(){
    // ------------------------------------------------
    //observables
    RooRealVar Mbc("Mbc","Mbc",5.23,5.3);
    RooRealVar deltaE("deltaE","deltaE",-.1,.1);
    RooRealVar isBBbar("isBBbar","",0,1);
    RooDataSet data("data","MC data", RooArgSet(Mbc,deltaE,isBBbar), ImportFromFile("FullMC_cand.root","B0_cand")); 
    //RooDataSet data("data","MC data", RooArgSet(Mbc,deltaE,isBBbar), ImportFromFile("FullMC_cand.root","B0_cand"),Cut("isBBbar<.05")); 

    Mbc.Print();
    Mbc.setBins(35);
    RooPlot *Mbc_frame = Mbc.frame(Title("M_{bc}"));
    data.plotOn(Mbc_frame);
    deltaE.Print();
    deltaE.setBins(40);
    RooPlot *deE_frame = deltaE.frame(Title("#Delta E"));
    data.plotOn(deE_frame);
   
    TH1 *H_data = data.createHistogram("H_data", Mbc, Binning(35), YVar(deltaE, Binning(40)));

    // n Events
    RooRealVar nsig("nsig", "", 3e1, 0, 3e6);
    RooRealVar nbkg("nbkg", "", 1e2, 0, 5e6);
    RooRealVar f_a("f_a","",.25,0,1);

    // deltaE signal
    RooRealVar deE_mean("deE_mean", "mean of deltaE sig",0,0,.01);
    RooRealVar deE_sigma1("deE_sigma1", "tail width of deltaE sig", .0281);
    RooRealVar deE_sigma2("deE_sigma2", "core width of deltaE sig", .00793);
    RooGaussian deE_sig1("deE_sig1", "Signal", deltaE, deE_mean, deE_sigma1);
    RooGaussian deE_sig2("deE_sig2", "Signal", deltaE, deE_mean, deE_sigma2);
    RooRealVar f_tail("f_tail", "", .1888);
    RooAddPdf deE_sig("deE_sig", "sig1+sig2", RooArgList(deE_sig1, deE_sig2), RooArgList(f_tail));

    // deltaE bkg
    //TODO
    //RooRealVar c_exp("c_exp","",-5,-10,-.01); 
    RooRealVar c_exp("c_exp","",-1.2561); 
    RooExponential deE_bkg("deE_bkg","",deltaE,c_exp);
    //RooExponential deE_pkg("deE_pkg","",deltaE,c_exp);
    RooGaussian deE_pkg("deE_pkg","",deltaE,deE_mean,deE_sigma1);

    RooAddPdf deE_BKG("deE_BKG","",RooArgList(deE_pkg,deE_bkg),f_a);
    RooAddPdf deE_model("deE_model","deE_model",RooArgList(deE_sig,deE_BKG),RooArgList(nsig,nbkg));

    // Mbc signal
    //RooRealVar Mbc_mean("Mbc_mean", "mean of Mbc sig", 5.28,5.2,5.3);
    //RooRealVar Mbc_sigma("Mbc_sigma", "core width of Mbc sig", 0.0016,0.,0.005);
    RooRealVar Mbc_mean("Mbc_mean", "mean of Mbc sig", 5.279);
    RooRealVar Mbc_sigma("Mbc_sigma", "core width of Mbc sig", 0.0025);
    RooGaussian Mbc_sig("Mbc_sig", "Signal", Mbc, Mbc_mean, Mbc_sigma);
    RooGaussian Mbc_pkg("Mbc_pkg", "pkg", Mbc, Mbc_mean, Mbc_sigma);

    // Mbc bkg
    //TODO
    //RooRealVar Mbc_thres("Mbc_thres","",5.2895,5.289,5.29);
    //RooRealVar Mbc_coef("Mbc_coef","coef",10,-1e3,1e3);
    RooRealVar Mbc_thres("Mbc_thres","",5.28977);
    RooRealVar Mbc_coef("Mbc_coef","coef",-32.38);
    RooArgusBG Mbc_bkg("Mbc_bkg","bkg",Mbc,Mbc_thres,Mbc_coef);
    RooAddPdf Mbc_BKG("Mbc_BKG","",RooArgList(Mbc_pkg,Mbc_bkg),f_a);

    RooAddPdf Mbc_model("Mbc_model","sig+bkg",RooArgList(Mbc_sig,Mbc_BKG),RooArgList(nsig,nbkg));
    
    // 2D model
    RooProdPdf sig("sig","",RooArgSet(deE_sig,Mbc_sig));
    RooProdPdf bkg("bkg","",RooArgSet(deE_bkg,Mbc_bkg));
    RooProdPdf pkg("pkg","",RooArgSet(deE_pkg,Mbc_pkg));
    RooProdPdf BKG("BKG","",RooArgSet(deE_BKG,Mbc_BKG));
    RooAddPdf model("model","",RooArgList(sig,BKG),RooArgList(nsig,nbkg));
    model.fitTo(data);

    TH1 *H_pdf = model.createHistogram("H_pdf", Mbc, Binning(35), YVar(deltaE, Binning(40)));
    deE_model.plotOn(deE_frame);
    deE_model.plotOn(deE_frame, Range("plotRange"), Components(deE_BKG), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
    deE_model.plotOn(deE_frame, Range("plotRange"), Components(deE_sig), DrawOption("F"), FillColor(kOrange), MoveToBack());
    Mbc_model.plotOn(Mbc_frame);
    Mbc_model.plotOn(Mbc_frame, Range("plotRange"), Components(Mbc_BKG), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
    Mbc_model.plotOn(Mbc_frame, Range("plotRange"), Components(Mbc_sig), DrawOption("F"), FillColor(kOrange), MoveToBack());

    nsig.Print();
    nbkg.Print();
    f_a.Print();

    gStyle -> SetOptStat(0);
    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 400);
    c2 -> Divide(3,1);
    c2 -> cd(1);
    Mbc_frame -> Draw();
    c2 -> cd(2);
    deE_frame -> Draw();
    c2 -> cd(3);
    H_data-> Draw("lego");

    return;

}//End File
