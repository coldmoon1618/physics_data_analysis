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

void nKS_evtBased() {

//From reconstructed evtBased KS0 file
    TH1F *h_KS_candidate = new TH1F("h_KS_candidate","Num KS0 candidates per event",22,0.,22.);
    TH1F *h_nTracks      = new TH1F("h_nTrakcs","nTracks of events with no cut",40,0.,40.);
    TH1F *h_KS_true_cand = new TH1F("h_KS_true_cand","KS0 candidates matched to truth per event",22,0.,22.);
    TH1F *h_pT_cand = new TH1F("h_pT_cand","pT of KS0 candidates",300,0.,3.);
    TH1F *h_pT_true = new TH1F("h_pT_true","pT of K_S0:generator",300,0.,3.);

    TChain *KS0s = new TChain("tree");
    KS0s -> AddFile("MC13a_Candidate_KS_EvtBased.root");

    int KEntries;
    KEntries = KS0s -> GetEntries();
    //The same for reco and truth

    int nCandidates;
    KS0s -> SetBranchAddress("__ncandidates__", &nCandidates);
    double nTracks;
    KS0s -> SetBranchAddress("nTracks", &nTracks);

    int nPart = 100;
    double isMCTruth[nPart];
    for(int i = 0; i< nPart; i++){isMCTruth[i]=0.;}
    KS0s->SetBranchAddress("isSignal",isMCTruth);
    double  pt[nPart];
    KS0s->SetBranchAddress("pt",pt);
    double  pt0[nPart];
    KS0s->SetBranchAddress("daughter__bo0__cmpt__bc",pt0);
    double  pt1[nPart];
    KS0s->SetBranchAddress("daughter__bo1__cmpt__bc",pt1);
    double decayLength[nPart];
    KS0s -> SetBranchAddress("flightDistance", decayLength);
    double decayLengthErr[nPart];
    KS0s -> SetBranchAddress("flightDistanceErr", decayLengthErr);
    double sig_2D =-9;

//From MCTruth evtBased KS0 file
    TH1F *h_KS_true_MC   = new TH1F("h_KS_true_MC","K_S0:generator per event",22,0.,22.);

    TChain *KS0_true = new TChain("tree");
    KS0_true -> AddFile("MC13a_KSpipi_MCTrue_EvtBased.root");

    int nTrueKS0s;
    KS0_true -> SetBranchAddress("__ncandidates__", &nTrueKS0s);
    double  pt_true[nPart];
    KS0_true->SetBranchAddress("pt",pt_true);
    double  PDG0[nPart];
    KS0_true->SetBranchAddress("daughter__bo0__cmPDG__bc",PDG0);
    double  PDG1[nPart];
    KS0_true->SetBranchAddress("daughter__bo1__cmPDG__bc",PDG1);
    //Turns out you can't have the daughter infos from generator

//Reco efficiency
    TH1D *h_reco_eff  = new TH1D("h_reco_eff","true KS0 cand/ true K_S0:generator",33,0.,3.3);

    int totalCand = 0;
    int totalTrueCand = 0;
    int totalTrueKS = 0;
    int totalTrks = 0;

//Event structure is the same for both 
    //int i = 31;
    //int i = 152572;
    for(int i=0; i<KEntries; i++)
    {
        KS0s -> GetEntry(i);
        KS0_true -> GetEntry(i);
        h_nTracks      -> Fill(nTracks);
        int nTrueCand = 0;
        int cutCandidates = 0;
        int cutTrue = 0;
        for(int iEnt = 0; iEnt < nCandidates; iEnt++){
            sig_2D = decayLength[iEnt]/decayLengthErr[iEnt];
            if(sig_2D<=3){cutCandidates++;continue;}
            //cout << sig_2D << " 2d sig " << endl;
            double isMCTrue = isMCTruth[iEnt];
            if(isMCTrue==1) {nTrueCand++; totalTrueCand++;}
            h_pT_cand  -> Fill(pt[iEnt]);
            sig_2D = -9;//reinitialize
        }//reco event level
        for(int iEnt = 0; iEnt < nTrueKS0s; iEnt++){
            h_pT_true  -> Fill(pt_true[iEnt]);
        }//truth event level
        nCandidates -= cutCandidates;
        nTrueKS0s   -= cutTrue;
        h_KS_true_cand -> Fill(nTrueCand);
        h_KS_candidate -> Fill(nCandidates);
        h_KS_true_MC   -> Fill(nTrueKS0s);
        totalTrueKS += nTrueKS0s;
        totalCand += nCandidates;
        totalTrks += nTracks;
        //if(nCandidates>2)
        //cout << nTracks << " trks " << nCandidates << " cands " << nTrueKS0s << " true" << endl;
        
        double reco_eff = nTrueCand/(1.0*nTrueKS0s);
        if(nTrueKS0s!=0)
            h_reco_eff -> Fill(reco_eff);
            //{cout<<"MC: "<<nTrueKS0s<<", reco: "<<nTrueCand<< "  "<<reco_eff<< endl;}
    }
    cout << totalCand << " total cand " << totalTrueCand << " true cand" << endl;
    cout << totalTrueKS << " true KS " << totalTrks << " total trks" << endl;

    gStyle->SetOptStat("nermou");

/*
    TCanvas *c1 = new TCanvas("c1","c1",1200,300);
    c1 -> Divide(2,1,0.01,0.01,0);
    c1 -> cd(1);
    h_pT_cand -> Draw();
    gPad -> SetLogy();
    h_pT_cand -> GetXaxis() -> SetTitle("GeV");
    h_pT_cand -> GetYaxis() -> SetTitle("Events");
    c1 -> cd(2);
    h_pT_true -> Draw();
    gPad -> SetLogy();
    h_pT_true -> GetXaxis() -> SetTitle("GeV");
    h_pT_true -> GetYaxis() -> SetTitle("Events");
*/

    TCanvas *c2 = new TCanvas("c2","c2",1200,700);
    c2 -> Divide(2,2,0.01,0.01,0);
    c2 -> cd(1);
    h_KS_candidate -> Draw();
    gPad -> SetLogy();
    h_KS_candidate -> GetXaxis() -> SetTitle("num_KS0_candidates");
    h_KS_candidate -> GetYaxis() -> SetTitle("Events");
    c2 -> cd(2);
    h_KS_true_cand -> Draw();
    gPad -> SetLogy();
    h_KS_true_cand -> GetXaxis() -> SetTitle("num_true_KS0_cand");
    h_KS_true_cand -> GetYaxis() -> SetTitle("Events");
    c2 -> cd(3);
    h_KS_true_MC -> Draw();
    gPad -> SetLogy();
    h_KS_true_MC -> GetXaxis() -> SetTitle("num_true_KS0_MC");
    h_KS_true_MC -> GetYaxis() -> SetTitle("Events");
   /*
    c2 -> cd(4);
    h_nTracks -> Draw();
    gPad -> SetLogy();
    h_nTracks -> GetXaxis() -> SetTitle("nTracks");
    h_nTracks -> GetYaxis() -> SetTitle("Events");
    h_reco_eff -> Draw();
    gPad -> SetLogy();
    h_reco_eff -> GetXaxis() -> SetTitle("KS0_reco/true");
    h_reco_eff -> GetYaxis() -> SetTitle("Events");
*/
    return ;
}
