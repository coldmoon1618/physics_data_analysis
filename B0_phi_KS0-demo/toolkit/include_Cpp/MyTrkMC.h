#ifndef MY_TRK_MC
#define MY_TRK_MC

struct MyTrkMC{
    
    int nCand;
    static const int nPart = 9999;

    double mdstIndex[nPart];
    double thetaInCDCAcceptance[nPart];
    double seenInCDC[nPart];
    double p[nPart];
    double pt[nPart];
    double genParticleID[nPart];
    double thetaStar[nPart];
    double motherDr[nPart];
    double dr[nPart];
    double mcProdVertexDX[nPart];
    double mcProdVertexDY[nPart];

    void setBranchAddresses(TTree *Trk_MC){
        Trk_MC -> SetBranchAddress("__ncandidates__", &nCand);
        Trk_MC -> SetBranchAddress("mdstIndex", mdstIndex);
        Trk_MC -> SetBranchAddress("thetaInCDCAcceptance", thetaInCDCAcceptance);
        Trk_MC -> SetBranchAddress("seenInCDC", seenInCDC);
        Trk_MC -> SetBranchAddress("pt", pt);
        Trk_MC -> SetBranchAddress("genParticleID", genParticleID);
        Trk_MC -> SetBranchAddress("thetaStar", thetaStar);
        Trk_MC -> SetBranchAddress("motherDr", motherDr);
        Trk_MC -> SetBranchAddress("dr", dr);
        Trk_MC -> SetBranchAddress("mcProdVertexDX", mcProdVertexDX);
        Trk_MC -> SetBranchAddress("mcProdVertexDY", mcProdVertexDY);
    }
};
#endif
