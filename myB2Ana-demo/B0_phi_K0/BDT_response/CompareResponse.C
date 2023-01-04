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

    TChain     *Data = new TChain("res_BDT");
    Data -> AddFile("Data_resp.root");
    float Data_Mbc;
    float Data_res;
    Data -> SetBranchAddress("Mbc",&Data_Mbc);
    Data -> SetBranchAddress("response",&Data_res);

    TChain     *MC = new TChain("res_BDT");
    MC -> AddFile("FullMC_resp.root");
    float MC_Mbc;
    float MC_res;
    MC -> SetBranchAddress("Mbc",&MC_Mbc);
    MC -> SetBranchAddress("response",&MC_res);

    int Data_entries = Data -> GetEntries();
    int MC_entries = MC -> GetEntries();
    
    TH1F *h1 = new TH1F("h1","",30,-1,1);
    TH1F *h2 = new TH1F("h2","",30,-1,1);
     
    for(int i=0; i<Data_entries; i++)
    {
        Data -> GetEntry(i);
        if(Data_Mbc<5.26||Data_Mbc>5.2)h1->Fill(Data_res);
    }

    for(int i=0; i<MC_entries; i++)
    {
        MC -> GetEntry(i);
        if(MC_Mbc<5.26||MC_Mbc>5.2)h2->Fill(MC_res);
    }

    normalize(h1);
    normalize(h2);
    setStyle(h1,kRed+1,"","BDTG response");
    setStyle(h2,kGreen+2,"","BDTG response");

    gStyle -> SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","",400,300);
    h1 -> Draw();
    h2 -> Draw("same");
    c1 -> SaveAs("Responses.pdf");

}
