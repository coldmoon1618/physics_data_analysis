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
void PrintCutFlow(){
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_B0_cut_selection_SigMC/SigMC_B0_Rec_20k.root");

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
    int has_std_phi_ind = 0;
    int has_std_phi_acc = 0;
    int has_std_Ks0_ind = 0;
    int has_std_Ks0_acc = 0;
    int has_mKK_phi_ind = 0;
    int has_mKK_phi_acc = 0;
    int has_mKs_Ks0_ind = 0;
    int has_mKs_Ks0_acc = 0;
    int has_B0__vtx_ind = 0;
    int has_B0__vtx_acc = 0;
    int has_B0__mbc_ind = 0;
    int has_B0__mbc_acc = 0;
    int has_B0__deE_ind = 0;
    int has_B0__deE_acc = 0;
    int has_B0__Mbc_ind = 0;
    int has_B0__Mbc_acc = 0;
    int has_B0__DeE_ind = 0;
    int has_B0__DeE_acc = 0;

    const double dE_wid_ = 0.10;
    const double min_Mbc = 5.26;
    const double max_Mbc = 9999;
    const double dE_WID_ = 0.06;
    const double MIN_Mbc = 5.27;
    const double MAX_Mbc = 5.29;
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
        bool has_StdPhi = false;
        bool has_mKKPhi = false;
        bool has_AnaPhi = false;

        bool has_StdKs0 = false;
        bool has_mKsKs0 = false;
        bool has_AnaKs0 = false;
        
        bool has_B0_vtx = false;
        bool has_B0_mbc = false;
        bool has_B0_deE = false;
        bool has_B0_Mbc = false;
        bool has_B0_DeE = false;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recPhi->nCand; iEnt++){
            bool vtx = recPhi->chiProb[iEnt]>0;
            bool cdc = recPhi->dau_0_thetaInCDCAcceptance[iEnt]==1 
                    && recPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool hit = recPhi->dau_0_nCDCHits[iEnt]>0
                    && recPhi->dau_1_nCDCHits[iEnt]>0;
            bool kid = recPhi->dau_0_kaonID[iEnt]>0.05
                    && recPhi->dau_1_kaonID[iEnt]>0.05;
            bool mKK = recPhi->M[iEnt]>min_mKK
                    && recPhi->M[iEnt]<max_mKK;
            bool ana = vtx && cdc && hit && kid;

            if(ana)     has_StdPhi = true;
            if(mKK)     has_mKKPhi = true;
            if(ana&&mKK)has_AnaPhi = true;

        }//phi_Rec candidates
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs->nCand; iEnt++){
            bool vtx = recKs->chiProb[iEnt]>0;
            bool gbk = recKs->goodBelleKshort[iEnt]==1;
            bool mKs = recKs->M[iEnt]>min_mKs 
                    && recKs->M[iEnt]<max_mKs;
            bool ana = vtx && gbk;

            if(ana)     has_StdKs0 = true;
            if(mKs)     has_mKsKs0 = true;
            if(ana&&mKs)has_AnaKs0 = true;
        }//Ks_Rec candidates
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];
            bool vtx = recB->chiProb[iEnt]>0;
            bool mbc = B_Mbc > min_Mbc
                    && B_Mbc < max_Mbc;
            bool deE = abs(B_deE)<dE_wid_;
            bool Mbc = B_Mbc > MIN_Mbc
                    && B_Mbc < MAX_Mbc;
            bool DeE = abs(B_deE)<dE_WID_;
            if(vtx)             has_B0_vtx = true;
            if(mbc)             has_B0_mbc = true;
            if(deE)             has_B0_deE = true;
            if(Mbc)             has_B0_Mbc = true;
            if(DeE)             has_B0_DeE = true;
            
        }//B0_Rec candidates
    //////////////////////////////////////////////////
        if(has_StdPhi)has_std_phi_ind++;
        if(has_mKKPhi)has_mKK_phi_ind++;
        if(has_StdKs0)has_std_Ks0_ind++;
        if(has_mKsKs0)has_mKs_Ks0_ind++;
        if(has_B0_vtx)has_B0__vtx_ind++;
        if(has_B0_mbc)has_B0__mbc_ind++;
        if(has_B0_deE)has_B0__deE_ind++;
        if(has_B0_Mbc)has_B0__Mbc_ind++;
        if(has_B0_DeE)has_B0__DeE_ind++;
        
        if(!has_StdPhi)continue;has_std_phi_acc++;
        if(!has_AnaPhi)continue;has_mKK_phi_acc++;
        if(!has_StdKs0)continue;has_std_Ks0_acc++;
        if(!has_AnaKs0)continue;has_mKs_Ks0_acc++;
        if(!has_B0_vtx)continue;has_B0__vtx_acc++;
        if(!has_B0_mbc)continue;has_B0__mbc_acc++;
        if(!has_B0_deE)continue;has_B0__deE_acc++;
        if(!has_B0_Mbc)continue;has_B0__Mbc_acc++;
        if(!has_B0_DeE)continue;has_B0__DeE_acc++;
    }//nTotalEvents

    //////////////////////////////////////////////////
    cout<<"Total exact BphiKs chain : "<<total_exact_evt;
    cout<<endl<<endl;
    printEff("phi ana cuts",has_std_phi_ind,has_std_phi_acc,total_exact_evt,total_exact_evt);
    printEff("phi mKK cuts",has_mKK_phi_ind,has_mKK_phi_acc,has_std_phi_acc,total_exact_evt);
    printEff("Ks0 ana cuts",has_std_Ks0_ind,has_std_Ks0_acc,has_mKK_phi_acc,total_exact_evt);
    printEff("Ks0 mKs cuts",has_mKs_Ks0_ind,has_mKs_Ks0_acc,has_std_Ks0_acc,total_exact_evt);
    printEff("B0  vtx cuts",has_B0__vtx_ind,has_B0__vtx_acc,has_mKs_Ks0_acc,total_exact_evt);
    printEff("B0  Mbc cuts",has_B0__mbc_ind,has_B0__mbc_acc,has_B0__vtx_acc,total_exact_evt);
    printEff("B0 delE cuts",has_B0__deE_ind,has_B0__deE_acc,has_B0__mbc_acc,total_exact_evt);

    printEff("B0  Mbc cuts",has_B0__Mbc_ind,has_B0__Mbc_acc,has_B0__vtx_acc,total_exact_evt);
    printEff("B0 delE cuts",has_B0__DeE_ind,has_B0__DeE_acc,has_B0__Mbc_acc,total_exact_evt);

    return;
}
