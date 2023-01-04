#include <iostream>
#include <fstream>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <TMath.h>
#include <TChain.h>

#include "MyB0Cand.h"
#include "MyPhiCand.h"
#include "MyKsCand.h"

#include "MySetStyle.h"
void SaveAcpParams(TString rBinType = "b2") {

    TChain     *B0_cand = new TChain("res_BDT");
    B0_cand -> AddFile("SigMC_resp.root");
    MyB0Cand    *candB  = new MyB0Cand();
    candB   -> setBranchAddresses(B0_cand);
    MyPhiCand   *cBPhi  = new MyPhiCand();
    cBPhi   -> setBranchAddresses(B0_cand, "phi_");
    MyKsCand    *cBKs0  = new MyKsCand();
    cBKs0   -> setBranchAddresses(B0_cand, "Ks_");
    float response;
    B0_cand -> SetBranchAddress("response",&response);

    int Entries = B0_cand -> GetEntries();
    cout<<Entries<<" candidates"<<endl;

    TTree *tree = new TTree("tree","");
    //deltaT
    float trueDeTau = -999;
    tree -> Branch("trueDeTau", &trueDeTau);
    float recoDeT   = -999;
    tree -> Branch("recoDeT",   &recoDeT);
    float recoDeTErr   = -999;
    tree -> Branch("recoDeTErr",   &recoDeTErr);
    //flavor
    int trueFlav    = 0;
    tree -> Branch("trueFlav",  &trueFlav);
    int recoFlav    = 0;
    tree -> Branch("recoFlav",  &recoFlav);
    float r  = 0;
    tree -> Branch("r",         &r);
    //rBins
    int rBins = 0;
    tree -> Branch("rBins",     &rBins);
    tree -> Branch("response",  &response);
   
    int nTagged = 0;
    int nSignal = 0;
     
    array<float,7> binMax;
    if(rBinType=="b2"){
        cout<<"Belle II binning"<<endl;
        binMax={.100, .250, .500, .625, .750, .825, 1.0};
    }else{
        cout<<"Belle binning"<<endl;
        binMax={.100, .230, .400, .570, .710, .880, 1.0};
    }

    for(int i=0; i<Entries; i++)
    {
        B0_cand -> GetEntry(i);

        //Reinitialize
        trueDeTau   = -999;
        recoDeT     = -999;
        trueFlav    = -999;
        recoFlav    = -999;

        //Get candidate values
        if(candB->isSignal!=1)continue;
        //if(abs(candB->deltaE)>=0.025)continue;//cut already made at save-cand stage
        //if(response<=-.1)continue;
        nSignal++;
        if(!isnan(candB->mcDeltaTau)){
            trueDeTau   = candB->mcDeltaTau;
        }        
        if(!isnan(candB->DeltaT)){
            recoDeT     = candB->DeltaT;
            recoDeTErr  = candB->DeltaTErr;
        }        
        trueFlav = candB->MCflvrOtherB;
        if(isnan(trueFlav)||abs(trueFlav)!=1)continue;
        float tagFlv    = candB -> flvrTag_FBDT;
        if(isnan(tagFlv))continue;
        if(tagFlv>0){
            recoFlav    = 1;
        }else{
            recoFlav    = -1;
        }
        nTagged++;
        r               = abs(tagFlv);
        if      (r <= binMax[0]){
            rBins       = 1;
        }else if(r <= binMax[1]){
            rBins       = 2;
        }else if(r <= binMax[2]){
            rBins       = 3;
        }else if(r <= binMax[3]){
            rBins       = 4;
        }else if(r <= binMax[4]){
            rBins       = 5;
        }else if(r <= binMax[5]){
            rBins       = 6;
        }else{
            rBins       = 7;
        }

        tree -> Fill();
    }//candidates
    
    cout<<nSignal<<" matched, "<<nTagged<<" tagged,"<<nTagged*1.1/(nSignal*1.1)*100<<"\% tagged"<<endl;

    TFile outFile("FitScripts/AcpFitParams.root","recreate");
    tree -> Write();

}
