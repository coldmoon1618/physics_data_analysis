//YZhai debug version

//General PHENIX tools
#include <getClass.h>
#include <PHCompositeNode.h>
#include <phool.h>
#include <RunHeader.h>

//Fun4All tools
#include <Fun4AllServer.h>
#include <Fun4AllHistoManager.h>
#include <Fun4AllReturnCodes.h>

//FastJet tools
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>

//My source file
#include "ISUJetTrkClus.h"

//Data classes I am using in analysis
#include <PHCentralTrack.h>
#include <PHGlobal.h>
#include <EventHeader.h>
#include <PreviousEvent.h>
#include <emcClusterContainer.h>
#include <emcClusterContent.h>

//spin stuff
#include <SpinDBOutput.hh>
#include <SpinDBContent.hh>


//Cpp tools
#include <iomanip>

//Root tools
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TH2.h>
#include <TCut.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TRandom.h>

//For Trigger
#include <TrigLvl1.h>
#include <ErtOut.h>
#include <TriggerHelper.h>

//good run list
#include "GoodRunList.h"

//Used for EMCal warnmap
#include "Run13Calibs/EmcMapRun13.h"
#include "Run12Calibs/EmcMapRun12.h"

//for getting sigmalized variables
#include "Run13Calibs/EmcMatchingRun13.h"
#include "Run13Calibs/PC3MatchingRun13.h"
#include "Run12Calibs/EmcMatchingRun12.h"
#include "Run12Calibs/Pc3MatchingRun12.h"
#include "Run15pAuCalibs/EmcMatchingRun15pAu.h"
#include "Run15pAuCalibs/PC3MatchingRun15pAu.h"
#include "Run15ppCalibs/EmcMatchingRun15pp.h"

//for simulation calib
#include "SimCalibs/PC3MatchingSim.h"
#include "SimCalibs/EmcMatchingSim.h"

//for correcting the EMC TOF
#include "Run13Calibs/EmcTofCorrRun13.h"


//for DC track quality
#include "Run13Calibs/TrackQualityPPRun13.h"
#include "Run12Calibs/TrackQualityPPRun12.h"
#include "Run12Calibs/TrackQualityCuAuRun12.h"
#include "Run15pAuCalibs/TrackQualityRun15pAu.h"


//for bitmask
#define X1_USED 1
#define X2_USED 2
#define UV_FOUND_UNIQUE 12

using namespace std;
using namespace findNode;
using namespace fastjet;
using namespace EmcMatchingRun13;
using namespace PC3MatchingRun13;
using namespace PC3MatchingSim;
using namespace Pc3MatchingRun12;
using namespace EMCMatchingRun12;
using namespace EmcMatchingRun15pAu;
using namespace PC3MatchingRun15pAu;
using namespace EmcMatchingRun15pp;
using namespace EmcMatchingSim;

SpinDBContent spin_cont;//

//================================ Constructor ================================
//Here we can initiate some variables
ISUJetTrkClus::ISUJetTrkClus(std::string outfilename, int RunNumber, std::string species)
  : SubsysReco("ISUJetTrkClus"),
    verbo(1),
    outfname(outfilename),
    writeTree(true),
    runFlag(RunNumber)
{

  SetRunBools(runFlag);
  SetSpeciesBools(species);

  outfile = new TFile(outfname.c_str(), "RECREATE");

  lvl1trigger_clock_cross = 0;
  fillNumber = 0;
  crossingShift = 0;
  
  InitTrees(writeTree);
 
  phglobal = NULL;
  evtheader = NULL;
  ertOut = NULL;
  lvl1trigger = NULL;
  clus = NULL;
  run_header = NULL;
  triggerHelper = NULL;
  zVertex = 0;
  runNumber = 0;
  eventNumber = 0;
  isMB = false;
  isData = false;
  ertTriggerFired = false;
  skipDeadWarnMap = false; 
  skipDCBrokenWires = false; 
 
//````````````````````````````````````````````` 
  hPC3E_yVSz = new TH2F("hPC3E_yVSz", "East PC3 y Vs PC3 z; after initial track cuts_arm0", 400, -200, 200, 800, -300, 500);
  hPC3E_yVSz->GetXaxis()->SetTitle("Z");
  hPC3E_yVSz->GetYaxis()->SetTitle("Y");

  hPC3W_yVSz = new TH2F("hPC3W_yVSz", "West PC3 y Vs PC3 z; after initial track cuts_arm1", 400, -200, 200, 800, -300, 500);
  hPC3W_yVSz->GetXaxis()->SetTitle("Z");
  hPC3W_yVSz->GetYaxis()->SetTitle("Y");

  hPC1E_yVSz = new TH2F("hPC1E_yVSz", "East PC1 y Vs PC1 z; after initial track cuts_arm0", 200, -100, 100, 400, -150, 250);
  hPC1E_yVSz->GetXaxis()->SetTitle("Z");
  hPC1E_yVSz->GetYaxis()->SetTitle("Y");

  hPC1W_yVSz = new TH2F("hPC1W_yVSz", "West PC1 y Vs PC1 z; after initial track cuts_arm1", 200, -100, 100, 400, -150, 250);
  hPC1W_yVSz->GetXaxis()->SetTitle("Z");
  hPC1W_yVSz->GetYaxis()->SetTitle("Y");

  hModifiedQuality_NE = new TH2F("hModifiedQuality_NE", "Modified Quality Cut, NE",  400, 0, 80, 120, -0.6, 0.6);
  hModifiedQuality_NE->GetXaxis()->SetTitle("board"); 
  hModifiedQuality_NE->GetYaxis()->SetTitle("alpha"); 
  hModifiedQuality_SE = new TH2F("hModifiedQuality_SE", "Modified Quality Cut, SE",  400, 0, 80, 120, -0.6, 0.6);
  hModifiedQuality_SE->GetXaxis()->SetTitle("board"); 
  hModifiedQuality_SE->GetYaxis()->SetTitle("alpha");
  hModifiedQuality_NW = new TH2F("hModifiedQuality_NW", "Modified Quality Cut, NW",  400, 0, 80, 120, -0.6, 0.6);
  hModifiedQuality_NW->GetXaxis()->SetTitle("board"); 
  hModifiedQuality_NW->GetYaxis()->SetTitle("alpha");
  hModifiedQuality_SW = new TH2F("hModifiedQuality_SW", "Modified Quality Cut, SW",  400, 0, 80, 120, -0.6, 0.6);
  hModifiedQuality_SW->GetXaxis()->SetTitle("board"); 
  hModifiedQuality_SW->GetYaxis()->SetTitle("alpha");
//````````````````````````````````````````````` 
  return;
}

int ISUJetTrkClus::Init(PHCompositeNode *topNode)
{
  if (verbo)
    {
      cout << ">>>>>>>>>>>>>>>>>>>  Init called <<<<<<<<<<<<<<<<<<<<<<<" << endl;
    }

  //Load and read the warnmap
  if(!isData){//simulation

    PC3MatchingSim::LoadPointsPC3Sim();
    EmcMatchingSim::LoadEmcMatchingParametersSim();

  }
  if(isRun13){
    
    if(isData){
    
      //load EMC files
      EmcMatchingRun13::LoadAllEMCFilesRun13();
      //load PC3 files
      PC3MatchingRun13::LoadPointsPC3Run13();
    }

    //load EMC warn map
    EmcMapRun13::ReadWarnMap("Run13Calibs/emcWarnMap/warnmap.txt");

    TrackQualityPPRun13::InitTrackQuality();

  }//Run13
  if(isRun12){
    if(isCuAu){

      if(isData){
	Pc3MatchingRun12::pc3_init_fit_pars_II();
	EMCMatchingRun12::LoadEmcMatchingParameters();
      }

      EmcMapRun12::ReadWarnMap("Run12Calibs/warnmapCuAu.txt");

    }//CuAu
    else{//PP

      TrackQualityPPRun12::InitTrackQuality();
      EmcMapRun12::ReadWarnMap("Run12Calibs/warnmapPP.txt");
      TrackQualityPPRun12::InitTrackQuality();
    }

  }//Run12
  
  if(isRun15)
    {
      if(ispAu)
	{
	  TrackQualityRun15pAu::InitTrackQuality();
	  if(isData)
	    {
	      EmcMatchingRun15pAu::LoadEmcMatchingParametersRun15pAu();
	    }
	 
	}
      if(isPP)
	{
	 TrackQualityRun15pAu::InitTrackQuality();
	  if(isData)
	    {
	      EmcMatchingRun15pp::LoadEmcMatchingParametersRun15pp();
	    }
	  
	}
      if(ispAl)
	{
	  TrackQualityRun15pAu::InitTrackQuality();
	  if(isData)
	    {
	      //EmcMatchingRun15pp::LoadEmcMatchingParametersRun15pp();
	    }
	  
	}
    }//Run15
  
  return EVENT_OK;

}//end Init()

