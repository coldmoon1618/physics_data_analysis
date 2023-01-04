#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyKsRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"

#include "MyCutFlow.h"
void Ks_matched_info(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");
    
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

    int tot_Ks0_hiP = 0;
    int Pre_Ks0_acc = 0;
    int Gbk_Ks0_acc = 0; 
    int MKs_Ks0_acc = 0;

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
                Ks_genCand++;
            }
        }//generated exact Kshort in the signal events
        if(Ks_genCand==1)tot_Ks0_hiP++;
        //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs.nCand; iEnt++){
            bool passVtx = recKs.chiProb[iEnt]>0;
            bool passGbk = recKs.goodBelleKshort[iEnt]==1;
            //bool passMKs = recKs.M[iEnt]>0.483 && recKs.M[iEnt]<0.513;
            bool passMKs = recKs.M[iEnt]>0.483 && recKs.M[iEnt]<0.513;
            bool isSig   = recKs.isSignal[iEnt]==1;
            if(!isSig)continue;
            if(Ks_genCand==1){
                if(!passVtx)continue;Pre_Ks0_acc++;
                if(!passVtx)continue;
                if(!passGbk)continue;Gbk_Ks0_acc++;
                if(!passMKs)continue;MKs_Ks0_acc++;
            }
        }//candidates
    }//evts

    cout<<"tot_Ks0_hiP: "<<tot_Ks0_hiP<<endl;
    cout<<"Pre_Ks0_acc: "<<Pre_Ks0_acc<<endl;
    cout<<"Gbk_Ks0_acc: "<<Gbk_Ks0_acc<<endl;
    cout<<"MKs_Ks0_acc: "<<MKs_Ks0_acc<<endl;

    return;
}
