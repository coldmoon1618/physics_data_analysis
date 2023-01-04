#ifndef __MAKEJETS_H__
#define __MAKEJETS_H__

//fastjet tools
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include <TString.h>

// TMVA
#include "TMVA/Types.h"
#include "TMVA/Reader.h"
#include "TMVA/IMethod.h"

namespace fastjet
{
  class JetDefinition;  
}

typedef struct
{
  float z_g;
  float pT1;
  float eta1;
  float phi1;
  int   nc1;
  int   chgd_nc1;
  int   neut_nc1;
  float cf1; 
  float pT2;
  float eta2;
  float phi2;
  int   nc2;
  int   chgd_nc2;
  int   neut_nc2;
  float cf2; 
  float oang; 
  int   iter;
}JetSubstructure;

class TFile;
class TTree;
class TBranch;
class TH1F;
class TH2F;
class TTree;

class MakeJets
{
 
 public:
  MakeJets();
  ~MakeJets() {}

  //core structure of the code
  int InitNow(std::string fnOutput,
              std::string fnTrkClus, float re_jet_R, float tr_jet_R, float jet_minPt, int run, 
              std::string fnPythia,  char truthType );
  int Run();
  void SetBranchAddress();
  void InitTrees();
  void InitHistograms();
  void InitSpinDB();
  int GetSpinPattern(int bspin, int yspin);
  int GetFakeJets(fastjet::JetDefinition *re_antikt);
  void ResetRecoJetVars();
  void ResetFakeJetVars();
  void ResetTruthJetVars();
  void ResetRestrictedTruthJetVars();
  void ResetMatchedJetVars();
  void ResetRestrictedMatchedJetVars();
  void ResetTruthRestrictedMatchedJetVars();
  bool TrackCuts(int h); 
  bool ClusterCuts(int n); 
  void InitMachineLearning(); 
  float GetDeta(float jeta); 
  float GetMachineLearningTransverseMomentum(float r_pT, float r_cf, float r_phi, float det_eta, 
					     int r_nc, float r_tndphi, float r_tndeta); 
  //functions to 'calculate' things
  float phiReduce(float phi);
  float dphiReduce(float dphi);
  float dR(float eta1, float eta2, float phi1, float phi2);
  int   getArm(float phi){
    int arm = 1;
    if(phi>1.57) arm = 0;
    return arm;    
  }//end getArm
 
  void setSubstructure(fastjet::JetDefinition C_A,  std::vector<fastjet::PseudoJet> SubCons, JetSubstructure *tTemp, float *particleCharge); 
 
 private:

  //read and write files
  TFile   *fPythiaIn;//for pythia input
  TFile   *fTrkClusIn;//for trkClus input
  TFile   *fOut;//for output
  TString pythiaFileName;
  TString trkclusFileName;
  TString outFileName;
  float re_R;
  float tr_R;
  float minPt_jets;
  int ml_run; 

  //for fTrkClusIn
  TTree  *tTracks;
  TTree  *tClusters;
  TTree  *tEvent;
  //for fPythiaIn
  TTree  *tPythiaHadron;
  TTree  *tPythiaParton;
  TTree  *tPythiaEvent;
  
  //Trees and arrays used to store fields
  TTree      *rJets;

  static const int kMaxRecoJets = 100;
  unsigned int  nRecoJets;
  unsigned int  runNumber;   
  unsigned int  eventNumber;
  int           fillNumber;
  int           level1_clock_cross;
  int           ip12_clock_cross;
  float         vertex;
  int           spinPattern;

  float re_pT[kMaxRecoJets];
  float re_eta[kMaxRecoJets];
  float re_phi[kMaxRecoJets];
  int   re_nc[kMaxRecoJets];
  int   re_chgd_nc[kMaxRecoJets];
  int   re_neut_nc[kMaxRecoJets];
  float re_cf[kMaxRecoJets];
  float re_ml_pT[kMaxRecoJets]; 
  float re_tphi[kMaxRecoJets];
  float re_nphi[kMaxRecoJets];
  float re_teta[kMaxRecoJets];
  float re_neta[kMaxRecoJets];
  bool re_ertFired[kMaxRecoJets];

  //substructure info
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

