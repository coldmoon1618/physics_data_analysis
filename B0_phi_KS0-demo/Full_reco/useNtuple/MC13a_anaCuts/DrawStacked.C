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

void DrawStacked(bool isTrain) {

    TChain     *B0_cand = new TChain("B0_cand");
    if(isTrain){
    B0_cand -> AddFile("train/mixed.root");
    B0_cand -> AddFile("train/charged.root");
    B0_cand -> AddFile("train/uubar.root");
    B0_cand -> AddFile("train/ddbar.root");
    B0_cand -> AddFile("train/ssbar.root");
    B0_cand -> AddFile("train/ccbar.root");
    }else{
    B0_cand -> AddFile("ana/mixed.root");
    B0_cand -> AddFile("ana/charged.root");
    B0_cand -> AddFile("ana/uubar.root");
    B0_cand -> AddFile("ana/ddbar.root");
    B0_cand -> AddFile("ana/ssbar.root");
    B0_cand -> AddFile("ana/ccbar.root");
    }
    MyB0Cand    *candB  = new MyB0Cand();
    candB   -> setBranchAddresses(B0_cand);
    MyPhiCand   *cBPhi  = new MyPhiCand();
    cBPhi   -> setBranchAddresses(B0_cand, "phi_");
    MyKsCand    *cBKs0  = new MyKsCand();
    cBKs0   -> setBranchAddresses(B0_cand, "Ks_");

    int Entries = B0_cand -> GetEntries();
    cout<<Entries<<" candidates"<<endl;

    int qqbar = 0;
    int BBbar = 0;
    int Mixed = 0;
    int signal = 0;

    TH1F *h4_Mbc = new TH1F("h4_Mbc",   "Continuum",    50,5.2 , 5.3);
    TH1F *h3_Mbc = new TH1F("h3_Mbc",   "B+B- bkg",     50,5.2 , 5.3);
    TH1F *h2_Mbc = new TH1F("h2_Mbc",   "B0B0bar bkg",  50,5.2 , 5.3);
    TH1F *h1_Mbc = new TH1F("h1_Mbc",   "signal evts",  50,5.2 , 5.3);
    
    TH1F *h4_deE = new TH1F("h4_deE",   "Continuum",    80,-.2,.2);
    TH1F *h3_deE = new TH1F("h3_deE",   "B+B- bkg",     80,-.2,.2);
    TH1F *h2_deE = new TH1F("h2_deE",   "B0B0bar bkg",  80,-.2,.2);
    TH1F *h1_deE = new TH1F("h1_deE",   "signal evts",  80,-.2,.2);

    TH1F *h4_mKK = new TH1F("h4_mKK",   "Continuum",    50,1.004,1.034);
    TH1F *h3_mKK = new TH1F("h3_mKK",   "B+B- bkg",     50,1.004,1.034);
    TH1F *h2_mKK = new TH1F("h2_mKK",   "B0B0bar bkg",  50,1.004,1.034);
    TH1F *h1_mKK = new TH1F("h1_mKK",   "signal evts",  50,1.004,1.034);

    TH1F *h4_cos = new TH1F("h4_cos",   "Continuum",    40, -1.,  1.);
    TH1F *h3_cos = new TH1F("h3_cos",   "B+B- bkg",     40, -1.,  1.);
    TH1F *h2_cos = new TH1F("h2_cos",   "B0B0bar bkg",  40, -1.,  1.);
    TH1F *h1_cos = new TH1F("h1_cos",   "signnal evts", 40, -1.,  1.);

    int nCand = 0;
    for(int i=0; i<Entries; i++)
    {
        B0_cand -> GetEntry(i);

        double B_Mbc    = candB->Mbc;
        double B_deE    = candB->deltaE;
        double isBphiK  = candB->isBphiK;
        double isMixed  = candB->isMixed;
        double isBBbar  = candB->isBBbar;

        double B_mKK    = cBPhi->M;
        double phiCo    = cBPhi->cosHel;

        if(isnan(B_Mbc))continue;
        if(isnan(B_deE))continue;
        if(isnan(B_mKK))continue;
        if(isnan(phiCo))continue;
        //if(fabs(B_mKK)<=1||fabs(B_mKK)>=1.05)continue;
        //if(fabs(B_deE)>=0.2 )continue;
        //if(B_Mbc<=5.25||B_Mbc>=5.30)continue;
        //if(phiCo<-1.||phiCo>1.){cout<<"phiCo="<<phiCo<<endl;continue;}
        nCand++;

        if(isBphiK > 0.5){
            h1_Mbc -> Fill(B_Mbc);
            h1_deE -> Fill(B_deE);
            h1_mKK -> Fill(B_mKK);
            h1_cos -> Fill(phiCo);
            signal++;
        }else if(isMixed >0.5){
            h2_Mbc -> Fill(B_Mbc);
            h2_deE -> Fill(B_deE);
            h2_mKK -> Fill(B_mKK);
            h2_cos -> Fill(phiCo);
            Mixed++;
        }else if(isBBbar >0.5){
            h3_Mbc -> Fill(B_Mbc);
            h3_deE -> Fill(B_deE);
            h3_mKK -> Fill(B_mKK);
            h3_cos -> Fill(phiCo);
            BBbar++;
        }else{
            h4_Mbc -> Fill(B_Mbc);
            h4_deE -> Fill(B_deE);
            h4_mKK -> Fill(B_mKK);
            h4_cos -> Fill(phiCo);   
            qqbar++;
        }
    }//candidates

    h4_Mbc->SetFillColor(kRed+2);
    h3_Mbc->SetFillColor(kOrange+7);
    h2_Mbc->SetFillColor(kOrange-3);
    h1_Mbc->SetFillColor(kGreen+2);

    h4_deE->SetFillColor(kRed+2);
    h3_deE->SetFillColor(kOrange+7);
    h2_deE->SetFillColor(kOrange-3);
    h1_deE->SetFillColor(kGreen+2);

    h4_mKK->SetFillColor(kRed+2);
    h3_mKK->SetFillColor(kOrange+7);
    h2_mKK->SetFillColor(kOrange-3);
    h1_mKK->SetFillColor(kGreen+2);

    h4_cos->SetFillColor(kRed+2);
    h3_cos->SetFillColor(kOrange+7);
    h2_cos->SetFillColor(kOrange-3);
    h1_cos->SetFillColor(kGreen+2);

    THStack *hs1_Mbc = new THStack("hs1_Mbc","");
    hs1_Mbc->Add(h4_Mbc);
    hs1_Mbc->Add(h3_Mbc);
    hs1_Mbc->Add(h2_Mbc);
    hs1_Mbc->Add(h1_Mbc);

    THStack *hs1_deE = new THStack("hs1_deE","");
    hs1_deE->Add(h4_deE);
    hs1_deE->Add(h3_deE);
    hs1_deE->Add(h2_deE);
    hs1_deE->Add(h1_deE);

    THStack *hs1_mKK = new THStack("hs1_mKK","");
    hs1_mKK->Add(h4_mKK);
    hs1_mKK->Add(h3_mKK);
    hs1_mKK->Add(h2_mKK);
    hs1_mKK->Add(h1_mKK);

    THStack *hs1_cos = new THStack("hs1_cos","");
    hs1_cos->Add(h4_cos);
    hs1_cos->Add(h3_cos);
    hs1_cos->Add(h2_cos);
    hs1_cos->Add(h1_cos);

    TText T; 
    T.SetTextFont(42); 
    T.SetTextAlign(21);

    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    c1 -> Divide(2,2,.01,.01);

    c1 -> cd(1);
    hs1_Mbc -> Draw();
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
    T.DrawTextNDC(.5,.95,"Mbc (1/ab MC13a BB+udsc)");
    T.DrawTextNDC(.9,.005,"GeV");

    c1 -> cd(2);
    hs1_deE -> Draw();
    T.DrawTextNDC(.5,.95,"deltaE (1/ab MC13a BB+udsc)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c1 -> cd(3);
    hs1_mKK -> Draw();
    T.DrawTextNDC(.5,.95,"mKK (1/ab MC13a BB+udsc)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c1 -> cd(4);
    hs1_cos -> Draw();
    T.DrawTextNDC(.5,.95,"cos (1/ab MC13a BB+udsc)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    
    if(isTrain) c1 -> SaveAs("all_comp_train.pdf");
    else        c1 -> SaveAs("all_comp_ana.pdf");

    THStack *hs2_Mbc = new THStack("hs2_Mbc","");
    hs2_Mbc->Add(h3_Mbc);
    hs2_Mbc->Add(h2_Mbc);
    hs2_Mbc->Add(h1_Mbc);

    THStack *hs2_deE = new THStack("hs2_deE","");
    hs2_deE->Add(h3_deE);
    hs2_deE->Add(h2_deE);
    hs2_deE->Add(h1_deE);

    THStack *hs2_cos = new THStack("hs2_cos","");
    hs2_cos->Add(h3_cos);
    hs2_cos->Add(h2_cos);
    hs2_cos->Add(h1_cos);

    THStack *hs2_mKK = new THStack("hs2_mKK","");
    hs2_mKK->Add(h3_mKK);
    hs2_mKK->Add(h2_mKK);
    hs2_mKK->Add(h1_mKK);

    TCanvas *c2 = new TCanvas("c2","c2",800,600);
    c2 -> Divide(2,2,.01,.01);
    c2 -> cd(1);

    hs2_Mbc -> Draw();
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
    T.DrawTextNDC(.5,.95,"Mbc (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");

    c2 -> cd(2);
    hs2_deE -> Draw();
    T.DrawTextNDC(.5,.95,"deltaE (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c2 -> cd(3);
    hs2_mKK -> Draw();
    T.DrawTextNDC(.5,.95,"mKK (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c2 -> cd(4);
    hs2_cos -> Draw();
    T.DrawTextNDC(.5,.95,"cos (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    
    if(isTrain) c2 -> SaveAs("BB_comp_train.pdf");
    else        c2 -> SaveAs("BB_comp_ana.pdf");

    cout<<"signal "<<signal<<endl;
    cout<<"Mixed "<<Mixed<<endl;
    cout<<"BBbar "<<BBbar<<endl;
    cout<<"qqbar "<<qqbar<<endl;

}
