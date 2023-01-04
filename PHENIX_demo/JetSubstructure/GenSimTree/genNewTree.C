//WARNING: made mistake on recoTree making, so the reco 
//trees are not reliable
//
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include <iostream>
#include "TH1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TRandom3.h"

//````````````````````````matched{
//For makejets_hadrons.root inputs
//event level info
static const int kMaxMatchedJets = 100;

unsigned int nMatchedJets;
float recoRadius;//jet info
float truthRadius;//jet info
float minPt_jets;//jet info
float evt_x1;
float evt_x2;
float evt_Qsqr;
int   process_id;
int   eventNumber;
float vertex;

//iniial anti-kt jet info
float r_pT[kMaxMatchedJets];
float r_eta[kMaxMatchedJets];
float r_phi[kMaxMatchedJets];
int   r_nc[kMaxMatchedJets];
int   r_chgd_nc[kMaxMatchedJets];
int   r_neut_nc[kMaxMatchedJets];
float r_cf[kMaxMatchedJets];

float t_pT[kMaxMatchedJets];
float t_eta[kMaxMatchedJets];
float t_phi[kMaxMatchedJets];
int   t_nc[kMaxMatchedJets];
int   t_chgd_nc[kMaxMatchedJets];
int   t_neut_nc[kMaxMatchedJets];
float t_cf[kMaxMatchedJets];

//jet substructure info
float r_z_g[kMaxMatchedJets]; 
float r_pT1[kMaxMatchedJets]; 
float r_eta1[kMaxMatchedJets]; 
float r_phi1[kMaxMatchedJets]; 
int   r_nc1[kMaxMatchedJets]; 
int   r_chgd_nc1[kMaxMatchedJets];
int   r_neut_nc1[kMaxMatchedJets];
float r_cf1[kMaxMatchedJets];
float r_pT2[kMaxMatchedJets]; 
float r_eta2[kMaxMatchedJets]; 
float r_phi2[kMaxMatchedJets]; 
int   r_nc2[kMaxMatchedJets]; 
int   r_chgd_nc2[kMaxMatchedJets];
int   r_neut_nc2[kMaxMatchedJets];
float r_cf2[kMaxMatchedJets];
float r_oang[kMaxMatchedJets];

float t_z_g[kMaxMatchedJets]; 
float t_pT1[kMaxMatchedJets]; 
float t_eta1[kMaxMatchedJets]; 
float t_phi1[kMaxMatchedJets]; 
int   t_nc1[kMaxMatchedJets]; 
int   t_chgd_nc1[kMaxMatchedJets];
int   t_neut_nc1[kMaxMatchedJets];
float t_cf1[kMaxMatchedJets];
float t_pT2[kMaxMatchedJets]; 
float t_eta2[kMaxMatchedJets]; 
float t_phi2[kMaxMatchedJets]; 
int   t_nc2[kMaxMatchedJets]; 
int   t_chgd_nc2[kMaxMatchedJets];
int   t_neut_nc2[kMaxMatchedJets];
float t_cf2[kMaxMatchedJets];
float t_oang[kMaxMatchedJets];

int r_iter[kMaxMatchedJets]; 
int t_iter[kMaxMatchedJets]; 

float dphi[kMaxMatchedJets]; 
float deta[kMaxMatchedJets]; 
float dR[kMaxMatchedJets]; 

//for generated New Tree
//evt level info directly filled

float r_pT_out;
float r_eta_out;
float r_phi_out;
int   r_nc_out;
int   r_chgd_nc_out;
int   r_neut_nc_out;
float r_cf_out;

float r_chgd_pT_out; 
float r_neut_pT_out; 

float t_pT_out;
float t_eta_out;
float t_phi_out;
int   t_nc_out;
int   t_chgd_nc_out;
int   t_neut_nc_out;
float t_cf_out;

float t_chgd_pT_out; 
float t_neut_pT_out; 

