//General PHENIX tools
#include <PHCompositeNode.h>
#include <PHNodeIterator.h>
#include <PHNodeReset.h>
#include <PHIODataNode.h>
#include <phool.h>
#include <getClass.h>
#include <RunHeader.h>

//Fun4All tools
#include <Fun4AllServer.h>
#include <Fun4AllHistoManager.h>
#include <Fun4AllReturnCodes.h>

//PHPythia tools
#include <PHPyTrigger.h>
#include <PHPythiaHeader.h>
#include <PHPyCommon.h>
#include <PHPythiaContainer.h>

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,15,8)
#include <TMCParticle.h>
#else
#include <TMCParticle6.h>
#endif

//Root tools
#include <TH1.h>
#include <TFile.h>
#include <TNtuple.h>

//My source file
#include "PythiaParticles.h"

using namespace std;
using namespace findNode;

//================================ Constructor ================================
//Initialize protected variables

PythiaParticles::PythiaParticles(std::string outFileName)
    : SubsysReco("PythiaParticles")
{
    outfile = new TFile(outFileName.c_str(),"RECREATE");
    nTotalEvents    = 0;
    event_x1 = -999.9;
    event_x2 = -999.9;
    event_Qsqr = -9999;
    event_processid = -999;
    SetParticleType('b');
    InitTrees();
    return;
}//Output file name

PythiaParticles::PythiaParticles(const char particleType,
    std::string outFileName)
    : SubsysReco("PythiaParticles")
{
    outfile = new TFile(outFileName.c_str(),"RECREATE");
    SetParticleType(particleType);
    nTotalEvents    = 0;
    event_x1 = -999.9;
    event_x2 = -999.9;
    event_Qsqr = -9999;
    event_processid = -999;
    InitTrees();
    return;
}//particle type and output file name

