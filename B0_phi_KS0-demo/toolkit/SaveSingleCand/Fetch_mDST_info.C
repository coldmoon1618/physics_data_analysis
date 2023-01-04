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

void Fetch_mDST_info(int ele = 1) {
    // Directly fetch MC truth info from mDDST
    // Save to event based ntuple to combine 
    // with reco ntuple during save cand stage
    Belle2::MCParticle *myParti = new Belle2::MCParticle();
    TClonesArray *arr = new TClonesArray("Belle2::MCParticle");

    TChain *mdstTree = new TChain("tree");
    mdstTree -> Add(Form("/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/%i/signalevents_2000.mdst.root",ele));

    TFile f(Form("mcDeltaTau/SigMC_mcDeT_%i.root",ele),"recreate");
    TTree *mcTree = new TTree("mcTree","");
    float mcDeltaTau;
    mcTree -> Branch("mcDeltaTau", &mcDeltaTau, "mcDeltaTau/F");

    int nEntries;
    nEntries = mdstTree -> GetEntries();
    mdstTree -> SetBranchAddress("MCParticles",&arr);

    for (int i=0;i<nEntries;i++) 
    {
        mdstTree -> GetEntry(i);
        
        //WARNING: Order of sig and tag B only applied to SigMC
        Belle2::MCParticle *SigB = (Belle2::MCParticle*)arr->At(1);
        Belle2::MCParticle *TagB = (Belle2::MCParticle*)arr->At(2);

        double Sigbeta = SigB->getMomentum().Mag() / SigB->getEnergy();
        double Tagbeta = TagB->getMomentum().Mag() / TagB->getEnergy();
        double Sig_tau = 1e3 * SigB->getLifetime() * sqrt( 1 - pow(Sigbeta,2) );
        double Tag_tau = 1e3 * TagB->getLifetime() * sqrt( 1 - pow(Tagbeta,2) );

        mcDeltaTau = Sig_tau - Tag_tau;
        mcTree -> Fill();
    }
    f.cd();
    mcTree -> Write();

}//end 
