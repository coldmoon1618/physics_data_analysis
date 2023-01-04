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

void MC13aCutFlow(){

    TChain      *B0_Rec  = new TChain("B0_Rec");
    for(int iF = 1; iF<=60; iF++){
        B0_Rec  -> AddFile(Form("/home/belle2/zhai/extraSpace/Mar2221/Reco_%i.root",iF));
    }
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs  = new MyKsRec();
    recBKs  -> setBranchAddresses(B0_Rec, "Ks_");
    
    int Entries = B0_Rec -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    TChain      *B0_MC   = new TChain("B0_MC");
    for(int iF = 1; iF<=60; iF++){
        B0_MC  -> AddFile(Form("/home/belle2/zhai/extraSpace/Mar2221/Reco_%i.root",iF));
    }
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
        B0_MC  -> GetEntry(i);

        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain

        //if(!hasSigChain)continue;
        total_exact_evt++;
        bool has_mKKPhi = false;
        bool has_mKsKs0 = false;
        bool has_B0_deE = false;
        bool has_B0_Mbc = false;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            bool vtx = recBPhi->chiProb[iEnt]>0;
            bool cdc = recBPhi->dau_0_thetaInCDCAcceptance[iEnt]==1 
                    && recBPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool hit = recBPhi->dau_0_nCDCHits[iEnt]>0
                    && recBPhi->dau_1_nCDCHits[iEnt]>0;
            bool kid = recBPhi->dau_0_kaonID[iEnt]>0.05
                    && recBPhi->dau_1_kaonID[iEnt]>0.05;
            bool mKK = recBPhi->M[iEnt]>min_mKK
                    && recBPhi->M[iEnt]<max_mKK;
            bool ana = vtx && cdc && hit && kid;

            if(ana&&mKK)has_mKKPhi = true;
        }//phi_Rec candidates
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            bool vtx = recBKs->chiProb[iEnt]>0;
            bool gbk = recBKs->goodBelleKshort[iEnt]==1;
            bool mKs = recBKs->M[iEnt]>min_mKs 
                    && recBKs->M[iEnt]<max_mKs;
            bool ana = vtx && gbk;

            if(mKs&&mKs)has_mKsKs0 = true;
        }//Ks_Rec candidates
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];
            bool vtx = recB->chiProb[iEnt]>0;
            bool deE = abs(B_deE)<dE_wid_;
            bool Mbc = B_Mbc > min_Mbc;
            
            if(deE)             has_B0_deE = true;
            if(Mbc)             has_B0_Mbc = true;
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
