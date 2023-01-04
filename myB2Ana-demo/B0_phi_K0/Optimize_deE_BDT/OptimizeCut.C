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
void OptimizeCut() {

    TChain *res_BDT = new TChain("res_BDT");
    res_BDT -> AddFile("FullMC_resp.root");
    float Mbc, deltaE, response, isBphiK;
    res_BDT -> SetBranchAddress("Mbc",      &Mbc);
    res_BDT -> SetBranchAddress("deltaE",   &deltaE);
    res_BDT -> SetBranchAddress("response", &response);
    res_BDT -> SetBranchAddress("isBphiK",  &isBphiK);

    int Entries = res_BDT -> GetEntries();
    
    const int nRes = 20;
    const int nDeE = 50;
    int n = nRes*nDeE;
    int nSig[n];
    int nBkg[n];
    int nTotSig = 0;
    int nTotBkg = 0;
    float res_cut[n];
    float deE_cut[n];
    float eff[n];
    float bkg[n];
    float rej[n];
    float Sig[n];
    const float lowRes = -1;
    const float widRes = 2.0;
    const float lowDeE = 0.02;
    const float widDeE = 0.18;

    for(int i = 0; i<nRes; i++){
        for(int j = 0; j<nDeE; j++){
            int k = nDeE*i+j;
            res_cut[k]=lowRes+(i+.5)*(widRes/(nRes));
            deE_cut[k]=lowDeE+(j+.5)*(widDeE/(nDeE));
            nSig[k]=0;
            nBkg[k]=0;
            
            //cout<<"res ="<<i<<", deE ="<<j<<", k="<< k <<endl;
        }
    }//Assign cuts

    for(int i=0; i<Entries; i++)
    {
        res_BDT -> GetEntry(i);

        if(Mbc<5.27||Mbc>5.29)continue;
        if(isBphiK>.5)nTotSig++;else nTotBkg++;
        for(int k = 0; k<n; k++){
            if(response     < res_cut[k])continue;
            if(abs(deltaE)  > deE_cut[k])continue;
            if(isBphiK>.5){
                nSig[k]++;
            }else{
                nBkg[k]++;
            }
        }//k 
    }//candidates

    for(int k = 0; k<n; k++){
        eff[k] = .5*nSig[k]/(.5*nTotSig);
        bkg[k] = .5*nBkg[k]/(.5*nTotBkg);
        rej[k] = 1-bkg[k];
        Sig[k] = nSig[k]*1.1/(1.1*sqrt(nSig[k]+nBkg[k]));

        //cout<<" res_cut="<<res_cut[k]<<" deE_cut="<<deE_cut[k]<<endl;
        //cout<<"nSig="<<nSig[k]<<", nBkg= "<<nBkg[k]<<", significance="<<Sig[k]<<endl;
    }
    int kMaxSig = -1;
    float maxSig = -1;
    for(int k = 0; k<n; k++){
        if(Sig[k]>maxSig){
            kMaxSig=k;
            maxSig=Sig[k];
        }
    }
    cout<<"nTotSig="<<nTotSig<<endl;
    cout<<"nTotBkg="<<nTotBkg<<endl;
    cout<<"Max S/sqrt(S+B)="<<Sig[kMaxSig]<<" at res_cut="<<res_cut[kMaxSig]<<" and deE_cut="<<deE_cut[kMaxSig]<<endl;
    cout<<"eff="<<eff[kMaxSig]<<", rej="<<rej[kMaxSig]<<endl;
    
    int nPassSig = 0;
    int nPassBkg = 0; 
    for(int i=0; i<Entries; i++)
    {
        res_BDT -> GetEntry(i);

        if(Mbc<5.27||Mbc>5.29)continue;
        if(response     < 0)continue;
        if(abs(deltaE)  > .025)continue;
        if(isBphiK>.5)nPassSig++;else nPassBkg++;
    }//candidates

    cout<<"At BDT>0 and abs(deE)<0.025, eff="<<nPassSig*1.0/nTotSig<<", rej="<<1-1.0*nPassBkg/nTotBkg<<endl;
    cout<<"Significance="<<nPassSig/sqrt(nPassSig+nPassBkg)<<endl<<endl;
    cout<<"nPassSig="<<nPassSig<<endl;

    TCanvas *c1 = new TCanvas("c1","c1",500,500);

    auto gr1 = new TGraph2D(nRes*nDeE,res_cut,deE_cut,Sig); 
    gr1 -> SetTitle("Significance; BDT response; deltaE width; Significance");
    gr1 -> Draw("surf1");
   
   /* 
    c1 -> cd(2);
    auto gr2 = new TGraph2D(nRes*nDeE,res_cut,deE_cut,eff); 
    gr2 -> SetTitle("Efficiency; BDT response; deltaE width; Efficiency");
    gr2 -> Draw("surf1");
    
    c1 -> cd(3);
    auto gr3 = new TGraph2D(nRes*nDeE,res_cut,deE_cut,rej); 
    gr3 -> SetTitle("Significance; BDT response; deltaE width; Significance");
    gr3 -> Draw("surf1");
    */

}