  TTree *fJets;
  static const int kMaxFakeJets = 100;
  unsigned int nFakeJets;
  float f_pT[kMaxFakeJets];
  float f_eta[kMaxFakeJets];
  float f_phi[kMaxFakeJets];
  int   f_nc[kMaxFakeJets];
  int   f_chgd_nc[kMaxFakeJets];
  int   f_neut_nc[kMaxFakeJets];
  float f_cf[kMaxFakeJets];
  float f_ml_pT[kMaxFakeJets]; 
  float f_tphi[kMaxFakeJets];
  float f_nphi[kMaxFakeJets];
  float f_teta[kMaxFakeJets];
  float f_neta[kMaxFakeJets];
  bool  f_ertFired[kMaxFakeJets];

  TTree      *tJets;
  static const int kMaxTruthJets = 200;
  unsigned int nTruthJets;
  float evt_x1;
  float evt_x2;
  float evt_Qsqr;
  int   process_id;
  float tr_pT[kMaxTruthJets];
  float tr_eta[kMaxTruthJets];
  float tr_phi[kMaxTruthJets];
  int   tr_nc[kMaxTruthJets];
  int   tr_chgd_nc[kMaxTruthJets];
  int   tr_neut_nc[kMaxTruthJets];
  float tr_cf[kMaxTruthJets];

  float tr_z_g[kMaxTruthJets];
  float tr_pT1[kMaxTruthJets];
  float tr_eta1[kMaxTruthJets];
  float tr_phi1[kMaxTruthJets];
  int   tr_nc1[kMaxTruthJets];
  int   tr_chgd_nc1[kMaxTruthJets];
  int   tr_neut_nc1[kMaxTruthJets];
  float tr_cf1[kMaxTruthJets];
  float tr_pT2[kMaxTruthJets];
  float tr_eta2[kMaxTruthJets];
  float tr_phi2[kMaxTruthJets];
  int   tr_nc2[kMaxTruthJets];
  int   tr_chgd_nc2[kMaxTruthJets];
  int   tr_neut_nc2[kMaxTruthJets];
  float tr_cf2[kMaxTruthJets];
  float tr_oang[kMaxTruthJets];
  int   tr_iter[kMaxTruthJets];

  TTree      *rtJets;
  static const int kMaxRestrictedTruthJets = 200;
  unsigned int nRestrictedTruthJets;
  float rtr_pT[kMaxRestrictedTruthJets];
  float rtr_eta[kMaxRestrictedTruthJets];
  float rtr_phi[kMaxRestrictedTruthJets];
  int   rtr_nc[kMaxRestrictedTruthJets];
  int   rtr_chgd_nc[kMaxRestrictedTruthJets];
  int   rtr_neut_nc[kMaxRestrictedTruthJets];
  float rtr_cf[kMaxRestrictedTruthJets];
  float rtr_tphi[kMaxRestrictedTruthJets];
  float rtr_nphi[kMaxRestrictedTruthJets];
  float rtr_teta[kMaxRestrictedTruthJets];
  float rtr_neta[kMaxRestrictedTruthJets];

  float rtr_z_g[kMaxRestrictedTruthJets];
  float rtr_pT1[kMaxRestrictedTruthJets];
  float rtr_eta1[kMaxRestrictedTruthJets];
  float rtr_phi1[kMaxRestrictedTruthJets];
  int   rtr_nc1[kMaxRestrictedTruthJets];
  int   rtr_chgd_nc1[kMaxRestrictedTruthJets];
  int   rtr_neut_nc1[kMaxRestrictedTruthJets];
  float rtr_cf1[kMaxRestrictedTruthJets];
  float rtr_pT2[kMaxRestrictedTruthJets];
  float rtr_eta2[kMaxRestrictedTruthJets];
  float rtr_phi2[kMaxRestrictedTruthJets];
  int   rtr_nc2[kMaxRestrictedTruthJets];
  int   rtr_chgd_nc2[kMaxRestrictedTruthJets];
  int   rtr_neut_nc2[kMaxRestrictedTruthJets];
  float rtr_cf2[kMaxRestrictedTruthJets];
  float rtr_oang[kMaxRestrictedTruthJets];
  int   rtr_iter[kMaxRestrictedTruthJets];

  TTree      *mJets; 
  static const int kMaxMatchedJets = 100;
  unsigned int nMatchedJets;
  //the event-info can be re-used
  //jet info  
  float m_r_pT[kMaxMatchedJets];
  float m_r_eta[kMaxMatchedJets];
  float m_r_phi[kMaxMatchedJets];
  int   m_r_nc[kMaxMatchedJets];
  int   m_r_chgd_nc[kMaxMatchedJets];
  int   m_r_neut_nc[kMaxMatchedJets];
  float m_r_cf[kMaxMatchedJets];
  float m_r_ml_pT[kMaxMatchedJets]; 
  float m_r_tphi[kMaxMatchedJets];
  float m_r_nphi[kMaxMatchedJets];
  float m_r_teta[kMaxMatchedJets];
  float m_r_neta[kMaxMatchedJets];
  bool  m_r_ertFired[kMaxMatchedJets];

