#include <iostream>

void DumpFlatFilePhisKs(TString inFileName="dummy.root",
		       TString flatFileName="dummy.text") {
  
  gROOT->Reset();
  
  // variables
  Int_t experiment, run, event, candidate, customIsSig;
  Double_t isSignal, Mbc, deltaE, B0_phi_M, phiCosHel, csv_FastBDT, DeltaT, DeltaTErr;
  
  
  // open file and set tree
  
  TFile *inFile = new TFile(inFileName);
  TTree *inTree = (TTree*)inFile->Get("outTree");

  inTree->SetBranchAddress("__experiment__", &experiment);
  inTree->SetBranchAddress("__run__", &run);
  inTree->SetBranchAddress("__event__", &event);
  inTree->SetBranchAddress("__candidate__", &candidate);
  
  inTree->SetBranchAddress("isSignal", &isSignal);
  inTree->SetBranchAddress("Mbc", &Mbc);
  inTree->SetBranchAddress("deltaE", &deltaE);
  inTree->SetBranchAddress("B0_phi_M", &B0_phi_M);
  inTree->SetBranchAddress("phiCosHel", &phiCosHel);
  inTree->SetBranchAddress("csv_FastBDT", &csv_FastBDT);
  inTree->SetBranchAddress("DeltaT", &DeltaT);
  inTree->SetBranchAddress("DeltaTErr", &DeltaTErr);
  inTree->SetBranchAddress("customIsSig", &customIsSig);
  

  // define out stream
    
  ofstream outStream;
  outStream.open(flatFileName);


  // loop on entries and dump data

  for (int iev = 0; iev < inTree->GetEntries(); iev++) {

    inTree->GetEntry(iev);
    
    outStream << fixed << setprecision(6)
	      << experiment << "\t"
	      << run << "\t"
	      << event << "\t"
	      << candidate << "\t"
	      << isSignal << "\t"
	      << Mbc << "\t"
	      << deltaE << "\t"
	      << B0_phi_M << "\t"
	      << phiCosHel << "\t"
	      << csv_FastBDT << "\t"
	      << DeltaT << "\t"
	      << DeltaTErr << "\t"
	      << customIsSig
	      << endl;
		 
  }

  outStream.close();   

      
  return;

}
