#include <iostream>
#include <fstream>
#include <iomanip>

#include <TChain.h>
#include <TMath.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1.h>

float sub_dR(float eta1, float eta2, float phi1, float phi2){
  float sub_deta = eta1 - eta2;
  float sub_dphi = phi1 - phi2;

  return sqrt(sub_dphi * sub_dphi + sub_deta * sub_deta);
}//opening angle

float dphiReduce(float dphi){
  if (dphi < -TMath::Pi()) 
    dphi += TMath::TwoPi();
  if (dphi >=  TMath::Pi()) 
    dphi -= TMath::TwoPi();
  return dphi; 
}//end dphireduce()

void readDataTree()
{
  int runno = 13;
//~~~~~~~~~~~~~~~~~ TChain for trees
  TChain *rJets = new TChain("TReco");
  unsigned long long rEntries;
  TCanvas *C_re_nc = new TCanvas("C_re_nc","re_nc",500,500);
  TCanvas *C_re_zg = new TCanvas("C_re_zg","re_zg",500,500);
//~~~~~~~~~~~~~~~~~ 
//~~~~~~~~~~~~~~~~
  TString jet_fname = "/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13_data.root";
  TFile fIn(jet_fname);
  if(fIn.IsZombie()) {fIn.Close();cout<<"Zombine file skipped"<<endl;}
  //if(fIn.GetSize()<121000){fIn.Close(); cout<<"Small file skipped"<<endl;continue;}
  fIn.Close();
    
  rJets  ->AddFile(jet_fname);

  rEntries = rJets->GetEntries();

  cout<<"rEntries = "<<rEntries<<endl;

  cout<<endl; 

  TH1F *h_re_nc	= new TH1F("h_re_nc", "re_nc",20,0,20);
  TH1F *h_re_nc_pT10	= new TH1F("h_re_nc_pT10", "re_nc",20,0,20);
  TH1F *h_re_nc_pT20	= new TH1F("h_re_nc_pT20", "re_nc",20,0,20);

  TH1F *h_re_zg	= new TH1F("h_re_zg", "re_zg",32,0,0.64);
  TH1F *h_re_zg_pT10	= new TH1F("h_re_zg_pT10", "re_zg",32,0,0.64);
  TH1F *h_re_zg_pT20	= new TH1F("h_re_zg_pT20", "re_zg",32,0,0.64);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets{
  float re_pT; 
  rJets->SetBranchAddress("pT",  &re_pT);
  float re_eta; 
  rJets->SetBranchAddress("eta", &re_eta);
  float re_phi; 
  rJets->SetBranchAddress("phi", &re_phi);
  int   re_nc; 
  rJets->SetBranchAddress("nc",  &re_nc);
  float re_cf; 
  rJets->SetBranchAddress("cf",  &re_cf);

  float re_z_g; 
  rJets->SetBranchAddress("z_g",&re_z_g);
  float re_pT1; 
  rJets->SetBranchAddress("pT1",  &re_pT1);
  float re_pT2; 
  rJets->SetBranchAddress("pT2",  &re_pT2);

  int   re_nc1; 
  rJets->SetBranchAddress("nc1",  &re_nc1);
  int   re_nc2; 
  rJets->SetBranchAddress("nc2",  &re_nc2);

  float re_eta1; 
  rJets->SetBranchAddress("eta1",  &re_eta1);
  float re_eta2; 
  rJets->SetBranchAddress("eta2",  &re_eta2);
  float re_phi1; 
  rJets->SetBranchAddress("phi1",  &re_phi1);
  float re_phi2; 
  rJets->SetBranchAddress("phi2",  &re_phi2);

  float re_oang;
  rJets->SetBranchAddress("oang",  &re_oang);

  float Evnt_vertex; 
  rJets->SetBranchAddress("vertex", &Evnt_vertex);

  for(unsigned long long i=0; i<rEntries; i++){
    rJets->GetEntry(i); 
    //Basic cuts [0.3<cf<0.7 already applied]
    if(Evnt_vertex>30.0)continue;
    //if(re_nc < 5)continue;
    if(re_nc < 3)continue;
    if(re_nc<5&&re_oang<0.25)continue;
    if(re_z_g<0) continue;  
      h_re_nc		-> Fill(re_nc);
      h_re_zg		-> Fill(re_z_g);
    if(re_pT<10.0) continue;
      h_re_nc_pT10	-> Fill(re_nc);
      h_re_zg_pT10	-> Fill(re_z_g);
    if(re_pT<20.0) continue;
      h_re_nc_pT20	-> Fill(re_nc);
      h_re_zg_pT20	-> Fill(re_z_g);
    
    //Check zg 0.1-0.2 what opening angle looks like
    //Opening angle
    float re_dR = sub_dR(re_eta1,re_eta2,re_phi1,re_phi2);
    //cout<<"re_dR="<<re_dR<<" ,re_oang="<<re_oang<<" ratio="<<re_oang/re_dR<<endl;

    if(re_nc==3)
    {
      ;
    }
    if(re_nc==4)
    {
      ;
    }
    if(re_nc==5)
    {
      ;
    }
    {
    }

  }//for entries
  C_re_nc		-> cd();
  gStyle		-> SetOptStat(10);
  h_re_nc		-> SetLineColor(kBlack);
  h_re_nc		-> Draw();
  h_re_nc_pT10		-> SetLineColor(kBlue);
  h_re_nc_pT10		-> Draw("same");
  h_re_nc_pT20		-> SetLineColor(kRed);
  h_re_nc_pT20		-> Draw("same");

  C_re_zg		-> cd();
  gStyle		-> SetOptStat(10);
  h_re_zg		-> SetLineColor(kBlack);
  h_re_zg		-> Draw();
  h_re_zg_pT10		-> SetLineColor(kBlue);
  h_re_zg_pT10		-> Draw("same");
  h_re_zg_pT20		-> SetLineColor(kRed);
  h_re_zg_pT20		-> Draw("same");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets}
  C_re_nc		-> SaveAs("data_NC.png");
  C_re_zg		-> SaveAs("data_zg.png");


  return;

}//end readDataTree