float r_z_g_out;  
float r_pT1_out; 
float r_eta1_out; 
float r_phi1_out; 
int   r_nc1_out; 
int   r_chgd_nc1_out;
int   r_neut_nc1_out;
float r_cf1_out;
float r_pT2_out; 
float r_eta2_out; 
float r_phi2_out; 
int   r_nc2_out; 
int   r_chgd_nc2_out;
int   r_neut_nc2_out;
float r_cf2_out;
float r_oang_out;

float t_z_g_out;  
float t_pT1_out; 
float t_eta1_out; 
float t_phi1_out; 
int   t_nc1_out; 
int   t_chgd_nc1_out;
int   t_neut_nc1_out;
float t_cf1_out;
float t_pT2_out; 
float t_eta2_out; 
float t_phi2_out; 
int   t_nc2_out; 
int   t_chgd_nc2_out;
int   t_neut_nc2_out;
float t_cf2_out;
float t_oang_out;

int r_iter_out; 
int t_iter_out; 

float dphi_out;
float deta_out; 
float dR_out; 

float ckin_out; 

//````````````````````````matched}
void LinkMatchedVars(TTree *tMatched){

  tMatched->SetBranchAddress("nMatchedJets", &nMatchedJets);
  tMatched->SetBranchAddress("recoRadius", &recoRadius);
  tMatched->SetBranchAddress("truthRadius", &truthRadius);
  tMatched->SetBranchAddress("minPt_jets", &minPt_jets);
  tMatched->SetBranchAddress("evt_x1", &evt_x1);
  tMatched->SetBranchAddress("evt_x2", &evt_x2);
  tMatched->SetBranchAddress("evt_Qsqr", &evt_Qsqr);
  tMatched->SetBranchAddress("process_id", &process_id);
  tMatched->SetBranchAddress("eventNumber", &eventNumber);
  tMatched->SetBranchAddress("vertex", &vertex);

  tMatched->SetBranchAddress("r_pT", r_pT);
  tMatched->SetBranchAddress("r_eta", r_eta);
  tMatched->SetBranchAddress("r_phi", r_phi);
  tMatched->SetBranchAddress("r_nc", r_nc);
  tMatched->SetBranchAddress("r_chgd_nc", r_chgd_nc);
  tMatched->SetBranchAddress("r_neut_nc", r_neut_nc);
  tMatched->SetBranchAddress("r_cf", r_cf);
  
  tMatched->SetBranchAddress("t_pT", t_pT);
  tMatched->SetBranchAddress("t_eta", t_eta);
  tMatched->SetBranchAddress("t_phi", t_phi);
  tMatched->SetBranchAddress("t_nc", t_nc);
  tMatched->SetBranchAddress("t_chgd_nc", t_chgd_nc);
  tMatched->SetBranchAddress("t_neut_nc", t_neut_nc);
  tMatched->SetBranchAddress("t_cf", t_cf);

}
void LinkMatchedVarsCont(TTree *tMatched){
  tMatched->SetBranchAddress("r_z_g", r_z_g);
  tMatched->SetBranchAddress("r_pT1", r_pT1);
  tMatched->SetBranchAddress("r_eta1", r_eta1);
  tMatched->SetBranchAddress("r_phi1", r_phi1);
  tMatched->SetBranchAddress("r_nc1", r_nc1);
  tMatched->SetBranchAddress("r_chgd_nc1", r_chgd_nc1);
  tMatched->SetBranchAddress("r_neut_nc1", r_neut_nc1);
  tMatched->SetBranchAddress("r_cf1", r_cf1);
  tMatched->SetBranchAddress("r_pT2", r_pT2);
  tMatched->SetBranchAddress("r_eta2", r_eta2);
  tMatched->SetBranchAddress("r_phi2", r_phi2);
  tMatched->SetBranchAddress("r_nc2", r_nc2);
  tMatched->SetBranchAddress("r_chgd_nc2", r_chgd_nc2);
  tMatched->SetBranchAddress("r_neut_nc2", r_neut_nc2);
  tMatched->SetBranchAddress("r_cf2", r_cf2);
  tMatched->SetBranchAddress("r_oang", r_oang);

  tMatched->SetBranchAddress("t_z_g", t_z_g);
  tMatched->SetBranchAddress("t_pT1", t_pT1);
  tMatched->SetBranchAddress("t_eta1", t_eta1);
  tMatched->SetBranchAddress("t_phi1", t_phi1);
  tMatched->SetBranchAddress("t_nc1", t_nc1);
  tMatched->SetBranchAddress("t_chgd_nc1", t_chgd_nc1);
  tMatched->SetBranchAddress("t_neut_nc1", t_neut_nc1);
  tMatched->SetBranchAddress("t_cf1", t_cf1);
  tMatched->SetBranchAddress("t_pT2", t_pT2);
  tMatched->SetBranchAddress("t_eta2", t_eta2);
  tMatched->SetBranchAddress("t_phi2", t_phi2);
  tMatched->SetBranchAddress("t_nc2", t_nc2);
  tMatched->SetBranchAddress("t_chgd_nc2", t_chgd_nc2);
  tMatched->SetBranchAddress("t_neut_nc2", t_neut_nc2);
  tMatched->SetBranchAddress("t_cf2", t_cf2);
  tMatched->SetBranchAddress("t_oang", t_oang);

  tMatched->SetBranchAddress("r_iter", r_iter);
  tMatched->SetBranchAddress("t_iter", t_iter);

  tMatched->SetBranchAddress("dphi", dphi);
  tMatched->SetBranchAddress("deta", deta);
  tMatched->SetBranchAddress("dR", dR);

  return;

}//LinkMatchedVars

