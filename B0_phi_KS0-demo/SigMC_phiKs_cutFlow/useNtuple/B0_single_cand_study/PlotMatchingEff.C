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
//Using random single candidates vs the matched ones to estimate matching eff
void PlotMatchingEff(){
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");

    TChain      *B0_Rec  = new TChain("B0_Rec");
    B0_Rec  -> AddFile(fileName);
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs  = new MyKsRec();
    recBKs  -> setBranchAddresses(B0_Rec, "Ks_");
    
    int Entries = B0_Rec -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    TChain      *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC      *mc      = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi   = new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs    = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    const double dE_wid_ = 0.1;
    const double MIN_Mbc = 5.27;
    const double MAX_Mbc = 5.29;
    const double min_mKK = 1.004;
    const double max_mKK = 1.034;
    const double min_mKs = 0.483;
    const double max_mKs = 0.513;

    bool makeLog = false;

    // With multiple candidate
    TH1F *h_Mbc_1 = new TH1F("h_Mbc_1","multiplicy 1.03",60,5.26,5.29);
    TH1F *h_deE_1 = new TH1F("h_deE_1","multiplicy 1.03",40, -.2,  .2);
    // Choose random single candidate
    TH1F *h_Mbc_2 = new TH1F("h_Mbc_2","random single cand",60,5.26,5.29);
    TH1F *h_deE_2 = new TH1F("h_deE_2","random single cand",40, -.2,  .2);
    // Save only matched candidate
    TH1F *h_Mbc_3 = new TH1F("h_Mbc_3","matched",60,5.26,5.29);
    TH1F *h_deE_3 = new TH1F("h_deE_3","matched",40, -.2,  .2);

    for(int i=0; i < Entries; i++){
        B0_Rec -> GetEntry(i);
        B0_MC  -> GetEntry(i);

        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain

        if(!hasSigChain)continue;
        double evt_Mbc = 0;
        double evt_deE = 0;
        double evt_ch2 = 0;
        bool has_B_ana = false;
        double evt_Mbc_m = 0;
        double evt_deE_m = 0;
        double evt_ch2_m = 0;

        double evt_cand = 0;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<1; iEnt++){
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

            bool Ks0_vtx = recBKs->chiProb[iEnt]>0;
            bool Ks0_gbk = recBKs->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recBKs->M[iEnt]>min_mKs 
                        && recBKs->M[iEnt]<max_mKs;
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
            h_Mbc_2 -> Fill(B_Mbc);
            h_deE_2 -> Fill(B_deE);
            if(recBPhi -> isSignal[iEnt]!=1)continue;
            h_Mbc_3 -> Fill(B_Mbc);
            h_deE_3 -> Fill(B_deE);
            }
        }
    }//nTotalEvents

    //////////////////////////////////////////////////
    setStyle(h_Mbc_1,kBlue+0,"Mbc");
    setStyle(h_deE_1,kBlue+0,"deltaE");

    setStyle(h_Mbc_2,kBlue+0,"Mbc");
    setStyle(h_deE_2,kBlue+0,"deltaE");

    setStyle(h_Mbc_3,kRed+1,"Mbc");
    setStyle(h_deE_3,kRed+1,"deltaE");

    //h_Mbc_2 -> Add(h_Mbc_1,-1);
    //h_deE_2 -> Add(h_deE_1,-1);

    gStyle -> SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",600, 300);
    c1 -> Divide(2,1,.01,.01);
    c1 -> cd(1);
    h_Mbc_2 -> Draw();
    h_Mbc_3 -> Draw("same");
    c1 -> cd(2);
    h_deE_2 -> Draw();
    h_deE_3 -> Draw("same");
    c1 -> SaveAs("matching_eff_same.pdf");

    gStyle -> SetOptStat("nermous");
    TCanvas *c2 = new TCanvas("c2","c2",600, 600);
    c2 -> Divide(2,2,.01,.01);
    c2 -> cd(1);
    h_Mbc_2 -> Draw();
    c2 -> cd(3);
    h_Mbc_3 -> Draw();
    c2 -> cd(2);
    h_deE_2 -> Draw();
    c2 -> cd(4);
    h_deE_3 -> Draw();
    c2 -> SaveAs("matching_eff_sep.pdf");

    return;
}
