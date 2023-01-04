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

#include "LatexDecModes.h"
#include "LatexSubModes.h"

void PassAFailB(TString fInName = "../AllModes_MC.root", double minProb=.5){

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

    TH1F *hDeE_b2notMine    = new TH1F("hDeE_b2notMine","",   50,-0.5,0.5);
    TH1F *hDeE_MineNotB2    = new TH1F("hDeE_MineNotB2","",   50,-0.5,0.5);
    TH1F *hDeE_looseNotMine = new TH1F("hDeE_looseNotMine","",50,-0.5,0.5);
    TH1F *hDeE_MineNotLoose = new TH1F("hDeE_MineNotLoose","",50,-0.5,0.5);

    TH1F *hDeE_accMissGamma = new TH1F("hDeE_accMissGamma","",50,-0.5,0.5);
    TH1F *hDeE_missNotGamma = new TH1F("hDeE_missNotGamma","",50,-0.5,0.5);
    TH1F *hDeE_wrongFSP     = new TH1F("hDeE_wrongFSP","",    50,-0.5,0.5);

    TH1F *hMbc_b2notMine    = new TH1F("hMbc_b2notMine","",   30,5.24,5.3);
    TH1F *hMbc_MineNotB2    = new TH1F("hMbc_MineNotB2","",   30,5.24,5.3);
    TH1F *hMbc_looseNotMine = new TH1F("hMbc_looseNotMine","",30,5.24,5.3);
    TH1F *hMbc_MineNotLoose = new TH1F("hMbc_MineNotLoose","",30,5.24,5.3);

    TH1F *hMbc_accMissGamma = new TH1F("hMbc_accMissGamma","",30,5.24,5.3);
    TH1F *hMbc_missNotGamma = new TH1F("hMbc_missNotGamma","",30,5.24,5.3);
    TH1F *hMbc_wrongFSP     = new TH1F("hMbc_wrongFSP","",    30,5.24,5.3);

    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    for(int i=0; i<Entries; i++){
        tree -> GetEntry(i);

        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        if(iMcType!=2)continue;
        
        if(isSignal>.5&&mySignal<.5){
            hMbc_b2notMine->Fill(Mbc);
            hDeE_b2notMine->Fill(DeE);
        }
        if(mySignal>.5&&isSignal<.5){
            hMbc_MineNotB2->Fill(Mbc);
            hDeE_MineNotB2->Fill(DeE);
        }
        if((isSignal>.5||accMiss>.5||accMissGamma>.5||accWrongFSP>.5)&&mySignal<.5){
            hMbc_looseNotMine->Fill(Mbc);
            hDeE_looseNotMine->Fill(DeE);
            if(accMissGamma>.5){
                hMbc_accMissGamma->Fill(Mbc);
                hDeE_accMissGamma->Fill(DeE);
                cnt1++;
            }else if(accMiss>.5){
                hMbc_missNotGamma->Fill(Mbc);
                hDeE_missNotGamma->Fill(DeE);
                cnt2++;
                cout<<ithDir<<"thDir "<<ithFile<<"thFile evtNum="<<evtNum<<endl;
            }else if(accWrongFSP){
                hMbc_wrongFSP->Fill(Mbc);
                hDeE_wrongFSP->Fill(DeE);
                cnt3++;
            }
        }
        if(mySignal>.5&&(isSignal<.5&&accMiss<.5&&accMissGamma<.5&&accWrongFSP<.5)){
            hMbc_MineNotLoose->Fill(Mbc);
            hDeE_MineNotLoose->Fill(DeE);
        }
        
    }
    cout<<cnt1+cnt2+cnt3<<" accounted for "<<endl;

    TCanvas *c1_Mbc = new TCanvas("c1_Mbc","c1_Mbc",800,600);
    c1_Mbc -> Divide(2,2,.01,.01);
    c1_Mbc -> cd(1);
    hMbc_b2notMine      -> SetTitle("isSignal>.5 but mySignal<.5");
    hMbc_b2notMine      -> GetXaxis() -> SetTitle("Mbc");
    hMbc_b2notMine      -> Draw();
    c1_Mbc -> cd(2);
    hMbc_MineNotB2      -> SetTitle("mySignal>.5 but isSignal<.5");
    hMbc_MineNotB2      -> GetXaxis() -> SetTitle("Mbc");
    hMbc_MineNotB2      -> Draw();
    c1_Mbc -> cd(3);
    hMbc_looseNotMine   -> SetTitle("looseMat>.5 but mySignal<.5");
    hMbc_looseNotMine   -> GetXaxis() -> SetTitle("Mbc");
    hMbc_looseNotMine   -> Draw();
    c1_Mbc -> cd(4);
    hMbc_MineNotLoose   -> SetTitle("mySignal>.5 but looseMat<.5");
    hMbc_MineNotLoose   -> GetXaxis() -> SetTitle("Mbc");
    hMbc_MineNotLoose   -> Draw();
    c1_Mbc -> SaveAs("AnotB_Mbc.pdf");

    TCanvas *c2_Mbc = new TCanvas("c2_Mbc","c2_Mbc",800,600);
    c2_Mbc -> Divide(2,2,.01,.01);
    c2_Mbc -> cd(1);
    //hMbc_looseNotMine -> SetTitle("looseMat>.5 but mySignal<.5");
    hMbc_looseNotMine   -> Draw();
    c2_Mbc -> cd(2);
    hMbc_accMissGamma   -> SetTitle("accMissGamma but mySignal<.5");
    hMbc_accMissGamma   -> GetXaxis() -> SetTitle("Mbc");
    hMbc_accMissGamma   -> Draw();
    c2_Mbc -> cd(3);
    hMbc_missNotGamma   -> SetTitle("accMiss(other) but mySignal<.5");
    hMbc_missNotGamma   -> GetXaxis() -> SetTitle("Mbc");
    hMbc_missNotGamma   -> Draw();
    c2_Mbc -> cd(4);
    hMbc_wrongFSP       -> SetTitle("accWrongFSP but mySignal<.5");
    hMbc_wrongFSP       -> GetXaxis() -> SetTitle("Mbc");
    hMbc_wrongFSP       -> Draw();

    c2_Mbc -> SaveAs("AnotBbreakdown_Mbc.pdf");

    gStyle -> SetOptStat("nermous");

    TCanvas *c1_DeE = new TCanvas("c1_DeE","c1_DeE",800,600);
    c1_DeE -> Divide(2,2,.01,.01);
    c1_DeE -> cd(1);
    hDeE_b2notMine      -> SetTitle("isSignal>.5 but mySignal<.5");
    hDeE_b2notMine      -> GetXaxis() -> SetTitle("DeE");
    hDeE_b2notMine      -> Draw();
    c1_DeE -> cd(2);
    hDeE_MineNotB2      -> SetTitle("mySignal>.5 but isSignal<.5");
    hDeE_MineNotB2      -> GetXaxis() -> SetTitle("DeE");
    hDeE_MineNotB2      -> Draw();
    c1_DeE -> cd(3);
    hDeE_looseNotMine   -> SetTitle("looseMat>.5 but mySignal<.5");
    hDeE_looseNotMine   -> GetXaxis() -> SetTitle("DeE");
    hDeE_looseNotMine   -> Draw();
    c1_DeE -> cd(4);
    hDeE_MineNotLoose   -> SetTitle("mySignal>.5 but looseMat<.5");
    hDeE_MineNotLoose   -> GetXaxis() -> SetTitle("DeE");
    hDeE_MineNotLoose   -> Draw();
    c1_DeE -> SaveAs("AnotB_DeE.pdf");

    TCanvas *c2_DeE = new TCanvas("c2_DeE","c2_DeE",800,600);
    c2_DeE -> Divide(2,2,.01,.01);
    c2_DeE -> cd(1);
    //hDeE_looseNotMine -> SetTitle("looseMat>.5 but mySignal<.5");
    hDeE_looseNotMine   -> Draw();
    c2_DeE -> cd(2);
    hDeE_accMissGamma   -> SetTitle("accMissGamma but mySignal<.5");
    hDeE_accMissGamma   -> GetXaxis() -> SetTitle("DeE");
    hDeE_accMissGamma   -> Draw();
    c2_DeE -> cd(3);
    hDeE_missNotGamma   -> SetTitle("accMiss(other) but mySignal<.5");
    hDeE_missNotGamma   -> GetXaxis() -> SetTitle("DeE");
    hDeE_missNotGamma   -> Draw();
    c2_DeE -> cd(4);
    hDeE_wrongFSP       -> SetTitle("accWrongFSP but mySignal<.5");
    hDeE_wrongFSP       -> GetXaxis() -> SetTitle("DeE");
    hDeE_wrongFSP       -> Draw();

    c2_DeE -> SaveAs("AnotBbreakdown_DeE.pdf");

    return;
}
