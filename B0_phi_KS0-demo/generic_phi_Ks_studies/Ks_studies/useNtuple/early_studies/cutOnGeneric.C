#include <iostream>
#include <iostream>
#include <fstream>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <TMath.h>
#include <TChain.h>
void cutOnGeneric() {

    TChain *KS0s = new TChain("tree");
    KS0s -> AddFile("MC13a_reco_200k.root");

    int KEntries;
    KEntries = KS0s -> GetEntries();
    cout<<"total entries: "<<KEntries<<endl;

    double  M;
    KS0s->SetBranchAddress("M",&M);
    double  p;
    KS0s->SetBranchAddress("p",&p);
    double  flightDistance;
    KS0s -> SetBranchAddress("flightDistance", &flightDistance);
    double  flightDistanceErr;
    KS0s -> SetBranchAddress("flightDistanceErr", &flightDistanceErr);
    double  sig_2D =-9;

    TFile fOut("MC13a_generic_cut.root","recreate");
    TTree *tree = new TTree("tree","");
    double mass;
    tree -> Branch("M",&mass,"M/D");

    for(int i=0; i<KEntries; i++)
    {
        KS0s -> GetEntry(i);
        if(p<1.8)continue;
        if(M<=0.46||M>=0.54)continue;
        sig_2D = flightDistance/flightDistanceErr;
        if(sig_2D<3)continue;
        mass = M;
        tree -> Fill();
    }//KEntries
    fOut.cd();

    tree -> Print();
    tree -> Write();

    fOut.Close();

    return ;
}
