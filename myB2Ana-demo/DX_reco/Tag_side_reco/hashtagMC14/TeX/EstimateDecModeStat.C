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
void EstimateDecModeStat(double minProb=.5){
    // 1-based index

    TChain *tree = new TChain("tree");
    tree -> AddFile("AllModes_Charged.root");
    double Mbc;
    tree -> SetBranchAddress("Mbc", &Mbc);
    double decMode;
    tree -> SetBranchAddress("decMode", &decMode);
    double SigProb;
    tree -> SetBranchAddress("SigProb", &SigProb);
    int mySignal;
    tree -> SetBranchAddress("mySignal", &mySignal);

    int Entries = tree -> GetEntries();

    TH1D *h1 = new TH1D("h1","",36,0,36);
    TH1D *h2 = new TH1D("h2","",36,0,36);
    TH1D *h3 = new TH1D("h3","",36,0,36);

    int nSig[36];
    for(int i=0; i<36;i++){
        nSig[i]=0;
    }
    int nSigTot = 0;
    for(int i=0; i<Entries; i++){
        tree -> GetEntry(i);
        if(isnan(Mbc))continue;                           
        if(Mbc<=5.24)continue;                            
        if(SigProb<=minProb)continue;    

        int intMode = floor(decMode);
        if(mySignal>.5){
            h1->Fill(intMode);
            h2->Fill(intMode);
            h3->Fill(intMode);
            nSig[intMode]++;
            nSigTot++;
        }
    }

    h1-> SetLineWidth(2);
    h1-> SetTitle("300/fb MC SigProb>0.1");
    h1-> GetXaxis() -> SetTitle("decModes");
    h1-> GetYaxis() -> SetTitle("mySignal>.5");
    h3-> SetLineWidth(2);
    h3-> SetTitle("DecModes yield\% (300/fb MC SigProb>0.5)");
    h3-> GetXaxis() -> SetTitle("decModes");
    h3-> GetYaxis() -> SetTitle("yield\%");

    gStyle -> SetOptStat("");
    TCanvas *c1 = new TCanvas("c1","c1",400,300);
    h1->Draw();
    c1 -> SaveAs("YieldStat_decModes.pdf");

    h3->Scale(100./nSigTot);
    TCanvas *c3 = new TCanvas("c3","c3",400,300);
    h3->Draw("HIST");
    gStyle->SetHistMinimumZero();
    TLine *line3 = new TLine(0,1,36,1);
    line3->SetLineWidth(1);
    line3->SetLineColor(kRed);
    line3-> Draw("same");
    TLatex T;
    T.SetNDC();
    T.SetTextSize(0.03);
    T.SetTextFont(42);

    T.DrawLatex(.13,.8,  Form("[00] %s",fState[0]));
    T.DrawLatex(.13,.75, Form("[01] %s",fState[1]));
    T.DrawLatex(.13,.7,  Form("[03] %s",fState[3]));
    T.DrawLatex(.13,.65, Form("[04] %s",fState[4]));
    
    T.DrawLatex(.29,.8,  Form("[14] %s",fState[14]));
    T.DrawLatex(.29,.75, Form("[15] %s",fState[15]));
    T.DrawLatex(.29,.7,  Form("[16] %s",fState[16]));
    T.DrawLatex(.29,.65, Form("[18] %s",fState[18]));

    T.DrawLatex(.45,.8,  Form("[20] %s",fState[20]));
    T.DrawLatex(.45,.75, Form("[21] %s",fState[21]));
    T.DrawLatex(.45,.7,  Form("[23] %s",fState[23]));
    T.DrawLatex(.45,.65, Form("[24] %s",fState[24]));

    T.DrawLatex(.61,.8,  Form("[25] %s",fState[25]));
    T.DrawLatex(.61,.75, Form("[26] %s",fState[26]));
    T.DrawLatex(.61,.7,  Form("[27] %s",fState[27]));
    T.DrawLatex(.61,.65, Form("[28] %s",fState[28]));

    T.DrawLatex(.77,.8,  Form("[30] %s",fState[30]));
    T.DrawLatex(.77,.75, Form("[31] %s",fState[31]));
    T.DrawLatex(.77,.7 , Form("[35] %s",fState[35]));
    
    c3 -> SaveAs("YieldRelTot_decModes.pdf");
    for(int i = 0; i<36; i++){
        cout<<nSig[i]<<endl;
    }
    cout<<nSigTot<<" from all decModes"<<endl;

    return;
}