bool ISUJetTrkClus::IsRunRange(){
  ///// CHECKING IF THE RUN NUMBER IS WITHIN THE RANGES FOR THE DIFFERENT RUNS ///

  //Run-13 runs
  if( (isRun13) && (isPP)  && (runNumber < 386773 || runNumber > 398149)){
    
    return false;
  }
  //Run-12 p+p 200GeV runs
  else if((isRun12) && (isPP) && (runNumber < 357665 || runNumber > 363228)){
    
    return false;
  }
  else if((isRun12) && (isCuAu) && (runNumber < 372402 || runNumber > 377310) ){

    return false;
  }
  //Run-15 p+Au 200GeV runs
  else if((isRun15) && (ispAu) && (runNumber < 432637 || runNumber > 436647)){

    return false;
  }
  else return true;

}//IsRunRange

bool ISUJetTrkClus::IsGoodRun(int runnumber){

  bool retval = false;

  if(isRun13){

    for(int i = 0; i < 780; i++){
      if(GoodRunList::goodRunArrayRun13[i] == runnumber){
	retval = true;
	break;
      }
    }//end i
    
  }//Run13
  if(isRun12){

    if(isPP) retval = Run12Goodpp(runnumber);

    if(isCuAu) retval = Run12GoodCuAu(runnumber);

  }//Run12

  if(isRun15){
    retval = true;
  }
  return retval;

}//end goodRun()


float ISUJetTrkClus::phiReduce(float phi){
  
  if (phi < -TMath::PiOver2())
    {
      phi += TMath::TwoPi();
    }
  if (phi >= 3.0 * TMath::PiOver2())
    {
      phi -= TMath::TwoPi();
    }

  return phi;
  
}//end phireduce()

float ISUJetTrkClus::dR(float eta1, float eta2, float phi1, float phi2){

  float deta = eta1 - eta2;
  float dphi = phi1 - phi2;

  if(dphi < -TMath::Pi())
    {
      dphi += 2*TMath::Pi();
    }
  if(dphi > TMath::Pi())
    {
      dphi -= 2*TMath::Pi();
    }

  return sqrt(dphi*dphi + deta*deta);

}//end dr()

float ISUJetTrkClus::getDiscriminant(float pT, float dR){

  return (pT*pT*exp(-dR*dR/(2.0*0.1*0.1)));

}


int ISUJetTrkClus::InitRun(PHCompositeNode *topNode)
{
  if (verbo)
    {
      cout << ">>>>>>>>>>>>>>>>>>>> InitRun called <<<<<<<<<<<<<<<<<<<<<<<" << endl;
    }

  run_header = getClass<RunHeader> (topNode, "RunHeader");
  if (!run_header)
    {
      cout << "No RunHeader! No sense continuing" << endl;
      exit(1);
    }

  runNumber = run_header->get_RunNumber();

  triggerHelper = new TriggerHelper(topNode);
  

  cout << "getting spinDBOutput" << endl;
  SpinDBOutput spin_out("phnxrc");
  
  int storedbcont = spin_out.StoreDBContent(runNumber, runNumber);
  if(storedbcont != 1) cout << "didn't get Database content" << endl;

  int getdbcontstore = spin_out.GetDBContentStore(spin_cont, runNumber);
  if(getdbcontstore != 1) cout << "didn't get Database content" << endl;
  
  if(spin_out.CheckRunRowStore(runNumber) != 1){
    cout << "ERROR: spin db runnumber doesn't agree" << endl;
  }
  
  fillNumber = spin_cont.GetFillNumber();

  if(isRun13){
    
    //load tof correction
    ReadTOFMap(fillNumber);

  }
    
  return EVENT_OK;

}//end InitRun()

int ISUJetTrkClus::process_event(PHCompositeNode *topNode)
{

  if(!IsRunRange()){//check if runnumber is within the ranges of the Run
    return DISCARDEVENT;
  }
  

  if(!IsGoodRun(runNumber)){//check if it's a good run
    return DISCARDEVENT;
  }
 
  //get the nodes
  getNodes(topNode);


  //get the spin information (put here before trigger to see the spin pattern for fill
  lvl1trigger_clock_cross = lvl1trigger->get_lvl1_clock_cross();
  crossingShift = spin_cont.GetCrossingShift();

  ip12_clock_cross = (lvl1trigger_clock_cross + crossingShift)%120;
  

  if(!isMB){//if running over ERT

    //different triggers for each run
    bool trigFired = ERTFired();
   
    if(!trigFired) return DISCARDEVENT;

  }//endif NOT MinBias
  if(isMB){

    //using the BBC no vtx for all Runs
    if(isRun12 && isCuAu && !(lvl1trigger->get_lvl1_trigscaled() & MB_CUAU_NARROWVTX)){

      return DISCARDEVENT;
    }
    if(!isRun12 && !isCuAu && !(lvl1trigger->get_lvl1_trigscaled() & MB_NOVTX_BIT)){

      return DISCARDEVENT;
      
    }

  }//endif MinBias

  //get event number
  eventNumber = evtheader->get_EvtSequence(); 
  //Vertex
  zVertex = phglobal->getBbcZVertex();

  if(isMB && abs(zVertex) > 30){

    return DISCARDEVENT;
  }
  
  //get stuff for jets
  float centrality = 0;//NEEDS TO CHANGE FOR NON-PP (need for PC3/EMC calibrations)
  if(ispAu || isCuAu || ispAl)
    {
      centrality = phglobal->getCentrality();

      //Centrality cut- some events can have undefined centrality
      if(centrality < 0.0 || centrality > 100.0) return DISCARDEVENT;

    }

  GetTracks(topNode, all_tracks, centrality);
  GetClusters(topNode, all_clusters);

  GetParticles(all_tracks, all_clusters);

  //Fill Trees
  FillTrees(writeTree);

  // any other return code might lead to aborting the event or analysis
  return EVENT_OK;

}//process_event

int ISUJetTrkClus::ResetEvent(PHCompositeNode *topNode)
{
  all_tracks.clear();
  all_clusters.clear();

  return EVENT_OK;
}


int ISUJetTrkClus::EndRun(const int runNumber)
{
  if (verbo)
    {
      cout << ">>>>>>>>>>>>>>>>>>>  EndRun called <<<<<<<<<<<<<<<<<<<<<<<" << endl;
    }
 

  return EVENT_OK;
}

int ISUJetTrkClus::End(PHCompositeNode *topNode)
{
  if (verbo)
    {
      cout << ">>>>>>>>>>>>>>>>>>>  End called <<<<<<<<<<<<<<<<<<<<<<<" << endl;
    }


  outfile->Write();
  //  outfile->Close();

  //made them in initRun
  delete triggerHelper;

  return EVENT_OK;

}//end End()

