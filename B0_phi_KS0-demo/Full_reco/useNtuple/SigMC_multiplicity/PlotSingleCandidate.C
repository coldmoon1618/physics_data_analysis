#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0Rec.h"
#include "MyPhiRec.h"
#include "MyKsRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
#include "MyCutFlow.h"

#include "MyB0Cand.h"
void PlotSingleCandidate(bool mass3sigma =0, bool MbcDelE=1){
    
    TString fileName("SigMC_20k_B0_candidate.root");

    TChain      *B0_cand  = new TChain("B0_cand");
    B0_cand  -> AddFile(fileName);
    MyB0Cand     *recB    = new MyB0Cand();
    recB    -> setBranchAddresses(B0_cand);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_cand, "phi_");
    MyKsRec     *recBKs  = new MyKsRec();
    recBKs  -> setBranchAddresses(B0_cand, "Ks_");
    int Entries = B0_cand -> GetEntries();
    cout<<Entries<<"  candidates"<<endl;

    TString text("");
    if( mass3sigma &&  MbcDelE)text+="all_cuts";
    if(!mass3sigma &&  MbcDelE)text+="exclude_3sigma";
    if( mass3sigma && !MbcDelE)text+="exclude_Mbc_deltaE";
    if(!mass3sigma && !MbcDelE)text+= "exclude_both";
    TH1F *h_Mbc_1 = new TH1F("h_Mbc_1",text,100,5.25,5.3);
    TH1F *h_deE_1 = new TH1F("h_deE_1",text,40, -.2,  .2);
    TH1F *h_Mbc_2 = new TH1F("h_Mbc_2","matched",100,5.25,5.3);
    TH1F *h_deE_2 = new TH1F("h_deE_2","matched",40, -.2,  .2);
    TH1F *h_Mbc_3 = new TH1F("h_Mbc_3","unmatched",100,5.25,5.3);
    TH1F *h_deE_3 = new TH1F("h_deE_3","unmatched",40, -.2,  .2);

    for(int i=0; i < Entries; i++){
        B0_cand -> GetEntry(i);
        double B_Mbc = recB->Mbc;
        double B_deE = recB->deltaE;

        h_Mbc_1 -> Fill(B_Mbc);
        h_deE_1 -> Fill(B_deE);
        if(recB->isSignal==1){
            h_Mbc_2 -> Fill(B_Mbc);
            h_deE_2 -> Fill(B_deE);
        }else{
            h_Mbc_3 -> Fill(B_Mbc);
            h_deE_3 -> Fill(B_deE);
        }
    }//nTotalEvents

    //////////////////////////////////////////////////
    setStyle(h_Mbc_1,kBlue+0,"Mbc");
    h_Mbc_1 -> SetMaximum(750);
    setStyle(h_deE_1,kBlue+0,"deltaE");
    h_deE_1 -> SetMaximum(3500);

    setStyle(h_Mbc_2,kGreen+2,"Mbc");
    h_Mbc_2 -> SetMaximum(750);
    setStyle(h_deE_2,kGreen+2,"deltaE");
    h_deE_2 -> SetMaximum(3500);

    setStyle(h_Mbc_3,kRed+1,"Mbc");
    h_Mbc_3 -> SetMaximum(20);
    setStyle(h_deE_3,kRed+1,"deltaE");
    h_deE_3 -> SetMaximum(20);

    gStyle -> SetOptStat("nermous");
    TCanvas *c2 = new TCanvas("c2","c2",600, 600);
    c2 -> Divide(2,3,.01,.01);
    c2 -> cd(1);
    h_Mbc_1 -> Draw();
    c2 -> cd(3);
    h_Mbc_2 -> Draw();
    c2 -> cd(5);
    h_Mbc_3 -> Draw();
    c2 -> cd(2);
    h_deE_1 -> Draw();
    c2 -> cd(4);
    h_deE_2 -> Draw();
    c2 -> cd(6);
    h_deE_3 -> Draw();
    c2 -> SaveAs(text+".pdf");

    return;
}
