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
void Apr21CutFlow(){
    
    //TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_phiKs_cutFlow/getNtuple/SigMC_B0_noUpdate.root");
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_phiKs_cutFlow/getNtuple/EarlySelections/SigMC_B0_Rec_20k.root");

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

    TChain      *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile(fileName);
    MyPhiRec    *recPhi  = new MyPhiRec();
    recPhi  -> setBranchAddresses(phi_Rec);
    
    TChain      *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec     *recKs   = new MyKsRec;
    recKs   -> setBranchAddresses(Ks_Rec);

    TChain      *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC      *mc      = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi   = new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs    = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    int total_exact_evt = 0;
    int has_mKK_phi_ind = 0;
    int has_mKK_phi_acc = 0;
    int has_mKs_Ks0_ind = 0;
    int has_mKs_Ks0_acc = 0;
    int has_B0__deE_ind = 0;
    int has_B0__deE_acc = 0;
    int has_B0__Mbc_ind = 0;
    int has_B0__Mbc_acc = 0;

    const double dE_wid_ = 0.02;
    const double min_Mbc = 5.2;
    const double min_mKK = 1.004;
    const double max_mKK = 1.034;
    const double min_mKs = 0.483;
    const double max_mKs = 0.513;

    for(int i=0; i < Entries; i++){
        B0_Rec -> GetEntry(i);
        Ks_Rec -> GetEntry(i);
        phi_Rec-> GetEntry(i);
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
        total_exact_evt++;
        bool has_mKKPhi = false;
        bool has_mKsKs0 = false;
        bool has_B0_deE = false;
        bool has_B0_Mbc = false;

    //////////////////////////////////////////////////
   /* 
        for(int iEnt = 0; iEnt<recPhi->nCand; iEnt++){
            bool phi_vtx = recPhi->chiProb[iEnt]>0;
            bool phi_cdc = recPhi->dau_0_thetaInCDCAcceptance[iEnt]==1 
                    && recPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool phi_hit = recPhi->dau_0_nCDCHits[iEnt]>0
                    && recPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recPhi->dau_0_kaonID[iEnt]>0.05
                    && recPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_mKK = recPhi->M[iEnt]>min_mKK
                    && recPhi->M[iEnt]<max_mKK;
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid && phi_mKK;

            if(phi_ana)has_mKKPhi = true;
        }//phi_Rec candidates
        for(int iEnt = 0; iEnt<recKs->nCand; iEnt++){
            bool Ks0_vtx = recKs->chiProb[iEnt]>0;
            bool Ks0_gbk = recKs->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recKs->M[iEnt]>min_mKs 
                    && recKs->M[iEnt]<max_mKs;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk && Ks0_mKs;

            if(Ks0_ana)has_mKsKs0 = true;
        }//Ks_Rec candidates
    */
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            bool vtx = recB->chiProb[iEnt]>0;
            if(!vtx)continue;
            
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

            if(phi_ana)has_mKKPhi = true;
            else continue;
            
            bool Ks0_vtx = recBKs->chiProb[iEnt]>0;
            bool Ks0_gbk = recBKs->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recBKs->M[iEnt]>min_mKs 
                    && recBKs->M[iEnt]<max_mKs;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk && Ks0_mKs;

            if(Ks0_ana)has_mKsKs0 = true;
            else continue;

            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];
            bool deE = abs(B_deE)<dE_wid_;
            bool Mbc = B_Mbc > min_Mbc;
            
            if(deE)             has_B0_deE = true;
            else continue;
            if(Mbc)             has_B0_Mbc = true;
            else continue;
        }//B0_Rec candidates
    //////////////////////////////////////////////////
        if(has_mKKPhi)has_mKK_phi_ind++;
        if(has_mKsKs0)has_mKs_Ks0_ind++;
        if(has_B0_deE)has_B0__deE_ind++;
        if(has_B0_Mbc)has_B0__Mbc_ind++;
        
        if(!has_mKKPhi)continue;has_mKK_phi_acc++;
        if(!has_mKsKs0)continue;has_mKs_Ks0_acc++;
        if(!has_B0_deE)continue;has_B0__deE_acc++;
        if(!has_B0_Mbc)continue;has_B0__Mbc_acc++;
    }//nTotalEvents

    //////////////////////////////////////////////////
    cout<<"Total exact BphiKs chain : "<<total_exact_evt;
    cout<<endl<<endl;
    printEff("phi mKK cuts",has_mKK_phi_ind,has_mKK_phi_acc,total_exact_evt,total_exact_evt);
    printEff("Ks0 mKs cuts",has_mKs_Ks0_ind,has_mKs_Ks0_acc,has_mKK_phi_acc,total_exact_evt);
    printEff("B0 delE cuts",has_B0__deE_ind,has_B0__deE_acc,has_mKs_Ks0_acc,total_exact_evt);
    printEff("B0  Mbc cuts",has_B0__Mbc_ind,has_B0__Mbc_acc,has_B0__deE_acc,total_exact_evt);

    return;
}
