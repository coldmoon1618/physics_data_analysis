#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyKsRec.h"
void Ks_save(){
    TString fileName("Data_Ks_134ipb.root");

    TChain  *Ks_Rec = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec *recKs  = new MyKsRec();
    recKs   -> setBranchAddresses(Ks_Rec);
    int Entries = Ks_Rec -> GetEntries();
    cout<<Entries<<" events"<<endl;

    TString fOutName("Ks_tree.root");
    TFile fOut(fOutName,"recreate");
    TTree *tree_1 = new TTree("tree_1","");
    TTree *tree_2 = new TTree("tree_2","");

    double M_Ks;

    tree_1 -> Branch("M", & M_Ks, "M/D");
    tree_2 -> Branch("M", & M_Ks, "M/D");
    for(int i=0; i < Entries; i++){
        Ks_Rec  -> GetEntry(i);

        //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs->nCand; iEnt++){
            M_Ks = recKs->M[iEnt];
            tree_1 -> Fill();
            if(!(recKs->chiProb[iEnt]>0))continue;
            if(!(recKs->goodBelleKshort[iEnt]>.5))continue;
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
