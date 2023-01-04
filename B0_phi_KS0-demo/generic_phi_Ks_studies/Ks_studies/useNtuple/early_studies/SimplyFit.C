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

//Selective fitting, also not iteration of different trees
void SimplyFit(){
    //observables
    RooRealVar M("M","M",0.46,0.54);
    M.Print();
    //data sets
    //RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MC13a_sigL_cut.root","unMatched"));//Quite slow for this one, also need to manually move the fit results 
    //RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MC13a_Candidate_KS_flat_matched.root","tree")); 
    //RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MC13a_KS_compareDistance.root","tree")); 
    //RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MC13a_reco_200k.root","tree")); 
    //RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MCBphiK_reco.root","tree")); 
    RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MC13a_generic_cut.root","tree")); 
    M.setBins(80);
    RooPlot *KS_frame = M.frame(Title("m_{K^0_S}"));
    KS_data.plotOn(KS_frame);

    // D e f i n e   s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------

    // Create a Gaussian PDF g(M,mean,sigma) 
    RooRealVar KS_mean("KS_mean", "mean of KS sig", 0.4977,0.497,0.498);
    RooRealVar KS_sigma1("KS_sigma1", "core width of KS sig", 0.0016,0.,0.005);
    RooRealVar KS_sigma2("KS_sigma2", "wide width of KS sig", 0.0056,0.,0.01);
    RooGaussian KS_sig1("KS_sig1", "Signal", M, KS_mean, KS_sigma1);
    RooGaussian KS_sig2("KS_sig2", "Signal", M, KS_mean, KS_sigma2);

    //Background form
    RooRealVar KS_A("KS_A","",-10000,-20000.,-5000.);
    RooRealVar KS_B("KS_B","", 10000.,  8000., 26000.);
    RooGenericPdf KS_bkg("KS_bkg","@0*@1+@2",RooArgList(M,KS_A,KS_B));


    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar KS_nbkg("KS_nbkg", "",   260000,  0,  800000);
    RooRealVar KS_nsig1("KS_nsig1", "",  50000,  0,  500000);
    RooRealVar KS_nsig2("KS_nsig2", "",  20000,  0,  500000);


    //signal region
    M.setRange("signal", 0.486, 0.515);

    //bkgd region
    M.setRange("background1", 0.47, 0.483);
    M.setRange("background2", 0.515, 0.52);

    RooAddPdf KS_model("KS_model", "bkg+sig1+sig2", RooArgList(KS_bkg,KS_sig1, KS_sig2), RooArgList(KS_nbkg,KS_nsig1,KS_nsig2));
    //RooFitResult *r_KS_bkg = KS_bkg.fitTo(KS_data, Save(kTRUE), Range("background1","background2"));
    //RooFitResult *r_KS_sig = KS_model.fitTo(KS_data, Save(kTRUE), Range("signal"));
    RooFitResult *r_KS_sig = KS_model.fitTo(KS_data, Save(kTRUE));
    KS_model.plotOn(KS_frame, Range("Full"), Components(KS_bkg), LineStyle(kDashed), LineColor(kRed));
    KS_model.plotOn(KS_frame, Range("Full"));
    KS_model.plotOn(KS_frame, Range("KS_Full"), Components("KS_sig1,KS_sig2"), DrawOption("F"),FillColor(kOrange),MoveToBack());

    KS_nsig1.Print();
    KS_nsig2.Print();
    KS_nbkg.Print();
    // Draw the frame on the canvas

    KS_sig1.paramOn(KS_frame, Layout(0.1,0.4,0.9));
    KS_frame->getAttText()->SetTextSize(0.025);
    KS_sig2.paramOn(KS_frame, Layout(0.1,0.4,0.78));
    KS_frame->getAttText()->SetTextSize(0.025);
    //KS_bkg.paramOn(KS_frame, Layout(0.1,0.4,0.66));
    //KS_frame->getAttText()->SetTextSize(0.025);

    TCanvas *delta_M = new TCanvas("delta_M", "delta_M", 1000, 800);
    KS_frame -> Draw();

    auto KS_M0  = KS_mean.getVal();
    auto KS_sM  = KS_sigma1.getVal();
    //M.setRange("KS_3sigma", KS_M0-3*KS_sM, KS_M0+3*KS_sM);
    //M.setRange("KS_lo_sband", 0.47, 0.47+3*KS_sM);
    //M.setRange("KS_hi_sband", 0.52-3*KS_sM, 0.52);
    M.setRange("KS_3sigma", 0.4929,0.5025);
    M.setRange("KS_lo_sband", 0.47, 0.4729);
    M.setRange("KS_hi_sband", 0.5152, 0.52);
    RooAbsReal* KS_sigfrac1 = KS_sig1.createIntegral(M,NormSet(M),Range("KS_3sigma"));
    RooAbsReal* KS_sigfrac2 = KS_sig2.createIntegral(M,NormSet(M),Range("KS_3sigma"));
    RooAbsReal* KS_bkgfrac  = KS_bkg.createIntegral(M,NormSet(M),Range("KS_3sigma"));
    RooAbsReal* KS_lo_frac  = KS_bkg.createIntegral(M,NormSet(M),Range("KS_lo_sband"));
    RooAbsReal* KS_hi_frac  = KS_bkg.createIntegral(M,NormSet(M),Range("KS_hi_sband"));
    cout << "KS  sig frac1: " << KS_sigfrac1 -> getVal()    << endl;
    cout << "KS  sig frac2: " << KS_sigfrac2 -> getVal()    << endl;
    auto KS_bkgfrac0 = KS_bkgfrac -> getVal();
    auto KS_lo_frac0 = KS_lo_frac -> getVal();
    auto KS_hi_frac0 = KS_hi_frac -> getVal();
    auto rescale = KS_bkgfrac0 / (KS_lo_frac0+KS_hi_frac0);
    cout << "KS  bkg frac: "  << KS_bkgfrac0 << endl;
    cout << "sideband frac: " << KS_lo_frac0 + KS_hi_frac0  << endl;
    cout << "rescale: " << rescale << endl;
    cout << endl;

    int KS_nsig0_1 = KS_nsig1.getVal()   * KS_sigfrac1 -> getVal();
    int KS_dnsig_1 = KS_nsig1.getError() * KS_sigfrac1 -> getVal();
    int KS_nsig0_2 = KS_nsig2.getVal()   * KS_sigfrac2 -> getVal();
    int KS_dnsig_2 = KS_nsig2.getError() * KS_sigfrac2 -> getVal();
    int KS_nbkg0 = KS_nbkg.getVal()   * KS_bkgfrac -> getVal();
    int KS_dnbkg = KS_nbkg.getError() * KS_bkgfrac -> getVal();

    cout << "KS sig: " << KS_nsig0_1+KS_nsig0_2 << "+/-" << sqrt(KS_dnsig_1^2+KS_dnsig_2^2) << endl;
    cout << "KS bkg: " << KS_nbkg0 << "+/-" << KS_dnbkg << endl;
    cout << "KS total reco: " << KS_nsig1.getVal()+KS_nsig2.getVal() << "+/-" << sqrt(KS_nsig1.getError()*KS_nsig1.getError()+KS_nsig2.getError()*KS_nsig2.getError()) << endl;

    return;

}//End File
