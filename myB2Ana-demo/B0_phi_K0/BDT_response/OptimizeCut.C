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
    float Mbc, response, isBphiK;
    res_BDT -> SetBranchAddress("Mbc",&Mbc);
    res_BDT -> SetBranchAddress("response",&response);
    res_BDT -> SetBranchAddress("isBphiK",&isBphiK);

    int Entries = res_BDT -> GetEntries();
    
    const int n = 20;
    int nSig[n];
    int nBkg[n];
    int nTotSig = 0;
    int nTotBkg = 0;
    float cut[n];
    float eff[n];
    float bkg[n];
    float rej[n];
    float Sig[n];

    for(int i = 0; i<n; i++){
        nSig[i]=0;
        nBkg[i]=0;
        cut[i]=-1+(i+.5)*2./n;
    }
     
    for(int i=0; i<Entries; i++)
    {
        res_BDT -> GetEntry(i);

        if(Mbc<5.27||Mbc>5.29)continue;
        if(isBphiK>.5)nTotSig++;else nTotBkg++;
        for(int iCut = 0; iCut<n; iCut++){
            if(response<cut[iCut])continue;
            if(isBphiK>.5){
                nSig[iCut]++;
            }else{
                nBkg[iCut]++;
            }
        }//n different cuts
    }//candidates

    for(int iCut = 0; iCut<n; iCut++){
        eff[iCut] = .5*nSig[iCut]/(.5*nTotSig);
        bkg[iCut] = .5*nBkg[iCut]/(.5*nTotBkg);
        rej[iCut] = 1-bkg[iCut];
        Sig[iCut] = nSig[iCut]/sqrt(nSig[iCut]+nBkg[iCut]);
    }
    int indMaxSig = -1;
    float maxSig = -1;
    for(int iCut = 0; iCut<n; iCut++){
        if(Sig[iCut]>maxSig){
            indMaxSig=iCut;
            maxSig = Sig[iCut];
        }
    }
    cout<<"Max S/sqrt(S+B)="<<Sig[indMaxSig]<<" at cut="<<cut[indMaxSig]<<endl;
    cout<<"nTotSig="<<nTotSig<<endl;
    cout<<"nTotBkg="<<nTotBkg<<endl;
   
    TCanvas *c1 = new TCanvas("c1","c1",500,500);
    auto gr_ROC = new TGraphErrors(n,eff,rej);
    gr_ROC->SetMarkerColor(kBlack);
    gr_ROC->SetLineColor(kBlack);
    gr_ROC->SetMarkerStyle(21);
    TMultiGraph *mg1 = new TMultiGraph();
    mg1->Add(gr_ROC,"APL");
    mg1->SetTitle("ROC curve; #varepsilon_{sig}; 1-#varepsilon_{bkg}");
    mg1->Draw("a");

    auto gr_eff = new TGraphErrors(n,cut,eff);
    gr_eff->SetMarkerColor(kGreen+2);
    gr_eff->SetLineColor(kGreen+2);
    gr_eff->SetMarkerStyle(21);

    auto gr_bkg = new TGraphErrors(n,cut,bkg);
    gr_bkg->SetMarkerColor(kRed+1);
    gr_bkg->SetLineColor(kRed+1);
    gr_bkg->SetMarkerStyle(21);

    TCanvas *c2 = new TCanvas("c2","c2",500,500);
    TMultiGraph *mg2 = new TMultiGraph();
    mg2->Add(gr_eff,"APL");
    mg2->Add(gr_bkg,"APL");
    mg2->SetTitle("Green sig, Red bkg, both efficiency; cut at response; #varepsilon");
    mg2->Draw("a");

    auto gr_Sig = new TGraphErrors(n,cut,Sig);
    gr_Sig->SetMarkerColor(kBlack);
    gr_Sig->SetLineColor(kBlack);
    gr_Sig->SetMarkerStyle(21);

    TCanvas *c3 = new TCanvas("c3","c3",500,500);
    TMultiGraph *mg3 = new TMultiGraph();
    mg3->Add(gr_Sig,"APL");
    mg3->SetTitle("S/#sqrt{S+B} optimization; cut at response; S/#sqrt{S+B}");
    mg3->Draw("a");

}
