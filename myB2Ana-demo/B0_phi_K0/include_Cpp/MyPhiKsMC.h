#ifndef MY_PHI_KS_MC
#define MY_PHI_KS_MC

struct MyPhiKsMC{
    int nCand;
    static const int nPart = 9999;

    double nDaus[nPart];
    double mdstIndex[nPart];
    double mcFlightDistance[nPart];
    double pstar[nPart];
    double p[nPart];
    double mcSecPhysProc[nPart];

    double dau_0_PDG[nPart];
    double dau_1_PDG[nPart];
    double dau_2_PDG[nPart];
    double dau_3_PDG[nPart];
    double dau_0_mcSecPhysProc[nPart];
    double dau_1_mcSecPhysProc[nPart];
    double dau_2_mcSecPhysProc[nPart];
    double dau_3_mcSecPhysProc[nPart];

    double dau_0_pt[nPart];
    double dau_1_pt[nPart];
    double dau_0_theta[nPart];
    double dau_1_theta[nPart];
    double dau_0_thetaInCDCAcceptance[nPart];
    double dau_1_thetaInCDCAcceptance[nPart];
    double dau_0_isReconstructible[nPart];
    double dau_1_isReconstructible[nPart];
    double dau_0_seenInCDC[nPart];
    double dau_1_seenInCDC[nPart];

    void setBranchAddresses(TTree *phiKs_MC){
        phiKs_MC -> SetBranchAddress("__ncandidates__", &nCand);
        setBranchAddresses(phiKs_MC, "");    
    }
    void setBranchAddresses(TTree *phiKs_MC, const char *prefix){
        phiKs_MC -> SetBranchAddress(Form("%snDaus",prefix), nDaus);
        phiKs_MC -> SetBranchAddress(Form("%smdstIndex",prefix), mdstIndex);
        phiKs_MC -> SetBranchAddress(Form("%smcFlightDistance",prefix), mcFlightDistance);
        phiKs_MC -> SetBranchAddress(Form("%spstar",prefix), pstar);
        phiKs_MC -> SetBranchAddress(Form("%sp",prefix), p);
        phiKs_MC -> SetBranchAddress(Form("%smcSecPhysProc",prefix), mcSecPhysProc);
        
        phiKs_MC -> SetBranchAddress(Form("%sdau_0_PDG",prefix), dau_0_PDG);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_PDG",prefix), dau_1_PDG);
        phiKs_MC -> SetBranchAddress(Form("%sdau_2_PDG",prefix), dau_2_PDG);
        phiKs_MC -> SetBranchAddress(Form("%sdau_3_PDG",prefix), dau_3_PDG);
        phiKs_MC -> SetBranchAddress(Form("%sdau_0_mcSecPhysProc",prefix), dau_0_mcSecPhysProc);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_mcSecPhysProc",prefix), dau_1_mcSecPhysProc);
        phiKs_MC -> SetBranchAddress(Form("%sdau_2_mcSecPhysProc",prefix), dau_2_mcSecPhysProc);
        phiKs_MC -> SetBranchAddress(Form("%sdau_3_mcSecPhysProc",prefix), dau_3_mcSecPhysProc);

        phiKs_MC -> SetBranchAddress(Form("%sdau_0_pt",prefix), dau_0_pt);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_pt",prefix), dau_1_pt);
        phiKs_MC -> SetBranchAddress(Form("%sdau_0_theta",prefix), dau_0_theta);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_theta",prefix), dau_1_theta);
        phiKs_MC -> SetBranchAddress(Form("%sdau_0_thetaInCDCAcceptance",prefix), dau_0_thetaInCDCAcceptance);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_thetaInCDCAcceptance",prefix), dau_1_thetaInCDCAcceptance);
        phiKs_MC -> SetBranchAddress(Form("%sdau_0_isReconstructible",prefix), dau_0_isReconstructible);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_isReconstructible",prefix), dau_1_isReconstructible);
        phiKs_MC -> SetBranchAddress(Form("%sdau_0_seenInCDC",prefix), dau_0_seenInCDC);
        phiKs_MC -> SetBranchAddress(Form("%sdau_1_seenInCDC",prefix), dau_1_seenInCDC);
    }
};
#endif