void ISUJetTrkClus::getNodes(PHCompositeNode *topNode){

  //Make trigger selection
  lvl1trigger = getClass<TrigLvl1>(topNode, "TrigLvl1");

 
  //For ERT trigger
  if(isData){
    
    ertOut = getClass<ErtOut>(topNode, "ErtOut");
    if (!ertOut)
      {
	cout << "No ertOut!  No sense continuing" << endl;
	exit(1);
      }
  }//endif isData

  //Get the data I need...
  phglobal = getClass<PHGlobal>(topNode, "PHGlobal");
  if (!phglobal){
    
    cout << "No PHGlobal!  No sense continuing" << endl;
    exit(1);
  }
  
  evtheader = getClass<EventHeader>(topNode, "EventHeader");
  if (!evtheader){
  
    cout << "No EventHeader! No sense continuing" << endl;
    exit(1);
  }

}//end getNodes()


void ISUJetTrkClus::InitTrees(bool writeTrees)
{
  if(writeTrees)
    {
      tClusters = new TTree("tClusters", "Clusters");
      tClusters->Branch("tClusters", &all_clusters);
      
      tTracks = new TTree("tTracks", "Tracks");
      tTracks->Branch("tTracks", &all_tracks);

      tEvent = new TTree("tEvent", "Event");
      tEvent->Branch("runNumber", &runNumber, "runNumber/I");
      tEvent->Branch("fillNumber", &fillNumber, "fillNumber/I");
      tEvent->Branch("eventNumber", &eventNumber, "eventNumber/I");
      tEvent->Branch("level1_clock_cross", &lvl1trigger_clock_cross, "level1_clock_cross/I");
      tEvent->Branch("ip12_clock_cross", &ip12_clock_cross, "ip12_clock_cross/I");
      tEvent->Branch("vertex", &zVertex, "zVertex/F");

    }

  return;
}

void ISUJetTrkClus::FillTrees(bool writeTrees)
{
  if(writeTrees)
    {
      tClusters->Fill();
      tTracks->Fill();
      tEvent->Fill();
    }

  return;
}

void ISUJetTrkClus::setMB(bool what = true)
{
  if(what)
    {
      cout << "Running over MB data" << endl << endl;
      
    }
  else
    {
      cout << "Running over ERT data" << endl << endl;
      cout << "Looking for ERT 4x4b: ERT_4x4b, bit mask: " << ERT_BIT_MASK_B << endl << endl;
    }

  isMB = what;
  cout << "***********************************************************************" << endl;
  cout << "***********************************************************************" << endl << endl;

  return; 
}


void ISUJetTrkClus::setData(bool data = true)
{
  if(data)
    {
      cout << "Running over Data" << endl << endl;
      
    }
  else
    { 
      cout << "Running over Simulation" << endl << endl;
    }

  isData = data;
 
  return; 
}

void ISUJetTrkClus::setSkipDeadWarnMap(bool skip = true)
{
  if(skip)
    {
      cout << "Dead/Warn map will not be applied" << endl << endl;
      
    }
  else
    { 
      cout << "Dead/Warn map will be applied" << endl << endl;
    }

  skipDeadWarnMap = skip;
 
  return; 
}

void ISUJetTrkClus::setSkipDCBrokenWires(bool skip = true)
{
  if(skip)
    {
      cout << "DC broken wires will not be applied" << endl << endl;
      
    }
  else
    { 
      cout << "DC broken wires will be applied" << endl << endl;
    }

  skipDCBrokenWires = skip;
 
  return; 
}

void ISUJetTrkClus::GetParticles(std::vector<tracks>& all_tracks, std::vector<clusters>& all_clusters){

  //all charged tracks and clusters
  unsigned int numTracks = all_tracks.size();
  int Nclus = all_clusters.size();
  
  for(unsigned int iClus = 0; iClus < Nclus; iClus++){
    bool charged = false;
   
    //don't double count, so loop over tracks to remove charged track associated clusters
    for(int iTrk = 0; iTrk < numTracks; iTrk++){

      //Only 3 pass conditions for tracks: pass DC quality, pass minPt, and track matching to EMC
      bool passAll = all_tracks[iTrk].passDC && all_tracks[iTrk].passMinPt && all_tracks[iTrk].passEMCMatching;
      if(!passAll) continue;
    
      if(all_clusters[iClus].emcid == all_tracks[iTrk].emcid){

	//track matches to EMC and both agree, then a charged track made the cluster
	charged = true;

	// If this track points to a hot-dead or out-of-time cluster then 
	// update the matching flags - JGL 5/23/2019
	if(!all_clusters[iClus].passNotBad || !all_clusters[iClus].passToF) {
	  all_tracks[iTrk].passEMCMatching = false; 
	  all_tracks[iTrk].passMatching = all_tracks[iTrk].passEMCMatching || all_tracks[iTrk].passPC3Matching; 
	}
	else{
	  all_tracks[iTrk].assClusToF = all_clusters[iClus].tof;
	  all_tracks[iTrk].assClusEnergy = all_clusters[iClus].energy;
	}

	break; 
	
      }
     
    }//iTrk

    if(!charged) all_clusters[iClus].passIsNeutral = true;    
    
  }//iClus

  // Clean bad clusters from the cluster list 

  for(vector<clusters>::iterator it = all_clusters.begin(); it != all_clusters.end(); ){	
    if( !(it->passEnergy) || !(it->passNotBad) || !(it->passToF) )
      it = all_clusters.erase(it);
    else 
      ++it;
  }

  // Clean bad tracks from the track list 
  
  for(vector<tracks>::iterator it = all_tracks.begin(); it != all_tracks.end(); ){	
    if( !(it->passMinPt) || !(it->passMatching) )
      it = all_tracks.erase(it);
    else 
      ++it;
  }

  return;

}//end getparticles


