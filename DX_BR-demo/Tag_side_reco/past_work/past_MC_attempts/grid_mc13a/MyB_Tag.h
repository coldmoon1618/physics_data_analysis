#ifndef MY_B_TAG
#define MY_B_TAG

struct MyB_Tag{

    int nCand;
    static const int nPart = 9999;

    double isSignal[nPart];
    double Mbc[nPart];
    double deltaE[nPart];
    double SigProb[nPart];
    double decMode[nPart];

    double isSigAccMiss[nPart];
    double isSigAccMissGamma[nPart];
    double isSigAccWrongFSP[nPart];

    void setBranchAddresses(TTree *B_Tag){
        B_Tag -> SetBranchAddress("__ncandidates__",           &nCand);
        B_Tag -> SetBranchAddress("isSignal",                   isSignal);
        B_Tag -> SetBranchAddress("Mbc",                        Mbc);
        B_Tag -> SetBranchAddress("deltaE",                     deltaE);
        B_Tag -> SetBranchAddress("SigProb",                    SigProb);
        B_Tag -> SetBranchAddress("decMode",                    decMode);

        B_Tag -> SetBranchAddress("isSignalAcceptMissing",      isSigAccMiss);
        B_Tag -> SetBranchAddress("isSignalAcceptMissingGamma", isSigAccMissGamma);
        B_Tag -> SetBranchAddress("isSignalAcceptWrongFSPs",    isSigAccWrongFSP);
    }
};
#endif

