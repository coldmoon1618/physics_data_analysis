#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB_Tag.h"
#include "MyB_cand.h"

#include "MyDecChain.h"
void SaveSingleCand(TString fileReco="out.root", TString fOutName="out_cand.root", TString mcType ="charged", float deE_width=999) {

    cout<<"Input: "<<fileReco<<endl;
    cout<<"Outout: "<<fOutName<<endl;
    cout<<"deltaE: "<<deE_width<<endl;

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

    if(mcType=="mixed")  {candB->isMixed = 1;}else{candB->isMixed = -1;}
    if(mcType=="charged"){candB->isCharg = 1;}else{candB->isCharg = -1;}
    if(mcType=="uubar")  {candB->isUUbar = 1;}else{candB->isUUbar = -1;}
    if(mcType=="ddbar")  {candB->isDDbar = 1;}else{candB->isDDbar = -1;}
    if(mcType=="ssbar")  {candB->isSSbar = 1;}else{candB->isSSbar = -1;}
    if(mcType=="ccbar")  {candB->isCCbar = 1;}else{candB->isCCbar = -1;}
    if(mcType=="uubar"||mcType=="ddbar"||mcType=="ssbar"||mcType=="ccbar")  {candB->isQQbar = 1;}else{candB->isQQbar = -1;}

    for(int i=0; i < Entries; i++)
    {
        B_Tag      -> GetEntry(i);
        //Y4S_chain   -> GetEntry(i);

        bool has_B_tag= false;
        double maxSigProb = -9;
        int  ind_best_cand= -9;
        for(int iEnt=0; iEnt<tagB->nCand; iEnt++){

            candB->Mbc                      = tagB->Mbc[iEnt];
            candB->deltaE                   = tagB->deltaE[iEnt];
            if(isnan(candB->Mbc)|| isnan(candB->deltaE))continue;
            if(candB->Mbc<=5.23 || abs(candB->deltaE)>=deE_width)continue;

            candB->SigProb                  = tagB->SigProb[iEnt];

            if(candB->SigProb<=0.01)continue;        
            if(candB->SigProb>maxSigProb){
                maxSigProb = candB->SigProb;
                ind_best_cand = iEnt;
                has_B_tag = true;//found the best candidate
            }
        }//nth candidate

        if(!has_B_tag)continue;

        int iEnt = ind_best_cand;
        if(mcType=="mixed"||mcType=="charged"||mcType=="uubar"||mcType=="ddbar"||mcType=="ssbar"||mcType=="ccbar"){
        candB->isSignal                 = tagB->isSignal[iEnt];
        }else{ candB->isSignal = -1; }
        candB->Mbc                      = tagB->Mbc[iEnt];
        candB->deltaE                   = tagB->deltaE[iEnt];
        candB->SigProb                  = tagB->SigProb[iEnt];
        candB->M                        = tagB->M[iEnt];
        candB->p                        = tagB->p[iEnt];
        candB->pstar                    = tagB->pstar[iEnt];

        candB->cosTBTO                  = tagB->cosTBTO[iEnt];
        candB->cosTBz                   = tagB->cosTBz[iEnt];
        candB->thrustOm                 = tagB->thrustOm[iEnt];
        candB->thrustBm                 = tagB->thrustBm[iEnt];
        candB->R2                       = tagB->R2[iEnt];

        candB->decMode                  = tagB->decMode[iEnt];
        candB->charge                   = tagB->charge[iEnt];
        candB->weight                   = tagB->weight;


        B_cand -> Fill();
    }//events
    //////////////////////////////////////////////////

    fOut.cd();
    B_cand -> Write();
    B_cand -> Print();
    fOut.Close();

    return;
}
