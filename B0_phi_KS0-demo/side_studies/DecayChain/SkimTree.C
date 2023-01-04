//Only works in basf2 environment
//Compatible with release-04-02-01
//Some errors with unknow PDG, see err_UnknowPDG, although not hurting anything for my analysis so far
//May 20, 2020
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

#include <TChain.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>

void SkimTree() {
//Read in original mdst
    Belle2::MCParticle *myParti = new Belle2::MCParticle();
    TClonesArray *arr = new TClonesArray("Belle2::MCParticle");

    TChain *mdstTree = new TChain("tree");
    mdstTree -> Add("/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/0/signalevents_2000.mdst.root");
    //mdstTree -> Add("/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009545_task10020000001.root");

    int nEntries;
    nEntries = mdstTree -> GetEntries();
    mdstTree -> SetBranchAddress("MCParticles",&arr);

    const int kMaxParticles = 200;
    int     nParticles;
    int     dec_step[kMaxParticles];//starts with 1
    int     pdg[kMaxParticles];
    float   energy[kMaxParticles];//MeV
    float   px[kMaxParticles];//MeV
    float   py[kMaxParticles];//MeV
    float   pz[kMaxParticles];//MeV
    float   pt[kMaxParticles];//MeV
    float   t_prod[kMaxParticles];//ns
    float   v_prod_x[kMaxParticles];//cm
    float   v_prod_y[kMaxParticles];//cm
    float   v_prod_z[kMaxParticles];//cm
    float   lifetime[kMaxParticles];//ns
    float   t_dec[kMaxParticles];//ns
    float   v_dec_x[kMaxParticles];//cm
    float   v_dec_y[kMaxParticles];//cm
    float   v_dec_z[kMaxParticles];//cm
    int     mother[kMaxParticles];//dec_step
    for(int i=0; i<kMaxParticles; i++){mother[i]=0;}
    int     first_dau[kMaxParticles];//dec_step
    int     last_dau[kMaxParticles];//dec_step
    int     numDau[kMaxParticles];
    int     secPhysProc[kMaxParticles];
    int     seenInPXD[kMaxParticles];
    int     seenInSVD[kMaxParticles];
    int     seenInCDC[kMaxParticles];
    float   charge[kMaxParticles];
    /*      0 if the particle is primary. -1 if no information is found.
     *      For the details, see the Geant4 package:
     *      (*G4Track)->GetCreatorProcess()->GetProcessSubType()
     *      processes/electromagnetic/utils/include/G4EmProcessSubType.hh
     *      processes/hadronic/management/include/G4HadronicProcessType.hh
     *      processes/decay/include/G4DecayProcessType.hh
     */
    int     first_index=0;
    int     last_index=0;
        
    TFile f("SigMC_decayChain.root","recreate");
    //TFile f("MC13a_decayChain.root","recreate");
    TTree *mcTree = new TTree("mcTree","MC Particle info");
    mcTree->Branch("nParticles",&nParticles,"nParticles/I");
    mcTree->Branch("dec_step",dec_step,"dec_step[nParticles]/I");
    mcTree->Branch("pdg",pdg,"pdg[nParticles]/I");
    mcTree->Branch("energy",energy,"energy[nParticles]/F");
    mcTree->Branch("px",px,"px[nParticles]/F");
    mcTree->Branch("py",py,"py[nParticles]/F");
    mcTree->Branch("pz",pz,"pz[nParticles]/F");
    mcTree->Branch("pt",pt,"pt[nParticles]/F");
    mcTree->Branch("lifetime",lifetime,"lifetime[nParticles]/F");
    mcTree->Branch("t_prod",t_prod,"t_prod[nParticles]/F");
    mcTree->Branch("v_prod_x",v_prod_x,"v_prod_x[nParticles]/F");
    mcTree->Branch("v_prod_y",v_prod_y,"v_prod_y[nParticles]/F");
    mcTree->Branch("v_prod_z",v_prod_z,"v_prod_z[nParticles]/F");
    mcTree->Branch("t_dec",t_dec,"t_dec[nParticles]/F");
    mcTree->Branch("v_dec_x",v_dec_x,"v_dec_x[nParticles]/F");
    mcTree->Branch("v_dec_y",v_dec_y,"v_dec_y[nParticles]/F");
    mcTree->Branch("v_dec_z",v_dec_z,"v_dec_z[nParticles]/F");
    mcTree->Branch("mother",mother,"mother[nParticles]/I");
    mcTree->Branch("first_dau",first_dau,"first_dau[nParticles]/I");
    mcTree->Branch("last_dau",last_dau,"last_dau[nParticles]/I");
    mcTree->Branch("numDau",numDau,"numDau[nParticles]/I");
    mcTree->Branch("secPhysProc",secPhysProc,"secPhysProc[nParticles]/I");
    mcTree->Branch("seenInPXD",seenInPXD,"seenInPXD[nParticles]/I");
    mcTree->Branch("seenInSVD",seenInSVD,"seenInSVD[nParticles]/I");
    mcTree->Branch("seenInCDC",seenInCDC,"seenInCDC[nParticles]/I");
    mcTree->Branch("charge",charge,"charge[nParticles]/F");
    for(int i=0; i<kMaxParticles; i++){charge[i]=-9;}

    auto *pdg_obj = Belle2::EvtGenDatabasePDG::Instance();
    //cout << pdg_obj -> GetParticle("pi+")->PdgCode() <<endl;//-> GetName() <<endl;
    //https://root.cern.ch/doc/master/classTDatabasePDG.html?fbclid=IwAR117f6BbZRwNWqUYoXn0JZAMPPCLCzZv2gF4JZV-LXRa_RnQ9Y8vH_Xk40

    for (int i=0;i<nEntries;i++) 
    {
        mdstTree -> GetEntry(i);
        nParticles = arr->GetEntries();
        for(int j = 0; j< nParticles; j++){
            Belle2::MCParticle *particle = (Belle2::MCParticle*)arr->At(j);
            //auto *particle = (Belle2::MCParticle*)arr->At(j);
            dec_step[j] = j+0;
            pdg[j]      = particle->getPDG();
            //cout << pdg[j] << endl;
            energy[j]   = particle->getEnergy()*1000;
            px[j]       = particle->getMomentum().Px()*1000;
            py[j]       = particle->getMomentum().Py()*1000;
            pz[j]       = particle->getMomentum().Pz()*1000;
            pt[j]       = particle->getMomentum().Pt()*1000;
            lifetime[j] = particle->getLifetime();
            t_prod[j]   = particle->getProductionTime();
            v_prod_x[j] = particle->getProductionVertex().Px();
            v_prod_y[j] = particle->getProductionVertex().Py();
            v_prod_z[j] = particle->getProductionVertex().Pz();
            t_dec[j]    = particle->getDecayTime();
            v_dec_x[j]  = particle->getDecayVertex().Px();
            v_dec_y[j]  = particle->getDecayVertex().Py();
            v_dec_z[j]  = particle->getDecayVertex().Pz();
            first_index = particle->getFirstDaughter()-1;
            last_index  = particle->getLastDaughter()-1; 
            first_dau[j]= first_index;
            last_dau[j] = last_index;
            numDau[j]   = particle->getNDaughters();
            if(first_index>0){
                for(int ind=first_index; ind<=last_index;ind++){
                    mother[ind]=j+0;
                }
            }
            secPhysProc[j]  = particle->getSecondaryPhysicsProcess();
            seenInPXD[j]    = particle->hasSeenInDetector(Belle2::Const::PXD); 
            seenInSVD[j]    = particle->hasSeenInDetector(Belle2::Const::SVD); 
            seenInCDC[j]    = particle->hasSeenInDetector(Belle2::Const::CDC); 
            charge[j]       = particle->getCharge();
        }
        mcTree->Fill();
        if(i%100==0)cout<<i<<", ";
    }
    //mcTree->Print();
    f.cd();
    mcTree->Write();
}
