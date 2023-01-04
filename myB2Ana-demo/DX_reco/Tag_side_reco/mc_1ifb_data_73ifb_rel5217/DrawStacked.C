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

void DrawStacked(double minProb = 0.01) {

    TChain     *B_cand = new TChain("B_cand");
    B_cand -> AddFile("BGx1.root");
    double Mbc;
    B_cand -> SetBranchAddress("Mbc", &Mbc);
    double decMode;
    B_cand -> SetBranchAddress("decMode", &decMode);
    double SigProb;
    B_cand -> SetBranchAddress("SigProb", &SigProb);
    double isSignal;
    B_cand -> SetBranchAddress("isSignal",&isSignal);
    double isCharg;
    B_cand -> SetBranchAddress("isCharg", &isCharg);
    double isMixed;
    B_cand -> SetBranchAddress("isMixed", &isMixed);
    double isQQbar;
    B_cand -> SetBranchAddress("isQQbar", &isQQbar);

    int Entries = B_cand -> GetEntries();

    TH1F *h4_Mbc = new TH1F("h4_Mbc",   "Continuum",    30,5.24, 5.3);
    TH1F *h3_Mbc = new TH1F("h3_Mbc",   "Mixed bkg",    30,5.24, 5.3);
    TH1F *h2_Mbc = new TH1F("h2_Mbc",   "B+B-  bkg",    30,5.24, 5.3);
    TH1F *h1_Mbc = new TH1F("h1_Mbc",   "signal evts",  30,5.24, 5.3);

    int nSignl = 0;
    int nCharg = 0;
    int nMixed = 0;
    int nQQbar = 0;
    for(int i=0; i<Entries; i++)
    {
        B_cand -> GetEntry(i);

        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        if(isSignal > 0.5){
            h1_Mbc -> Fill(Mbc);
            nSignl++;
        }else if(isCharg >0.5){
            h2_Mbc -> Fill(Mbc);
            nCharg++;
        }else if(isMixed >0.5){
            h3_Mbc -> Fill(Mbc);
            nMixed++;
        }else if(isQQbar >0.5){
            h4_Mbc -> Fill(Mbc);
            nQQbar++;
        }
    }//candidates

    h4_Mbc->SetFillColor(kRed+2);
    h3_Mbc->SetFillColor(kOrange+7);
    h2_Mbc->SetFillColor(kOrange-3);
    h1_Mbc->SetFillColor(kGreen+2);

    THStack *hs1_Mbc = new THStack("hs1_Mbc","");
    hs1_Mbc->Add(h4_Mbc);
    hs1_Mbc->Add(h3_Mbc);
    hs1_Mbc->Add(h2_Mbc);
    hs1_Mbc->Add(h1_Mbc);

    TText T; 
    T.SetTextFont(42); 
    T.SetTextAlign(21);

    TCanvas *c1 = new TCanvas("c1","c1",400,300);

    hs1_Mbc -> Draw();
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
    T.DrawTextNDC(.5,.95,"Mbc (1/ab MC13a BB+udsc)");
    T.DrawTextNDC(.9,.005,"GeV");

    c1 -> SaveAs("all_comp.pdf");

    THStack *hs2_Mbc = new THStack("hs2_Mbc","");
    hs2_Mbc->Add(h3_Mbc);
    hs2_Mbc->Add(h2_Mbc);
    hs2_Mbc->Add(h1_Mbc);

    TCanvas *c2 = new TCanvas("c2","c2",400,300);

    hs2_Mbc -> Draw();
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
    T.DrawTextNDC(.5,.95,"Mbc (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");
    c2 -> SaveAs("BB_comp.pdf");

    cout<<"nSignl = "<<nSignl<<endl;
    cout<<"nCharg = "<<nCharg<<endl;
    cout<<"nMixed = "<<nMixed<<endl;
    cout<<"nQQbar = "<<nQQbar<<endl;
}
