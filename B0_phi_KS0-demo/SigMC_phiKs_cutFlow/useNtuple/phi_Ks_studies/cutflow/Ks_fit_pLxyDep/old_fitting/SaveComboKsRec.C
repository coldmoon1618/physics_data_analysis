#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyKsRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"
void SaveComboKsRec(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Ks_selections/Combo/Combo_Ks_selection.root");
    
    TChain *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec recKs;
    recKs.setBranchAddresses(Ks_Rec);
    int Entries = Ks_Rec -> GetEntries();

    TChain *Ks_MC   = new TChain("Ks_MC");
    Ks_MC   -> AddFile(fileName);
    MyPhiKsMC genKs;
    genKs.setBranchAddresses(Ks_MC);

    TFile fOut("Combo_Ks_toFit.root","recreate");

    TTree *tPre_p_ind = new TTree("tPre_p_ind","");
    TTree *tVtx_p_ind = new TTree("tVtx_p_ind","");
    TTree *tGbk_p_ind = new TTree("tGbk_p_ind","");
    TTree *tPre_p_acc = new TTree("tPre_p_acc","");
    TTree *tVtx_p_acc = new TTree("tVtx_p_acc","");
    TTree *tGbk_p_acc = new TTree("tGbk_p_acc","");

    double M_Ks;
    tPre_p_ind -> Branch("M", & M_Ks, "M/D");
    tVtx_p_ind -> Branch("M", & M_Ks, "M/D");
    tGbk_p_ind -> Branch("M", & M_Ks, "M/D"); 
    tPre_p_acc -> Branch("M", & M_Ks, "M/D");
    tVtx_p_acc -> Branch("M", & M_Ks, "M/D");
    tGbk_p_acc -> Branch("M", & M_Ks, "M/D"); 

    int Pre_p_ind = 0;
    int Vtx_p_ind = 0;
    int Gbk_p_ind = 0; 
    int Pre_p_acc = 0;
    int Vtx_p_acc = 0;
    int Gbk_p_acc = 0; 

    int totGenKs = 0;

    for(int i=0; i < Entries; i++){
        Ks_Rec  -> GetEntry(i);
        Ks_MC   -> GetEntry(i);

    //////////////////////////////////////////////////
        int Ks_genCand = 0;
        for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
            if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                Ks_genCand++;
            }
        }//generated exact Kshort in the signal events
        totGenKs+=Ks_genCand-1;
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs.nCand; iEnt++){
            M_Ks = recKs.M[iEnt];
            bool isLoP = recKs.p[iEnt]<1.8;
            bool passVtx = recKs.chiProb[iEnt]>0;
            bool passGbk = recKs.goodBelleKshort[iEnt]==1;
            bool isSig = recKs.isSignal[iEnt]==1;
            if(!isLoP)continue;
                        {tPre_p_ind -> Fill(); if(isSig)Pre_p_ind++;}    
            if(passVtx) {tVtx_p_ind -> Fill(); if(isSig)Vtx_p_ind++;}
            if(passGbk) {tGbk_p_ind -> Fill(); if(isSig)Gbk_p_ind++;}
            
                        {tPre_p_acc -> Fill(); if(isSig)Pre_p_acc++;}    
            if(!passVtx)continue;
                        {tVtx_p_acc -> Fill(); if(isSig)Vtx_p_acc++;}
            if(!passGbk)continue;
                        {tGbk_p_acc -> Fill(); if(isSig)Gbk_p_acc++;}
        }//candidates
    }//phi

    cout<<"Total gen Ks"<<totGenKs<<endl;
    cout<<"Num matched"<<endl;
    cout<<"Pre_p_ind="<<Pre_p_ind<<endl;
    cout<<"Vtx_p_ind="<<Vtx_p_ind<<endl;
    cout<<"Gbk_p_ind="<<Gbk_p_ind<<endl;
    cout<<"Pre_p_acc="<<Pre_p_acc<<endl;
    cout<<"Vtx_p_acc="<<Vtx_p_acc<<endl;
    cout<<"Gbk_p_acc="<<Gbk_p_acc<<endl;

    fOut.cd();

    tPre_p_ind -> Write();
    tVtx_p_ind -> Write();
    tGbk_p_ind -> Write();

    tPre_p_acc -> Write();
    tVtx_p_acc -> Write();
    tGbk_p_acc -> Write();

    fOut.Close();

    return;
}
