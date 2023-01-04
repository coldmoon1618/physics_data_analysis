#ifndef MY_B_TAG
#define MY_B_TAG

struct MyB_Tag{

    int nCand;
    int evtNum;
    static const int nPart = 9999;

    double SigProb[nPart];
    double Mbc[nPart];
    double deltaE[nPart];
    double decMode[nPart];
    
    double p[nPart];
    double Ecms[nPart];

    void setBranchAddresses(TTree *B_Tag){
        B_Tag -> SetBranchAddress("__ncandidates__",&nCand);
        B_Tag -> SetBranchAddress("SigProb",        SigProb);
        B_Tag -> SetBranchAddress("Mbc",            Mbc);
        B_Tag -> SetBranchAddress("deltaE",         deltaE);
        B_Tag -> SetBranchAddress("decMode",        decMode);

        B_Tag -> SetBranchAddress("p",              p);
        B_Tag -> SetBranchAddress("Ecms",           Ecms);
    }
};
#endif

