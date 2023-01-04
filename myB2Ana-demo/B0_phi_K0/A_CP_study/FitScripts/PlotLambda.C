#include <iostream>
#include <fstream>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <TMath.h>
#include <TChain.h>

#include "MyB0Cand.h"
#include "MyPhiCand.h"
#include "MyKsCand.h"

#include "MySetStyle.h"
void PlotLambda() {

    const Int_t n = 7;
    const float markerSize = 0.8;

    // All these obtained from basf2 generated ntuple
    // TODO: obtain values directly from mDST and compare
    Double_t r_avg[n] ={0.0484,0.1710,0.3727,0.5649,0.6885,0.7881,0.9355};

    //TruthDeTau_TruthFlavor by rBins
    Double_t tru_abs_val[n] ={0.9836,0.9979,0.9973,0.9812,1.0023,1.0349,1.0098};
    Double_t tru_abs_err[n] ={0.0090,0.0084,0.0074,0.0099,0.0092,0.0121,0.0064};
    Double_t tru_arg_val[n] ={0.6872,0.6983,0.7102,0.6854,0.7138,0.7141,0.7056};
    Double_t tru_arg_err[n] ={0.0106,0.0096,0.0085,0.0118,0.0104,0.0132,0.0072};

    //TruthDeTau_RecoFlavor by rBins, use counting
    Double_t rec_abs_val[n] ={0.8447,0.9975,1.0085,0.9945,1.0050,1.0367,1.0108};
    Double_t rec_abs_err[n] ={0.2018,0.0580,0.0241,0.0214,0.0158,0.0176,0.0072};
    Double_t rec_arg_val[n] ={0.6745,0.7131,0.7168,0.6967,0.7627,0.7176,0.7137};
    Double_t rec_arg_err[n] ={0.3108,0.0777,0.0319,0.0284,0.0201,0.0213,0.0085};

    for(int i=0; i<n; i++){
        tru_abs_err[i] *=3.0;
        rec_abs_err[i] *=3.0;
        tru_arg_err[i] *=3.0;
        rec_arg_err[i] *=3.0;
    }

    Double_t abs_x[2]={0,1};
    Double_t abs_y[2]={1,1};
    Double_t abs_y_lo[2]={ .99, .99};
    Double_t abs_y_up[2]={1.01,1.01};
    auto abs_line = new TGraph(2,abs_x,abs_y);
    abs_line -> SetLineColor(kGray);
    auto abs_line_lo = new TGraph(2,abs_x,abs_y_lo);
    abs_line_lo -> SetLineColor(kGray);
    abs_line_lo -> SetLineStyle(9);
    auto abs_line_up = new TGraph(2,abs_x,abs_y_up);
    abs_line_up -> SetLineColor(kGray);
    abs_line_up -> SetLineStyle(9);

    Double_t arg_x[2]={0,1};
    Double_t arg_y[2]={.703,.703};
    Double_t arg_y_lo[2]={.713,.713};
    Double_t arg_y_up[2]={.693,.693};
    auto arg_line = new TGraph(2,arg_x,arg_y);
    arg_line -> SetLineColor(kGray);
    auto arg_line_lo = new TGraph(2,arg_x,arg_y_lo);
    arg_line_lo -> SetLineColor(kGray);
    arg_line_lo -> SetLineStyle(9);
    auto arg_line_up = new TGraph(2,arg_x,arg_y_up);
    arg_line_up -> SetLineColor(kGray);
    arg_line_up -> SetLineStyle(9);

    auto tru_abs = new TGraphErrors(n,r_avg,tru_abs_val,0,tru_abs_err);
    tru_abs -> SetLineColor(kGreen+2);
    tru_abs -> SetLineWidth(2);
    tru_abs -> SetMarkerStyle(8);
    tru_abs -> SetMarkerSize(markerSize);
    tru_abs -> SetMarkerColor(kGreen+2);
    auto tru_arg = new TGraphErrors(n,r_avg,tru_arg_val,0,tru_arg_err);
    tru_arg -> SetLineColor(kGreen+2);
    tru_arg -> SetLineWidth(2);
    tru_arg -> SetMarkerSize(markerSize);
    tru_arg -> SetMarkerStyle(8);
    tru_arg -> SetMarkerColor(kGreen+2);

    auto rec_abs = new TGraphErrors(n,r_avg,rec_abs_val,0,rec_abs_err);
    rec_abs -> SetLineColor(kRed+1);
    rec_abs -> SetLineWidth(2);
    rec_abs -> SetMarkerSize(markerSize);
    rec_abs -> SetMarkerStyle(8);
    rec_abs -> SetMarkerColor(kRed+1);
    auto rec_arg = new TGraphErrors(n,r_avg,rec_arg_val,0,rec_arg_err);
    rec_arg -> SetLineColor(kRed+1);
    rec_arg -> SetLineWidth(2);
    rec_arg -> SetMarkerSize(markerSize);
    rec_arg -> SetMarkerStyle(8);
    rec_arg -> SetMarkerColor(kRed+1);

    TMultiGraph *abs_mg = new TMultiGraph();
    abs_mg->SetTitle("Fit results 3#sigma bar, Gray=1.00#pm.01");
    abs_mg->Add(abs_line);
    abs_mg->Add(abs_line_lo);
    abs_mg->Add(abs_line_up);
    //abs_mg->Add(rec_abs,"AP");
    abs_mg->Add(tru_abs,"AP"); 
    abs_mg -> GetXaxis() -> SetTitle("<r_{FBDT}>");
    abs_mg -> GetYaxis() -> SetTitle("|#lambda|");

    TMultiGraph *arg_mg = new TMultiGraph();
    arg_mg->SetTitle("Fit results 3#sigma bar, Gray=0.703#pm.01");
    arg_mg->Add(arg_line);
    arg_mg->Add(arg_line_lo);
    arg_mg->Add(arg_line_up);
    //arg_mg->Add(rec_arg,"AP");
    arg_mg->Add(tru_arg,"AP"); 
    arg_mg -> GetXaxis() -> SetTitle("<r_{FBDT}>");
    arg_mg -> GetYaxis() -> SetTitle("arg(#lambda)");

    auto c1 = new TCanvas("c1","c1",666,666);
    c1 -> Divide(1,2,.01,.01);
    c1 -> cd(1);
    abs_mg -> Draw("a");
    c1 -> cd(2);
    arg_mg -> Draw("a");
    c1 -> SaveAs("LambdaValCompare.pdf");

}
