#include "MakeJets.h"

//ROOT stuff
#include <TFile.h>
#include <TTree.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <TVector3.h>
#include <TLorentzVector.h>

//c++
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <exception>

//fastjet tools
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>

//spin stuff
#include <SpinDBOutput.hh>
#include <SpinDBContent.hh>

// PHENIX Filter class
#include "PHENIXFilter.h" 

// TMVA
#include "TMVA/Types.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TMVA/MethodCuts.h"

#include "ml_training_data_mlp_Run13.h"
#include "ml_training_data_mlp_Run12.h"

using namespace std;

#define NUMCONST_CUT 3.0
#define CFLOW_CUT 0.3
#define CFHIGH_CUT 0.7
#define MATCHED_JET_DELTA_R 0.3
#define ZG_CUT 0.1
#define RECOVERED_CLUSTER_MIN_E 0.5

#define RESTRICTED_CLUSTER_MIN_E 0.5
#define RESTRICTED_TRACK_MIN_PT 0.5

SpinDBContent spin_cont;
 
// PUT TRACK CUTS HERE
// Used in both Run() and GetFakeJets():

bool MakeJets::TrackCuts(int h){

  //TRACK CUTS
  bool passCuts = true; 

  if(!tTracks_passMinPt[h]) passCuts = false; 
  if(!tTracks_passDC[h]) passCuts = false; 
  if(!tTracks_passMatching[h]) passCuts = false; 
  if(!tTracks_passGhost[h]) passCuts = false; 
  if(!tTracks_passDcConversion[h]) passCuts = false; 
  if(!tTracks_passConversionEdge[h]) passCuts = false; 
  if(!tTracks_passConversionElectron[h]) passCuts = false; 

  return passCuts; 
  
}

// PUT CLUSTER CUTS HERE
// Used in both Run() and GetFakeJets():

bool MakeJets::ClusterCuts(int n){
  
  //CLUSTER CUTS 
  bool passCuts = true; 

  if(!tClusters_passEnergy[n]) passCuts = false; 
  if(!tClusters_passNotBad[n]) passCuts = false; 
  if(!tClusters_passIsNeutral[n]) passCuts = false; 
  if(!tClusters_passToF[n]) passCuts = false; 
  
  return passCuts; 

}

// Function for jet eta acceptance based on vertex

// JGL 03/23/2019
bool JetEtaAcceptance(double jet_eta, double jet_R, double zvtx){

  static bool first = true; 
  static double zint = -9999.0; 
  double ref_radius = 200.0; // DC reference radius in cm
  if(first){
    // Calculate z_int parameter based on eta limit for z=0 (0.35) 
    // and DC reference radius (200.0 cm)
    // (with ref_radius = 200cm --> zint = 71.4cm)
    zint = ref_radius/tan(2.0*atan(exp(-0.35)));
    first = false; 
  }

  // CAREFUL! calculate angular limits over full 
  // range of possible vertex values

  double theta_min = -9999.0; 
  double theta_max = -9999.0;
  if(fabs(zvtx)<zint){
    theta_min = atan(ref_radius/(zint + zvtx));
    theta_max = atan(ref_radius/(zint - zvtx));
  }
  else if(fabs(zvtx)>zint){
    if(zvtx>0.0){
      theta_min = atan(ref_radius/(zint + zvtx));
      theta_max = TMath::Pi() - atan(ref_radius/(zvtx - zint));
    }
    else{
      theta_max = atan(ref_radius/(zint - zvtx));
      theta_min = TMath::Pi() - atan(ref_radius/(fabs(zvtx) - zint));
     }
  }
  else {
    if(zvtx>0.0){
      theta_min = atan(ref_radius/(zint + zvtx));
      theta_max = TMath::PiOver2();
    }
    else{
      theta_min = TMath::PiOver2();
      theta_max = atan(ref_radius/(zint - zvtx));
    }
  }

  double eta_min = log(tan(theta_min/2.0)); 
  double eta_max = -log(tan(theta_max/2.0));

  if( (jet_eta>(eta_min+jet_R)) && (jet_eta<(eta_max-jet_R)) )
    return true; 
  else
    return false; 

}

float MakeJets::GetDeta(float jeta){

  // Detector eta is used in ML process as it most closely 
  // relates to detector efficiencies

  float theta_jet = 2.0*atan(exp(-fabs(jeta))); 
  float tan_theta_det;
  if(jeta>=0.0) 
    tan_theta_det = tan(theta_jet) + vertex/200.0; // reference radius at 200cm
  else
    tan_theta_det = tan(theta_jet) - vertex/200.0; // reference radius at 200cm

  float theta_det = atan(tan_theta_det); 
  if(jeta<0.0) theta_det = TMath::Pi() - theta_det; 

  return -log(tan(theta_det/2.0)); 

}

//Constructor, initializes things
MakeJets::MakeJets(){
  fPythiaIn = NULL;
  fTrkClusIn = NULL;
  fOut = NULL;

  tTracks = NULL;
  tClusters = NULL;
  tEvent = NULL;
  writeHadron = false; 
  writeParton = false; 
  tPythiaHadron = NULL;
  tPythiaEvent = NULL;
  tPythiaParton = NULL;
  tPythiaEvent = NULL;
   
  tTracks_ = 0; tClusters_ = 0; 
  tPythiaHadron_ = 0;
  tPythiaParton_ = 0;
 
  for(int i=0; i< kMaxtPythiaHadron; i++){

    tPythiaHadron_charge[i] = 0;   
    tPythiaHadron_energy[i] = 0;   
    tPythiaHadron_mom[i] = 0;   
    tPythiaHadron_px[i] = 0;   
    tPythiaHadron_py[i] = 0;   
    tPythiaHadron_pz[i] = 0;   
    tPythiaHadron_pT[i] =0;
    tPythiaHadron_eta[i] = 0;   
    tPythiaHadron_phi[i] = 0;   
    tPythiaHadron_kf[i] = 0;   
    tPythiaHadron_isNeutrino[i] = false;   
    tPythiaHadron_isMuon[i] = false;   

  }

  for(int i=0; i< kMaxtPythiaHadron; i++){

    tPythiaHadron_charge[i] = 0;   
    tPythiaHadron_energy[i] = 0;   
    tPythiaHadron_mom[i] = 0;   
    tPythiaHadron_px[i] = 0;   
    tPythiaHadron_py[i] = 0;   
    tPythiaHadron_pz[i] = 0;   
    tPythiaHadron_pT[i] =0;
    tPythiaHadron_eta[i] = 0;   
    tPythiaHadron_phi[i] = 0;   
    tPythiaHadron_kf[i] = 0;   
    tPythiaHadron_isNeutrino[i] = false;   
    tPythiaHadron_isMuon[i] = false;   

  }//end i: Partons
  
  for(int i = 0; i < kMaxtTracks; i++){
    tTracks_runnumber[i] = 0;   
    tTracks_eventnumber[i] = 0;   
    tTracks_mom[i] = 0;   
    tTracks_theta[i] = 0;   
    tTracks_pT[i] = 0;   
    tTracks_eta[i] = 0;   
    tTracks_phi[i] = 0;   
    tTracks_phiDC[i] = 0;   
    tTracks_zedDC[i] = 0;   
    tTracks_alpha[i] = 0;   
    tTracks_energy[i] = 0;   
    tTracks_board[i] = 0;   

    tTracks_charge[i] = 0;   
    tTracks_quality[i] = 0;   
    tTracks_n0[i] = 0;   
    tTracks_arm[i] = 0;   
    tTracks_armsect[i] = 0;   
    tTracks_emcid[i] = 0;   

    tTracks_px[i] = 0;   
    tTracks_py[i] = 0;   
    tTracks_pz[i] = 0;   
    tTracks_eT[i] = 0;   

    tTracks_pc3dphi[i] = 0;   
    tTracks_pc3dz[i] = 0;   
    tTracks_pc3sdphi[i] = 0;   
    tTracks_pc3sdz[i] = 0;   
    tTracks_emcdphi[i] = 0;   
    tTracks_emcdz[i] = 0;   
    tTracks_emcsdphi[i] = 0;   
    tTracks_emcsdz[i] = 0;   

    tTracks_assClusToF[i] = 0;   
    tTracks_assClusEnergy[i] = 0;   
    tTracks_assClusERTfired[i] = false;   

    tTracks_inBrokenX1[i] = 0;   
    tTracks_inBrokenX2[i] = 0;   
    tTracks_inBrokenUV[i] = 0;   
    tTracks_x1Used[i] = 0;   
    tTracks_x2Used[i] = 0;   
    tTracks_uvUsed[i] = 0;   

    tTracks_passDC[i] = 0;   
    tTracks_passQuality[i] = 0;   
    tTracks_passMatching[i] = 0;   
    tTracks_passEMCMatching[i] = 0;   
    tTracks_passPC3Matching[i] = 0;   
    tTracks_passMinPt[i] = 0;   
    tTracks_passGhost[i] = false;   
    tTracks_passDcConversion[i] = 0;   
    tTracks_passConversionEdge[i] = 0;   
    tTracks_passConversionElectron[i] = 0;   
    
  }//end i (tracks)

  for(int i = 0; i < kMaxtClusters; i++){
    tClusters_arm[i] = 0;   
    tClusters_sector[i] = 0;   
    tClusters_armsect[i] = 0;   
    tClusters_emcid[i] = 0;   
    tClusters_yTowerPos[i] = 0;   
    tClusters_zTowerPos[i] = 0;   
    tClusters_towerId[i] = 0;   

    tClusters_ertFired[i] = 0;   

    tClusters_runnumber[i] = 0;   
    tClusters_eventnumber[i] = 0;   
    tClusters_energy[i] = 0;   
    tClusters_theta[i] = 0;   
    tClusters_pT[i] = 0;   
    tClusters_eT[i] = 0;   
    tClusters_phi[i] = 0;   
    tClusters_px[i] = 0;   
    tClusters_py[i] = 0;   
    tClusters_pz[i] = 0;   
    tClusters_eta[i] = 0;   
    tClusters_chi2[i] = 0;   
    tClusters_prob[i] = 0;   
    tClusters_tof[i] = 0;   
    tClusters_emctrkdphi[i] = 0;   
    tClusters_emctrkdz[i] = 0;   

    tClusters_passNotBad[i] = 0;   
    tClusters_passEnergy[i] = 0;   
    tClusters_passIsNeutral[i] = 0;   
    tClusters_passToF[i] = 0;   

  }//end i (clusters)

  {
    tEvent_runNumber = 0;   
    tEvent_eventNumber = 0;   
    tEvent_fillNumber = 0;   
    tEvent_level1_clock_cross = 0;   
    tEvent_ip12_clock_cross = 0;   
    tEvent_vertex = 0;   
  }//end tEvent

  tPythiaEvent_x1 = 0;   
  tPythiaEvent_x2 = 0;   
  tPythiaEvent_Qsqr = 0;   
  tPythiaEvent_processid = 0;   

  //null the branches for Hadrons
  b_tPythiaHadron_ = NULL;  
  b_tPythiaHadron_charge = NULL;  
  b_tPythiaHadron_energy = NULL;  
  b_tPythiaHadron_mom = NULL;  
  b_tPythiaHadron_pT = NULL;  
  b_tPythiaHadron_px = NULL;  
  b_tPythiaHadron_py = NULL;  
  b_tPythiaHadron_pz = NULL;  
  b_tPythiaHadron_eta = NULL;   
  b_tPythiaHadron_phi = NULL;   
  b_tPythiaHadron_kf = NULL;   
  b_tPythiaHadron_isNeutrino = NULL;   
  b_tPythiaHadron_isMuon = NULL;   

  //null the branches for Partons
  b_tPythiaParton_ = NULL;  
  b_tPythiaParton_charge = NULL;  
  b_tPythiaParton_energy = NULL;  
  b_tPythiaParton_mom = NULL;  
  b_tPythiaParton_pT = NULL;  
  b_tPythiaParton_px = NULL;  
  b_tPythiaParton_py = NULL;  
  b_tPythiaParton_pz = NULL;  
  b_tPythiaParton_eta = NULL;   
  b_tPythiaParton_phi = NULL;   
  b_tPythiaParton_kf = NULL;   

  //null the branches for tracks
  b_tTracks_ = NULL;   
  b_tTracks_runnumber = NULL;   
  b_tTracks_eventnumber = NULL;   
  b_tTracks_mom = NULL;   
  b_tTracks_theta = NULL;   
  b_tTracks_pT = NULL;   
  b_tTracks_eta = NULL;   
  b_tTracks_phi = NULL;   
  b_tTracks_phiDC = NULL;   
  b_tTracks_zedDC = NULL;   
  b_tTracks_alpha = NULL;   
  b_tTracks_energy = NULL;   
  b_tTracks_board = NULL;   

  b_tTracks_charge = NULL;   
  b_tTracks_quality = NULL;   
  b_tTracks_n0 = NULL;   
  b_tTracks_arm = NULL;   
  b_tTracks_armsect = NULL;   
  b_tTracks_emcid = NULL;   

  b_tTracks_px = NULL;   
  b_tTracks_py = NULL;   
  b_tTracks_pz = NULL;   
  b_tTracks_eT = NULL;   

  b_tTracks_pc3dphi = NULL;   
  b_tTracks_pc3dz = NULL;   
  b_tTracks_pc3sdphi = NULL;   
  b_tTracks_pc3sdz = NULL;   
  b_tTracks_emcdphi = NULL;   
  b_tTracks_emcdz = NULL;   
  b_tTracks_emcsdphi = NULL;   
  b_tTracks_emcsdz = NULL;   

  b_tTracks_assClusToF = NULL;   
  b_tTracks_assClusEnergy = NULL;   
  b_tTracks_assClusERTfired = NULL;   

  b_tTracks_inBrokenX1 = NULL;   
  b_tTracks_inBrokenX2 = NULL;   
  b_tTracks_inBrokenUV = NULL;   
  b_tTracks_x1Used = NULL;   
  b_tTracks_x2Used = NULL;   
  b_tTracks_uvUsed = NULL;   

  b_tTracks_passDC = NULL;   
  b_tTracks_passQuality = NULL;   
  b_tTracks_passMatching = NULL;   
  b_tTracks_passEMCMatching = NULL;   
  b_tTracks_passPC3Matching = NULL;   
  b_tTracks_passMinPt = NULL;   
  b_tTracks_passGhost = NULL;   
  b_tTracks_passDcConversion = NULL;   
  b_tTracks_passConversionEdge = NULL;   
  b_tTracks_passConversionElectron = NULL;   

  //null the branchs for clusters
  b_tClusters_ = NULL;   
  b_tClusters_arm = NULL;   
  b_tClusters_sector = NULL;   
  b_tClusters_armsect = NULL;   
  b_tClusters_emcid = NULL;   
  b_tClusters_yTowerPos = NULL;   
  b_tClusters_zTowerPos = NULL;   
  b_tClusters_towerId = NULL;   

  b_tClusters_ertFired = NULL;   

  b_tClusters_runnumber = NULL;   
  b_tClusters_eventnumber = NULL;   
  b_tClusters_energy = NULL;   
  b_tClusters_theta = NULL;   
  b_tClusters_pT = NULL;   
  b_tClusters_eT = NULL;   
  b_tClusters_phi = NULL;   
  b_tClusters_px = NULL;   
  b_tClusters_py = NULL;   
  b_tClusters_pz = NULL;   
  b_tClusters_eta = NULL;   
  b_tClusters_chi2 = NULL;   
  b_tClusters_prob = NULL;   
  b_tClusters_tof = NULL;   
  b_tClusters_emctrkdphi = NULL;   
  b_tClusters_emctrkdz = NULL;   

  b_tClusters_passNotBad = NULL;   
  b_tClusters_passEnergy = NULL;   
  b_tClusters_passIsNeutral = NULL;   
  b_tClusters_passToF = NULL;   

}//end MakeJets()

//This is called in main.C
//The user passes in the types of TTres to write out
//The user also passes in input and output file names
int MakeJets::InitNow(std::string fnOutput,
		      std::string fnTrkClus, float re_jet_R, float tr_jet_R, float jet_minPt, int run, 
		      std::string fnPythia,  char truthType ){
  if(re_jet_R > 0){
    re_R = re_jet_R;
  }else{
    cout<<"Invalid input re_jet_R = "<<re_jet_R<<", MakeJets aborted."<<endl;
    return(-1);
  }
  if(tr_jet_R > 0){
    tr_R = tr_jet_R;
  }else{
    cout<<"Invalid input tr_jet_R = "<<tr_jet_R<<", MakeJets aborted."<<endl;
    return(-1);
  }
  if(jet_minPt > 0){
    minPt_jets  = jet_minPt;
  }else{
    cout<<"Invalid input jet_minPt = "<<jet_minPt<<", MakeJets aborted."<<endl;
    return(-1);
  }

  ml_run = run; 

  // Only set up simulation input if filename is valid
  if(!fnPythia.empty()){
    pythiaFileName  = fnPythia;
    if(truthType == 'p'){
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"MakeJets will make reco and parton jets"<<endl;
      writeParton = true;
      writeHadron = false;
    }
    else if(truthType == 'h'){
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"MakeJets will make reco and hadron jets"<<endl;
      writeParton = false;
      writeHadron = true;
    }else{
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"Invalid truth particle type, MakeJets aborted. "<<endl;
      cout<<"Choose h for hadron, p for parton."<<endl;
      return(-1);
    }
    cout<<"Reading in "<<pythiaFileName<<" and "<<trkclusFileName<<endl;
    fPythiaIn = new TFile(pythiaFileName, "read");
    if(!fPythiaIn || fPythiaIn->IsZombie() ){
      cout << "Invalid input file for Pythia particles, MakeJets aborted." << endl;
      return(-1);
    }
  }
  else{
    fPythiaIn = NULL; 
    pythiaFileName = ""; 
    writeParton = false; 
    writeHadron = false; 
  }

  trkclusFileName = fnTrkClus;
  fTrkClusIn = new TFile(trkclusFileName,"read");
  if(!fTrkClusIn || fTrkClusIn->IsZombie() ){
    cout << "Invalid input file for tracks and clusters, MakeJets aborted." << endl;
    return(-1);
  }

  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

  tEvent = (TTree*)fTrkClusIn->Get("tEvent");
  tTracks = (TTree*)fTrkClusIn->Get("tTracks");
  tClusters = (TTree*)fTrkClusIn->Get("tClusters");
  if(!tEvent){
    cout<<"tEvent doesn't exist, MakeJets aborted."<<endl;
    return(-1);
  }
  if(!tTracks){
    cout<<"tTracks doesn't exist, MakeJets aborted."<<endl;
    return(-1);
  }
  if(!tClusters){
    cout<<"tClusters doesn't exist, MakeJets aborted."<<endl;
    return(-1);
  }
  if(writeHadron){
    tPythiaHadron = (TTree*)fPythiaIn->Get("tPythiaHadron");
    if(!tPythiaHadron){
      cout<<"tPythiaHadron doesn't exist, MakeJets aborted."<<endl;
      return(-1);
    }
    tPythiaEvent = (TTree*)fPythiaIn->Get("tPythiaEvent");
    if(!tPythiaEvent){
      cout<<"tPythiaEvent doesn't exist, MakeJets aborted."<<endl;
      return(-1);
    }
  }
  if(writeParton){
    tPythiaParton = (TTree*)fPythiaIn->Get("tPythiaParton");
    if(!tPythiaParton){
      cout<<"tPythiaParton doesn't exist, MakeJets aborted."<<endl;
      return(-1);
    }
    tPythiaEvent = (TTree*)fPythiaIn->Get("tPythiaEvent");
    if(!tPythiaEvent){
      cout<<"tPythiaEvent doesn't exist, MakeJets aborted."<<endl;
      return(-1);
    }
  }
  
  outFileName = fnOutput;
  fOut = new TFile(outFileName, "recreate");

  SetBranchAddress();

  InitTrees();

  InitHistograms();

  InitSpinDB();

  InitMachineLearning(); 

  return 0;

}//end InitNow()

