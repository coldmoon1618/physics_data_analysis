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

void plotRaw(){
    //observables
    RooRealVar M("M","M",0.45,0.55);
    M.Print();
    //data sets
    RooDataSet KS_data("KS_data","MC data",M, ImportFromFile("MC13a_KS_compareDistance.root","tree")); 
    M.setBins(100);
    RooPlot *KS_frame = M.frame(Title("m_{K^0_S}"));
    KS_data.plotOn(KS_frame);
    TCanvas *delta_M = new TCanvas("delta_M", "delta_M", 1000, 800);
    KS_frame -> Draw();

    return;

}//End File
