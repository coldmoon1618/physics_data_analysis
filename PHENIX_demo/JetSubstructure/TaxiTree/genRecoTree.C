#include <iostream>
#include <fstream>

#include "TTree.h"
#include "TChain.h"
#include <TString.h>
#include "TFile.h"
#include "TH1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TRandom3.h"

//````````````````````````reco{
static const int kMaxRecoJets = 100;

unsigned int nRecoJets;
float re_vertex;

bool  re_ertFired[kMaxRecoJets];

float re_pT[kMaxRecoJets];
float re_eta[kMaxRecoJets];
float re_phi[kMaxRecoJets];
int   re_nc[kMaxRecoJets]; 
int   re_chgd_nc[kMaxRecoJets]; 
int   re_neut_nc[kMaxRecoJets]; 
float re_cf[kMaxRecoJets];

  //z_g stuff
float re_z_g[kMaxRecoJets];
float re_pT1[kMaxRecoJets];
float re_eta1[kMaxRecoJets];
float re_phi1[kMaxRecoJets];
int   re_nc1[kMaxRecoJets];
int   re_chgd_nc1[kMaxRecoJets]; 
int   re_neut_nc1[kMaxRecoJets]; 
float re_cf1[kMaxRecoJets];
float re_pT2[kMaxRecoJets];
float re_eta2[kMaxRecoJets];
float re_phi2[kMaxRecoJets];
int   re_nc2[kMaxRecoJets];
int   re_chgd_nc2[kMaxRecoJets]; 
int   re_neut_nc2[kMaxRecoJets]; 
float re_cf2[kMaxRecoJets];
float re_oang[kMaxRecoJets];
int   re_iter[kMaxRecoJets];

float re_z_g_out;
float re_pT_out;
float re_eta_out;
float re_phi_out;
int   re_nc_out;
int   re_chgd_nc_out;
int   re_neut_nc_out;
float re_cf_out;
float re_neut_pT_out;
float re_chgd_pT_out;

float re_pT1_out;
float re_eta1_out;
float re_phi1_out;
int   re_nc1_out;
int   re_chgd_nc1_out;
int   re_neut_nc1_out;
float re_cf1_out;
float re_pT2_out;
float re_eta2_out;
float re_phi2_out;
int   re_nc2_out;
int   re_chgd_nc2_out;
int   re_neut_nc2_out;
float  re_cf2_out;

float re_oang_out;
int   re_iter_out;
//````````````````````````reco}
void LinkRecoVars(TTree *tReco){

  tReco->SetBranchAddress("nRecoJets", &nRecoJets);
  tReco->SetBranchAddress("vertex", &re_vertex);

  tReco->SetBranchAddress("ertFired", re_ertFired);

  tReco->SetBranchAddress("pT", re_pT);
  tReco->SetBranchAddress("eta", re_eta);
  tReco->SetBranchAddress("phi", re_phi);
  tReco->SetBranchAddress("nc", re_nc);
  tReco->SetBranchAddress("chgd_nc", re_chgd_nc);
  tReco->SetBranchAddress("neut_nc", re_neut_nc);
  tReco->SetBranchAddress("cf", re_cf);
 
  tReco->SetBranchAddress("z_g", re_z_g);
  tReco->SetBranchAddress("pT1", re_pT1);
  tReco->SetBranchAddress("eta1", re_eta1);
  tReco->SetBranchAddress("phi1", re_phi1);
  tReco->SetBranchAddress("nc1", re_nc1);
  tReco->SetBranchAddress("chgd_nc1", re_chgd_nc1);
  tReco->SetBranchAddress("neut_nc1", re_neut_nc1);
  tReco->SetBranchAddress("cf1", re_cf1);
  tReco->SetBranchAddress("pT2", re_pT2);
  tReco->SetBranchAddress("eta2", re_eta2);
  tReco->SetBranchAddress("phi2", re_phi2);
  tReco->SetBranchAddress("nc2", re_nc2);
  tReco->SetBranchAddress("chgd_nc2", re_chgd_nc2);
  tReco->SetBranchAddress("neut_nc2", re_neut_nc2);
  tReco->SetBranchAddress("cf2", re_cf2);
  tReco->SetBranchAddress("oang", re_oang);
  tReco->SetBranchAddress("iter", re_iter);

  return;

}//LinkRecoVars

