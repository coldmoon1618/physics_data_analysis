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
void cutAndFit() {

    TChain *KS0s = new TChain("tree");
    KS0s -> AddFile("MC13a_KS_compareDistance.root");

    int KEntries;
    KEntries = KS0s -> GetEntries();
    cout<<"total entries: "<<KEntries<<endl;

    double  M;
    KS0s->SetBranchAddress("M",&M);
    double  flightDistance;
    KS0s -> SetBranchAddress("flightDistance", &flightDistance);
    double  flightDistanceErr;
    KS0s -> SetBranchAddress("flightDistanceErr", &flightDistanceErr);
    double  sig_2D =-9;
    double  sig_3D;
    KS0s -> SetBranchAddress("significanceOfDistance", &sig_3D);
    double  isSignal;
    KS0s -> SetBranchAddress("isSignal", &isSignal);

    TFile fOut("MC13a_sigL_cut.root","recreate");
    const int nCuts = 10;
    TTree *tree_2D[nCuts];
    TTree *tree_3D[nCuts];
    double cuts[nCuts] = {-999 , .5, 1.5, 2.5, 3., 3.5, 4., 4.5, 5.,6.};
    double nPar_2D[nCuts] = { 0,  0,   0,   0,  0,   0,  0,   0,  0, 0};
    double nPar_3D[nCuts] = { 0,  0,   0,   0,  0,   0,  0,   0,  0, 0};
    double M_2D[nCuts];
    double M_3D[nCuts];
    for(int iCut=0; iCut<nCuts; iCut++){
        tree_2D[iCut]= new TTree(Form("tree_2D_%i",iCut), "cut and save tree_2D");
        tree_2D[iCut]->Branch("M",&M_2D[iCut],"M/D");
        tree_3D[iCut]= new TTree(Form("tree_3D_%i",iCut), "cut and save tree_3D");
        tree_3D[iCut]->Branch("M",&M_3D[iCut],"M/D");
    }

    TTree *unMatched = new TTree("unMatched","");
    double sigM;
    unMatched -> Branch("M",&sigM,"M/D");

    for(int i=0; i<KEntries; i++)
    //for(int i=0; i<99; i++)
    {
        KS0s -> GetEntry(i);
        if(M<=0.46||M>=0.54)continue;
        for(int iCut=0; iCut<nCuts; iCut++){
            sig_2D = flightDistance/flightDistanceErr;
            if(sig_2D>=cuts[iCut]){
                M_2D[iCut] = M;
                nPar_2D[iCut]++;
                tree_2D[iCut] -> Fill();
                //cout<<"iCut="<<iCut<<", "<<i<<"th Entry, sigLxy="<<sig_2D<<endl;
                //tree_2D[iCut] -> Scan();
            }//2D_cut
            if(sig_3D>=cuts[iCut]){
                M_3D[iCut] = M;
                nPar_3D[iCut]++;
                tree_3D[iCut] -> Fill();
            }//3D_cut
        }//iCut
        if(!isSignal)
        {
            sigM = M;
            unMatched -> Fill();
        }
    }//KEntries
    fOut.cd();
    for(int iCut=0; iCut<nCuts; iCut++){
        cout<<"nPar_2D="<<nPar_2D[iCut]<<endl;
        tree_2D[iCut] -> Print();
        tree_2D[iCut] -> Write();
        cout<<"nPar_3D="<<nPar_3D[iCut]<<endl;
        tree_3D[iCut] -> Print();
        tree_3D[iCut] -> Write();
    }
    unMatched -> Print();
    unMatched -> Write();

    fOut.Close();

    return ;
}