  float m_t_pT[kMaxMatchedJets];
  float m_t_eta[kMaxMatchedJets];
  float m_t_phi[kMaxMatchedJets];
  int   m_t_nc[kMaxMatchedJets];
  int   m_t_chgd_nc[kMaxMatchedJets];
  int   m_t_neut_nc[kMaxMatchedJets];
  float m_t_cf[kMaxMatchedJets];

  //substructure info
  float m_r_z_g[kMaxMatchedJets];
  float m_r_pT1[kMaxMatchedJets];
  float m_r_eta1[kMaxMatchedJets];
  float m_r_phi1[kMaxMatchedJets];
  int   m_r_nc1[kMaxMatchedJets];
  int   m_r_chgd_nc1[kMaxMatchedJets];
  int   m_r_neut_nc1[kMaxMatchedJets];
  float m_r_cf1[kMaxMatchedJets];
  float m_r_pT2[kMaxMatchedJets];
  float m_r_eta2[kMaxMatchedJets];
  float m_r_phi2[kMaxMatchedJets];
  int   m_r_nc2[kMaxMatchedJets];
  int   m_r_chgd_nc2[kMaxMatchedJets];
  int   m_r_neut_nc2[kMaxMatchedJets];
  float m_r_cf2[kMaxMatchedJets];
  float m_r_oang[kMaxMatchedJets];

  float m_t_z_g[kMaxMatchedJets];
  float m_t_pT1[kMaxMatchedJets];
  float m_t_eta1[kMaxMatchedJets];
  float m_t_phi1[kMaxMatchedJets];
  int   m_t_nc1[kMaxMatchedJets];
  int   m_t_chgd_nc1[kMaxMatchedJets];
  int   m_t_neut_nc1[kMaxMatchedJets];
  float m_t_cf1[kMaxMatchedJets];
  float m_t_pT2[kMaxMatchedJets];
  float m_t_eta2[kMaxMatchedJets];
  float m_t_phi2[kMaxMatchedJets];
  int   m_t_nc2[kMaxMatchedJets];
  int   m_t_chgd_nc2[kMaxMatchedJets];
  int   m_t_neut_nc2[kMaxMatchedJets];
  float m_t_cf2[kMaxMatchedJets];
  float m_t_oang[kMaxMatchedJets];

  int   m_r_iter[kMaxMatchedJets];
  int   m_t_iter[kMaxMatchedJets];

  float m_dphi[kMaxMatchedJets];
  float m_deta[kMaxMatchedJets];
  float m_dR[kMaxMatchedJets];

  TTree      *rmJets; 
  static const int kMaxRestrictedMatchedJets = 100;
  unsigned int nRestrictedMatchedJets;
  //the event-info can be re-used
  //jet info  
  float rm_r_pT[kMaxRestrictedMatchedJets];
  float rm_r_eta[kMaxRestrictedMatchedJets];
  float rm_r_phi[kMaxRestrictedMatchedJets];
  int   rm_r_nc[kMaxRestrictedMatchedJets];
  int   rm_r_chgd_nc[kMaxRestrictedMatchedJets];
  int   rm_r_neut_nc[kMaxRestrictedMatchedJets];
  float rm_r_cf[kMaxRestrictedMatchedJets];
  float rm_r_ml_pT[kMaxRestrictedMatchedJets]; 
  float rm_r_tphi[kMaxRestrictedMatchedJets];
  float rm_r_nphi[kMaxRestrictedMatchedJets];
  float rm_r_teta[kMaxRestrictedMatchedJets];
  float rm_r_neta[kMaxRestrictedMatchedJets];
  bool  rm_r_ertFired[kMaxMatchedJets];

  float rm_t_pT[kMaxRestrictedMatchedJets];
  float rm_t_eta[kMaxRestrictedMatchedJets];
  float rm_t_phi[kMaxRestrictedMatchedJets];
  int   rm_t_nc[kMaxRestrictedMatchedJets];
  int   rm_t_chgd_nc[kMaxRestrictedMatchedJets];
  int   rm_t_neut_nc[kMaxRestrictedMatchedJets];
  float rm_t_cf[kMaxRestrictedMatchedJets];
  float rm_t_tphi[kMaxRestrictedMatchedJets];
  float rm_t_nphi[kMaxRestrictedMatchedJets];
  float rm_t_teta[kMaxRestrictedMatchedJets];
  float rm_t_neta[kMaxRestrictedMatchedJets];

