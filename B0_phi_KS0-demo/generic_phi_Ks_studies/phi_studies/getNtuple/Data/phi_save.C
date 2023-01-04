#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyPhiRec.h"
void phi_save(){
    TString fileName("Data_phi_134ipb.root");

    TChain *phi_Rec  = new TChain("phi_Rec");
    phi_Rec  -> AddFile(fileName);
    MyPhiRec recPhi;
    recPhi.setBranchAddresses(phi_Rec);
    int Entries = phi_Rec -> GetEntries();
    cout<<Entries<<" events"<<endl;

    TString fOutName("phi_tree.root");
    TFile fOut(fOutName,"recreate");
    TTree *tree_1 = new TTree("tree_1","");
    TTree *tree_2 = new TTree("tree_2","");

    double M_phi;

    tree_1 -> Branch("M", & M_phi, "M/D");
    tree_2 -> Branch("M", & M_phi, "M/D");
    for(int i=0; i < Entries; i++){
        phi_Rec  -> GetEntry(i);

        //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recPhi.nCand; iEnt++){
            M_phi = recPhi.M[iEnt];
            tree_1 -> Fill();
            if(!(recPhi.chiProb[iEnt]>0))continue;
            if(!(recPhi.dau_0_nCDCHits[iEnt]>.5))continue;
            if(!(recPhi.dau_1_nCDCHits[iEnt]>.5))continue;
            if(!(recPhi.dau_0_thetaInCDCAcceptance[iEnt]>.5))continue;
            if(!(recPhi.dau_1_thetaInCDCAcceptance[iEnt]>.5))continue;
            if(!(recPhi.dau_0_kaonID[iEnt]>.05))continue;
            if(!(recPhi.dau_1_kaonID[iEnt]>.05))continue;
            tree_2 -> Fill();
        }
    }

    fOut.cd();
    tree_1 -> Print();
    tree_2 -> Print();
    tree_1 -> Write();
    tree_2 -> Write();
    fOut.Close();

    return;
}
