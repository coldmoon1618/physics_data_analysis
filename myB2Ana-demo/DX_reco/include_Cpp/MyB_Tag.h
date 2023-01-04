#ifndef MY_B_TAG
#define MY_B_TAG

struct MyB_Tag{

    int nCand;
    int evtNum;
    double weight;
    static const int nPart = 9999;

    double isSignal[nPart];
    double M[nPart];
    double p[nPart];
    double pstar[nPart];
    double SigProb[nPart];
    double Mbc[nPart];
    double deltaE[nPart];

    double R2[nPart];
    double cosTBTO[nPart];
    double cosTBz[nPart];
    double thrustOm[nPart];
    double thrustBm[nPart];

    double decMode[nPart];
    double charge[nPart];

    void setBranchAddresses(TTree *B_Tag){
        B_Tag -> SetBranchAddress("__ncandidates__",           &nCand);
        B_Tag -> SetBranchAddress("__event__",                 &evtNum);
        B_Tag -> SetBranchAddress("__weight__",                &weight);
        B_Tag -> SetBranchAddress("isSignal",                  isSignal);
        B_Tag -> SetBranchAddress("M",                         M);
        B_Tag -> SetBranchAddress("p",                         p);
        B_Tag -> SetBranchAddress("pstar",                     pstar);
        B_Tag -> SetBranchAddress("SigProb",                   SigProb);
        B_Tag -> SetBranchAddress("Mbc",                       Mbc);
        B_Tag -> SetBranchAddress("deltaE",                    deltaE);

        B_Tag -> SetBranchAddress("R2",                        R2);
        B_Tag -> SetBranchAddress("cosTBTO",                   cosTBTO);
        B_Tag -> SetBranchAddress("cosTBz",                    cosTBz);
        B_Tag -> SetBranchAddress("thrustOm",                  thrustOm);
        B_Tag -> SetBranchAddress("thrustBm",                  thrustBm);

        B_Tag -> SetBranchAddress("decMode",                   decMode);
        B_Tag -> SetBranchAddress("charge",                    charge);
    }
};
#endif

