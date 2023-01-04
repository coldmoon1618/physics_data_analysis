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

void DrawMatched(int myMode = 0, double minProb = 0.03) {

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

    TH1F *h4_Mbc = new TH1F("h4_Mbc",   "pass accWrongFSP", 120,5.24, 5.3);
    TH1F *h3_Mbc = new TH1F("h3_Mbc",   "pass accMiss",     120,5.24, 5.3);
    TH1F *h2_Mbc = new TH1F("h2_Mbc",   "pass accMissGamma",120,5.24, 5.3);
    TH1F *h1_Mbc = new TH1F("h1_Mbc",   "strict isSignal",  120,5.24, 5.3);

    int nTotal = 0;
    int nSignl = 0;
    int nAccGamma = 0;
    int nAccMiss = 0;
    int nAccFSP = 0;
    for(int i=0; i<Entries; i++)
    {
        B_cand -> GetEntry(i);

        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        if(isnan(isSignal))continue;
        if(isCharg<.5)continue;
        if      (isSignal<.5 && isSigAccMiss<.5 && isSigAccMissGamma<.5 && isSigAccWrongFSP<.5)continue;
        nTotal++;

        if      (isSignal>.5){
            h1_Mbc -> Fill(Mbc);
            nSignl++;
        }else if(isSigAccMissGamma>.5){
            h2_Mbc -> Fill(Mbc);
            nAccGamma++;
        }else if(isSigAccMiss>.5){
            h3_Mbc -> Fill(Mbc);
            nAccMiss++;
        }else if(isSigAccWrongFSP>.5){
            h4_Mbc -> Fill(Mbc);
            nAccFSP++;
        }
    }//candidates

    cout<<"nTotal = "<<nTotal<<endl;
    cout<<"nSignl = "<<nSignl<<endl;
    cout<<"nAccGamma = "<<nAccGamma<<endl;
    cout<<"nAccMiss = "<<nAccMiss<<endl;
    cout<<"nAccFSP = "<<nAccFSP<<endl;

    // Stacked
    DrawStacked(h1_Mbc,h2_Mbc,h3_Mbc,h4_Mbc,"Mbc",
    Form("[%02d]  %s  SigProb>%.3f looseMatched",myMode,fState[myMode],minProb),
    Form("histo_plots/Mode%02d_matched_comp.pdf",myMode));

    // Normalized draw same
    TLatex T;
    T.SetTextFont(42); 
    T.SetTextAlign(21);
    T.SetNDC();
    gStyle -> SetOptStat(0);
    gStyle->SetOptTitle(0);
    TCanvas *c2 = new TCanvas("c2","c2",800,600);
    setStyle(h4_Mbc, kRed+2, 4);
    normalize(h4_Mbc);
    setStyle(h3_Mbc, kOrange+7, 4);
    normalize(h3_Mbc);
    setStyle(h2_Mbc, kOrange, 4);
    normalize(h2_Mbc);
    setStyle(h1_Mbc, kGreen+2, 4);
    normalize(h1_Mbc);
    h4_Mbc -> Draw("HIST");
    h3_Mbc -> Draw("HIST same");
    h2_Mbc -> Draw("HIST same");
    h1_Mbc -> Draw("HIST same");
    gPad->BuildLegend(0.7,0.7,0.9,0.9,"");
    T.DrawLatex(.5,.93,Form("[%02d]  %s  SigProb>%.3f looseMatched",myMode,fState[myMode],minProb));
    T.DrawTextNDC(.9,.005,"Mbc");
    c2 -> SaveAs(Form("histo_plots/Mode%02d_matched_normalized.pdf",myMode));
}
