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

void GaussFitLooseMatched(  
    int myMode = 0, double minProb = 0.03,
    float upperRange = 5.27, bool lowerLabel = 0,
    bool useArgus= 1, float fixCoef = -999, float fixThres = 5.29,// 
    bool useGaus = 1, float fixMean = -999, float fixSigma = -999,
    int nBins = 120, float lowBound = 5.24){

    TString fInName = Form("decModes/mode_%02d/Total.root",myMode);
    // Variables
    RooRealVar Mbc("Mbc","Mbc",lowBound,5.3);
    RooRealVar decMode("decMode","",0,35);
    RooRealVar SigProb("SigProb","",0,1);

    /* Turns out you have to explicitly require chargedBB for some of these imperfect match flags */
    RooRealVar isCharg("isCharg","",0,1);
    RooRealVar isSignal("isSignal","",0,1);
    RooRealVar isSigAccMiss("isSigAccMiss","",0,1);
    RooRealVar isSigAccMissGamma("isSigAccMissGamma","",0,1);
    RooRealVar isSigAccWrongFSP("isSigAccWrongFSP","",0,1);

    RooDataSet Mbc_data("Mbc_data","",
    RooArgSet(Mbc, SigProb, decMode, isCharg, isSignal, isSigAccMiss, isSigAccMissGamma, isSigAccWrongFSP),
    ImportFromFile(fInName,"B_cand"),
    Cut(Form("decMode==1.0*%d&&SigProb>%.3f&&isCharg&&(isSignal>.5||isSigAccMiss>.5||isSigAccMissGamma>.5||isSigAccWrongFSP>.5)",myMode,minProb))); 

    const int nModes = 36;
    const char *fState[nModes] = {
/*  0 */"#bar{D}^{0}#pi^{+}",
/*  1 */"#bar{D}^{0}#pi^{+}#pi^{0}",
/*  2 */"#bar{D}^{0}#pi^{+}#pi^{0}#pi^{0}",
/*  3 */"#bar{D}^{0}#pi^{+}#pi^{+}#pi^{-}",
/*  4 */"#bar{D}^{0}#pi^{+}#pi^{+}#pi^{-}#pi^{0}",

/*  5 */"#bar{D}^{0}D^{+}",
/*  6 */"#bar{D}^{0}D^{+}K_{S}^{0}",
/*  7 */"#bar{D}^{0*}D^{+}K_{S}^{0}",
/*  8 */"#bar{D}^{0}D^{+*}K_{S}^{0}",
/*  9 */"#bar{D}^{0*}D^{+*}K_{S}^{0}",

/* 10 */"#bar{D}^{0}D^{0}K^{+}",
/* 11 */"#bar{D}^{0*}D^{0}K^{+}",
/* 12 */"#bar{D}^{0}D^{0*}K^{+}",
/* 13 */"#bar{D}^{0*}D^{0*}K^{+}",

/* 14 */"D^{+}_{s}#bar{D^{0}}",

/* 15 */"#bar{D}^{0*}#pi^{+}",
/* 16 */"#bar{D}^{0*}#pi^{+}#pi^{0}",
/* 17 */"#bar{D}^{0*}#pi^{+}#pi^{0}#pi^{0}",
/* 18 */"#bar{D}^{0*}#pi^{+}#pi^{+}#pi^{-}",
/* 19 */"#bar{D}^{0*}#pi^{+}#pi^{+}#pi^{-}#pi^{0}",

/* 20 */"D^{+*}_{s}#bar{D}^{0}",
/* 21 */"D^{+}_{s}#bar{D}^{0*}",

/* 22 */"#bar{D}^{0}K^{+}",

/* 23 */"D^{-}#pi^{+}#pi^{+}",
/* 24 */"D^{-}#pi^{+}#pi^{+}#pi^{0}",

/* 25 */"J/#psi K^{+}",
/* 26 */"J/#psi K^{+}#pi^{+}#pi^{-}",
/* 27 */"J/#psi K^{+}#pi^{0}",
/* 28 */"J/#psi K^{0}_{S}#pi^{+}",

/* 29 */"#Lambda_{c}^{-}p#pi^{+}#pi^{0}",
/* 30 */"#Lambda_{c}^{-}p#pi^{+}#pi^{-}#pi^{+}",

/* 31 */"#bar{D}^{0}p#bar{p}#pi^{+}",
/* 32 */"#bar{D}^{0*}p#bar{p}#pi^{+}",
/* 33 */"D^{+}p#bar{p}#pi^{+}#pi^{-}",
/* 34 */"D^{+*}p#bar{p}#pi^{+}#pi^{-}",

/* 35 */"#Lambda_{c}^{-}p#pi^{+}",
        }; 

    Mbc.setBins(nBins);
    RooPlot *frame = Mbc.frame(Title(Form("[%02d]   %s   SigProb>%.3f looseMatched GaussFit",myMode,fState[myMode],minProb)));


    Mbc_data.plotOn(frame);
    // D e f i n e   s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar *mean;
    if(fixMean  ==-999 && useGaus){
        mean = new RooRealVar("mean", "", 5.28,5.277,5.283);
    }else{
        mean = new RooRealVar("mean", "", fixMean);
    }
    RooRealVar *sigma;
    if(fixSigma ==-999 && useGaus){
        sigma = new RooRealVar("sigma", "", 0.0023,0.,0.2);
    }else{
        sigma = new RooRealVar("sigma", "", fixSigma);
    }
    RooGaussian sig("sig", "", Mbc, *mean, *sigma);

    RooRealVar *thres;
    if(fixThres ==-999){
        thres = new RooRealVar("thres","",5.29,5.28,5.3);
    }else{
        thres = new RooRealVar("thres","",fixThres);
    }
    RooRealVar *coef;
    if(fixCoef  ==-999){
        coef = new RooRealVar("coef","",-40,-1e3,1e3);
    }else{
        coef = new RooRealVar("coef","",fixCoef);
    }
    RooArgusBG bkg("bkg","bkg",Mbc,*thres,*coef);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar nsig("nsig", "", 3e3, 0, 3e9);
    RooRealVar nbkg("nbkg", "", 1e3, 0, 5e9);

    RooAddPdf *model;
    if(useGaus){
        cout<<endl<<"Model = Gauss + Argus"<<endl;
        model = new RooAddPdf("model","sig+bkg",RooArgList(sig,bkg),RooArgList(nsig,nbkg));
    }

    Mbc.setRange("signal", 5.275, 5.285);
    Mbc.setRange("sband1",lowBound,upperRange);
    Mbc.setRange("sband2",5.287,5.29);
    Mbc.setRange("plotRange",lowBound,5.29);

    bkg.fitTo    (Mbc_data, Range("sband1"));
    sig.fitTo    (Mbc_data, Range("signal"));
    model->fitTo (Mbc_data, Range("plotRange"));
    model->plotOn(frame,    Range("plotRange"), Components(bkg), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
    model->plotOn(frame,    Range("plotRange"));
    model->plotOn(frame,    Range("plotRange"), Components(sig), DrawOption("F"), FillColor(kOrange), MoveToBack());

    if(lowerLabel){
        model->paramOn  (frame,    Layout(0.1,0.4,0.4));  
    }else{
        model->paramOn  (frame,    Layout(0.1,0.4,0.9));
    }

    int nParams;
    if          ( useGaus  &&  useArgus ){
        nParams = 2;//nsig, nbkg
        if(fixMean  ==-999)nParams++;//mean
        if(fixSigma ==-999)nParams++;//sigma
        if(fixCoef  ==-999)nParams++;//coef
        if(fixThres ==-999)nParams++;//thres
    }else if    (!useGaus  &&  useArgus ){
        nParams = 0;
        if(fixCoef  ==-999)nParams++;//coef
        if(fixThres ==-999)nParams++;//thres
    }else if    ( useGaus  && !useArgus){
        nParams = 0;
        if(fixMean  ==-999)nParams++;//mean
        if(fixSigma ==-999)nParams++;//sigma
    }
    cout<<nParams<<" fit parameters"<<endl<<endl;

    cout<<"=====----- RESULTS  -----====="<<endl;
    // Print parameters in terminal
    if          ( useGaus  &&  useArgus ){
        nsig.Print();
        nbkg.Print();
        cout<<"mean  = "<<  mean->getValV()<<endl;
        cout<<"sigma = "<< sigma->getValV()<<endl;
        cout<<"coef  = "<<  coef->getValV()<<endl;
        cout<<"thres = "<< thres->getValV()<<endl;
    }else if    (!useGaus  &&  useArgus ){
        cout<<"coef  = "<<  coef->getValV()<<endl;
        cout<<"thres = "<< thres->getValV()<<endl;
    }else if    ( useGaus  && !useArgus){
        cout<<"mean  = "<<  mean->getValV()<<endl;
        cout<<"sigma = "<< sigma->getValV()<<endl;
    }
    double chi2 =frame->chiSquare();
    double ratio, nPeff;
    ratio = (lowBound-thres->getValV())/(lowBound-5.3);
    nPeff = floor(ratio*nBins);
    double chiProb=TMath::Prob(chi2*nBins,nPeff-nParams) * 100.;

    Mbc.setRange("opt", 5.27, 5.29);
    RooAbsReal *sig_frac = sig.createIntegral(Mbc, NormSet(Mbc), Range("opt"));
    RooAbsReal *bkg_frac = bkg.createIntegral(Mbc, NormSet(Mbc), Range("opt"));
    double sig_opt = sig_frac->getValV() * nsig.getValV();
    double bkg_opt = bkg_frac->getValV() * nbkg.getValV();
    double significance = sig_opt /sqrt(sig_opt+bkg_opt);
    double err_per = nsig.getError() * 100. / nsig.getValV();

    // Show plot
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    frame->getAttText()->SetTextSize(.03);
    frame -> Draw();
    TText T;
    T.SetTextFont(42);
    T.SetTextAlign(21);
    //T.DrawTextNDC(.85,.85,Form("signf=%.1f",significance));
    //T.DrawTextNDC(.85,.8,Form("nsig_err =%.2f%%",err_per));
    //T.DrawTextNDC(.85,.85,Form("fit chi2=%.1f",chi2));
    c1 -> SaveAs(Form("looseMatched_plots/Mode_%02d_Gauss_%.3f.pdf",myMode,minProb));

    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    RooPlot *frame2 = Mbc.frame(Title(Form("[%02d]   %s   SigProb>%.3f looseMatched GaussPull",myMode,fState[myMode],minProb)));
    RooHist *hpull = frame->pullHist();
    frame2->addPlotable(hpull, "PL");
    frame2->SetMinimum(-5);
    frame2->SetMaximum( 5);
    frame2 -> Draw();
    c2 -> SaveAs(Form("looseMatched_plots/Mode_%02d_Gauss_%.3f_pull.pdf",myMode,minProb));

    return;

}//End File
