#ifndef MY_DEC_CHAIN
#define MY_DEC_CHAIN

struct MyDecChain{
    int nCand;
    double PDG_1[1];
    double PDG_2[1];

    void setBranchAddresses(TTree *Y4S_chain){
        Y4S_chain -> SetBranchAddress("__ncandidates__", &nCand);
        Y4S_chain -> SetBranchAddress("PDG_1", PDG_1);
        Y4S_chain -> SetBranchAddress("PDG_2", PDG_2);
    }
};
#endif
