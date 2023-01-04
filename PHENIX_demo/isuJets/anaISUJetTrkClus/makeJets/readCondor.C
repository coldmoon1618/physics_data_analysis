/**
 *@Version: single-ckin(3),Run12pp200sim,pre-array makeJets
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
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>
void readCondor(){

//~~~~~~~~~~~~~~~~~ TChain for trees
  TChain * rJets = new TChain("rJets");
  TChain * tJets = new TChain("tJets");
  TChain * mJets = new TChain("mJets");
//~~~~~~~~~~~~~~~~ Input filename structure, individual style 
  TString myDir  = "/phenix/mpcex/mrpind/Run12Sim/output_ISUJet/"; 
  TString jetRoot = "/Run12SimJetAna.root";
  
//~~~~~~~~~~~~~~~~ counters
  unsigned long nFiles = 0;
  unsigned long long rEntries = 0;
  unsigned long long tEntries = 0;
  unsigned long long mEntries = 0;
  unsigned long long rCounter = 0;
  unsigned long long tCounter = 0;
  unsigned long long mCounter = 0;
//~~~~~~~~~~~~~~~~ Loops to add files to the trees
  unsigned long ini = 0;//initial index
  unsigned long fin = 100;//final index
  //unsigned long fin = 49999;//final index
  cout<<"Reading in "<<myDir<<" from "<< ini << " to "<< fin << endl;
  for(unsigned long fInd = ini; fInd <= fin; fInd++){
    TString jet_fname = myDir;
    jet_fname += fInd;
    jet_fname += jetRoot;
    cout<<fInd<<" ";
  
    // skip bad files
    // You'd still get error messages
    // But it won't get added
    // to TChain or the counter
    TFile fIn(jet_fname);
    if(fIn.IsZombie()) {fIn.Close();cout<<"Zombine file skipped"<<endl; continue;}
    if(fIn.GetSize()<12000){fIn.Close(); cout<<"Small file skipped"<<endl;continue;}
    fIn.Close();
      
    rJets  ->AddFile(jet_fname);
    tJets  ->AddFile(jet_fname);
    mJets  ->AddFile(jet_fname);

    nFiles++;
   }//end index
    cout<<endl<<"Total good files: "<<nFiles<<endl;

//~~~~~~~~~~~~~~~~ One output file to contain them all
//Declare fOut after fIn is closed.
  TFile *fOut = new TFile("Run12JetAna_pre_array.root","RECREATE");

//~~~~~~~~~~~histos
  TH1F *H_T_pT  = new TH1F("H_T_pT",  "pT distribution for truth Jets",140,0,140);
 
  TH2F *H_ZGs     = new TH2F("H_ZGs",     "ZG correlation between matched recoJet and trueJet",32,0,0.64,32,0,0.64);
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
  float T_z_g;
  tJets->SetBranchAddress("z_g", &T_z_g);
  int   T_nc; 
  tJets->SetBranchAddress("nc",  &T_nc);
  float T_pT; 
  tJets->SetBranchAddress("pT",  &T_pT);
  float T_eta; 
  tJets->SetBranchAddress("eta", &T_eta);
   
  for(unsigned long long i=0; i<tEntries; i++){
    tJets->GetEntry(i); 
   
    if(T_pT<6.0 || T_nc<3) continue;
    if(abs(T_eta)>0.3) continue;
    if(T_z_g>0.55) continue;

    H_T_pT   -> Fill(T_pT);
      
    tCounter++;
  }//end fill tJets for nth CKIN
  cout<<"tCounter = "<<tCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~tJets}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets{
  float R_z_g;
  rJets->SetBranchAddress("z_g", &R_z_g);
  int   R_nc; 
  rJets->SetBranchAddress("nc",  &R_nc);
  float R_pT; 
  rJets->SetBranchAddress("pT",  &R_pT);
  float R_eta; 
  rJets->SetBranchAddress("eta", &R_eta);
  float R_cf; 
  rJets->SetBranchAddress("cf",  &R_cf);
   
  for(unsigned long long i=0; i<rEntries; i++){
    rJets->GetEntry(i); 
    
    if(R_cf<0.1 || R_cf>0.8 || R_pT<6.0 || R_nc<3) continue;
    if(abs(R_eta)>0.3) continue;
    if(R_z_g>0.55) continue;
    
    rCounter++;
  }//end fill rJets for nth CKIN
  cout<<"rCounter = "<<rCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets{
  float M_r_z_g; 
  mJets->SetBranchAddress("r_z_g",&M_r_z_g);
  float M_t_z_g; 
  mJets->SetBranchAddress("t_z_g",&M_t_z_g);
  float M_r_cf; 
  mJets->SetBranchAddress("r_cf",  &M_r_cf);
  int   M_r_nc; 
  mJets->SetBranchAddress("r_nc",  &M_r_nc);
  float M_r_pT; 
  mJets->SetBranchAddress("r_pT",  &M_r_pT);
  float M_t_pT; 
  mJets->SetBranchAddress("t_pT",  &M_t_pT);
  float M_r_eta; 
  mJets->SetBranchAddress("r_eta", &M_r_eta);
  float M_t_eta; 
  mJets->SetBranchAddress("t_eta", &M_t_eta);
   
  for(unsigned long long i=0; i<mEntries; i++){
    mJets->GetEntry(i); 
    
    if(M_r_cf<0.1 || M_r_cf>0.8 || M_r_pT<6.0 || M_r_nc<3) continue;
    if(abs(M_r_eta)>0.3 || abs(M_t_eta)>0.3) continue;
    if(M_t_z_g>0.55 || M_r_z_g>0.55) continue;  
    mCounter++;

    H_ZGs      -> Fill(M_r_z_g,M_t_z_g);
    H_PTs      -> Fill(M_r_pT,M_t_pT);
  }//end fill mJets for nth CKIN
   
    cout<<"mCounter = "<<mCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets}

  fOut -> Write();
  fOut -> Close();
  return;

}//end readCondor
