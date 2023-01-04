#ifndef MY_B0_CAND
#define MY_B0_CAND

struct MyB0Cand{
    float isSignal;
    float M;
    float p;
    float pstar;
    float chiProb;
    float Mbc;
    float deltaE;

    float R2;
    float cosTBTO;
    float cosTBz;
    float thrustOm;
    float thrustBm;

    float weight;

    float isBBmix;
    float isBphiK;
    float isMixed;
    float isBBbar;
    float sigFlav;
    float tagFlav;
    float isNonre;

    float foxWolframR1;
    float foxWolframR2;
    float foxWolframR3;
    float foxWolframR4;

    float harmonicMomentThrust0;
    float harmonicMomentThrust1;
    float harmonicMomentThrust2;
    float harmonicMomentThrust3;
    float harmonicMomentThrust4;

    float cleoConeThrust0;
    float cleoConeThrust1;
    float cleoConeThrust2;
    float cleoConeThrust3;
    float cleoConeThrust4;
    float cleoConeThrust5;
    float cleoConeThrust6;
    float cleoConeThrust7;
    float cleoConeThrust8;

    float sphericity;
    float aplanarity;
    float thrust;
    float thrustAxisCosTheta;

    float CS_hso00;
    float CS_hso02;
    float CS_hso04;
    float CS_hso10;
    float CS_hso12;
    float CS_hso14;
    float CS_hso20;
    float CS_hso22;
    float CS_hso24;
    float CS_hoo1;
    float CS_hoo2;
    float CS_hoo3;
    float CS_hoo4;
    float CS_et;
    float CS_mm2;

    float qrMC;
    float flvrTag_FBDT;
    float flvrTag_FANN;
    float MCflvrOtherB;
    
    float Ecms;
    float beamE;
    float beamPx;
    float beamPy;
    float beamPz;

    float DeltaBoost;
    float DeltaBoostErr;
    float DeltaT;
    float DeltaTErr;
    float DeltaTRes;
    float DeltaZ;
    float DeltaZErr;

    float mcDeltaBoost;
    float mcDeltaT;
    float mcDeltaTau;

