#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0MC.h"

#include "MySetStyle.h"

void MakePlot(){
    TString fileName("Bug_report.root");

    TChain *B0_MC    = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    int Entries = B0_MC -> GetEntries();
    cout<<Entries<<" events "<<endl;

    TChain *B0_recMC = new TChain("B0_recMC");
    B0_recMC-> AddFile(fileName);
    MyB0MC recMc;
    recMc.setBranchAddresses(B0_recMC);

    TH1F *h_mc_nCand    = new TH1F("h_mc_nCand","h_mc_nCand",6,0,6);
    TH1F *h_recMc_nCand = new TH1F("h_recMc_nCand","h_recMc_nCand",6,0,6);

    for(int i=0; i < Entries; i++){
        B0_MC   -> GetEntry(i);
        B0_recMC-> GetEntry(i);

        if(mc.nCand!=recMc.nCand)cout<<"event "<<i<<" mc: "<<mc.nCand<<" recMc: "<<recMc.nCand<<endl;
        h_mc_nCand    -> Fill(mc.nCand);
        h_recMc_nCand -> Fill(recMc.nCand);

    }//nTotalEvents

    gStyle -> SetOptStat(0);
    setStyle(h_recMc_nCand, kRed+1,"nCandidates","directly loaded:Green reconMCDec: Red");
    setStyle(h_mc_nCand,  kGreen+2,"nCandidates","generated Ks p");
    TCanvas *c1 = new TCanvas("c1","c1",500,400);
    h_recMc_nCand -> SetMaximum(2000);
    h_recMc_nCand -> Draw();
    h_mc_nCand -> Draw("same");
    c1 -> SaveAs("c1.pdf");
    gPad -> SetLogy();
    c1 -> SaveAs("c1_log.pdf");

    return;
}
