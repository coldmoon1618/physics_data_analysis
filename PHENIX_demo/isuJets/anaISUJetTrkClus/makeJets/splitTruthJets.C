/**
 *@Version: single-ckin(3),Run12pp200sim,array makeJets
            Notice the quantity[index] format
 *@author: Y. Zhai in 2019
 *@purpose: Debug testing work to split out the zg steps
          over different nc and also draw the correlation 
          between nc and pT stuffs....
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
void splitTruthJets(TString jetRoot, unsigned long ini, unsigned long fin){
//~~~~~~~~~~~~~~~~~ TChain for trees
  TChain * tJets = new TChain("tJets");
  TChain * rJets = new TChain("rJets");
  TChain * mJets = new TChain("mJets");

//~~~~~~~~~~~~~~~~ Input filename structure, individual style 
  TString myDir  = "/sphenix/user/yzhai/Run12pp_zg/output/"; 
  
//~~~~~~~~~~~~~~~~ counters
  unsigned long nFiles = 0;
  unsigned long long tEntries = 0;
  unsigned long long tCounter = 0;
  unsigned long long rEntries = 0;
  unsigned long long rCounter = 0;
  unsigned long long mEntries = 0;
  unsigned long long mCounter = 0;
//~~~~~~~~~~~~~~~~ Loops to add files to the trees
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
    
    fIn.Close();
      
    tJets  ->AddFile(jet_fname);
    rJets  ->AddFile(jet_fname);
    mJets  ->AddFile(jet_fname);

    nFiles++;
   }//end index
    cout<<endl<<"Total good files: "<<nFiles<<endl;

  TH2F *H_NCs     = new TH2F("H_NCs",     "NC between matched rJets and tJets",15,0,15,15,0,15);
  H_NCs->GetXaxis()->SetTitle("Reco Jet NC");
  H_NCs->GetYaxis()->SetTitle("True Jet NC");
 
  TH1F *H_T_ZG_3  = new TH1F("H_T_ZG_3",  "z_g for truth Jets",32,0,0.64);
  TH1F *H_T_ZG_4  = new TH1F("H_T_ZG_4",  "z_g for truth Jets",32,0,0.64);
  TH1F *H_T_ZG_5  = new TH1F("H_T_ZG_5",  "z_g for truth Jets",32,0,0.64);
  TH1F *H_T_ZG_6  = new TH1F("H_T_ZG_6",  "z_g for truth Jets",32,0,0.64);

  TH1F *H_R_ZG_3  = new TH1F("H_R_ZG_3",  "z_g for reco Jets",32,0,0.64);
  TH1F *H_R_ZG_4  = new TH1F("H_R_ZG_4",  "z_g for reco Jets",32,0,0.64);
  TH1F *H_R_ZG_5  = new TH1F("H_R_ZG_5",  "z_g for reco Jets",32,0,0.64);
  TH1F *H_R_ZG_6  = new TH1F("H_R_ZG_6",  "z_g for reco Jets",32,0,0.64);

  TH1F *H_MR_ZG_3  = new TH1F("H_MR_ZG_3",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *H_MR_ZG_4  = new TH1F("H_MR_ZG_4",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *H_MR_ZG_5  = new TH1F("H_MR_ZG_5",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *H_MR_ZG_6  = new TH1F("H_MR_ZG_6",  "z_g of matched reco Jets",32,0,0.64);

  TH1F *H_MT_ZG_3  = new TH1F("H_MT_ZG_3",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *H_MT_ZG_4  = new TH1F("H_MT_ZG_4",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *H_MT_ZG_5  = new TH1F("H_MT_ZG_5",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *H_MT_ZG_6  = new TH1F("H_MT_ZG_6",  "z_g of matched truth Jets",32,0,0.64);
//~~~~~~~~~~~~~~~~
  tEntries = tJets->GetEntries();
  rEntries = rJets->GetEntries();
  mEntries = mJets->GetEntries();
  
  cout<<"tEntries = "<<tEntries<<endl;
  cout<<"rEntries = "<<rEntries<<endl;
  cout<<"mEntries = "<<mEntries<<endl;
  cout<<endl; 
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
      if(abs(R_eta[iJet])>0.3) continue;
      if(R_z_g[iJet]>0.55) continue;

      if(R_nc[iJet]==3)
      H_R_ZG_3      -> Fill(R_z_g[iJet]);
      else if(R_nc[iJet]==4)
      H_R_ZG_4      -> Fill(R_z_g[iJet]);
      else if(R_nc[iJet]==5)
      H_R_ZG_5      -> Fill(R_z_g[iJet]);
      else if(R_nc[iJet]==6)
      H_R_ZG_6      -> Fill(R_z_g[iJet]);

      rCounter++;
    }//event level, nJets

  }//end fill rJets for nth CKIN
  cout<<"rCounter = "<<rCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets}
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
      if(abs(T_eta[iJet])>0.3) continue;
      if(T_z_g[iJet]>0.55) continue;
      
      if(T_nc[iJet]==3)
      H_T_ZG_3      -> Fill(T_z_g[iJet]);
      else if(T_nc[iJet]==4)
      H_T_ZG_4      -> Fill(T_z_g[iJet]);
      else if(T_nc[iJet]==5)
      H_T_ZG_5      -> Fill(T_z_g[iJet]);
      else if(T_nc[iJet]==6)
      H_T_ZG_6      -> Fill(T_z_g[iJet]);
      
      tCounter++;
    }//event level, nJets
      
  }//end fill tJets for nth CKIN
  cout<<"tCounter = "<<tCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~tJets}
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
  int   M_t_nc [maxMatJets]; 
  mJets->SetBranchAddress("t_nc",  M_t_nc);
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

      H_NCs      -> Fill(M_r_nc[iJet],M_t_nc[iJet]);

      if(M_r_nc[iJet]==3){
        H_MR_ZG_3      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_3      -> Fill(M_t_z_g[iJet]);
      }
      else if(M_r_nc[iJet]==4){
        H_MR_ZG_4      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_4      -> Fill(M_t_z_g[iJet]);
      }
      else if(M_r_nc[iJet]==5){
        H_MR_ZG_5      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_5      -> Fill(M_t_z_g[iJet]);
      }
      else if(M_r_nc[iJet]==6){
        H_MR_ZG_6      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_6      -> Fill(M_t_z_g[iJet]);
      }

      mCounter++;
    }//event level, nJets
  
  }//end fill mJets for nth CKIN
   
    cout<<"mCounter = "<<mCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets}
  TCanvas *c2 = new TCanvas("c2","truth zg distr",1500,500);
  gStyle -> SetOptStat(0);
  c2 -> Divide(3,1,0.01,0.01,0);
  c2 -> cd(1);
  H_MR_ZG_3 -> SetLineColor(1);
  H_MR_ZG_3 -> DrawCopy();
  H_MR_ZG_4 -> SetLineColor(2);
  H_MR_ZG_4 -> DrawCopy("same");
  H_MR_ZG_5 -> SetLineColor(3);
  H_MR_ZG_5 -> DrawCopy("same");
  H_MR_ZG_6 -> SetLineColor(4);
  H_MR_ZG_6 -> DrawCopy("same");

  TLegend *mr_legend = new TLegend(0.75,0.7,0.9,0.9);
  mr_legend -> AddEntry("H_MR_ZG_3","r_nc=3","l");
  mr_legend -> AddEntry("H_MR_ZG_4","r_nc=4","l");
  mr_legend -> AddEntry("H_MR_ZG_5","r_nc=5","l");
  mr_legend -> AddEntry("H_MR_ZG_6","r_nc=6","l");
  mr_legend -> Draw("same");

  c2 -> cd(2);
  H_MT_ZG_3 -> SetLineColor(1);
  H_MT_ZG_3 -> DrawCopy();
  H_MT_ZG_4 -> SetLineColor(2);
  H_MT_ZG_4 -> DrawCopy("same");
  H_MT_ZG_5 -> SetLineColor(3);
  H_MT_ZG_5 -> DrawCopy("same");
  H_MT_ZG_6 -> SetLineColor(4);
  H_MT_ZG_6 -> DrawCopy("same");
  
  TLegend *mt_legend = new TLegend(0.75,0.7,0.9,0.9);
  mt_legend -> AddEntry("H_MT_ZG_3","r_nc=3","l");
  mt_legend -> AddEntry("H_MT_ZG_4","r_nc=4","l");
  mt_legend -> AddEntry("H_MT_ZG_5","r_nc=5","l");
  mt_legend -> AddEntry("H_MT_ZG_6","r_nc=6","l");
  mt_legend -> Draw("same");

  c2 -> cd(3);
  H_NCs -> Draw("colz");
  
  c2 -> SaveAs("matched_nc3456"+jetRoot+".png");

  TCanvas *c1 = new TCanvas("c1","truth zg distr",1000,500);
  gStyle -> SetOptStat(0);
  c1 -> Divide(2,1,0.01,0.01,0);
  c1 -> cd(1);
  H_R_ZG_3 -> SetLineColor(1);
  H_R_ZG_3 -> DrawCopy();
  H_R_ZG_4 -> SetLineColor(2);
  H_R_ZG_4 -> DrawCopy("same");
  H_R_ZG_5 -> SetLineColor(3);
  H_R_ZG_5 -> DrawCopy("same");
  H_R_ZG_6 -> SetLineColor(4);
  H_R_ZG_6 -> DrawCopy("same");

  TLegend *r_legend = new TLegend(0.8,0.7,0.9,0.9);
  r_legend -> AddEntry("H_R_ZG_3","nc=3","l");
  r_legend -> AddEntry("H_R_ZG_4","nc=4","l");
  r_legend -> AddEntry("H_R_ZG_5","nc=5","l");
  r_legend -> AddEntry("H_R_ZG_6","nc=6","l");
  r_legend -> Draw("same");

  c1 -> cd(2);
  H_T_ZG_3 -> SetLineColor(1);
  H_T_ZG_3 -> DrawCopy();
  H_T_ZG_4 -> SetLineColor(2);
  H_T_ZG_4 -> DrawCopy("same");
  H_T_ZG_5 -> SetLineColor(3);
  H_T_ZG_5 -> DrawCopy("same");
  H_T_ZG_6 -> SetLineColor(4);
  H_T_ZG_6 -> DrawCopy("same");
  
  TLegend *t_legend = new TLegend(0.8,0.7,0.9,0.9);
  t_legend -> AddEntry("H_T_ZG_3","nc=3","l");
  t_legend -> AddEntry("H_T_ZG_4","nc=4","l");
  t_legend -> AddEntry("H_T_ZG_5","nc=5","l");
  t_legend -> AddEntry("H_T_ZG_6","nc=6","l");
  t_legend -> Draw("same");

  c1 -> SaveAs("zg_nc3456"+jetRoot+".png");
//  fOut -> Write();
//  fOut -> Close();
  return;

}//end readCondor
