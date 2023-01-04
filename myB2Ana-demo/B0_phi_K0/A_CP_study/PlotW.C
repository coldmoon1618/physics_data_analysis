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
void PlotW() {

    const Int_t n = 7;

    //////////////////////////////////////////////////////////////////////////
    //Show that using midpoint vs average doesn't make much difference
    Double_t r_mea[n] ={0.0371,0.1523,0.3275,0.4878,0.6025,0.7084,0.8940};
    Double_t r_mid[n] ={0.0500,0.1750,0.3750,0.5625,0.6875,0.7875,0.9125};
    Double_t r_avg[n] ={0.0484,0.1710,0.3727,0.5649,0.6885,0.7881,0.9355};
    Double_t r_err[n] ={0.0396,0.0339,0.0260,0.0301,0.0236,0.0253,0.0078};
    Double_t X[2]={0,1};
    Double_t Y[2]={0,1};
    auto line = new TGraph(2,X,Y);
    line -> SetLineStyle(9);
    line -> SetLineWidth(5);
    line -> SetLineColor(kGray);
    auto gr1 = new TGraphErrors(n,r_avg,r_mea,0,r_err);
    gr1->SetLineColor(kBlue);
    gr1->SetLineWidth(2);
    auto gr2 = new TGraphErrors(n,r_mid,r_mea,0,r_err);
    gr2->SetLineColor(kPink);
    gr2->SetLineWidth(2);

    TMultiGraph *mg1 = new TMultiGraph();
    mg1->SetTitle("Red=midPoint Blue=Avg, 3#sigma bar");
    mg1->Add(line);
    mg1->Add(gr1,"AP");//mine avg 
    mg1->Add(gr2,"AP");//mine midPoint
    mg1 -> GetXaxis() -> SetTitle("<r_{FBDT}>");
    mg1 -> GetYaxis() -> SetTitle("r = 1-2w");
    mg1->Draw("a");
    //c1 -> SaveAs("linear.pdf");


    //Fitting
    Double_t w1_val[n] ={0.4819,0.4206,0.3296,0.2562,0.1774,0.1381,0.0458};
    Double_t w1_err[n] ={0.0098,0.0090,0.0079,0.0105,0.0092,0.0114,0.0057};
    //Counting
    /* With response >-.1 cut
    Double_t w2_val[n] ={0.4815,0.4238,0.3363,0.2561,0.1988,0.1458,0.0530};
    Double_t w2_err[n] ={0.0066,0.0056,0.0043,0.0050,0.0039,0.0042,0.0013};
    */
    //Witout response cut, still have deE and isSignal==1
    Double_t w2_val[n] ={0.4794,0.4235,0.3362,0.2552,0.1972,0.1452,0.0531};
    Double_t w2_err[n] ={0.0061,0.0053,0.0041,0.0047,0.0037,0.0039,0.0012};

    for(int i=0; i<n; i++){
        w1_err[i] *=3.0;
        w2_err[i] *=3.0;
    }
    auto c2 = new TCanvas("c2","",500,500);
    Double_t x[2]={0,1};
    Double_t y[2]={.5,0};
    auto line2 = new TGraph(2,x,y);
    line2-> SetLineStyle(9);
    line2-> SetLineWidth(5);
    line2-> SetLineColor(kGray);

    auto gr3 = new TGraphErrors(n,r_avg,w1_val,0,w1_err);
    gr3->SetLineColor(kRed);
    gr3->SetLineWidth(2);
    auto gr4 = new TGraphErrors(n,r_avg,w2_val,0,w2_err);
    gr4->SetLineColor(kBlue);
    gr4->SetLineWidth(2);

    TMultiGraph *mg2 = new TMultiGraph();
    mg2->SetTitle("Red=Fit Blue=Count, 3#sigma bar");
    mg2->Add(line2);
    mg2->Add(gr3,"AP");//mine avg 
    mg2->Add(gr4,"AP");//mine midPoint
    mg2 -> GetXaxis() -> SetTitle("<r_{FBDT}>");
    mg2 -> GetYaxis() -> SetTitle("w");
    mg2->Draw("a");

    //////////////////////////////////////////////////////////////////////////
    // Compare reco efficiency
    Double_t B0_eff[n] ={0.1248,0.1451,0.1830,0.0966,0.1189,0.0753,0.2562};
    Double_t B0bar_eff[n] ={0.1301,0.1470,0.1837,0.1005,0.1168,0.0703,0.2515};
    Double_t B0_eff_err[n]={0.0014,0.0015,0.0017,0.0012,0.0013,0.0010,0.0021};
    Double_t B0bar_eff_err[n]={0.0014,0.0015,0.0017,0.0012,0.0013,0.0010,0.0021};
    for(int i=0; i<n; i++){
        B0_eff_err[i] *=3.0;
        B0bar_eff_err[i] *=3.0;
    }//To draw 3sigma error bar

    auto gr5 = new TGraphErrors(n,r_avg,B0_eff,0,B0_eff_err);
    gr5->SetLineColor(kRed);
    gr5->SetLineWidth(2);
    auto gr6 = new TGraphErrors(n,r_avg,B0bar_eff,0,B0bar_eff_err);
    gr6->SetLineColor(kBlue);
    gr6->SetLineWidth(2);
    auto legend3 = new TLegend(.1,.7,.3,.9);
    legend3 -> AddEntry(gr5,"B0");
    legend3 -> AddEntry(gr6,"B0bar");

    TMultiGraph *mg3 = new TMultiGraph();
    mg3->SetTitle("Red=B0 Blue=B0bar, 3#sigma bar");
    mg3->Add(gr5,"AP"); 
    mg3->Add(gr6,"AP");
    mg3 -> GetXaxis() -> SetTitle("<r_{FBDT}>");
    mg3 -> GetYaxis() -> SetTitle("#epsilon");
    auto c3 = new TCanvas("c3","c3",500,500);
    mg3->Draw("a");
    legend3 -> Draw("P");

    Double_t eff_ratio[n] ={0.9592,0.9871,0.9961,0.9608,1.0183,1.0718,1.0186};
    Double_t eff_ratio_err[n] ={0.0149,0.0144,0.0132,0.0167,0.0163,0.0215,0.0118};
    for(int i=0; i<n; i++){
        eff_ratio_err[i] *=3.0;
    }
    auto gr8 = new TGraphErrors(n,r_avg,eff_ratio,0,eff_ratio_err);
    auto c4 = new TCanvas("c4","c4",500,500);
    gr8 -> SetTitle("B0/B0bar effRatio");
    gr8 -> SetLineWidth(2);
    gr8 -> Draw("AP");
}
