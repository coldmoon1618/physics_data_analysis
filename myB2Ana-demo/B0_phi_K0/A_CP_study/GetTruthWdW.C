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
void GetTruthWdW() {

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
    array<double ,nBins> mu;
    int nTaggedB0 = 0;
    int nTaggedB0bar = 0;
    //Tagging efficiency and error
    array<double ,nBins> B0Eff;
    array<double ,nBins> B0barEff;
    array<double ,nBins> B0EffErr;
    array<double ,nBins> B0barEffErr;
    array<double ,nBins> effRatio;
    array<double ,nBins> effRatioErr;
    //mistag rate and difference
    array<double ,nBins> w;
    array<double ,nBins> dw;
    // Using xErr \approx sqrt(x), error propagation:
    // W/(W+R) *sqrt( 1/W + 1/(W+R))
    array<double ,nBins> wErr;
    array<double ,nBins> rMea;//r measured from w, instead of BDT output
    array<double ,nBins> rErr;
    array<double ,nBins> rAvg;//average in each bin
    array<double ,nBins> rMid;//simple midpoint
    array<double ,nBins> rTot = {.0,.0,.0,.0,.0,.0,.0};
    array<int    ,nBins> rEnt = { 0, 0, 0, 0, 0, 0, 0};
    for(int j=0; j<7; j++){
        rMid[j] = .5*(binMin[j]+binMax[j]);
    }

    for(int i=0; i<Entries; i++)
    {
        tree -> GetEntry(i);

        int j = rBins-1;
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
    }//candidates

    for(int j =0; j<7;j++){
        double wB0   = nWB0   [j]*1.0/(nWB0   [j]+nRB0   [j]);
        double wB0bar= nWB0bar[j]*1.0/(nWB0bar[j]+nRB0bar[j]);
        w[j]        = .5*(wB0+wB0bar);
        dw[j]       = wB0-wB0bar;
        double wB0_e     = wB0      *sqrt(1./nWB0[j]+1./(nRB0[j]+nWB0[j]));
        double wB0bar_e  = wB0bar   *sqrt(1./nWB0bar[j]+1./(nRB0bar[j]+nWB0bar[j]));
        wErr[j] = .5 * sqrt( wB0_e * wB0_e + wB0bar_e * wB0bar_e);
        rMea[j] = 1-2.*w[j];
        rErr[j] = 3.*2*wErr[j];//3x
        rAvg[j] = rTot[j]/rEnt[j];
        
        B0Eff[j]= nB0Tag[j]*1./nTaggedB0;
        B0barEff[j]= nB0barTag[j]*1./nTaggedB0bar;
        B0EffErr[j]=B0Eff[j]*sqrt(1./nB0Tag[j]+1./nTaggedB0);
        B0barEffErr[j]=B0barEff[j]*sqrt(1./nB0barTag[j]+1./nTaggedB0bar);

        effRatio[j]= B0Eff[j]*1./B0barEff[j];
        effRatioErr[j]=effRatio[j]*sqrt(1./nB0Tag[j]+1./nTaggedB0+1./nB0barTag[j]+1./nTaggedB0bar);
        
        mu[j]=(B0Eff[j]-B0barEff[j])/(B0Eff[j]+B0barEff[j]);
    }

    cout<<endl;
    cout<<Form("w_values  ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
     w[0], w[1], w[2], w[3], w[4], w[5], w[6])<<endl;
    cout<<Form("dw_values ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    dw[0],dw[1],dw[2],dw[3],dw[4],dw[5],dw[6])<<endl;
    cout<<Form("w_err ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    wErr[0],wErr[1],wErr[2],wErr[3],wErr[4],wErr[5],wErr[6])<<endl;

    cout<<endl;
    cout<<Form("r_mea ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    rMea[0],rMea[1],rMea[2],rMea[3],rMea[4],rMea[5],rMea[6])<<endl;
    cout<<Form("r_mid ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    rMid[0],rMid[1],rMid[2],rMid[3],rMid[4],rMid[5],rMid[6])<<endl;
    cout<<Form("r_avg ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    rAvg[0],rAvg[1],rAvg[2],rAvg[3],rAvg[4],rAvg[5],rAvg[6])<<endl;
    cout<<Form("3*r_err ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    rErr[0],rErr[1],rErr[2],rErr[3],rErr[4],rErr[5],rErr[6])<<endl;

    cout<<endl;
    cout<<Form("B0_eff ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    B0Eff[0],B0Eff[1],B0Eff[2],B0Eff[3],B0Eff[4],B0Eff[5],B0Eff[6])<<endl;
    cout<<Form("B0bar_eff ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    B0barEff[0],B0barEff[1],B0barEff[2],B0barEff[3],B0barEff[4],B0barEff[5],B0barEff[6])<<endl;
    cout<<Form("B0_eff_err ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    B0EffErr[0],B0EffErr[1],B0EffErr[2],B0EffErr[3],B0EffErr[4],B0EffErr[5],B0EffErr[6])<<endl;
    cout<<Form("B0bar_eff_err ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    B0barEffErr[0],B0barEffErr[1],B0barEffErr[2],B0barEffErr[3],B0barEffErr[4],B0barEffErr[5],B0barEffErr[6])<<endl;

    cout<<endl;
    cout<<Form("eff_ratio ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    effRatio[0],effRatio[1],effRatio[2],effRatio[3],effRatio[4],effRatio[5],effRatio[6])<<endl;
    cout<<Form("eff_ratio_err ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    effRatioErr[0],effRatioErr[1],effRatioErr[2],effRatioErr[3],effRatioErr[4],effRatioErr[5],effRatioErr[6])<<endl;
    
    cout<<endl;
    cout<<Form("mu ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
    mu[0],mu[1],mu[2],mu[3],mu[4],mu[5],mu[6])<<endl;
}