void SetNewMatchedTree(TTree *tNew){

  tNew->Branch("recoRadius",&recoRadius,"recoRadius/F");
  tNew->Branch("truthRadius",&truthRadius,"truthRadius/F");
  tNew->Branch("minPt_jets",&minPt_jets,"minPt_jets/F");
  tNew->Branch("evt_x1",&evt_x1,"evt_x1/F");
  tNew->Branch("evt_x2",&evt_x2,"evt_x2/F");
  tNew->Branch("process_id",&process_id,"process_id/F");
  tNew->Branch("evt_Qsqr",&evt_Qsqr,"evt_Qsqr/F");
  tNew->Branch("eventNumber",&eventNumber,"eventNumber/I");
  tNew->Branch("vertex",&vertex,"vertex/F");

  tNew->Branch("r_pT", &r_pT_out, "r_pT_out/F"); 
  tNew->Branch("r_eta", &r_eta_out, "r_eta_out/F"); 
  tNew->Branch("r_phi", &r_phi_out, "r_phi_out/F"); 
  tNew->Branch("r_nc", &r_nc_out, "r_nc_out/I"); 
  tNew->Branch("r_chgd_nc", &r_chgd_nc_out, "r_chgd_nc_out/I"); 
  tNew->Branch("r_neut_nc", &r_neut_nc_out, "r_neut_nc_out/I"); 
  tNew->Branch("r_cf", &r_cf_out, "r_cf_out/F"); 

  tNew->Branch("r_chgd_pT", &r_chgd_pT_out, "r_chgd_pT_out/F"); 
  tNew->Branch("r_neut_pT", &r_neut_pT_out, "r_neut_pT_out/F"); 

  tNew->Branch("t_pT", &t_pT_out, "t_pT_out/F"); 
  tNew->Branch("t_eta", &t_eta_out, "t_eta_out/F"); 
  tNew->Branch("t_phi", &t_phi_out, "t_phi_out/F"); 
  tNew->Branch("t_nc", &t_nc_out, "t_nc_out/I"); 
  tNew->Branch("t_chgd_nc", &t_chgd_nc_out, "t_chgd_nc_out/I"); 
  tNew->Branch("t_neut_nc", &t_neut_nc_out, "t_neut_nc_out/I"); 
  tNew->Branch("t_cf", &t_cf_out, "t_cf_out/F"); 

  tNew->Branch("t_chgd_pT", &t_chgd_pT_out, "t_chgd_pT_out/F"); 
  tNew->Branch("t_neut_pT", &t_neut_pT_out, "t_neut_pT_out/F"); 

  tNew->Branch("r_z_g", &r_z_g_out, "r_z_g_out/F"); 
  tNew->Branch("r_pT1", &r_pT1_out, "r_pT1_out/F"); 
  tNew->Branch("r_eta1", &r_eta1_out, "r_eta1_out/F"); 
  tNew->Branch("r_phi1", &r_phi1_out, "r_phi1_out/F"); 
  tNew->Branch("r_nc1", &r_nc1_out, "r_nc1_out/I"); 
  tNew->Branch("r_chgd_nc1", &r_chgd_nc1_out, "r_chgd_nc1_out/I"); 
  tNew->Branch("r_neut_nc1", &r_neut_nc1_out, "r_neut_nc1_out/I"); 
  tNew->Branch("r_cf1", &r_cf1_out, "r_cf1_out/F"); 
  tNew->Branch("r_pT2", &r_pT2_out, "r_pT2_out/F"); 
  tNew->Branch("r_eta2", &r_eta2_out, "r_eta2_out/F"); 
  tNew->Branch("r_phi2", &r_phi2_out, "r_phi2_out/F"); 
  tNew->Branch("r_nc2", &r_nc2_out, "r_nc2_out/I"); 
  tNew->Branch("r_chgd_nc2", &r_chgd_nc2_out, "r_chgd_nc2_out/I"); 
  tNew->Branch("r_neut_nc2", &r_neut_nc2_out, "r_neut_nc2_out/I"); 
  tNew->Branch("r_cf2", &r_cf2_out, "r_cf2_out/F"); 
  tNew->Branch("r_oang", &r_oang_out,"r_oang_out/F");

  tNew->Branch("t_z_g", &t_z_g_out, "t_z_g_out/F"); 
  tNew->Branch("t_pT1", &t_pT1_out, "t_pT1_out/F"); 
  tNew->Branch("t_eta1", &t_eta1_out, "t_eta1_out/F"); 
  tNew->Branch("t_phi1", &t_phi1_out, "t_phi1_out/F"); 
  tNew->Branch("t_nc1", &t_nc1_out, "t_nc1_out/I"); 
  tNew->Branch("t_chgd_nc1", &t_chgd_nc1_out, "t_chgd_nc1_out/I"); 
  tNew->Branch("t_neut_nc1", &t_neut_nc1_out, "t_neut_nc1_out/I"); 
  tNew->Branch("t_cf1", &t_cf1_out, "t_cf1_out/F"); 
  tNew->Branch("t_pT2", &t_pT2_out, "t_pT2_out/F"); 
  tNew->Branch("t_eta2", &t_eta2_out, "t_eta2_out/F"); 
  tNew->Branch("t_phi2", &t_phi2_out, "t_phi2_out/F"); 
  tNew->Branch("t_nc2", &t_nc2_out, "t_nc2_out/I"); 
  tNew->Branch("t_chgd_nc2", &t_chgd_nc2_out, "t_chgd_nc2_out/I"); 
  tNew->Branch("t_neut_nc2", &t_neut_nc2_out, "t_neut_nc2_out/I"); 
  tNew->Branch("t_cf2", &t_cf2_out, "t_cf2_out/F"); 
  tNew->Branch("t_oang", &t_oang_out,"t_oang_out/F");

  tNew->Branch("r_iter", &r_iter_out, "r_iter_out/I"); 
  tNew->Branch("t_iter", &t_iter_out, "t_iter_out/I"); 
  
  tNew->Branch("dphi", &dphi_out, "dphi_out/F"); 
  tNew->Branch("deta", &deta_out, "deta_out/F"); 
  tNew->Branch("dR", &dR_out, "dR_out/F"); 

  tNew->Branch("ckin", &ckin_out, "ckin_out/F");

}//SetNewMatchedTree

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

