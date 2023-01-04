#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0MC.h"
#include "MyKsRec.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
void CompareKsGenRecVars(bool toNormalize=1){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/Full_selections/SigMC/SigMC_B0_Rec_20k.root");

    TChain *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec *recKs  = new MyKsRec();
    recKs   -> setBranchAddresses(Ks_Rec);
    int Entries = Ks_Rec -> GetEntries();
    cout << Entries << "events"<<endl;
   
    TChain *Ks_MC   = new TChain("Ks_MC");
    Ks_MC   -> AddFile(fileName);
    MyPhiKsMC *genKs= new MyPhiKsMC();
    genKs   -> setBranchAddresses(Ks_MC);

    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC  *mc     = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC *mcPhi= new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC *mcKs = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    double Ks_trk_pt;
    double Ks_trk_theta;
    double Ks_trk_dist;

    TH1F *h_rec_pt      = new TH1F("h_rec_pt","h_rec_pt",50,0,3);
    TH1F *h_gen_pt      = new TH1F("h_gen_pt","h_gen_pt",50,0,3);
    TH1F *h_rec_theta   = new TH1F("h_rec_theta","h_rec_theta",64,0,3.2);
    TH1F *h_gen_theta   = new TH1F("h_gen_theta","h_gen_theta",64,0,3.2);
    TH1F *h_rec_dist    = new TH1F("h_rec_dist","h_rec_dist",50,0,10);
    TH1F *h_gen_dist    = new TH1F("h_gen_dist","h_gen_dist",50,0,10);

    for(int i=0; i < Entries; i++){
        B0_MC   -> GetEntry(i);
        Ks_Rec  -> GetEntry(i);
        Ks_MC   -> GetEntry(i);

    //////////////////////////////////////////////////
        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if(mcPhi->nDaus[iEnt]>=2 && mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<genKs->nCand; iEnt++){
            if(abs(genKs->dau_0_PDG[iEnt])==211 && abs(genKs->dau_1_PDG[iEnt])==211){
                h_gen_pt    -> Fill(genKs->dau_0_pt[iEnt]);
                h_gen_pt    -> Fill(genKs->dau_1_pt[iEnt]);
                h_gen_theta -> Fill(genKs->dau_0_theta[iEnt]);
                h_gen_theta -> Fill(genKs->dau_1_theta[iEnt]);
                h_gen_dist  -> Fill(genKs->mcFlightDistance[iEnt]);
            }
        }//generated exact Kshort in the signal events

    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs->nCand; iEnt++){
            if(recKs->isSignal[iEnt]==1){
                h_rec_pt    -> Fill(recKs->dau_0_pt[iEnt]);
                h_rec_pt    -> Fill(recKs->dau_1_pt[iEnt]);
                h_rec_theta -> Fill(recKs->dau_0_theta[iEnt]);
                h_rec_theta -> Fill(recKs->dau_1_theta[iEnt]);
                h_rec_dist  -> Fill(recKs->flightDistance[iEnt]);
            }//matched
        }//Ks0 rec candis

    }//nTotalEvents

    gStyle -> SetOptStat(0);
    if(toNormalize){
        normalize(h_rec_pt);
        normalize(h_gen_pt);
        normalize(h_rec_theta);
        normalize(h_gen_theta);
        normalize(h_rec_dist);
        normalize(h_gen_dist);
    }

    TCanvas *c_theta= new TCanvas("c_theta","c_theta",500,400);
    setStyle(h_rec_theta,kGreen+2,"trk theta","gen(blue) matched(green)");
    setStyle(h_gen_theta,kBlue+0,"trk theta","gen(blue) matched(green)");
    if(toNormalize){
        h_rec_theta -> Draw();
        h_gen_theta -> Draw("same");
        c_theta -> SaveAs("Ks_theta_comparison_norm.pdf");
    }else{
        h_gen_theta -> Draw();
        h_rec_theta -> Draw("same");
        c_theta -> SaveAs("Ks_theta_comparison.pdf");
    }
    
    TCanvas *c_dist = new TCanvas("c_dist","c_dist",500,400);
    setStyle(h_rec_dist,kGreen+2,"trk flightDist","gen(blue) matched(green)");
    setStyle(h_gen_dist,kBlue+0,"trk flightDist","gen(blue) matched(green)");
    if(toNormalize){
        h_rec_dist -> Draw();
        h_gen_dist -> Draw("same");
        c_dist -> SaveAs("Ks_flightDist_comparison_norm.pdf");
    }else{
        h_gen_dist -> Draw();
        h_rec_dist -> Draw("same");
        c_dist -> SaveAs("Ks_flightDist_comparison.pdf");
    }
    
    TCanvas *c_pt   = new TCanvas("c_pt","c_pt",500,400);
    setStyle(h_rec_pt,kGreen+2,"trk pt","gen(blue) matched(green)");
    setStyle(h_gen_pt,kBlue+0,"trk pt","gen(blue) matched(green)");
    if(toNormalize){
        h_rec_pt -> Draw();
        h_gen_pt -> Draw("same");
        c_pt -> SaveAs("Ks_pT_comparison_norm.pdf");
        gPad -> SetLogy();
        c_pt -> SaveAs("Ks_pt_LOG.pdf");
    }else{
        h_gen_pt -> Draw();
        h_rec_pt -> Draw("same");
        c_pt -> SaveAs("Ks_pT_comparison.pdf");
    }
    return;
}
