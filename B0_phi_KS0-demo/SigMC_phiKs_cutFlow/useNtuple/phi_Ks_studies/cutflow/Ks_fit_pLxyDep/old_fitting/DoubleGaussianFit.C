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

void DoubleGaussianFit(){
    const char *treeName("tPre_p_ind");

    RooRealVar M("M","M",.46,.54);
    RooDataSet data("data","MC data",M, ImportFromFile("Combo_Ks_toFit.root",treeName));

    // Plot 
    M.setBins(200);
    RooPlot *frame = M.frame(Title(treeName));
    data.plotOn(frame);

    // Signal double Gaussian
    RooRealVar mean("mean","mean of signal", .49,.47,.50);
    RooRealVar sigma1("sigma1","sigma1",.0012,.0001,.0018);
    RooRealVar sigma2("sigma2","sigma2",.0045,.003,.0049);
    //RooRealVar sigma1("sigma1","sigma1",.0016,.0001,.002);
    //RooRealVar sigma2("sigma2","sigma2",.0048,.002,.009);
    RooGaussian sig1("sig1","sig1",M,mean,sigma1);
    RooGaussian sig2("sig2","sig2",M,mean,sigma2);
    RooRealVar frac1("frac1","frac1",.5,0,1);
    RooRealVar frac2("frac2","frac2",.5,0,1);
    RooAddPdf signal("signal","sig1+sig2",RooArgList(sig1,sig2),RooArgList(frac1,frac2));

    // Background
    RooRealVar c0("c0","c0",1.0,-1.,1.);
    RooChebychev background("background","background",M,RooArgList(c0));

    // Add signal and bakcground
    RooRealVar nsig("nsig","nsig", 4000,0,1e8);
    RooRealVar nbkg("nbkg","nbkg",40000,0,1e8);
    RooAddPdf model("model","sig+bkg",RooArgList(signal,background),RooArgList(nsig,nbkg));

    // Fitting
    M.setRange("bkg1",.460,.485);
    M.setRange("bkg2",.515,.535);
    M.setRange("peak",.485,.515);

    background.fitTo(data,Range("bkg1","bkg2"));
    signal.fitTo(data, Range("peak"));
    model.fitTo(data, Range("Full"));
    model.plotOn(frame, Range("Full"), Components(background), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(frame, Range("Full"));
    model.plotOn(frame, Range("Full"), Components(signal), DrawOption("F"), FillColor(kOrange), MoveToBack());

    sig1.paramOn(frame, Layout(.1,.3,.9));
    frame->getAttText()->SetTextSize(.02);
    sig2.paramOn(frame, Layout(.1,.3,.78));
    frame->getAttText()->SetTextSize(.02);

    nsig.Print();
    nbkg.Print();

    TCanvas *c_M = new TCanvas("c_M","c_M",1000,800);
    frame -> Draw();
    
    return ;
}
