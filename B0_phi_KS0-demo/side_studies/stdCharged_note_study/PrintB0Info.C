#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0Rec.h"
#include "MyPhiRec.h"
#include "MyKsRec.h"
void PrintB0Info(){
    TChain *B0_Rec = new TChain("B0_Rec");
    B0_Rec -> AddFile("/home/belle2/zhai/B0_phi_KS0/Full_selections/SigMC/SigMC_B0_Rec_20k.root");
    
    int Entries = B0_Rec -> GetEntries();

    MyB0Rec rec;
    rec.setBranchAddresses(B0_Rec);
    MyPhiRec recPhi;
    recPhi.setBranchAddresses(B0_Rec, "phi_");
    MyKsRec recKs;
    recKs.setBranchAddresses(B0_Rec, "Ks_");

    for(int i=0; i < 5; i++){
    //for(int i=0; i < Entries; i++){
        B0_Rec -> GetEntry(i);
        cout<<"event "<<i<<" ";
        cout<<rec.nCand<<" ";
        cout<<recPhi.nCand<<" ";
        cout<<recKs.nCand<<" ";
        cout<<endl;
        for(int iEnt = 0; iEnt<rec.nCand; iEnt++){
            if(rec.isSignal[iEnt]==1){
                cout<<"Mbc="<<rec.Mbc[iEnt]<<endl;
                cout<<"M phi="<<recPhi.M[iEnt]<<endl;
                cout<<"M Ks="<<recKs.M[iEnt]<<endl;
                cout<<endl;
            }
        }//candidates
    }//phi

    return;
}
