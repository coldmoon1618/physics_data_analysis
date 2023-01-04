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
void PrintB0Info(){
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_selections/SigMC/SigMC_B0_Rec_20k.root");

    TChain *B0_Rec = new TChain("B0_Rec");
    B0_Rec -> AddFile(fileName);
    int Entries = B0_Rec -> GetEntries();
    MyB0Rec rec;
    rec.setBranchAddresses(B0_Rec);
    MyPhiRec recPhi;
    recPhi.setBranchAddresses(B0_Rec, "phi_");
    MyKsRec recKs;
    recKs.setBranchAddresses(B0_Rec, "Ks_");

    TChain *B0_MC  = new TChain("B0_MC");
    B0_MC  -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");

    int tot_exact_evt = 0;
    int rec_sig_rec = 0;
    int rec_bkg_rec = 0;

    int Ks0_vtx_acc_sig = 0;
    int Ks0_vtx_acc_bkg = 0;
    int Ks0_vtx_ind_sig = 0;
    int Ks0_vtx_ind_bkg = 0;

    int Ks0_gbk_acc_sig = 0;
    int Ks0_gbk_acc_bkg = 0;
    int Ks0_gbk_ind_sig = 0;
    int Ks0_gbk_ind_bkg = 0;
    
    int phi_vtx_acc_sig = 0;
    int phi_vtx_acc_bkg = 0;
    int phi_vtx_ind_sig = 0;
    int phi_vtx_ind_bkg = 0;
    
    int phi_mKK_acc_sig = 0;
    int phi_mKK_acc_bkg = 0;
    int phi_mKK_ind_sig = 0;
    int phi_mKK_ind_bkg = 0;
    
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

    int rec_vtx_acc_sig = 0;
    int rec_vtx_acc_bkg = 0;
    int rec_vtx_ind_sig = 0;
    int rec_vtx_ind_bkg = 0;

    int rec_mbc_acc_sig = 0;
    int rec_mbc_acc_bkg = 0;
    int rec_mbc_ind_sig = 0;
    int rec_mbc_ind_bkg = 0;

    int rec_del_acc_sig = 0;
    int rec_del_acc_bkg = 0;
    int rec_del_ind_sig = 0;
    int rec_del_ind_bkg = 0;

    const double DELE_WIDTH = 0.06;
    const double MIN_MBC = 5.27;
    const double MAX_MBC = 5.29;
    const double min_mKK = 1.0;
    const double max_mKK = 1.04;

    for(int i=0; i < Entries; i++){
        B0_Rec -> GetEntry(i);
        B0_MC  -> GetEntry(i);

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
        for(int iEnt = 0; iEnt<rec.nCand; iEnt++){
            bool isSignal= rec.isSignal[iEnt]==1;

            bool Ks0_vtx = recKs.chiProb[iEnt]>0;
            bool Ks0_gbk = recKs.goodBelleKshort[iEnt]==1;

            bool phi_vtx = recPhi.chiProb[iEnt]>0;
            bool phi_mKK = recPhi.M[iEnt]>min_mKK && recPhi.M[iEnt]<max_mKK;
            bool phi_acc = recPhi.dau_0_thetaInCDCAcceptance[iEnt]==1 && recPhi.dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool phi_hit = recPhi.dau_0_nCDCHits[iEnt]>0 && recPhi.dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recPhi.dau_0_kaonID[iEnt]>0.05 && recPhi.dau_1_kaonID[iEnt]>0.05;

            bool rec_vtx = rec.chiProb[iEnt]>0;
            bool rec_mbc = rec.Mbc[iEnt]>MIN_MBC && rec.Mbc[iEnt]<MAX_MBC;
            bool rec_del = abs(rec.deltaE[iEnt])<DELE_WIDTH;

                        incCount(isSignal,&rec_sig_rec,&rec_bkg_rec);
            if(Ks0_vtx) incCount(isSignal,&Ks0_vtx_ind_sig,&Ks0_vtx_ind_bkg);
            if(Ks0_gbk) incCount(isSignal,&Ks0_gbk_ind_sig,&Ks0_gbk_ind_bkg);
            if(phi_vtx) incCount(isSignal,&phi_vtx_ind_sig,&phi_vtx_ind_bkg);
            if(phi_mKK) incCount(isSignal,&phi_mKK_ind_sig,&phi_mKK_ind_bkg);
            if(phi_acc) incCount(isSignal,&phi_acc_ind_sig,&phi_acc_ind_bkg);
            if(phi_hit) incCount(isSignal,&phi_hit_ind_sig,&phi_hit_ind_bkg);
            if(phi_kid) incCount(isSignal,&phi_kid_ind_sig,&phi_kid_ind_bkg);
            if(rec_vtx) incCount(isSignal,&rec_vtx_ind_sig,&rec_vtx_ind_bkg);
            if(rec_mbc) incCount(isSignal,&rec_mbc_ind_sig,&rec_mbc_ind_bkg);
            if(rec_del) incCount(isSignal,&rec_del_ind_sig,&rec_del_ind_bkg);
            
            if(!Ks0_vtx)continue; incCount(isSignal,&Ks0_vtx_acc_sig,&Ks0_vtx_acc_bkg);
            if(!Ks0_gbk)continue; incCount(isSignal,&Ks0_gbk_acc_sig,&Ks0_gbk_acc_bkg);
            if(!phi_vtx)continue; incCount(isSignal,&phi_vtx_acc_sig,&phi_vtx_acc_bkg);
            if(!phi_mKK)continue; incCount(isSignal,&phi_mKK_acc_sig,&phi_mKK_acc_bkg);
            if(!phi_acc)continue; incCount(isSignal,&phi_acc_acc_sig,&phi_acc_acc_bkg);
            if(!phi_hit)continue; incCount(isSignal,&phi_hit_acc_sig,&phi_hit_acc_bkg);
            if(!phi_kid)continue; incCount(isSignal,&phi_kid_acc_sig,&phi_kid_acc_bkg);
            if(!rec_vtx)continue; incCount(isSignal,&rec_vtx_acc_sig,&rec_vtx_acc_bkg);
            if(!rec_mbc)continue; incCount(isSignal,&rec_mbc_acc_sig,&rec_mbc_acc_bkg);
            if(!rec_del)continue; incCount(isSignal,&rec_del_acc_sig,&rec_del_acc_bkg);

        }//B0_Rec candidates
    //////////////////////////////////////////////////

    }//nTotalEvents

    //////////////////////////////////////////////////
    cout<<"Total exact BphiKs chain : "<<tot_exact_evt;
    cout<<endl<<endl;
    printEff("BphiKs reconstructed",rec_sig_rec,     rec_sig_rec,     tot_exact_evt,   tot_exact_evt);
    printEff("Ks chiProb>0",        Ks0_vtx_ind_sig, Ks0_vtx_acc_sig, rec_sig_rec,     tot_exact_evt);
    printEff("Ks goodBelleKshort",  Ks0_gbk_ind_sig, Ks0_gbk_acc_sig, Ks0_vtx_acc_sig, tot_exact_evt);
    printEff("phi chiProb>0",       phi_vtx_ind_sig, phi_vtx_acc_sig, Ks0_gbk_acc_sig, tot_exact_evt);
    printEff("phi m(K+K-)",         phi_mKK_ind_sig, phi_mKK_acc_sig, phi_vtx_acc_sig, tot_exact_evt);
    printEff("phi dau acc",         phi_acc_ind_sig, phi_acc_acc_sig, phi_mKK_acc_sig, tot_exact_evt);
    printEff("phi dau hit",         phi_hit_ind_sig, phi_hit_acc_sig, phi_acc_acc_sig, tot_exact_evt);
    printEff("phi dau kid",         phi_kid_ind_sig, phi_kid_acc_sig, phi_hit_acc_sig, tot_exact_evt);
    printEff("B0 chiProb>0",        rec_vtx_ind_sig, rec_vtx_acc_sig, phi_hit_acc_sig, tot_exact_evt);
    printEff("B0 Mbc cut",          rec_mbc_ind_sig, rec_mbc_acc_sig, phi_hit_acc_sig, tot_exact_evt);
    printEff("B0 deltaE cut",       rec_del_ind_sig, rec_del_acc_sig, phi_hit_acc_sig, tot_exact_evt);
    cout<<endl<<endl;
    printRej("Ks chiProb>0",        Ks0_vtx_ind_bkg, Ks0_vtx_acc_bkg, rec_bkg_rec,     rec_bkg_rec);
    printRej("Ks goodBelleKshort",  Ks0_gbk_ind_bkg, Ks0_gbk_acc_bkg, Ks0_vtx_acc_bkg, rec_bkg_rec);
    printRej("phi chiProb>0",       phi_vtx_ind_bkg, phi_vtx_acc_bkg, Ks0_gbk_acc_bkg, rec_bkg_rec);
    printRej("phi m(K+K-)",         phi_mKK_ind_bkg, phi_mKK_acc_bkg, phi_vtx_acc_bkg, rec_bkg_rec);
    printRej("phi dau acc",         phi_acc_ind_bkg, phi_acc_acc_bkg, phi_mKK_acc_bkg, rec_bkg_rec);
    printRej("phi dau hit",         phi_hit_ind_bkg, phi_hit_acc_bkg, phi_acc_acc_bkg, rec_bkg_rec);
    printRej("phi dau kid",         phi_kid_ind_bkg, phi_kid_acc_bkg, phi_hit_acc_bkg, rec_bkg_rec);
    printRej("B0 chiProb>0",        rec_vtx_ind_bkg, rec_vtx_acc_bkg, phi_hit_acc_bkg, rec_bkg_rec);
    printRej("B0 Mbc cut",          rec_mbc_ind_bkg, rec_mbc_acc_bkg, phi_hit_acc_bkg, rec_bkg_rec);
    printRej("B0 deltaE cut",       rec_del_ind_bkg, rec_del_acc_bkg, phi_hit_acc_bkg, rec_bkg_rec);
    
    return;
}
