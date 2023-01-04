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

void DrawStacked(double minProb = 0.5) {

    TChain     *tree = new TChain("tree");
    tree -> AddFile("AllModes_MC.root");
    double Mbc;
    tree -> SetBranchAddress("Mbc", &Mbc);
    double SigProb;
    tree -> SetBranchAddress("SigProb", &SigProb);
    int iMcType;
    tree -> SetBranchAddress("iMcType", &iMcType);

    int Entries = tree -> GetEntries();

    TH1F *h4_Mbc = new TH1F("h4_Mbc",   "Continuum",            180,5.21, 5.3);
    TH1F *h3_Mbc = new TH1F("h3_Mbc",   "B^{0} #bar{B^{0}} ",   180,5.21, 5.3);
    TH1F *h2_Mbc = new TH1F("h2_Mbc",   "B^{+}B^{-}     ",      180,5.21, 5.3);

    int nCharg = 0;
    int nMixed = 0;
    int nQQbar = 0;
    for(int i=0; i<Entries; i++)
    {
        tree -> GetEntry(i);

        if(isnan(Mbc))continue;
        if(Mbc<=5.21)continue;
        if(SigProb<=minProb)continue;
        if(iMcType==1){
            h3_Mbc -> Fill(Mbc);
            nMixed++;
        }else if(iMcType==2){
            h2_Mbc -> Fill(Mbc);
            nCharg++;
        }else{
            h4_Mbc -> Fill(Mbc);
            nQQbar++;
        }
    }//candidates
    cout<<"nCharg = "<<nCharg<<endl;
    cout<<"nMixed = "<<nMixed<<endl;
    cout<<"nQQbar = "<<nQQbar<<endl;

    DrawStacked(h2_Mbc,h3_Mbc,h4_Mbc,"Mbc","SigProb>0.5 300/fb","AllModes_comp.pdf");
}
