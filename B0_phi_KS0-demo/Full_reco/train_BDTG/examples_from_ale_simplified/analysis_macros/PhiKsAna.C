#define PhiKsAna_cxx
#include "PhiKsAna.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void PhiKsAna::Loop(bool isDebug = true,
		    bool isBlind = true,
		    bool onlyOneCand = false,
		    TString outRootFileName="testOutTree.root",
		    bool applyDataPID = false)
{

  // main selection cuts

  Float_t cutCosTBTO = 0.95;
  Float_t cutR2 = 0.5;
  Float_t cutDeltaE = 0.20;
  Float_t cutMbc = 5.25;
  Float_t cutMPhi_low = 1.00;
  Float_t cutMPhi_high = 1.05;
  Float_t cutMKs_low  = 0.485;
  Float_t cutMKs_high = 0.51;

  Float_t cutKs_flLen = 50;
  
  Float_t cutTrkdr = 0.5;
  Float_t cutTrkdz = 2.0;
  
  Float_t cutPIDPhiK = 0.6;

  Float_t cutMbcBox = 5.27;
  Float_t cutDeltaEBox = 0.10;

  
  // additional variables
  
  Int_t prevEvt(-1);
  Int_t nCand = 0;
  Int_t nEvt = 0;


  // load data PID efficiency histograms

  TFile *inFilePID = new TFile("PID_corrections/Saved_PID_eff_histos.root");

  TH2F *hKpDataEff1 = (TH2F*)inFilePID->Get("hKpDataEff1");
  TH2F *hKpDataEff6 = (TH2F*)inFilePID->Get("hKpDataEff6");
  TH2F *hKmDataEff1 = (TH2F*)inFilePID->Get("hKmDataEff1");
  TH2F *hKmDataEff6 = (TH2F*)inFilePID->Get("hKmDataEff6");
  
  Int_t myBinPhiK1(0), myBinPhiK2(0);

  
  // define output tree

  TFile *outTreeFile = new TFile(outRootFileName, "recreate");
  outTreeFile->cd();

  TTree *outTree = new TTree("outTree", "events passing the selection");

  outTree->Branch("__experiment__", &__experiment__, "__experiment__/I");
  outTree->Branch("__run__", &__run__, "__run__/I");
  outTree->Branch("__event__", &__event__, "__event__/I");
  outTree->Branch("__candidate__", &__candidate__, "__candidate__/I");
  outTree->Branch("__ncandidates__", &__ncandidates__, "__ncandidates__/I");
  outTree->Branch("isSignal", &isSignal, "isSignal/D");
  outTree->Branch("customIsSig", &customIsSig, "customIsSig/I");
  outTree->Branch("Mbc", &Mbc, "Mbc/D");
  outTree->Branch("deltaE", &deltaE, "deltaE/D");
  outTree->Branch("foxWolframR2", &foxWolframR2, "foxWolframR2/D");
  outTree->Branch("sphericity", &sphericity, "sphericity/D");
  outTree->Branch("aplanarity", &aplanarity, "aplanarity/D");
  outTree->Branch("thrust", &thrust, "thrust/D");
  outTree->Branch("cosTBTO", &cosTBTO, "cosTBTO/D");
  outTree->Branch("DeltaT", &DeltaT, "DeltaT/D");
  outTree->Branch("DeltaTErr", &DeltaTErr, "DeltaTErr/D");
  outTree->Branch("TagVx", &TagVx, "TagVx/D");
  outTree->Branch("TagVxErr", &TagVxErr, "TagVxErr/D");
  outTree->Branch("TagVy", &TagVy, "TagVy/D");
  outTree->Branch("TagVyErr", &TagVyErr, "TagVyErr/D");
  outTree->Branch("TagVz", &TagVz, "TagVz/D");
  outTree->Branch("TagVzErr", &TagVzErr, "TagVzErr/D");
  outTree->Branch("csv_FastBDT2", &csv_FastBDT2, "csv_FastBDT2/D");
  outTree->Branch("B0_phi_M", &B0_phi_M, "B0_phi_M/D");
  outTree->Branch("B0_phi_isSignal", &B0_phi_isSignal, "B0_phi_isSignal/D");
  outTree->Branch("B0_phi_K1_charge", &B0_phi_K1_charge, "B0_phi_K1_charge/D");
  outTree->Branch("B0_phi_K1_kaonBID", &B0_phi_K1_kaonID, "B0_phi_K1_kaonBID/D");
  outTree->Branch("B0_phi_K2_charge", &B0_phi_K2_charge, "B0_phi_K2_charge/D");
  outTree->Branch("B0_phi_K2_kaonBID", &B0_phi_K2_kaonID, "B0_phi_K2_kaonBID/D");
  outTree->Branch("B0_Ks_M", &B0_Ks_M, "B0_Ks_M/D");
  outTree->Branch("B0_Ks_isSignal", &B0_Ks_isSignal, "B0_Ks_isSignal/D");
  outTree->Branch("B0_Ks_significanceOfDistance", &B0_Ks_significanceOfDistance, "B0_Ks_significanceOfDistance/D");
  outTree->Branch("phiCosHel", &phiCosHel, "phiCosHel/D");


  if (fChain == 0) return;
  Int_t nentries = fChain->GetEntries();
  if ( isDebug ) nentries = 1000;

  cout << "Number of events in the chain = " << nentries << "\n" << endl ;

  
  
  // loop on entries
  for (Int_t jentry=0; jentry<nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;

    fChain->GetEntry(jentry);
    if ( jentry%100000 == 0 ) cout << "Analyzing event " << jentry << endl;

    // select only one candidate per event
    // candidates are already ranked at basf2 level
    if ( onlyOneCand && ( __event__ == prevEvt ) && nCand > 0 ) continue;

    
    if ( __event__ != prevEvt ) {
      hNCand->Fill(nCand);
      nCand = 0;
    }
    prevEvt = __event__;


    // apply selection

    if ( fabs( B0_phi_K1_dr > cutTrkdr ) ) continue;
    if ( fabs( B0_phi_K2_dr > cutTrkdr ) ) continue;
    if ( fabs( B0_phi_K1_dz > cutTrkdz ) ) continue;
    if ( fabs( B0_phi_K2_dz > cutTrkdz ) ) continue;
    
    if ( cosTBTO > cutCosTBTO ) continue;
    if ( foxWolframR2 > cutR2 ) continue;
    if ( fabs(deltaE) > cutDeltaE ) continue;
    if ( Mbc < cutMbc ) continue;
    if ( B0_phi_M < cutMPhi_low ) continue;
    if ( B0_phi_M > cutMPhi_high ) continue;
    if ( B0_Ks_M < cutMKs_low ) continue;
    if ( B0_Ks_M > cutMKs_high ) continue;

    if ( B0_Ks_significanceOfDistance < cutKs_flLen ) continue;

    if ( !applyDataPID ) {
      if ( B0_phi_K1_kaonBID < cutPIDPhiK && B0_phi_K2_kaonBID < cutPIDPhiK ) continue;
    }
    else {

      // K1 is always negative, K2 is always positive
      
      myBinPhiK1 = hKmDataEff6->FindBin( B0_phi_K1_p, B0_phi_K1_pz/B0_phi_K1_p );
      myBinPhiK2 = hKpDataEff6->FindBin( B0_phi_K2_p, B0_phi_K2_pz/B0_phi_K2_p );

      if ( gRandom->Uniform(0, 1) > hKmDataEff6->GetBinContent(myBinPhiK1) && gRandom->Uniform(0, 1) > hKpDataEff6->GetBinContent(myBinPhiK2) ) continue; 

    }

    
    // remove signal box if blinding flag is on

    if ( isBlind && Mbc > cutMbcBox && fabs(deltaE) < cutDeltaEBox ) continue;
    

    // select or discard signal and nonRes events (based on the kind of study needed)

    //if ( !(isSignal || customIsSig) ) continue;
    //if ( customIsSig ) continue;
    //if ( isSignal ) continue;
    //if ( isNonRes ) continue;
    
    
    // fill the output tree

    outTree->Fill();
   

    nCand++;
    nEvt++;
    
  }  // end loop on entries

  
  hNCand->Draw("hist");

  cout << "\n\n Total number of events selected = " << nEvt << endl;
  cout << " The average number of candidates is = " << hNCand->GetMean() << "\n\n" << endl; 

  cout << " Reweighted number of events = " << nEvt/(hNCand->GetMean()) << "\n\n" << endl; 

  outTree->Write();
  outTreeFile->Close();
  
}
