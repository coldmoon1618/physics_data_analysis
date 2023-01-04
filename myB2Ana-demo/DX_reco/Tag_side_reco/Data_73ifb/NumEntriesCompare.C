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

// Compare number of entries in each decMode before and after
// explicit exclusion. They look exactly the same. Although the
// the fitting results alter every so slighter.
// This means that the exclusion of certain decModes didn't really 
// improve the existing ones to keep, but that's okay.
// At least for larger set it might shortens the computation?
// Dec 5, 2021
void NumEntriesCompare(){
    int intMode[25]={
        0,  1,  2,  3,  4,
        14, 15, 16, 17, 18,
        19, 20, 21, 22, 23,
        24, 25, 26, 27, 28,
        29, 30, 31, 32, 35
    };

    cout<<"Before exclusion:"<<endl;
    for(int iMode=0; iMode<25; iMode++){

        int myMode = intMode[iMode];

        TChain *B_cand = new TChain("B_cand");
        B_cand -> AddFile(Form("full_output/Cand_mode_%02d.root",myMode));

        int Entries = B_cand -> GetEntries();

        cout<<Entries<<", ";
        if((iMode+1)%5==0)cout<<endl;
    }
    cout<<endl<<endl;

    cout<<"After exclusion:"<<endl;
    for(int iMode=0; iMode<25; iMode++){

        int myMode = intMode[iMode];

        TChain *B_cand = new TChain("B_cand");
        B_cand -> AddFile(Form("output/Cand_mode_%02d.root",myMode));

        int Entries = B_cand -> GetEntries();

        cout<<Entries<<", ";
        if((iMode+1)%5==0)cout<<endl;
    }
    cout<<endl<<endl;
    return;
}