  //substructure info
  float rm_r_z_g[kMaxRestrictedMatchedJets];
  float rm_r_pT1[kMaxRestrictedMatchedJets];
  float rm_r_eta1[kMaxRestrictedMatchedJets];
  float rm_r_phi1[kMaxRestrictedMatchedJets];
  int   rm_r_nc1[kMaxRestrictedMatchedJets];
  int   rm_r_chgd_nc1[kMaxRestrictedMatchedJets];
  int   rm_r_neut_nc1[kMaxRestrictedMatchedJets];
  float rm_r_cf1[kMaxRestrictedMatchedJets];
  float rm_r_pT2[kMaxRestrictedMatchedJets];
  float rm_r_eta2[kMaxRestrictedMatchedJets];
  float rm_r_phi2[kMaxRestrictedMatchedJets];
  int   rm_r_nc2[kMaxRestrictedMatchedJets];
  int   rm_r_chgd_nc2[kMaxRestrictedMatchedJets];
  int   rm_r_neut_nc2[kMaxRestrictedMatchedJets];
  float rm_r_cf2[kMaxRestrictedMatchedJets];
  float rm_r_oang[kMaxRestrictedMatchedJets];

  float rm_t_z_g[kMaxRestrictedMatchedJets];
  float rm_t_pT1[kMaxRestrictedMatchedJets];
  float rm_t_eta1[kMaxRestrictedMatchedJets];
  float rm_t_phi1[kMaxRestrictedMatchedJets];
  int   rm_t_nc1[kMaxRestrictedMatchedJets];
  int   rm_t_chgd_nc1[kMaxRestrictedMatchedJets];
  int   rm_t_neut_nc1[kMaxRestrictedMatchedJets];
  float rm_t_cf1[kMaxRestrictedMatchedJets];
  float rm_t_pT2[kMaxRestrictedMatchedJets];
  float rm_t_eta2[kMaxRestrictedMatchedJets];
  float rm_t_phi2[kMaxRestrictedMatchedJets];
  int   rm_t_nc2[kMaxRestrictedMatchedJets];
  int   rm_t_chgd_nc2[kMaxRestrictedMatchedJets];
  int   rm_t_neut_nc2[kMaxRestrictedMatchedJets];
  float rm_t_cf2[kMaxRestrictedMatchedJets];
  float rm_t_oang[kMaxRestrictedMatchedJets];

  int   rm_r_iter[kMaxRestrictedMatchedJets];
  int   rm_t_iter[kMaxRestrictedMatchedJets];

  float rm_dphi[kMaxRestrictedMatchedJets];
  float rm_deta[kMaxRestrictedMatchedJets];
  float rm_dR[kMaxRestrictedMatchedJets];

  TTree      *trmJets; 
  static const int kMaxTruthRestrictedMatchedJets = 100;
  unsigned int nTruthRestrictedMatchedJets;
  //the event-info can be re-used
  //jet info  
  float trm_r_pT[kMaxTruthRestrictedMatchedJets];
  float trm_r_eta[kMaxTruthRestrictedMatchedJets];
  float trm_r_phi[kMaxTruthRestrictedMatchedJets];
  int   trm_r_nc[kMaxTruthRestrictedMatchedJets];
  int   trm_r_chgd_nc[kMaxTruthRestrictedMatchedJets];
  int   trm_r_neut_nc[kMaxTruthRestrictedMatchedJets];
  float trm_r_cf[kMaxTruthRestrictedMatchedJets];
  float trm_r_ml_pT[kMaxTruthRestrictedMatchedJets];
  float trm_r_tphi[kMaxTruthRestrictedMatchedJets];
  float trm_r_nphi[kMaxTruthRestrictedMatchedJets];
  float trm_r_teta[kMaxTruthRestrictedMatchedJets];
  float trm_r_neta[kMaxTruthRestrictedMatchedJets];

  float trm_t_pT[kMaxTruthRestrictedMatchedJets];
  float trm_t_eta[kMaxTruthRestrictedMatchedJets];
  float trm_t_phi[kMaxTruthRestrictedMatchedJets];
  int   trm_t_nc[kMaxTruthRestrictedMatchedJets];
  int   trm_t_chgd_nc[kMaxTruthRestrictedMatchedJets];
  int   trm_t_neut_nc[kMaxTruthRestrictedMatchedJets];
  float trm_t_cf[kMaxTruthRestrictedMatchedJets];

