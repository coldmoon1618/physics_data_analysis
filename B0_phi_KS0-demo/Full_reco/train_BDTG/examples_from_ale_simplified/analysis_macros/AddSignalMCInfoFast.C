#include <TTree.h>
#include <TFile.h>
#include <TString.h>

#include <iostream>

using namespace std;

void AddSignalMCInfoFast(TString inFileName, TString treeName="Bp_ch1", TString option="PhiKs") {

    TFile *inFile = new TFile(inFileName, "update");

    // variables
    Double_t isSignal, deltaE,
             mcPDG, mothID, gdMothID, gdgdMothID, mothPDG, gdMothPDG, gdgdMothPDG;

    Double_t B0_phi_mcPDG, B0_phi_mothID, B0_phi_gdMothID, B0_phi_gdgdMothID, B0_phi_mothPDG, B0_phi_gdMothPDG, B0_phi_gdgdMothPDG,
             B0_phi_K1_mcPDG, B0_phi_K1_mothID, B0_phi_K1_gdMothID, B0_phi_K1_gdgdMothID, B0_phi_K1_mothPDG, B0_phi_K1_gdMothPDG, B0_phi_K1_gdgdMothPDG,
             B0_phi_K2_mcPDG, B0_phi_K2_mothID, B0_phi_K2_gdMothID, B0_phi_K2_gdgdMothID, B0_phi_K2_mothPDG, B0_phi_K2_gdMothPDG, B0_phi_K2_gdgdMothPDG,
             B0_Ks_mcPDG, B0_Ks_mothID, B0_Ks_gdMothID, B0_Ks_gdgdMothID, B0_Ks_mothPDG, B0_Ks_gdMothPDG, B0_Ks_gdgdMothPDG,
             B0_Ks_pi1_mcPDG, B0_Ks_pi1_mothID, B0_Ks_pi1_gdMothID, B0_Ks_pi1_gdgdMothID, B0_Ks_pi1_mothPDG, B0_Ks_pi1_gdMothPDG, B0_Ks_pi1_gdgdMothPDG, 
             B0_Ks_pi2_mcPDG, B0_Ks_pi2_mothID, B0_Ks_pi2_gdMothID, B0_Ks_pi2_gdgdMothID, B0_Ks_pi2_mothPDG, B0_Ks_pi2_gdMothPDG, B0_Ks_pi2_gdgdMothPDG;

    TTree *myTree = (TTree*)inFile->Get(treeName);

    // check if the signalMC info branch already exists, if so return
    TBranch *myBranch = (TBranch*)myTree->GetListOfBranches()->FindObject("isNonRes");

    if (myBranch) {
        cout << "\nApparently the isNonRes branch is already there! Exiting... \n" << endl ;
        return;
    }
    else {
        cout << "\nCreating and setting the isNonRes branch for " << treeName << "\n" << endl ;
    }

    // "generic branches"
    myTree->SetBranchAddress("isSignal", &isSignal);
    myTree->SetBranchAddress("deltaE", &deltaE);
    myTree->SetBranchAddress("mcPDG", &mcPDG);
    myTree->SetBranchAddress("mothID", &mothID);
    myTree->SetBranchAddress("gdMothID", &gdMothID);
    myTree->SetBranchAddress("gdgdMothID", &gdgdMothID);
    myTree->SetBranchAddress("mothPDG", &mothPDG);
    myTree->SetBranchAddress("gdMothPDG", &gdMothPDG);
    myTree->SetBranchAddress("gdgdMothPDG", &gdgdMothPDG);

    // branches specific to decay modes
    if ( option == "PhiKs" ) {

        myTree->SetBranchAddress("B0_phi_mcPDG", &B0_phi_mcPDG);
        myTree->SetBranchAddress("B0_phi_mothID", &B0_phi_mothID);
        myTree->SetBranchAddress("B0_phi_gdMothID", &B0_phi_gdMothID);
        myTree->SetBranchAddress("B0_phi_gdgdMothID", &B0_phi_gdgdMothID);
        myTree->SetBranchAddress("B0_phi_mothPDG", &B0_phi_mothPDG);
        myTree->SetBranchAddress("B0_phi_gdMothPDG", &B0_phi_gdMothPDG);
        myTree->SetBranchAddress("B0_phi_gdgdMothPDG", &B0_phi_gdgdMothPDG);

        myTree->SetBranchAddress("B0_phi_K1_mcPDG", &B0_phi_K1_mcPDG);
        myTree->SetBranchAddress("B0_phi_K1_mothID", &B0_phi_K1_mothID);
        myTree->SetBranchAddress("B0_phi_K1_gdMothID", &B0_phi_K1_gdMothID);
        myTree->SetBranchAddress("B0_phi_K1_gdgdMothID", &B0_phi_K1_gdgdMothID);
        myTree->SetBranchAddress("B0_phi_K1_mothPDG", &B0_phi_K1_mothPDG);
        myTree->SetBranchAddress("B0_phi_K1_gdMothPDG", &B0_phi_K1_gdMothPDG);
        myTree->SetBranchAddress("B0_phi_K1_gdgdMothPDG", &B0_phi_K1_gdgdMothPDG);

        myTree->SetBranchAddress("B0_phi_K2_mcPDG", &B0_phi_K2_mcPDG);
        myTree->SetBranchAddress("B0_phi_K2_mothID", &B0_phi_K2_mothID);
        myTree->SetBranchAddress("B0_phi_K2_gdMothID", &B0_phi_K2_gdMothID);
        myTree->SetBranchAddress("B0_phi_K2_gdgdMothID", &B0_phi_K2_gdgdMothID);
        myTree->SetBranchAddress("B0_phi_K2_mothPDG", &B0_phi_K2_mothPDG);
        myTree->SetBranchAddress("B0_phi_K2_gdMothPDG", &B0_phi_K2_gdMothPDG);
        myTree->SetBranchAddress("B0_phi_K2_gdgdMothPDG", &B0_phi_K2_gdgdMothPDG);

        myTree->SetBranchAddress("B0_Ks_mcPDG", &B0_Ks_mcPDG);
        myTree->SetBranchAddress("B0_Ks_mothID", &B0_Ks_mothID);
        myTree->SetBranchAddress("B0_Ks_gdMothID", &B0_Ks_gdMothID);
        myTree->SetBranchAddress("B0_Ks_gdgdMothID", &B0_Ks_gdgdMothID);
        myTree->SetBranchAddress("B0_Ks_mothPDG", &B0_Ks_mothPDG);
        myTree->SetBranchAddress("B0_Ks_gdMothPDG", &B0_Ks_gdMothPDG);
        myTree->SetBranchAddress("B0_Ks_gdgdMothPDG", &B0_Ks_gdgdMothPDG);

        myTree->SetBranchAddress("B0_Ks_pi1_mcPDG", &B0_Ks_pi1_mcPDG);
        myTree->SetBranchAddress("B0_Ks_pi1_mothID", &B0_Ks_pi1_mothID);
        myTree->SetBranchAddress("B0_Ks_pi1_gdMothID", &B0_Ks_pi1_gdMothID);
        myTree->SetBranchAddress("B0_Ks_pi1_gdgdMothID", &B0_Ks_pi1_gdgdMothID);
        myTree->SetBranchAddress("B0_Ks_pi1_mothPDG", &B0_Ks_pi1_mothPDG);
        myTree->SetBranchAddress("B0_Ks_pi1_gdMothPDG", &B0_Ks_pi1_gdMothPDG);
        myTree->SetBranchAddress("B0_Ks_pi1_gdgdMothPDG", &B0_Ks_pi1_gdgdMothPDG);

        myTree->SetBranchAddress("B0_Ks_pi2_mcPDG", &B0_Ks_pi2_mcPDG);
        myTree->SetBranchAddress("B0_Ks_pi2_mothID", &B0_Ks_pi2_mothID);
        myTree->SetBranchAddress("B0_Ks_pi2_gdMothID", &B0_Ks_pi2_gdMothID);
        myTree->SetBranchAddress("B0_Ks_pi2_gdgdMothID", &B0_Ks_pi2_gdgdMothID);
        myTree->SetBranchAddress("B0_Ks_pi2_mothPDG", &B0_Ks_pi2_mothPDG);
        myTree->SetBranchAddress("B0_Ks_pi2_gdMothPDG", &B0_Ks_pi2_gdMothPDG);
        myTree->SetBranchAddress("B0_Ks_pi2_gdgdMothPDG", &B0_Ks_pi2_gdgdMothPDG);

    }
    else {
        cout << "\nInvalid option! Exiting... \n" << endl;
        return;
    }

    // add new branch(es) and loop on entries

    Int_t isNonRes(0);
    Int_t customIsSig(0);

    TBranch *b_isNonRes = myTree->Branch("isNonRes", &isNonRes, "isNonRes/I");
    TBranch *b_customIsSig = myTree->Branch("customIsSig", &customIsSig, "customIsSig/I");

    Long64_t nentries = myTree->GetEntriesFast();

    for (Long64_t jentry=0; jentry<nentries; jentry++) {

        myTree->GetEntry(jentry);

        isNonRes = 1;
        customIsSig = 1;

        if ( isSignal > 0.5 ) isNonRes = 0;

        if ( option == "PhiKs" ) {

            if ( fabs(mcPDG) != 511 ) isNonRes = 0; 
            if ( B0_phi_mcPDG != 333 && B0_phi_mcPDG != 0 ) customIsSig = 0;
            if ( B0_Ks_mcPDG != 310 && B0_Ks_mcPDG != 0 ) customIsSig = 0;
            if ( fabs(B0_phi_mcPDG) == 333 ) isNonRes = 0;
            if ( fabs(B0_phi_K1_mcPDG) != 321 ) isNonRes = 0; 
            if ( fabs(B0_phi_K2_mcPDG) != 321 ) isNonRes = 0;
            if ( fabs(B0_Ks_mcPDG) != 310 ) isNonRes = 0;

            if ( B0_phi_K1_mothID != B0_phi_K2_mothID ) isNonRes = 0;
            if ( B0_Ks_pi1_mothID != B0_Ks_pi2_mothID ) isNonRes = 0;
            if ( fabs(B0_Ks_mothPDG) != 511 ) isNonRes = 0;


            // two cases: either the K+K- come from the B, or they come from an intermediate object
            if ( !(fabs(B0_phi_K1_mothPDG) == 511 && B0_phi_K1_mothID == B0_Ks_mothID) &&
                    !(fabs(B0_phi_K1_gdMothPDG) == 511 && B0_phi_K1_gdMothID == B0_Ks_mothID) ) isNonRes = 0;

            // final cut on deltaE (to account for missing pions etc...)
            if ( isNonRes == 1 && fabs(deltaE) > 0.05 ) isNonRes = 0;

            b_isNonRes->Fill();
            b_customIsSig->Fill();

        }// end of "PhiKs" option

    }// end loop on entries

    myTree->Write("", TObject::kOverwrite);
    inFile->Close();

    return;
}
