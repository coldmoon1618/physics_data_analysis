#ifndef MY_PHI_REC
#define MY_PHI_REC

struct MyPhiRec{

    int nCand;
    static const int nPart = 9999;

    double isSignal[nPart];
    double M[nPart];
    double p[nPart];
    double pstar[nPart];
    double chiProb[nPart];
    double cosHel[nPart];
    
    double dau_0_p[nPart];
    double dau_0_pt[nPart];
    double dau_0_pstar[nPart];
    double dau_0_nCDCHits[nPart];
    double dau_0_nSVDHits[nPart];
    double dau_0_kaonID[nPart];
    double dau_0_thetaInCDCAcceptance[nPart];
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
    double dau_1_nSVDHits[nPart];
    double dau_1_kaonID[nPart];
    double dau_1_thetaInCDCAcceptance[nPart];
    double dau_1_lastCDCLayer[nPart];
    double dau_1_d0[nPart];
    double dau_1_z0[nPart];
    double dau_1_dr[nPart];
    double dau_1_dz[nPart];
    double dau_1_isCloneTrack[nPart];

    double dau_0_isSignal[nPart];
    double dau_1_isSignal[nPart];

    void setBranchAddresses(TTree *phi_Rec){
        phi_Rec -> SetBranchAddress("__ncandidates__", &nCand);
        setBranchAddresses(phi_Rec, "");
    }
    void setBranchAddresses(TTree *phi_Rec, const char* prefix){
        phi_Rec -> SetBranchAddress(Form("%sisSignal",prefix), isSignal);
        phi_Rec -> SetBranchAddress(Form("%sM",prefix), M);
        phi_Rec -> SetBranchAddress(Form("%sp",prefix), p);
        phi_Rec -> SetBranchAddress(Form("%spstar",prefix), pstar);
        phi_Rec -> SetBranchAddress(Form("%schiProb",prefix), chiProb);
        phi_Rec -> SetBranchAddress(Form("%scosHel",prefix), cosHel);
        
        phi_Rec -> SetBranchAddress(Form("%sdau_0_p",prefix), dau_0_p);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_pt",prefix), dau_0_pt);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_pstar",prefix), dau_0_pstar);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_nCDCHits",prefix), dau_0_nCDCHits);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_nSVDHits",prefix), dau_0_nSVDHits);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_kaonID",prefix), dau_0_kaonID);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_thetaInCDCAcceptance",prefix), dau_0_thetaInCDCAcceptance);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_lastCDCLayer",prefix), dau_0_lastCDCLayer);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_d0",prefix), dau_0_d0);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_z0",prefix), dau_0_z0);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_dr",prefix), dau_0_dr);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_dz",prefix), dau_0_dz);
        phi_Rec -> SetBranchAddress(Form("%sdau_0_isCloneTrack",prefix), dau_0_isCloneTrack);
        
        phi_Rec -> SetBranchAddress(Form("%sdau_1_p",prefix), dau_1_p);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_pt",prefix), dau_1_pt);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_pstar",prefix), dau_1_pstar);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_nCDCHits",prefix), dau_1_nCDCHits);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_nSVDHits",prefix), dau_1_nSVDHits);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_kaonID",prefix), dau_1_kaonID);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_thetaInCDCAcceptance",prefix), dau_1_thetaInCDCAcceptance);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_lastCDCLayer",prefix), dau_1_lastCDCLayer);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_d0",prefix), dau_1_d0);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_z0",prefix), dau_1_z0);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_dr",prefix), dau_1_dr);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_dz",prefix), dau_1_dz);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_isCloneTrack",prefix), dau_1_isCloneTrack);

        phi_Rec -> SetBranchAddress(Form("%sdau_0_isSignal",prefix), dau_0_isSignal);
        phi_Rec -> SetBranchAddress(Form("%sdau_1_isSignal",prefix), dau_1_isSignal);
    }
};
#endif
