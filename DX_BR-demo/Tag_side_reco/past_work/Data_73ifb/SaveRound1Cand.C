#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB_Tag.h"
#include "MyB_cand.h"
void SaveRound1Cand(
TString fileReco="Data_Rec.root", TString fOutName="Round1_cand.root", 
double minProb = 0.01, double minMbc = 5.24) {

    cout<<"Input: "<<fileReco<<endl;
    cout<<"Outout: "<<fOutName<<endl;

    TFile fOut(fOutName,"recreate");

    TChain      *B_Tag  = new TChain("B_Tag");
    B_Tag  -> AddFile(fileReco);
    MyB_Tag     *tagB   = new MyB_Tag();
    tagB    -> setBranchAddresses(B_Tag);
    
    TTree       *B_cand = new TTree("B_cand","");
    MyB_cand     *candB  = new MyB_cand();
    candB   -> branches(B_cand);
    
    int Entries = B_Tag -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    double Mbc, deltaE, decMode, SigProb;

    for(int i=0; i < Entries; i++)
    {
        B_Tag      -> GetEntry(i);

        bool has_B_tag= false;
        double maxSigProb = -9;
        int  ind_best_cand= -999;   //var[ind] gives seg fault if -999 used
                                    //whereas -9 could be real index
        for(int iEnt=0; iEnt<tagB->nCand; iEnt++){

            Mbc      = tagB->Mbc[iEnt];
            deltaE   = tagB->deltaE[iEnt];
            SigProb  = tagB->SigProb[iEnt];
            decMode  = tagB->decMode[iEnt];

            if(SigProb<=minProb)continue;        
            if(isnan(Mbc)|| isnan(deltaE))continue;
            if(Mbc<=minMbc)continue;

            if(SigProb>maxSigProb){
                maxSigProb = SigProb;
                ind_best_cand = iEnt;
                has_B_tag = true;//found the best candidate
            }
        }//nth candidate

        if(!has_B_tag)continue;

        int iEnt = ind_best_cand;
        candB->Mbc                      = tagB->Mbc[iEnt];
        candB->deltaE                   = tagB->deltaE[iEnt];
        candB->decMode                  = tagB->decMode[iEnt];
        candB->SigProb                  = tagB->SigProb[iEnt];

        B_cand -> Fill();
    }//events
    //////////////////////////////////////////////////

    fOut.cd();
    B_cand -> Write();
    B_cand -> Print();
    fOut.Close();

    return;
}
