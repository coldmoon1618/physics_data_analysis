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

#include "LatexDecModes.h"
#include "LatexSubModes.h"
void PlotDecMode(bool isMC = 0,
    int myMode=0, double minProb=.3){
    // 0-based index

    TChain *tree = new TChain("tree");
    if(isMC)tree -> AddFile("../AllModes_MC.root");
    else    tree -> AddFile("../AllModes_Data.root");
    double Mbc;
    tree -> SetBranchAddress("Mbc", &Mbc);
    double decMode;
    tree -> SetBranchAddress("decMode", &decMode);
    double SigProb;
    tree -> SetBranchAddress("SigProb", &SigProb);

    int Entries = tree -> GetEntries();

    TH1F *h_Mbc = new TH1F("Mbc","",30,5.24,5.3);

    int counter = 0;
    for(int i=0; i<Entries; i++){
        tree -> GetEntry(i);

        if(decMode!=myMode*1.0)continue;
        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        
        h_Mbc->Fill(Mbc);
        counter++;
    }

    cout<<counter<<endl;
    h_Mbc -> SetLineWidth(4);
    h_Mbc -> SetLineColor(kViolet+2);
        h_Mbc -> SetTitle(Form("[%02d]  %s  SigProb>%.3f",myMode,fState[myMode],minProb));

    h_Mbc -> GetXaxis() -> SetTitle("Mbc");

    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    h_Mbc->Draw();
    if(isMC)c1 -> SaveAs(Form("decMode_plots/MC_Mode%02d_Histo_%.3f.pdf", myMode, minProb));
    else    c1 -> SaveAs(Form("decMode_plots/Data_Mode%02d_Histo_%.3f.pdf", myMode, minProb));

    return;
}
