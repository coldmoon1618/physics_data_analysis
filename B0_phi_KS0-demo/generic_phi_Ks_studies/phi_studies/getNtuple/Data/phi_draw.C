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

void phi_draw(){

    TCanvas *c_M = new TCanvas("c_M","c_M",800,500);

    TString fInName("phi_tree.root");

    RooRealVar M("M","M",.98,1.06);

    RooDataSet data("data","",M, ImportFromFile(fInName,"tree_1"));
    RooDataSet cuts("cuts","",M, ImportFromFile(fInName,"tree_2"));

    // Plot 
    M.setBins(200);
    RooPlot *frame = M.frame(Title("#phi from 134/pb of data"));
    data.plotOn(frame, MarkerSize(1));
    cuts.plotOn(frame, MarkerSize(1), MarkerColor(kGreen+2));

    frame -> Draw();
    //c_M -> SaveAs("phiCuts.pdf");
    c_M -> SaveAs("phiBoth.pdf");
    
    return ;
}
