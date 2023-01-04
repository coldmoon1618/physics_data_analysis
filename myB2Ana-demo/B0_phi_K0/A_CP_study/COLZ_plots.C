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
void COLZ_plots() {

    TChain     *tree = new TChain("tree");
    tree -> AddFile("FitScripts/AcpFitParams.root");
    int trueFlav;
    tree -> SetBranchAddress("trueFlav", &trueFlav);
    int recoFlav;
    tree -> SetBranchAddress("recoFlav", &recoFlav);
    float r;
    tree -> SetBranchAddress("r",        &r);
    int rBins;
    tree -> SetBranchAddress("rBins",    &rBins);
    float trueDeTau;
    tree -> SetBranchAddress("trueDeTau",&trueDeTau);

    int Entries = tree -> GetEntries();
    cout<<Entries<<" candidates"<<endl;

    const int nBins = 7;
     
    array<double ,nBins> binMin;
    array<double ,nBins> binMax;
    binMin={0.00, .100, .250, .500, .625, .750, .825};
    binMax={.100, .250, .500, .625, .750, .825, 1.00};
    array<int   ,nBins> nRB0     = {0,0,0,0,0,0,0};
    array<int   ,nBins> nRB0bar  = {0,0,0,0,0,0,0};
    array<int   ,nBins> nWB0     = {0,0,0,0,0,0,0};
    array<int   ,nBins> nWB0bar  = {0,0,0,0,0,0,0};
    array<int   ,nBins> nB0Tag   = {0,0,0,0,0,0,0};
    array<int   ,nBins> nB0barTag= {0,0,0,0,0,0,0};
    array<double,nBins> mu;
    int nTaggedB0 = 0;
    int nTaggedB0bar = 0;
    array<double,nBins> B0Eff;
    array<double,nBins> B0barEff;
    array<double,nBins> B0EffErr;
    array<double,nBins> B0barEffErr;
    array<double,nBins> effRatio;
    array<double,nBins> effRatioErr;
    array<double,nBins> w;
    array<double,nBins> dw;
    // Using xErr \approx sqrt(x), error propagation:
    // W/(W+R) *sqrt( 1/W + 1/(W+R))
    array<double,nBins> wErr;
    array<double,nBins> rMea;//r measured from 2, instead of BDT output
    array<double,nBins> rErr;
    array<double,nBins> rAvg;
    array<double,nBins> rMid;
    array<double,nBins> rTot    = {.0,.0,.0,.0,.0,.0,.0};
    array<int   ,nBins> rEnt    = { 0, 0, 0, 0, 0, 0, 0};

    int myB0W = 0;
    int myB0R = 0;
    int myB0barW = 0;
    int myB0barR = 0;
    const int nDeT = 30;
    const int nR   = 15;

    TH2F *h1 = new TH2F("h1","h1",40,-10,10,20,0.825,1.);
    TH2F *h2 = new TH2F("h2","h2",40,-10,10,20,0.,.1);
    
    TH2F *H[nBins];
    for(int j=0; j<7; j++){
        rMid[j] = .5*(binMin[j]+binMax[j]);
        H[j] = new TH2F(Form("H%i",j+1),Form("bin%i",j+1),nDeT,-10,10,nR,binMin[j],binMax[j]);
    }

    for(int i=0; i<Entries; i++)
    {
        tree -> GetEntry(i);

        if(abs(trueDeTau)>10)continue;
        int j = rBins-1;

        H[j]->Fill(trueDeTau,r);

        rEnt[j] ++;
        rTot[j] +=r;
        if      (trueFlav== 1){
            nB0Tag[j]++;
            nTaggedB0++;
            if(trueFlav==recoFlav)  nRB0[j]++;
            else                    nWB0[j]++;
        }else if(trueFlav==-1){
            nB0barTag[j]++;
            nTaggedB0bar++;
            if(trueFlav==recoFlav)  nRB0bar[j]++;
            else                    nWB0bar[j]++;
        }

        if(r>0.825){
            if      (trueFlav== 1){
                if(trueFlav==recoFlav)  myB0R++;
                else                    myB0W++;
            }else if(trueFlav==-1){
                if(trueFlav==recoFlav)  myB0barR++;
                else                    myB0barW++;
            }
            h1 -> Fill(trueDeTau,r);
        }else if(r<0.1){
            h2 -> Fill(trueDeTau,r);
        }

    }//candidates

    gStyle -> SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",800,400);
    for(int j=0; j<7; j++){
        setStyleTH2F(H[j],"mcDeltaTau","r_{FBDT}",Form("bin%i",j+1));
        H[j] -> Draw("col");
        c1 -> SaveAs(Form("Dep%i.pdf",j+1));
    }
    for(int j=6; j<7; j++){
        c1 -> SetLogz();
        c1 -> SaveAs(Form("Dep%i_log.pdf",j+1));
    }

}
