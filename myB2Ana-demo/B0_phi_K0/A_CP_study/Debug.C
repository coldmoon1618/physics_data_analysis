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
void Debug() {

    TChain     *tree = new TChain("tree");
    tree -> AddFile("FitScripts/AcpFitParams.root");
    int trueFlav;
    tree -> SetBranchAddress("trueFlav", &trueFlav);
    int recoFlav;
    tree -> SetBranchAddress("recoFlav", &recoFlav);
    float r;
    tree -> SetBranchAddress("r",        &r);
    float trueDeTau;
    tree -> SetBranchAddress("trueDeTau",&trueDeTau);
    int   rBins;
    tree -> SetBranchAddress("rBins",    &rBins);

    const int nBins = 7;

    int Entries = tree -> GetEntries();
    cout<<Entries<<" candidates"<<endl;
    
    const int nDeT = 10;
    int    nRB0[nDeT];
    int    nWB0[nDeT];
    int    nRB0bar[nDeT];
    int    nWB0bar[nDeT];
    double w_val[nDeT]; 
    double w_err[nDeT];
    double t_mid[nDeT];
    double t_tot[nDeT];
    double t_avg[nDeT];

    for(int j=0; j<nDeT; j++){
        nRB0[j]=0;
        nWB0[j]=0;
        nRB0bar[j]=0;
        nWB0bar[j]=0;
        t_mid[j] = 2.*j-9.;
        t_tot[j] = 0.;
    }

    TH1F *h1 = new TH1F("h1","h1",nDeT,-10,10);
    
    for(int i=0; i<Entries; i++)
    {
        tree -> GetEntry(i);

        if(abs(trueDeTau)>10)continue;
        //if(rBins!=7)continue;
        //if(r<0.7)continue;
        if(r>0.2)continue;
        int j = floor((trueDeTau+10)/(20./nDeT));
        t_tot[j]+=trueDeTau;

        if      (trueFlav== 1){
            if(trueFlav==recoFlav)  nRB0[j]++;
            else                    nWB0[j]++;
        }else if(trueFlav==-1){
            if(trueFlav==recoFlav)  nRB0bar[j]++;
            else                    nWB0bar[j]++;
        }
    }//candidates
    for(int j=0; j<nDeT; j++){
        double wB0   = nWB0   [j]*1.0/(nWB0   [j]+nRB0   [j]);
        double wB0bar= nWB0bar[j]*1.0/(nWB0bar[j]+nRB0bar[j]);
        w_val[j]     = .5*(wB0+wB0bar);
        double wB0_e     = wB0      *sqrt(1./nWB0[j]+1./(nRB0[j]+nWB0[j]));
        double wB0bar_e  = wB0bar   *sqrt(1./nWB0bar[j]+1./(nRB0bar[j]+nWB0bar[j]));
        w_err[j] = .5 * sqrt( wB0_e * wB0_e + wB0bar_e * wB0bar_e);
        t_avg[j]=t_tot[j]/(nRB0[j]+nWB0[j]+nRB0bar[j]+nWB0bar[j]);
        //cout<<w_val[j]<<"+/-"<<w_err[j]<<endl;
        cout<<t_avg[j]<<endl;
        cout<<1-2.*w_val[j]<<"+/-"<<2.*w_err[j]<<endl;
    }

    TCanvas *c1 = new TCanvas("c1","c1",800,400);
    auto gr = new TGraphErrors(nDeT,t_avg,w_val,0,w_err);
    gr -> SetLineColor(kBlue);
    gr -> SetLineWidth(2);
    gr -> Draw("AP");

    /*
    Work in progress, need to divide t bins with entries: fine in center, board on sides
    */
}
