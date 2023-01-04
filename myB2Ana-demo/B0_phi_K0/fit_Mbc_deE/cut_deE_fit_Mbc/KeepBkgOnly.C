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

void KeepBkgOnly(bool cutNonres=0) {

    TChain *input = new TChain("res_BDT");
    input -> AddFile("Ana_resp.root");
    float Mbc;
    float response;
    float isBphiK;
    float isMixed;
    input -> SetBranchAddress("Mbc",        &Mbc);
    input -> SetBranchAddress("response",   &response);
    input -> SetBranchAddress("isBphiK",    &isBphiK);
    input -> SetBranchAddress("isMixed",    &isMixed);

    TTree *res_BDT = new TTree("res_BDT","BDTG response");
    res_BDT -> Branch("Mbc",                &Mbc);
    res_BDT -> Branch("response",           &response);

    int Entries = input -> GetEntries();
    cout << Entries << " entries "<< endl;

    for(int i = 0; i<Entries; i++){
        input -> GetEntry(i);

        if(isBphiK>0.5)continue;
        if(cutNonres&&isMixed)continue;
        res_BDT -> Fill();
    }

    TString filename("");
    if(cutNonres){
        filename+="par_bkg_resp.root";
    }else{
        filename+="all_bkg_resp.root";
    }

    TFile *outFile = new TFile(filename,"RECREATE");
    res_BDT -> Write();

}
