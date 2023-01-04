#ifndef MY_PHI_CAND
#define MY_PHI_CAND

struct MyPhiCand{
    float isSignal;
    float M;
    float p;
    float pstar;
    float cosHel;
    
    float dau_0_p;
    float dau_0_pt;
    float dau_0_isCloneTrack;
    
    float dau_1_p;
    float dau_1_pt;
    float dau_1_isCloneTrack;

    float dau_0_isSignal;
    float dau_1_isSignal;

    void setBranchAddresses(TTree *phi_cand){
        setBranchAddresses(phi_cand,"");
    }
    void setBranchAddresses(TTree *phi_cand, const char* prefix){
        phi_cand -> SetBranchAddress(Form("%sisSignal",prefix), &isSignal);
        phi_cand -> SetBranchAddress(Form("%sM",prefix), &M);
        phi_cand -> SetBranchAddress(Form("%sp",prefix), &p);
        phi_cand -> SetBranchAddress(Form("%spstar",prefix), &pstar);
        phi_cand -> SetBranchAddress(Form("%scosHel",prefix), &cosHel);

        phi_cand -> SetBranchAddress(Form("%sdau_0_p",prefix), &dau_0_p);
        phi_cand -> SetBranchAddress(Form("%sdau_0_pt",prefix), &dau_0_pt);
        phi_cand -> SetBranchAddress(Form("%sdau_0_isCloneTrack",prefix), &dau_0_isCloneTrack);

        phi_cand -> SetBranchAddress(Form("%sdau_1_p",prefix), &dau_1_p);
        phi_cand -> SetBranchAddress(Form("%sdau_1_pt",prefix), &dau_1_pt);
        phi_cand -> SetBranchAddress(Form("%sdau_1_isCloneTrack",prefix), &dau_1_isCloneTrack);

        phi_cand -> SetBranchAddress(Form("%sdau_0_isSignal",prefix), &dau_0_isSignal);
        phi_cand -> SetBranchAddress(Form("%sdau_1_isSignal",prefix), &dau_1_isSignal);
    }

    void branches(TTree *phi_cand){
        branches(phi_cand,"");
    }
    void branches(TTree *phi_cand, const char* prefix){
        phi_cand -> Branch(Form("%sisSignal",prefix), &isSignal, Form("%sisSignal/F",prefix));
        phi_cand -> Branch(Form("%sM",prefix), &M, Form("%sM/F",prefix));
        phi_cand -> Branch(Form("%sp",prefix), &p, Form("%sp/F",prefix));
        phi_cand -> Branch(Form("%spstar",prefix), &pstar, Form("%spstar/F",prefix));
        phi_cand -> Branch(Form("%scosHel",prefix), &cosHel, Form("%scosHel/F",prefix));

        phi_cand -> Branch(Form("%sdau_0_p",prefix), &dau_0_p, Form("%sdau_0_p/F",prefix));
        phi_cand -> Branch(Form("%sdau_0_pt",prefix), &dau_0_pt, Form("%sdau_0_pt/F",prefix));
        phi_cand -> Branch(Form("%sdau_0_isCloneTrack",prefix), &dau_0_isCloneTrack, Form("%sdau_0_isCloneTrack/F",prefix));

        phi_cand -> Branch(Form("%sdau_1_p",prefix), &dau_1_p, Form("%sdau_1_p/F",prefix));
        phi_cand -> Branch(Form("%sdau_1_pt",prefix), &dau_1_pt, Form("%sdau_1_pt/F",prefix));
        phi_cand -> Branch(Form("%sdau_1_isCloneTrack",prefix), &dau_1_isCloneTrack, Form("%sdau_1_isCloneTrack/F",prefix));

        phi_cand -> Branch(Form("%sdau_0_isSignal",prefix), &dau_0_isSignal, Form("%sdau_0_isSignal/F",prefix));
        phi_cand -> Branch(Form("%sdau_1_isSignal",prefix), &dau_1_isSignal, Form("%sdau_1_isSignal/F",prefix));
    }
};
#endif
