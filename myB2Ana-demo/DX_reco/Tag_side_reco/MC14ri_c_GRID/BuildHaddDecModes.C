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

void BuildHaddDecModes(){

    int intMode[25]={
        0,  1,  2,  3,  4,
        14, 15, 16, 17, 18,
        19, 20, 21, 22, 23,
        24, 25, 26, 27, 28,
        29, 30, 31, 32, 35
    };

    ofstream myfile;
    myfile.open("hadd_decModes.sh");

    myfile<<"mkdir decModes"<<endl;
    myfile<<endl;

    for(int iMode=0; iMode<25; iMode++){

        int myMode = intMode[iMode];

        myfile<<Form("mkdir mode_%02d",myMode)<<endl;
        myfile<<Form("ln mcTypes/mixed_output/Cand_mode_%02d.root mode_%02d/mixed_mode_%02d.root"     ,myMode,myMode,myMode)<<endl;
        myfile<<Form("ln mcTypes/charged_output/Cand_mode_%02d.root mode_%02d/charged_mode_%02d.root" ,myMode,myMode,myMode)<<endl;
        myfile<<Form("ln mcTypes/uubar_output/Cand_mode_%02d.root mode_%02d/uubar_mode_%02d.root"     ,myMode,myMode,myMode)<<endl;
        myfile<<Form("ln mcTypes/ddbar_output/Cand_mode_%02d.root mode_%02d/ddbar_mode_%02d.root"     ,myMode,myMode,myMode)<<endl;
        myfile<<Form("ln mcTypes/ssbar_output/Cand_mode_%02d.root mode_%02d/ssbar_mode_%02d.root"     ,myMode,myMode,myMode)<<endl;
        myfile<<Form("ln mcTypes/ccbar_output/Cand_mode_%02d.root mode_%02d/ccbar_mode_%02d.root"     ,myMode,myMode,myMode)<<endl;
        myfile<<Form("cd mode_%02d",myMode)<<endl;
        myfile<<"hadd -f Total.root *root"<<endl;
        myfile<<"cd ../."<<endl;
        myfile<<Form("mv mode_%02d decModes/.",myMode)<<endl;
        myfile<<endl;

    }

    return;
}
