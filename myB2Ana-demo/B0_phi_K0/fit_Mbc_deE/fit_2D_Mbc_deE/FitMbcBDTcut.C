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

void FitMbcBDTcut(int iType = 0, float cut=-9, float fixSigma=-9, float fixCoef = -9, float fixMean = -9, float fixThres = -9){
    //TODO: iType will be used to make cuts on vars like isBphiK, isMixed, isBBbar

    // ------------------------------------------------
    RooRealVar Mbc("Mbc","",5.23,5.3);
    RooRealVar response("response","",-1,1);
    RooRealVar isBBbar("isBBbar",0,1);
    Mbc.Print();
    const char *fIn  [1] = {"FullMC_resp.root"      };
    const char *fOut [1] = {"FullMC_Mbc.pdf"        };
    const char *title[1] = {"M_{bc} (MC13a 1/ab)"   };
    const int  nBins [1] = {35                      };

    RooDataSet Mbc_data("Mbc_data","Mbc_data",RooArgSet(Mbc,response,isBBbar), ImportFromFile(fIn[iType],"res_BDT"),Cut(Form("response>%f&&isBBbar<.5",cut))); 
    Mbc.setBins(nBins[iType]);
    RooPlot *frame = Mbc.frame(Title(title[iType]));

    Mbc.setRange("plotRange",5.23,5.3);
    Mbc_data.plotOn(frame, Range("plotRange"));

    // D e f i n e   s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar mean("mean", "mean of Mbc sig", 5.28,5.277,5.283);
    RooRealVar sigma("sigma", "core width of Mbc sig", 0.0023,0.,0.2);
    RooGaussian sig("sig", "Signal", Mbc, mean, sigma);

    RooRealVar M_B0("M_B0","B0 mass",5.2895,5.289,5.29);
    RooRealVar coef("coef","coef",10,-1e3,1e3);
    RooArgusBG bkg("bkg","bkg",Mbc,M_B0,coef);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar nsig("nsig", "", 3e1, 0, 3e5);
    RooRealVar nbkg("nbkg", "", 1e2, 0, 5e4);
    RooAddPdf model("model","sig+bkg",RooArgList(sig,bkg),RooArgList(nsig,nbkg));

    //signal region
    Mbc.setRange("signal", 5.275, 5.285);
    Mbc.setRange("sband1",5.23,5.275);
    Mbc.setRange("sband2",5.287,5.3);

    bkg.fitTo(Mbc_data,Save(kTRUE),Range("sband1","sband2"));
    sig.fitTo(Mbc_data,Save(kTRUE),Range("signal"));
    model.fitTo(Mbc_data,Save(kTRUE),Range("plotRange"));
    model.plotOn(frame, Range("plotRange"), Components(bkg), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
    model.plotOn(frame, Range("plotRange"));
    model.plotOn(frame, Range("plotRange"), Components(sig), DrawOption("F"), FillColor(kOrange), MoveToBack());

    nsig.Print();

    sig.paramOn(frame, Layout(0.1,0.4,0.5));
    bkg.paramOn(frame, Layout(0.1,0.4,0.38));
    TCanvas *c1 = new TCanvas("c1", "c1", 500, 400);

    RooRealVar empt("empt","",5.23,5.3);
    RooDataSet Empty("Empty","",empt);
    Empty.Draw("hist");

    frame -> Draw();
    cout<<"chi2/ndf="<<frame -> chiSquare(6)<<endl;

    c1 -> SaveAs(fOut[iType]);

    return;

}//End File
