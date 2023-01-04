#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyPhiRec.h"
void Save2tMatched(){
    TChain *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile("/home/belle2/zhai/B0_phi_KS0/Phi_cut_selection_Combo/Combo_phi_selection.root");
    
    int Entries = phi_Rec -> GetEntries();

    MyPhiRec rec;
    rec.setBranchAddresses(phi_Rec);

    TFile fOut("Combo_phi_signal.root","recreate");

    TTree *tMatched = new TTree("tMatched","");
    double M_sig;
    tMatched -> Branch("M",&M_sig,"M/D");

    for(int i=0; i < Entries; i++){
        phi_Rec -> GetEntry(i);
        for(int iEnt = 0; iEnt<rec.nCand; iEnt++){
            if(rec.isSignal[iEnt]==1){
                M_sig = rec.M[iEnt];
                tMatched -> Fill();
            }
        }//candidates
    }//phi

    fOut.cd();
    tMatched -> Print();
    tMatched -> Write();

    fOut.Close();


    return;
}
