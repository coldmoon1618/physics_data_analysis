#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0MC.h"
#include "MyKsRec.h"
#include "MyPhiRec.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
void PlotGenPhiKsP(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");
   
    TChain *Ks_MC   = new TChain("Ks_MC");
    Ks_MC   -> AddFile(fileName);
    MyPhiKsMC genKs;
    genKs.setBranchAddresses(Ks_MC);
    int Entries = Ks_MC -> GetEntries();

    TChain *phi_MC  = new TChain("phi_MC");
    phi_MC -> AddFile(fileName);
    MyPhiKsMC genPhi;
    genPhi.setBranchAddresses(phi_MC);

    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");

    TH1F *h_Ks0_pstar   = new TH1F("h_Ks0_pstar","h_Ks0_pstar",50,0,5);
    TH1F *h_phi_pstar   = new TH1F("h_phi_pstar","h_phi_pstar",50,0,5);
    TH1F *h_Ks0_p   = new TH1F("h_Ks0_p","h_Ks0_p",50,0,5);
    TH1F *h_phi_p   = new TH1F("h_phi_p","h_phi_p",50,0,5);

    for(int i=0; i < Entries; i++){
        B0_MC   -> GetEntry(i);
        Ks_MC   -> GetEntry(i);
        phi_MC  -> GetEntry(i);

    //////////////////////////////////////////////////
        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc.nCand; iEnt++){
            if(mcPhi.nDaus[iEnt]>=2 && mcKs.nDaus[iEnt]>=2
                && abs(mcPhi.dau_0_PDG[iEnt])==321 && abs(mcPhi.dau_1_PDG[iEnt])==321
                && abs( mcKs.dau_0_PDG[iEnt])==211 && abs( mcKs.dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
            if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                h_Ks0_pstar -> Fill(genKs.pstar[iEnt]);
                h_Ks0_p -> Fill(genKs.p[iEnt]);
            }
        }//generated exact Kshort in the signal events

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<genPhi.nCand; iEnt++){
            if(abs(genPhi.dau_0_PDG[iEnt])==321 && abs(genPhi.dau_1_PDG[iEnt])==321){
                h_phi_pstar -> Fill(genPhi.pstar[iEnt]);
                h_phi_p -> Fill(genPhi.p[iEnt]);
            }
        }//generated exact Phi in the signal events

    }//nTotalEvents

    gStyle -> SetOptStat(0);

    TCanvas *c_pstar= new TCanvas("c_pstar","c_pstar",1000,400);
    c_pstar -> Divide(2,1,0.01,0.01);
    setStyle(h_Ks0_pstar,kBlue+0,"pstar","generated Ks(pipi)");
    setStyle(h_phi_pstar,kBlue+0,"pstar","generated phi(KK)");
    c_pstar -> cd(1);
    h_Ks0_pstar -> Draw();
    c_pstar -> cd(2);
    h_phi_pstar -> Draw();
    //c_pstar -> SaveAs("/home/belle2/zhai/B0_phi_KS0/reco_eff_cut_flow_study/plots/Generated_phi_Ks_pstar.pdf");
    c_pstar -> SaveAs("Generated_phi_Ks_pstar.pdf");

    TCanvas *c_p= new TCanvas("c_p","c_p",1000,400);
    c_p -> Divide(2,1,0.01,0.01);
    setStyle(h_Ks0_p,kBlue+0,"p","generated Ks(pipi)");
    setStyle(h_phi_p,kBlue+0,"p","generated phi(KK)");
    c_p -> cd(1);
    h_Ks0_p -> Draw();
    c_p -> cd(2);
    h_phi_p -> Draw();
    //c_p -> SaveAs("/home/belle2/zhai/B0_phi_KS0/reco_eff_cut_flow_study/plots/Generated_phi_Ks_p.pdf");
    c_p -> SaveAs("Generated_phi_Ks_p.pdf");

    return;
}
