#ifndef __PYTHIAPARTICLES_H__
#define __PYTHIAPARTICLES_H__

//Data classes I am using in analysis
#include <SubsysReco.h>
#include <PHGlobal.h>
#include <EventHeader.h>
#include <PreviousEvent.h>

#include <TPythia6.h>
#include <TMath.h>
#include <TLorentzVector.h>

class Fun4AllHistoManager;
class PHCompositeNode;
class PHPythiaContainer;
class TH1F;
class TFile;
class TTree;

//const float MM2CM = 0.1;

typedef struct
{
    float charge;
    float energy;
    float mom;
    float pT;
    float px;
    float py;
    float pz;
    float eta;
    float phi;

    //The PDG standard, with the local Pythia extensions
    // KF particle code
    int   kf;
    bool  isNeutrino;
    bool  isMuon;

} pythiaHadron;

typedef struct
{
    float charge;
    float energy;
    float mom;
    float pT;
    float px;
    float py;
    float pz;
    float eta;
    float phi;

    int   kf;

} pythiaParton;

class PythiaParticles: public SubsysReco
{
 public:
    //pass in output file name
    PythiaParticles(std::string outFileName);
    //Allows user to store 'p'arton only or 'h'adron only
    //Invalid arguments uses default: to store both
    PythiaParticles(const char particleType, std::string outFileName);

    virtual ~PythiaParticles() {}
    
    void SetParticleType(char partiType);
    //  For this analysis we only use following:
    int Init         (PHCompositeNode *topNode);
    int InitRun      (PHCompositeNode *topNode);
    int ResetEvent   (PHCompositeNode *topNode);
    int process_event(PHCompositeNode *topNode);
    int End          (PHCompositeNode *topNode);
    int EndRun       (const int runNumber);

    void GetHadrons(PHCompositeNode *topNode,
                   std::vector<pythiaHadron>& hadron_list);

    void GetPartons(PHCompositeNode *topNode,
                   std::vector<pythiaParton>& parton_list);
    void InitTrees();
    void FillTrees();

    float phiReduce(float phi)
    {
        if (phi < -TMath::PiOver2())
            {
                phi += TMath::TwoPi();
            }
        if (phi >= 3.0 * TMath::PiOver2())
            {
                phi -= TMath::TwoPi();
            }
        return phi;
    }

 protected:

    TH1F *hEvents;
    
    TFile *outfile;
    
    //counter for events already ran
    unsigned int nTotalEvents;
    //counter for hadrons produced
    unsigned int nTotalHadrons;
    //counter for partons produced
    unsigned int nTotalPartons;
    
    //boolean to store hadrons
    bool writeHadron;
    //boolean to store partons
    bool writeParton;
  
    TTree *tPythiaHadron;
    std::vector<pythiaHadron> pythia_hadrons;
    TTree *tPythiaParton;
    std::vector<pythiaParton> pythia_partons;
    TTree *tPythiaEvent;
    float event_x1;
    float event_x2;
    float event_Qsqr;
    int   event_processid;
};

#endif
