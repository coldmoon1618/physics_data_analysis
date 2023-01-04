#ifndef MY_TRK_REC
#define MY_TRK_REC

struct MyTrkRec{
    
    int nCand;
    static const int nPart = 9999;

    double thetaInCDCAcceptance[nPart];
    double p[nPart];
    double mcP[nPart];
    double pt[nPart];
    double mcPT[nPart];
    double isCloneTrack[nPart];
    double nMCMatches[nPart];
    double genParticleID[nPart];
    double theta[nPart];
    double thetaStar[nPart];
    double motherDr[nPart];
    double dr[nPart];
    double mcProdVertexDX[nPart];
    double mcProdVertexDY[nPart];

    void setBranchAddresses(TTree *Trk_Rec){
        Trk_Rec -> SetBranchAddress("__ncandidates__", &nCand);
        Trk_Rec -> SetBranchAddress("thetaInCDCAcceptance", thetaInCDCAcceptance);
        Trk_Rec -> SetBranchAddress("p", p);
        Trk_Rec -> SetBranchAddress("mcP", mcP);
        Trk_Rec -> SetBranchAddress("pt", pt);
        Trk_Rec -> SetBranchAddress("mcPT", mcPT);
        Trk_Rec -> SetBranchAddress("isCloneTrack", isCloneTrack);
        Trk_Rec -> SetBranchAddress("nMCMatches", nMCMatches);
        Trk_Rec -> SetBranchAddress("genParticleID", genParticleID);
        Trk_Rec -> SetBranchAddress("theta", theta);
        Trk_Rec -> SetBranchAddress("thetaStar", thetaStar);
        Trk_Rec -> SetBranchAddress("motherDr", motherDr);
        Trk_Rec -> SetBranchAddress("dr", dr);
        Trk_Rec -> SetBranchAddress("mcProdVertexDX", mcProdVertexDX);
        Trk_Rec -> SetBranchAddress("mcProdVertexDY", mcProdVertexDY);
    }
};
#endif
