#ifndef MY_B_CAND
#define MY_B_CAND

struct MyB_cand{
    double isSignal;
    double M;
    double p;
    double pstar;
    double SigProb;
    double Mbc;
    double deltaE;

    double R2;
    double cosTBTO;
    double cosTBz;
    double thrustOm;
    double thrustBm;

    double decMode;
    double charge;

    double weight;

    double isMixed;
    double isCharg;
    double isUUbar;
    double isDDbar;
    double isSSbar;
    double isCCbar;
    double isQQbar;

    void setBranchAddresses(TTree *B_cand){
        B_cand -> SetBranchAddress("isSignal",                 &isSignal);
        B_cand -> SetBranchAddress("M",                        &M);
        B_cand -> SetBranchAddress("p",                        &p);
        B_cand -> SetBranchAddress("pstar",                    &pstar);
        B_cand -> SetBranchAddress("SigProb",                  &SigProb);
        B_cand -> SetBranchAddress("Mbc",                      &Mbc);
        B_cand -> SetBranchAddress("deltaE",                   &deltaE);

        B_cand -> SetBranchAddress("__weight__",               &weight);

        B_cand -> SetBranchAddress("R2",                       &R2);
        B_cand -> SetBranchAddress("cosTBTO",                  &cosTBTO);
        B_cand -> SetBranchAddress("cosTBz",                   &cosTBz);
        B_cand -> SetBranchAddress("thrustOm",                 &thrustOm);
        B_cand -> SetBranchAddress("thrustBm",                 &thrustBm);

        B_cand -> SetBranchAddress("decMode",                  &decMode);
        B_cand -> SetBranchAddress("charge",                   &charge);

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
        B_cand -> Branch("M",                     &M,                     "M/D");
        B_cand -> Branch("p",                     &p,                     "p/D");
        B_cand -> Branch("pstar",                 &pstar,                 "pstar/D");
        B_cand -> Branch("SigProb",               &SigProb,               "SigProb/D");
        B_cand -> Branch("Mbc",                   &Mbc,                   "Mbc/D");
        B_cand -> Branch("deltaE",                &deltaE,                "deltaE/D");

        B_cand -> Branch("__weight__",            &weight,                "__weight__/D");

        B_cand -> Branch("R2",                    &R2,                    "R2/D");
        B_cand -> Branch("cosTBTO",               &cosTBTO,               "cosTBTO/D");
        B_cand -> Branch("cosTBz",                &cosTBz,                "cosTBz/D");
        B_cand -> Branch("thrustOm",              &thrustOm,              "thrustOm/D");
        B_cand -> Branch("thrustBm",              &thrustBm,              "thrustBm/D");

        B_cand -> Branch("decMode",               &decMode,               "decMode/D");
        B_cand -> Branch("charge",                &charge,                "charge/D");

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
