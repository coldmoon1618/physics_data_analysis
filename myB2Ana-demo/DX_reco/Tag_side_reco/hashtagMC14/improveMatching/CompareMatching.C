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

void CompareMatching(TString fInName = "../AllModes_MC.root", double minProb=.03){

    TChain *tree = new TChain("tree");
    tree -> AddFile(fInName);
    double Mbc;
    tree -> SetBranchAddress("Mbc", &Mbc);
    double DeE;
    tree -> SetBranchAddress("deltaE", &DeE);
    double subMode;
    tree -> SetBranchAddress("subMode", &subMode);
    double SigProb;
    tree -> SetBranchAddress("SigProb", &SigProb);
    int mySignal;
    tree -> SetBranchAddress("mySignal", &mySignal);
    int isSignal;
    tree -> SetBranchAddress("isSignal", &isSignal);
    int accMiss;
    tree -> SetBranchAddress("accMiss", &accMiss);
    int accMissGamma;
    tree -> SetBranchAddress("accMissGamma", &accMissGamma);
    int accWrongFSP;
    tree -> SetBranchAddress("accWrongFSP", &accWrongFSP);
    int iMcType;
    tree -> SetBranchAddress("iMcType", &iMcType);
    int ithDir;
    tree -> SetBranchAddress("ithDir", &ithDir);
    int ithFile;
    tree -> SetBranchAddress("ithFile", &ithFile);
    int evtNum;
    tree -> SetBranchAddress("evtNum", &evtNum);

    int Entries = tree -> GetEntries();

    //TH1F *hDeE_b2notMine    = new TH1F("hDeE_b2notMine","",   50,-0.5,0.5);

    TH1F *hMbc_mySignal     = new TH1F("hMbc_mySignal","",      30,5.24,5.3);
    TH1F *hMbc_notMySignal  = new TH1F("hMbc_notMySignal","",   30,5.24,5.3);
    TH1F *hMbc_isLooseMatch = new TH1F("hMbc_isLooseMatch","",  30,5.24,5.3);
    TH1F *hMbc_notLooseMatch= new TH1F("hMbc_notLooseMatch","", 30,5.24,5.3);
    
    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    for(int i=0; i<Entries; i++){
        tree -> GetEntry(i);
        if(subMode!=13)continue;

        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        if(iMcType!=2)continue;
        
        if(mySignal>.5){
            hMbc_mySignal       -> Fill(Mbc);
        }else{
            hMbc_notMySignal    -> Fill(Mbc);
        }

        if(isSignal>.5||accMiss>.5||accMissGamma>.5||accWrongFSP>.5){
            hMbc_isLooseMatch   -> Fill(Mbc);
        }else{
            hMbc_notLooseMatch  -> Fill(Mbc);
        }
        
    }
    cout<<cnt1+cnt2+cnt3<<" accounted for "<<endl;

    TCanvas *c1_Mbc = new TCanvas("c1_Mbc","c1_Mbc",800,600);
    c1_Mbc -> Divide(2,2,.01,.01);
    c1_Mbc -> cd(1);
    hMbc_isLooseMatch   -> SetTitle("looseMatched");
    hMbc_isLooseMatch   -> GetXaxis() -> SetTitle("Mbc");
    hMbc_isLooseMatch   -> SetLineColor(kRed+1);
    hMbc_isLooseMatch   -> Draw();
    c1_Mbc -> cd(3);
    hMbc_notLooseMatch  -> SetTitle("failed looseMatch");
    hMbc_notLooseMatch  -> GetXaxis() -> SetTitle("Mbc");
    hMbc_notLooseMatch  -> SetLineColor(kRed+1);
    hMbc_notLooseMatch  -> Draw();
    c1_Mbc -> cd(2);
    hMbc_mySignal   -> SetTitle("myMatched");
    hMbc_mySignal   -> GetXaxis() -> SetTitle("Mbc");
    hMbc_mySignal   -> SetLineColor(kGreen+2);
    hMbc_mySignal   -> Draw();
    c1_Mbc -> cd(4);
    hMbc_notMySignal  -> SetTitle("failed myMatch");
    hMbc_notMySignal  -> GetXaxis() -> SetTitle("Mbc");
    hMbc_notMySignal  -> SetLineColor(kGreen+2);
    hMbc_notMySignal  -> Draw();
    c1_Mbc -> SaveAs("compareMatching.pdf");

    return;
}
