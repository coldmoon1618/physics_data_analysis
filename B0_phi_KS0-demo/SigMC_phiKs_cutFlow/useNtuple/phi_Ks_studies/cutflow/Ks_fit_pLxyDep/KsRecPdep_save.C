#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyKsRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"

#include "MyCutFlow.h"
void KsRecPdep_save(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_selections/SigMC/SigMC_B0_Rec_20k.root");
    
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

    for(int isMat=0; isMat<2; isMat++)
    {

        TString fOutName("");
        if(isMat) fOutName += "KsMatPdep_toFit.root";
        else      fOutName += "KsRecPdep_toFit.root";
        cout<<"Saving to "<<fOutName<<" ... "<<endl;

        TFile fOut(fOutName,"recreate");

        TTree *tPre_LoP_ind = new TTree("tPre_LoP_ind","");
        TTree *tVtx_LoP_ind = new TTree("tVtx_LoP_ind","");
        TTree *tGbk_LoP_ind = new TTree("tGbk_LoP_ind","");
        TTree *tGbk_LoP_acc = new TTree("tGbk_LoP_acc","");

        TTree *tPre_HiP_ind = new TTree("tPre_HiP_ind","");
        TTree *tVtx_HiP_ind = new TTree("tVtx_HiP_ind","");
        TTree *tGbk_HiP_ind = new TTree("tGbk_HiP_ind","");
        TTree *tGbk_HiP_acc = new TTree("tGbk_HiP_acc","");

        double M_Ks;
        double p_Ks;
        double isSig_Ks;
        double Lxy_Ks;

        tPre_LoP_ind -> Branch("M", & M_Ks, "M/D");
        tVtx_LoP_ind -> Branch("M", & M_Ks, "M/D");
        tGbk_LoP_ind -> Branch("M", & M_Ks, "M/D"); 
        tGbk_LoP_acc -> Branch("M", & M_Ks, "M/D"); 

        tPre_HiP_ind -> Branch("M", & M_Ks, "M/D");
        tVtx_HiP_ind -> Branch("M", & M_Ks, "M/D");
        tGbk_HiP_ind -> Branch("M", & M_Ks, "M/D"); 
        tGbk_HiP_acc -> Branch("M", & M_Ks, "M/D"); 

        tPre_LoP_ind -> Branch("p", & p_Ks, "p/D");
        tVtx_LoP_ind -> Branch("p", & p_Ks, "p/D");
        tGbk_LoP_ind -> Branch("p", & p_Ks, "p/D"); 
        tGbk_LoP_acc -> Branch("p", & p_Ks, "p/D"); 

        tPre_HiP_ind -> Branch("p", & p_Ks, "p/D");
        tVtx_HiP_ind -> Branch("p", & p_Ks, "p/D");
        tGbk_HiP_ind -> Branch("p", & p_Ks, "p/D"); 
        tGbk_HiP_acc -> Branch("p", & p_Ks, "p/D"); 

        tPre_LoP_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tVtx_LoP_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tGbk_LoP_ind -> Branch("isSig", & isSig_Ks, "isSig/D"); 
        tGbk_LoP_acc -> Branch("isSig", & isSig_Ks, "isSig/D"); 

        tPre_HiP_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tVtx_HiP_ind -> Branch("isSig", & isSig_Ks, "isSig/D");
        tGbk_HiP_ind -> Branch("isSig", & isSig_Ks, "isSig/D"); 
        tGbk_HiP_acc -> Branch("isSig", & isSig_Ks, "isSig/D"); 

        tPre_LoP_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tVtx_LoP_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tGbk_LoP_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 
        tGbk_LoP_acc -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 

        tPre_HiP_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tVtx_HiP_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D");
        tGbk_HiP_ind -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 
        tGbk_HiP_acc -> Branch("Lxy", & Lxy_Ks, "Lxy/D"); 

        int Pre_LoP_ind = 0;
        int Vtx_LoP_ind = 0;
        int Gbk_LoP_ind = 0; 
        int Gbk_LoP_acc = 0; 

        int Pre_HiP_ind = 0;
        int Vtx_HiP_ind = 0;
        int Gbk_HiP_ind = 0; 
        int Gbk_HiP_acc = 0; 

        int totGenLoP = 0;
        int totGenHiP = 0;

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
            int Ks_genCand = 0;
            for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
                if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                    if(genKs.mcFlightDistance[iEnt]<10)
                        Ks_genCand++;
                }
            }//generated exact Kshort in the signal events
            if(Ks_genCand==1)totGenHiP++;
            if(Ks_genCand>=2)totGenLoP+=Ks_genCand-1;
            //////////////////////////////////////////////////
            for(int iEnt = 0; iEnt<recKs.nCand; iEnt++){
                M_Ks = recKs.M[iEnt];
                p_Ks = recKs.p[iEnt];
                isSig_Ks = recKs.isSignal[iEnt]==1;
                Lxy_Ks = recKs.flightDistance[iEnt];
                bool isLoP   = recKs.p[iEnt]<1.8;
                bool passVtx = recKs.chiProb[iEnt]>0;
                bool passGbk = recKs.goodBelleKshort[iEnt]==1;
                bool isSig   = recKs.isSignal[iEnt]==1;
                bool isLoLxy = recKs.flightDistance[iEnt]<10;
                if(!isLoLxy)continue;
                if(isMat&&!isSig)continue;
                if(isLoP&&Ks_genCand>=2){
                    {tPre_LoP_ind -> Fill(); if(isSig)Pre_LoP_ind++;}    
                    if(passVtx) {tVtx_LoP_ind -> Fill(); if(isSig)Vtx_LoP_ind++;}
                    if(passGbk) {tGbk_LoP_ind -> Fill(); if(isSig)Gbk_LoP_ind++;}

                    if(passVtx&&passGbk) {tGbk_LoP_acc -> Fill(); if(isSig)Gbk_LoP_acc++;}
                }else if(Ks_genCand==1){
                    {tPre_HiP_ind -> Fill(); if(isSig)Pre_HiP_ind++;}    
                    if(passVtx) {tVtx_HiP_ind -> Fill(); if(isSig)Vtx_HiP_ind++;}
                    if(passGbk) {tGbk_HiP_ind -> Fill(); if(isSig)Gbk_HiP_ind++;}

                    if(passVtx&&passGbk) {tGbk_HiP_acc -> Fill(); if(isSig)Gbk_HiP_acc++;}
                }
            }//candidates
        }//phi

        cout<<"Total LoP Ks"<<totGenLoP<<endl;
        cout<<"Total HiP Ks"<<totGenHiP<<endl;
        cout<<"Num matched"<<endl;
        cout<<"Pre_LoP_ind="<<Pre_LoP_ind<<endl;
        cout<<"Vtx_LoP_ind="<<Vtx_LoP_ind<<endl;
        cout<<"Gbk_LoP_ind="<<Gbk_LoP_ind<<endl;
        cout<<"Gbk_LoP_acc="<<Gbk_LoP_acc<<endl;

        cout<<"Pre_HiP_ind="<<Pre_HiP_ind<<endl;
        cout<<"Vtx_HiP_ind="<<Vtx_HiP_ind<<endl;
        cout<<"Gbk_HiP_ind="<<Gbk_HiP_ind<<endl;
        cout<<"Gbk_HiP_acc="<<Gbk_HiP_acc<<endl;

        fOut.cd();

        tPre_LoP_ind -> Write();
        tVtx_LoP_ind -> Write();
        tGbk_LoP_ind -> Write();
        tGbk_LoP_acc -> Write();

        tPre_HiP_ind -> Write();
        tVtx_HiP_ind -> Write();
        tGbk_HiP_ind -> Write();
        tGbk_HiP_acc -> Write();

        fOut.Close();

        cout<<"HiP Ks"<<endl;
        printEff("Ks pre-vtx M",                    Pre_HiP_ind, totGenHiP,    totGenHiP);
        printEff("Ks chiProb>0",                    Vtx_HiP_ind, Pre_HiP_ind,  totGenHiP);
        printEff("Ks goodBelleKs",                  Gbk_HiP_ind, -Gbk_HiP_ind, totGenHiP);
        printEff("Ks chiProb>0 and goodBelleKs",    Gbk_HiP_acc, Vtx_HiP_ind,  totGenHiP);

        cout<<"LoP Ks"<<endl;
        printEff("Ks pre-vtx M",                    Pre_LoP_ind, totGenLoP,  totGenLoP);
        printEff("Ks chiProb>0",                    Vtx_LoP_ind, Pre_LoP_ind,  totGenLoP);
        printEff("Ks goodBelleKs",                  Gbk_LoP_ind, -Gbk_LoP_ind, totGenLoP);
        printEff("Ks chiProb>0 and goodBelleKs",    Gbk_LoP_acc, Vtx_LoP_ind,  totGenLoP);
    }

    return;
}
