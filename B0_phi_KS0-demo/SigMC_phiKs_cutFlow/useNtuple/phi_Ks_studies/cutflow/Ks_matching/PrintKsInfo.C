#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0MC.h"
#include "MyKsRec.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
void PrintKsInfo(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_selections/SigMC/SigMC_B0_Rec_20k.root");

    TChain *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec recKs;
    recKs.setBranchAddresses(Ks_Rec);
    int Entries = Ks_Rec -> GetEntries();
   
    TChain *Ks_MC   = new TChain("Ks_MC");
    Ks_MC   -> AddFile(fileName);
    MyPhiKsMC genKs;
    genKs.setBranchAddresses(Ks_MC);

    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");

    int tot_exact_evt = 0;
    int tot_exact_Ks0 = 0;
    
    int Ks0_sig_rec = 0;
    int Ks0_bkg_rec = 0;
    int Ks0_sig_vtx = 0;
    int Ks0_bkg_vtx = 0;
    int Ks0_sig_gbk = 0;
    int Ks0_bkg_gbk = 0;

    int mc_trk_in   = 0;
    int mc_2trks_in = 0;
    int mc_trk_pt   = 0;
    int mc_2trks_pt = 0;
    int mc_trk_bo   = 0;
    int mc_2trks_bo = 0;

    for(int i=0; i < Entries; i++){
        B0_MC   -> GetEntry(i);
        Ks_Rec  -> GetEntry(i);
        Ks_MC   -> GetEntry(i);

    //////////////////////////////////////////////////
        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc.nCand; iEnt++){
            if(mcPhi.nDaus[iEnt]>=2 && mcKs.nDaus[iEnt]>=2
                && abs(mcPhi.dau_0_PDG[iEnt])==321 && abs(mcPhi.dau_1_PDG[iEnt])==321
                && abs( mcKs.dau_0_PDG[iEnt])==211 && abs( mcKs.dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;
        tot_exact_evt++;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
            if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                tot_exact_Ks0++;
            }
        }//generated exact Kshort in the signal events

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs.nCand; iEnt++){
            if(recKs.isSignal[iEnt]!=1)continue;
            bool Ks0_vtx = recKs.chiProb[iEnt]>0;
            bool Ks0_gbk = recKs.goodBelleKshort[iEnt]==1;

            incCount(evtPass_Ks0_pre, &Ks0_pre_ind_sig, &Ks0_pre_ind_bkg);
            incCount(evtPass_Ks0_mom, &Ks0_mom_ind_sig, &Ks0_mom_ind_bkg);
            incCount(evtPass_Ks0_vtx, &Ks0_vtx_ind_sig, &Ks0_vtx_ind_bkg);
            incCount(evtPass_Ks0_gbk, &Ks0_gbk_ind_sig, &Ks0_gbk_ind_bkg);
            //Accumulative counters
            if(!evtPass_Ks0_pre)continue; incCount(evtPass_Ks0_pre, &Ks0_pre_acc_sig, &Ks0_pre_acc_bkg);
            if(!evtPass_Ks0_mom)continue; incCount(evtPass_Ks0_mom, &Ks0_mom_acc_sig, &Ks0_mom_acc_bkg);
            if(!evtPass_Ks0_vtx)continue; incCount(evtPass_Ks0_vtx, &Ks0_vtx_acc_sig, &Ks0_vtx_acc_bkg);
            if(!evtPass_Ks0_gbk)continue; incCount(evtPass_Ks0_gbk, &Ks0_gbk_acc_sig, &Ks0_gbk_acc_bkg);

        }//Ks0 rec candis

    }//nTotalEvents

    //////////////////////////////////////////////////
    cout<<"Total exact events : "<<tot_exact_evt<<endl;
    cout<<endl;
    printEff("Ks pre vertexing",    Ks0_pre_ind_sig, Ks0_pre_acc_sig, total_exact_evt, total_exact_evt);
    printEff("Ks Momentum binning", Ks0_mom_ind_sig, Ks0_mom_acc_sig, Ks0_pre_acc_sig, total_exact_evt);
    printEff("Ks chiProb>0",        Ks0_vtx_ind_sig, Ks0_vtx_acc_sig, Ks0_mom_acc_sig, total_exact_evt);
    printEff("Ks goodBelleKshort",  Ks0_gbk_ind_sig, Ks0_gbk_acc_sig, Ks0_vtx_acc_sig, total_exact_evt);

    return;
}
