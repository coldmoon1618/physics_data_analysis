#ifndef MY_CUT_FLOW
#define MY_CUT_FLOW

#include <iostream>

struct MyCutFlow{
};

inline void incCount(bool isSignal, int *sigCount, int *bkgCount){
    if(isSignal)*sigCount+=1;
    else        *bkgCount+=1;
}

//name, indepent cut, accumulative cut, total signal events
inline void printEff(const char *cutName, int sig_cut, int sig_last_cut, int tot_sig){
    cout<<cutName << endl;
    cout<<"\tRel eff="<< sig_cut*.5/(sig_last_cut*.5);
    cout<<"\tAbs eff="<< sig_cut*.5/(tot_sig*.5);
    cout<<endl;
}
inline void printEff(const char *cutName, int ind_sig_cut, int acc_sig_cut, int sig_last_cut, int tot_sig){
    cout<<cutName<<"\tind:"<<ind_sig_cut<<"\tacc:"<<acc_sig_cut<<endl;
    cout<<"\tRel Acc Eff="<< acc_sig_cut*.5/(sig_last_cut*.5);
    cout<<"\tAbs Acc Eff="<< acc_sig_cut*.5/(tot_sig*.5);
    cout<<"\tAbs Ind Eff="<< ind_sig_cut*.5/(tot_sig*.5);
    cout<<endl;
    cout<<endl;
}
inline void printRej(const char *cutName, int ind_bkg_cut, int acc_bkg_cut, int bkg_last_cut, int tot_bkg){
    cout<<cutName<<endl;
    cout<<"\tRel Acc Rej="<< 1-acc_bkg_cut*.5/(bkg_last_cut*.5);
    cout<<"\tAbs Acc Rej="<< 1-acc_bkg_cut*.5/(tot_bkg*.5);
    cout<<"\tAbs Ind Rej="<< 1-ind_bkg_cut*.5/(tot_bkg*.5);
    cout<<endl;
}

#endif
