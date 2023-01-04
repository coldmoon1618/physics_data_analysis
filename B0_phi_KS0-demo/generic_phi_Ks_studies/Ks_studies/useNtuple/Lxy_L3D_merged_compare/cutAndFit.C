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

#include "MyKsRec.h"
void cutAndFit() {

    TString fileName("/home/belle2/zhai/B0_phi_KS0/generic_phi_Ks_studies/Ks_studies/getNtuple/Combo/Combo_Ks_selection.root");

    TChain *Ks_Rec = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec *recKs = new MyKsRec(); 
    recKs   -> setBranchAddresses(Ks_Rec);
    int Entries = Ks_Rec -> GetEntries();
    cout<<Entries<<" events"<<endl;

    TFile fOut("Combo_sigL_cut.root","recreate");
    const int nCuts = 10;
    TTree *tree_2D[nCuts];
    TTree *tree_3D[nCuts];
    double cuts[nCuts] = {-999 , .5, 1.5, 2.5, 3., 3.5, 4., 4.5, 5.,6.};
    //number of candidates passed cuts
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

    for(int i=0; i<Entries; i++)
    {
        Ks_Rec -> GetEntry(i);
        //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recKs->nCand; iEnt++){
            double M_Ks   = recKs->M[iEnt];
            //cout<<recKs->flightDistance[iEnt]<<" "<<recKs->flightDistanceErr[iEnt]<<" "<<recKs->significanceOfDistance[iEnt]<<endl;
            double sig_2D = recKs->flightDistance[iEnt]/recKs->flightDistanceErr[iEnt];
            double sig_3D = recKs->significanceOfDistance[iEnt];
            if(M_Ks<=0.46||M_Ks>=0.54)continue;
            for(int iCut=0; iCut<nCuts; iCut++){
                if(sig_2D>=cuts[iCut]){
                    M_2D[iCut] = M_Ks;
                    nPar_2D[iCut]++;
                    tree_2D[iCut] -> Fill();
                }//2D_cut
                if(sig_3D>=cuts[iCut]){
                    M_3D[iCut] = M_Ks;
                    nPar_3D[iCut]++;
                    tree_3D[iCut] -> Fill();
                }//3D_cut
            }//cuts
        }//cands
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
    fOut.Close();

    return ;
}