void genNewTree(int CKIN, int runno=13, bool isLate = false)
{

  if( (runno!=12) && (runno!=13) ){
    cout << "ERROR: BAD runno = " << runno << endl;
    return; 
  }

  TString fileLoc =  Form("/phenix/mpcex/lajoie/ISU/SimOut/Run%i_PP/510/ckin%i", runno, CKIN);
  //TString fileLoc =  Form("/phenix/mpcex/lajoie/ISU/SimOut/Run%i_PP/200/ckin%i", runno, CKIN);
  
  TChain *tMatched	= new TChain("mJets"); 

  if(runno==12){
    cout<<"Run12pp200"<<endl;

    const int	nRuns = 9; 
    int ckins[nRuns]  =	{      10,	  20,	       30,	    
			       40,	  50,	       60,	   
			       70,	  80,	       90};
			
    int nFiles[nRuns] =	{   20000,      20000,      20000,      
			    20000,      20000,      20000,      
			    20000,      20000,      20000}; 
    int   index = -1;
    for(int i = 0; i < 9; i++){
      if(CKIN == ckins[i]){
	index = i;
	break;
      }
    }//i

    if(index<0) {
      cout << "ERROR: no CKIN index found." << endl;
      return; 
    }
  
    for(int iter=1; iter<nFiles[index]; iter++)
    {
      tMatched	->Add(Form("%s/%i/makejets_%s.root", fileLoc.Data(), iter, "hadrons"));
    }

  }//end if runno==12
  if(runno==13){
    cout<<"Run13pp510"<<endl;

    const int	nRuns = 15; 
    int ckins[nRuns]  =	{       5,	  10,	       20,	   30,	       50,
			       80,       100,         120,	  135,	      150,
			      170,       190,	      210,	  225,	      240};
    int nFiles[nRuns] = {  20000,      20000,      20000,      20000,      40000,
			   40000,      40000,      60000,      60000,      60000,
			   50000,      50000,      50000,      50000,      50000}; 
			   //For the index=10-14 need to run twice for early and late stage
    int   index = -1;
    for(int i = 0; i < 15; i++){
      if(CKIN == ckins[i]){
	index = i;
	break;
      }
    }//i

    if(index<0) {
      cout << "ERROR: no CKIN index found." << endl;
      return; 
    }
  
    int ini, fin;
    if(isLate)	{ini = nFiles[index];	fin = 2*nFiles[index];}
    else	{ini = 0;		fin = nFiles[index];}
    for(int iter=ini; iter<fin; iter++)
    {
      tMatched	->Add(Form("%s/%i/makejets_%s.root", fileLoc.Data(), iter, "hadrons"));
    }

  }//end if runno==13

  LinkMatchedVars(tMatched);
  LinkMatchedVarsCont(tMatched);

  TString sCKIN = "";
  sCKIN += CKIN; 

  TFile *fOut;
  if(isLate){
    fOut = new TFile(Form("/sphenix/user/yzhai/JetSubStructure/GenSimTree/outTree/late/newTree_Run%i_ckin%s_%s.root", runno, sCKIN.Data(), "hadrons"),"recreate");
  }else{
    fOut = new TFile(Form("/sphenix/user/yzhai/JetSubStructure/GenSimTree/outTree/early/newTree_Run%i_ckin%s_%s.root", runno, sCKIN.Data(), "hadrons"),"recreate");
  }
  TTree *tNewMatched = new TTree("TMatched", "Matched Tree Highest pT");
  SetNewMatchedTree(tNewMatched);

//````````````````````````matched{
  int mEntries = tMatched->GetEntries();
  cout << "Examining " << mEntries << " events (matched jets) ...." << endl; 

  for(int mEnt = 0; mEnt < mEntries; mEnt++){
    tMatched->GetEntry(mEnt);

    if(nMatchedJets<=0) continue; 
    if(nMatchedJets>kMaxMatchedJets){
      cout << "ERROR: nMatchedJets OVERFLOW!" << endl; 
      return; 
    }
    
    if(abs(vertex)>30.0) continue;

    // Get the maxiumum pT reco jet
    
    int idx_max = -1; 
    float pT_max = -9999.0; 

    for(unsigned int mJet = 0; mJet < nMatchedJets; mJet++){
      if(r_pT[mJet] <= 0) continue;
      if(r_pT[mJet]>pT_max) {
	pT_max = r_pT[mJet]; 
	idx_max = mJet;
      }
    }
    if(idx_max<0) continue; 

    //changed from t_eta to r_eta to be consistent with the reco jet tree making
    //YZ July 8, 2019
    int iarm = getArm(r_eta[idx_max]); 
    if( (iarm<0) || (iarm>1)) continue;

    //check for NaN before filling
    if( isNAN(r_pT[idx_max]) || isNAN(r_eta[idx_max]) || isNAN(r_phi[idx_max]) || isNAN(r_nc[idx_max])  || 
	isNAN(r_cf[idx_max]) || isNAN(t_pT[idx_max])  || isNAN(t_eta[idx_max]) || isNAN(t_phi[idx_max]) || 
	isNAN(t_nc[idx_max]) ) continue;
    
    r_pT_out	= r_pT[idx_max];
    r_eta_out	= r_eta[idx_max];
    r_phi_out	= r_phi[idx_max];
    r_nc_out	= r_nc[idx_max];
    r_chgd_nc_out	= r_chgd_nc[idx_max];
    r_neut_nc_out	= r_neut_nc[idx_max];
    r_cf_out	= r_cf[idx_max];

    r_chgd_pT_out = r_cf_out*r_pT_out;
    r_neut_pT_out = (1.0 - r_cf_out)*r_pT_out;
 
    t_pT_out	= t_pT[idx_max];
    t_eta_out	= t_eta[idx_max];
    t_phi_out	= t_phi[idx_max];
    t_nc_out	= t_nc[idx_max];
    t_chgd_nc_out	= t_chgd_nc[idx_max];
    t_neut_nc_out	= t_neut_nc[idx_max];
    t_cf_out	= t_cf[idx_max];

    t_chgd_pT_out = t_cf_out*t_pT_out;
    t_neut_pT_out = (1.0 - t_cf_out)*t_pT_out;

    r_z_g_out	= r_z_g[idx_max]; 
    r_pT1_out	= r_pT1[idx_max]; 
    r_phi1_out	= r_phi1[idx_max]; 
    r_eta1_out	= r_eta1[idx_max]; 
    r_nc1_out	= r_nc1[idx_max]; 
    r_chgd_nc1_out	= r_chgd_nc1[idx_max]; 
    r_neut_nc1_out	= r_neut_nc1[idx_max]; 
    r_cf1_out	= r_cf1[idx_max]; 
    r_pT2_out	= r_pT2[idx_max]; 
    r_phi2_out	= r_phi2[idx_max]; 
    r_eta2_out	= r_eta2[idx_max]; 
    r_nc2_out	= r_nc2[idx_max]; 
    r_chgd_nc2_out	= r_chgd_nc2[idx_max]; 
    r_neut_nc2_out	= r_neut_nc2[idx_max]; 
    r_cf2_out	= r_cf2[idx_max]; 
    r_oang_out  = r_oang[idx_max];

    t_z_g_out	= t_z_g[idx_max]; 
    t_pT1_out	= t_pT1[idx_max]; 
    t_phi1_out	= t_phi1[idx_max]; 
    t_eta1_out	= t_eta1[idx_max]; 
    t_nc1_out	= t_nc1[idx_max]; 
    t_chgd_nc1_out	= t_chgd_nc1[idx_max]; 
    t_neut_nc1_out	= t_neut_nc1[idx_max]; 
    t_cf1_out	= t_cf1[idx_max]; 
    t_pT2_out	= t_pT2[idx_max]; 
    t_phi2_out	= t_phi2[idx_max]; 
    t_eta2_out	= t_eta2[idx_max]; 
    t_nc2_out	= t_nc2[idx_max]; 
    t_chgd_nc2_out	= t_chgd_nc2[idx_max]; 
    t_neut_nc2_out	= t_neut_nc2[idx_max]; 
    t_cf2_out	= t_cf2[idx_max]; 
    t_oang_out  = t_oang[idx_max];

    r_iter_out	= r_iter[idx_max]; 
    t_iter_out	= t_iter[idx_max]; 

    dphi_out	= dphi[idx_max]; 
    deta_out	= deta[idx_max]; 
    dR_out	= dR[idx_max]; 
   
    ckin_out	= (float) CKIN; 

    tNewMatched->Fill();
    
  }//mEntries
//````````````````````````matched}

  fOut->cd();
  fOut->Write();
  fOut->Close();

  return;

}//genNewTree