void SetNewRecoTree(TTree *tNew){

  tNew->Branch("vertex",&re_vertex,"vertex/F");

  tNew->Branch("pT",	&re_pT_out,	"pT_out/F"); 
  tNew->Branch("eta",	&re_eta_out,	"eta_out/F"); 
  tNew->Branch("phi",	&re_phi_out,	"phi_out/F"); 
  tNew->Branch("nc",	&re_nc_out,	"nc_out/I"); 
  tNew->Branch("cf",	&re_cf_out,	"cf_out/F"); 
  tNew->Branch("neut_pT", &re_neut_pT_out, "neut_pT_out/F"); 
  tNew->Branch("chgd_pT", &re_chgd_pT_out, "chgd_pT_out/F"); 
  
  tNew->Branch("z_g",	&re_z_g_out,	"z_g_out/F"); 
  tNew->Branch("pT1",	&re_pT1_out,	"pT1_out/F"); 
  tNew->Branch("eta1",	&re_eta1_out,	"eta1_out/F"); 
  tNew->Branch("phi1",	&re_phi1_out,	"phi1_out/F"); 
  tNew->Branch("nc1",	&re_nc1_out,	"nc1_out/I"); 
  tNew->Branch("chgd_nc1",	&re_chgd_nc1_out,	"nc1_chgd_out/I"); 
  tNew->Branch("neut_nc1",	&re_neut_nc1_out,	"nc1_neu_out/I"); 
  tNew->Branch("cf1",	&re_cf1_out,	"nc1_out/F"); 
  tNew->Branch("pT2",	&re_pT2_out,	"pT2_out/F"); 
  tNew->Branch("eta2",	&re_eta2_out,	"eta2_out/F"); 
  tNew->Branch("phi2",	&re_phi2_out,	"phi2_out/F"); 
  tNew->Branch("nc2",	&re_nc2_out,	"nc2_out/I"); 
  tNew->Branch("chgd_nc2",	&re_chgd_nc2_out,	"nc2_chgd_out/I"); 
  tNew->Branch("neut_nc2",	&re_neut_nc2_out,	"nc2_neu_out/I"); 
  tNew->Branch("cf2",	&re_cf2_out,	"nc2_out/F"); 
  tNew->Branch("oang",	&re_oang_out,	"oang_out/F"); 
  tNew->Branch("iter",	&re_iter_out,	"iter_out/I"); 

}//SetNewRecoTree

bool isNAN(float tNum){

  if( !((tNum>0.0) || (tNum<0.0) || (tNum==0.0)) ){

    return true;
  }
  else{
    return false;
  }

}//isNAN

bool isNAN(int tNum){

  if( !((tNum>0) || (tNum<0) || (tNum==0)) ){

    return true;
  }
  else{
    return false;
  }

}//isNAN

int getArm(float phi) {

  if (phi >= -TMath::PiOver2() && phi < TMath::PiOver2()) return 0; 
  if (phi >=  TMath::PiOver2() && phi < 3.0*TMath::PiOver2()) return 0; 
  return -1;

}