    void setBranchAddresses(TTree *B0_cand){
        B0_cand -> SetBranchAddress("isSignal",                 &isSignal);
        B0_cand -> SetBranchAddress("M",                        &M);
        B0_cand -> SetBranchAddress("p",                        &p);
        B0_cand -> SetBranchAddress("pstar",                    &pstar);
        B0_cand -> SetBranchAddress("chiProb",                  &chiProb);
        B0_cand -> SetBranchAddress("Mbc",                      &Mbc);
        B0_cand -> SetBranchAddress("deltaE",                   &deltaE);

        B0_cand -> SetBranchAddress("__weight__",               &weight);
        B0_cand -> SetBranchAddress("isBBmix",                  &isBBmix);
        B0_cand -> SetBranchAddress("isBphiK",                  &isBphiK);
        B0_cand -> SetBranchAddress("isMixed",                  &isMixed);
        B0_cand -> SetBranchAddress("isBBbar",                  &isBBbar);
        B0_cand -> SetBranchAddress("sigFlav",                  &sigFlav);
        B0_cand -> SetBranchAddress("tagFlav",                  &tagFlav);
        B0_cand -> SetBranchAddress("isNonre",                  &isNonre);

        B0_cand -> SetBranchAddress("R2",                       &R2);
        B0_cand -> SetBranchAddress("cosTBTO",                  &cosTBTO);
        B0_cand -> SetBranchAddress("cosTBz",                   &cosTBz);
        B0_cand -> SetBranchAddress("thrustOm",                 &thrustOm);
        B0_cand -> SetBranchAddress("thrustBm",                 &thrustBm);

        B0_cand -> SetBranchAddress("foxWolframR1",             &foxWolframR1);
        B0_cand -> SetBranchAddress("foxWolframR2",             &foxWolframR2);
        B0_cand -> SetBranchAddress("foxWolframR3",             &foxWolframR3);
        B0_cand -> SetBranchAddress("foxWolframR4",             &foxWolframR4);

        B0_cand -> SetBranchAddress("harmonicMomentThrust0",    &harmonicMomentThrust0);
        B0_cand -> SetBranchAddress("harmonicMomentThrust1",    &harmonicMomentThrust1);
        B0_cand -> SetBranchAddress("harmonicMomentThrust2",    &harmonicMomentThrust2);
        B0_cand -> SetBranchAddress("harmonicMomentThrust3",    &harmonicMomentThrust3);
        B0_cand -> SetBranchAddress("harmonicMomentThrust4",    &harmonicMomentThrust4);

        B0_cand -> SetBranchAddress("cleoConeThrust0",          &cleoConeThrust0);
        B0_cand -> SetBranchAddress("cleoConeThrust1",          &cleoConeThrust1);
        B0_cand -> SetBranchAddress("cleoConeThrust2",          &cleoConeThrust2);
        B0_cand -> SetBranchAddress("cleoConeThrust3",          &cleoConeThrust3);
        B0_cand -> SetBranchAddress("cleoConeThrust4",          &cleoConeThrust4);
        B0_cand -> SetBranchAddress("cleoConeThrust5",          &cleoConeThrust5);
        B0_cand -> SetBranchAddress("cleoConeThrust6",          &cleoConeThrust6);
        B0_cand -> SetBranchAddress("cleoConeThrust7",          &cleoConeThrust7);
        B0_cand -> SetBranchAddress("cleoConeThrust8",          &cleoConeThrust8);

        B0_cand -> SetBranchAddress("sphericity",               &sphericity);
        B0_cand -> SetBranchAddress("aplanarity",               &aplanarity);
        B0_cand -> SetBranchAddress("thrust",                   &thrust);
        B0_cand -> SetBranchAddress("thrustAxisCosTheta",       &thrustAxisCosTheta);

        B0_cand -> SetBranchAddress("CS_hso00",                 &CS_hso00);
        B0_cand -> SetBranchAddress("CS_hso02",                 &CS_hso02);
        B0_cand -> SetBranchAddress("CS_hso04",                 &CS_hso04);
        B0_cand -> SetBranchAddress("CS_hso10",                 &CS_hso10);
        B0_cand -> SetBranchAddress("CS_hso12",                 &CS_hso12);
        B0_cand -> SetBranchAddress("CS_hso14",                 &CS_hso14);
        B0_cand -> SetBranchAddress("CS_hso20",                 &CS_hso20);
        B0_cand -> SetBranchAddress("CS_hso22",                 &CS_hso22);
        B0_cand -> SetBranchAddress("CS_hso24",                 &CS_hso24);
        B0_cand -> SetBranchAddress("CS_hoo1",                  &CS_hoo1);
        B0_cand -> SetBranchAddress("CS_hoo2",                  &CS_hoo2);
        B0_cand -> SetBranchAddress("CS_hoo3",                  &CS_hoo3);
        B0_cand -> SetBranchAddress("CS_hoo4",                  &CS_hoo4);
        B0_cand -> SetBranchAddress("CS_et",                    &CS_et);
        B0_cand -> SetBranchAddress("CS_mm2",                   &CS_mm2);

        B0_cand -> SetBranchAddress("qrMC",                     &qrMC);
        B0_cand -> SetBranchAddress("flvrTag_FBDT",             &flvrTag_FBDT);
        B0_cand -> SetBranchAddress("flvrTag_FANN",             &flvrTag_FANN);
        B0_cand -> SetBranchAddress("MCflvrOtherB",             &MCflvrOtherB);

        B0_cand -> SetBranchAddress("Ecms",                     &Ecms);
        B0_cand -> SetBranchAddress("beamE",                    &beamE);
        B0_cand -> SetBranchAddress("beamPx",                   &beamPy);
        B0_cand -> SetBranchAddress("beamPy",                   &beamPy);
        B0_cand -> SetBranchAddress("beamPz",                   &beamPy);

        B0_cand -> SetBranchAddress("DeltaBoost",               &DeltaBoost);
        B0_cand -> SetBranchAddress("DeltaBoostErr",            &DeltaBoostErr);
        B0_cand -> SetBranchAddress("DeltaT",                   &DeltaT);
        B0_cand -> SetBranchAddress("DeltaTErr",                &DeltaTErr);
        B0_cand -> SetBranchAddress("DeltaTRes",                &DeltaTRes);
        B0_cand -> SetBranchAddress("DeltaZ",                   &DeltaZ);
        B0_cand -> SetBranchAddress("DeltaZErr",                &DeltaZErr);

        B0_cand -> SetBranchAddress("mcDeltaBoost",             &mcDeltaBoost);
        B0_cand -> SetBranchAddress("mcDeltaT",                 &mcDeltaT);
        B0_cand -> SetBranchAddress("mcDeltaTau",               &mcDeltaTau);
    }

