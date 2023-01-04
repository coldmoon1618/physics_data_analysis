#ifndef MY_B_CAND
#define MY_B_CAND

struct MyB_cand{
    double isSignal;
    double Mbc;
    double deltaE;
    double SigProb;
    double decMode;

    double isSigAccMiss;
    double isSigAccMissGamma;
    double isSigAccWrongFSP;

    double isMixed;
    double isCharg;
    double isUUbar;
    double isDDbar;
    double isSSbar;
    double isCCbar;
    double isQQbar;

    void setBranchAddresses(TTree *B_cand){
        B_cand -> SetBranchAddress("isSignal",                 &isSignal);
        B_cand -> SetBranchAddress("Mbc",                      &Mbc);
        B_cand -> SetBranchAddress("deltaE",                   &deltaE);
        B_cand -> SetBranchAddress("SigProb",                  &SigProb);
        B_cand -> SetBranchAddress("decMode",                  &decMode);

        B_cand -> SetBranchAddress("isSigAccMiss",             &isSigAccMiss);
        B_cand -> SetBranchAddress("isSigAccMissGamma",        &isSigAccMissGamma);
        B_cand -> SetBranchAddress("isSigAccWrongFSP",         &isSigAccWrongFSP);

        B_cand -> SetBranchAddress("isMixed",                  &isMixed);
        B_cand -> SetBranchAddress("isCharg",                  &isCharg);
        B_cand -> SetBranchAddress("isUUbar",                  &isUUbar);
        B_cand -> SetBranchAddress("isDDbar",                  &isDDbar);
        B_cand -> SetBranchAddress("isSSbar",                  &isSSbar);
        B_cand -> SetBranchAddress("isCCbar",                  &isCCbar);
        B_cand -> SetBranchAddress("isQQbar",                  &isQQbar);
    }

    void branches(TTree *B_cand){
        B_cand -> Branch("isSignal",              &isSignal,              "isSignal/D");
        B_cand -> Branch("Mbc",                   &Mbc,                   "Mbc/D");
        B_cand -> Branch("deltaE",                &deltaE,                "deltaE/D");
        B_cand -> Branch("SigProb",               &SigProb,               "SigProb/D");
        B_cand -> Branch("decMode",               &decMode,               "decMode/D");

        B_cand -> Branch("isSigAccMiss",          &isSigAccMiss,          "isSigAccMiss/D");
        B_cand -> Branch("isSigAccMissGamma",     &isSigAccMissGamma,     "isSigAccMissGamma/D");
        B_cand -> Branch("isSigAccWrongFSP",      &isSigAccWrongFSP,      "isSigAccWrongFSP/D");

        B_cand -> Branch("isMixed",               &isMixed,               "isMixed/D");
        B_cand -> Branch("isCharg",               &isCharg,               "isCharg/D");
        B_cand -> Branch("isUUbar",               &isUUbar,               "isUUbar/D");
        B_cand -> Branch("isDDbar",               &isDDbar,               "isDDbar/D");
        B_cand -> Branch("isSSbar",               &isSSbar,               "isSSbar/D");
        B_cand -> Branch("isCCbar",               &isCCbar,               "isCCbar/D");
        B_cand -> Branch("isQQbar",               &isQQbar,               "isQQbar/D");
    }
};
#endif