void ISUJetTrkClus::GetTracks(PHCompositeNode *topNode, std::vector<tracks>& all_tracks, float centrality){

  PHCentralTrack *phcentral = getClass<PHCentralTrack>(topNode, "PHCentralTrack");
  if (!phcentral)
    {
      cout << "No PHCentral!  No sense continuing" << endl;
      exit(1);
    }
  
  //phcentral node for charged tracks
  for (unsigned int i = 0; i < phcentral->get_npart(); i++)
    {

      float mom           = phcentral->get_mom(i); // Magnitude of the momentum.
      float theta         = phcentral->get_the0(i); //The track's theta direction at the vertex
      float pT            = mom * sin(theta);
      float eta           = (float)(-log(tan(0.5 * theta)));
      float phi           = phcentral->get_phi0(i); // The track's phi direction at the vertex
      float phiDC         = phcentral->get_phi(i); // The phi coordinate at which the track crosses the drift chamber reference radius
      float zedDC         = phcentral->get_zed(i); //The Z coordinate at which the track crosses PC1
      float alpha         = phcentral->get_alpha(i); // This is the inclination of the track w.r.t. an infinite momentum track
      float energy        = phcentral->get_ecore(i); // EMC "shower core" energy
      
      int charge          = phcentral->get_charge(i);
      int quality         = phcentral->get_quality(i); // Quality of the Drift Chamber Tracks
      int n0              = phcentral->get_n0(i); //The number of phototubes that fired in the normally sized ring area
      int arm             = phcentral->get_dcarm(i); //Arm containing the track (East=0, West=1)
      int sector          = phcentral->get_sect(i); //EMC sector of the associated cluster
      int emcid           = phcentral->get_emcid(i); // Index of the emc cluster used
      
      float pc3dphi       = phcentral->get_pc3dphi(i);
      float pc3sdphi      = phcentral->get_pc3sdphi(i);
      float pc3dz         = phcentral->get_pc3dz(i);
      float pc3sdz        = phcentral->get_pc3sdz(i);
      float emcdphi       = phcentral->get_emcdphi(i);
      float emcsdphi      = phcentral->get_emcsdphi(i);
      float emcdz         = phcentral->get_emcdz(i);
      float emcsdz        = phcentral->get_emcsdz(i);
      
      float beta          = phcentral->get_beta(i);
      
      float px = pT * cos(phi);
      float py = pT * sin(phi);
      float pz = mom * cos(theta);
      float eT = energy * sin(theta);
     
      int armsect = -99;
      if(sector >= 0)//Calculate armsect if sector is properly initialized
	{
	  if (arm == 1){
	
	    armsect = sector;
	  }
	  else{
	
	    armsect = 4 + sector;
	  }
	}

      ////  DO THE MODIFIED DC QUALITY STUFF ///////////////
      float board = GetBoard(phiDC, arm);
      bool passQuality = IsModifiedQuality(runFlag, quality, phiDC, alpha, zedDC, arm);
      if(!skipDCBrokenWires) passQuality = passQuality && IsSimulationMatchOK(runFlag, phiDC, alpha, zedDC, arm); 

      //min pT cut
      bool passPt = PassMinPt(runFlag, pT);
      bool passZedDC = fabs(zedDC) < 75.0;
      bool passDC = passQuality && passZedDC && (armsect >= 0) && (armsect < 8);
    
      bool inBrokenX1 = false, inBrokenX2 = false, inBrokenUV = false;
      if(!skipDCBrokenWires) InBrokenWires(runFlag, inBrokenX1, inBrokenX2, inBrokenUV, phiDC, alpha, zedDC, arm);
     
      //for quality mask (I THINK THESE ARE THE SAME FOR ALL RUNS)
      bool x1Used = ((quality & X1_USED) == X1_USED);
      bool x2Used = ((quality & X2_USED) == X2_USED);
      bool uvUsed = ((quality & UV_FOUND_UNIQUE) == UV_FOUND_UNIQUE);

      //matching bools
      int ized = getDCZedBin(zedDC);
      if(ized == -9999) continue;

      int icent = (int)centrality;//set the centrality

      if(isRun12 && isCuAu){//from arbin's jetAnalyzer code

	ized = (int)(EMCMatchingRun12::NZED * (zedDC + 75.0) / 150.0);
	icent = getRun12CuAuCent(centrality);

      }//Run 12 CuAu

      if(isRun15)
	{
	  icent = 0;//No centrality dependence, but need to feed a 0 to the code
	}
      
      bool passEMCMatching = GetEMCMatching(runFlag, beta, pT, charge, armsect, ized, icent, emcdphi, emcdz, emcsdphi, emcsdz, energy);

      //need to do specialized & icent for CuAu PC3 matching (from Arbin)
      if(isRun12 && isCuAu){

	ized = getDCZedBinRun12CuAu(zedDC);
	int NMUL = 10;
	int icent = (int) ( NMUL * ((centrality - 0.001) / 100.) );

      }

      bool passPC3Matching = GetPC3Matching(runFlag, pT, charge, arm, ized, icent, pc3dphi, pc3dz, pc3sdphi, pc3sdz);
      bool passMatching = passPC3Matching || passEMCMatching;

      if(passQuality && pT > 0.2){
	if(zedDC > 0 && arm==0){//NE
	  hModifiedQuality_NE->Fill(board, alpha);
	}//endif NE
	if(zedDC > 0 && arm==1){//NW
	  hModifiedQuality_NW->Fill(board, alpha);
	}//endif NW
	if(zedDC < 0 && arm==0){//SE
	  hModifiedQuality_SE->Fill(board, alpha);
	}//endif SE
	if(zedDC < 0 && arm==1){//SW
	  hModifiedQuality_SW->Fill(board, alpha);
	}//endif SW
     
	if( (abs(pc3sdphi) < 3.0) && (abs(pc3sdz) < 3.0) ){
	  if(arm == 0) hPC3E_yVSz->Fill(phcentral->get_ppc3z(i), phcentral->get_ppc3y(i));
	  if(arm == 1) hPC3W_yVSz->Fill(phcentral->get_ppc3z(i), phcentral->get_ppc3y(i));
        }
        if(arm == 0) hPC1E_yVSz->Fill(phcentral->get_ppc1z(i), phcentral->get_ppc1y(i));
        if(arm == 1) hPC1W_yVSz->Fill(phcentral->get_ppc1z(i), phcentral->get_ppc1y(i));

      }//modified quality cut


      //conversion electron
      bool passConversionElectron = !GetConversionElectron(runFlag, pT, n0, energy, mom);
      //edge conversion cut
      bool passConversionEdge = !InEdge(phi);
     
      //    MAKING CUTS ON THE TRACKS  ///
      if(!passMatching || !passDC ) continue; 

      tracks tempTracks;
      tempTracks.mom            = mom;
      tempTracks.theta          = theta;
      tempTracks.pT             = pT;
      tempTracks.eta            = eta;
      tempTracks.phi            = phi;
      tempTracks.phiDC          = phiDC;
      tempTracks.zedDC          = zedDC;
      tempTracks.alpha          = alpha;
      tempTracks.energy         = energy;
      tempTracks.board          = board;

      tempTracks.charge         = charge;
      tempTracks.quality        = quality;
      tempTracks.n0             = n0;
      tempTracks.arm            = arm;
      tempTracks.armsect        = armsect;
      tempTracks.emcid          = emcid;

      tempTracks.px             = px;
      tempTracks.py             = py;
      tempTracks.pz             = pz;
      tempTracks.eT             = eT;

      tempTracks.pc3dphi        = pc3dphi;
      tempTracks.pc3dz          = pc3dz;
      tempTracks.pc3sdphi       = pc3sdphi;
      tempTracks.pc3sdz         = pc3sdz;
      tempTracks.emcdphi        = emcdphi;
      tempTracks.emcdz          = emcdz;
      tempTracks.emcsdphi       = emcsdphi;
      tempTracks.emcsdz         = emcsdz;
      
      tempTracks.inBrokenX1     = inBrokenX1;
      tempTracks.inBrokenX2     = inBrokenX2;
      tempTracks.inBrokenUV     = inBrokenUV;
      tempTracks.x1Used         = x1Used;
      tempTracks.x2Used         = x2Used;
      tempTracks.uvUsed         = uvUsed;

      //global variables
      tempTracks.runnumber      = runNumber;
      tempTracks.eventnumber    = eventNumber;

      //bools for different track cuts
      tempTracks.passDC                 = passDC; 
      tempTracks.passQuality            = passQuality;
      tempTracks.passMatching           = passMatching;
      tempTracks.passEMCMatching        = passEMCMatching;
      tempTracks.passPC3Matching        = passPC3Matching;
      tempTracks.passMinPt              = passPt;
      tempTracks.passConversionElectron = passConversionElectron;
      tempTracks.passConversionEdge     = passConversionEdge;

      //cluster assocation stuff will be set in GetParticles()
      tempTracks.assClusToF    = -999;
      tempTracks.assClusEnergy = -999;

      //pair cuts so need to do nested loop over tracks; in func SetPairCuts()
      tempTracks.passGhost          = true;
      tempTracks.passDcConversion   = true;

      all_tracks.push_back(tempTracks); 
      
    }//i (all tracks)

  SetPairCuts(all_tracks);
  

}//GetTracks

