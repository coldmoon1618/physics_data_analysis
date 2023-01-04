#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0MC.h"
#include "MyPhiRec.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
void PrintPhiInfo(bool phi_cutAway_0 =1){
    if(phi_cutAway_0){
        cout<<"Set to cut away chiProb==0"<<endl;
    }else{
        cout<<"Set to  include chiProb==0"<<endl;
    }
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_selections/SigMC/SigMC_B0_Rec_20k.root");

    TChain *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile(fileName);
    MyPhiRec recPhi;
    recPhi.setBranchAddresses(phi_Rec);
    int Entries = phi_Rec -> GetEntries();

    TChain *phi_MC  = new TChain("phi_MC");
    phi_MC  -> AddFile(fileName);
    MyPhiKsMC genPhi;
    genPhi.setBranchAddresses(phi_MC);
   
    TChain *B0_MC  = new TChain("B0_MC");
    B0_MC  -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC,  "Ks_");

    int tot_exact_evt = 0;
    int tot_exact_phi = 0;
    
    int phi_sig_rec = 0;
    int phi_bkg_rec = 0;
    int phi_sig_vtx = 0;
    int phi_bkg_vtx = 0;
    int phi_sig_VTX = 0;
    int phi_bkg_VTX = 0;
    int phi_sig_acc = 0;
    int phi_bkg_acc = 0;
    int phi_sig_hit = 0;
    int phi_bkg_hit = 0;
    int phi_sig_kid = 0;
    int phi_bkg_kid = 0;

    double minPt = 0.06;
    int mc_trk_in   = 0;
    int mc_2trks_in = 0;
    int mc_trk_pt   = 0;
    int mc_2trks_pt = 0;
    int mc_trk_bo   = 0;
    int mc_2trks_bo = 0;
    for(int i=0; i < Entries; i++){
        B0_MC  -> GetEntry(i);
        phi_Rec-> GetEntry(i);
        phi_MC -> GetEntry(i);

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
        for(int iEnt = 0; iEnt<genPhi.nCand; iEnt++){
            if(abs(genPhi.dau_0_PDG[iEnt])==321 && abs(genPhi.dau_1_PDG[iEnt])==321){
                tot_exact_phi++;
                bool dau_0_in = genPhi.dau_0_thetaInCDCAcceptance[iEnt]==1;
                bool dau_1_in = genPhi.dau_1_thetaInCDCAcceptance[iEnt]==1;
                bool dau_0_pt = genPhi.dau_0_pt[iEnt]>minPt;
                bool dau_1_pt = genPhi.dau_1_pt[iEnt]>minPt;
                bool dau_0_bo = dau_0_in && dau_0_pt;
                bool dau_1_bo = dau_1_in && dau_1_pt;

                if(dau_0_in)mc_trk_in++;
                if(dau_0_in)mc_trk_in++;
                if(dau_0_in&&dau_1_in)mc_2trks_in++;
                if(dau_0_pt)mc_trk_pt++;
                if(dau_1_pt)mc_trk_pt++;
                if(dau_0_pt&&dau_1_pt)mc_2trks_pt++;
                if(dau_0_bo)mc_trk_bo++;
                if(dau_1_bo)mc_trk_bo++;
                if(dau_0_bo&&dau_1_bo)mc_2trks_bo++;
            }
        }//generated exact phi in the signal events

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recPhi.nCand; iEnt++){
            if(recPhi.isSignal[iEnt]==1){
                phi_sig_rec++;
            }else{
                phi_bkg_rec++;
            }

            //vertex cut comparison
            if(!phi_cutAway_0){
                if(recPhi.chiProb[iEnt]<0)continue;
                if(recPhi.isSignal[iEnt]==1){
                    phi_sig_vtx++;
                }else{
                    phi_bkg_vtx++;
                }
            }else{
                if(recPhi.chiProb[iEnt]<=0)continue;
                if(recPhi.isSignal[iEnt]==1){
                    phi_sig_VTX++;
                }else{
                    phi_bkg_VTX++;
                }
            }

            // CDC acceptance cut
            if(recPhi.dau_0_thetaInCDCAcceptance[iEnt]!=1
             ||recPhi.dau_1_thetaInCDCAcceptance[iEnt]!=1)continue;
            if(recPhi.isSignal[iEnt]==1){
                phi_sig_acc++;
            }else{
                phi_bkg_acc++;
            }
       
            // CDC hit cut
            if(recPhi.dau_0_nCDCHits[iEnt]<=0
             ||recPhi.dau_1_nCDCHits[iEnt]<=0)continue;
            if(recPhi.isSignal[iEnt]==1){
                phi_sig_hit++;
            }else{
                phi_bkg_hit++;
            }

            // kaonID cut
            if(recPhi.dau_0_kaonID[iEnt]<=0.05
             ||recPhi.dau_1_kaonID[iEnt]<=0.05)continue;
            if(recPhi.isSignal[iEnt]==1){
                phi_sig_kid++;
            }else{
                phi_bkg_kid++;
            }
        }//phi rec candis

    }//nTotalEvents

    //////////////////////////////////////////////////
    cout<<"Total exact events : "<<tot_exact_evt<<endl;
    cout<<endl;
    //////////////////////////////////////////////////
    cout<<"Total exact  phis  : "<<tot_exact_phi<<endl;
    cout<<"   both trks inCDC : "<<mc_2trks_in<<"\t percentage: "<<mc_2trks_in*.5/(tot_exact_phi*.5)<<endl;
    cout<<"   tot dau trks in : "<<mc_trk_in  <<"\t percentage: "<<mc_trk_in*.5/(tot_exact_phi)<<endl;
    cout<<"   both trks>minPt : "<<mc_2trks_pt<<"\t percentage: "<<mc_2trks_pt*.5/(tot_exact_phi*.5)<<endl;
    cout<<"   tot dau trks    : "<<mc_trk_pt  <<"\t percentage: "<<mc_trk_pt*.5/(tot_exact_phi)<<endl;
    cout<<"   both trks both  : "<<mc_2trks_bo<<"\t percentage: "<<mc_2trks_bo*.5/(tot_exact_phi*.5)<<endl;
    cout<<"   tot dau trks    : "<<mc_trk_bo  <<"\t percentage: "<<mc_trk_bo*.5/(tot_exact_phi)<<endl;
    cout<<endl;
    //////////////////////////////////////////////////
    //////////////////////////////////////////////////
    cout<<"Phi cand  sig   tot: "<<phi_sig_rec<<endl;
    cout<<"          bkg   tot: "<<phi_bkg_rec<<endl;
    //////////////////////////////////////////////////
    if(phi_cutAway_0){
    cout<<"    chiProb>0  sig : "<<phi_sig_VTX;
    cout<<"\trse="<<  phi_sig_VTX*.5/(phi_sig_rec*.5);
    cout<<"\tase="<<  phi_sig_VTX*.5/(tot_exact_phi*.5)<<endl;

    cout<<"               bkg : "<<phi_bkg_VTX;
    cout<<"\trbr="<<1-phi_bkg_VTX*.5/(phi_bkg_rec*.5)<<endl;
    }else{
    cout<<"    chiProb>=0 sig : "<<phi_sig_vtx;
    cout<<"\trse="<<  phi_sig_vtx*.5/(phi_sig_rec*.5);
    cout<<"\tase="<<  phi_sig_vtx*.5/(tot_exact_phi*.5)<<endl;

    cout<<"               bkg : "<<phi_bkg_vtx;
    cout<<"\trbr="<<1-phi_bkg_vtx*.5/(phi_bkg_rec*.5)<<endl;
    }
    //////////////////////////////////////////////////
    cout<<"    trks inAcc sig : "<<phi_sig_acc;
    cout<<"\trse="<<  phi_sig_acc*.5/(phi_sig_rec*.5);
    cout<<"\tase="<<  phi_sig_acc*.5/(tot_exact_phi*.5)<<endl;

    cout<<"               bkg : "<<phi_bkg_acc;
    cout<<"\trbr="<<1-phi_bkg_acc*.5/(phi_bkg_rec*.5)<<endl;
    //////////////////////////////////////////////////
    cout<<"    trks hit>0 sig : "<<phi_sig_hit;
    cout<<"\trse="<<  phi_sig_hit*.5/(phi_sig_rec*.5);
    cout<<"\tase="<<  phi_sig_hit*.5/(tot_exact_phi*.5)<<endl;
    
    cout<<"               bkg : "<<phi_bkg_hit;
    cout<<"\trbr="<<1-phi_bkg_hit*.5/(phi_bkg_rec*.5)<<endl;
    //////////////////////////////////////////////////
    cout<<"    trks kID>.05 sig:"<<phi_sig_kid;
    cout<<"\trse="<<  phi_sig_kid*.5/(phi_sig_rec*.5);
    cout<<"\tase="<<  phi_sig_kid*.5/(tot_exact_phi*.5)<<endl;
    
    cout<<"               bkg : "<<phi_bkg_kid;
    cout<<"\trbr="<<1-phi_bkg_kid*.5/(phi_bkg_rec*.5)<<endl;
    cout<<endl;

    return;
}
