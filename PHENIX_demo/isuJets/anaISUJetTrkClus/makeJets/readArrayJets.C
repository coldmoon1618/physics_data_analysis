/**
 *@Version: single-ckin(3),Run12pp200sim,array makeJets
            Notice the quantity[index] format
 *@author: Y. Zhai in 2019
 *@purpose: This reads in MakeJets output from Run12pp200sim
 *  There's no renormalization
 */
#include <iostream>
#include <fstream>

#include <TChain.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>
void readArrayJets(TString jetRoot, unsigned long ini, unsigned long fin){
//~~~~~~~~~~~~~~~~~ TChain for trees
  TChain * rJets = new TChain("rJets");
  TChain * fJets = new TChain("fJets");
  TChain * tJets = new TChain("tJets");
  TChain * mJets = new TChain("mJets");

//~~~~~~~~~~~~~~~~ Input filename structure, individual style 
  TString myDir  = "/sphenix/user/yzhai/Run12pp_zg/output/"; 
  
//~~~~~~~~~~~~~~~~ histos that reply on Add, need to declare before fIn closed  
  TH1F *h_tra_phi = new TH1F("h_tra_phi","phi distr for all charged particles used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_clu_phi = new TH1F("h_clu_phi","phi distr for all neutral particles used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_cha_phi = new TH1F("h_cha_phi","phi distr for all charged particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_neu_phi = new TH1F("h_neu_phi","phi distr for all neutral particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());

  TH1F *h_tra_eta = new TH1F("h_tra_eta","eta distr for all tracks used in reco jets",20,-5,5);
  TH1F *h_clu_eta = new TH1F("h_clu_eta","eta distr for all clusters used in reco jets",20,-5,5);  
  TH1F *h_neu_eta = new TH1F("h_neu_eta","eta distr for all neutral particles used in truth jets",20,-5,5);  
  TH1F *h_cha_eta = new TH1F("h_cha_eta","eta distr for all charged particles used in truth jets",20,-5,5);  

  TH1F *h_rJet_phi = new TH1F("h_rJet_phi","phi distribution for all reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_tJet_phi = new TH1F("h_tJet_phi","phi distribution for all truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  
  TH1F *h_rJet_eta = new TH1F("h_rJet_eta","eta distribution for all reco jets",20,-5,5);
  TH1F *h_tJet_eta = new TH1F("h_tJet_eta","eta distribution for all truth jets",20,-5,5);
  
  TH1F *h_d_R     = new TH1F("h_d_R",    "dR for all reco-truth jet pairs"   , 100,  0, 6);
  TH1F *h_dEta    = new TH1F("h_dEta",   "dEta for all reco-truth jet pairs" , 100, -1, 1);

  TH1F *h_dPhi    = new TH1F("h_dPhi",   "dPhi for all reco-truth jet pairs" , 20, -5, 5);
  TH1F *h_dPhi_r  = new TH1F("h_dPhi_r", "reduced dPhi for all reco-truth jet pairs" , 20, -5, 5);
  TH1F *h_dPhi_ra = new TH1F("h_dPhi_ra","reduced abs(dPhi) for all reco-truth jet pairs" , 20, -5, 5);
//~~~~~~~~~~~~~~~~ counters
  unsigned long nFiles = 0;
  unsigned long long rEntries = 0;
  unsigned long long fEntries = 0;
  unsigned long long tEntries = 0;
  unsigned long long mEntries = 0;
  unsigned long long rCounter = 0;
  unsigned long long fCounter = 0;
  unsigned long long tCounter = 0;
  unsigned long long mCounter = 0;
//~~~~~~~~~~~~~~~~ Loops to add files to the trees
  //unsigned long ini = 1;//initial index
  //unsigned long fin = 2500;//final index
  //unsigned long ini = 33;//initial index
  //unsigned long fin = 33;//final index
  cout<<"Reading in "<<myDir<<" from "<< ini << " to "<< fin << endl;
  for(unsigned long fInd = ini; fInd <= fin; fInd++){
    TString jet_fname = myDir;
    jet_fname += fInd;
    jet_fname += "/Run12SimJetAna";
    jet_fname += jetRoot;
    jet_fname += ".root";
    cout<<fInd<<" ";
  
    // skip bad files
    // You'd still get error messages
    // But it won't get added
    // to TChain or the counter
    TFile fIn(jet_fname);
    if(fIn.IsZombie()) {fIn.Close();cout<<"Zombine file skipped"<<endl; continue;}
    if(fIn.GetSize()<12000){fIn.Close(); cout<<"Small file skipped"<<endl;continue;}
    //adding histos must be before closing the files
    h_tra_phi -> Add((TH1F*)fIn.Get("h_track_phi"));
    h_clu_phi -> Add((TH1F*)fIn.Get("h_cluster_phi"));
    h_cha_phi -> Add((TH1F*)fIn.Get("h_charged_phi"));
    h_neu_phi -> Add((TH1F*)fIn.Get("h_neutral_phi"));

    h_tra_eta -> Add((TH1F*)fIn.Get("h_track_eta"));
    h_clu_eta -> Add((TH1F*)fIn.Get("h_cluster_eta"));
    h_cha_eta -> Add((TH1F*)fIn.Get("h_charged_eta"));
    h_neu_eta -> Add((TH1F*)fIn.Get("h_neutral_eta"));
    
    h_d_R     -> Add((TH1F*)fIn.Get("h_dR"));
    h_dEta    -> Add((TH1F*)fIn.Get("h_deta"));

    h_dPhi    -> Add((TH1F*)fIn.Get("h_dphi"));
    h_dPhi_r  -> Add((TH1F*)fIn.Get("h_red_dphi"));
    h_dPhi_ra -> Add((TH1F*)fIn.Get("h_red_abs_dphi"));
    
    fIn.Close();
      
    rJets  ->AddFile(jet_fname);
    fJets  ->AddFile(jet_fname);
    tJets  ->AddFile(jet_fname);
    mJets  ->AddFile(jet_fname);

    nFiles++;
   }//end index
    cout<<endl<<"Total good files: "<<nFiles<<endl;

    TCanvas *c1 = new TCanvas("c1","Reco phi distr",1000,500);
    c1 -> Divide(2,1,0.01,0.01,0);
    c1 -> cd(1);
    h_tra_phi -> Draw();
    c1 -> cd(2);
    h_clu_phi -> Draw();
    c1 -> SaveAs("rPhi"+jetRoot+".png");

    TCanvas *c2 = new TCanvas("c2","Truth phi distr",1000,500);
    c2 -> Divide(2,1,0.01,0.01,0);
    c2 -> cd(1);
    h_cha_phi -> Draw();
    c2 -> cd(2);
    h_neu_phi -> Draw();
    c2 -> SaveAs("tPhi"+jetRoot+".png");
    
    TCanvas *c3 = new TCanvas("c3","Reco eta distr",1000,500);
    c3 -> Divide(2,1,0.01,0.01,0);
    c3 -> cd(1);
    h_tra_eta -> Draw();
    c3 -> cd(2);
    h_clu_eta -> Draw();
    c3 -> SaveAs("rEta"+jetRoot+".png");

    TCanvas *c4 = new TCanvas("c4","Truth eta distr",1000,500);
    c4 -> Divide(2,1,0.01,0.01,0);
    c4 -> cd(1);
    h_cha_eta -> Draw();
    c4 -> cd(2);
    h_neu_eta -> Draw();
    c4 -> SaveAs("tEta"+jetRoot+".png");

    TCanvas *c5 = new TCanvas("c5","dEta, dPhi, dR between all pairs",1500,500);
    c5 -> Divide(3,1,0.01,0.01,0);
    c5 -> cd(1);
    h_dEta -> Draw();
    c5 -> cd(2);
    h_dPhi_r -> Draw();
    c5 -> cd(3);
    h_d_R -> Draw();
    c5 -> SaveAs("deta_dphi_dR"+jetRoot+".png");

    //draw the abs stuffs, and also compare before the eta acceptance cut and after
//~~~~~~~~~~~~~~~~ One output file to contain them all
//Declare fOut after fIn is closed.
//TFile *fOut = new TFile(jetRoot+".root","RECREATE");

//~~~~~~~~~~~~~~~~~ TH1F for histos
  TH1F *H_T_pT  = new TH1F("H_T_pT",  "pT distribution for truth Jets",140,0,140);
  
  TH1F *H_R_ZG  = new TH1F("H_R_ZG",  "z_g distribution for reco Jets",32,0,0.64);
  TH1F *H_T_ZG  = new TH1F("H_T_ZG",  "z_g distribution for truth Jets",32,0,0.64);
 
  TH2F *H_ZGs     = new TH2F("H_ZGs",     "ZG between matched rJets and tJets",32,0,0.64,32,0,0.64);
  H_ZGs->GetXaxis()->SetTitle("Reco Jet ZG");
  H_ZGs->GetYaxis()->SetTitle("True Jet ZG");
 
  TH2F *H_PTs     = new TH2F("H_PTs",     "PT correlation between matched recoJet and trueJet",140,0,140,140,0,140);
  H_PTs->GetXaxis()->SetTitle("Reco Jet PT");
  H_PTs->GetYaxis()->SetTitle("True Jet PT");
//~~~~~~~~~~~~~~~~
  rEntries = rJets->GetEntries();
  tEntries = tJets->GetEntries();
  mEntries = mJets->GetEntries();
  
  //cout<<"Good files "<<nFiles<<endl;
  cout<<"rEntries = "<<rEntries<<endl;
  cout<<"tEntries = "<<tEntries<<endl;
  cout<<"mEntries = "<<mEntries<<endl;
  cout<<endl; 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~tJets{
  const int maxTruthJets = 100;
  int nTruthJets;
  tJets->SetBranchAddress("nTruthJets", &nTruthJets);
  float T_z_g[maxTruthJets];
  tJets->SetBranchAddress("z_g", T_z_g);
  int   T_nc [maxTruthJets]; 
  tJets->SetBranchAddress("nc",  T_nc);
  float T_pT [maxTruthJets]; 
  tJets->SetBranchAddress("pT",  T_pT);
  float T_eta[maxTruthJets]; 
  tJets->SetBranchAddress("eta", T_eta);
  float T_phi[maxTruthJets]; 
  tJets->SetBranchAddress("phi", T_phi);
   
  for(unsigned long long i=0; i<tEntries; i++){
    tJets->GetEntry(i); 
  
    for(int iJet = 0; iJet < nTruthJets; iJet++){ 
      //TRUTH JET CUTS
      if(T_pT[iJet]<6.0 || T_nc[iJet]<3) continue;
      if(T_z_g[iJet]>0.55) continue;

      H_T_pT      -> Fill(T_pT[iJet]);
      h_tJet_phi -> Fill(T_phi[iJet]);
      h_tJet_eta -> Fill(T_eta[iJet]);
      H_T_ZG      -> Fill(T_z_g[iJet]);
      tCounter++;
    }//event level, nJets
      
  }//end fill tJets for nth CKIN
  cout<<"tCounter = "<<tCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~tJets}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~fJets{
  const int maxFakeJets = 20;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~fJets}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets{
  const int maxRecoJets = 20;
  int nRecoJets;
  rJets->SetBranchAddress("nRecoJets", &nRecoJets);
  float R_z_g[maxRecoJets];
  rJets->SetBranchAddress("z_g", R_z_g);
  int   R_nc [maxRecoJets]; 
  rJets->SetBranchAddress("nc",  R_nc);
  float R_pT [maxRecoJets]; 
  rJets->SetBranchAddress("pT",  R_pT);
  float R_eta[maxRecoJets]; 
  rJets->SetBranchAddress("eta", R_eta);
  float R_phi[maxRecoJets]; 
  rJets->SetBranchAddress("phi", R_phi);
  float R_cf [maxRecoJets]; 
  rJets->SetBranchAddress("cf",  R_cf);
   
  for(unsigned long long i=0; i<rEntries; i++){
    rJets->GetEntry(i); 
    
    for(int iJet = 0; iJet < nRecoJets; iJet++){ 
      //RECO JET CUTS
      if(R_cf[iJet]<0.1 || R_cf[iJet]>0.8 || R_pT[iJet]<6.0 || R_nc[iJet]<3) continue;
      if(R_z_g[iJet]>0.55) continue;

      H_R_ZG      -> Fill(R_z_g[iJet]);
      h_rJet_phi -> Fill(R_phi[iJet]);
      h_rJet_eta -> Fill(R_eta[iJet]);
      rCounter++;
    }//event level, nJets

  }//end fill rJets for nth CKIN
  cout<<"rCounter = "<<rCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets{
  const int maxMatJets = 20;
  int nMatchedJets;
  mJets->SetBranchAddress("nMatchedJets", &nMatchedJets);
  float M_r_z_g[maxMatJets]; 
  mJets->SetBranchAddress("r_z_g", M_r_z_g);
  float M_t_z_g[maxMatJets]; 
  mJets->SetBranchAddress("t_z_g", M_t_z_g);
  float M_r_cf [maxMatJets]; 
  mJets->SetBranchAddress("r_cf",  M_r_cf);
  int   M_r_nc [maxMatJets]; 
  mJets->SetBranchAddress("r_nc",  M_r_nc);
  float M_r_pT [maxMatJets]; 
  mJets->SetBranchAddress("r_pT",  M_r_pT);
  float M_t_pT [maxMatJets]; 
  mJets->SetBranchAddress("t_pT",  M_t_pT);
  float M_r_eta[maxMatJets]; 
  mJets->SetBranchAddress("r_eta", M_r_eta);
  float M_t_eta[maxMatJets]; 
  mJets->SetBranchAddress("t_eta", M_t_eta);
   
  for(unsigned long long i=0; i<mEntries; i++){
    mJets->GetEntry(i); 
    
    for(int iJet = 0; iJet < nMatchedJets; iJet++){ 
      //MATCHED JET CUTS
      if(M_r_cf[iJet]<0.1 || M_r_cf[iJet]>0.8 || M_r_pT[iJet]<6.0 || M_r_nc[iJet]<3) continue;
      if(M_t_z_g[iJet]>0.55 || M_r_z_g[iJet]>0.55) continue;  

      H_ZGs      -> Fill(M_r_z_g[iJet],M_t_z_g[iJet]);
      H_PTs      -> Fill(M_r_pT[iJet],M_t_pT[iJet]);
      mCounter++;
    }//event level, nJets
  
  }//end fill mJets for nth CKIN
   
    cout<<"mCounter = "<<mCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets}

    TCanvas *c6 = new TCanvas("c6","z_g comparison",1500,500);
    c6 -> Divide(3,1,0.01,0.01,0);
    c6 -> cd(1);
    H_R_ZG -> Draw();
    c6 -> cd(2);
    H_T_ZG -> Draw();
    c6 -> cd(3);
    H_ZGs -> Draw("colz");
    c6 -> SaveAs("zgs"+jetRoot+".png");

    TCanvas *c7 = new TCanvas("c7","jet phi comparison",1000,500);
    c7 -> Divide(2,1,0.01,0.01,0);
    c7 -> cd(1);
    h_rJet_phi -> Draw();
    c7 -> cd(2);
    h_tJet_phi -> Draw();
    c7 -> SaveAs("jet_phis"+jetRoot+".png");

    TCanvas *c8 = new TCanvas("c8","jet eta comparison",1000,500);
    c8 -> Divide(2,1,0.01,0.01,0);
    c8 -> cd(1);
    h_rJet_eta -> Draw();
    c8 -> cd(2);
    h_tJet_eta -> Draw();
    c8 -> SaveAs("jet_etas"+jetRoot+".png");
//  fOut -> Write();
//  fOut -> Close();
  return;

}//end readCondor