void ISUJetTrkClus::GetClusters(PHCompositeNode *topNode, std::vector<clusters>& all_clusters){

  emcClusterContainer *emcclustercontainer = getClass<emcClusterContainer>(topNode, "emcClusterContainer");
  if (!emcclustercontainer){
    
    cout << "No emcClusterContainer!  No sense continuing" << endl;
    exit(1);
  }
  
  float bbc_t0 = phglobal->getBbcTimeZero();

  int Nclus = emcclustercontainer->size();
  for (int iclus = 0; iclus < Nclus; iclus++)
    {
      clus = emcclustercontainer->getCluster(iclus);
      int arm         = clus->arm(); //In EMCal convention, West Arm is 0 and East Arm is 1, and thus armsector 0...7 are W0...W3 E0...E3
      int sector      = clus->sector();
      int emcid       = clus->id();
      float energy    = clus->ecore();
      float prob      = clus->prob_photon();
      float chi2      = clus->chi2();
      float emctrkdphi = clus->emctrkdphi();
      float emctrkdz  = clus->emctrkdz();
      //need to construct cluster theta and pT
      float x         = clus->x();
      float y         = clus->y();
      float z         = clus->z() - zVertex;
      float theta     = clus->theta();//acos(z / sqrt(x * x + y * y + z * z));
      int yTowerPos   = clus->iypos();
      int zTowerPos   = clus->izpos();
      int armsect     = (arm * 4) + sector; //Arm for emcal is different
      float tof_corr  = 0.0;
      if(isRun15)
	{
	  tof_corr  = clus->tofcorr();
	}
      if(isRun13)
	{
	  tof_corr = clus->tof() - TofCorrection(armsect, yTowerPos, zTowerPos);
	}
      if(isRun12)
	{
	  tof_corr = clus->tofcorr();
	}

      float tof = tof_corr - bbc_t0;//TOF of the central tower, minus the flash time (=sqrt(x2+y2+z2)/30.0)) Unit ns.

      TVector3 v3;
      TLorentzVector v4;
      v3.SetXYZ(x, y, z);
      v3 = energy * v3.Unit();
      v4.SetT(energy);
      v4.SetVect(v3);

      float pT        = v4.Pt();
      if(pT == 0) continue;//protect against eta crash
      float phi       = phiReduce(v4.Phi());
      float px        = v4.Px();
      float py        = v4.Py();
      float pz        = v4.Pz();
      float eta       = v4.Eta();
      float eT        = v4.Et();

      int towerId         = EmcMapRun13::GetTowerID(armsect, yTowerPos, zTowerPos);
      bool passIsValid    = EmcMapRun13::IsValidYZ(armsect, yTowerPos, zTowerPos);
      bool passNotBad     = ((clus->warnmap() & CUT3x3MAP) == 0) && ((clus->deadmap() & CUT3x3MAP) == 0);

      //needed for Run13
      if(isRun13){

	bool passIsValid    = EmcMapRun13::IsValidYZ(armsect, yTowerPos, zTowerPos);
	bool passNotBad2    = !EmcMapRun13::IsBad(armsect, yTowerPos, zTowerPos);
	bool passNotHot     = !EmcMapRun13::IsHot(armsect, yTowerPos, zTowerPos) && !EmcMapRun13::isHotAdditional(towerId);
	bool passNotDead    = !EmcMapRun13::IsDead(armsect, yTowerPos, zTowerPos);
	bool passNotUncalib = !EmcMapRun13::IsUncalib(armsect, yTowerPos, zTowerPos);

	passNotBad = passIsValid && passNotBad2 && passNotHot && passNotDead && passNotUncalib;
	
      }
      //run12 dead/hot towers
      if(isRun12){

	bool passIsValid    = EmcMapRun12::IsValidYZ(armsect, yTowerPos, zTowerPos);
	bool passNotBad2    = !EmcMapRun12::IsBad(armsect, yTowerPos, zTowerPos);
	bool passNotHot     = !EmcMapRun12::IsHot(armsect, yTowerPos, zTowerPos);
	bool passNotDead    = !EmcMapRun12::IsDead(armsect, yTowerPos, zTowerPos);
	bool passNotUncalib = !EmcMapRun12::IsUncalib(armsect, yTowerPos, zTowerPos);

	passNotBad = passIsValid && passNotBad2 && passNotHot && passNotDead && passNotUncalib;
      }

      bool ertFired;
      if(isData)
	{
	  ertFired = GetERT(runFlag, arm, sector, yTowerPos, zTowerPos);
	}
      else
	{
	  ertFired = false;
	}
      
      //Cluster selection
      bool passEnergy = PassMinEnergy(runFlag,energy);

      //tof cut
      bool passToF = abs(tof) < 15.0;

      clusters temp;
      temp.arm             = arm;
      temp.sector          = sector;
      temp.armsect         = armsect;
      temp.emcid           = emcid;
      temp.yTowerPos       = yTowerPos;
      temp.zTowerPos       = zTowerPos;
      temp.towerId         = towerId;
	
      temp.ertFired        = ertFired;
	
      temp.energy          = energy;
      temp.theta           = theta;
      temp.pT              = pT;
      temp.eT              = eT;
      temp.phi             = phi;
      temp.px              = px;
      temp.py              = py;
      temp.pz              = pz;
      temp.eta             = eta;
      temp.chi2            = chi2;
      temp.prob            = prob;
      temp.runnumber       = runNumber;
      temp.eventnumber     = eventNumber;
      temp.tof             = tof;
      temp.emctrkdphi      = emctrkdphi;
      temp.emctrkdz        = emctrkdz;
      
      if(skipDeadWarnMap)
	temp.passNotBad      = true;
      else
	temp.passNotBad      = passNotBad;

      temp.passEnergy      = passEnergy; 
      temp.passToF         = passToF;
      temp.passIsNeutral   = false;//will set later
	
      all_clusters.push_back(temp);

    }//iclus
   

}//GetClusters

void ISUJetTrkClus::SetPairCuts(std::vector<tracks>& all_tracks){

  // std::cout << "Conversion Electron cut the same for each run\n";

  //do pair cuts
  for(int iTrk = 0; iTrk < all_tracks.size(); iTrk++){
    for(int jTrk2 = 0; jTrk2 < iTrk; jTrk2++){

      bool passFirstiTrk = all_tracks[iTrk].passMinPt && all_tracks[iTrk].passQuality && all_tracks[iTrk].passMatching && all_tracks[iTrk].passDC;
      bool passFirstjTrk2 = all_tracks[jTrk2].passMinPt && all_tracks[jTrk2].passQuality && all_tracks[jTrk2].passMatching && all_tracks[jTrk2].passDC;
      bool sameArm = all_tracks[iTrk].arm == all_tracks[jTrk2].arm;

      if(!passFirstiTrk || !passFirstjTrk2 || !sameArm) continue;

      float dPhi = all_tracks[iTrk].phiDC - all_tracks[jTrk2].phiDC;
      float dZed = all_tracks[iTrk].zedDC - all_tracks[jTrk2].zedDC;

      //Ghost Cut
      if ((all_tracks[iTrk].charge == all_tracks[jTrk2].charge) && fabs(dPhi) < 0.024 && fabs(dZed) < 0.105){

	  float asymmetry = fabs(all_tracks[iTrk].pT - all_tracks[jTrk2].pT) / (all_tracks[iTrk].pT + all_tracks[jTrk2].pT);

	  if (asymmetry < 0.3){
	
	    all_tracks[jTrk2].passGhost = false;
	  }
	  else{
	    
	    all_tracks[iTrk].passGhost  = false;
	    all_tracks[jTrk2].passGhost = false;

	  }

      }//endif Ghost Cut

      //Converion in DC/PC1: Conversion pairs have different charge
      if ((all_tracks[iTrk].charge != all_tracks[jTrk2].charge) && fabs(dPhi) < 0.07 && fabs(dZed) < 0.105){
	
	  all_tracks[iTrk].passDcConversion  = false;
	  all_tracks[jTrk2].passDcConversion = false;

	}//endif (conversion cut)


    }//jTrk2
  }//iTrk



}//SetPairCuts

bool ISUJetTrkClus::InEdge(float phi0){  
      

  return ((phi0 > -0.65 && phi0 < -0.49) ||
	  (phi0 > 0.89 && phi0 < 1.05) ||
	  (phi0 > 2.10 && phi0 < 2.26) ||
	  (phi0 > 3.62 && phi0 < 3.78));
}//InEdge


bool ISUJetTrkClus::GetConversionElectron(int runFlag, float pT, int n0, float energy, float mom){

  //std::cout << "Conversion Electron cut the same for each run\n";

  bool conversionElectron = (pT < 4.5) && (n0 >= 2) && ( (energy / mom) < 0.6);

  return conversionElectron;

}


