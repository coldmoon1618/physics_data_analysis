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
/*

Before tighter cuts
uubar candidates:  5,146,735
ddbar candidates:  1,387,274
ssbar candidates:  2,870,167
ccbar candidates: 14,769,884
uubar : 5,105,486
ddbar : 1,376,027
ssbar : 2,855,012
ccbar :14,670,988
Cont_comp_preCut.pdf

after
uubar candidates:   384
ddbar candidates:   129
ssbar candidates: 1,798
ccbar candidates: 2,161
uubar :  384
ddbar :  129
ssbar :1,798
ccbar :2,161
Cont_comp.pdf

*/
void DrawCont() {

    TH1F *h_Mbc[4];
    h_Mbc[3] = new TH1F("h4_Mbc", "ccbar", 50,5.2, 5.3);
    h_Mbc[2] = new TH1F("h3_Mbc", "ssbar", 50,5.2, 5.3);
    h_Mbc[1] = new TH1F("h2_Mbc", "ddbar", 50,5.2, 5.3);
    h_Mbc[0] = new TH1F("h1_Mbc", "uubar", 50,5.2, 5.3);
    
    TH1F *h_deE[4];
    h_deE[3] = new TH1F("h4_deE", "ccbar", 40,-.2 ,.2 );
    h_deE[2] = new TH1F("h3_deE", "ssbar", 40,-.2 ,.2 );
    h_deE[1] = new TH1F("h2_deE", "ddbar", 40,-.2 ,.2 );
    h_deE[0] = new TH1F("h1_deE", "uubar", 40,-.2 ,.2 );

    TH1F *h_mKK[4];
    h_mKK[3] = new TH1F("h4_mKK", "ccbar", 50,1.004,1.034);
    h_mKK[2] = new TH1F("h3_mKK", "ssbar", 50,1.004,1.034);
    h_mKK[1] = new TH1F("h2_mKK", "ddbar", 50,1.004,1.034);
    h_mKK[0] = new TH1F("h1_mKK", "uubar", 50,1.004,1.034);

    TH1F *h_cos[4];
    h_cos[3] = new TH1F("h4_cos", "ccbar", 40, -1.,  1.);
    h_cos[2] = new TH1F("h3_cos", "ssbar", 40, -1.,  1.);
    h_cos[1] = new TH1F("h2_cos", "ddbar", 40, -1.,  1.);
    h_cos[0] = new TH1F("h1_cos", "uubar", 40, -1.,  1.);

    TString filename("Initial_cut_study/");
    //TString filename("/home/belle2/zhai/B0_phi_KS0/evt_to_B0cand/MC13a/");
    const char *typeName[4] = {"uubar","ddbar","ssbar","ccbar"};
    int count[4]={0,0,0,0};
    for(int iType = 0; iType<4; iType++){
        TChain     *B0_cand = new TChain("B0_cand");
        B0_cand -> AddFile(filename+typeName[iType]+".root");
        int Entries = B0_cand -> GetEntries();
        cout<<typeName[iType]<<" candidates: "<<Entries<<endl;

        MyB0Cand    *candB  = new MyB0Cand();
        candB   -> setBranchAddresses(B0_cand);
        MyPhiCand   *cBPhi  = new MyPhiCand();
        cBPhi   -> setBranchAddresses(B0_cand, "phi_");
        MyKsCand    *cBKs0  = new MyKsCand();
        cBKs0   -> setBranchAddresses(B0_cand, "Ks_");

        for(int i=0; i<Entries; i++)
        {
            B0_cand -> GetEntry(i);

            double B_Mbc    = candB->Mbc;
            double B_deE    = candB->deltaE;
            double B_mKK    = cBPhi->M;
            double phiCo    = cBPhi->cosHel;

            if(isnan(B_Mbc))continue;
            if(isnan(B_deE))continue;
            if(isnan(B_mKK))continue;
            if(isnan(phiCo))continue;

            if(B_Mbc<5.2){continue;}

            h_Mbc[iType]->Fill(B_Mbc);
            h_deE[iType]->Fill(B_deE);
            h_mKK[iType]->Fill(B_mKK);
            h_cos[iType]->Fill(phiCo);

            count[iType]++;

        }//candidates
    }//continuum types
    for(int iType = 0; iType<4; iType++)
    {
        cout<<typeName[iType]<<" :"<<count[iType]<<endl;
    }

    h_Mbc[3]->SetFillColor(kRed+2);
    h_Mbc[2]->SetFillColor(kRed-3);
    h_Mbc[1]->SetFillColor(kRed-7);
    h_Mbc[0]->SetFillColor(kRed-9);

    h_deE[3]->SetFillColor(kRed+2);
    h_deE[2]->SetFillColor(kRed-3);
    h_deE[1]->SetFillColor(kRed-7);
    h_deE[0]->SetFillColor(kRed-9);

    h_mKK[3]->SetFillColor(kRed+2);
    h_mKK[2]->SetFillColor(kRed-3);
    h_mKK[1]->SetFillColor(kRed-7);
    h_mKK[0]->SetFillColor(kRed-9);

    h_cos[3]->SetFillColor(kRed+2);
    h_cos[2]->SetFillColor(kRed-3);
    h_cos[1]->SetFillColor(kRed-7);
    h_cos[0]->SetFillColor(kRed-9);

    THStack *hs_Mbc = new THStack("hs_Mbc","");
    hs_Mbc->Add(h_Mbc[3]);
    hs_Mbc->Add(h_Mbc[2]);
    hs_Mbc->Add(h_Mbc[1]);
    hs_Mbc->Add(h_Mbc[0]);

    THStack *hs_deE = new THStack("hs_deE","");
    hs_deE->Add(h_deE[3]);
    hs_deE->Add(h_deE[2]);
    hs_deE->Add(h_deE[1]);
    hs_deE->Add(h_deE[0]);

    THStack *hs_mKK = new THStack("hs_mKK","");
    hs_mKK->Add(h_mKK[3]);
    hs_mKK->Add(h_mKK[2]);
    hs_mKK->Add(h_mKK[1]);
    hs_mKK->Add(h_mKK[0]);

    THStack *hs_cos = new THStack("hs_cos","");
    hs_cos->Add(h_cos[3]);
    hs_cos->Add(h_cos[2]);
    hs_cos->Add(h_cos[1]);
    hs_cos->Add(h_cos[0]);

    TText T; 
    T.SetTextFont(42); 
    T.SetTextAlign(21);

    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    c1 -> Divide(2,2,.01,.01);

    c1 -> cd(1);
    hs_Mbc -> Draw();
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
    T.DrawTextNDC(.5,.95,"Mbc (1/ab MC13a udsc)");
    T.DrawTextNDC(.9,.005,"GeV");

    c1 -> cd(2);
    hs_deE -> Draw();
    T.DrawTextNDC(.5,.95,"deltaE (1/ab MC13a udsc)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c1 -> cd(3);
    hs_mKK -> Draw();
    T.DrawTextNDC(.5,.95,"mKK (1/ab MC13a udsc)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c1 -> cd(4);
    hs_cos -> Draw();
    T.DrawTextNDC(.5,.95,"cos (1/ab MC13a udsc)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    //c1->SaveAs("Cont_comp.pdf");
    c1->SaveAs("Cont_comp_preCut.pdf");

    return ;
}
