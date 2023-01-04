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

#include "MyB0Cand.h"
#include "MyPhiCand.h"
#include "MyKsCand.h"
#include "MyDecChain.h"
//Get multiplicity and save candidates at different stages
void Multiplicity() {
    TString fileName("SigMC_B0_1.root");

    TChain      *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC      *mc      = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi   = new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs    = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    TChain      *B0_Rec  = new TChain("B0_Rec");
    B0_Rec  -> AddFile(fileName);
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs0  = new MyKsRec();
    recBKs0 -> setBranchAddresses(B0_Rec, "Ks_");

    int Entries = B0_Rec -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    TH1F *h1 = new TH1F("h1","raw reco [Exact signal events]",10,0,10);
    TH1F *h2 = new TH1F("h2","after cuts",10,0,10);

    const double dE_wid_ = 0.025;
    const double MIN_Mbc = 5.27;
    const double MAX_Mbc = 5.29;
    const double min_mKK = 1.004;
    const double max_mKK = 1.034;
    const double min_mKs = 0.483;
    const double max_mKs = 0.513;

    for(int i=0; i < Entries; i++)
    {
        B0_Rec      -> GetEntry(i);
        B0_MC       -> GetEntry(i);

        double hasSigChain = 0.;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = 1.;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;
        //cout<<recB->nCand<<endl;
        int nCandRaw = recB->nCand;
        if(nCandRaw>0)h1->Fill(nCandRaw);
        
        int nCandCut = 0;
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            bool phi_vtx = recBPhi->chiProb[iEnt]>0;
            bool phi_cdc = recBPhi->dau_0_thetaInCDCAcceptance[iEnt]==1
                && recBPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool phi_hit = recBPhi->dau_0_nCDCHits[iEnt]>0
                && recBPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recBPhi->dau_0_kaonID[iEnt]>0.05
                && recBPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_mKK = recBPhi->M[iEnt]>min_mKK
                && recBPhi->M[iEnt]<max_mKK;
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid && phi_mKK;

            bool Ks0_vtx = recBKs0->chiProb[iEnt]>0;
            bool Ks0_gbk = recBKs0->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recBKs0->M[iEnt]>min_mKs
                && recBKs0->M[iEnt]<max_mKs;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk && Ks0_mKs;

            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];
            double B_ch2 = recB->chiProb[iEnt];
            bool B0__vtx = B_ch2 >0;
            bool B0__Mbc = B_Mbc > MIN_Mbc
                && B_Mbc < MAX_Mbc;
            bool B0__DeE = abs(B_deE)<dE_wid_;
            bool B0__ana = B0__vtx && B0__Mbc && B0__DeE;

            if(phi_ana&&Ks0_ana&&B0__ana){
                nCandCut++;
            }//pass cuts
        }//B0_Rec candidates

        if(nCandCut>0)h2->Fill(nCandCut);

    }//events
    //////////////////////////////////////////////////

    gStyle -> SetOptStat("nermous");
    TCanvas *c1 = new TCanvas("c1","",500,800);
    c1 -> Divide(1,2,.01,.01);
    c1 -> cd(1);
    h1 -> Draw();
    c1 -> cd(2);
    h2 -> Draw();

    return;
}
