#ifndef MY_B0_REC
#define MY_B0_REC

struct MyB0Rec{

    int nCand;
    int evtNum;
    double weight;
    static const int nPart = 9999;

    double isSignal[nPart];
    double M[nPart];
    double p[nPart];
    double pstar[nPart];
    double chiProb[nPart];
    double Mbc[nPart];
    double deltaE[nPart];

    double R2[nPart];
    double cosTBTO[nPart];
    double cosTBz[nPart];
    double thrustOm[nPart];
    double thrustBm[nPart];

    double foxWolframR1[nPart];
    double foxWolframR2[nPart];
    double foxWolframR3[nPart];
    double foxWolframR4[nPart];

    double harmonicMomentThrust0[nPart];
    double harmonicMomentThrust1[nPart];
    double harmonicMomentThrust2[nPart];
    double harmonicMomentThrust3[nPart];
    double harmonicMomentThrust4[nPart];

    double cleoConeThrust0[nPart];
    double cleoConeThrust1[nPart];
    double cleoConeThrust2[nPart];
    double cleoConeThrust3[nPart];
    double cleoConeThrust4[nPart];
    double cleoConeThrust5[nPart];
    double cleoConeThrust6[nPart];
    double cleoConeThrust7[nPart];
    double cleoConeThrust8[nPart];

    double sphericity[nPart];
    double aplanarity[nPart];
    double thrust[nPart];
    double thrustAxisCosTheta[nPart];

    double CS_hso00[nPart];
    double CS_hso02[nPart];
    double CS_hso04[nPart];
    double CS_hso10[nPart];
    double CS_hso12[nPart];
    double CS_hso14[nPart];
    double CS_hso20[nPart];
    double CS_hso22[nPart];
    double CS_hso24[nPart];
    double CS_hoo1[nPart];
    double CS_hoo2[nPart];
    double CS_hoo3[nPart];
    double CS_hoo4[nPart];
    double CS_et[nPart];
    double CS_mm2[nPart];

    double qrMC[nPart];
    double flvrTag_FBDT[nPart];
    double flvrTag_FANN[nPart];
    double MCflvrOtherB[nPart];

    double Ecms[nPart];
    double beamE[nPart];
    double beamPx[nPart];
    double beamPy[nPart];
    double beamPz[nPart];

    double DeltaBoost[nPart];
    double DeltaBoostErr[nPart];
    double DeltaT[nPart];
    double DeltaTErr[nPart];
    double DeltaTRes[nPart];
    double DeltaZ[nPart];
    double DeltaZErr[nPart];

    double mcDeltaBoost[nPart];
    double mcDeltaT[nPart];
    double mcDeltaTau[nPart];

