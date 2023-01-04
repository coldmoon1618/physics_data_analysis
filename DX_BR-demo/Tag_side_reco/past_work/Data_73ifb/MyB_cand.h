#ifndef MY_B_CAND
#define MY_B_CAND

struct MyB_cand{
    double SigProb;
    double Mbc;
    double deltaE;
    double decMode;
    double myMbc;

    void setBranchAddresses(TTree *B_cand){
        B_cand -> SetBranchAddress("SigProb",   &SigProb);
        B_cand -> SetBranchAddress("Mbc",       &Mbc);
        B_cand -> SetBranchAddress("deltaE",    &deltaE);
        B_cand -> SetBranchAddress("decMode",   &decMode);
        B_cand -> SetBranchAddress("myMbc",     &myMbc);
    }

    void branches(TTree *B_cand){
        B_cand -> Branch("SigProb", &SigProb,   "SigProb/D");
        B_cand -> Branch("Mbc",     &Mbc,       "Mbc/D");
        B_cand -> Branch("deltaE",  &deltaE,    "deltaE/D");
        B_cand -> Branch("decMode", &decMode,   "decMode/D");
        B_cand -> Branch("myMbc",   &myMbc,     "myMbc/D");
    }
};
#endif
