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
void SaveSingleCand(bool mass3sigma =0, bool MbcDelE=1){
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_phiKs_cutFlow/getNtuple/SigMC_B0_Rec_20k.root");
    TString fOutName("SigMC_20k_B0_candidate.root");
    TFile fOut(fOutName,"recreate");

    TChain      *B0_Rec  = new TChain("B0_Rec");
    B0_Rec  -> AddFile(fileName);
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs0  = new MyKsRec();
    recBKs0  -> setBranchAddresses(B0_Rec, "Ks_");

    TTree       *B0_cand = new TTree("B0_cand","");
    MyB0Cand     *candB  = new MyB0Cand();
    candB   -> branches(B0_cand);
    MyPhiCand    *cBPhi  = new MyPhiCand();
    cBPhi   -> branches(B0_cand, "phi_");
    MyKsCand     *cBKs0  = new MyKsCand();
    cBKs0   -> branches(B0_cand, "Ks_");
    
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
    const double MIN_Mbc = 5.25;
    const double MAX_Mbc = 9999;
    const double min_mKK = 1.004;
    const double max_mKK = 1.034;
    const double min_mKs = 0.483;
    const double max_mKs = 0.513;

    TH1F *h_Mbc_1 = new TH1F("h_Mbc_1","multiplicy 1.03",60,5.26,5.29);
    TH1F *h_deE_1 = new TH1F("h_deE_1","multiplicy 1.03",40, -.2,  .2);

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
        double evt_ch2 = 0;
        double evt_ind = 0;
        bool has_B_ana = false;

    //////////////////////////////////////////////////
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
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid;
            if(mass3sigma)
                 phi_ana = phi_ana && phi_mKK;

            bool Ks0_vtx = recBKs0->chiProb[iEnt]>0;
            bool Ks0_gbk = recBKs0->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recBKs0->M[iEnt]>min_mKs 
                        && recBKs0->M[iEnt]<max_mKs;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk;
            if(mass3sigma)
                 Ks0_ana = Ks0_ana && Ks0_mKs;

            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];
            double B_ch2 = recB->chiProb[iEnt];
            bool B0__vtx = B_ch2 >0;
            bool B0__Mbc = B_Mbc > MIN_Mbc
                    && B_Mbc < MAX_Mbc;
            bool B0__DeE = abs(B_deE)<dE_wid_;
            bool B0__ana = B0__vtx;
            if(MbcDelE)
                 B0__ana = B0__vtx && B0__Mbc && B0__DeE;

            if(phi_ana&&Ks0_ana&&B0__ana) {
                has_B_ana = true;
                if(B_ch2>evt_ch2){
                    evt_ind = iEnt;
                }//save index of wanted candidate
            }//pass cuts
        }//B0_Rec candidates

        if(!has_B_ana)continue;
        for(int iEnt = evt_ind; iEnt<evt_ind+1; iEnt++){
            candB->isSignal = recB->isSignal[iEnt];
            candB->M        = recB->M[iEnt];
            candB->p        = recB->p[iEnt];
            candB->pstar    = recB->pstar[iEnt];
            candB->Mbc      = recB->Mbc[iEnt];
            candB->deltaE   = recB->deltaE[iEnt];

            cBPhi->isSignal = recBPhi->isSignal[iEnt];
            cBPhi->M        = recBPhi->M[iEnt];
            cBPhi->p        = recBPhi->p[iEnt];
            cBPhi->pstar    = recBPhi->pstar[iEnt];

            cBKs0->isSignal = recBKs0->isSignal[iEnt];
            cBKs0->M        = recBKs0->M[iEnt];
            cBKs0->p        = recBKs0->p[iEnt];
            cBKs0->pstar    = recBKs0->pstar[iEnt];

            B0_cand -> Fill();
        }
    }//events
    //////////////////////////////////////////////////

    fOut.cd();
    B0_cand -> Write();
    B0_cand -> Print();
    fOut.Close();

    return;
}
