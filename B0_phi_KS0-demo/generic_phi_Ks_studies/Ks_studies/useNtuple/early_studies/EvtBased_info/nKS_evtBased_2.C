//Instead of using framework generated MCTruth KS0 particles, use self generated skim
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

void nKS_evtBased_2() {

//From reconstructed evtBased KS0 file
    TH1F *h_KS_candidate = new TH1F("h_KS_candidate","Num KS0 candidates per event",22,0.,22.);
    //TH1F *h_nTracks      = new TH1F("h_nTrakcs","nTracks of events with no cut",40,0.,40.);
    TH1F *h_KS_true_cand = new TH1F("h_KS_true_cand","KS0 candidates matched to truth per event",22,0.,22.);
    TH1F *h_KS_mass_dist = new TH1F("h_KS_mass_dist","KS0 candidates invariant mass from 4-momentum",100,0.45,.55);

    TChain *KS0s = new TChain("tree");
    KS0s -> AddFile("MC13a_Candidate_KS_EvtBased.root");
    //KS0s -> AddFile("MC13aCh_Candidate_KS_EvtBased.root");

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
    double  mass[nPart];
    KS0s->SetBranchAddress("M",mass);//inv mass from particles 4mom, InvM is from daughters' 4mom
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

    TChain *KS0_true = new TChain("mcTree");
    KS0_true -> AddFile("skimTrees/MC13a_mixed_MCParticles_skim.root");
    KS0_true -> AddFile("skimTrees/MC13a_charged_MCParticles_skim.root");

    int nMCPart = 200;
    int nParticles;
    KS0_true -> SetBranchAddress("nParticles", &nParticles);
    double  pt_true[nMCPart];
    KS0_true->SetBranchAddress("pt",pt_true);
    int mcPDG[nMCPart];
    KS0_true->SetBranchAddress("pdg",mcPDG);
    int first_dau[nMCPart];
    KS0_true->SetBranchAddress("first_dau",first_dau);//dec_step
    int last_dau[nMCPart];
    KS0_true->SetBranchAddress("last_dau",last_dau);//dec_step
    int numDau[nMCPart];
    KS0_true->SetBranchAddress("numDau",numDau);
    int secPhysProc[nMCPart];
    KS0_true->SetBranchAddress("secPhysProc",secPhysProc);

//Reco efficiency
    TH1D *h_reco_eff  = new TH1D("h_reco_eff","true KS0 cand/ true K_S0:generator",33,0.,3.3);

    int totalCand = 0;
    int totalTrueCand = 0;
    int totalTrueKS = 0;
    int totalTrks = 0;
    int nEvtKS = 0;

//Event structure is the same for both 
    //int i = 31;
    //int i = 152572;
    for(int i=0; i<KEntries; i++)
    {
        KS0s -> GetEntry(i);
        KS0_true -> GetEntry(i);
        //h_nTracks      -> Fill(nTracks);
        int nTrueCand = 0;
        int cutCandidates = 0;//num cand that fail selection cut
        int cutTrue = 0;//num mcTrue that are not K_S0 -> pi+ pi-
        for(int iEnt = 0; iEnt < nCandidates; iEnt++){
            sig_2D = decayLength[iEnt]/decayLengthErr[iEnt];
            //if(mass[iEnt]<=0.45){cutCandidates++;continue;}
            //if(sig_2D<=3){cutCandidates++;continue;}
            //if(pt0[iEnt]<.26||pt1[iEnt]<.26){cutCandidates++;continue;}
            h_KS_mass_dist->Fill(mass[iEnt]);
            //cout << sig_2D << " 2d sig " << endl;
            double isMCTrue = isMCTruth[iEnt];
            if(isMCTrue==1) {nTrueCand++; totalTrueCand++;}
            sig_2D = -9;//reinitialize
        }//reco event level
        for(int iEnt = 0; iEnt < nParticles; iEnt++){
            int first_arrIndx = first_dau[iEnt] -1;//the original is dec_step
            int last_arrIndx  = last_dau[iEnt]  -1;//the original is dec_step
            if( (numDau[iEnt]==2&&mcPDG[iEnt]==310)//either K_S0 -> pi+ pi- or K_S0 -> pi- pi+ 
                && ((mcPDG[first_arrIndx]==211&&mcPDG[last_arrIndx]==-211)||(mcPDG[first_arrIndx]==-211&&mcPDG[last_arrIndx]==211)) ){
                ;//mass histogram
                //The two secPhysProc are 46348 88&89 but both from K_S0 so not sure why listed as secondary
            }else{
                cutTrue++;
            }
        }//truth event level
        nCandidates -= cutCandidates;
        nParticles  -= cutTrue;
        if(nParticles>0)nEvtKS++;
        h_KS_true_cand -> Fill(nTrueCand);
        h_KS_candidate -> Fill(nCandidates);
        h_KS_true_MC   -> Fill(nParticles);
        totalTrueKS += nParticles;
        totalCand += nCandidates;
        //totalTrks += nTracks;
        //if(nCandidates>2)
        //cout << nTracks << " trks " << nCandidates << " cands " << nTrueKS0s << " true" << endl;
        
        //double reco_eff = nTrueCand/(1.0*nTrueKS0s);
        //if(nTrueKS0s!=0)
        //    h_reco_eff -> Fill(reco_eff);
            //{cout<<"MC: "<<nTrueKS0s<<", reco: "<<nTrueCand<< "  "<<reco_eff<< endl;}
    }
    cout << totalCand << " total cand " << totalTrueCand << " true cand" << endl;
    cout << totalTrueKS << " true KS " << nEvtKS << " events with KS0s" << endl;

    gStyle->SetOptStat("nermou");

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
   //Draw invariant mass for the candidates
    c2 -> cd(4);
    h_KS_mass_dist -> Draw();
    //gPad -> SetLogy();
    h_KS_mass_dist -> SetMinimum(0.);
    h_KS_mass_dist -> SetMaximum(15000.);
    h_KS_mass_dist -> GetXaxis() -> SetTitle("GeV");
    h_KS_mass_dist -> GetYaxis() -> SetTitle("Entries");

    return ;
}
