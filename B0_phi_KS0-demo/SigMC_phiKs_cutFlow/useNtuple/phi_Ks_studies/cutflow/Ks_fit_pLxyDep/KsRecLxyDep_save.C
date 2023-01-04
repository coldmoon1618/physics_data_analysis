#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyKsRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"
void KsRecLxyDep_save(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_B0_cut_selection_SigMC/SigMC_B0_Rec_20k.root");
    
    TChain *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec recKs;
    recKs.setBranchAddresses(Ks_Rec);
    int Entries = Ks_Rec -> GetEntries();
    cout<<Entries<<" events"<<endl;

    TChain *Ks_MC   = new TChain("Ks_MC");
    Ks_MC   -> AddFile(fileName);
    MyPhiKsMC genKs;
    genKs.setBranchAddresses(Ks_MC);

    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    //MyPhiKsMC mcPhi;
    //mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");

    for(int isMat = 0; isMat<2; isMat++){

        TString fOutName("");
        if(isMat) fOutName += "KsMatLxyDep_toFit.root";
        else      fOutName += "KsRecLxyDep_toFit.root";
        cout<<"Saving to "<<fOutName<<" ... "<<endl;

        TFile fOut(fOutName,"recreate");

        TTree *tPre_LoLxy_ind = new TTree("tPre_LoLxy_ind","");
        TTree *tVtx_LoLxy_ind = new TTree("tVtx_LoLxy_ind","");
        TTree *tGbk_LoLxy_ind = new TTree("tGbk_LoLxy_ind","");
        TTree *tGbk_LoLxy_acc = new TTree("tGbk_LoLxy_acc","");

        TTree *tPre_HiLxy_ind = new TTree("tPre_HiLxy_ind","");
        TTree *tVtx_HiLxy_ind = new TTree("tVtx_HiLxy_ind","");
        TTree *tGbk_HiLxy_ind = new TTree("tGbk_HiLxy_ind","");
        TTree *tGbk_HiLxy_acc = new TTree("tGbk_HiLxy_acc","");

        double p_Ks;
        double M_Ks;
        double isSig_Ks;
        double Lxy_Ks;

        tPre_LoLxy_ind -> Branch("M", & M_Ks, "M/D");
        tVtx_LoLxy_ind -> Branch("M", & M_Ks, "M/D");
        tGbk_LoLxy_ind -> Branch("M", & M_Ks, "M/D"); 
        tGbk_LoLxy_acc -> Branch("M", & M_Ks, "M/D"); 

        tPre_HiLxy_ind -> Branch("M", & M_Ks, "M/D");
        tVtx_HiLxy_ind -> Branch("M", & M_Ks, "M/D");
        tGbk_HiLxy_ind -> Branch("M", & M_Ks, "M/D"); 
        tGbk_HiLxy_acc -> Branch("M", & M_Ks, "M/D"); 

        tPre_LoLxy_ind -> Branch("p", & p_Ks, "p/D");
        tVtx_LoLxy_ind -> Branch("p", & p_Ks, "p/D");
        tGbk_LoLxy_ind -> Branch("p", & p_Ks, "p/D"); 
        tGbk_LoLxy_acc -> Branch("p", & p_Ks, "p/D"); 

        tPre_HiLxy_ind -> Branch("p", & p_Ks, "p/D");
        tVtx_HiLxy_ind -> Branch("p", & p_Ks, "p/D");
        tGbk_HiLxy_ind -> Branch("p", & p_Ks, "p/D"); 
        tGbk_HiLxy_acc -> Branch("p", & p_Ks, "p/D"); 

        tPre_LoLxy_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tVtx_LoLxy_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tGbk_LoLxy_ind -> Branch("isSig", & isSig_Ks, "isSig/D"); 
        tGbk_LoLxy_acc -> Branch("isSig", & isSig_Ks, "isSig/D"); 

        tPre_HiLxy_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tVtx_HiLxy_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tGbk_HiLxy_ind -> Branch("isSig", & isSig_Ks, "isSig/D"); 
        tGbk_HiLxy_acc -> Branch("isSig", & isSig_Ks, "isSig/D"); 

        tPre_LoLxy_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tVtx_LoLxy_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tGbk_LoLxy_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 
        tGbk_LoLxy_acc -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 

        tPre_HiLxy_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tVtx_HiLxy_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tGbk_HiLxy_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 
        tGbk_HiLxy_acc -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 

        int Pre_LoLxy_ind = 0;
        int Vtx_LoLxy_ind = 0;
        int Gbk_LoLxy_ind = 0; 
        int Gbk_LoLxy_acc = 0; 

        int Pre_HiLxy_ind = 0;
        int Vtx_HiLxy_ind = 0;
        int Gbk_HiLxy_ind = 0; 
        int Gbk_HiLxy_acc = 0; 

        int totGenLoLxy = 0;
        int totGenHiLxy = 0;

        const int LxyCut = 5;

        for(int i=0; i < Entries; i++){
            Ks_Rec  -> GetEntry(i);
            Ks_MC   -> GetEntry(i);
            B0_MC   -> GetEntry(i);

            //////////////////////////////////////////////////
            bool hasSigChain = false;
            for(int iEnt = 0; iEnt< mc.nCand; iEnt++){
                if( /*mcPhi.nDaus[iEnt]>=2&&*/  mcKs.nDaus[iEnt]>=2
                        //&& abs(mcPhi.dau_0_PDG[iEnt])==321 && abs(mcPhi.dau_1_PDG[iEnt])==321
                        && abs( mcKs.dau_0_PDG[iEnt])==211 && abs( mcKs.dau_1_PDG[iEnt])==211){
                    hasSigChain = true;
                }
            }//B0_MC ?hasSigChain
            if(!hasSigChain)continue;
            //////////////////////////////////////////////////
            for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
                if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                    if(genKs.p[iEnt]<1.8)continue;
                    if(genKs.mcFlightDistance[iEnt]<LxyCut) totGenLoLxy++;
                    else                                totGenHiLxy++;
                }
            }//generated exact Kshort in the signal events
            //////////////////////////////////////////////////
            for(int iEnt = 0; iEnt<recKs.nCand; iEnt++){
                M_Ks = recKs.M[iEnt];
                p_Ks = recKs.p[iEnt];
                isSig_Ks = recKs.isSignal[iEnt]==1;
                Lxy_Ks = recKs.flightDistance[iEnt];
                bool isLoLxy = recKs.flightDistance[iEnt]<LxyCut;
                bool passVtx = recKs.chiProb[iEnt]>0;
                bool passGbk = recKs.goodBelleKshort[iEnt]==1;
                bool isSig = recKs.isSignal[iEnt]==1;
                if(p_Ks<=1.8)continue;
                if(isMat&&!isSig)continue;
                if(isLoLxy){
                    {tPre_LoLxy_ind -> Fill(); if(isSig)Pre_LoLxy_ind++;}    
                    if(passVtx) {tVtx_LoLxy_ind -> Fill(); if(isSig)Vtx_LoLxy_ind++;}
                    if(passGbk) {tGbk_LoLxy_ind -> Fill(); if(isSig)Gbk_LoLxy_ind++;}

                    if(passVtx&&passGbk) {tGbk_LoLxy_acc -> Fill(); if(isSig)Gbk_LoLxy_acc++;}
                }else{
                    {tPre_HiLxy_ind -> Fill(); if(isSig)Pre_HiLxy_ind++;}    
                    if(passVtx) {tVtx_HiLxy_ind -> Fill(); if(isSig)Vtx_HiLxy_ind++;}
                    if(passGbk) {tGbk_HiLxy_ind -> Fill(); if(isSig)Gbk_HiLxy_ind++;}

                    if(passVtx&&passGbk) {tGbk_HiLxy_acc -> Fill(); if(isSig)Gbk_HiLxy_acc++;}
                }
            }//candidates
        }//phi

        cout<<"Total LoLxy Ks"<<totGenLoLxy<<endl;
        cout<<"Total HiLxy Ks"<<totGenHiLxy<<endl;
        cout<<"Num matched"<<endl;
        cout<<"Pre_LoLxy_ind="<<Pre_LoLxy_ind<<endl;
        cout<<"Vtx_LoLxy_ind="<<Vtx_LoLxy_ind<<endl;
        cout<<"Gbk_LoLxy_ind="<<Gbk_LoLxy_ind<<endl;
        cout<<"Gbk_LoLxy_acc="<<Gbk_LoLxy_acc<<endl;

        cout<<"Pre_HiLxy_ind="<<Pre_HiLxy_ind<<endl;
        cout<<"Vtx_HiLxy_ind="<<Vtx_HiLxy_ind<<endl;
        cout<<"Gbk_HiLxy_ind="<<Gbk_HiLxy_ind<<endl;
        cout<<"Gbk_HiLxy_acc="<<Gbk_HiLxy_acc<<endl;

        fOut.cd();

        tPre_LoLxy_ind -> Write();
        tVtx_LoLxy_ind -> Write();
        tGbk_LoLxy_ind -> Write();
        tGbk_LoLxy_acc -> Write();

        tPre_HiLxy_ind -> Write();
        tVtx_HiLxy_ind -> Write();
        tGbk_HiLxy_ind -> Write();
        tGbk_HiLxy_acc -> Write();

        fOut.Close();

    }
    return;
}
