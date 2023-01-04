#ifndef MY_B0_MC
#define MY_B0_MC

struct MyB0MC{
    
    int nCand;
    static const int nPart = 9999;

    double mdstIndex[nPart];

    void setBranchAddresses(TTree *B0_MC){
        B0_MC -> SetBranchAddress("__ncandidates__", &nCand);
        B0_MC -> SetBranchAddress("mdstIndex", mdstIndex);
    }
};
#endif