  //substructure info
  float trm_r_z_g[kMaxTruthRestrictedMatchedJets];
  float trm_r_pT1[kMaxTruthRestrictedMatchedJets];
  float trm_r_eta1[kMaxTruthRestrictedMatchedJets];
  float trm_r_phi1[kMaxTruthRestrictedMatchedJets];
  int   trm_r_nc1[kMaxTruthRestrictedMatchedJets];
  int   trm_r_chgd_nc1[kMaxTruthRestrictedMatchedJets];
  int   trm_r_neut_nc1[kMaxTruthRestrictedMatchedJets];
  float trm_r_cf1[kMaxTruthRestrictedMatchedJets];
  float trm_r_pT2[kMaxTruthRestrictedMatchedJets];
  float trm_r_eta2[kMaxTruthRestrictedMatchedJets];
  float trm_r_phi2[kMaxTruthRestrictedMatchedJets];
  int   trm_r_nc2[kMaxTruthRestrictedMatchedJets];
  int   trm_r_chgd_nc2[kMaxTruthRestrictedMatchedJets];
  int   trm_r_neut_nc2[kMaxTruthRestrictedMatchedJets];
  float trm_r_cf2[kMaxTruthRestrictedMatchedJets];
  float trm_r_oang[kMaxTruthRestrictedMatchedJets];

  float trm_t_z_g[kMaxTruthRestrictedMatchedJets];
  float trm_t_pT1[kMaxTruthRestrictedMatchedJets];
  float trm_t_eta1[kMaxTruthRestrictedMatchedJets];
  float trm_t_phi1[kMaxTruthRestrictedMatchedJets];
  int   trm_t_nc1[kMaxTruthRestrictedMatchedJets];
  int   trm_t_chgd_nc1[kMaxTruthRestrictedMatchedJets];
  int   trm_t_neut_nc1[kMaxTruthRestrictedMatchedJets];
  float trm_t_cf1[kMaxTruthRestrictedMatchedJets];
  float trm_t_pT2[kMaxTruthRestrictedMatchedJets];
  float trm_t_eta2[kMaxTruthRestrictedMatchedJets];
  float trm_t_phi2[kMaxTruthRestrictedMatchedJets];
  int   trm_t_nc2[kMaxTruthRestrictedMatchedJets];
  int   trm_t_chgd_nc2[kMaxTruthRestrictedMatchedJets];
  int   trm_t_neut_nc2[kMaxTruthRestrictedMatchedJets];
  float trm_t_cf2[kMaxTruthRestrictedMatchedJets];
  float trm_t_oang[kMaxTruthRestrictedMatchedJets];

  int   trm_r_iter[kMaxTruthRestrictedMatchedJets];
  int   trm_t_iter[kMaxTruthRestrictedMatchedJets];

  float trm_dphi[kMaxTruthRestrictedMatchedJets];
  float trm_deta[kMaxTruthRestrictedMatchedJets];
  float trm_dR[kMaxTruthRestrictedMatchedJets];

  //booleans
  //pass in 'p' to set writeParton=true, the other=false
  //pass in 'h' to set writeHadron=true, the other=false
  //pass in 'r' to set both=false
  bool writeParton;
  bool writeHadron;

  //Histos to find phi range of reco jets
  TH1F *h_track_phi;
  TH1F *h_cluster_phi;
  TH1F *h_charged_phi;
  TH1F *h_neutral_phi;
 
  TH1F *h_track_eta;
  TH1F *h_cluster_eta;
  TH1F *h_charged_eta;
  TH1F *h_neutral_eta;
 
