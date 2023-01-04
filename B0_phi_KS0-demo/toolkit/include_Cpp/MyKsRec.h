#ifndef MY_KS_REC
#define MY_KS_REC

struct MyKsRec{

    int nCand;
    static const int nPart = 9999;

    double M[nPart];
    double chiProb[nPart];
    double goodBelleKshort[nPart];
    double isSignal[nPart];
    double p[nPart];
    double pstar[nPart];
    double isOrHasCloneTrack[nPart];

    double distance[nPart];
    double significanceOfDistance[nPart];
    double flightDistance[nPart];
    double flightDistanceErr[nPart];
    
    double dau_0_p[nPart];
    double dau_0_pt[nPart];
    double dau_0_pstar[nPart];
    double dau_0_nCDCHits[nPart];
    double dau_0_kaonID[nPart];
    double dau_0_thetaInCDCAcceptance[nPart];
    double dau_0_theta[nPart];
    double dau_0_lastCDCLayer[nPart];
    double dau_0_d0[nPart];
    double dau_0_z0[nPart];
    double dau_0_dr[nPart];
    double dau_0_dz[nPart];
    double dau_0_isCloneTrack[nPart];
    
    double dau_1_p[nPart];
    double dau_1_pt[nPart];
    double dau_1_pstar[nPart];
    double dau_1_nCDCHits[nPart];
    double dau_1_kaonID[nPart];
    double dau_1_thetaInCDCAcceptance[nPart];
    double dau_1_theta[nPart];
    double dau_1_lastCDCLayer[nPart];
    double dau_1_d0[nPart];
    double dau_1_z0[nPart];
    double dau_1_dr[nPart];
    double dau_1_dz[nPart];
    double dau_1_isCloneTrack[nPart];

    void setBranchAddresses(TTree *Ks_Rec){
        Ks_Rec -> SetBranchAddress("__ncandidates__", &nCand);
        setBranchAddresses(Ks_Rec, "");
    }
    void setBranchAddresses(TTree *Ks_Rec, const char *prefix){
        Ks_Rec -> SetBranchAddress(Form("%sisSignal",prefix), isSignal);
        Ks_Rec -> SetBranchAddress(Form("%sM",prefix), M);
        Ks_Rec -> SetBranchAddress(Form("%schiProb",prefix), chiProb);
        Ks_Rec -> SetBranchAddress(Form("%sgoodBelleKshort",prefix), goodBelleKshort);

        Ks_Rec -> SetBranchAddress(Form("%sp",prefix), p);
        Ks_Rec -> SetBranchAddress(Form("%spstar",prefix), pstar);
        Ks_Rec -> SetBranchAddress(Form("%sisOrHasCloneTrack",prefix), isOrHasCloneTrack);
       
        Ks_Rec -> SetBranchAddress(Form("%sdistance",prefix), distance);
        Ks_Rec -> SetBranchAddress(Form("%ssignificanceOfDistance",prefix), significanceOfDistance);
        Ks_Rec -> SetBranchAddress(Form("%sflightDistance",prefix), flightDistance);
        Ks_Rec -> SetBranchAddress(Form("%sflightDistanceErr",prefix), flightDistanceErr);
        
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_p",prefix), dau_0_p);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_pt",prefix), dau_0_pt);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_pstar",prefix), dau_0_pstar);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_nCDCHits",prefix), dau_0_nCDCHits);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_kaonID",prefix), dau_0_kaonID);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_thetaInCDCAcceptance",prefix), dau_0_thetaInCDCAcceptance);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_theta",prefix), dau_0_theta);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_lastCDCLayer",prefix), dau_0_lastCDCLayer);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_d0",prefix), dau_0_d0);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_z0",prefix), dau_0_z0);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_dr",prefix), dau_0_dr);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_dz",prefix), dau_0_dz);
        Ks_Rec -> SetBranchAddress(Form("%sdau_0_isCloneTrack",prefix), dau_0_isCloneTrack);
        
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_p",prefix), dau_1_p);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_pt",prefix), dau_1_pt);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_pstar",prefix), dau_1_pstar);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_nCDCHits",prefix), dau_1_nCDCHits);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_kaonID",prefix), dau_1_kaonID);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_thetaInCDCAcceptance",prefix), dau_1_thetaInCDCAcceptance);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_theta",prefix), dau_1_theta);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_lastCDCLayer",prefix), dau_1_lastCDCLayer);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_d0",prefix), dau_1_d0);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_z0",prefix), dau_1_z0);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_dr",prefix), dau_1_dr);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_dz",prefix), dau_1_dz);
        Ks_Rec -> SetBranchAddress(Form("%sdau_1_isCloneTrack",prefix), dau_1_isCloneTrack);
    }
};
#endif