void MakeJets::InitSpinDB(){

  //each input Data file is from a different Run
  tEvent->GetEntry(0);
  int runNumber = tEvent_runNumber;

  cout << "Spin DB run number:  " << runNumber << endl;

  SpinDBOutput spin_out("phnxrc");
  
  int storedbcont = spin_out.StoreDBContent(runNumber, runNumber);
  if(storedbcont != 1) cout << "didn't get Database content" << endl;

  int getdbcontstore = spin_out.GetDBContentStore(spin_cont, runNumber);
  if(getdbcontstore != 1) cout << "didn't get Database content" << endl;


  return;

}//end InitSpinDB



int MakeJets::GetSpinPattern(int bspin, int yspin){

  int ret_spinPat = -9999; 
  if(bspin == 1 && yspin == 1) {

    ret_spinPat = 0;
  } 
  else if(bspin == -1 && yspin == 1) { 
 
   ret_spinPat = 1;

  } 
  else if( bspin == 1 && yspin == -1) { 
   
    ret_spinPat = 2;
  }
  else if( bspin == -1 && yspin == -1) { 
  
    ret_spinPat = 3;
  }
  else{                     
    
    ret_spinPat = 4;
  }

  return ret_spinPat;
}


//Init Histograms
void MakeJets::InitHistograms(){
  //cout << "init histograms" << endl;
  h_track_phi   = new TH1F("h_track_phi",  "phi distr for all tracks used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());  
  h_cluster_phi = new TH1F("h_cluster_phi","phi distr for all clusters used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());  
  h_neutral_phi = new TH1F("h_neutral_phi","phi distr for all neutral particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());  
  h_charged_phi = new TH1F("h_charged_phi","phi distr for all charged particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());  

  h_track_eta   = new TH1F("h_track_eta",  "eta distr for all tracks used in reco jets",20,-5,5);
  h_cluster_eta = new TH1F("h_cluster_eta","eta distr for all clusters used in reco jets",20,-5,5);  
  h_neutral_eta = new TH1F("h_neutral_eta","eta distr for all neutral particles used in truth jets",20,-5,5);  
  h_charged_eta = new TH1F("h_charged_eta","eta distr for all charged particles used in truth jets",20,-5,5);  

  return;
}//end InitHistograms

//Init TTrees
void MakeJets::InitTrees(){

  rJets  = new TTree("rJets", "Reco jets");
  fJets  = new TTree("fJets", "Fake Jets");
  tJets  = new TTree("tJets", "Truth jets");
  rtJets = new TTree("rtJets", "Restricted Truth jets");
  mJets  = new TTree("mJets", "Matched jets");
  rmJets  = new TTree("rmJets", "Restricted Matched jets");
  trmJets  = new TTree("trmJets", "Truth Restricted Matched jets");
 
  //rJets filled with individual fields 
  rJets -> Branch("nRecoJets", &nRecoJets, "nRecoJets/I");
  rJets -> Branch("recoRadius",&re_R,"re_R/F");
  rJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  rJets -> Branch("runNumber",&runNumber,"runNumber/I");
  rJets -> Branch("eventNumber",&eventNumber,"eventNumber/I");
  rJets -> Branch("fillNumber",&fillNumber,"fillNumber/I");
  rJets -> Branch("level1_clock_cross",&level1_clock_cross,"level1_clock_cross/I");
  rJets -> Branch("ip12_clock_cross",&ip12_clock_cross,"ip12_clock_cross/I");
  rJets -> Branch("vertex",&vertex,"vertex/F");
  rJets -> Branch("spinPattern", &spinPattern, "spinPattern/I");
  rJets -> Branch("pT",re_pT, "re_pT[nRecoJets]/F");
  rJets -> Branch("eta",re_eta, "re_eta[nRecoJets]/F");
  rJets -> Branch("phi",re_phi, "re_phi[nRecoJets]/F");
  rJets -> Branch("nc",re_nc, "re_nc[nRecoJets]/I"); 
  rJets -> Branch("chgd_nc",re_chgd_nc, "re_chgd_nc[nRecoJets]/I"); 
  rJets -> Branch("neut_nc",re_neut_nc, "re_neut_nc[nRecoJets]/I"); 
  rJets -> Branch("cf",re_cf, "re_cf[nRecoJets]/F");
  rJets -> Branch("ml_pT",re_ml_pT, "re_ml_pT[nRecoJets]/F");
  rJets -> Branch("tphi",re_tphi, "re_tphi[nRecoJets]/F");
  rJets -> Branch("nphi",re_nphi, "re_nphi[nRecoJets]/F");
  rJets -> Branch("teta",re_teta, "re_teta[nRecoJets]/F");
  rJets -> Branch("neta",re_neta, "re_neta[nRecoJets]/F");
  rJets -> Branch("ertFired", re_ertFired, "re_ertFired[nRecoJets]/b");

  //z_g stuff
  rJets -> Branch("z_g",re_z_g, "re_z_g[nRecoJets]/F");
  rJets -> Branch("pT1",re_pT1, "re_pT1[nRecoJets]/F");
  rJets -> Branch("eta1",re_eta1, "re_eta1[nRecoJets]/F");
  rJets -> Branch("phi1",re_phi1, "re_phi1[nRecoJets]/F");
  rJets -> Branch("nc1",re_nc1, "re_nc1[nRecoJets]/I");
  rJets -> Branch("chgd_nc1",re_chgd_nc1, "re_chgd_nc1[nRecoJets]/I");
  rJets -> Branch("neut_nc1",re_neut_nc1, "re_neut_nc1[nRecoJets]/I");
  rJets -> Branch("cf1",re_cf1, "re_cf1[nRecoJets]/F");
  rJets -> Branch("pT2",re_pT2, "re_pT2[nRecoJets]/F");
  rJets -> Branch("eta2",re_eta2, "re_eta2[nRecoJets]/F");
  rJets -> Branch("phi2",re_phi2, "re_phi2[nRecoJets]/F");
  rJets -> Branch("nc2",re_nc2, "re_nc2[nRecoJets]/I");
  rJets -> Branch("chgd_nc2",re_chgd_nc2, "re_chgd_nc2[nRecoJets]/I");
  rJets -> Branch("neut_nc2",re_neut_nc2, "re_neut_nc2[nRecoJets]/I");
  rJets -> Branch("cf2",re_cf2, "re_cf2[nRecoJets]/F");
  rJets -> Branch("oang",re_oang, "re_oang[nRecoJets]/F");
  rJets -> Branch("iter",re_iter, "re_iter[nRecoJets]/I");
  
  //fakeJets filled with individual fields 
  fJets -> Branch("nFakeJets", &nFakeJets, "nFakeJets/I");
  fJets -> Branch("fakeRadius",&re_R,"re_R/F");
  fJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  fJets -> Branch("spinPattern", &spinPattern, "spinPattern/I");
  fJets -> Branch("eventNumber",&eventNumber,"eventNumber/I");
  fJets -> Branch("fillNumber",&fillNumber,"fillNumber/I");
  fJets -> Branch("level1_clock_cross",&level1_clock_cross,"level1_clock_cross/I");
  fJets -> Branch("ip12_clock_cross",&ip12_clock_cross,"ip12_clock_cross/I");
  fJets -> Branch("vertex",&vertex,"vertex/F");
  fJets -> Branch("pT",f_pT, "f_pT[nFakeJets]/F");
  fJets -> Branch("eta",f_eta, "f_eta[nFakeJets]/F");
  fJets -> Branch("phi",f_phi, "f_phi[nFakeJets]/F");
  fJets -> Branch("nc",f_nc, "f_nc[nFakeJets]/I");
  fJets -> Branch("chgd_nc",f_chgd_nc, "f_chgd_nc[nFakeJets]/I");
  fJets -> Branch("neut_nc",f_neut_nc, "f_neut_nc[nFakeJets]/I");
  fJets -> Branch("cf",f_cf, "f_cf[nFakeJets]/F");
  fJets -> Branch("ml_pT",f_ml_pT, "f_ml_pT[nFakeJets]/F");
  fJets -> Branch("tphi",f_tphi, "f_tphi[nFakeJets]/F");
  fJets -> Branch("nphi",f_nphi, "f_nphi[nFakeJets]/F");
  fJets -> Branch("teta",f_teta, "f_teta[nFakeJets]/F");
  fJets -> Branch("neta",f_neta, "f_neta[nFakeJets]/F");
  fJets -> Branch("ertFired", f_ertFired, "f_ertFired[nFakeJets]/b");

  //tJets filled with individual fields 
  tJets -> Branch("nTruthJets", &nTruthJets, "nTruthJets/I");
  tJets -> Branch("truthRadius",&tr_R,"tr_R/F");
  tJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  tJets -> Branch("evt_x1",&evt_x1,"evt_x1/F");
  tJets -> Branch("evt_x2",&evt_x2,"evt_x2/F");
  tJets -> Branch("evt_Qsqr",&evt_Qsqr,"evt_Qsqr/F");
  tJets -> Branch("process_id",&process_id,"process_id/I");
  tJets -> Branch("pT",tr_pT, "tr_pT[nTruthJets]/F");
  tJets -> Branch("eta",tr_eta, "tr_eta[nTruthJets]/F");
  tJets -> Branch("phi",tr_phi, "tr_phi[nTruthJets]/F");
  tJets -> Branch("nc",tr_nc, "tr_nc[nTruthJets]/I");
  tJets -> Branch("chgd_nc",tr_chgd_nc, "tr_chgd_nc[nTruthJets]/I");
  tJets -> Branch("neut_nc",tr_neut_nc, "tr_neut_nc[nTruthJets]/I");
  tJets -> Branch("cf",tr_cf, "tr_cf[nTruthJets]/F");
  tJets -> Branch("writeParton", &writeParton, "writeParton/b");
  tJets -> Branch("writeHadron", &writeHadron, "writeHadron/b");
  tJets -> Branch("z_g",tr_z_g, "tr_z_g[nTruthJets]/F");
  tJets -> Branch("pT1",tr_pT1, "tr_pT1[nTruthJets]/F");
  tJets -> Branch("eta1",tr_eta1, "tr_eta1[nTruthJets]/F");
  tJets -> Branch("phi1",tr_phi1, "tr_phi1[nTruthJets]/F");
  tJets -> Branch("nc1",tr_nc1, "tr_nc1[nTruthJets]/I");
  tJets -> Branch("chgd_nc1",tr_chgd_nc1, "tr_chgd_nc1[nTruthJets]/I");
  tJets -> Branch("neut_nc1",tr_neut_nc1, "tr_neut_nc1[nTruthJets]/I");
  tJets -> Branch("cf1",tr_cf1, "tr_cf1[nTruthJets]/F");
  tJets -> Branch("pT2",tr_pT2, "tr_pT2[nTruthJets]/F");
  tJets -> Branch("eta2",tr_eta2, "tr_eta2[nTruthJets]/F");
  tJets -> Branch("phi2",tr_phi2, "tr_phi2[nTruthJets]/F");
  tJets -> Branch("nc2",tr_nc2, "tr_nc2[nTruthJets]/I");
  tJets -> Branch("chgd_nc2",tr_chgd_nc2, "tr_chgd_nc2[nTruthJets]/I");
  tJets -> Branch("neut_nc2",tr_neut_nc2, "tr_neut_nc2[nTruthJets]/I");
  tJets -> Branch("cf2",tr_cf2, "tr_cf2[nTruthJets]/F");
  tJets -> Branch("oang",tr_oang, "tr_oang[nTruthJets]/F");
  tJets -> Branch("iter",tr_iter, "tr_iter[nTruthJets]/I");

  //rtJets filled with individual fields 
  rtJets -> Branch("nRestrictedTruthJets", &nRestrictedTruthJets, "nRestrictedTruthJets/I");
  rtJets -> Branch("writeParton", &writeParton, "writeParton/b");
  rtJets -> Branch("writeHadron", &writeHadron, "writeHadron/b");
  rtJets -> Branch("truthRadius",&tr_R,"tr_R/F");
  rtJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  rtJets -> Branch("evt_x1",&evt_x1,"evt_x1/F");
  rtJets -> Branch("evt_x2",&evt_x2,"evt_x2/F");
  rtJets -> Branch("evt_Qsqr",&evt_Qsqr,"evt_Qsqr/F");
  rtJets -> Branch("process_id",&process_id,"process_id/I");
  rtJets -> Branch("pT",rtr_pT, "rtr_pT[nRestrictedTruthJets]/F");
  rtJets -> Branch("eta",rtr_eta, "rtr_eta[nRestrictedTruthJets]/F");
  rtJets -> Branch("phi",rtr_phi, "rtr_phi[nRestrictedTruthJets]/F");
  rtJets -> Branch("nc",rtr_nc, "rtr_nc[nRestrictedTruthJets]/I");
  rtJets -> Branch("chgd_nc",rtr_chgd_nc, "rtr_chgd_nc[nRestrictedTruthJets]/I");
  rtJets -> Branch("neut_nc",rtr_neut_nc, "rtr_neut_nc[nRestrictedTruthJets]/I");
  rtJets -> Branch("cf",rtr_cf, "rtr_cf[nRestrictedTruthJets]/F");
  rtJets -> Branch("tphi",rtr_tphi, "rtr_tphi[nRestrictedTruthJets]/F");
  rtJets -> Branch("nphi",rtr_nphi, "rtr_nphi[nRestrictedTruthJets]/F");
  rtJets -> Branch("teta",rtr_teta, "rtr_teta[nRestrictedTruthJets]/F");
  rtJets -> Branch("neta",rtr_neta, "rtr_neta[nRestrictedTruthJets]/F");
  rtJets -> Branch("z_g",rtr_z_g, "rtr_z_g[nRestrictedTruthJets]/F");
  rtJets -> Branch("pT1",rtr_pT1, "rtr_pT1[nRestrictedTruthJets]/F");
  rtJets -> Branch("eta1",rtr_eta1, "rtr_eta1[nRestrictedTruthJets]/F");
  rtJets -> Branch("phi1",rtr_phi1, "rtr_phi1[nRestrictedTruthJets]/F");
  rtJets -> Branch("nc1",rtr_nc1, "rtr_nc1[nRestrictedTruthJets]/I");
  rtJets -> Branch("chgd_nc1",rtr_chgd_nc1, "rtr_chgd_nc1[nRestrictedTruthJets]/I");
  rtJets -> Branch("neut_nc1",rtr_neut_nc1, "rtr_neut_nc1[nRestrictedTruthJets]/I");
  rtJets -> Branch("cf1",rtr_cf1, "rtr_cf1[nRestrictedTruthJets]/F");
  rtJets -> Branch("pT2",rtr_pT2, "rtr_pT2[nRestrictedTruthJets]/F");
  rtJets -> Branch("eta2",rtr_eta2, "rtr_eta2[nRestrictedTruthJets]/F");
  rtJets -> Branch("phi2",rtr_phi2, "rtr_phi2[nRestrictedTruthJets]/F");
  rtJets -> Branch("nc2",rtr_nc2, "rtr_nc2[nRestrictedTruthJets]/I");
  rtJets -> Branch("chgd_nc2",rtr_chgd_nc2, "rtr_chgd_nc2[nRestrictedTruthJets]/I");
  rtJets -> Branch("neut_nc2",rtr_neut_nc2, "rtr_neut_nc2[nRestrictedTruthJets]/I");
  rtJets -> Branch("cf2",rtr_cf2, "rtr_cf2[nRestrictedTruthJets]/F");
  rtJets -> Branch("oang",rtr_oang, "rtr_oang[nRestrictedTruthJets]/F");
  rtJets -> Branch("iter",rtr_iter, "rtr_iter[nRestrictedTruthJets]/I");

  //mJets filled with individual fields 
  mJets -> Branch("nMatchedJets", &nMatchedJets, "nMatchedJets/I");
  mJets -> Branch("recoRadius",&re_R,"re_R/F");
  mJets -> Branch("truthRadius",&tr_R,"tr_R/F");
  mJets -> Branch("writeParton", &writeParton, "writeParton/b");
  mJets -> Branch("writeHadron", &writeHadron, "writeHadron/b");
  mJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  mJets -> Branch("evt_x1",&evt_x1,"evt_x1/F");
  mJets -> Branch("evt_x2",&evt_x2,"evt_x2/F");
  mJets -> Branch("evt_Qsqr",&evt_Qsqr,"evt_Qsqr/F");
  mJets -> Branch("process_id",&process_id,"process_id/I");
  mJets -> Branch("runNumber",&runNumber,"runNumber/I");
  mJets -> Branch("eventNumber",&eventNumber,"eventNumber/I");
  mJets -> Branch("vertex",&vertex,"vertex/F");

  mJets -> Branch("r_pT", m_r_pT, "m_r_pT[nMatchedJets]/F"); 
  mJets -> Branch("r_eta", m_r_eta, "m_r_eta[nMatchedJets]/F"); 
  mJets -> Branch("r_phi", m_r_phi, "m_r_phi[nMatchedJets]/F"); 
  mJets -> Branch("r_nc", m_r_nc, "m_r_nc[nMatchedJets]/I"); 
  mJets -> Branch("r_chgd_nc", m_r_chgd_nc, "m_r_chgd_nc[nMatchedJets]/I"); 
  mJets -> Branch("r_neut_nc", m_r_neut_nc, "m_r_neut_nc[nMatchedJets]/I"); 
  mJets -> Branch("r_cf", m_r_cf, "m_r_cf[nMatchedJets]/F"); 
  mJets -> Branch("r_ml_pT",m_r_ml_pT, "m_r_ml_pT[nMatchedJets]/F");
  mJets -> Branch("r_tphi",m_r_tphi, "m_r_tphi[nMatchedJets]/F");
  mJets -> Branch("r_nphi",m_r_nphi, "m_r_nphi[nMatchedJets]/F");
  mJets -> Branch("r_teta",m_r_teta, "m_r_teta[nMatchedJets]/F");
  mJets -> Branch("r_neta",m_r_neta, "m_r_neta[nMatchedJets]/F");
  mJets -> Branch("r_ertFired", m_r_ertFired, "m_r_ertFired[nMatchedJets]/b");
  
  mJets -> Branch("t_pT", m_t_pT, "m_t_pT[nMatchedJets]/F"); 
  mJets -> Branch("t_eta", m_t_eta, "m_t_eta[nMatchedJets]/F"); 
  mJets -> Branch("t_phi", m_t_phi, "m_t_phi[nMatchedJets]/F"); 
  mJets -> Branch("t_nc", m_t_nc, "m_t_nc[nMatchedJets]/I"); 
  mJets -> Branch("t_chgd_nc", m_t_chgd_nc, "m_t_chgd_nc[nMatchedJets]/I"); 
  mJets -> Branch("t_neut_nc", m_t_neut_nc, "m_t_neut_nc[nMatchedJets]/I"); 
  mJets -> Branch("t_cf", m_t_cf, "m_t_cf[nMatchedJets]/F"); 

  mJets -> Branch("r_z_g", m_r_z_g, "m_r_z_g[nMatchedJets]/F"); 
  mJets -> Branch("r_pT1", m_r_pT1, "m_r_pT1[nMatchedJets]/F"); 
  mJets -> Branch("r_eta1", m_r_eta1, "m_r_eta1[nMatchedJets]/F"); 
  mJets -> Branch("r_phi1", m_r_phi1, "m_r_phi1[nMatchedJets]/F"); 
  mJets -> Branch("r_nc1", m_r_nc1, "m_r_nc1[nMatchedJets]/I"); 
  mJets -> Branch("r_chgd_nc1", m_r_chgd_nc1, "m_r_chgd_nc1[nMatchedJets]/I"); 
  mJets -> Branch("r_neut_nc1", m_r_neut_nc1, "m_r_neut_nc1[nMatchedJets]/I"); 
  mJets -> Branch("r_cf1", m_r_cf1, "m_r_cf1[nMatchedJets]/F"); 
  mJets -> Branch("r_pT2", m_r_pT2, "m_r_pT2[nMatchedJets]/F"); 
  mJets -> Branch("r_eta2", m_r_eta2, "m_r_eta2[nMatchedJets]/F"); 
  mJets -> Branch("r_phi2", m_r_phi2, "m_r_phi2[nMatchedJets]/F"); 
  mJets -> Branch("r_nc2", m_r_nc2, "m_r_nc2[nMatchedJets]/I"); 
  mJets -> Branch("r_chgd_nc2", m_r_chgd_nc2, "m_r_chgd_nc2[nMatchedJets]/I"); 
  mJets -> Branch("r_neut_nc2", m_r_neut_nc2, "m_r_neut_nc2[nMatchedJets]/I"); 
  mJets -> Branch("r_cf2", m_r_cf2, "m_r_cf2[nMatchedJets]/F"); 
  mJets -> Branch("r_oang", m_r_oang, "m_r_oang[nMatchedJets]/F"); 
  
  mJets -> Branch("t_z_g", m_t_z_g, "m_t_z_g[nMatchedJets]/F"); 
  mJets -> Branch("t_pT1", m_t_pT1, "m_t_pT1[nMatchedJets]/F"); 
  mJets -> Branch("t_eta1", m_t_eta1, "m_t_eta1[nMatchedJets]/F"); 
  mJets -> Branch("t_phi1", m_t_phi1, "m_t_phi1[nMatchedJets]/F"); 
  mJets -> Branch("t_nc1", m_t_nc1, "m_t_nc1[nMatchedJets]/I"); 
  mJets -> Branch("t_chgd_nc1", m_t_chgd_nc1, "m_t_chgd_nc1[nMatchedJets]/I"); 
  mJets -> Branch("t_neut_nc1", m_t_neut_nc1, "m_t_neut_nc1[nMatchedJets]/I"); 
  mJets -> Branch("t_cf1", m_t_cf1, "m_t_cf1[nMatchedJets]/F"); 
  mJets -> Branch("t_pT2", m_t_pT2, "m_t_pT2[nMatchedJets]/F"); 
  mJets -> Branch("t_eta2", m_t_eta2, "m_t_eta2[nMatchedJets]/F"); 
  mJets -> Branch("t_phi2", m_t_phi2, "m_t_phi2[nMatchedJets]/F"); 
  mJets -> Branch("t_nc2", m_t_nc2, "m_t_nc2[nMatchedJets]/I"); 
  mJets -> Branch("t_chgd_nc2", m_t_chgd_nc2, "m_t_chgd_nc2[nMatchedJets]/I"); 
  mJets -> Branch("t_neut_nc2", m_t_neut_nc2, "m_t_neut_nc2[nMatchedJets]/I"); 
  mJets -> Branch("t_cf2", m_t_cf2, "m_t_cf2[nMatchedJets]/F"); 
  mJets -> Branch("t_oang", m_t_oang, "m_t_oang[nMatchedJets]/F"); 
 
  mJets -> Branch("r_iter", m_r_iter, "m_r_iter[nMatchedJets]/I");
  mJets -> Branch("t_iter", m_t_iter, "m_t_iter[nMatchedJets]/I"); 
  
  mJets -> Branch("dphi", m_dphi, "m_dphi[nMatchedJets]/F"); 
  mJets -> Branch("deta", m_deta, "m_deta[nMatchedJets]/F"); 
  mJets -> Branch("dR", m_dR, "m_dR[nMatchedJets]/F"); 

  //rmJets filled with individual fields 
  rmJets -> Branch("nMatchedJets", &nRestrictedMatchedJets, "nRestrictedMatchedJets/I");
  rmJets -> Branch("recoRadius",&re_R,"re_R/F");
  rmJets -> Branch("truthRadius",&tr_R,"tr_R/F");
  rmJets -> Branch("writeParton", &writeParton, "writeParton/b");
  rmJets -> Branch("writeHadron", &writeHadron, "writeHadron/b");
  rmJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  rmJets -> Branch("evt_x1",&evt_x1,"evt_x1/F");
  rmJets -> Branch("evt_x2",&evt_x2,"evt_x2/F");
  rmJets -> Branch("evt_Qsqr",&evt_Qsqr,"evt_Qsqr/F");
  rmJets -> Branch("process_id",&process_id,"process_id/I");
  rmJets -> Branch("runNumber",&runNumber,"runNumber/I");
  rmJets -> Branch("eventNumber",&eventNumber,"eventNumber/I");
  rmJets -> Branch("vertex",&vertex,"vertex/F");

  rmJets -> Branch("r_pT", rm_r_pT, "rm_r_pT[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_eta", rm_r_eta, "rm_r_eta[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_phi", rm_r_phi, "rm_r_phi[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_nc", rm_r_nc, "rm_r_nc[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_chgd_nc", rm_r_chgd_nc, "rm_r_chgd_nc[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_neut_nc", rm_r_neut_nc, "rm_r_neut_nc[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_cf", rm_r_cf, "rm_r_cf[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_ml_pT",rm_r_ml_pT, "rm_r_ml_pT[nRestrictedMatchedJets]/F");
  rmJets -> Branch("r_tphi",rm_r_tphi, "rm_r_tphi[nRestrictedMatchedJets]/F");
  rmJets -> Branch("r_nphi",rm_r_nphi, "rm_r_nphi[nRestrictedMatchedJets]/F");
  rmJets -> Branch("r_teta",rm_r_teta, "rm_r_teta[nRestrictedMatchedJets]/F");
  rmJets -> Branch("r_neta",rm_r_neta, "rm_r_neta[nRestrictedMatchedJets]/F");
  rmJets -> Branch("r_ertFired", rm_r_ertFired, "rm_r_ertFired[nRestrictedMatchedJets]/b");
  
  rmJets -> Branch("t_pT", rm_t_pT, "rm_t_pT[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_eta", rm_t_eta, "rm_t_eta[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_phi", rm_t_phi, "rm_t_phi[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_nc", rm_t_nc, "rm_t_nc[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_chgd_nc", rm_t_chgd_nc, "rm_t_chgd_nc[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_neut_nc", rm_t_neut_nc, "rm_t_neut_nc[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_cf", rm_t_cf, "rm_t_cf[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_tphi",rm_t_tphi, "rm_t_tphi[nRestrictedMatchedJets]/F");
  rmJets -> Branch("t_nphi",rm_t_nphi, "rm_t_nphi[nRestrictedMatchedJets]/F");
  rmJets -> Branch("t_teta",rm_t_teta, "rm_t_teta[nRestrictedMatchedJets]/F");
  rmJets -> Branch("t_neta",rm_t_neta, "rm_t_neta[nRestrictedMatchedJets]/F");
 
  rmJets -> Branch("r_z_g", rm_r_z_g, "rm_r_z_g[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_pT1", rm_r_pT1, "rm_r_pT1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_eta1", rm_r_eta1, "rm_r_eta1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_phi1", rm_r_phi1, "rm_r_phi1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_nc1", rm_r_nc1, "rm_r_nc1[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_chgd_nc1", rm_r_chgd_nc1, "rm_r_chgd_nc1[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_neut_nc1", rm_r_neut_nc1, "rm_r_neut_nc1[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_cf1", rm_r_cf1, "rm_r_cf1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_pT2", rm_r_pT2, "rm_r_pT2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_eta2", rm_r_eta2, "rm_r_eta2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_phi2", rm_r_phi2, "rm_r_phi2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_nc2", rm_r_nc2, "rm_r_nc2[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_chgd_nc2", rm_r_chgd_nc2, "rm_r_chgd_nc2[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_neut_nc2", rm_r_neut_nc2, "rm_r_neut_nc2[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("r_cf2", rm_r_cf2, "rm_r_cf2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("r_oang", rm_r_oang, "rm_r_oang[nRestrictedMatchedJets]/F"); 
  
  rmJets -> Branch("t_z_g", rm_t_z_g, "rm_t_z_g[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_pT1", rm_t_pT1, "rm_t_pT1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_eta1", rm_t_eta1, "rm_t_eta1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_phi1", rm_t_phi1, "rm_t_phi1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_nc1", rm_t_nc1, "rm_t_nc1[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_chgd_nc1", rm_t_chgd_nc1, "rm_t_chgd_nc1[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_neut_nc1", rm_t_neut_nc1, "rm_t_neut_nc1[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_cf1", rm_t_cf1, "rm_t_cf1[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_pT2", rm_t_pT2, "rm_t_pT2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_eta2", rm_t_eta2, "rm_t_eta2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_phi2", rm_t_phi2, "rm_t_phi2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_nc2", rm_t_nc2, "rm_t_nc2[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_chgd_nc2", rm_t_chgd_nc2, "rm_t_chgd_nc2[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_neut_nc2", rm_t_neut_nc2, "rm_t_neut_nc2[nRestrictedMatchedJets]/I"); 
  rmJets -> Branch("t_cf2", rm_t_cf2, "rm_t_cf2[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("t_oang", rm_t_oang, "rm_t_oang[nRestrictedMatchedJets]/F"); 
 
  rmJets -> Branch("r_iter", rm_r_iter, "rm_r_iter[nRestrictedMatchedJets]/I");
  rmJets -> Branch("t_iter", rm_t_iter, "rm_t_iter[nRestrictedMatchedJets]/I"); 
  
  rmJets -> Branch("dphi", rm_dphi, "rm_dphi[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("deta", rm_deta, "rm_deta[nRestrictedMatchedJets]/F"); 
  rmJets -> Branch("dR", rm_dR, "rm_dR[nRestrictedMatchedJets]/F"); 

  //trmJets filled with individual fields 
  trmJets -> Branch("nMatchedJets", &nTruthRestrictedMatchedJets, "nTruthRestrictedMatchedJets/I");
  trmJets -> Branch("recoRadius",&re_R,"re_R/F");
  trmJets -> Branch("truthRadius",&tr_R,"tr_R/F");
  trmJets -> Branch("writeParton", &writeParton, "writeParton/b");
  trmJets -> Branch("writeHadron", &writeHadron, "writeHadron/b");
  trmJets -> Branch("minPt_jets", &minPt_jets, "minPt_jets/F");
  trmJets -> Branch("evt_x1",&evt_x1,"evt_x1/F");
  trmJets -> Branch("evt_x2",&evt_x2,"evt_x2/F");
  trmJets -> Branch("evt_Qsqr",&evt_Qsqr,"evt_Qsqr/F");
  trmJets -> Branch("process_id",&process_id,"process_id/I");
  trmJets -> Branch("runNumber",&runNumber,"runNumber/I");
  trmJets -> Branch("eventNumber",&eventNumber,"eventNumber/I");
  trmJets -> Branch("vertex",&vertex,"vertex/F");

  trmJets -> Branch("r_pT", trm_r_pT, "trm_r_pT[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_eta", trm_r_eta, "trm_r_eta[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_phi", trm_r_phi, "trm_r_phi[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_nc", trm_r_nc, "trm_r_nc[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_chgd_nc", trm_r_chgd_nc, "trm_r_chgd_nc[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_neut_nc", trm_r_neut_nc, "trm_r_neut_nc[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_cf", trm_r_cf, "trm_r_cf[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_tphi",trm_r_tphi, "trm_r_tphi[nTruthRestrictedMatchedJets]/F");
  trmJets -> Branch("r_nphi",trm_r_nphi, "trm_r_nphi[nTruthRestrictedMatchedJets]/F");
  trmJets -> Branch("r_teta",trm_r_teta, "trm_r_teta[nTruthRestrictedMatchedJets]/F");
  trmJets -> Branch("r_neta",trm_r_neta, "trm_r_neta[nTruthRestrictedMatchedJets]/F");
  
  trmJets -> Branch("t_pT", trm_t_pT, "trm_t_pT[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_eta", trm_t_eta, "trm_t_eta[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_phi", trm_t_phi, "trm_t_phi[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_nc", trm_t_nc, "trm_t_nc[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_chgd_nc", trm_t_chgd_nc, "trm_t_chgd_nc[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_neut_nc", trm_t_neut_nc, "trm_t_neut_nc[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_cf", trm_t_cf, "trm_t_cf[nTruthRestrictedMatchedJets]/F"); 
 
  trmJets -> Branch("r_z_g", trm_r_z_g, "trm_r_z_g[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_pT1", trm_r_pT1, "trm_r_pT1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_eta1", trm_r_eta1, "trm_r_eta1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_phi1", trm_r_phi1, "trm_r_phi1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_nc1", trm_r_nc1, "trm_r_nc1[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_chgd_nc1", trm_r_chgd_nc1, "trm_r_chgd_nc1[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_neut_nc1", trm_r_neut_nc1, "trm_r_neut_nc1[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_cf1", trm_r_cf1, "trm_r_cf1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_pT2", trm_r_pT2, "trm_r_pT2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_eta2", trm_r_eta2, "trm_r_eta2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_phi2", trm_r_phi2, "trm_r_phi2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_nc2", trm_r_nc2, "trm_r_nc2[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_chgd_nc2", trm_r_chgd_nc2, "trm_r_chgd_nc2[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_neut_nc2", trm_r_neut_nc2, "trm_r_neut_nc2[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("r_cf2", trm_r_cf2, "trm_r_cf2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("r_oang", trm_r_oang, "trm_r_oang[nTruthRestrictedMatchedJets]/F"); 
  
  trmJets -> Branch("t_z_g", trm_t_z_g, "trm_t_z_g[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_pT1", trm_t_pT1, "trm_t_pT1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_eta1", trm_t_eta1, "trm_t_eta1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_phi1", trm_t_phi1, "trm_t_phi1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_nc1", trm_t_nc1, "trm_t_nc1[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_chgd_nc1", trm_t_chgd_nc1, "trm_t_chgd_nc1[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_neut_nc1", trm_t_neut_nc1, "trm_t_neut_nc1[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_cf1", trm_t_cf1, "trm_t_cf1[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_pT2", trm_t_pT2, "trm_t_pT2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_eta2", trm_t_eta2, "trm_t_eta2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_phi2", trm_t_phi2, "trm_t_phi2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_nc2", trm_t_nc2, "trm_t_nc2[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_chgd_nc2", trm_t_chgd_nc2, "trm_t_chgd_nc2[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_neut_nc2", trm_t_neut_nc2, "trm_t_neut_nc2[nTruthRestrictedMatchedJets]/I"); 
  trmJets -> Branch("t_cf2", trm_t_cf2, "trm_t_cf2[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("t_oang", trm_t_oang, "trm_t_oang[nTruthRestrictedMatchedJets]/F"); 
 
  trmJets -> Branch("r_iter", trm_r_iter, "trm_r_iter[nTruthRestrictedMatchedJets]/I");
  trmJets -> Branch("t_iter", trm_t_iter, "trm_t_iter[nTruthRestrictedMatchedJets]/I"); 
  
  trmJets -> Branch("dphi", trm_dphi, "trm_dphi[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("deta", trm_deta, "trm_deta[nTruthRestrictedMatchedJets]/F"); 
  trmJets -> Branch("dR", trm_dR, "trm_dR[nTruthRestrictedMatchedJets]/F"); 


  return;

}//end InitHistograms

float MakeJets::phiReduce(float phi){
  if ( phi < -TMath::PiOver2()) 
    phi += TMath::TwoPi();
  if (phi >= 3.0 * TMath::PiOver2()) 
    phi -= TMath::TwoPi();
  return phi; 
}//end phireduce()

float MakeJets::dphiReduce(float dphi){
  if (dphi < -TMath::Pi()) 
    dphi += TMath::TwoPi();
  if (dphi >=  TMath::Pi()) 
    dphi -= TMath::TwoPi();
  return dphi; 
}//end phireduce()

int MakeJets::Run(){
  //Define anti-kt algorithms here
  fastjet::JetDefinition *re_antikt = new fastjet::JetDefinition(fastjet::antikt_algorithm, re_R, fastjet::E_scheme, fastjet::Best);
  fastjet::JetDefinition *tr_antikt = new fastjet::JetDefinition(fastjet::antikt_algorithm, tr_R, fastjet::E_scheme, fastjet::Best);
  // Cambridge-Aachen for jet substructure
  fastjet::JetDefinition *C_A = new fastjet::JetDefinition(fastjet::cambridge_algorithm, re_R, fastjet::E_scheme, fastjet::Best);

  //The counters confirmed the # of entries are the same  
  unsigned int entriesTrack     = tTracks      ->  GetEntries();
  unsigned int entriesCluster   = tClusters    ->  GetEntries();
  unsigned int entriesEvent     = tEvent       ->  GetEntries();
  unsigned int entriesParticle  = -999;

  if(writeHadron){
    entriesParticle  = tPythiaHadron   ->  GetEntries(); 
    cout << "From " << pythiaFileName << endl;
    cout << "Hadron  entries: " <<  entriesParticle << endl;
  }
  if(writeParton){
    entriesParticle  = tPythiaParton   ->  GetEntries(); 
    cout << "From " << pythiaFileName << endl;
    cout << "Parton  entries: " <<  entriesParticle << endl;
  }
  cout << "From " << trkclusFileName << endl;
  cout << "Event   entries: " <<  entriesEvent << endl;
  cout << "Track   entries: " <<  entriesTrack << endl;
  cout << "Cluster entries: " <<  entriesCluster << endl;

  if( (entriesEvent!=entriesTrack) || (entriesEvent!=entriesCluster) ){
    cout << "ERROR: entries mismatch between Event, Track, and Cluster trees!" << endl; 
    fOut->Write();
    fOut->Close();
    return(-1);
  }

  if(writeHadron||writeParton){
    if(entriesEvent!=entriesParticle){
      cout << "ERROR: entries mismatch between Event and Particle trees!" << endl; 
      fOut->Write();
      fOut->Close();
      return(-1);
    }
  }

  int my_matchos = 0;
  int my_rmatchos = 0;
  int my_trmatchos = 0;
  int my_recos = 0;
  int my_trues = 0;
  int my_rtrues = 0;
  int my_fakes = 0; 
  int my_fake_evts = 0; 
  unsigned int my_tracks    = 0;
  unsigned int my_clusters  = 0;
  unsigned int my_partis    = 0;
  unsigned int my_rpartis    = 0;
  unsigned int tot_tracks   = 0;
  unsigned int tot_clusters = 0;
  unsigned int tot_partis   = 0;

  //loop over all entries i (the same number)
  //of particles, clusters and tracks
  for(unsigned int i = 0; i < entriesEvent; i++){

    if(i%100000 == 0) cout  <<  "at:  "  <<  i  <<  endl;

    //get the entry
    tEvent    -> GetEntry(i);
    tClusters -> GetEntry(i);
    tTracks   -> GetEntry(i);

    //check for overflows
    if( (tClusters_ > kMaxtClusters) || (tClusters_ < 0) ) {
      cout << "ERROR - possible memory overflow, tClusters_ = " << tClusters_ << endl;
      fOut->Write();
      fOut->Close();
      return(-1); 
    }
    if( (tTracks_ > kMaxtTracks) || (tTracks_ < 0) ) {
      cout << "ERROR - possible memory overflow, tTracks_ = " << tTracks_ << endl;
      fOut->Write();
      fOut->Close();
      return(-1); 
    }

    //before any cuts
    tot_tracks   += tTracks_;
    tot_clusters += tClusters_;

    if(writeHadron && (!writeParton)){

      tPythiaHadron->GetEntry(i);
      tPythiaEvent->GetEntry(i);

      if( (tPythiaHadron_ > kMaxtPythiaHadron) || (tPythiaHadron_ < 0) ) {
	cout << "ERROR - possible memory overflow, tPythiaHadron_ = " << tPythiaHadron_ << endl;
        fOut->Write();
        fOut->Close();
	return(-1); 
      }

      tot_partis += tPythiaHadron_;

    }
    if(writeParton && (!writeHadron)){

      tPythiaParton->GetEntry(i);
      tPythiaEvent->GetEntry(i);

      if( (tPythiaParton_ > kMaxtPythiaParton) || (tPythiaParton_ < 0) ) {
	cout << "ERROR - possible memory overflow, tPythiaParton_ = " << tPythiaParton_ << endl;
        fOut->Write();
        fOut->Close();
	return(-1); 
      }

      tot_partis += tPythiaParton_;

    }

    // DO NOT MAKE EVENT CUTS - 
    // Every event must have an entry in the trees, even in that entry is zero.
    // This allows you to keep events coordinated across multiple runs of MakeJets, 
    // which is necessary for the "perfect detector" analysis
    
    //For Pythia event info
    if(writeHadron||writeParton){

      evt_x1     = tPythiaEvent_x1;
      evt_x2     = tPythiaEvent_x2;
      evt_Qsqr   = tPythiaEvent_Qsqr;
      process_id = tPythiaEvent_processid;

    }

    //For DST event info
    runNumber           = tEvent_runNumber;   
    eventNumber         = tEvent_eventNumber;
    fillNumber          = tEvent_fillNumber;
    level1_clock_cross  = tEvent_level1_clock_cross;
    ip12_clock_cross    = tEvent_ip12_clock_cross;
    vertex              = tEvent_vertex;

    int bspin = spin_cont.GetSpinPatternBlue(ip12_clock_cross);
    int yspin = spin_cont.GetSpinPatternYellow(ip12_clock_cross);

    //for recoJet TTree
    spinPattern = GetSpinPattern(bspin, yspin);

    ////////////////////////////////////////       
    ///make jets from tracks and clusters///
    ////////////////////////////////////////

    std::vector<fastjet::PseudoJet> jetParticles_all;
    jetParticles_all.clear();
    const int indexTotal = 2*tTracks_ + tClusters_; // allow for overlaps
    float particlePx[indexTotal];
    float particlePy[indexTotal];
    float particlePz[indexTotal];
    float particleCharge[indexTotal]; 
    bool ertFired[indexTotal];

    int index = 0;
    for (int h = 0; h < tTracks_; h++)//loop over tracks
      { 

	if(!TrackCuts(h)) continue; 

	// track passes all cuts

	fastjet::PseudoJet pseudoCharged(tTracks_px[h],
					 tTracks_py[h],
					 tTracks_pz[h],
					 tTracks_mom[h]);

	h_track_phi -> Fill(tTracks_phi[h]);
	h_track_eta -> Fill(tTracks_eta[h]);

	pseudoCharged.set_user_index(index);
	particlePx[index] = tTracks_px[h];
	particlePy[index] = tTracks_py[h];
	particlePz[index] = tTracks_pz[h];
	ertFired[index] = tTracks_assClusERTfired[h];
	particleCharge[index] = tTracks_charge[h]; 
	jetParticles_all.push_back(pseudoCharged);
	index++;
	my_tracks++;
      }//end h (loop over tracks)

    //So this changes with the cuts
    unsigned int indexCharged=index; 
    
    for (int n = 0; n < tClusters_; n++)//loop over clusters
      {
	
	if(!ClusterCuts(n)) continue; 

	fastjet::PseudoJet pseudoNeutral(tClusters_px[n],
					 tClusters_py[n],
					 tClusters_pz[n],
					 tClusters_energy[n]);
  
	h_cluster_phi -> Fill(tClusters_phi[n]);
	h_cluster_eta -> Fill(tClusters_eta[n]);

	pseudoNeutral.set_user_index(index);
	particlePx[index] = tClusters_px[n];
	particlePy[index] = tClusters_py[n];
	particlePz[index] = tClusters_pz[n];
	ertFired[index] = tClusters_ertFired[n];
	particleCharge[index] = 0; 
	jetParticles_all.push_back(pseudoNeutral);
	index++;
	my_clusters++;
      }//end n: loop over clusters

    // Loop over all tracks that have associated EMCal clusters
    // If the cluster energy is less than the track momentum, subtract
    // the track momentum from the cluster energy and enter the remainder 
    // as a neutral cluster.  This attempts to recapture some 
    // neutral energy that would otherwise get vetoed by tracks. 

    for (int h = 0; h < tTracks_; h++)//loop over tracks
      { 

	if(!TrackCuts(h)) continue; 
	if(!tTracks_passEMCMatching[h]) continue;
  
	// track passes all cuts, find assoc. cluster
	int n_assoc = -1; 
	for (int n = 0; n < tClusters_; n++)//loop over clusters
	  {
	    if(tTracks_emcid[h]==tClusters_emcid[n]){
	      n_assoc = n; 
	      break; 
	    }
	  }
	if(n_assoc<0) continue; 
	if(!tClusters_passNotBad[n_assoc]) continue; // NEED FOR MC 

	if((tClusters_energy[n_assoc]-tTracks_mom[h])>RECOVERED_CLUSTER_MIN_E){

	  float scale = (tClusters_energy[n_assoc]-tTracks_mom[h])/
	                tClusters_energy[n_assoc]; 

	  fastjet::PseudoJet pseudoNeutral(scale*tClusters_px[n_assoc],
					   scale*tClusters_py[n_assoc],
					   scale*tClusters_pz[n_assoc],
					   scale*tClusters_energy[n_assoc]);
  
	  h_cluster_phi -> Fill(tClusters_phi[n_assoc]);
	  h_cluster_eta -> Fill(tClusters_eta[n_assoc]);

	  pseudoNeutral.set_user_index(index);
	  particlePx[index] = scale*tClusters_px[n_assoc];
	  particlePy[index] = scale*tClusters_py[n_assoc];
	  particlePz[index] = scale*tClusters_pz[n_assoc];
	  ertFired[index] = false; // should be filled above
	  particleCharge[index] = 0; 
	  jetParticles_all.push_back(pseudoNeutral);
	  index++;
	  my_clusters++;

	}
 
      }//end h (loop over tracks)

    //done with making particles, make jets
    fastjet::ClusterSequence jetAll(jetParticles_all, *re_antikt);
    std::vector<fastjet::PseudoJet> fastAll = jetAll.inclusive_jets( minPt_jets );//these are jets
  
    unsigned int fsize = fastAll.size();
    bool doFakeJets = !(fsize>0); 
  
    ResetRecoJetVars();//set them to -999
   
    for (unsigned int n = 0; n < fsize; n++) {

      fastjet::PseudoJet aFastJet = fastAll[n];
	    
      float chargedPt = 0.0;
      float chargedPx = 0.0;
      float chargedPy = 0.0;
      float chargedPz = 0.0;
      float neutralPt = 0.0;
      float neutralPx = 0.0;
      float neutralPy = 0.0;
      float neutralPz = 0.0;
      float pT        = aFastJet.perp();
      float eta       = aFastJet.pseudorapidity();
      float phi       = phiReduce( aFastJet.phi() );
	    
      //These part is only in the fsize-loop
      vector<fastjet::PseudoJet> constituents = jetAll.constituents(aFastJet);
      unsigned int nc = constituents.size();
	   
      bool ertJet = false;
      unsigned int chgd_nc = 0; 
      unsigned int neut_nc = 0; 
      for (unsigned int iconst = 0; iconst < nc; iconst++) {
	unsigned int indx = constituents[iconst].user_index(); 
	if (indx < indexCharged){//Charged particles
	  chargedPx += particlePx[indx]; 
	  chargedPy += particlePy[indx]; 
	  chargedPz += particlePz[indx];
	  chgd_nc++; 
	}
	else{
	  neutralPx += particlePx[indx]; 
	  neutralPy += particlePy[indx]; 
	  neutralPz += particlePz[indx];
	  neut_nc++; 
	}
	if(ertFired[indx] == true) ertJet = true;
      }
      chargedPt = sqrt(chargedPx*chargedPx + chargedPy*chargedPy);
      float cf= chargedPt/pT;
      neutralPt = sqrt(neutralPx*neutralPx + neutralPy*neutralPy);

      //RECO JET CUTS
      bool passJetLevelCuts = (nc >= NUMCONST_CUT) && 
	                      (cf<CFHIGH_CUT) && (cf>CFLOW_CUT) && 
	                      (JetEtaAcceptance(eta,re_R,tEvent_vertex));
      if(!passJetLevelCuts) continue;
           
      //fill arrays as well
      //good jets will always be stored starting at index 0 by using nRecoJets (# of good jets)
      re_pT[nRecoJets]    = pT; 
      re_eta[nRecoJets]   = eta; 
      re_phi[nRecoJets]   = phi; 
      re_nc[nRecoJets]    = nc;
      re_chgd_nc[nRecoJets] = chgd_nc; 
      re_neut_nc[nRecoJets] = neut_nc; 
      re_cf[nRecoJets]    = cf; 
      re_ertFired[nRecoJets] = ertJet;

      if(chargedPt>0.0)
        re_tphi[nRecoJets] = phiReduce(atan2(chargedPy,chargedPx)); 
      else
	re_tphi[nRecoJets] = -9999.0; 

      if(neutralPt>0.0)
        re_nphi[nRecoJets] = phiReduce(atan2(neutralPy,neutralPx)); 
      else
	re_nphi[nRecoJets] = -9999.0; 

      if(chargedPt==0.0)
	re_teta[nRecoJets] = -9999.0; 
      else if(chargedPz==0.0)
	re_teta[nRecoJets] = 0.0; 
      else
        re_teta[nRecoJets] = -log(tan(atan2(chargedPt,chargedPz)/2.0));

      if(neutralPt==0.0)
	re_neta[nRecoJets] = -9999.0; 
      else if(neutralPz==0.0)
	re_neta[nRecoJets] = 0.0; 
      else
        re_neta[nRecoJets] = -log(tan(atan2(neutralPt,neutralPz)/2.0)); 


      float det_eta = GetDeta(re_eta[nRecoJets]); 

      re_ml_pT[nRecoJets] = GetMachineLearningTransverseMomentum(re_pT[nRecoJets], re_cf[nRecoJets], re_phi[nRecoJets], det_eta, 
								 re_nc[nRecoJets], re_tphi[nRecoJets]-re_nphi[nRecoJets], 
								 re_teta[nRecoJets]-re_neta[nRecoJets]); 
 
      //Insert ZG code here
      JetSubstructure tempJet;
      setSubstructure(*C_A,constituents,&tempJet,particleCharge);
     
      re_z_g[nRecoJets]   = tempJet.z_g; 
      re_pT1[nRecoJets]   = tempJet.pT1; 
      re_eta1[nRecoJets]  = tempJet.eta1; 
      re_phi1[nRecoJets]  = tempJet.phi1; 
      re_nc1[nRecoJets]   = tempJet.nc1; 
      re_chgd_nc1[nRecoJets]   = tempJet.chgd_nc1; 
      re_neut_nc1[nRecoJets]   = tempJet.neut_nc1;
      re_cf1[nRecoJets]   = tempJet.cf1; 
      re_pT2[nRecoJets]   = tempJet.pT2; 
      re_eta2[nRecoJets]  = tempJet.eta2; 
      re_phi2[nRecoJets]  = tempJet.phi2; 
      re_nc2[nRecoJets]   = tempJet.nc2; 
      re_chgd_nc2[nRecoJets]   = tempJet.chgd_nc2; 
      re_neut_nc2[nRecoJets]   = tempJet.neut_nc2; 
      re_cf2[nRecoJets]   = tempJet.cf2; 
      re_oang[nRecoJets]   = tempJet.oang; 
      re_iter[nRecoJets]  = tempJet.iter; 
      
      if(nRecoJets<kMaxRecoJets){
        nRecoJets++;
      }
      else{
	cout << "ERROR : >kMaxRecoJets, results truncated" << endl; 
      }

      my_recos++;

      
    }//end n: loop over jets made from tracks and clusters

    rJets->Fill(); 
   
    ///////////
    ////End make jets from tracks and clusters}///    
    //////////////////////////////////             

    if( writeHadron || writeParton ) { 

      /////////////////////////////////////////        
      //// make jets from tPythiaParticles ////    
      /////////////////////////////////////////        

      //make particles from tPythiaHadron
      const int indexTotal = tPythiaHadron_;
      float particleCharge[indexTotal];	
      int index = 0; 

      std::vector<fastjet::PseudoJet> partParticles_all;
      partParticles_all.clear();
    
      if( writeHadron && (!writeParton) ){

	//make particles from tPythiaHadron
	int index = 0; 
	for (int h = 0; h < tPythiaHadron_; h++)//loop over tracks
	  {

	    // Eliminate neutrinos from truth jet
	    if(tPythiaHadron_isNeutrino[h]) continue; 

	    fastjet::PseudoJet pythiaParticle(tPythiaHadron_px[h],
					      tPythiaHadron_py[h],
					      tPythiaHadron_pz[h],
					      tPythiaHadron_energy[h]);

	    pythiaParticle.set_user_index(index);
	    particleCharge[index] = tPythiaHadron_charge[h];
	    index++;
	    partParticles_all.push_back(pythiaParticle);
	    my_partis++;

	  }//end h (loop over particles)
      }//end-writeHadron 

      //make particles from tPythiaParton
      if( writeParton && (!writeHadron) ){
	for (int h = 0; h < tPythiaParton_; h++)//loop over tracks
	  {
	    fastjet::PseudoJet pythiaParticle(tPythiaParton_px[h],
					      tPythiaParton_py[h],
					      tPythiaParton_pz[h],
					      tPythiaParton_energy[h]);
	    //PARTON CUTS     

	    partParticles_all.push_back(pythiaParticle);
	    my_partis++;
	  }//end h (loop over particles)
      }//end-writeParton 
      //done with making particles, make jets
      fastjet::ClusterSequence pythAll(partParticles_all, *tr_antikt);
      std::vector<fastjet::PseudoJet> truthAll = pythAll.inclusive_jets( minPt_jets );//these are jets
   
      unsigned int tsize = truthAll.size();

      ResetTruthJetVars();

      for (unsigned int n = 0; n < tsize; n++) {
	fastjet::PseudoJet aPythJet = truthAll[n];
	
	float pT        = aPythJet.perp();
	float eta       = aPythJet.pseudorapidity();
	float phi       = phiReduce( aPythJet.phi() );
	    
	vector<fastjet::PseudoJet> constituents = pythAll.constituents(aPythJet);
	unsigned int nc = constituents.size();

	// constituent cut ONLY for hadron jets
	bool passJetLevelCuts = (nc >= NUMCONST_CUT) || writeParton;  
	if(!passJetLevelCuts) continue;

	float cf = -9999.0; 
	unsigned int chgd_nc = 0; 
	unsigned int neut_nc = 0; 
	if(writeHadron && !(writeParton)){
	  float chargedPx = 0.0; 
	  float chargedPy = 0.0; 
	  for (unsigned int iconst = 0; iconst < nc; iconst++) {
	    unsigned int indx = constituents[iconst].user_index(); 
	    if (particleCharge[indx]!=0.0){//Charged particles
	      chargedPx += constituents[iconst].px(); 
	      chargedPy += constituents[iconst].py();
	      chgd_nc++; 
	    }
	    else
	      neut_nc++; 
	  }
	  float chargedPt = sqrt(chargedPx*chargedPx + chargedPy*chargedPy);
	  cf = chargedPt/pT;
	}
            
	//fill arrays as well
	tr_pT[nTruthJets]    = pT; 
	tr_eta[nTruthJets]   = eta; 
	tr_phi[nTruthJets]   = phi; 
	tr_nc[nTruthJets]    = nc; 
	tr_chgd_nc[nTruthJets]    = chgd_nc; 
	tr_neut_nc[nTruthJets]    = neut_nc; 
	tr_cf[nTruthJets]    = cf; 
      
	//Insert ZGcode here
	JetSubstructure     tTemp;
	setSubstructure(*C_A,constituents,&tTemp,particleCharge);
     
	tr_z_g[nTruthJets]   = tTemp.z_g; 
	tr_pT1[nTruthJets]   = tTemp.pT1; 
	tr_eta1[nTruthJets]  = tTemp.eta1; 
	tr_phi1[nTruthJets]  = tTemp.phi1; 
	tr_nc1[nTruthJets]   = tTemp.nc1; 
	tr_chgd_nc1[nTruthJets]   = tTemp.chgd_nc1; 
	tr_neut_nc1[nTruthJets]   = tTemp.neut_nc1; 
	tr_cf1[nTruthJets]   = tTemp.cf1; 
	tr_pT2[nTruthJets]   = tTemp.pT2; 
	tr_eta2[nTruthJets]  = tTemp.eta2; 
	tr_phi2[nTruthJets]  = tTemp.phi2; 
	tr_nc2[nTruthJets]   = tTemp.nc2; 
	tr_chgd_nc2[nTruthJets]   = tTemp.chgd_nc2; 
	tr_neut_nc2[nTruthJets]   = tTemp.neut_nc2; 
	tr_cf2[nTruthJets]   = tTemp.cf2; 
	tr_oang[nTruthJets]   = tTemp.oang; 
    
	tr_iter[nTruthJets]  = tTemp.iter; 
     
	if(nTruthJets<kMaxTruthJets){
	  nTruthJets++;
	}
	else{
	  cout << "ERROR : >kMaxTruthJets, results truncated" << endl; 
	}
 
	my_trues++;

      }//end n: loop over jets made from tPythiaHadron	
     
      tJets -> Fill();

      //////////////////////////////////////////////           
      //////End make jets from tPythiaParticles}////   
      //////////////////////////////////////////////           
 
      ///////////////////////////////////////////////
      // Make restricted truth jets from restricted
      // set of truth particles (only for hadrons)
      ///////////////////////////////////////////////

      partParticles_all.clear();
      if( writeHadron && (!writeParton) ){

	//make particles from tPythiaHadron
	const int indexTotal = tPythiaHadron_;
	float particleCharge[indexTotal];

	int index = 0; 
	for (int h = 0; h < tPythiaHadron_; h++)//loop over tracks
	  {

	    // Eliminate neutrinos from truth jet
	    if(tPythiaHadron_isNeutrino[h]) continue;

	    // Eliminate neutrons and K0L (no hadronic calorimeter)
	    if(abs(tPythiaHadron_kf[h])==2112) continue;
	    if(abs(tPythiaHadron_kf[h])==130) continue;

	    //Need phi_arm to fall into the [0,1,2,3,4,5,6,7] range to be valid 
	    int phi_arm = (PHENIXFilter(tPythiaHadron_pT[h], tPythiaHadron_phi[h], tPythiaHadron_pz[h], tEvent_vertex, tPythiaHadron_charge[h]));

	    if( (phi_arm<0) || (phi_arm>7) ) continue;
	    if(tPythiaHadron_charge[h] == 0){
	      if(tPythiaHadron_energy[h] < RESTRICTED_CLUSTER_MIN_E)continue;
	    }else{
	      if(tPythiaHadron_pT[h] < RESTRICTED_TRACK_MIN_PT)continue;       
	    }

	    fastjet::PseudoJet pythiaParticle(tPythiaHadron_px[h],
					      tPythiaHadron_py[h],
					      tPythiaHadron_pz[h],
					      tPythiaHadron_energy[h]);

	    pythiaParticle.set_user_index(index);
	    particleCharge[index] = tPythiaHadron_charge[h];
	    partParticles_all.push_back(pythiaParticle);
	    index++; 
	    my_rpartis++;

	  }//end h (loop over particles)

	//done with making particles, make jets
	fastjet::ClusterSequence rpythAll(partParticles_all, *tr_antikt);
	std::vector<fastjet::PseudoJet> rtruthAll = rpythAll.inclusive_jets( minPt_jets );//these are jets
   
	//assign fields here
	unsigned int rtsize = rtruthAll.size();

	ResetRestrictedTruthJetVars();

	for (unsigned int n = 0; n < rtsize; n++) {
	  fastjet::PseudoJet aPythJet = rtruthAll[n];
	
	  float pT        = aPythJet.perp();
	  float eta       = aPythJet.pseudorapidity();
	  float phi       = phiReduce( aPythJet.phi() );
	    
	  vector<fastjet::PseudoJet> constituents = rpythAll.constituents(aPythJet);
	  unsigned int nc = constituents.size();

	  float chargedPx = 0.0; 
	  float chargedPy = 0.0; 
	  float chargedPz = 0.0; 
	  float neutralPx = 0.0; 
	  float neutralPy = 0.0; 
	  float neutralPz = 0.0; 
	  unsigned int chgd_nc = 0; 
	  unsigned int neut_nc = 0; 
	  for (unsigned int iconst = 0; iconst < nc; iconst++) {
	    unsigned int indx = constituents[iconst].user_index(); 
	    if (particleCharge[indx]!=0.0){//Charged particles
	      chargedPx += constituents[iconst].px(); 
	      chargedPy += constituents[iconst].py(); 
	      chargedPz += constituents[iconst].pz();
	      chgd_nc++; 
	    }
	    else{
	      neutralPx += constituents[iconst].px(); 
	      neutralPy += constituents[iconst].py(); 
	      neutralPz += constituents[iconst].pz();
	      neut_nc++; 
	    }
	  }
	  float chargedPt = sqrt(chargedPx*chargedPx + chargedPy*chargedPy);
	  float cf = chargedPt/pT;
	  float neutralPt = sqrt(neutralPx*chargedPx + neutralPy*neutralPy);

	  bool passJetLevelCuts = (nc >= NUMCONST_CUT) &&
	    // no CF cut for restricted truth jets!
	    //  (cf<CFHIGH_CUT) && (cf>CFLOW_CUT) && 
	    (JetEtaAcceptance(eta,tr_R,tEvent_vertex));
	  if(!passJetLevelCuts) continue;
            
	  //fill arrays as well
	  rtr_pT[nRestrictedTruthJets]    = pT; 
	  rtr_eta[nRestrictedTruthJets]   = eta; 
	  rtr_phi[nRestrictedTruthJets]   = phi; 
	  rtr_nc[nRestrictedTruthJets]    = nc; 
	  rtr_chgd_nc[nRestrictedTruthJets]    = chgd_nc; 
	  rtr_neut_nc[nRestrictedTruthJets]    = neut_nc; 
	  rtr_cf[nRestrictedTruthJets]    = cf; 
 
	  if(chargedPt>0.0)
	    rtr_tphi[nRestrictedTruthJets] = phiReduce(atan2(chargedPy,chargedPx));
	  else
	    rtr_tphi[nRestrictedTruthJets] = -9999.0; 

	  if(neutralPt>0.0)
	    rtr_nphi[nRestrictedTruthJets] = phiReduce(atan2(neutralPy,neutralPx)); 
	  else
	    rtr_nphi[nRestrictedTruthJets] = -9999.0; 

	  if(chargedPt==0.0)
	    rtr_teta[nRestrictedTruthJets] = -9999.0; 
	  else if(chargedPz==0.0)
	    rtr_teta[nRestrictedTruthJets] = 0.0; 
	  else
	    rtr_teta[nRestrictedTruthJets] = -log(tan(atan2(chargedPt,chargedPz)/2.0)); 

	  if(neutralPt==0.0)
	    rtr_neta[nRestrictedTruthJets] = -9999.0; 
	  else if(neutralPz==0.0)
	    rtr_neta[nRestrictedTruthJets] = 0.0; 
	  else
	    rtr_neta[nRestrictedTruthJets] = -log(tan(atan2(neutralPt,neutralPz)/2.0)); 
     
	  //Insert ZGcode here
	  JetSubstructure     tTemp;
	  setSubstructure(*C_A,constituents,&tTemp,particleCharge);
     
	  rtr_z_g[nRestrictedTruthJets]   = tTemp.z_g; 
	  rtr_pT1[nRestrictedTruthJets]   = tTemp.pT1; 
	  rtr_eta1[nRestrictedTruthJets]  = tTemp.eta1; 
	  rtr_phi1[nRestrictedTruthJets]  = tTemp.phi1; 
	  rtr_nc1[nRestrictedTruthJets]   = tTemp.nc1; 
	  rtr_chgd_nc1[nRestrictedTruthJets]   = tTemp.chgd_nc1; 
	  rtr_neut_nc1[nRestrictedTruthJets]   = tTemp.neut_nc1; 
	  rtr_cf1[nRestrictedTruthJets]   = tTemp.cf1; 
	  rtr_pT2[nRestrictedTruthJets]   = tTemp.pT2; 
	  rtr_eta2[nRestrictedTruthJets]  = tTemp.eta2; 
	  rtr_phi2[nRestrictedTruthJets]  = tTemp.phi2; 
	  rtr_nc2[nRestrictedTruthJets]   = tTemp.nc2; 
	  rtr_chgd_nc2[nRestrictedTruthJets]   = tTemp.chgd_nc2; 
	  rtr_neut_nc2[nRestrictedTruthJets]   = tTemp.neut_nc2; 
	  rtr_cf2[nRestrictedTruthJets]   = tTemp.cf2; 
	  rtr_oang[nRestrictedTruthJets]   = tTemp.oang; 
    
	  rtr_iter[nRestrictedTruthJets]  = tTemp.iter; 
     
	  if(nRestrictedTruthJets<kMaxRestrictedTruthJets){
	    nRestrictedTruthJets++;
	  }
	  else{
	    cout << "ERROR : >kMaxRestrictedTruthJets, results truncated" << endl; 
	  }
 
	  my_rtrues++;

	}//end n: loop over jets made from tPythiaHadron	
     
	rtJets -> Fill();

      }//end-writeHadron 

      /////////////////////////////////////            
      //////End make restricted truth jets
      /////////////////////////////////////            
 
      ////////////////////////////////        
      ///match recoJet and trueJet ///   
      ////////////////////////////////

      //set the variables to -999
      ResetMatchedJetVars();

      for(unsigned int r = 0; r < nRecoJets; r++){

	float match_dR = MATCHED_JET_DELTA_R;
	int matchedIDX = -1; 

	for(unsigned int t = 0; t < nTruthJets; t++){
 
	  float deltaR = dR(re_eta[r], tr_eta[t], re_phi[r], tr_phi[t]);

	  if(deltaR < match_dR){
	    match_dR = deltaR; 
	    matchedIDX = t; 
	  }

	}// end loop over truth jets

	if(matchedIDX>=0){
          
	  m_r_pT[nMatchedJets]   = re_pT[r];
	  m_t_pT[nMatchedJets]   = tr_pT[matchedIDX];
	  m_r_eta[nMatchedJets]  = re_eta[r]; 
	  m_t_eta[nMatchedJets]  = tr_eta[matchedIDX];
	  m_r_phi[nMatchedJets]  = re_phi[r];
	  m_t_phi[nMatchedJets]  = tr_phi[matchedIDX];
	  m_r_nc[nMatchedJets]   = re_nc[r];
	  m_r_chgd_nc[nMatchedJets]   = re_chgd_nc[r];
	  m_r_neut_nc[nMatchedJets]   = re_neut_nc[r];
	  m_t_nc[nMatchedJets]   = tr_nc[matchedIDX];
	  m_t_chgd_nc[nMatchedJets]   = tr_chgd_nc[matchedIDX];
	  m_t_neut_nc[nMatchedJets]   = tr_neut_nc[matchedIDX];

	  m_r_iter[nMatchedJets] = re_iter[r];
	  m_t_iter[nMatchedJets] = tr_iter[matchedIDX];

	  m_r_cf[nMatchedJets]      = re_cf[r];
	  m_t_cf[nMatchedJets]      = tr_cf[matchedIDX];

	  m_r_ml_pT[nMatchedJets]   = re_ml_pT[r];

	  m_r_ertFired[nMatchedJets] = re_ertFired[r]; 

	  m_r_tphi[nMatchedJets] = re_tphi[r]; 
	  m_r_nphi[nMatchedJets] = re_nphi[r]; 
	  m_r_teta[nMatchedJets] = re_teta[r]; 
	  m_r_neta[nMatchedJets] = re_neta[r]; 
         
	  m_r_z_g[nMatchedJets]  = re_z_g[r];
	  m_r_pT1[nMatchedJets]  = re_pT1[r];
	  m_r_eta1[nMatchedJets] = re_eta1[r];
	  m_r_phi1[nMatchedJets] = re_phi1[r];
	  m_r_nc1[nMatchedJets]  = re_nc1[r];
	  m_r_chgd_nc1[nMatchedJets]  = re_chgd_nc1[r];
	  m_r_neut_nc1[nMatchedJets]  = re_neut_nc1[r];
	  m_r_cf1[nMatchedJets]  = re_cf1[r];
	  m_r_pT2[nMatchedJets]  = re_pT2[r];
	  m_r_eta2[nMatchedJets] = re_eta2[r];
	  m_r_phi2[nMatchedJets] = re_phi2[r];
	  m_r_nc2[nMatchedJets]  = re_nc2[r];
	  m_r_chgd_nc2[nMatchedJets]  = re_chgd_nc2[r];
	  m_r_neut_nc2[nMatchedJets]  = re_neut_nc2[r];
	  m_r_cf2[nMatchedJets]  = re_cf2[r];
	  m_r_oang[nMatchedJets]  = re_oang[r];
          
	  m_t_z_g[nMatchedJets]  = tr_z_g[matchedIDX];
	  m_t_pT1[nMatchedJets]  = tr_pT1[matchedIDX];
	  m_t_eta1[nMatchedJets] = tr_eta1[matchedIDX];
	  m_t_phi1[nMatchedJets] = tr_phi1[matchedIDX];
	  m_t_nc1[nMatchedJets]  = tr_nc1[matchedIDX];
	  m_t_chgd_nc1[nMatchedJets]  = tr_chgd_nc1[matchedIDX];
	  m_t_neut_nc1[nMatchedJets]  = tr_neut_nc1[matchedIDX];
	  m_t_cf1[nMatchedJets]  = tr_cf1[matchedIDX];
	  m_t_pT2[nMatchedJets]  = tr_pT2[matchedIDX];
	  m_t_eta2[nMatchedJets] = tr_eta2[matchedIDX];
	  m_t_phi2[nMatchedJets] = tr_phi2[matchedIDX];
	  m_t_nc2[nMatchedJets]  = tr_nc2[matchedIDX];
	  m_t_chgd_nc2[nMatchedJets]  = tr_chgd_nc2[matchedIDX];
	  m_t_neut_nc2[nMatchedJets]  = tr_neut_nc2[matchedIDX];
	  m_t_cf2[nMatchedJets]  = tr_cf2[matchedIDX];
	  m_t_oang[nMatchedJets]  = tr_oang[matchedIDX];

	  m_dphi[nMatchedJets] = dphiReduce(re_phi[r] - tr_phi[matchedIDX]); 
	  m_deta[nMatchedJets] = re_eta[r] - tr_eta[matchedIDX]; 
	  m_dR[nMatchedJets] = dR(re_eta[r], tr_eta[matchedIDX], re_phi[r], tr_phi[matchedIDX]); 

	  if(nMatchedJets<kMaxMatchedJets){
	    nMatchedJets++;
	  }
	  else{
	    cout << "ERROR : >kMaxMatchedJets, results truncated" << endl; 
	  }
	  my_matchos++;

	}//found matched jet

      }//end r: loop over num_rJets

      //Fill for every single event even if there's no matched jet
      mJets->Fill();

      ///////////////////////////////////////////        
      ///match recoJet and RESTRICTED trueJet ///   
      ///////////////////////////////////////////

      //set the variables to -999
      ResetRestrictedMatchedJetVars();

      for(unsigned int r = 0; r < nRecoJets; r++){

	float match_dR = MATCHED_JET_DELTA_R;
	int matchedIDX = -1; 

	for(unsigned int t = 0; t < nRestrictedTruthJets; t++){
 
	  float deltaR = dR(re_eta[r], rtr_eta[t], re_phi[r], rtr_phi[t]);

	  if(deltaR < match_dR){
	    match_dR = deltaR; 
	    matchedIDX = t; 
	  }

	}// end loop over truth jets

	if(matchedIDX>=0){
          
	  rm_r_pT[nRestrictedMatchedJets]   = re_pT[r];
	  rm_t_pT[nRestrictedMatchedJets]   = rtr_pT[matchedIDX];
	  rm_r_eta[nRestrictedMatchedJets]  = re_eta[r]; 
	  rm_t_eta[nRestrictedMatchedJets]  = rtr_eta[matchedIDX];
	  rm_r_phi[nRestrictedMatchedJets]  = re_phi[r];
	  rm_t_phi[nRestrictedMatchedJets]  = rtr_phi[matchedIDX];
	  rm_r_nc[nRestrictedMatchedJets]   = re_nc[r];
	  rm_r_chgd_nc[nRestrictedMatchedJets]   = re_chgd_nc[r];
	  rm_r_neut_nc[nRestrictedMatchedJets]   = re_neut_nc[r];
	  rm_t_nc[nRestrictedMatchedJets]   = rtr_nc[matchedIDX];
	  rm_t_chgd_nc[nRestrictedMatchedJets]   = rtr_chgd_nc[matchedIDX];
	  rm_t_neut_nc[nRestrictedMatchedJets]   = rtr_neut_nc[matchedIDX];

	  rm_r_iter[nRestrictedMatchedJets] = re_iter[r];
	  rm_t_iter[nRestrictedMatchedJets] = rtr_iter[matchedIDX];

	  rm_r_cf[nRestrictedMatchedJets]      = re_cf[r];
	  rm_t_cf[nRestrictedMatchedJets]      = rtr_cf[matchedIDX];

	  rm_r_ml_pT[nRestrictedMatchedJets]   = re_ml_pT[r];
	  rm_r_ertFired[nRestrictedMatchedJets] = re_ertFired[r]; 
       
	  rm_r_tphi[nRestrictedMatchedJets] = re_tphi[r]; 
	  rm_r_nphi[nRestrictedMatchedJets] = re_nphi[r]; 
	  rm_r_teta[nRestrictedMatchedJets] = re_teta[r]; 
	  rm_r_neta[nRestrictedMatchedJets] = re_neta[r]; 

	  rm_t_tphi[nRestrictedMatchedJets] = rtr_tphi[matchedIDX]; 
	  rm_t_nphi[nRestrictedMatchedJets] = rtr_nphi[matchedIDX]; 
	  rm_t_teta[nRestrictedMatchedJets] = rtr_teta[matchedIDX]; 
	  rm_t_neta[nRestrictedMatchedJets] = rtr_neta[matchedIDX]; 
  
	  rm_r_z_g[nRestrictedMatchedJets]  = re_z_g[r];
	  rm_r_pT1[nRestrictedMatchedJets]  = re_pT1[r];
	  rm_r_eta1[nRestrictedMatchedJets] = re_eta1[r];
	  rm_r_phi1[nRestrictedMatchedJets] = re_phi1[r];
	  rm_r_nc1[nRestrictedMatchedJets]  = re_nc1[r];
	  rm_r_chgd_nc1[nRestrictedMatchedJets]  = re_chgd_nc1[r];
	  rm_r_neut_nc1[nRestrictedMatchedJets]  = re_neut_nc1[r];
	  rm_r_cf1[nRestrictedMatchedJets]  = re_cf1[r];
	  rm_r_pT2[nRestrictedMatchedJets]  = re_pT2[r];
	  rm_r_eta2[nRestrictedMatchedJets] = re_eta2[r];
	  rm_r_phi2[nRestrictedMatchedJets] = re_phi2[r];
	  rm_r_nc2[nRestrictedMatchedJets]  = re_nc2[r];
	  rm_r_chgd_nc2[nRestrictedMatchedJets]  = re_chgd_nc2[r];
	  rm_r_neut_nc2[nRestrictedMatchedJets]  = re_neut_nc2[r];
	  rm_r_cf2[nRestrictedMatchedJets]  = re_cf2[r];
	  rm_r_oang[nRestrictedMatchedJets]  = re_oang[r];
          
	  rm_t_z_g[nRestrictedMatchedJets]  = rtr_z_g[matchedIDX];
	  rm_t_pT1[nRestrictedMatchedJets]  = rtr_pT1[matchedIDX];
	  rm_t_eta1[nRestrictedMatchedJets] = rtr_eta1[matchedIDX];
	  rm_t_phi1[nRestrictedMatchedJets] = rtr_phi1[matchedIDX];
	  rm_t_nc1[nRestrictedMatchedJets]  = rtr_nc1[matchedIDX];
	  rm_t_chgd_nc1[nRestrictedMatchedJets]  = rtr_chgd_nc1[matchedIDX];
	  rm_t_neut_nc1[nRestrictedMatchedJets]  = rtr_neut_nc1[matchedIDX];
	  rm_t_cf1[nRestrictedMatchedJets]  = rtr_cf1[matchedIDX];
	  rm_t_pT2[nRestrictedMatchedJets]  = rtr_pT2[matchedIDX];
	  rm_t_eta2[nRestrictedMatchedJets] = rtr_eta2[matchedIDX];
	  rm_t_phi2[nRestrictedMatchedJets] = rtr_phi2[matchedIDX];
	  rm_t_nc2[nRestrictedMatchedJets]  = rtr_nc2[matchedIDX];
	  rm_t_chgd_nc2[nRestrictedMatchedJets]  = rtr_chgd_nc2[matchedIDX];
	  rm_t_neut_nc2[nRestrictedMatchedJets]  = rtr_neut_nc2[matchedIDX];
	  rm_t_cf2[nRestrictedMatchedJets]  = rtr_cf2[matchedIDX];
	  rm_t_oang[nRestrictedMatchedJets]  = rtr_oang[matchedIDX];

	  rm_dphi[nRestrictedMatchedJets] = dphiReduce(re_phi[r] - rtr_phi[matchedIDX]); 
	  rm_deta[nRestrictedMatchedJets] = re_eta[r] - rtr_eta[matchedIDX]; 
	  rm_dR[nRestrictedMatchedJets] = dR(re_eta[r], rtr_eta[matchedIDX], re_phi[r], rtr_phi[matchedIDX]); 

	  if(nRestrictedMatchedJets<kMaxRestrictedMatchedJets){
	    nRestrictedMatchedJets++;
	  }
	  else{
	    cout << "ERROR : >kMaxRestrictedMatchedJets, results truncated" << endl; 
	  }
	  my_rmatchos++;

	}//found restricted matched jet

      }//end r: loop over num_rJets

      //Fill for every single event even if there's no matched jet
      rmJets->Fill();

      ///////////////////////////////////////////        
      /// match RESTRICTED trueJet and trueJet ///   
      ///////////////////////////////////////////

      //set the variables to -999
      ResetTruthRestrictedMatchedJetVars();

      for(unsigned int r = 0; r < nRestrictedTruthJets; r++){

	float match_dR = MATCHED_JET_DELTA_R;
	int matchedIDX = -1; 

	for(unsigned int t = 0; t < nTruthJets; t++){
 
	  float deltaR = dR(rtr_eta[r], tr_eta[t], rtr_phi[r], tr_phi[t]);

	  if(deltaR < match_dR){
	    match_dR = deltaR; 
	    matchedIDX = t; 
	  }

	}// end loop over truth jets

	if(matchedIDX>=0){
          
	  trm_r_pT[nTruthRestrictedMatchedJets]   = rtr_pT[r];
	  trm_t_pT[nTruthRestrictedMatchedJets]   = tr_pT[matchedIDX];
	  trm_r_eta[nTruthRestrictedMatchedJets]  = rtr_eta[r]; 
	  trm_t_eta[nTruthRestrictedMatchedJets]  = tr_eta[matchedIDX];
	  trm_r_phi[nTruthRestrictedMatchedJets]  = rtr_phi[r];
	  trm_t_phi[nTruthRestrictedMatchedJets]  = tr_phi[matchedIDX];
	  trm_r_nc[nTruthRestrictedMatchedJets]   = rtr_nc[r];
	  trm_r_chgd_nc[nTruthRestrictedMatchedJets]   = rtr_chgd_nc[r];
	  trm_r_neut_nc[nTruthRestrictedMatchedJets]   = rtr_neut_nc[r];
	  trm_t_nc[nTruthRestrictedMatchedJets]   = tr_nc[matchedIDX];
	  trm_t_chgd_nc[nTruthRestrictedMatchedJets]   = tr_chgd_nc[matchedIDX];
	  trm_t_neut_nc[nTruthRestrictedMatchedJets]   = tr_neut_nc[matchedIDX];

	  trm_r_iter[nTruthRestrictedMatchedJets] = rtr_iter[r];
	  trm_t_iter[nTruthRestrictedMatchedJets] = tr_iter[matchedIDX];

	  trm_r_cf[nTruthRestrictedMatchedJets]      = rtr_cf[r];
	  trm_t_cf[nTruthRestrictedMatchedJets]      = tr_cf[matchedIDX];
  
	  trm_r_tphi[nTruthRestrictedMatchedJets] = rtr_tphi[r]; 
	  trm_r_nphi[nTruthRestrictedMatchedJets] = rtr_nphi[r]; 
	  trm_r_teta[nTruthRestrictedMatchedJets] = rtr_teta[r]; 
	  trm_r_neta[nTruthRestrictedMatchedJets] = rtr_neta[r]; 
       
	  trm_r_z_g[nTruthRestrictedMatchedJets]  = rtr_z_g[r];
	  trm_r_pT1[nTruthRestrictedMatchedJets]  = rtr_pT1[r];
	  trm_r_eta1[nTruthRestrictedMatchedJets] = rtr_eta1[r];
	  trm_r_phi1[nTruthRestrictedMatchedJets] = rtr_phi1[r];
	  trm_r_nc1[nTruthRestrictedMatchedJets]  = rtr_nc1[r];
	  trm_r_chgd_nc1[nTruthRestrictedMatchedJets]  = rtr_chgd_nc1[r];
	  trm_r_neut_nc1[nTruthRestrictedMatchedJets]  = rtr_neut_nc1[r];
	  trm_r_cf1[nTruthRestrictedMatchedJets]  = rtr_cf1[r];
	  trm_r_pT2[nTruthRestrictedMatchedJets]  = rtr_pT2[r];
	  trm_r_eta2[nTruthRestrictedMatchedJets] = rtr_eta2[r];
	  trm_r_phi2[nTruthRestrictedMatchedJets] = rtr_phi2[r];
	  trm_r_nc2[nTruthRestrictedMatchedJets]  = rtr_nc2[r];
	  trm_r_chgd_nc2[nTruthRestrictedMatchedJets]  = rtr_chgd_nc2[r];
	  trm_r_neut_nc2[nTruthRestrictedMatchedJets]  = rtr_neut_nc2[r];
	  trm_r_cf2[nTruthRestrictedMatchedJets]  = rtr_cf2[r];
	  trm_r_oang[nTruthRestrictedMatchedJets]  = rtr_oang[r];
          
	  trm_t_z_g[nTruthRestrictedMatchedJets]  = tr_z_g[matchedIDX];
	  trm_t_pT1[nTruthRestrictedMatchedJets]  = tr_pT1[matchedIDX];
	  trm_t_eta1[nTruthRestrictedMatchedJets] = tr_eta1[matchedIDX];
	  trm_t_phi1[nTruthRestrictedMatchedJets] = tr_phi1[matchedIDX];
	  trm_t_nc1[nTruthRestrictedMatchedJets]  = tr_nc1[matchedIDX];
	  trm_t_chgd_nc1[nTruthRestrictedMatchedJets]  = tr_chgd_nc1[matchedIDX];
	  trm_t_neut_nc1[nTruthRestrictedMatchedJets]  = tr_neut_nc1[matchedIDX];
	  trm_t_cf1[nTruthRestrictedMatchedJets]  = tr_cf1[matchedIDX];
	  trm_t_pT2[nTruthRestrictedMatchedJets]  = tr_pT2[matchedIDX];
	  trm_t_eta2[nTruthRestrictedMatchedJets] = tr_eta2[matchedIDX];
	  trm_t_phi2[nTruthRestrictedMatchedJets] = tr_phi2[matchedIDX];
	  trm_t_nc2[nTruthRestrictedMatchedJets]  = tr_nc2[matchedIDX];
	  trm_t_chgd_nc2[nTruthRestrictedMatchedJets]  = tr_chgd_nc2[matchedIDX];
	  trm_t_neut_nc2[nTruthRestrictedMatchedJets]  = tr_neut_nc2[matchedIDX];
	  trm_t_cf2[nTruthRestrictedMatchedJets]  = tr_cf2[matchedIDX];
	  trm_t_oang[nTruthRestrictedMatchedJets]  = tr_oang[matchedIDX];

	  trm_dphi[nTruthRestrictedMatchedJets] = dphiReduce(rtr_phi[r] - tr_phi[matchedIDX]); 
	  trm_deta[nTruthRestrictedMatchedJets] = rtr_eta[r] - tr_eta[matchedIDX]; 
	  trm_dR[nTruthRestrictedMatchedJets] = dR(rtr_eta[r], tr_eta[matchedIDX], rtr_phi[r], tr_phi[matchedIDX]); 

	  if(nTruthRestrictedMatchedJets<kMaxTruthRestrictedMatchedJets){
	    nTruthRestrictedMatchedJets++;
	  }
	  else{
	    cout << "ERROR : >kMaxTruthRestrictedMatchedJets, results truncated" << endl; 
	  }
	  my_trmatchos++;

	}//found restricted matched jet

      }//end r: loop over num_rJets

      //Fill for every single event even if there's no matched jet
      trmJets->Fill();

    }

    // If no reco jets were found, use the event to look for fake jets
    if(doFakeJets){
      my_fakes += GetFakeJets(re_antikt);
      my_fake_evts++; 
    }
 
  }//end i: entries for particle, tracks and clusters

  //we're done making jets
  //delete the algorithm
  delete re_antikt;
  delete tr_antikt;
  delete C_A; 
 
  cout << "Read in from input files: " << endl;
  cout << tot_tracks << " tracks" << endl;
  cout << tot_clusters << " clusters" << endl;
  if(writeParton){
    cout << tot_partis << " partons" << endl;
  }
  if(writeHadron){
    cout << tot_partis << " hadrons" << endl;
  }
  cout << endl; 
  
  cout << "Jet making used:" << endl;
  cout << my_tracks << " tracks" << endl;
  cout << my_clusters << " clusters" << endl;
  if(writeParton){
    cout << my_partis << " partons" << endl;
  }
  if(writeHadron){
    cout << my_partis << " hadrons" << endl;
    cout << my_rpartis << " restricted hadrons" << endl;
  }
  cout << endl; 

  cout  << "MakeJets made:" << endl;
  cout  <<  my_recos    <<  "   reco  jets total"     <<  endl;
  if(writeParton)
    cout  <<  my_trues    <<  "  parton jets total"     <<  endl;
  if(writeHadron){
    cout  <<  my_trues    <<  "  hadron truth jets total"     <<  endl;
    cout  <<  my_rtrues    <<  "  restricted hadron truth jets total"     <<  endl;
  }
  if(writeParton||writeHadron)
    cout  <<  my_matchos  <<  " matched jets total"  <<  endl;
  if(writeHadron){
    cout  <<  my_rmatchos  <<  " restricted matched jets total"  <<  endl;
    cout  <<  my_trmatchos  <<  " restricted-truth matched jets total"  <<  endl;
  }
  cout << my_fakes << " fake jets total in " << my_fake_evts << " events " << endl; 
  cout << endl;
 
  cout << "Jets saved in " << outFileName << endl << endl; 

  fOut->Write();
  fOut->Close();
  
  return 0;
}//end Run()

void MakeJets::setSubstructure(fastjet::JetDefinition C_A,  std::vector<fastjet::PseudoJet> SubCons, JetSubstructure *tTemp, float *particleCharge){
   
  int nSD = 0; 
  bool splitFound = false; 
  int sSize = 0;
  int bigCons = -999; 
  int bigCons_chgd = -999; 
  int bigCons_neut = -999; 
  int smallCons = -999; 
  int smallCons_chgd = -999; 
  int smallCons_neut = -999; 
  float bigCons_cf = -999; 
  float smallCons_cf = -999; 
  float pT_1 = -999.0; 
  float pT_2 = -999.0; 
  float eta1 = -999.0; 
  float eta2 = -999.0;
  float phi1 = -999.0; 
  float phi2 = -999.0;
  float oang = -999.0; 
  fastjet::PseudoJet BigSub;
  fastjet::PseudoJet SmallSub;
  
  // z_g values:
  // -1 : uninitialized
  // -2 : singleton (one jet)
  // -3 : zero jets

  float zg_found = -1.0; 

  do{
    fastjet::ClusterSequence cs(SubCons, C_A);
    std::vector<fastjet::PseudoJet> subJets = cs.exclusive_jets_up_to(2);
    sSize = subJets.size();

    if(sSize==2){

      BigSub          = subJets[0];
      SmallSub        = subJets[1];
      pT_1            = BigSub.perp();
      pT_2            = SmallSub.perp();
      if(pT_1 < pT_2){
	fastjet::PseudoJet tTempJet = SmallSub;
	SmallSub      = BigSub;
	BigSub        = tTempJet;
	float tTempPt  = pT_2;
	pT_2          = pT_1;
	pT_1          = tTempPt;
      }

      float z_g             = pT_2 /(pT_1 + pT_2);
      if(z_g > ZG_CUT){
	zg_found = z_g;
	bigCons = BigSub.constituents().size(); 
	smallCons = SmallSub.constituents().size(); 
	eta1     = BigSub.pseudorapidity();
        phi1     = phiReduce(BigSub.phi());
	eta2     = SmallSub.pseudorapidity();
        phi2     = phiReduce(SmallSub.phi());
	
	float chargedPx = 0.0;  
	float chargedPy = 0.0;  
	float chargedPz = 0.0; 
	bigCons_chgd = 0; 
	smallCons_chgd = 0; 
	bigCons_neut = 0; 
	smallCons_neut = 0; 

	for(unsigned int i=0; i<BigSub.constituents().size(); i++){
	  unsigned int indx = BigSub.constituents().at(i).user_index(); 
	  if(particleCharge[indx]!=0){
	    bigCons_chgd++; 
	    chargedPx += BigSub.constituents().at(i).px(); 
	    chargedPy += BigSub.constituents().at(i).py(); 
	    chargedPz += BigSub.constituents().at(i).pz(); 
	  }
	  else{
	    bigCons_neut++; 
	  }
	}

	bigCons_cf = sqrt(pow(chargedPx,2) + pow(chargedPy,2))/pT_1; 

	chargedPx = 0.0;  
	chargedPy = 0.0;  
	chargedPz = 0.0; 

	for(unsigned int i=0; i<SmallSub.constituents().size(); i++){
	  unsigned int indx = SmallSub.constituents().at(i).user_index(); 
	  if(particleCharge[indx]!=0){
	    smallCons_chgd++; 
	    chargedPx += SmallSub.constituents().at(i).px(); 
	    chargedPy += SmallSub.constituents().at(i).py(); 
	    chargedPz += SmallSub.constituents().at(i).pz(); 
	  }
	  else{
	    smallCons_neut++; 
	  }
	}

	smallCons_cf = sqrt(pow(chargedPx,2) + pow(chargedPy,2))/pT_2; 

	TVector3 bigVect(BigSub.px(),BigSub.py(),BigSub.pz()); 
	TVector3 smallVect(SmallSub.px(),SmallSub.py(),SmallSub.pz()); 

	oang = bigVect.Angle(smallVect); 

	splitFound = true; 
      }
      else{
	SubCons = BigSub.constituents(); 
      }
	      
      // Number of soft drop splittings
      nSD++; 

    }
    else{
      if(sSize==1)
        zg_found = -2;
      else
	zg_found = -3; 
    }

  }while((sSize==2)&&!splitFound); 

  tTemp->pT1 = pT_1; 
  tTemp->pT2 = pT_2;
  tTemp->eta1 = eta1; 
  tTemp->eta2 = eta2; 
  tTemp->phi1 = phi1; 
  tTemp->phi2 = phi2; 
  tTemp->z_g = zg_found; 
  tTemp->nc1 = bigCons; 
  tTemp->chgd_nc1 = bigCons_chgd; 
  tTemp->neut_nc1 = bigCons_neut; 
  tTemp->cf1 = bigCons_cf; 
  tTemp->nc2 = smallCons; 
  tTemp->chgd_nc2 = smallCons_chgd; 
  tTemp->neut_nc2 = smallCons_neut; 
  tTemp->iter = nSD; 
  tTemp->cf2 = smallCons_cf; 
  tTemp->oang = oang; 

  return;
}

float MakeJets::dR(float eta1, float eta2, float phi1, float phi2){
  float deta = eta1 - eta2;
  float dphi = dphiReduce(phi1 - phi2);

  return sqrt(dphi * dphi + deta * deta);
}

void MakeJets::ResetRecoJetVars(){

  nRecoJets = 0;

  for(int i = 0; i < kMaxRecoJets; i++){

   re_pT[i] = -999;
   re_eta[i] = -999;
   re_phi[i] = -999;
   re_nc[i] = -999;
   re_chgd_nc[i] = -999;
   re_neut_nc[i] = -999;
   re_cf[i] = -999;
   re_tphi[i] = -999;
   re_nphi[i] = -999; 
   re_teta[i] = -999;
   re_neta[i] = -999; 
   re_ertFired[i] = false; 
  //substructure info
   re_z_g[i] = -999;
   re_pT1[i] = -999;
   re_eta1[i] = -999;  
   re_phi1[i] = -999;
   re_nc1[i] = -999;
   re_chgd_nc1[i] = -999;
   re_neut_nc1[i] = -999;
   re_cf1[i] = -999;
   re_pT2[i] = -999;
   re_eta2[i] = -999;
   re_phi2[i] = -999;
   re_nc2[i] = -999;
   re_chgd_nc2[i] = -999;
   re_neut_nc2[i] = -999;
   re_cf2[i] = -999;
   re_oang[i] = -999;
   re_iter[i] = -999;

   re_ml_pT[i] = -999; 

  }//i

  return;

}//ResetRecoJetVars

void MakeJets::ResetTruthJetVars(){

  nTruthJets = 0;

  for(int i = 0; i < kMaxTruthJets; i++){

   tr_pT[i] = -999;
   tr_eta[i] = -999;
   tr_phi[i] = -999;
   tr_nc[i] = -999;
   tr_chgd_nc[i] = -999;
   tr_neut_nc[i] = -999;
   tr_cf[i] = -999;

   //substructure info
   tr_z_g[i] = -999;
   tr_pT1[i] = -999;
   tr_eta1[i] = -999;  
   tr_phi1[i] = -999;
   tr_nc1[i] = -999;
   tr_chgd_nc1[i] = -999;
   tr_neut_nc1[i] = -999;
   tr_cf1[i] = -999;
   tr_pT2[i] = -999;
   tr_eta2[i] = -999;
   tr_phi2[i] = -999;
   tr_nc2[i] = -999;
   tr_chgd_nc2[i] = -999;
   tr_neut_nc2[i] = -999;
   tr_cf2[i] = -999;
   tr_oang[i] = -999;
   tr_iter[i] = -999;

  }//i

  return;

}//ResetTrueJetVars

void MakeJets::ResetRestrictedTruthJetVars(){

  nRestrictedTruthJets = 0;

  for(int i = 0; i < kMaxRestrictedTruthJets; i++){

   rtr_pT[i] = -999;
   rtr_eta[i] = -999;
   rtr_phi[i] = -999;
   rtr_nc[i] = -999;
   rtr_chgd_nc[i] = -999;
   rtr_neut_nc[i] = -999;
   rtr_cf[i] = -999;
   rtr_tphi[i] = -999;
   rtr_nphi[i] = -999; 
   rtr_teta[i] = -999;
   rtr_neta[i] = -999; 

   //substructure info
   rtr_z_g[i] = -999;
   rtr_pT1[i] = -999;
   rtr_eta1[i] = -999;  
   rtr_phi1[i] = -999;
   rtr_nc1[i] = -999;
   rtr_chgd_nc1[i] = -999;
   rtr_neut_nc1[i] = -999;
   rtr_cf1[i] = -999;
   rtr_pT2[i] = -999;
   rtr_eta2[i] = -999;
   rtr_phi2[i] = -999;
   rtr_nc2[i] = -999;
   rtr_chgd_nc2[i] = -999;
   rtr_neut_nc2[i] = -999;
   rtr_cf2[i] = -999;
   rtr_oang[i] = -999;
   rtr_iter[i] = -999;

  }//i

  return;

}//ResetRestrictedTrueJetVars

void MakeJets::ResetMatchedJetVars(){

  nMatchedJets = 0;

  for(int i = 0; i < kMaxMatchedJets; i++){

   m_r_pT[i] = -999;
   m_r_eta[i] = -999;
   m_r_phi[i] = -999;
   m_r_nc[i] = -999;
   m_r_chgd_nc[i] = -999;
   m_r_neut_nc[i] = -999;
   m_r_cf[i] = -999;
   m_r_ml_pT[i] = -999;
   m_r_tphi[i] = 999; 
   m_r_nphi[i] = 999; 
   m_r_teta[i] = 999; 
   m_r_neta[i] = 999; 
   
   m_r_ertFired[i] = false; 

   m_t_pT[i] = -999;
   m_t_eta[i] = -999;
   m_t_phi[i] = -999;
   m_t_nc[i] = -999;
   m_t_chgd_nc[i] = -999;
   m_t_neut_nc[i] = -999;
   m_t_cf[i] = -999;

  //substructure info
   m_r_z_g[i] = -999;
   m_r_pT1[i] = -999;
   m_r_eta1[i] = -999;  
   m_r_phi1[i] = -999;
   m_r_nc1[i] = -999;
   m_r_chgd_nc1[i] = -999;
   m_r_neut_nc1[i] = -999;
   m_r_cf1[i] = -999;
   m_r_pT2[i] = -999;
   m_r_eta2[i] = -999;
   m_r_phi2[i] = -999;
   m_r_nc2[i] = -999;
   m_r_chgd_nc2[i] = -999;
   m_r_neut_nc2[i] = -999;
   m_r_cf2[i] = -999;
   m_r_oang[i] = -999;
   m_r_iter[i] = -999;

   m_t_z_g[i] = -999;
   m_t_pT1[i] = -999;
   m_t_eta1[i] = -999;  
   m_t_phi1[i] = -999;
   m_t_nc1[i] = -999;
   m_t_pT2[i] = -999;
   m_t_eta2[i] = -999;
   m_t_phi2[i] = -999;
   m_t_nc2[i] = -999;
   m_t_oang[i] = -999;
   m_t_iter[i] = -999;

   m_dphi[i] = -999; 
   m_deta[i] = -999;
   m_dR[i] = -999; 

  }//i

  return;

}//ResetMatchedJetVars

void MakeJets::ResetRestrictedMatchedJetVars(){

  nRestrictedMatchedJets = 0;

  for(int i = 0; i < kMaxRestrictedMatchedJets; i++){

   rm_r_pT[i] = -999;
   rm_r_eta[i] = -999;
   rm_r_phi[i] = -999;
   rm_r_nc[i] = -999;
   rm_r_chgd_nc[i] = -999;
   rm_r_neut_nc[i] = -999;
   rm_r_cf[i] = -999;
   rm_r_ml_pT[i] = -999;
   rm_r_tphi[i] = 999; 
   rm_r_nphi[i] = 999; 
   rm_r_teta[i] = 999; 
   rm_r_neta[i] = 999; 

   rm_r_ertFired[i] = false; 

   rm_t_pT[i] = -999;
   rm_t_eta[i] = -999;
   rm_t_phi[i] = -999;
   rm_t_nc[i] = -999;
   rm_t_chgd_nc[i] = -999;
   rm_t_neut_nc[i] = -999;
   rm_t_cf[i] = -999;
   rm_t_tphi[i] = 999; 
   rm_t_nphi[i] = 999; 
   rm_t_teta[i] = 999; 
   rm_t_neta[i] = 999; 

  //substructure info
   rm_r_z_g[i] = -999;
   rm_r_pT1[i] = -999;
   rm_r_eta1[i] = -999;  
   rm_r_phi1[i] = -999;
   rm_r_nc1[i] = -999;
   rm_r_chgd_nc1[i] = -999;
   rm_r_neut_nc1[i] = -999;
   rm_r_cf1[i] = -999;
   rm_r_pT2[i] = -999;
   rm_r_eta2[i] = -999;
   rm_r_phi2[i] = -999;
   rm_r_nc2[i] = -999;
   rm_r_chgd_nc2[i] = -999;
   rm_r_neut_nc2[i] = -999;
   rm_r_cf2[i] = -999;
   rm_r_oang[i] = -999;
   rm_r_iter[i] = -999;

   rm_t_z_g[i] = -999;
   rm_t_pT1[i] = -999;
   rm_t_eta1[i] = -999;  
   rm_t_phi1[i] = -999;
   rm_t_nc1[i] = -999;
   rm_t_chgd_nc1[i] = -999;
   rm_t_neut_nc1[i] = -999;
   rm_t_cf1[i] = -999;
   rm_t_pT2[i] = -999;
   rm_t_eta2[i] = -999;
   rm_t_phi2[i] = -999;
   rm_t_nc2[i] = -999;
   rm_t_chgd_nc2[i] = -999;
   rm_t_neut_nc2[i] = -999;
   rm_t_cf2[i] = -999;
   rm_t_oang[i] = -999;
   rm_t_iter[i] = -999;

   rm_dphi[i] = -999; 
   rm_deta[i] = -999;
   rm_dR[i] = -999; 

  }//i

  return;

}//ResetRestrictedMatchedJetVars

void MakeJets::ResetTruthRestrictedMatchedJetVars(){

  nTruthRestrictedMatchedJets = 0;

  for(int i = 0; i < kMaxTruthRestrictedMatchedJets; i++){

   trm_r_pT[i] = -999;
   trm_r_eta[i] = -999;
   trm_r_phi[i] = -999;
   trm_r_nc[i] = -999;
   trm_r_chgd_nc[i] = -999;
   trm_r_neut_nc[i] = -999;
   trm_r_cf[i] = -999;
   trm_r_tphi[i] = 999; 
   trm_r_nphi[i] = 999; 
   trm_r_teta[i] = 999; 
   trm_r_neta[i] = 999; 

   trm_t_pT[i] = -999;
   trm_t_eta[i] = -999;
   trm_t_phi[i] = -999;
   trm_t_nc[i] = -999;
   trm_t_chgd_nc[i] = -999;
   trm_t_neut_nc[i] = -999;
   trm_t_cf[i] = -999;

  //substructure info
   trm_r_z_g[i] = -999;
   trm_r_pT1[i] = -999;
   trm_r_eta1[i] = -999;  
   trm_r_phi1[i] = -999;
   trm_r_nc1[i] = -999;
   trm_r_chgd_nc1[i] = -999;
   trm_r_neut_nc1[i] = -999;
   trm_r_cf1[i] = -999;
   trm_r_pT2[i] = -999;
   trm_r_eta2[i] = -999;
   trm_r_phi2[i] = -999;
   trm_r_nc2[i] = -999;
   trm_r_chgd_nc2[i] = -999;
   trm_r_neut_nc2[i] = -999;
   trm_r_cf2[i] = -999;
   trm_r_oang[i] = -999;
   trm_r_iter[i] = -999;

   trm_t_z_g[i] = -999;
   trm_t_pT1[i] = -999;
   trm_t_eta1[i] = -999;  
   trm_t_phi1[i] = -999;
   trm_t_nc1[i] = -999;
   trm_t_chgd_nc1[i] = -999;
   trm_t_neut_nc1[i] = -999;
   trm_t_cf1[i] = -999;
   trm_t_pT2[i] = -999;
   trm_t_eta2[i] = -999;
   trm_t_phi2[i] = -999;
   trm_t_nc2[i] = -999;
   trm_t_chgd_nc2[i] = -999;
   trm_t_neut_nc2[i] = -999;
   trm_t_cf2[i] = -999;
   trm_t_oang[i] = -999;
   trm_t_iter[i] = -999;

   trm_dphi[i] = -999; 
   trm_deta[i] = -999;
   trm_dR[i] = -999; 

  }//i

  return;

}//ResetTruthRestrictedMatchedJetVars


void MakeJets::SetBranchAddress(){
  cout  <<  "Setting branch address"  <<  endl;

  tEvent        ->  SetMakeClass(1);
  tTracks       ->  SetMakeClass(1);  
  tClusters     ->  SetMakeClass(1);

  if(writeHadron || writeParton){
    tPythiaEvent  ->  SetMakeClass(1);  
  }
  if(writeHadron){
    tPythiaHadron ->  SetMakeClass(1);

    tPythiaHadron ->  SetBranchAddress("tPythiaHadron",&tPythiaHadron_,&b_tPythiaHadron_); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.charge",tPythiaHadron_charge,&b_tPythiaHadron_charge); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.energy",tPythiaHadron_energy,&b_tPythiaHadron_energy); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.mom",tPythiaHadron_mom,&b_tPythiaHadron_mom); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.pT",tPythiaHadron_pT,&b_tPythiaHadron_pT); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.px",tPythiaHadron_px,&b_tPythiaHadron_px); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.py",tPythiaHadron_py,&b_tPythiaHadron_py); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.pz",tPythiaHadron_pz,&b_tPythiaHadron_pz); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.eta",tPythiaHadron_eta,&b_tPythiaHadron_eta); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.phi",tPythiaHadron_phi,&b_tPythiaHadron_phi); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.kf",tPythiaHadron_kf,&b_tPythiaHadron_kf); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.isNeutrino",tPythiaHadron_isNeutrino,&b_tPythiaHadron_isNeutrino); 
    tPythiaHadron ->  SetBranchAddress("tPythiaHadron.isMuon",tPythiaHadron_isMuon,&b_tPythiaHadron_isMuon); 
  }

  if(writeParton){
    tPythiaParton ->  SetMakeClass(1);

    tPythiaParton ->  SetBranchAddress("tPythiaParton",&tPythiaParton_,&b_tPythiaParton_); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.charge",tPythiaParton_charge,&b_tPythiaParton_charge); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.energy",tPythiaParton_energy,&b_tPythiaParton_energy); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.mom",tPythiaParton_mom,&b_tPythiaParton_mom); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.pT",tPythiaParton_pT,&b_tPythiaParton_pT); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.px",tPythiaParton_px,&b_tPythiaParton_px); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.py",tPythiaParton_py,&b_tPythiaParton_py); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.pz",tPythiaParton_pz,&b_tPythiaParton_pz); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.eta",tPythiaParton_eta,&b_tPythiaParton_eta); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.phi",tPythiaParton_phi,&b_tPythiaParton_phi); 
    tPythiaParton ->  SetBranchAddress("tPythiaParton.kf",tPythiaParton_kf,&b_tPythiaParton_kf); 
  }

  tTracks->SetBranchAddress("tTracks", &tTracks_, &b_tTracks_);
  tTracks->SetBranchAddress("tTracks.runnumber", tTracks_runnumber, &b_tTracks_runnumber);
  tTracks->SetBranchAddress("tTracks.eventnumber", tTracks_eventnumber, &b_tTracks_eventnumber);
  tTracks->SetBranchAddress("tTracks.mom", tTracks_mom, &b_tTracks_mom);
  tTracks->SetBranchAddress("tTracks.theta", tTracks_theta, &b_tTracks_theta);
  tTracks->SetBranchAddress("tTracks.pT", tTracks_pT, &b_tTracks_pT);
  tTracks->SetBranchAddress("tTracks.eta", tTracks_eta, &b_tTracks_eta);
  tTracks->SetBranchAddress("tTracks.phi", tTracks_phi, &b_tTracks_phi);
  tTracks->SetBranchAddress("tTracks.phiDC", tTracks_phiDC, &b_tTracks_phiDC);
  tTracks->SetBranchAddress("tTracks.zedDC", tTracks_zedDC, &b_tTracks_zedDC);
  tTracks->SetBranchAddress("tTracks.alpha", tTracks_alpha, &b_tTracks_alpha);
  tTracks->SetBranchAddress("tTracks.energy", tTracks_energy, &b_tTracks_energy);
  tTracks->SetBranchAddress("tTracks.board", tTracks_board, &b_tTracks_board);

  tTracks->SetBranchAddress("tTracks.charge", tTracks_charge, &b_tTracks_charge);
  tTracks->SetBranchAddress("tTracks.quality", tTracks_quality, &b_tTracks_quality);
  tTracks->SetBranchAddress("tTracks.n0", tTracks_n0, &b_tTracks_n0);
  tTracks->SetBranchAddress("tTracks.arm", tTracks_arm, &b_tTracks_arm);
  tTracks->SetBranchAddress("tTracks.armsect", tTracks_armsect, &b_tTracks_armsect);
  tTracks->SetBranchAddress("tTracks.emcid", tTracks_emcid, &b_tTracks_emcid);

  tTracks->SetBranchAddress("tTracks.px", tTracks_px, &b_tTracks_px);
  tTracks->SetBranchAddress("tTracks.py", tTracks_py, &b_tTracks_py);
  tTracks->SetBranchAddress("tTracks.pz", tTracks_pz, &b_tTracks_pz);
  tTracks->SetBranchAddress("tTracks.eT", tTracks_eT, &b_tTracks_eT);

  tTracks->SetBranchAddress("tTracks.pc3dphi", tTracks_pc3dphi, &b_tTracks_pc3dphi);
  tTracks->SetBranchAddress("tTracks.pc3dz", tTracks_pc3dz, &b_tTracks_pc3dz);
  tTracks->SetBranchAddress("tTracks.pc3sdphi", tTracks_pc3sdphi, &b_tTracks_pc3sdphi);
  tTracks->SetBranchAddress("tTracks.pc3sdz", tTracks_pc3sdz, &b_tTracks_pc3sdz);
  tTracks->SetBranchAddress("tTracks.emcdphi", tTracks_emcdphi, &b_tTracks_emcdphi);
  tTracks->SetBranchAddress("tTracks.emcdz", tTracks_emcdz, &b_tTracks_emcdz);
  tTracks->SetBranchAddress("tTracks.emcsdphi", tTracks_emcsdphi, &b_tTracks_emcsdphi);
  tTracks->SetBranchAddress("tTracks.emcsdz", tTracks_emcsdz, &b_tTracks_emcsdz);

  tTracks->SetBranchAddress("tTracks.assClusToF", tTracks_assClusToF, &b_tTracks_assClusToF);
  tTracks->SetBranchAddress("tTracks.assClusEnergy", tTracks_assClusEnergy, &b_tTracks_assClusEnergy);
  tTracks->SetBranchAddress("tTracks.assClusERTfired", tTracks_assClusERTfired, &b_tTracks_assClusERTfired);

  tTracks->SetBranchAddress("tTracks.inBrokenX1", tTracks_inBrokenX1, &b_tTracks_inBrokenX1);
  tTracks->SetBranchAddress("tTracks.inBrokenX2", tTracks_inBrokenX2, &b_tTracks_inBrokenX2);
  tTracks->SetBranchAddress("tTracks.inBrokenUV", tTracks_inBrokenUV, &b_tTracks_inBrokenUV);
  tTracks->SetBranchAddress("tTracks.x1Used", tTracks_x1Used, &b_tTracks_x1Used);
  tTracks->SetBranchAddress("tTracks.x2Used", tTracks_x2Used, &b_tTracks_x2Used);
  tTracks->SetBranchAddress("tTracks.uvUsed", tTracks_uvUsed, &b_tTracks_uvUsed);

  tTracks->SetBranchAddress("tTracks.passDC", tTracks_passDC, &b_tTracks_passDC);
  tTracks->SetBranchAddress("tTracks.passQuality", tTracks_passQuality, &b_tTracks_passQuality);
  tTracks->SetBranchAddress("tTracks.passMatching", tTracks_passMatching, &b_tTracks_passMatching);
  tTracks->SetBranchAddress("tTracks.passEMCMatching", tTracks_passEMCMatching, &b_tTracks_passEMCMatching);
  tTracks->SetBranchAddress("tTracks.passPC3Matching", tTracks_passPC3Matching, &b_tTracks_passPC3Matching);
  tTracks->SetBranchAddress("tTracks.passMinPt", tTracks_passMinPt, &b_tTracks_passMinPt);
  tTracks->SetBranchAddress("tTracks.passGhost", tTracks_passGhost, &b_tTracks_passGhost);
  tTracks->SetBranchAddress("tTracks.passDcConversion", tTracks_passDcConversion, &b_tTracks_passDcConversion);
  tTracks->SetBranchAddress("tTracks.passConversionEdge", tTracks_passConversionEdge, &b_tTracks_passConversionEdge);
  tTracks->SetBranchAddress("tTracks.passConversionElectron", tTracks_passConversionElectron, &b_tTracks_passConversionElectron);

  //for clusters
  tClusters->SetBranchAddress("tClusters", &tClusters_, &b_tClusters_);
  tClusters->SetBranchAddress("tClusters.arm", tClusters_arm, &b_tClusters_arm);
  tClusters->SetBranchAddress("tClusters.sector", tClusters_sector, &b_tClusters_sector);
  tClusters->SetBranchAddress("tClusters.armsect", tClusters_armsect, &b_tClusters_armsect);
  tClusters->SetBranchAddress("tClusters.emcid", tClusters_emcid, &b_tClusters_emcid);
  tClusters->SetBranchAddress("tClusters.yTowerPos", tClusters_yTowerPos, &b_tClusters_yTowerPos);
  tClusters->SetBranchAddress("tClusters.zTowerPos", tClusters_zTowerPos, &b_tClusters_zTowerPos);
  tClusters->SetBranchAddress("tClusters.towerId", tClusters_towerId, &b_tClusters_towerId);

  tClusters->SetBranchAddress("tClusters.ertFired", tClusters_ertFired, &b_tClusters_ertFired);

  tClusters->SetBranchAddress("tClusters.runnumber", tClusters_runnumber, &b_tClusters_runnumber);
  tClusters->SetBranchAddress("tClusters.eventnumber", tClusters_eventnumber, &b_tClusters_eventnumber);
  tClusters->SetBranchAddress("tClusters.energy", tClusters_energy, &b_tClusters_energy);
  tClusters->SetBranchAddress("tClusters.theta", tClusters_theta, &b_tClusters_theta);
  tClusters->SetBranchAddress("tClusters.pT", tClusters_pT, &b_tClusters_pT);
  tClusters->SetBranchAddress("tClusters.eT", tClusters_eT, &b_tClusters_eT);
  tClusters->SetBranchAddress("tClusters.phi", tClusters_phi, &b_tClusters_phi);
  tClusters->SetBranchAddress("tClusters.px", tClusters_px, &b_tClusters_px);
  tClusters->SetBranchAddress("tClusters.py", tClusters_py, &b_tClusters_py);
  tClusters->SetBranchAddress("tClusters.pz", tClusters_pz, &b_tClusters_pz);
  tClusters->SetBranchAddress("tClusters.eta", tClusters_eta, &b_tClusters_eta);
  tClusters->SetBranchAddress("tClusters.chi2", tClusters_chi2, &b_tClusters_chi2);
  tClusters->SetBranchAddress("tClusters.prob", tClusters_prob, &b_tClusters_prob);
  tClusters->SetBranchAddress("tClusters.tof", tClusters_tof, &b_tClusters_tof);
  tClusters->SetBranchAddress("tClusters.emctrkdphi", tClusters_emctrkdphi, &b_tClusters_emctrkdphi);
  tClusters->SetBranchAddress("tClusters.emctrkdz", tClusters_emctrkdz, &b_tClusters_emctrkdz);
  
  tClusters->SetBranchAddress("tClusters.passNotBad", tClusters_passNotBad, &b_tClusters_passNotBad);
  tClusters->SetBranchAddress("tClusters.passEnergy", tClusters_passEnergy, &b_tClusters_passEnergy);
  tClusters->SetBranchAddress("tClusters.passIsNeutral", tClusters_passIsNeutral, &b_tClusters_passIsNeutral);
  tClusters->SetBranchAddress("tClusters.passToF", tClusters_passToF, &b_tClusters_passToF);

  //for tEvent
  //Notice how leaves are directly linked to tree like the jet trees
  tEvent->SetBranchAddress("runNumber", &tEvent_runNumber);
  tEvent->SetBranchAddress("eventNumber", &tEvent_eventNumber);
  tEvent->SetBranchAddress("fillNumber", &tEvent_fillNumber);
  tEvent->SetBranchAddress("level1_clock_cross", &tEvent_level1_clock_cross);
  tEvent->SetBranchAddress("ip12_clock_cross", &tEvent_ip12_clock_cross);
  tEvent->SetBranchAddress("vertex", &tEvent_vertex);

  //for tPythiaEvent
  //Notice how leaves are directly linked to tree like the jet trees
  if(writeHadron || writeParton){
    tPythiaEvent->SetBranchAddress("event_x1", &tPythiaEvent_x1);
    tPythiaEvent->SetBranchAddress("event_x2", &tPythiaEvent_x2);
    tPythiaEvent->SetBranchAddress("event_Qsqr", &tPythiaEvent_Qsqr);
    tPythiaEvent->SetBranchAddress("event_processid", &tPythiaEvent_processid);
  }

  return;
}//end SetBranchAddress

int MakeJets::GetFakeJets(fastjet::JetDefinition *fa_antikt){

  if( (tTracks_==0) && (tClusters_==0) ) {
    // Make sure there is one entry for every event, empty or not
    ResetFakeJetVars();
    fJets->Fill();
    return 0; 
  }

  std::vector<int> chIndexEast; 
  std::vector<int> chIndexWest;
  std::vector<int> nIndexEast; 
  std::vector<int> nIndexWest;
  chIndexEast.clear(); 
  chIndexWest.clear(); 
  nIndexEast.clear(); 
  nIndexWest.clear(); 
  std::vector<int> chSwaps;
  chSwaps.resize(tTracks_);  
  std::vector<int> nSwaps;
  nSwaps.resize(tClusters_); 
    
  for(int i = 0; i < tTracks_; i++){
    chSwaps[i] = i; 
    if(tTracks_arm[i] == 0){
      chIndexEast.push_back(i);
    }
    else
      chIndexWest.push_back(i);  
  }
  for(int j = 0; j < tClusters_; j++){
    nSwaps[j] = j; 
    if(tClusters_arm[j] == 0)
      nIndexEast.push_back(j);
    else
      nIndexWest.push_back(j);  
  }

  // Randomly swap tracks and clusters 
  // If a track is associated with a cluster swap it with the track! 

  // Use a different seed value so that we don't get same result each time
  srand(time(NULL));
    
  //Start from the last element- don't need to run first element- hence i>0
  if(chIndexEast.size()>1){
    for (unsigned int i = chIndexEast.size() - 1; i > 0; i--)
      {
	// Pick a random index from 0 to i
	unsigned int indexRandomEast = rand() % (i + 1);
      
	// Only swap *good* tracks
	if(!TrackCuts(chIndexEast[i]) || !TrackCuts(chIndexEast[indexRandomEast])) continue; 
	
	chSwaps[chIndexEast[i]] = chIndexEast[indexRandomEast]; 
	chSwaps[chIndexEast[indexRandomEast]] = chIndexEast[i]; 
	
      }
  }

  if(chIndexWest.size()>1){
    for (unsigned int i = chIndexWest.size()  - 1; i > 0; i--)
      {
	// Pick a random index from 0 to i
	unsigned int indexRandomWest = rand() % (i + 1);

	// Only swap *good* tracks
	if(!TrackCuts(chIndexWest[i]) || !TrackCuts(chIndexWest[indexRandomWest])) continue; 
	
	chSwaps[chIndexWest[i]] = chIndexWest[indexRandomWest]; 
	chSwaps[chIndexWest[indexRandomWest]] = chIndexWest[i]; 

      }
  }

  if(nIndexEast.size()>1){
    for (unsigned int i = nIndexEast.size() - 1; i > 0; i--)
      {
	// Pick a random index from 0 to i
	unsigned int indexRandomEast = rand() % (i + 1);

	// Only swap *good* clusters - must include passIsNeutral
	// to avoid swapping clusters associated with hadrons
	if(!ClusterCuts(nIndexEast[i]) || !ClusterCuts(nIndexEast[indexRandomEast])) continue; 

	nSwaps[nIndexEast[i]] = nIndexEast[indexRandomEast]; 
	nSwaps[nIndexEast[indexRandomEast]] = nIndexEast[i]; 

      }
  }

  if(nIndexWest.size()>1){
    for (unsigned int i = nIndexWest.size() - 1; i > 0; i--)
      {
	// Pick a random index from 0 to i
	unsigned int indexRandomWest = rand() % (i + 1);

	// Only swap *good* clusters - must include passIsNeutral
	// to avoid swapping clusters associated with hadrons
	if(!ClusterCuts(nIndexWest[i]) || !ClusterCuts(nIndexWest[indexRandomWest])) continue; 

	nSwaps[nIndexWest[i]] = nIndexWest[indexRandomWest]; 
	nSwaps[nIndexWest[indexRandomWest]] = nIndexWest[i]; 

      }
  }

  
  /////////////         make fake jets     ////////////////////////
    
  std::vector<fastjet::PseudoJet> jetParticles_all;
  jetParticles_all.clear();

  const int indexTotal = 2*tTracks_ + tClusters_;
  float particlePx[indexTotal];
  float particlePy[indexTotal];
  float particlePz[indexTotal];
  float ertFired[indexTotal]; 
  
  int index = 0;

  for (int h = 0; h < tTracks_; h++)//loop over tracks
    { 
      // TRACK CUTS
      if(!TrackCuts(h)) continue; 

      //calculate the swapped eta and phi quantities
      TLorentzVector *fVector = new TLorentzVector();
      int swapped = chSwaps[h];  

      fVector->SetPtEtaPhiE(tTracks_mom[h] * sin(2 * atan(exp(fabs(tTracks_eta[swapped])))),
			    tTracks_eta[swapped],
			    tTracks_phi[swapped],
			    tTracks_mom[h]);
 
      fastjet::PseudoJet pseudoCharged(fVector->Px(),
				       fVector->Py(),
				       fVector->Pz(),
				       fVector->P()); 
      
      pseudoCharged.set_user_index(index);

      particlePx[index] = fVector->Px();
      particlePy[index] = fVector->Py();
      particlePz[index] = fVector->Pz();
      
      ertFired[index] = tTracks_assClusERTfired[h]; 

      jetParticles_all.push_back(pseudoCharged);

      delete fVector; 

      index++;

    }//end h (loop over tracks)


  unsigned int indexCharged = index; 
    
  for (int n = 0; n < tClusters_; n++)//loop over clusters
    {

      if(!ClusterCuts(n)) continue; 

      TLorentzVector *fVector = new TLorentzVector();
      int swapped = nSwaps[n];  

      fVector->SetPtEtaPhiE(tClusters_energy[n] * sin(2 * atan(exp(fabs(tClusters_eta[swapped])))),
			    tClusters_eta[swapped],
			    tClusters_phi[swapped],
			    tClusters_energy[n]);

      fastjet::PseudoJet pseudoNeutral(fVector->Px(),
				       fVector->Py(),
				       fVector->Pz(),
				       fVector->E());

      pseudoNeutral.set_user_index(index);

      particlePx[index] = fVector->Px();
      particlePy[index] = fVector->Py();
      particlePz[index] = fVector->Pz();
      
      ertFired[index] = tClusters_ertFired[n]; 

      jetParticles_all.push_back(pseudoNeutral);

      delete fVector; 

      index++;

    }//end n: loop over clusters
  
    // Loop over all tracks that have associated EMCal clusters
    // If the cluster energy is less than the track momentum, subtract
    // the track momentum from the cluster energy and enter the remainder 
    // as a neutral cluster.  This attempts to recapture some 
    // neutral energy that would otherwise get vetoed by tracks. 

    for (int h = 0; h < tTracks_; h++)//loop over tracks
      { 

	if(!TrackCuts(h)) continue; 
	if(!tTracks_passEMCMatching[h]) continue;
  
	// track passes all cuts, find assoc. cluster
	int n_assoc = -1; 
	for (int n = 0; n < tClusters_; n++)//loop over clusters
	  {
	    if(tTracks_emcid[h]==tClusters_emcid[n]){
	      n_assoc = n; 
	      break; 
	    }
	  }
	if(n_assoc<0) continue; 

	// Swapping keeps the momentum and energy the same and just 
	// rescales the pT, so this is valid: 

	if((tClusters_energy[n_assoc]-tTracks_mom[h])>RECOVERED_CLUSTER_MIN_E){

	  float scale = (tClusters_energy[n_assoc]-tTracks_mom[h])/
	                tClusters_energy[n_assoc]; 

	  // To get the kinematics correct we need to consider the swapped variables

	  TLorentzVector *fVector = new TLorentzVector();
	  int swapped = chSwaps[h];  

	  // Clusters associated with hadrons were not swapped, so 
	  // we use the track kinematics to add in the remaining energy

	  fVector->SetPtEtaPhiE(scale*tClusters_energy[n_assoc] * sin(2 * atan(exp(fabs(tTracks_eta[swapped])))),
				tTracks_eta[swapped],
				tTracks_phi[swapped],
				scale*tClusters_energy[n_assoc]);

	  fastjet::PseudoJet pseudoNeutral(fVector->Px(),
					   fVector->Py(),
					   fVector->Pz(),
					   fVector->E());

	  pseudoNeutral.set_user_index(index);

	  particlePx[index] = fVector->Px();
	  particlePy[index] = fVector->Py();
	  particlePz[index] = fVector->Pz();
      
	  ertFired[index] = false; // filled above

	  jetParticles_all.push_back(pseudoNeutral);

	  delete fVector; 

	  index++; 

	}
 
      }//end h (loop over tracks)

  
  //done with making particles, make jets
  fastjet::ClusterSequence jetAll(jetParticles_all, *fa_antikt);
  std::vector<fastjet::PseudoJet> fastAll = jetAll.inclusive_jets(minPt_jets);//these are jets
  
  ResetFakeJetVars();

  unsigned int fsize = fastAll.size();
  for (unsigned int n = 0; n < fsize; n++) {

    fastjet::PseudoJet aFastJet = fastAll[n];

    float chargedPt = 0.0;
    float chargedPx = 0.0;
    float chargedPy = 0.0;
    float chargedPz = 0.0;
    float neutralPt = 0.0;
    float neutralPx = 0.0;
    float neutralPy = 0.0;
    float neutralPz = 0.0;
    float pT        = aFastJet.perp();
    float eta       = aFastJet.pseudorapidity();
    float phi       = phiReduce( aFastJet.phi() );
	    
    vector<fastjet::PseudoJet> constituents = jetAll.constituents(aFastJet);
    unsigned int nc = constituents.size();
	   
    bool ertJet = false; 
    unsigned int chgd_nc = 0; 
    unsigned int neut_nc = 0; 
    for (unsigned int iconst = 0; iconst < nc; iconst++) {

      unsigned int constituent_index = constituents[iconst].user_index(); 
	
      if (constituent_index < indexCharged){//Charged particles
	chargedPx += particlePx[constituent_index]; 
	chargedPy += particlePy[constituent_index]; 
	chargedPz += particlePz[constituent_index];
	chgd_nc++; 
      }
      else{
	neutralPx += particlePx[constituent_index]; 
	neutralPy += particlePy[constituent_index]; 
	neutralPz += particlePz[constituent_index];
	neut_nc++; 
      }
      if(ertFired[constituent_index]) ertJet = true; 

    }//iconst
      
    chargedPt = sqrt(chargedPx*chargedPx + chargedPy*chargedPy);
    float cf = chargedPt/pT;
    neutralPt = sqrt(neutralPx*neutralPx + neutralPy*neutralPy);
      
    //JET CUTS
    bool passJetLevelCuts = (nc >= NUMCONST_CUT) && 
                            (cf < CFHIGH_CUT) && (cf > CFLOW_CUT) && 
                            (JetEtaAcceptance(eta,re_R,tEvent_vertex));
 
    if(!passJetLevelCuts)continue;
      
    f_pT[nFakeJets]  = pT; 
    f_eta[nFakeJets] = eta; 
    f_phi[nFakeJets] = phi; 
    f_nc[nFakeJets]  = nc;
    f_chgd_nc[nFakeJets]  = chgd_nc;
    f_neut_nc[nFakeJets]  = neut_nc;
    f_cf[nFakeJets]  = cf; 
    f_ertFired[nFakeJets] = ertJet; 

    if(chargedPt>0.0)
      f_tphi[nFakeJets] = phiReduce(atan2(chargedPy,chargedPx));
    else
      f_tphi[nFakeJets] = -9999.0; 

    if(neutralPt>0.0)
      f_nphi[nFakeJets] = phiReduce(atan2(neutralPy,neutralPx));
    else
      f_nphi[nFakeJets] = 0.0; 

    if(chargedPt==0.0) 
      f_teta[nFakeJets] = -9999.0;
    else if(chargedPz==0.0)
      f_teta[nFakeJets] = 0.0; 
    else
      f_teta[nFakeJets] = -log(tan(atan2(chargedPt,chargedPz)/2.0)); 

    if(neutralPt==0.0)
      f_neta[nFakeJets] = -9999.0;
    else if(chargedPz==0.0)
      f_neta[nFakeJets] = 0.0; 
    else
      f_neta[nRecoJets] = -log(tan(atan2(neutralPt,neutralPz)/2.0)); 

    float det_eta = GetDeta(f_eta[nFakeJets]); 

    f_ml_pT[nFakeJets] = GetMachineLearningTransverseMomentum(f_pT[nFakeJets], f_cf[nFakeJets], f_phi[nFakeJets], det_eta, 
								 f_nc[nFakeJets], f_tphi[nFakeJets]-f_nphi[nFakeJets], 
								 f_teta[nFakeJets]-f_neta[nFakeJets]); 
    if(nFakeJets<kMaxFakeJets){
      nFakeJets++;
    }
    else{
      cout << "ERROR : >kMaxFakeJets, results truncated" << endl; 
    }
 
  }//end n: loop over jets made from tracks and clusters
 
  fJets->Fill();

  return nFakeJets;

}//getfakejets


void MakeJets::ResetFakeJetVars(){

  nFakeJets = 0;

  for(int i = 0; i < kMaxFakeJets; i++){

   f_pT[i] = -999;
   f_eta[i] = -999;
   f_phi[i] = -999;
   f_nc[i] = -999;
   f_chgd_nc[i] = -999;
   f_neut_nc[i] = -999;
   f_cf[i] = -999;
   f_ml_pT[i] = -999;
   f_ertFired[i] = false; 
 
  }//i

  return;

}//ResetFakeJetVars

void MakeJets::InitMachineLearning()
{

  // Machine learning initialization 

  reader = new TMVA::Reader( "!Color:!Silent" );   
 
  // Add variables
      
  reader->AddVariable( "r_pT", &ml_r_pT );
  reader->AddVariable( "r_cf", &ml_r_cf );
  reader->AddVariable( "r_phi", &ml_r_phi );
  reader->AddVariable( "det_eta", &ml_det_eta );
  reader->AddVariable( "r_nc", &ml_r_nc );
  reader->AddVariable( "r_tndphi", &ml_r_tndphi );
  reader->AddVariable( "r_tndeta", &ml_r_tndeta );

  unsigned char *xmlstr = NULL; 
  
  switch(ml_run){

  case 12: 
    xmlstr = TMVARegression_MLP_weights_Run12_xml; 
    break; 
  case 13: 
    xmlstr = TMVARegression_MLP_weights_Run13_xml; 
    break; 
  default:
    cout << "ERROR - unrecognized run for MLP configuration!" << endl; 
    
  }
   
  // Read training data
  
  if(xmlstr){
    method = dynamic_cast<TMVA::MethodBase*>(reader->BookMVA(TMVA::Types::kMLP, (char *)xmlstr )); 
  }
  else{
    method = NULL;
  }

  return; 
}

float MakeJets::GetMachineLearningTransverseMomentum(float r_pT, float r_cf, float r_phi, float det_eta, 
						     int r_nc, float r_tndphi, float r_tndeta)
{

  // Machine learning routine to go from 
  // reco to "perfect detector" transverse momentum. 

  ml_r_pT = r_pT; 
  ml_r_cf = r_cf; 
  ml_r_phi = r_phi; 
  ml_det_eta = det_eta; 
  ml_r_nc = (float)r_nc; 
  ml_r_tndphi = r_tndphi; 
  ml_r_tndeta = r_tndeta; 

  if (method)
    return (r_pT/(reader->EvaluateRegression( method ))[0]); 
  else
    return -9999.0; 

}
