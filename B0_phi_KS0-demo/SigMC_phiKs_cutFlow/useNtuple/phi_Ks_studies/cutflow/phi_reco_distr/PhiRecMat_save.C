#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyPhiRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"
void PhiRecMat_save(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");

    TChain      *phi_Rec  = new TChain("phi_Rec");
    phi_Rec     -> AddFile(fileName);
    MyPhiRec    *recPhi = new MyPhiRec();
    recPhi      -> setBranchAddresses(phi_Rec);
    int Entries = phi_Rec -> GetEntries();
    cout<< Entries << " events" <<endl;

    TChain      *phi_MC   = new TChain("phi_MC");
    phi_MC      -> AddFile(fileName);
    MyPhiKsMC   *genPhi = new MyPhiKsMC();
    genPhi      -> setBranchAddresses(phi_MC);

    TChain      *B0_MC    = new TChain("B0_MC");
    B0_MC       -> AddFile(fileName);
    MyB0MC      *mc = new MyB0MC();
    mc          -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi    = new MyPhiKsMC();
    mcPhi       -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs     = new MyPhiKsMC();
    mcKs        -> setBranchAddresses(B0_MC, "Ks_");

    TFile fMat("PhiMat.root","recreate");
    TFile fRec("PhiRec.root","recreate");
    for(int iMat = 0; iMat<=1; iMat++)
    {
        if(iMat==0)cout<<"Saving to PhiRec.root"<<endl;
        if(iMat==1)cout<<"Saving to PhiMat.root"<<endl;

        TTree *tPre_LoP_ind = new TTree("tPre_LoP_ind","");
        TTree *tVtx_LoP_ind = new TTree("tVtx_LoP_ind","");
        TTree *tCdc_LoP_ind = new TTree("tCdc_LoP_ind","");
        TTree *tHit_LoP_ind = new TTree("tHit_LoP_ind","");
        TTree *tKID_LoP_ind = new TTree("tKID_LoP_ind","");
        TTree *tMKK_LoP_ind = new TTree("tMKK_LoP_ind","");
        TTree *tPre_LoP_acc = new TTree("tPre_LoP_acc","");
        TTree *tVtx_LoP_acc = new TTree("tVtx_LoP_acc","");
        TTree *tCdc_LoP_acc = new TTree("tCdc_LoP_acc","");
        TTree *tHit_LoP_acc = new TTree("tHit_LoP_acc","");
        TTree *tKID_LoP_acc = new TTree("tKID_LoP_acc","");
        TTree *tMKK_LoP_acc = new TTree("tMKK_LoP_acc","");

        TTree *tPre_HiP_ind = new TTree("tPre_HiP_ind","");
        TTree *tVtx_HiP_ind = new TTree("tVtx_HiP_ind","");
        TTree *tCdc_HiP_ind = new TTree("tCdc_HiP_ind","");
        TTree *tHit_HiP_ind = new TTree("tHit_HiP_ind","");
        TTree *tKID_HiP_ind = new TTree("tKID_HiP_ind","");
        TTree *tMKK_HiP_ind = new TTree("tMKK_HiP_ind","");
        TTree *tPre_HiP_acc = new TTree("tPre_HiP_acc","");
        TTree *tVtx_HiP_acc = new TTree("tVtx_HiP_acc","");
        TTree *tCdc_HiP_acc = new TTree("tCdc_HiP_acc","");
        TTree *tHit_HiP_acc = new TTree("tHit_HiP_acc","");
        TTree *tKID_HiP_acc = new TTree("tKID_HiP_acc","");
        TTree *tMKK_HiP_acc = new TTree("tMKK_HiP_acc","");

        double M_phi;
        double p_phi;
        double isSig_phi;

        tPre_LoP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tVtx_LoP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tCdc_LoP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tHit_LoP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tKID_LoP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tMKK_LoP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tPre_LoP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tVtx_LoP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tCdc_LoP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tHit_LoP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tKID_LoP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tMKK_LoP_acc -> Branch("isSig", & isSig_phi, "isSig/D");

        tPre_HiP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tVtx_HiP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tCdc_HiP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tHit_HiP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tKID_HiP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tMKK_HiP_ind -> Branch("isSig", & isSig_phi, "isSig/D");
        tPre_HiP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tVtx_HiP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tCdc_HiP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tHit_HiP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tKID_HiP_acc -> Branch("isSig", & isSig_phi, "isSig/D");
        tMKK_HiP_acc -> Branch("isSig", & isSig_phi, "isSig/D");

        tPre_LoP_ind -> Branch("p", & p_phi, "p/D");
        tVtx_LoP_ind -> Branch("p", & p_phi, "p/D");
        tCdc_LoP_ind -> Branch("p", & p_phi, "p/D");
        tHit_LoP_ind -> Branch("p", & p_phi, "p/D");
        tKID_LoP_ind -> Branch("p", & p_phi, "p/D");
        tMKK_LoP_ind -> Branch("p", & p_phi, "p/D");
        tPre_LoP_acc -> Branch("p", & p_phi, "p/D");
        tVtx_LoP_acc -> Branch("p", & p_phi, "p/D");
        tCdc_LoP_acc -> Branch("p", & p_phi, "p/D");
        tHit_LoP_acc -> Branch("p", & p_phi, "p/D");
        tKID_LoP_acc -> Branch("p", & p_phi, "p/D");
        tMKK_LoP_acc -> Branch("p", & p_phi, "p/D");

        tPre_HiP_ind -> Branch("p", & p_phi, "p/D");
        tVtx_HiP_ind -> Branch("p", & p_phi, "p/D");
        tCdc_HiP_ind -> Branch("p", & p_phi, "p/D");
        tHit_HiP_ind -> Branch("p", & p_phi, "p/D");
        tKID_HiP_ind -> Branch("p", & p_phi, "p/D");
        tMKK_HiP_ind -> Branch("p", & p_phi, "p/D");
        tPre_HiP_acc -> Branch("p", & p_phi, "p/D");
        tVtx_HiP_acc -> Branch("p", & p_phi, "p/D");
        tCdc_HiP_acc -> Branch("p", & p_phi, "p/D");
        tHit_HiP_acc -> Branch("p", & p_phi, "p/D");
        tKID_HiP_acc -> Branch("p", & p_phi, "p/D");
        tMKK_HiP_acc -> Branch("p", & p_phi, "p/D");

        tPre_LoP_ind -> Branch("M", & M_phi, "M/D");
        tVtx_LoP_ind -> Branch("M", & M_phi, "M/D");
        tCdc_LoP_ind -> Branch("M", & M_phi, "M/D");
        tHit_LoP_ind -> Branch("M", & M_phi, "M/D");
        tKID_LoP_ind -> Branch("M", & M_phi, "M/D");
        tMKK_LoP_ind -> Branch("M", & M_phi, "M/D");
        tPre_LoP_acc -> Branch("M", & M_phi, "M/D");
        tVtx_LoP_acc -> Branch("M", & M_phi, "M/D");
        tCdc_LoP_acc -> Branch("M", & M_phi, "M/D");
        tHit_LoP_acc -> Branch("M", & M_phi, "M/D");
        tKID_LoP_acc -> Branch("M", & M_phi, "M/D");
        tMKK_LoP_acc -> Branch("M", & M_phi, "M/D");

        tPre_HiP_ind -> Branch("M", & M_phi, "M/D");
        tVtx_HiP_ind -> Branch("M", & M_phi, "M/D");
        tCdc_HiP_ind -> Branch("M", & M_phi, "M/D");
        tHit_HiP_ind -> Branch("M", & M_phi, "M/D");
        tKID_HiP_ind -> Branch("M", & M_phi, "M/D");
        tMKK_HiP_ind -> Branch("M", & M_phi, "M/D");
        tPre_HiP_acc -> Branch("M", & M_phi, "M/D");
        tVtx_HiP_acc -> Branch("M", & M_phi, "M/D");
        tCdc_HiP_acc -> Branch("M", & M_phi, "M/D");
        tHit_HiP_acc -> Branch("M", & M_phi, "M/D");
        tKID_HiP_acc -> Branch("M", & M_phi, "M/D");
        tMKK_HiP_acc -> Branch("M", & M_phi, "M/D");

        int Pre_LoP_ind = 0;
        int Vtx_LoP_ind = 0;
        int Cdc_LoP_ind = 0;
        int Hit_LoP_ind = 0;
        int KID_LoP_ind = 0;
        int MKK_LoP_ind = 0;
        int Pre_LoP_acc = 0;
        int Vtx_LoP_acc = 0;
        int Cdc_LoP_acc = 0;
        int Hit_LoP_acc = 0;
        int KID_LoP_acc = 0;
        int MKK_LoP_acc = 0;

        int Pre_LoPmat_ind = 0;
        int Vtx_LoPmat_ind = 0;
        int Cdc_LoPmat_ind = 0;
        int Hit_LoPmat_ind = 0;
        int KID_LoPmat_ind = 0;
        int MKK_LoPmat_ind = 0;
        int Pre_LoPmat_acc = 0;
        int Vtx_LoPmat_acc = 0;
        int Cdc_LoPmat_acc = 0;
        int Hit_LoPmat_acc = 0;
        int KID_LoPmat_acc = 0;
        int MKK_LoPmat_acc = 0;

        int Pre_HiP_ind = 0;
        int Vtx_HiP_ind = 0;
        int Cdc_HiP_ind = 0;
        int Hit_HiP_ind = 0;
        int KID_HiP_ind = 0;
        int MKK_HiP_ind = 0;
        int Pre_HiP_acc = 0;
        int Vtx_HiP_acc = 0;
        int Cdc_HiP_acc = 0;
        int Hit_HiP_acc = 0;
        int KID_HiP_acc = 0;
        int MKK_HiP_acc = 0;

        int totGenLoP = 0;
        int totGenHiP = 0;

        for(int i=0; i < Entries; i++){
            phi_Rec  -> GetEntry(i);
            phi_MC   -> GetEntry(i);
            B0_MC    -> GetEntry(i);

        //////////////////////////////////////////////////
            bool hasSigChain = false;
            for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
                if(mcPhi->nDaus[iEnt]>=2 && mcKs->nDaus[iEnt]>=2
                        && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                        && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                    hasSigChain = true;
                }
            }//B0_MC ?hasSigChain
            if(!hasSigChain)continue;
        //////////////////////////////////////////////////
            int phi_genCand = 0;
            for(int iEnt = 0; iEnt<genPhi->nCand; iEnt++){
                if(abs(genPhi->dau_0_PDG[iEnt])==321 && abs(genPhi->dau_1_PDG[iEnt])==321){
                    phi_genCand++;
                }
            }//generated exact Kshort in the signal events
            if(phi_genCand==1)totGenHiP++;
            if(phi_genCand>=2)totGenLoP+=phi_genCand-1;
        //////////////////////////////////////////////////
            for(int iEnt = 0; iEnt<recPhi->nCand; iEnt++){
                bool isLoP = recPhi->p[iEnt]<1.8;
                bool passVtx = recPhi->chiProb[iEnt]>0;
                bool passCdc = recPhi->dau_0_thetaInCDCAcceptance[iEnt]==1&&recPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
                bool passHit = recPhi->dau_0_nCDCHits[iEnt]>0&&recPhi->dau_1_nCDCHits[iEnt]>0;
                bool passKID = recPhi->dau_0_kaonID[iEnt]>0.05&&recPhi->dau_1_kaonID[iEnt]>0.05;
                bool passMKK = recPhi->M[iEnt]>1.004&&recPhi->M[iEnt]<1.034;
                bool isSig = recPhi->isSignal[iEnt]==1;
                M_phi = recPhi->M[iEnt];
                p_phi = recPhi->p[iEnt];
                isSig_phi = recPhi->isSignal[iEnt];
                if(iMat==1 && !isSig)continue;
                if(isLoP&&phi_genCand>=2){
                                {tPre_LoP_ind -> Fill(); Pre_LoP_ind++;if(isSig)Pre_LoPmat_ind++;}   
                    if(passVtx) {tVtx_LoP_ind -> Fill(); Vtx_LoP_ind++;if(isSig)Vtx_LoPmat_ind++;}
                    if(passCdc) {tCdc_LoP_ind -> Fill(); Cdc_LoP_ind++;if(isSig)Cdc_LoPmat_ind++;}
                    if(passHit) {tHit_LoP_ind -> Fill(); Hit_LoP_ind++;if(isSig)Hit_LoPmat_ind++;}
                    if(passKID) {tKID_LoP_ind -> Fill(); KID_LoP_ind++;if(isSig)KID_LoPmat_ind++;}
                    if(passMKK) {tMKK_LoP_ind -> Fill(); MKK_LoP_ind++;if(isSig)MKK_LoPmat_ind++;}
                                {tPre_LoP_acc -> Fill(); Pre_LoP_acc++;if(isSig)Pre_LoPmat_acc++;}    
                    if(!passVtx)continue;
                                {tVtx_LoP_acc -> Fill(); Vtx_LoP_acc++;if(isSig)Vtx_LoPmat_acc++;}
                    if(!passCdc)continue;
                                {tCdc_LoP_acc -> Fill(); Cdc_LoP_acc++;if(isSig)Cdc_LoPmat_acc++;}
                    if(!passHit)continue;
                                {tHit_LoP_acc -> Fill(); Hit_LoP_acc++;if(isSig)Hit_LoPmat_acc++;}
                    if(!passKID)continue;
                                {tKID_LoP_acc -> Fill(); KID_LoP_acc++;if(isSig)KID_LoPmat_acc++;}
                    if(!passMKK)continue;
                                {tMKK_LoP_acc -> Fill(); MKK_LoP_acc++;if(isSig)MKK_LoPmat_acc++;}

                }else if(phi_genCand==1){
                                {tPre_HiP_ind -> Fill(); if(isSig)Pre_HiP_ind++;}    
                    if(passVtx) {tVtx_HiP_ind -> Fill(); if(isSig)Vtx_HiP_ind++;}
                    if(passCdc) {tCdc_HiP_ind -> Fill(); if(isSig)Cdc_HiP_ind++;}
                    if(passHit) {tHit_HiP_ind -> Fill(); if(isSig)Hit_HiP_ind++;}
                    if(passKID) {tKID_HiP_ind -> Fill(); if(isSig)KID_HiP_ind++;}
                    if(passMKK) {tMKK_HiP_ind -> Fill(); if(isSig)MKK_HiP_ind++;}
                                {tPre_HiP_acc -> Fill(); if(isSig)Pre_HiP_acc++;}    
                    if(!passVtx)continue;
                                {tVtx_HiP_acc -> Fill(); if(isSig)Vtx_HiP_acc++;}
                    if(!passCdc)continue;
                                {tCdc_HiP_acc -> Fill(); if(isSig)Cdc_HiP_acc++;}
                    if(!passHit)continue;
                                {tHit_HiP_acc -> Fill(); if(isSig)Hit_HiP_acc++;}
                    if(!passKID)continue;
                                {tKID_HiP_acc -> Fill(); if(isSig)KID_HiP_acc++;}
                    if(!passMKK)continue;
                                {tMKK_HiP_acc -> Fill(); if(isSig)MKK_HiP_acc++;}
                }//LoP vs HiP
            }//candidates
        }//events

        cout<<"totGenLoP "<<totGenLoP<<endl;
        cout<<"totGenHiP "<<totGenHiP<<endl;
        cout<<endl;
        cout<<"Pre_LoP_ind="<<Pre_LoP_ind<<endl;
        cout<<"Vtx_LoP_ind="<<Vtx_LoP_ind<<endl;
        cout<<"Cdc_LoP_ind="<<Cdc_LoP_ind<<endl;
        cout<<"Hit_LoP_ind="<<Hit_LoP_ind<<endl;
        cout<<"KID_LoP_ind="<<KID_LoP_ind<<endl;
        cout<<"MKK_LoP_ind="<<MKK_LoP_ind<<endl;
        cout<<endl;
        cout<<"Pre_LoP_acc="<<Pre_LoP_acc<<endl;
        cout<<"Vtx_LoP_acc="<<Vtx_LoP_acc<<endl;
        cout<<"Cdc_LoP_acc="<<Cdc_LoP_acc<<endl;
        cout<<"Hit_LoP_acc="<<Hit_LoP_acc<<endl;
        cout<<"KID_LoP_acc="<<KID_LoP_acc<<endl;
        cout<<"MKK_LoP_acc="<<MKK_LoP_acc<<endl;
        cout<<"Number matched:"<<endl;
        cout<<"Pre_LoPmat_ind="<<Pre_LoPmat_ind<<endl;
        cout<<"Vtx_LoPmat_ind="<<Vtx_LoPmat_ind<<endl;
        cout<<"Cdc_LoPmat_ind="<<Cdc_LoPmat_ind<<endl;
        cout<<"Hit_LoPmat_ind="<<Hit_LoPmat_ind<<endl;
        cout<<"KID_LoPmat_ind="<<KID_LoPmat_ind<<endl;
        cout<<"MKK_LoPmat_ind="<<MKK_LoPmat_ind<<endl;
        cout<<endl;
        cout<<"Pre_LoPmat_acc="<<Pre_LoPmat_acc<<endl;
        cout<<"Vtx_LoPmat_acc="<<Vtx_LoPmat_acc<<endl;
        cout<<"Cdc_LoPmat_acc="<<Cdc_LoPmat_acc<<endl;
        cout<<"Hit_LoPmat_acc="<<Hit_LoPmat_acc<<endl;
        cout<<"KID_LoPmat_acc="<<KID_LoPmat_acc<<endl;
        cout<<"MKK_LoPmat_acc="<<MKK_LoPmat_acc<<endl;
        cout<<endl;
        cout<<"Pre_HiP_ind="<<Pre_HiP_ind<<endl;
        cout<<"Vtx_HiP_ind="<<Vtx_HiP_ind<<endl;
        cout<<"Cdc_HiP_ind="<<Cdc_HiP_ind<<endl;
        cout<<"Hit_HiP_ind="<<Hit_HiP_ind<<endl;
        cout<<"KID_HiP_ind="<<KID_HiP_ind<<endl;
        cout<<"MKK_HiP_ind="<<MKK_HiP_ind<<endl;
        cout<<endl;
        cout<<"Pre_HiP_acc="<<Pre_HiP_acc<<endl;
        cout<<"Vtx_HiP_acc="<<Vtx_HiP_acc<<endl;
        cout<<"Cdc_HiP_acc="<<Cdc_HiP_acc<<endl;
        cout<<"Hit_HiP_acc="<<Hit_HiP_acc<<endl;
        cout<<"KID_HiP_acc="<<KID_HiP_acc<<endl;
        cout<<"MKK_HiP_acc="<<MKK_HiP_acc<<endl;
        cout<<endl;

        if(iMat==0)fRec.cd();
        if(iMat==1)fMat.cd();

        tPre_LoP_ind -> Write();
        tVtx_LoP_ind -> Write();
        tCdc_LoP_ind -> Write();
        tHit_LoP_ind -> Write();
        tKID_LoP_ind -> Write();
        tMKK_LoP_ind -> Write();

        tPre_LoP_acc -> Write();
        tVtx_LoP_acc -> Write();
        tCdc_LoP_acc -> Write();
        tHit_LoP_acc -> Write();
        tKID_LoP_acc -> Write();
        tMKK_LoP_acc -> Write();

        tPre_HiP_ind -> Write();
        tVtx_HiP_ind -> Write();
        tCdc_HiP_ind -> Write();
        tHit_HiP_ind -> Write();
        tKID_HiP_ind -> Write();
        tMKK_HiP_ind -> Write();

        tPre_HiP_acc -> Write();
        tVtx_HiP_acc -> Write();
        tCdc_HiP_acc -> Write();
        tHit_HiP_acc -> Write();
        tKID_HiP_acc -> Write();
        tMKK_HiP_acc -> Write();

        if(iMat==0)fRec.Close();
        if(iMat==1)fMat.Close();
    }

    return;
}
