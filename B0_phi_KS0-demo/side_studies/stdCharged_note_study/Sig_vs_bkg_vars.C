#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

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

#include "MyPhiRec.h"
#include "MySetStyle.h"

void Sig_vs_bkg_vars() {
    TChain *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile("/home/belle2/zhai/B0_phi_KS0/Phi_cut_selection_Combo/Combo_phi_selection.root");
    
    int Entries = phi_Rec -> GetEntries();

    MyPhiRec rec;
    rec.setBranchAddresses(phi_Rec);

    TH1F *h_sig_PID = new TH1F("h_sig_PID","",50,0,1);
    TH1F *h_bkg_PID = new TH1F("h_bkg_PID","",50,0,1);

    TH1F *h_sig_nSVDhits = new TH1F("h_sig_nSVDhits","",24,0,24);
    TH1F *h_bkg_nSVDhits = new TH1F("h_bkg_nSVDhits","",24,0,24);
    TH1F *h_sig_nCDChits = new TH1F("h_sig_nCDChits","",120,0,120);
    TH1F *h_bkg_nCDChits = new TH1F("h_bkg_nCDChits","",120,0,120);

    TH1F *h_sig_d0  = new TH1F("h_sig_d0","",25,0,5); 
    TH1F *h_bkg_d0  = new TH1F("h_bkg_d0","",25,0,5);
    TH1F *h_sig_z0  = new TH1F("h_sig_z0","",25,0,5);
    TH1F *h_bkg_z0  = new TH1F("h_bkg_z0","",25,0,5);

    TH1F *h_sig_dr  = new TH1F("h_sig_dr","",25,0,5);
    TH1F *h_bkg_dr  = new TH1F("h_bkg_dr","",25,0,5);
    TH1F *h_sig_dz  = new TH1F("h_sig_dz","",25,0,5);
    TH1F *h_bkg_dz  = new TH1F("h_bkg_dz","",25,0,5);

    for(int i=0; i < Entries; i++){
        phi_Rec -> GetEntry(i);
        for(int iEnt=0; iEnt<rec.nCand; iEnt++){
            if(rec.isSignal[iEnt]==1){
                h_sig_PID       -> Fill(rec.dau_0_kaonID[iEnt]);
                h_sig_PID       -> Fill(rec.dau_1_kaonID[iEnt]);
                h_sig_nCDChits  -> Fill(rec.dau_0_nCDCHits[iEnt]);
                h_sig_nCDChits  -> Fill(rec.dau_1_nCDCHits[iEnt]);
                h_sig_nSVDhits  -> Fill(rec.dau_0_nSVDHits[iEnt]);
                h_sig_nSVDhits  -> Fill(rec.dau_1_nSVDHits[iEnt]);

                h_sig_d0        -> Fill(abs(rec.dau_0_d0[iEnt]));
                h_sig_d0        -> Fill(abs(rec.dau_1_d0[iEnt]));
                h_sig_z0        -> Fill(abs(rec.dau_0_z0[iEnt]));
                h_sig_z0        -> Fill(abs(rec.dau_1_z0[iEnt]));

                h_sig_dr        -> Fill(abs(rec.dau_0_dr[iEnt]));
                h_sig_dr        -> Fill(abs(rec.dau_1_dr[iEnt]));
                h_sig_dz        -> Fill(abs(rec.dau_0_dz[iEnt]));
                h_sig_dz        -> Fill(abs(rec.dau_1_dz[iEnt]));
            }else{
                h_bkg_PID       -> Fill(rec.dau_0_kaonID[iEnt]);
                h_bkg_PID       -> Fill(rec.dau_1_kaonID[iEnt]);
                h_bkg_nCDChits  -> Fill(rec.dau_0_nCDCHits[iEnt]);
                h_bkg_nCDChits  -> Fill(rec.dau_1_nCDCHits[iEnt]);
                h_bkg_nSVDhits  -> Fill(rec.dau_0_nSVDHits[iEnt]);
                h_bkg_nSVDhits  -> Fill(rec.dau_1_nSVDHits[iEnt]);

                h_bkg_d0        -> Fill(abs(rec.dau_0_d0[iEnt]));
                h_bkg_d0        -> Fill(abs(rec.dau_1_d0[iEnt]));
                h_bkg_z0        -> Fill(abs(rec.dau_0_z0[iEnt]));
                h_bkg_z0        -> Fill(abs(rec.dau_1_z0[iEnt]));

                h_bkg_dr        -> Fill(abs(rec.dau_0_dr[iEnt]));
                h_bkg_dr        -> Fill(abs(rec.dau_1_dr[iEnt]));
                h_bkg_dz        -> Fill(abs(rec.dau_0_dz[iEnt]));
                h_bkg_dz        -> Fill(abs(rec.dau_1_dz[iEnt]));
            }

        }//candidates
    }//phi

    //KID graphs
    TCanvas *c1 = new TCanvas("c1","c1",900,400);
    c1 -> Divide(2,1,.01,.01,0);
    c1 -> cd(1);
    setStyle(h_sig_PID,kBlue+0,"KID","trks KID(BphiK, p*(#phi)>2.2GeV, SIG)");
    gPad -> SetLogy();
    h_sig_PID -> Draw();
    c1 -> cd(2);
    setStyle(h_bkg_PID,kBlue+0,"KID","trks KID(BphiK, p*(#phi)>2.2GeV, BKG)");
    gPad -> SetLogy();
    h_bkg_PID -> Draw();
    c1 -> SaveAs("plots/SigVsBkg_KID.pdf");

    //detector hits
    TCanvas *c2 = new TCanvas("c2","c2",900,800);
    c2 -> Divide(2,2,.01,.01,0);
    c2 -> cd(1);
    setStyle(h_sig_nCDChits,kBlue+0,"nCDChits","trks CDChits(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_nCDChits -> Draw();
    c2 -> cd(2);
    setStyle(h_bkg_nCDChits,kBlue+0,"nCDChits","trks CDChits(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_nCDChits -> Draw();
    c2 -> cd(3);
    setStyle(h_sig_nSVDhits,kBlue+0,"nSVDhits","trks SVDhits(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_nSVDhits -> Draw();
    c2 -> cd(4);
    setStyle(h_bkg_nSVDhits,kBlue+0,"nSVDhits","trks SVDhits(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_nSVDhits -> Draw();
    c2 -> SaveAs("plots/SigVsBkg_hits.pdf");


    //d0 z0
    TCanvas *c3 = new TCanvas("c3","c3",900,800);
    c3 -> Divide(2,2,.01,.01,0);
    c3 -> cd(1);
    setStyle(h_sig_z0,kBlue+0,"z0 and dz","MC combo SIG trks abs(z0) and abs(dz)");
    setStyle(h_sig_dz,kRed +0,"z0 and dz","MC combo SIG trks abs(z0) and abs(dz)");
    h_sig_dz -> SetLineStyle(kDashed);
    h_sig_d0 -> Draw();
    h_sig_dz -> Draw("same");
    gPad -> SetLogy();
    c3 -> cd(2);
    setStyle(h_bkg_z0,kBlue+0,"z0 and dz","MC combo BKG trks abs(z0) and abs(dz)");
    setStyle(h_bkg_dz,kRed +0,"z0 and dz","MC combo BKG trks abs(z0) and abs(dz)");
    h_bkg_dz -> SetLineStyle(kDashed);
    h_bkg_d0 -> Draw();
    h_bkg_dz -> Draw("same");
    gPad -> SetLogy();
    c3 -> cd(3);
    setStyle(h_sig_d0,kBlue+0,"d0 and dr","MC combo SIG trks abs(d0) and abs(dr)");
    setStyle(h_sig_dr,kRed +0,"d0 and dr","MC combo SIG trks abs(d0) and abs(dr)");
    h_sig_dr -> SetLineStyle(kDashed);
    h_sig_z0 -> Draw();
    h_sig_dr -> Draw("same");
    gPad -> SetLogy();
    c3 -> cd(4);
    setStyle(h_bkg_d0,kBlue+0,"d0 and dr","MC combo BKG trks abs(d0) and abs(dr)");
    setStyle(h_bkg_dr,kRed +0,"d0 and dr","MC combo BKG trks abs(d0) and abs(dr)");
    h_bkg_dr -> SetLineStyle(kDashed);
    h_bkg_z0 -> Draw();
    h_bkg_dr -> Draw("same");
    gPad -> SetLogy();
    c3 -> SaveAs("plots/SigVsBkg_d0z0.pdf");

    return ;
}
