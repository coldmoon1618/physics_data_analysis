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
//Makes M ditr of phi and Ks candidates from SigMC output
//Compare all, matched, unmatched
void PlotSigMCPhiKs(){
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");

    TChain      *phi_Rec  = new TChain("phi_Rec");
    phi_Rec  -> AddFile(fileName);
    TChain      *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyPhiRec    *recPhi = new MyPhiRec();
    recPhi -> setBranchAddresses(phi_Rec);
    MyKsRec     *recKs0  = new MyKsRec();
    recKs0  -> setBranchAddresses(Ks_Rec);

    int Entries = phi_Rec -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    TChain      *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC      *mc      = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi   = new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs    = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    TH1F *h1_M_Ks0  = new TH1F("h1_M_Ks0","reconstructed",80,.46,.54);
    TH1F *h2_M_Ks0  = new TH1F("h2_M_Ks0","   matched   ",80,.46,.54);
    TH1F *h3_M_Ks0  = new TH1F("h3_M_Ks0","  unmatched  ",80,.46,.54);
    TH1F *h1_M_phi  = new TH1F("h1_M_phi","reconstructed",80,.98,1.06);
    TH1F *h2_M_phi  = new TH1F("h2_M_phi","   matched   ",80,.98,1.06);
    TH1F *h3_M_phi  = new TH1F("h3_M_phi","  unmatched  ",80,.98,1.06);

    for(int i=0; i < Entries; i++){
        B0_MC   -> GetEntry(i);
        phi_Rec -> GetEntry(i);
        Ks_Rec  -> GetEntry(i);

        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recPhi->nCand; iEnt++){
            bool phi_vtx = recPhi->chiProb[iEnt]>0;
            bool phi_cdc = recPhi->dau_0_thetaInCDCAcceptance[iEnt]==1 
                        && recPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool phi_hit = recPhi->dau_0_nCDCHits[iEnt]>0
                        && recPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recPhi->dau_0_kaonID[iEnt]>0.05
                        && recPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid;
            double phi_M = recPhi->M[iEnt];

            if(!phi_ana)continue;
                h1_M_phi -> Fill(phi_M);
            if(recPhi->isSignal[iEnt]){
                h2_M_phi -> Fill(phi_M);
            }else{
                h3_M_phi -> Fill(phi_M);
            }

        }//B0_Rec candidates

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs0->nCand; iEnt++){
            bool Ks0_vtx = recKs0->chiProb[iEnt]>0;
            bool Ks0_gbk = recKs0->goodBelleKshort[iEnt]==1;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk;
            double Ks0_M = recKs0->M[iEnt];

            if(!Ks0_ana)continue;
                h1_M_Ks0 -> Fill(Ks0_M);
            if(recKs0->isSignal[iEnt]){
                h2_M_Ks0 -> Fill(Ks0_M);
            }else{
                h3_M_Ks0 -> Fill(Ks0_M);
            }

        }//B0_Rec candidates

    }//events
    //////////////////////////////////////////////////
    setStyle(h1_M_phi,kBlue+0,"phi_M");
    setStyle(h1_M_Ks0,kBlue+0,"Ks0_M");

    setStyle(h2_M_phi,kGreen+2,"phi_M");
    setStyle(h2_M_Ks0,kGreen+2,"Ks0_M");

    setStyle(h3_M_phi,kRed+1,"phi_M");
    setStyle(h3_M_Ks0,kRed+1,"Ks0_M");
      
    gStyle -> SetOptStat("nermous");
    TCanvas *c2 = new TCanvas("c2","c2",600, 600);
    c2 -> Divide(2,3,.01,.01);
    c2 -> cd(1);
    h1_M_phi -> Draw();
    c2 -> cd(3);
    h2_M_phi -> Draw();
    c2 -> cd(5);
    h3_M_phi -> Draw();
    c2 -> cd(2);
    h1_M_Ks0 -> Draw();
    c2 -> cd(4);
    h2_M_Ks0 -> Draw();
    c2 -> cd(6);
    h3_M_Ks0 -> Draw();
    c2 -> SaveAs("phi_Ks_M_SigMC.pdf");

    return;
}
