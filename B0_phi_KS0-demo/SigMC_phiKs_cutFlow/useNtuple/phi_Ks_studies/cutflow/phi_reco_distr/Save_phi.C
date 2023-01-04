#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyPhiRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"
void Save_phi(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");

    TChain *phi_Rec  = new TChain("phi_Rec");
    phi_Rec  -> AddFile(fileName);
    MyPhiRec recPhi;
    recPhi.setBranchAddresses(phi_Rec);
    int Entries = phi_Rec -> GetEntries();
    cout<< Entries << " events" <<endl;

    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");

    TFile fOut("PhiMatched.root","recreate");

    TTree *tree = new TTree("tree","");

    double M_phi;

    tree -> Branch("M", & M_phi, "M/D");

    for(int i=0; i < Entries; i++){
        phi_Rec  -> GetEntry(i);
        B0_MC    -> GetEntry(i);

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
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recPhi.nCand; iEnt++){
            bool isLoP = recPhi.p[iEnt]<1.8;
            bool passVtx = recPhi.chiProb[iEnt]>0;
            bool passCdc = recPhi.dau_0_thetaInCDCAcceptance[iEnt]==1&&recPhi.dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool passHit = recPhi.dau_0_nCDCHits[iEnt]>0&&recPhi.dau_1_nCDCHits[iEnt]>0;
            bool passKID = recPhi.dau_0_kaonID[iEnt]>0.05&&recPhi.dau_1_kaonID[iEnt]>0.05;
            bool isSig = recPhi.isSignal[iEnt]==1;
            M_phi = recPhi.M[iEnt];
            if(!isSig)continue;
            if(!passVtx)continue;
            if(!passCdc)continue;
            if(!passHit)continue;
            if(!passKID)continue;
                {tree -> Fill();}
        }//candidates
    }//events

    fOut.cd();

    tree -> Write();

    fOut.Close();

    return;
}
