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

void EstimateYield(bool sameCut=1){

//Before exclusion
    /*
    //const int nModes = 36;
    double nSig[nModes]={
    86967, 72377,  1792,
    73548, 20878,     0,
    10834,     0,     0,//0-8 

        0,     0,     0, 
        0,     0, 13303, 
    53528, 44771,  2012,//9-17 

    27345,  5859,  6275, 
     5904, 22221, 10834, 
     3178,  5931,  4275,//18-26 

     2194,  1527,  1161, 
     2354,  1907,  1248, 
        0,     0,   868,//27-35 
    };
    double nErr[nModes]={
     1344,   934,   166, 
      931,   513,     0,
        0,     0,     0,//0-8

        0,     0,     0, 
        0,     0,   380, 
      714,   835,   221,//9-17 

      639,   338,   275, 
      257,   491,   423, 
      199,   334,   214,//18-26 

      128,    87,   117, 
      157,   173,   177, 
        0,     0,    92,//27-35 
    };
    */
    double nMode00, eMode00;
    if(sameCut){
        nMode00=80844;
        eMode00=931;
    }else{
        nMode00=86967;
        eMode00=1344;
    }
    const int nModes = 25;
    double nSig[nModes]={
  nMode00, 72377,  1792, 73548, 20878,
    13301, 53528, 44771,  2012, 27345,
     5859,  6275,  5904, 22221, 10834,
     3178,  5931,  4275,  2194,  1527,
     1161,  2354,  1097,  1248,   868,
    };
    double nErr[nModes]={
  eMode00,   934,   166,  1244,   513,
      385,   805,   835,   221,   639,
      338,   275,   257,   491,   423,
      199,   334,   214,   128,    87,
      117,   157,   173,   177,    92,
    };
    double totSig = 0.;
    double totErrSqr = 0.;
    for(int iMode = 0; iMode<nModes; iMode++){
        totSig      += nSig[iMode];
        totErrSqr   += nErr[iMode]*nErr[iMode];
    }
    cout<<"Before explicit exclusion"<<endl;
    cout<<totSig<<endl;
    cout<<sqrt(totErrSqr)<<endl;

    cout<<endl;
    cout<<"Per each ifb:"<<endl;
    cout<<floor(totSig/72.8)<<endl;
    cout<<"==============="<<endl;
    cout<<endl;
    cout<<endl;

//After exclusion
    const int n_modes = 25;
    double n_sig[nModes]={
    80844, 72376,  1793, 73531, 20877,
    13301, 53531, 44771,  2012, 27345,
     5859,  6275,  5904, 22221, 10834,
     3178,  5931,  4275,  2194,  1527,
     1161,  2354,  1097,  1248,   868,
    };
    double n_err[nModes]={
      931,   934,   166,  1244,   513,
      385,   807,   835,   221,   639,
      338,   275,   257,   491,   423,
      199,   334,   214,   128,    87,
      117,   157,   173,   177,    92,
    };
    double tot_sig = 0.;
    double tot_errSqr = 0.;
    for(int iMode = 0; iMode<n_modes; iMode++){
        tot_sig      += n_sig[iMode];
        tot_errSqr   += n_err[iMode]*nErr[iMode];
    }
    cout<<"After explicit exclusion"<<endl;
    cout<<tot_sig<<endl;
    cout<<sqrt(tot_errSqr)<<endl;

    cout<<endl;
    cout<<"Per each ifb:"<<endl;
    cout<<floor(tot_sig/72.8)<<endl;
    cout<<"==============="<<endl;
    cout<<endl;
    cout<<endl;

    return;
}

