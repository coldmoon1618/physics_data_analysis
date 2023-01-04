#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TMath.h>

#include "MyB0MC.h"
#include "MyPhiKsMC.h"
#include "MyTrkRec.h"
#include "MyTrkMC.h"

#include "MySetStyle.h"

void PhiKs_daughters(){
    TString fileName("Trk_eff_SigMC.root");
   
    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");
    int Entries = B0_MC -> GetEntries();
    cout<<Entries<<" events "<<endl;

    const double pi = TMath::Pi();
    const double minTheta = pi/4.;
    const double maxTheta = pi*3/4.;

    TChain *K_MC = new TChain("K_MC");
    K_MC -> AddFile(fileName);
    MyTrkMC mcK;
    mcK.setBranchAddresses(K_MC);
    TChain *K_Rec = new TChain("K_Rec");
    K_Rec -> AddFile(fileName);
    MyTrkRec recK;
    recK.setBranchAddresses(K_Rec);

    TChain *pi_MC = new TChain("pi_MC");
    pi_MC -> AddFile(fileName);
    MyTrkMC mcPi;
    mcPi.setBranchAddresses(pi_MC);
    TChain *pi_Rec = new TChain("pi_Rec");
    pi_Rec -> AddFile(fileName);
    MyTrkRec recPi;
    recPi.setBranchAddresses(pi_Rec);

    // pT-dep of reco eff
    TH1F *hpT_full_pi_MC  = new TH1F("hpT_full_pi_MC",  "hpT_full_pi_MC", 12,.0,2.4);
    TH1F *hpT_full_pi_Rec = new TH1F("hpT_full_pi_Rec", "hpT_full_pi_Rec",12,.0,2.4);
    TH1F *hpT_full_K_MC   = new TH1F("hpT_full_K_MC",   "hpT_full_K_MC",  12,.0,2.4);
    TH1F *hpT_full_K_Rec  = new TH1F("hpT_full_K_Rec",  "hpT_full_K_Rec", 12,.0,2.4);
    TH1F *hpT_zoom_pi_MC  = new TH1F("hpT_zoom_pi_MC",  "hpT_zoom_pi_MC", 7,0,.21);
    TH1F *hpT_zoom_pi_Rec = new TH1F("hpT_zoom_pi_Rec", "hpT_zoom_pi_Rec",7,0,.21);
    TH1F *hpT_zoom_K_MC   = new TH1F("hpT_zoom_K_MC",   "hpT_zoom_K_MC",  7,0,.21);
    TH1F *hpT_zoom_K_Rec  = new TH1F("hpT_zoom_K_Rec",  "hpT_zoom_K_Rec", 7,0,.21);

    // dr-dep of reco eff
    TH1F *hdr_pi_MC  = new TH1F("hdr_pi_MC",  "hdr_pi_MC", 30,0,90);
    TH1F *hdr_pi_Rec = new TH1F("hdr_pi_Rec", "hdr_pi_Rec",30,0,90);

    // ThetaStar-dependency of reconstruction efficiency
    TH1F *hTheta_pi_MC  = new TH1F("hTheta_pi_MC",  "hTheta_pi_MC", 12,0,pi);
    TH1F *hTheta_pi_Rec = new TH1F("hTheta_pi_Rec", "hTheta_pi_Rec",12,0,pi);
    TH1F *hTheta_K_MC   = new TH1F("hTheta_K_MC",   "hTheta_K_MC",  12,0,pi);
    TH1F *hTheta_K_Rec  = new TH1F("hTheta_K_Rec",  "hTheta_K_Rec", 12,0,pi);

    // pT distribution at the edge
    TH1F *h_edge_pT_pi_MC  = new TH1F("h_edge_pT_pi_MC",  "", 20,0,1.);
    TH1F *h_edge_pT_pi_Rec = new TH1F("h_edge_pT_pi_Rec", "", 20,0,1.);
    TH1F *h_edge_pT_K_MC   = new TH1F("h_edge_pT_K_MC",   "", 20,0,1.);
    TH1F *h_edge_pT_K_Rec  = new TH1F("h_edge_pT_K_Rec",  "", 20,0,1.);

    // Central vs edge reco eff
    TH1F *h_edge_dr_MC  = new TH1F("h_edge_dr_MC",  "",18,0,90);
    TH1F *h_edge_dr_Rec = new TH1F("h_edge_dr_Rec", "",18,0,90);
    TH1F *h_cent_dr_MC  = new TH1F("h_cent_dr_MC",  "",18,0,90);
    TH1F *h_cent_dr_Rec = new TH1F("h_cent_dr_Rec", "",18,0,90);

    const int nPart = 999;
    bool hasMat[nPart];

    double thetaStar;
    double pT;

    for(int i=0; i < Entries; i++)
    //for(int i=0; i < 10; i++)
    {
        B0_MC   -> GetEntry(i);
        pi_MC   -> GetEntry(i);
        pi_Rec  -> GetEntry(i);
        K_MC    -> GetEntry(i);
        K_Rec   -> GetEntry(i);
    //////////////////////////////////////////////////
        bool hasSigChain = false;
        bool exactPhi = false;
        bool exactKs = false;
        //Always only one B-> phi Ks
        if(mcPhi.nDaus[0]>=2 && mcKs.nDaus[0]>=2){
            if(abs(mcPhi.dau_0_PDG[0])==321 && abs(mcPhi.dau_1_PDG[0])==321)exactPhi=true;
            if(abs( mcKs.dau_0_PDG[0])==211 && abs( mcKs.dau_1_PDG[0])==211)exactKs=true;
            hasSigChain = exactPhi&&exactKs;
        }
        if(!hasSigChain)continue;
    //////////////////////////////////////////////////
        for(int iEnt=0; iEnt<mcK.nCand; iEnt++){
            pT = mcK.pt[iEnt];
            thetaStar = mcK.thetaStar[iEnt];
            hpT_full_K_MC -> Fill(pT);
            hpT_zoom_K_MC -> Fill(pT);
            hTheta_K_MC -> Fill(thetaStar);
            if(thetaStar<minTheta||thetaStar>maxTheta){
                h_edge_pT_K_MC -> Fill(pT);
            }
        }
        for(int j=0; j<nPart; j++){hasMat[j]=false;}
        for(int iEnt=0; iEnt<recK.nCand; iEnt++){
            int genID = (int)recK.genParticleID[iEnt]; 
            if(hasMat[genID]){continue;}
            hasMat[genID]=true;
            pT = recK.mcPT[iEnt];
            thetaStar = recK.thetaStar[iEnt];
            hpT_full_K_Rec -> Fill(pT);
            hpT_zoom_K_Rec -> Fill(pT);
            hTheta_K_Rec -> Fill(thetaStar);
            if(thetaStar<minTheta||thetaStar>maxTheta){
                h_edge_pT_K_Rec -> Fill(pT);
            }
        }
        
    //////////////////////////////////////////////////
        for(int iEnt=0; iEnt<mcPi.nCand; iEnt++){
            pT = mcPi.pt[iEnt];
            thetaStar = mcPi.thetaStar[iEnt];
            hpT_full_pi_MC -> Fill(pT);
            hpT_zoom_pi_MC -> Fill(pT);
            hTheta_pi_MC -> Fill(thetaStar);
            double Lxy = sqrt(mcPi.mcProdVertexDX[iEnt]*mcPi.mcProdVertexDX[iEnt]+mcPi.mcProdVertexDY[iEnt]*mcPi.mcProdVertexDY[iEnt]);
            hdr_pi_MC -> Fill(Lxy);
            if(thetaStar<minTheta||thetaStar>maxTheta){
                h_edge_pT_pi_MC -> Fill(pT);
                h_edge_dr_MC -> Fill(Lxy);
            }
            else{
                h_cent_dr_MC -> Fill(Lxy);
            }
        }
        for(int j=0; j<nPart; j++){hasMat[j]=false;}
        for(int iEnt=0; iEnt<recPi.nCand; iEnt++){
            int genID = (int)recPi.genParticleID[iEnt]; 
            if(hasMat[genID]){continue;}
            pT = recPi.mcPT[iEnt];
            thetaStar = recPi.thetaStar[iEnt];
            hasMat[genID]=true;
            hpT_full_pi_Rec -> Fill(pT);
            hpT_zoom_pi_Rec -> Fill(pT);
            hTheta_pi_Rec -> Fill(thetaStar);
            double Lxy = sqrt(recPi.mcProdVertexDX[iEnt]*recPi.mcProdVertexDX[iEnt]+recPi.mcProdVertexDY[iEnt]*recPi.mcProdVertexDY[iEnt]);
            hdr_pi_Rec -> Fill(Lxy);
            if(thetaStar<minTheta||thetaStar>maxTheta){
                h_edge_pT_pi_Rec -> Fill(pT);
                h_edge_dr_Rec -> Fill(Lxy);
            }else{
                h_cent_dr_Rec -> Fill(Lxy);
            }
        }

    }//nTotalEvents

    gStyle -> SetOptStat(0);

// pT-dep
    if(0){
        TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
        c1 -> Divide(2,2,.01,.01);
        c1 -> cd(1);
        setStyle(hpT_full_pi_MC,kGreen+2, "pT", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(hpT_full_pi_Rec, kRed+1, "pT", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        hpT_full_pi_Rec -> SetMaximum(8e4);
        auto rp1 = new TRatioPlot(hpT_full_pi_Rec,hpT_full_pi_MC);
        drawRatioPlot(rp1, 1.,.8,504,.9,c1);

        c1 -> cd(2);
        setStyle(hpT_full_K_MC,kGreen+2, "pT", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        setStyle(hpT_full_K_Rec, kRed+1, "pT", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        hpT_full_K_Rec -> SetMaximum(2.e5);
        auto rp2 = new TRatioPlot(hpT_full_K_Rec,hpT_full_K_MC);
        drawRatioPlot(rp2, 1.,.8,504,.9,c1);

        c1 -> cd(3);
        setStyle(hpT_zoom_pi_MC,kGreen+2, "pT", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(hpT_zoom_pi_Rec, kRed+1, "pT", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        hpT_zoom_pi_Rec -> SetMaximum(1.6e4);
        auto rp3 = new TRatioPlot(hpT_zoom_pi_Rec,hpT_zoom_pi_MC);
        drawRatioPlot(rp3, 1.,0.,505,.5,c1);

        c1 -> cd(4);
        setStyle(hpT_zoom_K_MC,kGreen+2, "pT", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        setStyle(hpT_zoom_K_Rec, kRed+1, "pT", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        hpT_zoom_K_Rec -> SetMaximum(1.6e3);
        auto rp4 = new TRatioPlot(hpT_zoom_K_Rec,hpT_zoom_K_MC);
        drawRatioPlot(rp4, 1.,0.,505,.5,c1);
        c1 -> SaveAs("pT_dependency.pdf");
    }


    // dr-dep
    if(0){
        TCanvas *c1 = new TCanvas("c1","c1",600,500);
        setStyle(hdr_pi_MC,kGreen+2, "production dr [cm]", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(hdr_pi_Rec, kRed+1, "production dr [cm]", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        hdr_pi_Rec -> SetMaximum(1.8e5);
        auto rp1 = new TRatioPlot(hdr_pi_Rec,hdr_pi_MC);
        drawRatioPlot(rp1,1.,0.,505,.5,c1);
        c1 -> SaveAs("dr_dependency.pdf");
    }

    // theta-dep
    if(0){
        TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
        c1 -> Divide(2,2,.01,.01);
        setStyle(hTheta_pi_MC,kGreen+2, "#theta*", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(hTheta_pi_Rec, kRed+1, "#theta*", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        hTheta_pi_Rec -> SetMaximum(8e4);
        c1 -> cd(1);
        auto rp1 = new TRatioPlot(hTheta_pi_Rec,hTheta_pi_MC);
        drawRatioPlot(rp1,1.,.6,504,.8,c1);

        setStyle(hTheta_K_MC,kGreen+2, "#theta*", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        setStyle(hTheta_K_Rec, kRed+1, "#theta*", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        c1 -> cd(2);
        hTheta_K_Rec -> SetMaximum(1e5);
        auto rp2 = new TRatioPlot(hTheta_K_Rec,hTheta_K_MC);
        drawRatioPlot(rp2,1.,.6,504,.8,c1);

        setStyle(h_edge_dr_MC,kGreen+2, "dr edge", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(h_edge_dr_Rec, kRed+1, "dr edge", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        c1 -> cd(3);
        h_edge_dr_Rec -> SetMaximum(7e4);
        auto rp3 = new TRatioPlot(h_edge_dr_Rec,h_edge_dr_MC);
        drawRatioPlot(rp3,1.,0.,505,.5,c1);

        setStyle(h_cent_dr_MC,kGreen+2, "dr central", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(h_cent_dr_Rec, kRed+1, "dr central", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        h_cent_dr_Rec -> SetMaximum(2e5);
        c1 -> cd(4);
        auto rp4 = new TRatioPlot(h_cent_dr_Rec,h_cent_dr_MC);
        drawRatioPlot(rp4,1.,0.,505,.5,c1);
        c1 -> SaveAs("theta_dependency.pdf");
    }

    // Edge-effect, pT comparison
    if(1){
        TCanvas *c1 = new TCanvas("c1","c1",1200,500);
        c1 -> Divide(2,1,.01,.01);
        setStyle(h_edge_pT_pi_MC,kGreen+2, "pT edge", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        setStyle(h_edge_pT_pi_Rec, kRed+1, "pT edge", "#pi^{#pm} from Ks(#pi^{+}#pi^{-}) Reco/MC");
        h_edge_pT_pi_Rec -> SetMaximum(1e4);
        c1 -> cd(1);
        auto rp1 = new TRatioPlot(h_edge_pT_pi_Rec,h_edge_pT_pi_MC);
        drawRatioPlot(rp1,1.2,0.,505,.5,c1);

        c1 -> cd(2);
        setStyle(h_edge_pT_K_MC,kGreen+2, "pT edge", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        setStyle(h_edge_pT_K_Rec, kRed+1, "pT edge", "K^{#pm} from #phi(K^{+}K^{-}) Reco/MC");
        h_edge_pT_K_Rec -> SetMaximum(2e4);
        auto rp2 = new TRatioPlot(h_edge_pT_K_Rec,h_edge_pT_K_MC);
        drawRatioPlot(rp2,1.2,0.,505,.5,c1);
        c1 -> SaveAs("pT_at_edge.pdf");
    }
    return;
}
