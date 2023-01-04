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

void CDCHitsCutEff() {
    TChain *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile("/home/belle2/zhai/B0_phi_KS0/Phi_cut_selection_Combo/Combo_phi_selection.root");
    
    int Entries = phi_Rec -> GetEntries();

    MyPhiRec rec;
    rec.setBranchAddresses(phi_Rec);

    const int llayer6[6] = {5,15,25,35,45,55};
    const int llayer8[8] = {5,10,20,21,22,23,40,55};

    const int nLayers = 56;
    int nTot_candi[nLayers];
    int nTrk_passHits[nLayers];
    float percentPass[nLayers];
    for(int iLayer=0; iLayer<nLayers; iLayer++){
        nTot_candi[iLayer]  = 0;
        nTrk_passHits[iLayer]=0;
        percentPass[iLayer] = 0.;
    }

    int minHit = 20;
    TH1F *h_eff = new TH1F("h_eff","",60,0,60);
    TH2F *H_pT_llayer = new TH2F("H_pT_llayer","",60,0,60,50,0,2.5);
    
    TH1F *h6_nhit[6];
    TH1F *h8_nhit[8];
    for(int iHisto=0; iHisto<6; iHisto++){
        h6_nhit[iHisto] = new TH1F(Form("h6_nhit_%i",llayer6[iHisto]),"",50,0,250);
    }
    for(int iHisto=0; iHisto<8; iHisto++){
        h8_nhit[iHisto] = new TH1F(Form("h8_nhit_%i",llayer8[iHisto]),"",50,0,250);
    }

    for(int i=0; i < Entries; i++){
        phi_Rec -> GetEntry(i);
        for(int iEnt = 0; iEnt < rec.nCand; iEnt++){
            H_pT_llayer -> Fill(rec.dau_0_lastCDCLayer[iEnt], rec.dau_0_pt[iEnt]);
            H_pT_llayer -> Fill(rec.dau_1_lastCDCLayer[iEnt], rec.dau_1_pt[iEnt]);

            for(int iLayer=0; iLayer<nLayers ; iLayer++){
                if(rec.dau_0_lastCDCLayer[iEnt]==iLayer){
                    nTot_candi[iLayer]++;
                    if(rec.dau_0_nCDCHits[iEnt]>minHit)nTrk_passHits[iLayer]++;
                }
                if(rec.dau_1_lastCDCLayer[iEnt]==iLayer){
                    nTot_candi[iLayer]++;
                    if(rec.dau_1_nCDCHits[iEnt]>minHit)nTrk_passHits[iLayer]++;
                }
            }//CDC last layer
            for(int iHisto=0; iHisto<6; iHisto++){
                if(rec.dau_0_lastCDCLayer[iEnt]==llayer6[iHisto])h6_nhit[iHisto] -> Fill(rec.dau_0_nCDCHits[iEnt]); 
                if(rec.dau_1_lastCDCLayer[iEnt]==llayer6[iHisto])h6_nhit[iHisto] -> Fill(rec.dau_1_nCDCHits[iEnt]); 
            }//fill in the nHit distr histos
            for(int iHisto=0; iHisto<8; iHisto++){
                if(rec.dau_0_lastCDCLayer[iEnt]==llayer8[iHisto])h8_nhit[iHisto] -> Fill(rec.dau_0_nCDCHits[iEnt]); 
                if(rec.dau_1_lastCDCLayer[iEnt]==llayer8[iHisto])h8_nhit[iHisto] -> Fill(rec.dau_1_nCDCHits[iEnt]); 
            }//fill in the nHit distr histos
        }//candidates
    }//phi

    for(int iLayer=0; iLayer<nLayers ; iLayer++){
        if(iLayer==54)continue;
        float percent = nTrk_passHits[iLayer]*1./nTot_candi[iLayer];
        percentPass[iLayer]=percent;
        h_eff->Fill(iLayer,percent);
    }//fill in trk eff histo

    gStyle->SetOptStat(0);
    TCanvas *c_eff = new TCanvas("c_eff","c_eff",500,400);
    setStyle(h_eff,kBlue+0,"lastCDCLayer of the trk","Efficiency of the cut nCDCHits>20", "All trks from MC combo candidates");
    h_eff -> Draw("HIST");
    c_eff -> SaveAs("plots/MCcombo_CDChits20_eff.pdf");

    TCanvas *c_pT_llayer = new TCanvas("c_pT_llayer","c_pT_llayer",500,400);
    setStyleTH2F(H_pT_llayer,"lastCDCLayer of the trk","pT(GeV)","All trks from MC combo candidates");
    H_pT_llayer -> Draw("COLZ");
    gPad -> SetLogz();
    c_pT_llayer -> SaveAs("plots/MCcombo_pT_lastCDClayer.pdf");

    TCanvas *c6_histos = new TCanvas("c6_histos","c6_histos",150*6,600);
    c6_histos -> Divide(6/2,2,0.01,0.01,0);
    for(int iHisto = 0; iHisto < 6; iHisto++){
        c6_histos -> cd(iHisto+1);
        setStyle(h6_nhit[iHisto], kBlue+0, "nCDCHits",Form("layer%i:  %.3f trks pass cut",llayer6[iHisto],percentPass[llayer6[iHisto]]));
        h6_nhit[iHisto] -> Draw();
    }
    c6_histos -> SaveAs("plots/6_hit_histos.pdf");
    
    TCanvas *c8_histos = new TCanvas("c8_histos","c8_histos",150*8,600);
    c8_histos -> Divide(8/2,2,0.01,0.01,0);
    for(int iHisto = 0; iHisto < 8; iHisto++){
        c8_histos -> cd(iHisto+1);
        setStyle(h8_nhit[iHisto], kBlue+0, "nCDCHits",Form("layer%i:  %.3f trks pass cut",llayer8[iHisto],percentPass[llayer8[iHisto]]));
        h8_nhit[iHisto] -> Draw();
    }
    c8_histos -> SaveAs("plots/8_hit_histos.pdf");

    return ;
}