  //Array sizes, big number to avoid index of out bounds seg fault
  static const int kMaxtTracks = 1000;
  static const int kMaxtClusters = 1000;
  static const int kMaxtPythiaHadron = 1000;
  static const int kMaxtPythiaParton = 1000;
  // Declaration of leaf types
  int           tPythiaHadron_;   //[tPythiaHadron_]
  float         tPythiaHadron_charge[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_energy[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_mom[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_pT[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_px[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_py[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_pz[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_eta[kMaxtPythiaHadron];   //[tPythiaHadron_]
  float         tPythiaHadron_phi[kMaxtPythiaHadron];   //[tPythiaHadron_]
  int           tPythiaHadron_kf[kMaxtPythiaHadron];   //[tPythiaHadron_]
  bool          tPythiaHadron_isNeutrino[kMaxtPythiaHadron];   //[tPythiaHadron_]
  bool          tPythiaHadron_isMuon[kMaxtPythiaHadron];   //[tPythiaHadron_]
 
  // Declaration of leaf types
  int           tPythiaParton_;   //[tPythiaParton_]
  int           tPythiaParton_charge[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_energy[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_mom[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_pT[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_px[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_py[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_pz[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_eta[kMaxtPythiaParton];   //[tPythiaParton_]
  float         tPythiaParton_phi[kMaxtPythiaParton];   //[tPythiaParton_]
  int           tPythiaParton_kf[kMaxtPythiaParton];   //[tPythiaParton_]
 
  // Declaration of leaf types
  int           tTracks_;
  float         tTracks_runnumber[kMaxtTracks];   //[tTracks_]
  float         tTracks_eventnumber[kMaxtTracks];   //[tTracks_]
  float         tTracks_mom[kMaxtTracks];   //[tTracks_]
  float         tTracks_theta[kMaxtTracks];   //[tTracks_]
  float         tTracks_pT[kMaxtTracks];   //[tTracks_]
  float         tTracks_eta[kMaxtTracks];   //[tTracks_]
  float         tTracks_phi[kMaxtTracks];   //[tTracks_]
  float         tTracks_phiDC[kMaxtTracks];   //[tTracks_]
  float         tTracks_zedDC[kMaxtTracks];   //[tTracks_]
  float         tTracks_alpha[kMaxtTracks];   //[tTracks_]
  float         tTracks_energy[kMaxtTracks];   //[tTracks_]
  float         tTracks_board[kMaxtTracks];   //[tTracks_]


  int           tTracks_charge[kMaxtTracks];   //[tTracks_]
  int           tTracks_quality[kMaxtTracks];   //[tTracks_]
  int           tTracks_n0[kMaxtTracks];   //[tTracks_]
  int           tTracks_arm[kMaxtTracks];   //[tTracks_]
  int           tTracks_armsect[kMaxtTracks];   //[tTracks_]
  int           tTracks_emcid[kMaxtTracks];   //[tTracks_]

  float         tTracks_px[kMaxtTracks];   //[tTracks_]
  float         tTracks_py[kMaxtTracks];   //[tTracks_]
  float         tTracks_pz[kMaxtTracks];   //[tTracks_]
  float         tTracks_eT[kMaxtTracks];   //[tTracks_]

  float         tTracks_pc3dphi[kMaxtTracks];   //[tTracks_]
  float         tTracks_pc3dz[kMaxtTracks];   //[tTracks_]
  float         tTracks_pc3sdphi[kMaxtTracks];   //[tTracks_]
  float         tTracks_pc3sdz[kMaxtTracks];   //[tTracks_]
  float         tTracks_emcdphi[kMaxtTracks];   //[tTracks_]
  float         tTracks_emcdz[kMaxtTracks];   //[tTracks_]
  float         tTracks_emcsdphi[kMaxtTracks];   //[tTracks_]
  float         tTracks_emcsdz[kMaxtTracks];   //[tTracks_]

  //for associated clusters w/energy > 0.1
  float         tTracks_assClusToF[kMaxtTracks];   //[tTracks_]
  float         tTracks_assClusEnergy[kMaxtTracks];    //[tTracks_]
  bool         tTracks_assClusERTfired[kMaxtTracks];    //[tTracks_]

  bool          tTracks_inBrokenX1[kMaxtTracks];   //[tTracks_]
  bool          tTracks_inBrokenX2[kMaxtTracks];   //[tTracks_]
  bool          tTracks_inBrokenUV[kMaxtTracks];   //[tTracks_]
  bool          tTracks_x1Used[kMaxtTracks];   //[tTracks_]
  bool          tTracks_x2Used[kMaxtTracks];   //[tTracks_]
  bool          tTracks_uvUsed[kMaxtTracks];   //[tTracks_]

  bool          tTracks_passDC[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passQuality[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passMatching[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passEMCMatching[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passPC3Matching[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passMinPt[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passGhost[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passDcConversion[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passConversionEdge[kMaxtTracks];   //[tTracks_]
  bool          tTracks_passConversionElectron[kMaxtTracks];   //[tTracks_]

  // Declaration of leaf types
  int           tClusters_;
  int           tClusters_arm[kMaxtClusters];   //[tClusters_]
  int           tClusters_sector[kMaxtClusters];   //[tClusters_]
  int           tClusters_armsect[kMaxtClusters];   //[tClusters_]
  int           tClusters_emcid[kMaxtClusters];   //[tClusters_]
  int           tClusters_yTowerPos[kMaxtClusters];   //[tClusters_]
  int           tClusters_zTowerPos[kMaxtClusters];   //[tClusters_]
  int           tClusters_towerId[kMaxtClusters];   //[tClusters_]

  bool          tClusters_ertFired[kMaxtClusters];   //[tClusters_]

  float         tClusters_runnumber[kMaxtClusters];   //[tClusters_]
  float         tClusters_eventnumber[kMaxtClusters];   //[tClusters_]
  float         tClusters_energy[kMaxtClusters];   //[tClusters_]
  float         tClusters_theta[kMaxtClusters];   //[tClusters_]
  float         tClusters_pT[kMaxtClusters];   //[tClusters_]
  float         tClusters_eT[kMaxtClusters];   //[tClusters_]
  float         tClusters_phi[kMaxtClusters];   //[tClusters_]
  float         tClusters_px[kMaxtClusters];   //[tClusters_]
  float         tClusters_py[kMaxtClusters];   //[tClusters_]
  float         tClusters_pz[kMaxtClusters];   //[tClusters_]
  float         tClusters_eta[kMaxtClusters];   //[tClusters_]
  float         tClusters_chi2[kMaxtClusters];   //[tClusters_]
  float         tClusters_prob[kMaxtClusters];   //[tClusters_]
  float         tClusters_tof[kMaxtClusters];   //[tClusters_]
  float         tClusters_emctrkdphi[kMaxtClusters];   //[tClusters_]
  float         tClusters_emctrkdz[kMaxtClusters];   //[tClusters_]

  bool          tClusters_passNotBad[kMaxtClusters];   //[tClusters_]
  bool          tClusters_passEnergy[kMaxtClusters];   //[tClusters_]
  bool          tClusters_passIsNeutral[kMaxtClusters];   //[tClusters_]
  bool          tClusters_passToF[kMaxtClusters];   //[tClusters_]

  //These are for reading in the TTrees, vertex not made yet
  // Declaration of leaf types
  // These are directly linked to the tree
  unsigned int  tEvent_runNumber;   //[tEvent_]
  unsigned int  tEvent_eventNumber;   //[tEvent_]
  int           tEvent_fillNumber;   //[tEvent_]
  int           tEvent_level1_clock_cross;   //[tEvent_]
  int           tEvent_ip12_clock_cross;   //[tEvent_]
  float         tEvent_vertex;   //[tEvent_]
  // Declaration of leaf types
  // These are directly linked to the tree
  float         tPythiaEvent_x1;   //[tPythiaEvent_]
  float         tPythiaEvent_x2;   //[tPythiaEvent_]
  float         tPythiaEvent_Qsqr;   //[tPythiaEvent_]
  int           tPythiaEvent_processid;   //[tPythiaEvent_]

  // List of branches
  TBranch        *b_tPythiaHadron_;
  TBranch        *b_tPythiaHadron_charge;
  TBranch        *b_tPythiaHadron_energy;
  TBranch        *b_tPythiaHadron_mom;
  TBranch        *b_tPythiaHadron_px;
  TBranch        *b_tPythiaHadron_py;
  TBranch        *b_tPythiaHadron_pz;
  TBranch        *b_tPythiaHadron_pT;
  TBranch        *b_tPythiaHadron_eta;
  TBranch        *b_tPythiaHadron_phi;
  TBranch        *b_tPythiaHadron_kf;
  TBranch        *b_tPythiaHadron_isNeutrino;
  TBranch        *b_tPythiaHadron_isMuon;

  // List of branches
  TBranch        *b_tPythiaParton_;
  TBranch        *b_tPythiaParton_charge;
  TBranch        *b_tPythiaParton_energy;
  TBranch        *b_tPythiaParton_mom;
  TBranch        *b_tPythiaParton_px;
  TBranch        *b_tPythiaParton_py;
  TBranch        *b_tPythiaParton_pz;
  TBranch        *b_tPythiaParton_pT;
  TBranch        *b_tPythiaParton_eta;
  TBranch        *b_tPythiaParton_phi;
  TBranch        *b_tPythiaParton_kf;
  
  // List of branches
  TBranch        *b_tTracks_;   //!
  TBranch        *b_tTracks_runnumber;   //!
  TBranch        *b_tTracks_eventnumber;   //!
  TBranch        *b_tTracks_mom;   //!
  TBranch        *b_tTracks_theta;   //!
  TBranch        *b_tTracks_pT;   //!
  TBranch        *b_tTracks_eta;   //!
  TBranch        *b_tTracks_phi;   //!
  TBranch        *b_tTracks_phiDC;   //!
  TBranch        *b_tTracks_zedDC;   //!
  TBranch        *b_tTracks_alpha;   //!
  TBranch        *b_tTracks_energy;   //!
  TBranch        *b_tTracks_board;   //!

  TBranch        *b_tTracks_charge;   //!
  TBranch        *b_tTracks_quality;   //!
  TBranch        *b_tTracks_n0;   //!
  TBranch        *b_tTracks_arm;   //!
  TBranch        *b_tTracks_armsect;   //!
  TBranch        *b_tTracks_emcid;   //!

  TBranch        *b_tTracks_px;   //!
  TBranch        *b_tTracks_py;   //!
  TBranch        *b_tTracks_pz;   //!
  TBranch        *b_tTracks_eT;   //!

  TBranch        *b_tTracks_pc3dphi;   //!
  TBranch        *b_tTracks_pc3dz;   //!
  TBranch        *b_tTracks_pc3sdphi;   //!
  TBranch        *b_tTracks_pc3sdz;   //!
  TBranch        *b_tTracks_emcdphi;   //!
  TBranch        *b_tTracks_emcdz;   //!
  TBranch        *b_tTracks_emcsdphi;   //!
  TBranch        *b_tTracks_emcsdz;   //!

  TBranch        *b_tTracks_assClusToF;   //!
  TBranch        *b_tTracks_assClusEnergy;   //!
  TBranch        *b_tTracks_assClusERTfired;   //!

  TBranch        *b_tTracks_inBrokenX1;   //!
  TBranch        *b_tTracks_inBrokenX2;   //!
  TBranch        *b_tTracks_inBrokenUV;   //!
  TBranch        *b_tTracks_x1Used;   //!
  TBranch        *b_tTracks_x2Used;   //!
  TBranch        *b_tTracks_uvUsed;   //!
  TBranch        *b_tTracks_passDC;   //!
  TBranch        *b_tTracks_passQuality;   //!
  TBranch        *b_tTracks_passMatching;   //!
  TBranch        *b_tTracks_passEMCMatching;   //!
  TBranch        *b_tTracks_passPC3Matching;   //!
  TBranch        *b_tTracks_passMinPt;   //!
  TBranch        *b_tTracks_passGhost;   //!
  TBranch        *b_tTracks_passDcConversion;   //!
  TBranch        *b_tTracks_passConversionEdge;   //!
  TBranch        *b_tTracks_passConversionElectron;   //!
  // List of branches
  TBranch        *b_tClusters_;   //!
  TBranch        *b_tClusters_arm;   //!
  TBranch        *b_tClusters_sector;   //!
  TBranch        *b_tClusters_armsect;   //!
  TBranch        *b_tClusters_emcid;   //!
  TBranch        *b_tClusters_yTowerPos;   //!
  TBranch        *b_tClusters_zTowerPos;   //!
  TBranch        *b_tClusters_towerId;   //!

  TBranch        *b_tClusters_ertFired;   //!

  TBranch        *b_tClusters_runnumber;   //!
  TBranch        *b_tClusters_eventnumber;   //!
  TBranch        *b_tClusters_energy;   //!
  TBranch        *b_tClusters_theta;   //!
  TBranch        *b_tClusters_pT;   //!
  TBranch        *b_tClusters_eT;   //!
  TBranch        *b_tClusters_phi;   //!
  TBranch        *b_tClusters_px;   //!
  TBranch        *b_tClusters_py;   //!
  TBranch        *b_tClusters_pz;   //!
  TBranch        *b_tClusters_eta;   //!
  TBranch        *b_tClusters_chi2;   //!
  TBranch        *b_tClusters_prob;   //!
  TBranch        *b_tClusters_tof;   //!
  TBranch        *b_tClusters_emctrkdphi;   //!
  TBranch        *b_tClusters_emctrkdz;   //!

  TBranch        *b_tClusters_passNotBad;   //!
  TBranch        *b_tClusters_passEnergy;   //!
  TBranch        *b_tClusters_passIsNeutral;   //!
  TBranch        *b_tClusters_passToF;   //!

  // TMVA - machine learning variables
  TMVA::Reader *reader;
  TMVA::MethodBase *method;

  float ml_r_pT; 
  float ml_r_cf; 
  float ml_r_phi; 
  float ml_det_eta; 
  float ml_r_nc; 
  float ml_r_tndphi; 
  float ml_r_tndeta; 
  
};

#endif/* __MAKEJETS_H__*/
