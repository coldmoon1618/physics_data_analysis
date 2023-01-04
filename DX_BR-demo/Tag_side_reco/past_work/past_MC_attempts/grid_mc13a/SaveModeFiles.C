#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB_Tag.h"
#include "MyB_cand.h"
void SaveModeFiles(
int myMode = 0,
TString fileReco="MC_Rec.root",TString mcType = "charged",  
double minProb = 0.01, double minMbc = 5.24) {

    TString fOutName=mcType+Form("_output/Cand_mode_%02d.root",myMode);
    cout<<"Input: "<<fileReco<<endl;
    cout<<"Outout: "<<fOutName<<endl;

    TChain      *B_Tag  = new TChain("B_Tag");
    B_Tag  -> AddFile(fileReco);
    MyB_Tag     *tagB   = new MyB_Tag();
    tagB    -> setBranchAddresses(B_Tag);
    
    TTree       *B_cand = new TTree("B_cand","");
    MyB_cand     *candB  = new MyB_cand();
    candB   -> branches(B_cand);
    
    int Entries = B_Tag -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    if(mcType=="mixed")  {candB->isMixed = 1;}else{candB->isMixed = -1;}
    if(mcType=="charged"){candB->isCharg = 1;}else{candB->isCharg = -1;}
    if(mcType=="uubar")  {candB->isUUbar = 1;}else{candB->isUUbar = -1;}
    if(mcType=="ddbar")  {candB->isDDbar = 1;}else{candB->isDDbar = -1;}
    if(mcType=="ssbar")  {candB->isSSbar = 1;}else{candB->isSSbar = -1;}
    if(mcType=="ccbar")  {candB->isCCbar = 1;}else{candB->isCCbar = -1;}
    if(mcType=="uubar"||mcType=="ddbar"||mcType=="ssbar"||mcType=="ccbar")  {candB->isQQbar = 1;}else{candB->isQQbar = -1;}

    double Mbc, deltaE, decMode, SigProb;
    int    counter = 0;

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
            
            if(decMode!=myMode*1.)continue;
            if(SigProb<=minProb)continue;        
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

        candB->isSignal                 = tagB->isSignal[iEnt];
        candB->isSigAccMiss             = tagB->isSigAccMiss[iEnt];
        candB->isSigAccMissGamma        = tagB->isSigAccMissGamma[iEnt];
        candB->isSigAccWrongFSP         = tagB->isSigAccWrongFSP[iEnt];
        counter++;

        B_cand -> Fill();
    }//events
    //////////////////////////////////////////////////

    if(counter>0){
        TFile fOut(fOutName,"recreate");
        fOut.cd();
        B_cand -> Write();
        B_cand -> Print();
        fOut.Close();
    }

    return;
}
