/**
 * Reads in the histos already in the JetAna output and put them into .root files
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
#include <TList.h>
#include <TObject.h>
//void splitTruthJets(TString jetRoot, unsigned long ini, unsigned long fin){
void readHistos(int type, unsigned long ind){
  TString jetRoot = "_";
  if(type==1) jetRoot += "raw";
  else        jetRoot += "phi_eta_cuts";
  unsigned long ini = ind*10000;
  unsigned long fin = ind*10000+9999;
//~~~~~~~~~~~~~~~~~ TChain for trees
  TChain * mJets = new TChain("mJets");
//~~~~ histos
  TH2F *H_ZGs     = new TH2F("H_ZGs",     "ZG between matched rJets and tJets",32,0,.64,32,0,.64);
  H_ZGs->GetXaxis()->SetTitle("Reco Jet ZG");
  H_ZGs->GetYaxis()->SetTitle("True Jet ZG");

  TH1F *H_MR_ZG_3  = new TH1F("H_MR_ZG_3",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *H_MR_ZG_4  = new TH1F("H_MR_ZG_4",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *H_MR_ZG_5  = new TH1F("H_MR_ZG_5",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *H_MR_ZG_6  = new TH1F("H_MR_ZG_6",  "z_g of matched reco Jets",32,0,0.64);

  TH1F *H_MT_ZG_3  = new TH1F("H_MT_ZG_3",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *H_MT_ZG_4  = new TH1F("H_MT_ZG_4",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *H_MT_ZG_5  = new TH1F("H_MT_ZG_5",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *H_MT_ZG_6  = new TH1F("H_MT_ZG_6",  "z_g of matched truth Jets",32,0,0.64);

  TH1F *H_mt_nc_r3  = new TH1F("H_mt_nc_r3", "nc for mtJets when mr_nc=3",15,0,15);
  TH1F *H_mt_nc_r4  = new TH1F("H_mt_nc_r4", "nc for mtJets when mr_nc=4",15,0,15);
  TH1F *H_mt_nc_r5  = new TH1F("H_mt_nc_r5", "nc for mtJets when mr_nc=5",15,0,15);
  TH1F *H_mt_nc_r6  = new TH1F("H_mt_nc_r6", "nc for mtJets when mr_nc=6",15,0,15);

  TH1F *H_tra_phi = new TH1F("H_tra_phi","phi distr for all charged particles used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *H_clu_phi = new TH1F("H_clu_phi","phi distr for all neutral particles used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *H_cha_phi = new TH1F("H_cha_phi","phi distr for all charged particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *H_neu_phi = new TH1F("H_neu_phi","phi distr for all neutral particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());

  TH1F *H_tra_eta = new TH1F("H_tra_eta","eta distr for all tracks used in reco jets",100,-5,5);
  TH1F *H_clu_eta = new TH1F("H_clu_eta","eta distr for all clusters used in reco jets",100,-5,5);  
  TH1F *H_neu_eta = new TH1F("H_neu_eta","eta distr for all neutral particles used in truth jets",100,-5,5);  
  TH1F *H_cha_eta = new TH1F("H_cha_eta","eta distr for all charged particles used in truth jets",100,-5,5);  

  TH1F *H_mrJet_phi = new TH1F("H_mrJet_phi","phi distribution for matched reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *H_mtJet_phi = new TH1F("H_mtJet_phi","phi distribution for matched truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  
  TH1F *H_mrJet_eta = new TH1F("H_mrJet_eta","eta distribution for matched reco jets",100,-5,5);
  TH1F *H_mtJet_eta = new TH1F("H_mtJet_eta","eta distribution for matched truth jets",100,-5,5);

//~~~~~~~~~~~~~~~~ Input filename structure, individual style 
  TString myDir  = "/sphenix/user/yzhai/Run12pp_zg/output/"; 
  
//~~~~~~~~~~~~~~~~ counters
  unsigned long nFiles = 0;
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
    
    H_tra_phi -> Add((TH1F*)fIn.Get("h_track_phi"));
    H_clu_phi -> Add((TH1F*)fIn.Get("h_cluster_phi"));
    H_cha_phi -> Add((TH1F*)fIn.Get("h_charged_phi"));
    H_neu_phi -> Add((TH1F*)fIn.Get("h_neutral_phi"));

    H_tra_eta -> Add((TH1F*)fIn.Get("h_track_eta"));
    H_clu_eta -> Add((TH1F*)fIn.Get("h_cluster_eta"));
    H_cha_eta -> Add((TH1F*)fIn.Get("h_charged_eta"));
    H_neu_eta -> Add((TH1F*)fIn.Get("h_neutral_eta"));
    fIn.Close();
      
    mJets  ->AddFile(jet_fname);

    nFiles++;
   }//end index
    
  cout<<endl<<"Total good files: "<<nFiles<<endl;
  TString outname = "nc3456"+jetRoot+"_"+ind+".root";
  TFile *fOut = new TFile(outname,"RECREATE");

//~~~~~~~~~~~~~~~~
  mEntries = mJets->GetEntries();
  
  cout<<"mEntries = "<<mEntries<<endl;
  cout<<endl; 
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
  float M_r_phi[maxMatJets]; 
  mJets->SetBranchAddress("r_phi", M_r_phi);
  float M_t_phi[maxMatJets]; 
  mJets->SetBranchAddress("t_phi", M_t_phi);
   
  for(unsigned long long i=0; i<mEntries; i++){
    mJets->GetEntry(i); 
    
    for(int iJet = 0; iJet < nMatchedJets; iJet++){ 
      //MATCHED JET CUTS
      if(M_r_cf[iJet]<0.1 || M_r_cf[iJet]>0.8 || M_r_pT[iJet]<6.0 || M_r_nc[iJet]<3) continue;
      if(M_t_z_g[iJet]>0.55 || M_r_z_g[iJet]>0.55) continue;  
      H_ZGs      -> Fill(M_r_z_g[iJet],M_t_z_g[iJet]);

      if(M_r_nc[iJet]==3){
        H_MR_ZG_3      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_3      -> Fill(M_t_z_g[iJet]);
        H_mt_nc_r3     -> Fill(M_t_nc[iJet]);
      }
      else if(M_r_nc[iJet]==4){
        H_MR_ZG_4      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_4      -> Fill(M_t_z_g[iJet]);
        H_mt_nc_r4     -> Fill(M_t_nc[iJet]);
      }
      else if(M_r_nc[iJet]==5){
        H_MR_ZG_5      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_5      -> Fill(M_t_z_g[iJet]);
        H_mt_nc_r5     -> Fill(M_t_nc[iJet]);
      }
      else if(M_r_nc[iJet]==6){
        H_MR_ZG_6      -> Fill(M_r_z_g[iJet]);
        H_MT_ZG_6      -> Fill(M_t_z_g[iJet]);
        H_mt_nc_r6     -> Fill(M_t_nc[iJet]);
      }

      H_mrJet_phi -> Fill(M_r_phi[iJet]);
      H_mtJet_phi -> Fill(M_t_phi[iJet]);
      H_mrJet_eta -> Fill(M_r_eta[iJet]);
      H_mtJet_eta -> Fill(M_t_eta[iJet]);

      mCounter++;
    }//event level, nJets
  
  }//end fill mJets for nth CKIN
   
    cout<<"mCounter = "<<mCounter<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets}
  H_ZGs     -> Write("H_ZGs");

  H_MR_ZG_3 -> Write("H_MR_ZG_3");
  H_MR_ZG_4 -> Write("H_MR_ZG_4");
  H_MR_ZG_5 -> Write("H_MR_ZG_5"); 
  H_MR_ZG_6 -> Write("H_MR_ZG_6");

  H_MT_ZG_3 -> Write("H_MT_ZG_3");
  H_MT_ZG_4 -> Write("H_MT_ZG_4");
  H_MT_ZG_5 -> Write("H_MT_ZG_5");
  H_MT_ZG_6 -> Write("H_MT_ZG_6");

  H_mt_nc_r3 -> Write("H_mt_nc_r3");
  H_mt_nc_r4 -> Write("H_mt_nc_r4");
  H_mt_nc_r5 -> Write("H_mt_nc_r5");
  H_mt_nc_r6 -> Write("H_mt_nc_r6");

  H_tra_phi  -> Write("H_tra_phi");
  H_clu_phi  -> Write("H_clu_phi");
  H_cha_phi  -> Write("H_cha_phi");
  H_neu_phi  -> Write("H_neu_phi");

  H_tra_eta  -> Write("H_tra_eta");
  H_clu_eta  -> Write("H_clu_eta"); 
  H_neu_eta  -> Write("H_neu_eta"); 
  H_cha_eta  -> Write("H_cha_eta"); 

  H_mrJet_phi-> Write("H_mrJet_phi");
  H_mtJet_phi-> Write("H_mtJet_phi");
  
  H_mrJet_eta-> Write("H_mrJet_eta");
  H_mtJet_eta-> Write("H_mtJet_eta");
  
  //fOut -> Write();
  fOut -> Close();
  cout<<"Output written in file "<<outname<<endl;
  return;

}//end readCondor