void genRecoTree(int runno=13)
{

  if( (runno!=12) && (runno!=13) ){
    cout << "ERROR: BAD runno = " << runno << endl;
    return; 
  }

  TChain *tMatched	= new TChain("mJets"); 
  TChain *tReco		= new TChain("rJets");

  int goodrunno;
    ifstream goodrunnofile("/sphenix/user/yzhai/Dch_QA/notes/newRunList.txt");
//~~~~~~~~~~~~~~~~ Input filename structure 
int counter = 0;//for testing on smaller amounts
  if(runno==13){
    cout<<"Run13pp510"<<endl;
    while(goodrunnofile>>goodrunno){
      //if(counter<10)
      {
	//counter++;cout<<"counter="<<counter<<endl;
	//TString fname  = "/sphenix/user/yzhai/JetSubStructure/TaxiTree/jetOut_run13/";
	TString fname  = "/sphenix/user/yzhai/JetSubStructure/TaxiTree/jetOut_run13MB/";
		fname += goodrunno;
		fname += "_out.root";
	cout<<fname<<endl;
	TFile fIn(fname);
	if(fIn.IsZombie()) {fIn.Close();cout<<"Zombine file skipped"<<endl; continue;}
	tReco -> Add(fname);

	fIn.Close();
      }
     }//end index

  }//end if runno==13
  LinkRecoVars(tReco);

  //TFile *fOut = new TFile("/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13_data.root","recreate");
  //TFile *fOut = new TFile("/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13MB_data.root","recreate");
  TFile *fOut = new TFile("/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13MB_data_ertFired.root","recreate");

  TTree *tNewReco = new TTree("TReco", "Reco Tree Highest pT");
  SetNewRecoTree(tNewReco);

//````````````````````````reco{
  int rEntries = tReco->GetEntries();
  cout << "Examining " << rEntries << " events (reco jets) ...." << endl;

  for(int rEnt = 0; rEnt < rEntries; rEnt++){
    tReco->GetEntry(rEnt);
    if(nRecoJets<=0) continue;
    if(nRecoJets>kMaxRecoJets){
      cout << "ERROR: nRecoJets OVERFLOW!" << endl;
    }

    if(abs(re_vertex)>30.0) continue;

    // Get the maxiumum pT reco jet
    int idx_max = -1; 
    float pT_max = -9999.0; 

    for(unsigned int rJet = 0; rJet < nRecoJets; rJet++){
      if(re_pT[rJet] <= 0) continue;
      if(re_ertFired[rJet]==0)continue;
      if(re_pT[rJet]>pT_max) {
	pT_max = re_pT[rJet]; 
	idx_max = rJet;
      }
    }
    if(idx_max<0) continue; 

    int iarm = getArm(re_eta[idx_max]); 
    if( (iarm<0) || (iarm>1)) continue;

    //check for NaN before filling
    if( isNAN(re_pT[idx_max]) || isNAN(re_eta[idx_max]) || isNAN(re_phi[idx_max]) || 
        isNAN(re_nc[idx_max]) || isNAN(re_cf[idx_max])  ) continue;
    
    re_pT_out	= re_pT[idx_max];
    re_eta_out	= re_eta[idx_max];
    re_phi_out	= re_phi[idx_max];
    re_nc_out	= re_nc[idx_max];
    re_chgd_nc_out	= re_chgd_nc[idx_max];
    re_neut_nc_out	= re_neut_nc[idx_max];
    re_cf_out	= re_cf[idx_max];

    re_z_g_out	= re_z_g[idx_max];
    re_pT1_out	= re_pT1[idx_max];
    re_eta1_out	= re_eta1[idx_max];
    re_phi1_out	= re_phi1[idx_max];
    re_nc1_out	= re_nc1[idx_max];
    re_chgd_nc1_out	= re_chgd_nc1[idx_max];
    re_neut_nc1_out	= re_neut_nc1[idx_max];
    re_cf1_out	= re_cf1[idx_max];
    re_pT2_out	= re_pT2[idx_max];
    re_eta2_out	= re_eta2[idx_max];
    re_phi2_out	= re_phi2[idx_max];
    re_nc2_out	= re_nc2[idx_max];
    re_chgd_nc2_out	= re_chgd_nc2[idx_max];
    re_neut_nc2_out	= re_neut_nc2[idx_max];
    re_cf2_out	= re_cf2[idx_max];

    re_oang_out = re_oang[idx_max];
    re_iter_out = re_iter[idx_max];

    re_neut_pT_out = (1.0 - re_cf_out)*re_pT_out;
    re_chgd_pT_out = re_cf_out*re_pT_out;
    
    tNewReco->Fill();
  }//rEntries

//````````````````````````reco}

  fOut->cd();
  fOut->Write();
  fOut->Close();

  return;

}//genRecoTree