bool ISUJetTrkClus::GetPC3Matching(int runFlag, float pT, int icharge, int arm, int ized, int icent, float pc3dphi, float pc3dz, float& pc3sdphi, float& pc3sdz){

  bool passPC3Matching = false;
  if((pc3dphi == -9999) || (pc3dz == -9999))
    {
      return passPC3Matching;
    }
  //fix the charge for the selector matching
  if(icharge == -1) icharge = 0;


  if(!isData){//simulation (any Run)

    float pc3dphi_init = CalcInitialPC3Sim(pT, icharge, arm, ized, icent,  INITIAL, 1, pc3dphi);
    pc3sdphi = CalcFinalPC3Sim(pT, icharge, arm, ized, icent, INTERMEDIATE, 1, pc3dphi_init);

    float pc3dz_init = CalcInitialPC3Sim(pT, icharge, arm, ized, icent,  INITIAL, 0, pc3dz);//0 for dz
    pc3sdz = CalcFinalPC3Sim(pT, icharge, arm, ized, icent, INTERMEDIATE, 0, pc3dz_init);//0 for dz

    

    passPC3Matching = sqrt((pc3sdphi*pc3sdphi) + (pc3sdz*pc3sdz)) < SIGMA_MATCHING_RUN13;

    return passPC3Matching;

  }//simulation

  if(isRun13){

    if(isData){

      float pc3dphi_initial = CalcPC3Run13(pT, icharge, arm, ized, icent, pc3dphi, INITIAL, DPHI);
      pc3sdphi = CalcPC3Run13(pT, icharge, arm, ized, icent, pc3dphi_initial, INTERMEDIATE, DPHI);
      
      float pc3dz_initial = CalcPC3Run13(pT, icharge, arm, ized, icent, pc3dz, INITIAL, DZ);
      pc3sdz = CalcPC3Run13(pT, icharge, arm, ized, icent, pc3dz_initial, INTERMEDIATE, DZ);
    }
    
    passPC3Matching = sqrt((pc3sdphi*pc3sdphi) + (pc3sdz*pc3sdz)) < SIGMA_MATCHING_RUN13;
    

  }//Run 13
  else if(isRun12){

    if(isCuAu){

      if(icent > 0 && icent < 10){
	
	pc3sdphi = pc3_sdphi_func_II(icharge, arm, icent, ized, pT, pc3dphi);
	pc3sdz   = pc3_sdz_func_II(icharge, arm, icent, ized, pT, pc3dphi);
	
	passPC3Matching = sqrt((pc3sdphi*pc3sdphi) + (pc3sdz*pc3sdz)) < SIGMA_MATCHING_RUN12;

      }
      else{

	passPC3Matching = false;
      }

    }//CuAu
    if(isPP){

      //no calibs needed
      passPC3Matching = sqrt((pc3sdphi*pc3sdphi) + (pc3sdz*pc3sdz)) < SIGMA_MATCHING_RUN12;

    }

  }//Run 12
  else if(isRun15){
    //PC3 Matching is in the master recalibrator
    
    passPC3Matching = sqrt((pc3sdphi*pc3sdphi) + (pc3sdz*pc3sdz)) < SIGMA_MATCHING_RUN15;

  }//Run 15
  else{
    std::cout << "DON'T HAVE PROPER RUNFLAG (GetPC3Matching)\n";
  }
  
  return passPC3Matching;

}//GetPC3Matching

bool ISUJetTrkClus::GetEMCMatching(int runFlag, float beta, float pT, int charge, int armsect, int ized, int icent, float emcdphi, float emcdz, float& emcsdphi, float& emcsdz, float& energy){

  bool passEMCMatching = false;
  if(armsect < 0)
    {
      return passEMCMatching;//Uninitialized sector can't match
    }
  if(energy < EMCMATCHING_ENE_CUT) return passEMCMatching;

  //fix the charge for the selector matching
  if(charge == -1) charge = 0;

  if(!isData)
    {
      //do calibration
      float emcdphi_initial = CalculateInitialEmcsdPhiSim(charge, armsect, ized, icent, emcdphi, pT);
      emcsdphi = CalculateFinalEmcsdPhiSim(charge, armsect, ized, icent, emcdphi_initial, pT);
      float emcdz_corrected = CalculateCorrectedEmcdZSim(beta, pT, emcdz, armsect);
      float emcdz_initial = CalculateInitialEmcsdZSim(charge, armsect, ized, icent, emcdz_corrected, pT);
      emcsdz = CalculateFinalEmcsdZSim(charge, armsect, ized, icent, emcdz_initial, pT);
   
    
      passEMCMatching = sqrt((emcsdphi*emcsdphi) + (emcsdz*emcsdz)) < SIGMA_MATCHING_RUN15;
      return passEMCMatching;//Skip the run dependence if is simulation and just return here
    }

  if(isRun13){
    
    float emcdphi_initial = CalcEMCRun13(pT, charge, armsect, ized, icent, emcdphi, INITIAL, DPHI);
    emcsdphi = CalcEMCRun13(pT, charge, armsect, ized, icent, emcdphi_initial, INTERMEDIATE, DPHI);
    
    float emcdz_corrected = CalculateCorrectedEMCdzRun13(beta, pT, emcdz, armsect);
    float emcdz_initial = CalcEMCRun13(pT, charge, armsect, ized, icent, emcdz_corrected, INITIAL, DZ);
    emcsdz = CalcEMCRun13(pT, charge, armsect, ized, icent, emcdz_initial, INTERMEDIATE, DZ);

    passEMCMatching =sqrt((emcsdphi*emcsdphi) + (emcsdz*emcsdz)) < SIGMA_MATCHING_RUN13;
     
  }//Run 13
  else if(isRun12){
    if(isCuAu){

      if( !((ized >=0 && ized <=9) && (icent >= 0 && icent <= 5)) ) return false;

      float emcdphi_initial = CalculateInitialEmcsdPhi(charge, armsect, ized, icent, emcdphi, pT);
      emcsdphi = CalculateFinalEmcsdPhi(charge, armsect, ized, icent, emcdphi_initial, pT);
      
      float emcdz_corrected = CalculateCorrectedEmcdZRun12(beta, pT, emcdz, armsect);
      float emcdz_initial = CalculateInitialEmcsdZ(charge, armsect, ized, icent, emcdz_corrected, pT);
      emcsdz = CalculateFinalEmcsdZ(charge, armsect, ized, icent, emcdz_initial, pT);
      
      passEMCMatching = sqrt((emcsdphi*emcsdphi) + (emcsdz*emcsdz)) < SIGMA_MATCHING_RUN12;

    }
    if(isPP){

      passEMCMatching = sqrt((emcsdphi*emcsdphi) + (emcsdz*emcsdz)) < SIGMA_MATCHING_RUN12;

    }

  }//Run 12
  else if(isRun15){
    if(ispAu)
      {
	float emcdphi_initial = CalculateInitialEmcsdPhiRun15pAu(charge, armsect, ized, icent, emcdphi, pT);
	emcsdphi = CalculateFinalEmcsdPhiRun15pAu(charge, armsect, ized, icent, emcdphi_initial, pT);
      
	float emcdz_corrected = CalculateCorrectedEmcdZRun15pAu(beta, pT, emcdz, armsect);
	float emcdz_initial = CalculateInitialEmcsdZRun15pAu(charge, armsect, ized, icent, emcdz_corrected, pT);
	emcsdz = CalculateFinalEmcsdZRun15pAu(charge, armsect, ized, icent, emcdz_initial, pT);
   
    
	passEMCMatching = sqrt((emcsdphi*emcsdphi) + (emcsdz*emcsdz)) < SIGMA_MATCHING_RUN15;
      }
    if(isPP)
      {
	float emcdphi_initial = CalculateInitialEmcsdPhiRun15pp(charge, armsect, ized, icent, emcdphi, pT);
	emcsdphi = CalculateFinalEmcsdPhiRun15pp(charge, armsect, ized, icent, emcdphi_initial, pT);
      
	float emcdz_corrected = CalculateCorrectedEmcdZRun15pp(beta, pT, emcdz, armsect);
	float emcdz_initial = CalculateInitialEmcsdZRun15pp(charge, armsect, ized, icent, emcdz_corrected, pT);
	emcsdz = CalculateFinalEmcsdZRun15pp(charge, armsect, ized, icent, emcdz_initial, pT);
   
    
	passEMCMatching = sqrt((emcsdphi*emcsdphi) + (emcsdz*emcsdz)) < SIGMA_MATCHING_RUN15;
      }
    if(ispAl)
      {
	passEMCMatching = true;
      }
  }//Run 15
  else{

    std::cout << "DON'T HAVE PROPER RUNFLAG (GetEMCMatching)\n";
  }

  return passEMCMatching;


}//GetEMCMatching