    void branches(TTree *B0_cand){
        B0_cand -> Branch("isSignal",              &isSignal,              "isSignal/F");
        B0_cand -> Branch("M",                     &M,                     "M/F");
        B0_cand -> Branch("p",                     &p,                     "p/F");
        B0_cand -> Branch("pstar",                 &pstar,                 "pstar/F");
        B0_cand -> Branch("chiProb",               &chiProb,               "chiProb/F");
        B0_cand -> Branch("Mbc",                   &Mbc,                   "Mbc/F");
        B0_cand -> Branch("deltaE",                &deltaE,                "deltaE/F");

        B0_cand -> Branch("__weight__",            &weight,                "__weight__/F");
        B0_cand -> Branch("isBBmix",               &isBBmix,               "isBBmix/F");
        B0_cand -> Branch("isBphiK",               &isBphiK,               "isBphiK/F");
        B0_cand -> Branch("isMixed",               &isMixed,               "isMixed/F");
        B0_cand -> Branch("isBBbar",               &isBBbar,               "isBBbar/F");
        B0_cand -> Branch("sigFlav",               &sigFlav,               "sigFlav/F");
        B0_cand -> Branch("tagFlav",               &tagFlav,               "tagFlav/F");
        B0_cand -> Branch("isNonre",               &isNonre,               "isNonre/F");

        B0_cand -> Branch("R2",                    &R2,                    "R2/F");
        B0_cand -> Branch("cosTBTO",               &cosTBTO,               "cosTBTO/F");
        B0_cand -> Branch("cosTBz",                &cosTBz,                "cosTBz/F");
        B0_cand -> Branch("thrustOm",              &thrustOm,              "thrustOm/F");
        B0_cand -> Branch("thrustBm",              &thrustBm,              "thrustBm/F");

        B0_cand -> Branch("foxWolframR1",          &foxWolframR1,          "foxWolframR1/F");
        B0_cand -> Branch("foxWolframR2",          &foxWolframR2,          "foxWolframR2/F");
        B0_cand -> Branch("foxWolframR3",          &foxWolframR3,          "foxWolframR3/F");
        B0_cand -> Branch("foxWolframR4",          &foxWolframR4,          "foxWolframR4/F");

        B0_cand -> Branch("harmonicMomentThrust0", &harmonicMomentThrust0, "harmonicMomentThrust0/F");
        B0_cand -> Branch("harmonicMomentThrust1", &harmonicMomentThrust1, "harmonicMomentThrust1/F");
        B0_cand -> Branch("harmonicMomentThrust2", &harmonicMomentThrust2, "harmonicMomentThrust2/F");
        B0_cand -> Branch("harmonicMomentThrust3", &harmonicMomentThrust3, "harmonicMomentThrust3/F");
        B0_cand -> Branch("harmonicMomentThrust4", &harmonicMomentThrust4, "harmonicMomentThrust4/F");

        B0_cand -> Branch("cleoConeThrust0",       &cleoConeThrust0,       "cleoConeThrust0/F");
        B0_cand -> Branch("cleoConeThrust1",       &cleoConeThrust1,       "cleoConeThrust1/F");
        B0_cand -> Branch("cleoConeThrust2",       &cleoConeThrust2,       "cleoConeThrust2/F");
        B0_cand -> Branch("cleoConeThrust3",       &cleoConeThrust3,       "cleoConeThrust3/F");
        B0_cand -> Branch("cleoConeThrust4",       &cleoConeThrust4,       "cleoConeThrust4/F");
        B0_cand -> Branch("cleoConeThrust5",       &cleoConeThrust5,       "cleoConeThrust5/F");
        B0_cand -> Branch("cleoConeThrust6",       &cleoConeThrust6,       "cleoConeThrust6/F");
        B0_cand -> Branch("cleoConeThrust7",       &cleoConeThrust7,       "cleoConeThrust7/F");
        B0_cand -> Branch("cleoConeThrust8",       &cleoConeThrust8,       "cleoConeThrust8/F");

        B0_cand -> Branch("sphericity",            &sphericity,            "sphericity/F");
        B0_cand -> Branch("aplanarity",            &aplanarity,            "aplanarity/F");
        B0_cand -> Branch("thrust",                &thrust,                "thrust/F");
        B0_cand -> Branch("thrustAxisCosTheta",    &thrustAxisCosTheta,    "thrustAxisCosTheta/F");

        B0_cand -> Branch("CS_hso00",              &CS_hso00,              "CS_hso00/F");
        B0_cand -> Branch("CS_hso02",              &CS_hso02,              "CS_hso02/F");
        B0_cand -> Branch("CS_hso04",              &CS_hso04,              "CS_hso04/F");
        B0_cand -> Branch("CS_hso10",              &CS_hso10,              "CS_hso10/F");
        B0_cand -> Branch("CS_hso12",              &CS_hso12,              "CS_hso12/F");
        B0_cand -> Branch("CS_hso14",              &CS_hso14,              "CS_hso14/F");
        B0_cand -> Branch("CS_hso20",              &CS_hso20,              "CS_hso20/F");
        B0_cand -> Branch("CS_hso22",              &CS_hso22,              "CS_hso22/F");
        B0_cand -> Branch("CS_hso24",              &CS_hso24,              "CS_hso24/F");
        B0_cand -> Branch("CS_hoo1",               &CS_hoo1,               "CS_hoo1/F");
        B0_cand -> Branch("CS_hoo2",               &CS_hoo2,               "CS_hoo2/F");
        B0_cand -> Branch("CS_hoo3",               &CS_hoo3,               "CS_hoo3/F");
        B0_cand -> Branch("CS_hoo4",               &CS_hoo4,               "CS_hoo4/F");
        B0_cand -> Branch("CS_et",                 &CS_et,                 "CS_et/F");
        B0_cand -> Branch("CS_mm2",                &CS_mm2,                "CS_mm2/F");

        B0_cand -> Branch("qrMC",                  &qrMC,                  "qrMC/F");
        B0_cand -> Branch("flvrTag_FBDT",          &flvrTag_FBDT,          "flvrTag_FBDT/F");
        B0_cand -> Branch("flvrTag_FANN",          &flvrTag_FANN,          "flvrTag_FANN/F");
        B0_cand -> Branch("MCflvrOtherB",          &MCflvrOtherB,          "MCflvrOtherB/F");
        
        B0_cand -> Branch("Ecms",                  &Ecms,                  "Ecms/F");
        B0_cand -> Branch("beamE",                 &beamE,                 "beamE/F");
        B0_cand -> Branch("beamPx",                &beamPx,                "beamPx/F");
        B0_cand -> Branch("beamPy",                &beamPy,                "beamPy/F");
        B0_cand -> Branch("beamPz",                &beamPz,                "beamPz/F");

        B0_cand -> Branch("DeltaBoost",            &DeltaBoost,            "DeltaBoost/F");
        B0_cand -> Branch("DeltaBoostErr",         &DeltaBoostErr,         "DeltaBoostErr/F");
        B0_cand -> Branch("DeltaT",                &DeltaT,                "DeltaT/F");
        B0_cand -> Branch("DeltaTErr",             &DeltaTErr,             "DeltaTErr/F");
        B0_cand -> Branch("DeltaTRes",             &DeltaTRes,             "DeltaTRes/F");
        B0_cand -> Branch("DeltaZ",                &DeltaZ,                "DeltaZ/F");
        B0_cand -> Branch("DeltaZErr",             &DeltaZErr,             "DeltaZErr/F");
        
        B0_cand -> Branch("mcDeltaBoost",          &mcDeltaBoost,          "mcDeltaBoost/F");
        B0_cand -> Branch("mcDeltaT",              &mcDeltaT,              "mcDeltaT/F");
        B0_cand -> Branch("mcDeltaTau",            &mcDeltaTau,            "mcDeltaTau/F");
    }
};
#endif
