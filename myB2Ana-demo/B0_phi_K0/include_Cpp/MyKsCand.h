#ifndef MY_KS_CAND
#define MY_KS_CAND

struct MyKsCand{
    float isSignal;
    float M;
    float p;
    float pstar;
    
    float dau_0_p;
    float dau_0_pt;
    float dau_0_isCloneTrack;
    
    float dau_1_p;
    float dau_1_pt;
    float dau_1_isCloneTrack;

    void setBranchAddresses(TTree *Ks_cand){
        setBranchAddresses(Ks_cand,"");
    }
    void setBranchAddresses(TTree *Ks_cand, const char* prefix){
        Ks_cand -> SetBranchAddress(Form("%sisSignal",prefix), &isSignal);
        Ks_cand -> SetBranchAddress(Form("%sM",prefix), &M);
        Ks_cand -> SetBranchAddress(Form("%sp",prefix), &p);
        Ks_cand -> SetBranchAddress(Form("%spstar",prefix), &pstar);

        Ks_cand -> SetBranchAddress(Form("%sdau_0_p",prefix), &dau_0_p);
        Ks_cand -> SetBranchAddress(Form("%sdau_0_pt",prefix), &dau_0_pt);
        Ks_cand -> SetBranchAddress(Form("%sdau_0_isCloneTrack",prefix), &dau_0_isCloneTrack);

        Ks_cand -> SetBranchAddress(Form("%sdau_1_p",prefix), &dau_1_p);
        Ks_cand -> SetBranchAddress(Form("%sdau_1_pt",prefix), &dau_1_pt);
        Ks_cand -> SetBranchAddress(Form("%sdau_1_isCloneTrack",prefix), &dau_1_isCloneTrack);
    }

    void branches(TTree *Ks_cand){
        branches(Ks_cand,"");
    }
    void branches(TTree *Ks_cand, const char* prefix){
        Ks_cand -> Branch(Form("%sisSignal",prefix), &isSignal, Form("%sisSignal/F",prefix));
        Ks_cand -> Branch(Form("%sM",prefix), &M, Form("%sM/F",prefix));
        Ks_cand -> Branch(Form("%sp",prefix), &p, Form("%sp/F",prefix));
        Ks_cand -> Branch(Form("%spstar",prefix), &pstar, Form("%spstar/F",prefix));

        Ks_cand -> Branch(Form("%sdau_0_p",prefix), &dau_0_p, Form("%sdau_0_p/F",prefix));
        Ks_cand -> Branch(Form("%sdau_0_pt",prefix), &dau_0_pt, Form("%sdau_0_pt/F",prefix));
        Ks_cand -> Branch(Form("%sdau_0_isCloneTrack",prefix), &dau_0_isCloneTrack, Form("%sdau_0_isCloneTrack/F",prefix));

        Ks_cand -> Branch(Form("%sdau_1_p",prefix), &dau_1_p, Form("%sdau_1_p/F",prefix));
        Ks_cand -> Branch(Form("%sdau_1_pt",prefix), &dau_1_pt, Form("%sdau_1_pt/F",prefix));
        Ks_cand -> Branch(Form("%sdau_1_isCloneTrack",prefix), &dau_1_isCloneTrack, Form("%sdau_1_isCloneTrack/F",prefix));
    }
};
#endif
