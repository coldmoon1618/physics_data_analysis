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
void CompareResponse() {

    TChain *MC = new TChain("res_BDT");
    MC -> AddFile("Ana_resp.root");
    float MC_Mbc;
    float MC_res;
    MC -> SetBranchAddress("Mbc",&MC_Mbc);
    MC -> SetBranchAddress("response",&MC_res);

    int MC_entries = MC -> GetEntries();
    
    TH1F *h1 = new TH1F("h1","",30,-1,1);
    TH1F *h2 = new TH1F("h2","",30,-1,1);
     
    for(int i=0; i<MC_entries; i++)
    {
        MC -> GetEntry(i);
        if(MC_Mbc<5.26||MC_Mbc>5.2)h2->Fill(MC_res);
    }

/*
    setStyle(h1,kRed+1);
    setStyle(h2,kGreen+2);

    gStyle -> SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","",500,600);
    h1 -> Draw();
    h2 -> Draw("same");
*/

}