void ISUJetTrkClus::InBrokenWires(int runFlag, bool& inBrokenX1, bool& inBrokenX2, bool& inBrokenUV, float& phiDC, float& alpha, float& zedDC, int& arm){

  if(isRun13){

    inBrokenX1 = TrackQualityPPRun13::inBrokenX1(phiDC, alpha, zedDC, arm);
    inBrokenX2 = TrackQualityPPRun13::inBrokenX2(phiDC, alpha, zedDC, arm);
    inBrokenUV = TrackQualityPPRun13::inBrokenUV(phiDC, alpha, zedDC, arm);

  }//Run 13
  else if(isRun12){

    if(isPP){

      inBrokenX1 = TrackQualityPPRun12::inBrokenX1(phiDC, alpha, zedDC, arm);
      inBrokenX2 = TrackQualityPPRun12::inBrokenX2(phiDC, alpha, zedDC, arm);
      inBrokenUV = TrackQualityPPRun12::inBrokenUV(phiDC, alpha, zedDC, arm);

    }//PP

    else{//CuAu

      inBrokenX1 = TrackQualityCuAuRun12::inBrokenX1(phiDC, alpha, zedDC, arm);
      inBrokenX2 = TrackQualityCuAuRun12::inBrokenX2(phiDC, alpha, zedDC, arm);
      inBrokenUV = TrackQualityCuAuRun12::inBrokenUV(phiDC, alpha, zedDC, arm);

    }//CuAu

  }//12
  else if(isRun15){
    
    inBrokenX1 = TrackQualityRun15pAu::inBrokenX1(phiDC, alpha, zedDC, arm);
    inBrokenX2 = TrackQualityRun15pAu::inBrokenX2(phiDC, alpha, zedDC, arm);
    inBrokenUV = TrackQualityRun15pAu::inBrokenUV(phiDC, alpha, zedDC, arm);
    
  }//15
  else{

    std::cout << "DON'T HAVE PROPER RUNFLAG (InBrokenWires)\n";
  }
  
}//InBrokenWires


bool ISUJetTrkClus::PassMinPt(int runFlag, float pT){

  bool retVal = false;

  if(isRun13){

    retVal = pT > TRACK_MIN_PT_CUT_RUN13;
  }
  else if(isRun12){

    retVal = pT > TRACK_MIN_PT_CUT_RUN12;
  }
  else if(isRun15){

    retVal = pT > TRACK_MIN_PT_CUT_RUN15;
  }
  else{

    std::cout << "DON'T HAVE PROPER RUNFLAG (PassMinPt)\n";
  }

  return retVal;

}//PassMinPt

bool ISUJetTrkClus::PassMinEnergy(int runFlag, float energy){

  bool retVal = false;

  if(isRun13){

    retVal = energy > CLUSTER_MIN_ENERGY_CUT_RUN13;
  }
  else if(isRun12){

    retVal = energy > CLUSTER_MIN_ENERGY_CUT_RUN12;
  }
  else if(isRun15){

    retVal = energy > CLUSTER_MIN_ENERGY_CUT_RUN15;
  }
  else{

    std::cout << "DON'T HAVE PROPER RUNFLAG (PassMinEnergy)\n";
  }

  return retVal;

}//PassMinEnergy


bool ISUJetTrkClus::IsModifiedQuality(int runFlag, int quality, float phiDC, float alpha, float zedDC, int arm){

  bool retVal = false;

  if(isRun13){

    retVal = TrackQualityPPRun13::passQualityMask(quality, phiDC, alpha, zedDC, arm);
  }
  else if(isRun12){
    
    if(isPP){
      retVal = TrackQualityPPRun12::passQualityMask(quality, phiDC, alpha, zedDC, arm);
    }
    else if(isCuAu){
      retVal = TrackQualityCuAuRun12::passQualityMask(quality, phiDC, alpha, zedDC, arm);
    }
    else return false;

  }
  else if(isRun15){

    retVal = TrackQualityRun15pAu::passQualityMask(quality, phiDC, alpha, zedDC, arm);
  }
  else{
  
    std::cout << "DON'T HAVE PROPER RUNFLAG (IsModifiedQuality)\n";
  }

  return retVal;

}//IsModifiedQuality

bool ISUJetTrkClus::IsSimulationMatchOK(int runFlag, float phiDC, float alpha, float zed, int arm){

  bool retVal = false;

  if(isRun13){

    retVal = TrackQualityPPRun13::passSimulationMatchCuts(phiDC, alpha, zed, arm);
  }
  else if(isRun12){
    
    if(isPP){
      retVal = TrackQualityPPRun12::passSimulationMatchCuts(phiDC, alpha, arm);
    }
    else if(isCuAu){
      retVal = TrackQualityCuAuRun12::passSimulationMatchCuts(phiDC, alpha, arm);
    }
    else return false;

  }
  else if(isRun15){

    retVal = TrackQualityRun15pAu::passSimulationMatchCuts(phiDC, alpha, arm);
  }
  else{
  
    std::cout << "DON'T HAVE PROPER RUNFLAG (IsSimulationMatchOK)\n";
  }

  return retVal;

}//IsSimulationMatchOK


float ISUJetTrkClus::GetBoard(float phi, int arm){

  float board = -9999;
  if (arm == 0) board = (3.72402 - phi + 0.008047 * cos(phi + 0.87851)) / 0.01963496;
  if (arm == 1) board = (0.573231 + phi - 0.0046 * cos(phi + 0.05721)) / 0.01963496;
  
  return board;
  
}//GetBoard


bool ISUJetTrkClus::ERTFired(){


  if(isRun13){
    if(!(lvl1trigger->get_lvl1_trigscaled() & ERT_BIT_MASK_B)){
    
      return false;
    }
    else return true;
  }//Run13
  else if(isRun12){

    if(isPP && !(lvl1trigger->get_lvl1_trigscaled() & ERT_BIT_MASK_C)){
      
      return false;
    }
    else return true;
  }//Run12
  else if(isRun15){
    //SET THE SAME AS RUN 12 FOR NOW, CHANGE IF IT'S DIFFERENT (REMOVE THIS COMMENT IF NOT)
    if(!(lvl1trigger->get_lvl1_trigscaled() & ERT_BIT_MASK_B)){
      
      return false;
    }
    else return true;

  }//Run15
  else return false;

}//ERTFired

bool ISUJetTrkClus::GetERT(int runFlag, int arm, int sector, int y, int z){

  //ERT information; ertBit: 0=4x4a, 1=4x4b, 2=4x4c
  int ertSM = (arm == 1 && sector < 2)  ?  (y / 12) * 8 + z / 12  :  (y / 12) * 6 + z / 12;
   
  bool ertFired = false;

  bool ert4x4A = false;
  bool ert4x4B = false;
  bool ert4x4C = false;

  ert4x4A = ertOut->get_ERTbit(0, arm, sector, ertSM);
  ert4x4B = ertOut->get_ERTbit(1, arm, sector, ertSM);
  ert4x4C = ertOut->get_ERTbit(2, arm, sector, ertSM);
     
 
  if(isRun12)
    {
      ertFired = ert4x4C;
    }  
  if(isRun13)
    {
      ertFired = ert4x4B;
    }
  if(isRun15)
    {
      ertFired = ert4x4B;
    }

  return ertFired;

}//GetERT

