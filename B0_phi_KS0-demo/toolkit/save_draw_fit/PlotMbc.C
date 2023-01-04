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
void PlotMbc() {

    TChain     *B0_cand = new TChain("B0_cand");
    B0_cand -> AddFile("Data_cand.root");
    MyB0Cand    *candB  = new MyB0Cand();
    candB   -> setBranchAddresses(B0_cand);
    MyPhiCand   *cBPhi  = new MyPhiCand();
    cBPhi   -> setBranchAddresses(B0_cand, "phi_");
    MyKsCand    *cBKs0  = new MyKsCand();
    cBKs0   -> setBranchAddresses(B0_cand, "Ks_");

    int Entries = B0_cand -> GetEntries();
    cout<<Entries<<" candidates"<<endl;

    TH1F *h1_Mbc = new TH1F("h1_Mbc", "Mbc ~68/fb Data", 18, 5.2 , 5.3);
    
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
        if(fabs(B_deE)>=0.02 )continue;
        nCand++;

        h1_Mbc -> Fill(B_Mbc);
    }//candidates

    gStyle -> SetOptStat(0);
    TCanvas *c2 = new TCanvas("c2","c2",400,300);
    setStyle(h1_Mbc, kBlack, "GeV");
    h1_Mbc -> Draw();
    c2->SaveAs("Data_Mbc.pdf");

}
