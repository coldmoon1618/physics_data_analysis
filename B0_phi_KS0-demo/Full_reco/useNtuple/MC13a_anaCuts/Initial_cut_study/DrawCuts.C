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

//Making bkg component study on BBbar files only
/*
 * All have if(isnan(var))continue;
 * CosHel always confined in -1<cos<1
BigRange_noCut.pdf      phi ana cuts, Ks ana cuts + 3sigma range
signal 666
Mixed 13,069,805
BBbar 12,551,602
BigRange_rangeCut.pdf   4.3<Mbc<5.3,  -4.<deltaE<4.
signal 648
Mixed 13,011,038
BBbar 12,497,925
BigRange_MbcCut.pdf     5.25<Mbc<5.3, -4.<deltaE<4.
signal 538
Mixed 3,288,511
BBbar 3,101,320
BigRange_deECut.pdf     4.3<Mbc<5.3,  -.2<deltaE<.2
signal 538
Mixed 3,288,511
BBbar 3,101,320
BigRange_anaCut         5.25<Mbc<5.5< -.2<deltaE<.2
signal 530
Mixed 130
BBbar 60

Diferent drawing range with the same cuts:
RegRange_deECut.pdf
RegRange_anaCut.pdf

*/
void DrawCuts() {

    TChain     *B0_cand = new TChain("B0_cand");
    B0_cand -> AddFile("mixed.root");
    B0_cand -> AddFile("charged.root");
    MyB0Cand    *candB  = new MyB0Cand();
    candB   -> setBranchAddresses(B0_cand);
    MyPhiCand   *cBPhi  = new MyPhiCand();
    cBPhi   -> setBranchAddresses(B0_cand, "phi_");
    MyKsCand    *cBKs0  = new MyKsCand();
    cBKs0   -> setBranchAddresses(B0_cand, "Ks_");

    int Entries = B0_cand -> GetEntries();
    cout<<Entries<<" candidates"<<endl;

    int BBbar = 0;
    int Mixed = 0;
    int signal = 0;

    TH1F *h3_Mbc = new TH1F("h3_Mbc",   "B+B- bkg",     100, 5.25, 5.3);
    TH1F *h2_Mbc = new TH1F("h2_Mbc",   "B0B0bar bkg",  100, 5.25, 5.3);
    TH1F *h1_Mbc = new TH1F("h1_Mbc",   "signal evts",  100, 5.25, 5.3);
    
    TH1F *h3_deE = new TH1F("h3_deE",   "B+B- bkg",     100, -.2, .2);
    TH1F *h2_deE = new TH1F("h2_deE",   "B0B0bar bkg",  100, -.2, .2);
    TH1F *h1_deE = new TH1F("h1_deE",   "signal evts",  100, -.2, .2);

    for(int i=0; i<Entries; i++)
    {
        B0_cand -> GetEntry(i);

        double B_Mbc    = candB->Mbc;
        double B_deE    = candB->deltaE;
        double isBphiK  = candB->isBphiK;
        double isMixed  = candB->isMixed;
        double isBBbar  = candB->isBBbar;

        double B_mKK    = cBPhi->M;

        if(isnan(B_Mbc))continue;
        if(isnan(B_deE))continue;
        if(isnan(B_mKK))continue;
        if(B_Mbc<5.25||B_Mbc>5.3)continue;
        if(B_deE<-.2||B_deE>.2)continue;

        if(isBphiK > 0.5){
            h1_Mbc -> Fill(B_Mbc);
            h1_deE -> Fill(B_deE);
            signal++;
        }else if(isMixed >0.5){
            h2_Mbc -> Fill(B_Mbc);
            h2_deE -> Fill(B_deE);
            Mixed++;
        }else if(isBBbar >0.5){
            h3_Mbc -> Fill(B_Mbc);
            h3_deE -> Fill(B_deE);
            BBbar++;
        }
    }//candidates

    h3_Mbc->SetFillColor(kOrange+7);
    h2_Mbc->SetFillColor(kOrange-3);
    h1_Mbc->SetFillColor(kGreen+2);

    h3_deE->SetFillColor(kOrange+7);
    h2_deE->SetFillColor(kOrange-3);
    h1_deE->SetFillColor(kGreen+2);

    TText T; 
    T.SetTextFont(42); 
    T.SetTextAlign(21);

    THStack *hs2_Mbc = new THStack("hs2_Mbc","");
    hs2_Mbc->Add(h3_Mbc);
    hs2_Mbc->Add(h2_Mbc);
    hs2_Mbc->Add(h1_Mbc);

    THStack *hs2_deE = new THStack("hs2_deE","");
    hs2_deE->Add(h3_deE);
    hs2_deE->Add(h2_deE);
    hs2_deE->Add(h1_deE);

    TCanvas *c2 = new TCanvas("c2","c2",800,300);
    c2 -> Divide(2,1,.01,.01);
    c2 -> cd(1);
    hs2_Mbc -> Draw();
    T.DrawTextNDC(.5,.95,"Mbc (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");

    c2 -> cd(2);
    hs2_deE -> Draw();
    T.DrawTextNDC(.5,.95,"deltaE (1/ab MC13a BB)");
    T.DrawTextNDC(.9,.005,"GeV");
    gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

    c2 -> SaveAs("RegRange_anaCut.pdf");

    cout<<"signal "<<signal<<endl;
    cout<<"Mixed "<<Mixed<<endl;
    cout<<"BBbar "<<BBbar<<endl;

}