void PythiaParticles::SetParticleType(char partiType)
{
    if(partiType=='p'){
      writeHadron     = false;
      writeParton     = true;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"In PythiaParticles, particleType = p, write out partons only..."<<endl;
      cout<<"To choose particles to write out: 'p' for partons, 'h' for hadrons"<<endl;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
    else if(partiType=='h'){
      writeHadron     = true;
      writeParton     = false;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"In PythiaParticles, particleType = h, write out hadrons only..."<<endl;
      cout<<"To choose particles to write out: 'p' for partons, 'h' for hadrons"<<endl;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
    else if(partiType=='b'){//b for both
      writeHadron     = true;
      writeParton     = true;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"In PythiaParticles, default used, write out both hadrons and partons"<<endl;
      cout<<"To choose particles to write out: 'p' for partons, 'h' for hadrons"<<endl;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
    else{
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
      cout<<"!!!!  Invalid option in PythiaParticles(..., char particleType)  !!!!"<<endl;
      cout<<"Default is used, write out both partons and hadrons"<<endl;
      cout<<"To choose particles to write out: 'p' for partons, 'h' for hadrons"<<endl;
      cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    writeHadron     = true;
    writeParton     = true;
    }
  
}

void PythiaParticles::InitTrees()
{
  hEvents = new TH1F("hEvents","number of events", 2, 0,2);
  if(writeHadron){
    tPythiaHadron = new TTree("tPythiaHadron", "Pythia simulated hadrons");
    tPythiaHadron -> Branch("tPythiaHadron", &pythia_hadrons);
  }
  if(writeParton){
    tPythiaParton = new TTree("tPythiaParton", "Pythia simulated partons");
    tPythiaParton -> Branch("tPythiaParton", &pythia_partons);
  }
  
  tPythiaEvent = new TTree("tPythiaEvent", "PythiaEvent");
  tPythiaEvent->Branch("event_x1", &event_x1, "event_x1/F");
  tPythiaEvent->Branch("event_x2", &event_x2, "event_x2/F");
  tPythiaEvent->Branch("event_Qsqr", &event_Qsqr, "event_Qsqr/F");
  tPythiaEvent->Branch("event_processid", &event_processid, "event_processid/I");
  
  return;
}

void PythiaParticles::FillTrees()
{
  if(writeHadron){
    tPythiaHadron -> Fill();
  }
  if(writeParton){
    tPythiaParton -> Fill();
  }
  tPythiaEvent -> Fill();

  return ;
}

int PythiaParticles::Init(PHCompositeNode *topNode)
{
  nTotalEvents    = 0;
  nTotalHadrons   = 0;
  nTotalPartons   = 0;
  return EVENT_OK;
}

int PythiaParticles::InitRun(PHCompositeNode *topNode)
{
    return EVENT_OK;
}

int PythiaParticles::ResetEvent(PHCompositeNode *topNode)
{
    pythia_hadrons.clear();
    pythia_partons.clear();

    return EVENT_OK;
}

int PythiaParticles::process_event(PHCompositeNode *topNode)
{
    if(writeHadron){
      GetHadrons(topNode, pythia_hadrons);
    } 
    if(writeParton){
      GetPartons(topNode, pythia_partons);
    } 
    nTotalEvents++;
    FillTrees();
    return EVENT_OK;
}

void PythiaParticles::GetHadrons(PHCompositeNode *topNode,
                                      std::vector<pythiaHadron>& hadron_list)
{
    PHPythiaContainer *phpythia = getClass<PHPythiaContainer> (topNode, "PHPythia");
    if (!phpythia)
        {
            cout << "No PHPythia! No sense continuing" << endl;
            exit(1);
        }
    PHPythiaHeader *phpythiaheader = findNode::getClass<PHPythiaHeader>(topNode, "PHPythiaHeader");
    if (!phpythiaheader)
        {
            cout << "No PHPythiaHeader! No sense continuing" << endl;
            exit(1);
        }
   
    event_x1   = phpythiaheader->GetX1();
    event_x2   = phpythiaheader->GetX2();
    event_Qsqr = phpythiaheader->GetX2();
    event_processid  = phpythiaheader->GetProcessid();

    int npart = phpythia->size();
    for (int ipart = 0; ipart < npart; ipart++) {
      TMCParticle *part = phpythia->getParticle(ipart);

      //Look for final state particles
      bool passKS = (part->GetKS() == 1);
      if(!passKS)continue;
      //Ignore all beam remnants
      bool isBeamRemnants = (sqrt(pow(part->GetPx(), 2) + pow(part->GetPy(), 2)) == 0.0);
      if(isBeamRemnants)continue;
                //KF- flavor

      int kf = part->GetKF();
      
      float energy = part->GetEnergy();
      TLorentzVector *fVector = new TLorentzVector(part->GetPx(), part->GetPy(), part->GetPz(), energy);
      float eta     = fVector->Eta();
      float theta   = fVector->Theta();
      float mom     = fVector->P();
      float pT      = mom * sin(theta);
      float phi     = phiReduce(fVector->Phi());

      TPythia6 *tpythia6 = new TPythia6();
      float charge  = tpythia6->Pychge(kf);
      //convert pythia charge to physical charge
      delete tpythia6;
      
      pythiaHadron temp;
      temp.charge      = charge/3.0;
      temp.energy      = energy;
      temp.mom         = mom;
      temp.pT          = pT;
      temp.px          = part->GetPx();
      temp.py          = part->GetPy();
      temp.pz          = part->GetPz();
      temp.eta         = eta;
      temp.phi         = phi;
      temp.kf          = kf;
      
      bool isNeutrino  = ((kf == PY_NU_E)   || (kf == -PY_NU_E)  ||
                          (kf == PY_NU_MU)  || (kf == -PY_NU_MU) ||
                          (kf == PY_NU_TAU) || (kf == -PY_NU_TAU));

      bool isMuon      = ((kf == PY_MU)     || (kf == -PY_MU));
     
      temp.isNeutrino  = isNeutrino;
      temp.isMuon      = isMuon;
     
      hadron_list.push_back(temp);
      nTotalHadrons++; 
  }//end npart
}//end GetHadrons

void PythiaParticles::GetPartons(PHCompositeNode *topNode,
                                      std::vector<pythiaParton>& parton_list)
{
    PHPythiaContainer *phpythia = getClass<PHPythiaContainer> (topNode, "PHPythia");
    if (!phpythia)
        {
            cout << "No PHPythia! No sense continuing" << endl;
            exit(1);
        }
    PHPythiaHeader *phpythiaheader = findNode::getClass<PHPythiaHeader>(topNode, "PHPythiaHeader");
    if (!phpythiaheader)
        {
            cout << "No PHPythiaHeader! No sense continuing" << endl;
            exit(1);
        }
    
    int npart = phpythia->size();
    for (int ipart = 8; ipart < npart; ipart++) { // note starting point!!!
            
      TMCParticle *part = phpythia->getParticle(ipart);
	    
      //Ignore all beam remnants
      bool passBeamRemnants = (sqrt(pow(part->GetPx(), 2) + pow(part->GetPy(), 2)) != 0.0);
      if(!passBeamRemnants) continue; 
	    
      bool isInitialParton = (part->GetParent() < 9); // parton shower
      if(!isInitialParton)continue;

      //KF- flavor
      int kf = part->GetKF();

      float energy = part->GetEnergy();
      TLorentzVector *fVector = new TLorentzVector(part->GetPx(), part->GetPy(), part->GetPz(), energy);
      float eta   = fVector->Eta();
      float theta = fVector->Theta();
      float mom   = fVector->P();
      float pT    = mom * sin(theta);
      float phi   = phiReduce(fVector->Phi());

      TPythia6 *tpythia6 = new TPythia6();
      float charge = tpythia6->Pychge(kf);
      delete tpythia6;

      pythiaParton temp;
      temp.charge      = charge/3.0;
      temp.energy      = energy;
      temp.mom         = mom;
      temp.pT          = pT;
      temp.px          = part->GetPx();
      temp.py          = part->GetPy();
      temp.pz          = part->GetPz();
      temp.eta         = eta;
      temp.phi         = phi;
      temp.kf          = kf;

      parton_list.push_back(temp);
      nTotalPartons++; 
  }//end npart
}//GetPartons

int PythiaParticles::EndRun(const int runNumber)
{
    return EVENT_OK;
}

int PythiaParticles::End(PHCompositeNode *topNode)
{
    hEvents->SetBinContent(1, nTotalEvents);
    cout << endl;
    cout << "++++++++++++++++++++++++ PythiaParticles.C ++++++++++++++++++++++++++++" << endl;
    cout << "Total events processed:               " << nTotalEvents << endl;
    cout << "Total hadrons in TTree:               " << nTotalHadrons << endl;
    cout << "Total partons in TTree:               " << nTotalPartons << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

    outfile -> Write();
    outfile -> Close();
    return EVENT_OK;
}