    void setBranchAddresses(TTree *B0_Rec){
        B0_Rec -> SetBranchAddress("__ncandidates__",           &nCand);
        B0_Rec -> SetBranchAddress("__event__",                 &evtNum);
        B0_Rec -> SetBranchAddress("__weight__",                &weight);
        B0_Rec -> SetBranchAddress("isSignal",                  isSignal);
        B0_Rec -> SetBranchAddress("M",                         M);
        B0_Rec -> SetBranchAddress("p",                         p);
        B0_Rec -> SetBranchAddress("pstar",                     pstar);
        B0_Rec -> SetBranchAddress("chiProb",                   chiProb);
        B0_Rec -> SetBranchAddress("Mbc",                       Mbc);
        B0_Rec -> SetBranchAddress("deltaE",                    deltaE);

        B0_Rec -> SetBranchAddress("R2",                        R2);
        B0_Rec -> SetBranchAddress("cosTBTO",                   cosTBTO);
        B0_Rec -> SetBranchAddress("cosTBz",                    cosTBz);
        B0_Rec -> SetBranchAddress("thrustOm",                  thrustOm);
        B0_Rec -> SetBranchAddress("thrustBm",                  thrustBm);

        B0_Rec -> SetBranchAddress("foxWolframR1",              foxWolframR1);
        B0_Rec -> SetBranchAddress("foxWolframR2",              foxWolframR2);
        B0_Rec -> SetBranchAddress("foxWolframR3",              foxWolframR3);
        B0_Rec -> SetBranchAddress("foxWolframR4",              foxWolframR4);

        B0_Rec -> SetBranchAddress("harmonicMomentThrust0",     harmonicMomentThrust0);
        B0_Rec -> SetBranchAddress("harmonicMomentThrust1",     harmonicMomentThrust1);
        B0_Rec -> SetBranchAddress("harmonicMomentThrust2",     harmonicMomentThrust2);
        B0_Rec -> SetBranchAddress("harmonicMomentThrust3",     harmonicMomentThrust3);
        B0_Rec -> SetBranchAddress("harmonicMomentThrust4",     harmonicMomentThrust4);

        B0_Rec -> SetBranchAddress("cleoConeThrust0",           cleoConeThrust0);
        B0_Rec -> SetBranchAddress("cleoConeThrust1",           cleoConeThrust1);
        B0_Rec -> SetBranchAddress("cleoConeThrust2",           cleoConeThrust2);
        B0_Rec -> SetBranchAddress("cleoConeThrust3",           cleoConeThrust3);
        B0_Rec -> SetBranchAddress("cleoConeThrust4",           cleoConeThrust4);
        B0_Rec -> SetBranchAddress("cleoConeThrust5",           cleoConeThrust5);
        B0_Rec -> SetBranchAddress("cleoConeThrust6",           cleoConeThrust6);
        B0_Rec -> SetBranchAddress("cleoConeThrust7",           cleoConeThrust7);
        B0_Rec -> SetBranchAddress("cleoConeThrust8",           cleoConeThrust8);

        B0_Rec -> SetBranchAddress("sphericity",                sphericity);
        B0_Rec -> SetBranchAddress("aplanarity",                aplanarity);
        B0_Rec -> SetBranchAddress("thrust",                    thrust);
        B0_Rec -> SetBranchAddress("thrustAxisCosTheta",        thrustAxisCosTheta);

        B0_Rec -> SetBranchAddress("CS_hso00",                  CS_hso00);
        B0_Rec -> SetBranchAddress("CS_hso02",                  CS_hso02);
        B0_Rec -> SetBranchAddress("CS_hso04",                  CS_hso04);
        B0_Rec -> SetBranchAddress("CS_hso10",                  CS_hso10);
        B0_Rec -> SetBranchAddress("CS_hso12",                  CS_hso12);
        B0_Rec -> SetBranchAddress("CS_hso14",                  CS_hso14);
        B0_Rec -> SetBranchAddress("CS_hso20",                  CS_hso20);
        B0_Rec -> SetBranchAddress("CS_hso22",                  CS_hso22);
        B0_Rec -> SetBranchAddress("CS_hso24",                  CS_hso24);
        B0_Rec -> SetBranchAddress("CS_hoo1",                   CS_hoo1);
        B0_Rec -> SetBranchAddress("CS_hoo2",                   CS_hoo2);
        B0_Rec -> SetBranchAddress("CS_hoo3",                   CS_hoo3);
        B0_Rec -> SetBranchAddress("CS_hoo4",                   CS_hoo4);
        B0_Rec -> SetBranchAddress("CS_et",                     CS_et);
        B0_Rec -> SetBranchAddress("CS_mm2",                    CS_mm2);

        B0_Rec -> SetBranchAddress("qrMC",                      qrMC);
        B0_Rec -> SetBranchAddress("flvrTag_FBDT",              flvrTag_FBDT);
        B0_Rec -> SetBranchAddress("flvrTag_FANN",              flvrTag_FANN);
        B0_Rec -> SetBranchAddress("mcFlavorOfOtherB",          MCflvrOtherB);

        B0_Rec -> SetBranchAddress("Ecms",                      Ecms);
        B0_Rec -> SetBranchAddress("beamE",                     beamE);
        B0_Rec -> SetBranchAddress("beamPx",                    beamPx);
        B0_Rec -> SetBranchAddress("beamPy",                    beamPy);
        B0_Rec -> SetBranchAddress("beamPz",                    beamPz);

        B0_Rec -> SetBranchAddress("DeltaBoost",                DeltaBoost);
        B0_Rec -> SetBranchAddress("DeltaBoostErr",             DeltaBoostErr);
        B0_Rec -> SetBranchAddress("DeltaT",                    DeltaT);
        B0_Rec -> SetBranchAddress("DeltaTErr",                 DeltaTErr);
        B0_Rec -> SetBranchAddress("DeltaTRes",                 DeltaTRes);
        B0_Rec -> SetBranchAddress("DeltaZ",                    DeltaZ);
        B0_Rec -> SetBranchAddress("DeltaZErr",                 DeltaZErr);

        B0_Rec -> SetBranchAddress("mcDeltaBoost",              mcDeltaBoost);
        B0_Rec -> SetBranchAddress("mcDeltaT",                  mcDeltaT);
        B0_Rec -> SetBranchAddress("mcDeltaTau",                mcDeltaTau);

    }
};
#endif

