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

#include "MySetStyle.h"
#include "LatexDecModes.h"

void DrawStacked(int myMode = 0, double minProb = 0.03) {

    TChain     *B_cand = new TChain("B_cand");
    B_cand -> AddFile(Form("decModes/mode_%02d/Total.root",myMode));
    double Mbc;
    B_cand -> SetBranchAddress("Mbc", &Mbc);
    double decMode;
    B_cand -> SetBranchAddress("decMode", &decMode);
    double SigProb;
    B_cand -> SetBranchAddress("SigProb", &SigProb);
    double isSignal;
    B_cand -> SetBranchAddress("isSignal",&isSignal);
    double isCharg;
    B_cand -> SetBranchAddress("isCharg", &isCharg);
    double isMixed;
    B_cand -> SetBranchAddress("isMixed", &isMixed);
    double isQQbar;
    B_cand -> SetBranchAddress("isQQbar", &isQQbar);

    double isSigAccMiss;
    B_cand -> SetBranchAddress("isSigAccMiss",&isSigAccMiss);
    double isSigAccMissGamma;
    B_cand -> SetBranchAddress("isSigAccMissGamma",&isSigAccMissGamma);
    double isSigAccWrongFSP;
    B_cand -> SetBranchAddress("isSigAccWrongFSP",&isSigAccWrongFSP);

    int Entries = B_cand -> GetEntries();

    TH1F *h4_Mbc = new TH1F("h4_Mbc",   "Continuum",    120,5.24, 5.3);
    TH1F *h3_Mbc = new TH1F("h3_Mbc",   "Mixed bkg",    120,5.24, 5.3);
    TH1F *h2_Mbc = new TH1F("h2_Mbc",   "B+B-  bkg",    120,5.24, 5.3);
    TH1F *h1_Mbc = new TH1F("h1_Mbc",   "loose matched",120,5.24, 5.3);

    int nSignl = 0;
    int nCharg = 0;
    int nMixed = 0;
    int nQQbar = 0;
    for(int i=0; i<Entries; i++)
    {
        B_cand -> GetEntry(i);

        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        if(isSignal>.5 || isSigAccMiss>.5 || isSigAccMissGamma>.5 || isSigAccWrongFSP>.5){
            if(isCharg<.5)continue;
            h1_Mbc -> Fill(Mbc);
            nSignl++;
        }else if(isCharg >0.5){
            h2_Mbc -> Fill(Mbc);
            nCharg++;
        }else if(isMixed >0.5){
            h3_Mbc -> Fill(Mbc);
            nMixed++;
        }else if(isQQbar >0.5){
            h4_Mbc -> Fill(Mbc);
            nQQbar++;
        }
    }//candidates
    cout<<"nSignl = "<<nSignl<<endl;
    cout<<"nCharg = "<<nCharg<<endl;
    cout<<"nMixed = "<<nMixed<<endl;
    cout<<"nQQbar = "<<nQQbar<<endl;

    DrawStacked(h1_Mbc,h2_Mbc,h3_Mbc,h4_Mbc,"Mbc",
    Form("[%02d]  %s  SigProb>%.3f (100/fb MC)",myMode,fState[myMode],minProb),
    Form("histo_plots/Mode%02d_all_comp.pdf",myMode));

    DrawStacked(h1_Mbc,h2_Mbc,h3_Mbc,"Mbc",
    Form("[%02d]  %s  SigProb>%.3f (100/fb MC)",myMode,fState[myMode],minProb),
    Form("histo_plots/Mode%02d_BB_comp.pdf",myMode));
}
