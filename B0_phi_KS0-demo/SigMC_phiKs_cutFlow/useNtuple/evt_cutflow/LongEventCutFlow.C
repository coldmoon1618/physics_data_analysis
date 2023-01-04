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
void LongEventCutFlow(){
    TString fNoPhiPreVtxCut("/home/belle2/zhai/B0_phi_KS0/Full_B0_cut_selection_SigMC/SigMC_B0_Rec_20k_noPhiPreVtxCut.root");
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_B0_cut_selection_SigMC/SigMC_B0_Rec_20k.root");
    
    // Make Ks cuts on B0_Rec without phi pre-vertexing mass cut
    TChain *B0_noPhiPreVtx  = new TChain("B0_Rec");
    B0_noPhiPreVtx          -> AddFile(fNoPhiPreVtxCut);
    MyB0Rec *noPre          = new MyB0Rec();
    noPre                   -> setBranchAddresses(B0_noPhiPreVtx);
    MyKsRec *recKs          = new MyKsRec();
    recKs                   -> setBranchAddresses(B0_noPhiPreVtx, "Ks_");

    // The rest build up from B0_Rec with phi pre-vertexing cut already applied
    TChain *B0_Rec          = new TChain("B0_Rec");
    B0_Rec                  -> AddFile(fileName);
    int Entries = B0_Rec -> GetEntries();
    cout<<Entries<<" events"<<endl;
    MyB0Rec *rec            = new MyB0Rec();
    rec                     -> setBranchAddresses(B0_Rec);
    MyPhiRec *recPhi        = new MyPhiRec();
    recPhi                  -> setBranchAddresses(B0_Rec, "phi_");
    
    // B0_MC the same for either input, so stick to the one with shorter name haha
    TChain *B0_MC           = new TChain("B0_MC");
    B0_MC                   -> AddFile(fileName);
    MyB0MC *mc              = new MyB0MC();
    mc                      -> setBranchAddresses(B0_MC);
    MyPhiKsMC *mcPhi        = new MyPhiKsMC();
    mcPhi                   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC *mcKs         = new MyPhiKsMC();
    mcKs                    -> setBranchAddresses(B0_MC, "Ks_");

    int total_exact_evt = 0;
    
    int Ks0_pre_acc_sig = 0;
    int Ks0_pre_acc_bkg = 0;
    int Ks0_pre_ind_sig = 0;
    int Ks0_pre_ind_bkg = 0;

    int Ks0_vtx_acc_sig = 0;
    int Ks0_vtx_acc_bkg = 0;
    int Ks0_vtx_ind_sig = 0;
    int Ks0_vtx_ind_bkg = 0;

    int Ks0_gbk_acc_sig = 0;
    int Ks0_gbk_acc_bkg = 0;
    int Ks0_gbk_ind_sig = 0;
    int Ks0_gbk_ind_bkg = 0;
    
    int phi_pre_acc_sig = 0;
    int phi_pre_acc_bkg = 0;
    int phi_pre_ind_sig = 0;
    int phi_pre_ind_bkg = 0;
    
    int phi_vtx_acc_sig = 0;
    int phi_vtx_acc_bkg = 0;
    int phi_vtx_ind_sig = 0;
    int phi_vtx_ind_bkg = 0;
    
    int phi_acc_acc_sig = 0;
    int phi_acc_acc_bkg = 0;
    int phi_acc_ind_sig = 0;
    int phi_acc_ind_bkg = 0;
    
    int phi_hit_acc_sig = 0;
    int phi_hit_acc_bkg = 0;
    int phi_hit_ind_sig = 0;
    int phi_hit_ind_bkg = 0;
    
    int phi_kid_acc_sig = 0;
    int phi_kid_acc_bkg = 0;
    int phi_kid_ind_sig = 0;
    int phi_kid_ind_bkg = 0;
    
    int phi_mKK_acc_sig = 0;
    int phi_mKK_acc_bkg = 0;
    int phi_mKK_ind_sig = 0;
    int phi_mKK_ind_bkg = 0;

    int rec_vtx_acc_sig = 0;
    int rec_vtx_acc_bkg = 0;
    int rec_vtx_ind_sig = 0;
    int rec_vtx_ind_bkg = 0;

    int rec_Mbc_acc_sig = 0;
    int rec_Mbc_acc_bkg = 0;
    int rec_Mbc_ind_sig = 0;
    int rec_Mbc_ind_bkg = 0;

    int rec_deE_acc_sig = 0;
    int rec_deE_acc_bkg = 0;
    int rec_deE_ind_sig = 0;
    int rec_deE_ind_bkg = 0;

    const double DELE_WIDTH = 0.1;
    const double MIN_MBC = 5.26;
    const double MAX_MBC = 999;
    const double min_mKK = 1.0;
    const double max_mKK = 1.04;

    for(int i=0; i < Entries; i++){
        B0_noPhiPreVtx -> GetEntry(i);
        B0_Rec -> GetEntry(i);
        B0_MC  -> GetEntry(i);

        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if(mcPhi->nDaus[iEnt]>=2 && mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain

        if(!hasSigChain)continue;
        total_exact_evt++;
    //////////////////////////////////////////////////
        // Based on noPhiPreVtx
        bool evtPass_Ks0_pre = false;
        bool evtPass_Ks0_vtx = false;
        bool evtPass_Ks0_gbk = false;

        // Based on regular
        bool evtPass_phi_pre = false;
        bool evtPass_phi_vtx = false;
        bool evtPass_phi_acc = false;
        bool evtPass_phi_hit = false;
        bool evtPass_phi_kid = false;
        bool evtPass_phi_mKK = false;

        bool evtPass_rec_vtx = false;
        bool evtPass_rec_Mbc = false;
        bool evtPass_rec_deE = false;
       
        for(int iEnt = 0; iEnt<noPre->nCand; iEnt++){
            bool Ks0_vtx = recKs->chiProb[iEnt]>0;
            bool Ks0_gbk = recKs->goodBelleKshort[iEnt]==1;

                       evtPass_Ks0_pre=true;
            if(Ks0_vtx)evtPass_Ks0_vtx=true;
            if(Ks0_gbk)evtPass_Ks0_gbk=true;
        }//B0_Rec candidates without phi pre vtx

        for(int iEnt = 0; iEnt<rec->nCand; iEnt++){
            bool phi_vtx = recPhi->chiProb[iEnt]>0;
            bool phi_acc = recPhi->dau_0_thetaInCDCAcceptance[iEnt]==1 
                        && recPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool phi_hit = recPhi->dau_0_nCDCHits[iEnt]>0 
                        && recPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recPhi->dau_0_kaonID[iEnt]>0.05 
                        && recPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_mKK = recPhi->M[iEnt]>min_mKK 
                        && recPhi->M[iEnt]<max_mKK;

            bool rec_vtx = rec->chiProb[iEnt]>0;
            bool rec_Mbc = rec->Mbc[iEnt]>MIN_MBC 
                        && rec->Mbc[iEnt]<MAX_MBC;
            bool rec_deE = abs(rec->deltaE[iEnt])<DELE_WIDTH;
            
                       evtPass_phi_pre=true;
            if(phi_vtx)evtPass_phi_vtx=true;
            if(phi_acc)evtPass_phi_acc=true;
            if(phi_hit)evtPass_phi_hit=true;
            if(phi_kid)evtPass_phi_kid=true;
            if(phi_mKK)evtPass_phi_mKK=true;

            if(rec_vtx)evtPass_rec_vtx=true;
            if(rec_Mbc)evtPass_rec_Mbc=true;
            if(rec_deE)evtPass_rec_deE=true;
        }//B0_Rec candidates
        //Get indep cuts first (accumulative to follow with the continue statements)
        incCount(evtPass_Ks0_pre, &Ks0_pre_ind_sig, &Ks0_pre_ind_bkg);
        incCount(evtPass_Ks0_vtx, &Ks0_vtx_ind_sig, &Ks0_vtx_ind_bkg);
        incCount(evtPass_Ks0_gbk, &Ks0_gbk_ind_sig, &Ks0_gbk_ind_bkg);

        incCount(evtPass_phi_pre, &phi_pre_ind_sig, &phi_pre_ind_bkg);
        incCount(evtPass_phi_vtx, &phi_vtx_ind_sig, &phi_vtx_ind_bkg);
        incCount(evtPass_phi_acc, &phi_acc_ind_sig, &phi_acc_ind_bkg);
        incCount(evtPass_phi_hit, &phi_hit_ind_sig, &phi_hit_ind_bkg);
        incCount(evtPass_phi_kid, &phi_kid_ind_sig, &phi_kid_ind_bkg);
        incCount(evtPass_phi_mKK, &phi_mKK_ind_sig, &phi_mKK_ind_bkg);
        
        incCount(evtPass_rec_vtx, &rec_vtx_ind_sig, &rec_vtx_ind_bkg);
        incCount(evtPass_rec_Mbc, &rec_Mbc_ind_sig, &rec_Mbc_ind_bkg);
        incCount(evtPass_rec_deE, &rec_deE_ind_sig, &rec_deE_ind_bkg);
        //Accumulative counters
        if(!evtPass_Ks0_pre)continue; incCount(evtPass_Ks0_pre, &Ks0_pre_acc_sig, &Ks0_pre_acc_bkg);
        if(!evtPass_Ks0_vtx)continue; incCount(evtPass_Ks0_vtx, &Ks0_vtx_acc_sig, &Ks0_vtx_acc_bkg);
        if(!evtPass_Ks0_gbk)continue; incCount(evtPass_Ks0_gbk, &Ks0_gbk_acc_sig, &Ks0_gbk_acc_bkg);

        if(!evtPass_phi_pre)continue; incCount(evtPass_phi_pre, &phi_pre_acc_sig, &phi_pre_acc_bkg);
        if(!evtPass_phi_vtx)continue; incCount(evtPass_phi_vtx, &phi_vtx_acc_sig, &phi_vtx_acc_bkg);
        if(!evtPass_phi_acc)continue; incCount(evtPass_phi_acc, &phi_acc_acc_sig, &phi_acc_acc_bkg);
        if(!evtPass_phi_hit)continue; incCount(evtPass_phi_hit, &phi_hit_acc_sig, &phi_hit_acc_bkg);
        if(!evtPass_phi_kid)continue; incCount(evtPass_phi_kid, &phi_kid_acc_sig, &phi_kid_acc_bkg);
        if(!evtPass_phi_mKK)continue; incCount(evtPass_phi_mKK, &phi_mKK_acc_sig, &phi_mKK_acc_bkg);
        
        if(!evtPass_rec_vtx)continue; incCount(evtPass_rec_vtx, &rec_vtx_acc_sig, &rec_vtx_acc_bkg);
        if(!evtPass_rec_Mbc)continue; incCount(evtPass_rec_Mbc, &rec_Mbc_acc_sig, &rec_Mbc_acc_bkg);
        if(!evtPass_rec_deE)continue; incCount(evtPass_rec_deE, &rec_deE_acc_sig, &rec_deE_acc_bkg);
    }//nTotalEvents

    //////////////////////////////////////////////////
    cout<<"Total exact BphiKs chain : "<<total_exact_evt;
    cout<<endl<<endl;
    printEff("Ks pre vertexing",    Ks0_pre_ind_sig, Ks0_pre_acc_sig, total_exact_evt, total_exact_evt);
    printEff("Ks chiProb>0",        Ks0_vtx_ind_sig, Ks0_vtx_acc_sig, Ks0_pre_acc_sig, total_exact_evt);
    printEff("Ks goodBelleKshort",  Ks0_gbk_ind_sig, Ks0_gbk_acc_sig, Ks0_vtx_acc_sig, total_exact_evt);

    printEff("phi pre vertexing",   phi_pre_ind_sig, phi_pre_acc_sig, Ks0_gbk_acc_sig, total_exact_evt);
    printEff("phi chiProb>0",       phi_vtx_ind_sig, phi_vtx_acc_sig, phi_pre_acc_sig, total_exact_evt);
    printEff("phi dau acc",         phi_acc_ind_sig, phi_acc_acc_sig, phi_vtx_acc_sig, total_exact_evt);
    printEff("phi dau hit",         phi_hit_ind_sig, phi_hit_acc_sig, phi_acc_acc_sig, total_exact_evt);
    printEff("phi dau kid",         phi_kid_ind_sig, phi_kid_acc_sig, phi_hit_acc_sig, total_exact_evt);
    printEff("phi m(K+K-)",         phi_mKK_ind_sig, phi_mKK_acc_sig, phi_kid_acc_sig, total_exact_evt);
    
    printEff("B0 chiProb>0",        rec_vtx_ind_sig, rec_vtx_acc_sig, phi_kid_acc_sig, total_exact_evt);
    printEff("B0 Mbc cut",          rec_Mbc_ind_sig, rec_Mbc_acc_sig, rec_vtx_acc_sig, total_exact_evt);
    printEff("B0 deltaE cut",       rec_deE_ind_sig, rec_deE_acc_sig, rec_Mbc_acc_sig, total_exact_evt);
    cout<<endl<<endl;
    
    return;
}