int ISUJetTrkClus::getRun12CuAuCent(float& centrality){

  int icent = -9999;
  if (centrality > 0 && centrality <= 10)
    {
      icent = 0;
    }
  if (centrality > 10 && centrality <= 20)
    {
      icent = 1;
    }
  if (centrality > 20 && centrality <= 30)
    {
      icent = 2;
    }
  if (centrality > 30 && centrality <= 40)
    {
      icent = 3;
    }
  if (centrality > 40 && centrality <= 60)
    {
      icent = 4;
    }
  if (centrality > 60 && centrality <= 100)
    {
      icent = 5;
    }

  return icent;

}//getRun12 CuAu icent

int ISUJetTrkClus::getDCZedBinRun12CuAu(float& zedDC){

  int ized = -9999;
  if     (zedDC > -70 && zedDC <= -57)
    {
      ized = 0;
    }
  else if (zedDC > -57 && zedDC <= -44)
    {
      ized = 1;
    }
  else if (zedDC > -44 && zedDC <= -31)
    {
      ized = 2;
    }
  else if (zedDC > -31 && zedDC <= -18)
    {
      ized = 3;
    }
  else if (zedDC > -18 && zedDC <= -5)
    {
      ized = 4;
    }
  else if (zedDC >= 5  && zedDC <  18)
    {
      ized = 5;
    }
  else if (zedDC >= 18 && zedDC <  31)
    {
      ized = 6;
    }
  else if (zedDC >= 31 && zedDC <  44)
    {
      ized = 7;
    }
  else if (zedDC >= 44 && zedDC <  57)
    {
      ized = 8;
    }
  else if (zedDC >= 57 && zedDC <  70)
    {
      ized = 9;
    }

  return ized;

}//ZedBin Run12 CuAu

int ISUJetTrkClus::getDCZedBin(float& zedDC){

  int ized;
  if(zedDC > -75 && zedDC <= -60){
    ized = 0;
  }
  else if(zedDC > -60 && zedDC <= -45){
    ized = 1;
  }
  else if(zedDC > -45 && zedDC <= -30){
    ized = 2;
  }
  else if(zedDC > -30 && zedDC <= -15){
    ized = 3;
  }
  else if(zedDC > -15 && zedDC <= 0){
    ized = 4;
  }
  else if(zedDC > 0 && zedDC <= 15){
    ized = 5;
  }
  else if(zedDC > 15 && zedDC <= 30){
    ized = 6;
  }
  else if(zedDC > 30 && zedDC <= 45){
    ized = 7;
  }
  else if(zedDC > 45 && zedDC <= 60){
    ized = 8;
  }
  else if(zedDC > 60 && zedDC <= 75){
    ized = 9;
  }
  else ized = -9999;



  return ized;

}//getDCZedBin

bool ISUJetTrkClus::Run12GoodCuAu(int runNumber){

  //CuAu bad run list: 30 Runs out of 451
  if(runNumber == 372402 || runNumber == 372524 || runNumber == 372525 || runNumber == 372531 || runNumber == 372533 || runNumber == 372536 || runNumber == 372647 || runNumber == 372648 || runNumber == 372959 || runNumber == 372961 || runNumber == 373407 || runNumber == 373655 || runNumber == 373672 || runNumber == 374428 || runNumber == 375773 || runNumber == 375774 || runNumber == 375906 || runNumber == 375953 || runNumber == 375957 || runNumber == 376433 || runNumber == 376434 || runNumber == 376435 || runNumber == 376620 || runNumber == 377155 || runNumber == 377156 || runNumber == 377157 || runNumber == 377167 || runNumber == 377171 || runNumber == 377172 || runNumber == 377173)  {
    return false;
  }
  else return true;

}//run12GoodCuAu

bool ISUJetTrkClus::Run12Goodpp(int runNumber){

if(runNumber == 358661 || runNumber == 358663 || runNumber == 358665 || runNumber == 358667 || runNumber == 358710 ||
     runNumber == 358711 || runNumber == 358717 || runNumber == 358719 || runNumber == 358720 || runNumber == 358722 ||
     runNumber == 358724 || runNumber == 358725 || runNumber == 358740 || runNumber == 358742 || runNumber == 358743 ||
     runNumber == 358749 || runNumber == 358750 || runNumber == 358751 || runNumber == 358752 || runNumber == 358754 ||
     runNumber == 358758 || runNumber == 358759 || runNumber == 358767 || runNumber == 358768 || runNumber == 358771 ||
     runNumber == 358772 || runNumber == 358773 || runNumber == 358776 || runNumber == 358777 || runNumber == 358778 ||
     runNumber == 358779 || runNumber == 358780 || runNumber == 358782 || runNumber == 358783 || runNumber == 358898 ||
     runNumber == 358899 || runNumber == 358900 || runNumber == 358903 || runNumber == 358904 || runNumber == 358924 ||
     runNumber == 358985 || runNumber == 358986 || runNumber == 358988 || runNumber == 358991 || runNumber == 358992 ||
     runNumber == 358996 || runNumber == 358997 || runNumber == 358998 || runNumber == 359002 || runNumber == 359060 ||
     runNumber == 359061 || runNumber == 359062 || runNumber == 359064 || runNumber == 359293 || runNumber == 359520 || runNumber == 359696 || runNumber == 359791 || runNumber == 360075 || runNumber == 360076 || runNumber == 360077 || runNumber == 360079 || runNumber == 360081 || runNumber == 360082 || runNumber == 360083 || runNumber == 360088 || runNumber == 360089 || runNumber == 360125 || runNumber == 360126 || runNumber == 360128 || runNumber == 360132 || runNumber == 360135 || runNumber == 360136 || runNumber == 360138 || runNumber == 360139 || runNumber == 360140 || runNumber == 360141 || runNumber == 360510 || runNumber == 361244 || runNumber == 361640 || runNumber == 361641 || runNumber == 362214 || runNumber == 362260 ||
     //Post QM15- bad mean vertex
   runNumber == 360475 || runNumber == 360501 || runNumber == 363196){
  
  return false;
 }
 else return true;


}//badRun12

void ISUJetTrkClus::SetRunBools(int runNum){

  //set them to false
  isRun12 = false;
  isRun13 = false;
  isRun15 = false;

  if(runNum == 12) isRun12 = true;
  else if(runNum == 13) isRun13 = true;
  else if(runNum == 15) isRun15 = true;
  else{
    std::cout << "ERROR: THE RUNNUMBER IS NOT SET PROPERLY (not 12, 13, OR 15)\n";
  }

}//setRunBools

void ISUJetTrkClus::SetSpeciesBools(std::string species){

  isCuAu = false;
  isPP = false;
  ispAu = false;
  ispAl = false;

  std::string PP = "PP";
  std::string pAu = "PAU";
  std::string pAl = "PAL";
  std::string CuAu = "CUAU";

  std::transform(species.begin(), species.end(), species.begin(), ::toupper);
  
  std::cout << std::endl << std::endl << species << std::endl << std::endl;

  if(species.compare(PP) == 0) isPP = true;
  else if(species.compare(pAu) == 0) ispAu = true;
  else if(species.compare(pAl) == 0) ispAl = true;
  else if(species.compare(CuAu) == 0) isCuAu = true;
  else{
    std::cout << "ERROR: BEAM SPECIES NOT SET(not pp, pAu, pAl, or CuAu)\n";
  }


}//SetSpeciesBool

