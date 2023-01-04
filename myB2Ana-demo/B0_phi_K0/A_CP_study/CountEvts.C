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
void CountEvts() {

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

    int Entries = tree -> GetEntries();
    cout<<Entries<<" candidates"<<endl;
    
    const int nBin = 7;
    const int nDeT = 10;
    double tMax[nDeT] = {-2.5,-1.5,-1,-.5,0,.5,1,1.5,2.5,10};
    int    nRB0     [nBin][nDeT];
    int    nWB0     [nBin][nDeT];
    int    nRB0bar  [nBin][nDeT];
    int    nWB0bar  [nBin][nDeT];
    unsigned int nTotRB0        [nBin];
    unsigned int nTotWB0        [nBin];
    unsigned int nTotRB0bar     [nBin];
    unsigned int nTotWB0bar     [nBin];
    double w_val    [nBin][nDeT]; 
    double w_err    [nBin][nDeT];
    double t_tot    [nBin][nDeT];
    double t_avg    [nBin][nDeT];

    for(int iBin=0; iBin<nBin; iBin++){
        nTotRB0    [iBin] = 0;
        nTotWB0    [iBin] = 0;
        nTotRB0bar [iBin] = 0;
        nTotWB0bar [iBin] = 0;
        for(int iDeT=0; iDeT<nDeT; iDeT++){
            nRB0    [iBin][iDeT] = 0;
            nWB0    [iBin][iDeT] = 0;
            nRB0bar [iBin][iDeT] = 0;
            nWB0bar [iBin][iDeT] = 0;
            t_tot   [iBin][iDeT] = 0.;
        }
    }
    
    for(int i=0; i<Entries; i++)
    {
        tree -> GetEntry(i);

        int iBin = rBins-1;
        int iDeT = -9;

        if(abs(trueDeTau)>10)continue;
        if      (trueDeTau<tMax[0]){
            iDeT = 0;
        }else if(trueDeTau<tMax[1]){
            iDeT = 1;
        }else if(trueDeTau<tMax[2]){
            iDeT = 2;
        }else if(trueDeTau<tMax[3]){
            iDeT = 3;
        }else if(trueDeTau<tMax[4]){
            iDeT = 4;
        }else if(trueDeTau<tMax[5]){
            iDeT = 5;
        }else if(trueDeTau<tMax[6]){
            iDeT = 6;
        }else if(trueDeTau<tMax[7]){
            iDeT = 7;
        }else if(trueDeTau<tMax[8]){
            iDeT = 8;
        }else if(trueDeTau<tMax[9]){
            iDeT = 9;
        }
        if(iDeT<0){continue;cout<<"Something wrong assigning DeT bin."<<endl;}

        t_tot[iBin][iDeT]+=trueDeTau;

        if      (trueFlav== 1){
            if(trueFlav==recoFlav)  nRB0    [iBin][iDeT]++;
            else                    nWB0    [iBin][iDeT]++;
        }else if(trueFlav==-1){
            if(trueFlav==recoFlav)  nRB0bar [iBin][iDeT]++;
            else                    nWB0bar [iBin][iDeT]++;
        }
    }//candidates
    for(int iBin=0; iBin<nBin; iBin++){
        for(int iDeT=0; iDeT<nDeT; iDeT++){
            nTotRB0     [iBin]+=nRB0    [iBin][iDeT];
            nTotWB0     [iBin]+=nWB0    [iBin][iDeT];
            nTotRB0bar  [iBin]+=nRB0bar [iBin][iDeT];
            nTotWB0bar  [iBin]+=nWB0bar [iBin][iDeT];
        }
    }
    for(int iBin=0; iBin<nBin; iBin++){
        for(int iDeT=0; iDeT<nDeT;  iDeT++){
            double wB0   = nWB0   [iBin][iDeT]*1.0/(nWB0   [iBin][iDeT]+nRB0   [iBin][iDeT]);
            double wB0bar= nWB0bar[iBin][iDeT]*1.0/(nWB0bar[iBin][iDeT]+nRB0bar[iBin][iDeT]);
            w_val[iBin][iDeT]     = .5*(wB0+wB0bar);
            double wB0_e     = wB0      *sqrt(1./nWB0[iBin][iDeT]+1./(nRB0[iBin][iDeT]+nWB0[iBin][iDeT]));
            double wB0bar_e  = wB0bar   *sqrt(1./nWB0bar[iBin][iDeT]+1./(nRB0bar[iBin][iDeT]+nWB0bar[iBin][iDeT]));
            w_err[iBin][iDeT] = .5 * sqrt( wB0_e * wB0_e + wB0bar_e * wB0bar_e);
            t_avg[iBin][iDeT]=t_tot[iBin][iDeT]/(nRB0[iBin][iDeT]+nWB0[iBin][iDeT]+nRB0bar[iBin][iDeT]+nWB0bar[iBin][iDeT]);
        }
        //cout<<endl;
    }

    TCanvas *c1 = new TCanvas("c1","c1",500,400);
    double w1_val[nBin] ={0.4819,0.4206,0.3296,0.2562,0.1774,0.1381,0.0458}; 
    double w1_err[nBin] ={0.0098,0.0090,0.0079,0.0105,0.0092,0.0114,0.0057};

    auto mg = new TMultiGraph();
    for(int iBin=0; iBin<nBin; iBin++){
        int myBin = iBin+1;
        auto gr = new TGraphErrors(nDeT,t_avg[iBin],w_val[iBin],0,w_err[iBin]);
        Double_t X[2]={-10,10};
        Double_t Y[2]={w1_val[iBin],w1_val[iBin]};
        Double_t up[2] ={w1_val[iBin]+w1_err[iBin],w1_val[iBin]+w1_err[iBin]};
        Double_t low[2]={w1_val[iBin]-w1_err[iBin],w1_val[iBin]-w1_err[iBin]};
        auto line = new TGraph(2,X,Y);
        line-> SetLineWidth(2);
        line-> SetLineColor(kGray);
        auto upper = new TGraph(2,X,up);
        upper-> SetLineWidth(2);
        upper-> SetLineColor(kGray);
        upper-> SetLineStyle(9);
        auto lower = new TGraph(2,X,low);
        lower-> SetLineWidth(2);
        lower-> SetLineColor(kGray);
        lower-> SetLineStyle(9);
        gr -> SetLineColor(kViolet+iBin);
        gr -> SetLineWidth(2);
        gr -> Draw("AP");
        gr -> GetXaxis() -> SetLimits(-10,10);
        mg -> GetXaxis() -> SetTitle("#Delta t");
        mg -> GetYaxis() -> SetTitle("average w");
        mg -> Add(line);
        mg -> Add(upper);
        mg -> Add(lower);
        mg -> Add(gr,"AP");
        TCanvas *c2 = new TCanvas("c2","c2",500,400);
        auto mg2 = new TMultiGraph();
        mg2 -> Add(line);
        mg2 -> Add(upper);
        mg2 -> Add(lower);
        mg2 -> Add(gr,"AP");
        mg2 -> SetTitle(Form("#Delta t dependency bin%i",myBin));
        mg2 -> GetXaxis() -> SetTitle("#Delta t");
        mg2 -> GetYaxis() -> SetTitle("average w");
        mg2 -> Draw("a");
        c2 -> SaveAs(Form("rBin%i.pdf",myBin));
    }
    mg -> Draw("a");
}
