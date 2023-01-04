#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB_Tag.h"
#include "MyB_cand.h"

#include "MyDecChain.h"

void FlagDecChain(TString fileReco="decChain.root") 
{

    TChain *B_MC = new TChain("B_MC");
    B_MC -> AddFile(fileReco);  
    int Entries = B_MC -> GetEntries();
    cout<<Entries<<"  events"<<endl;
    int event;
    B_MC -> SetBranchAddress("__event__",&event);
    double charge[2];
    B_MC -> SetBranchAddress("charge",charge);
    double PDG_0[2];
    B_MC -> SetBranchAddress("PDG_0",PDG_0);
    double PDG_1[2];
    B_MC -> SetBranchAddress("PDG_1",PDG_1);
    double PDG_2[2];
    B_MC -> SetBranchAddress("PDG_2",PDG_2);
    double PDG_3[2];
    B_MC -> SetBranchAddress("PDG_3",PDG_3);
    double PDG_4[2];
    B_MC -> SetBranchAddress("PDG_4",PDG_4);
    double PDG_5[2];
    B_MC -> SetBranchAddress("PDG_5",PDG_5);
    double PDG_0_0[2];
    B_MC -> SetBranchAddress("PDG_0_0",PDG_0_0);
    double PDG_0_1[2];
    B_MC -> SetBranchAddress("PDG_0_1",PDG_0_1);
    double PDG_0_2[2];
    B_MC -> SetBranchAddress("PDG_0_2",PDG_0_2);
    double PDG_0_3[2];
    B_MC -> SetBranchAddress("PDG_0_3",PDG_0_3);
    double PDG_1_0[2];
    B_MC -> SetBranchAddress("PDG_1_0",PDG_1_0);
    double PDG_1_1[2];
    B_MC -> SetBranchAddress("PDG_1_1",PDG_1_1);
    double PDG_1_2[2];
    B_MC -> SetBranchAddress("PDG_1_2",PDG_1_2);
    double PDG_1_3[2];
    B_MC -> SetBranchAddress("PDG_1_3",PDG_1_3);

    int B_Chain[2];
    TTree   *Bflag = new TTree("Bflag","");
    Bflag -> Branch("B0Chain",&B_Chain[0],"B0Chain/I");
    Bflag -> Branch("B1Chain",&B_Chain[1],"B1Chain/I");

    TFile fOut("Bflag.root","recreate");

    const int nChnl = 36;
    int counter[nChnl];
    int inclCnt = 0;
    for(int i=0; i<nChnl; i++){
        counter[i]=0;
    }

    const int gamma=  22;
    const int D0   = 421;
    const int Dst0 = 423;//D*0
    const int Dpl  = 411;//D+
    const int Dstpl= 413;//D*+
    const int D_spl= 431;//D_s*+
    const int D_sstpl= 433;//D_s*+
    const int pipl = 211;//pi+
    const int pi0  = 111;
    const int K_S0 = 310;
    const int Kpl  = 321;
    const int Jpsi = 443;
    const int proton=2212;
    const int Lambda=4112;

    for(int i=0; i < Entries; i++)
    {
        B_MC      -> GetEntry(i);
        B_Chain[0] = -9;
        B_Chain[1] = -9;

        for(int iB=0; iB<2; iB++){
            if(      PDG_0[iB]*charge[iB]== -D0     && PDG_1[iB]*charge[iB]== pipl){
                if(  isnan(PDG_2[iB]) || PDG_2[iB]==gamma){
                    B_Chain[iB]=0;  counter[0]++;
                }else if(  PDG_2[iB]*charge[iB]== pi0 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=1;  counter[1]++;
                }else if(  PDG_2[iB]*charge[iB]== pi0 
                        && PDG_3[iB]*charge[iB]== pi0 
                        && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=2;  counter[2]++;
                }else if(  PDG_2[iB]*charge[iB]== pipl 
                        && PDG_3[iB]*charge[iB]==-pipl 
                        && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=3;  counter[3]++;
                }else if(  PDG_2[iB]*charge[iB]== pipl 
                        && PDG_3[iB]*charge[iB]==-pipl
                        && PDG_4[iB]*charge[iB]== pi0 
                    && (isnan(PDG_5[iB]) || PDG_5[iB]==gamma)){
                    B_Chain[iB]=4;  counter[4]++;
                }
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== -D0      && PDG_1[iB]*charge[iB]== Dpl){
                if(  isnan(PDG_2[iB]) || PDG_2[iB]==gamma){
                    B_Chain[iB]=5;  counter[5]++;
                }else if(  PDG_2[iB]*charge[iB]== K_S0 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=6;  counter[6]++;
                }
            }
            else if(PDG_0[iB]*charge[iB]== -Dst0    && PDG_1[iB]*charge[iB]== Dpl
                    && PDG_2[iB]*charge[iB]== K_S0 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=7;  counter[7]++;
            }
            else if(PDG_0[iB]*charge[iB]== -D0      && PDG_1[iB]*charge[iB]== Dstpl
                    && PDG_2[iB]*charge[iB]== K_S0 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=8;  counter[8]++;
            }
            else if(PDG_0[iB]*charge[iB]== -Dst0    && PDG_1[iB]*charge[iB]== Dstpl
                    && PDG_2[iB]*charge[iB]== K_S0 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=9;  counter[9]++;
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== -D0      && PDG_1[iB]*charge[iB]== D0
                    && PDG_2[iB]*charge[iB]== Kpl 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=10; counter[10]++;
            }
            else if(PDG_0[iB]*charge[iB]== -Dst0    && PDG_1[iB]*charge[iB]== D0
                    && PDG_2[iB]*charge[iB]== Kpl 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=11; counter[11]++;
            }
            else if(PDG_0[iB]*charge[iB]== -D0      && PDG_1[iB]*charge[iB]== Dst0
                    && PDG_2[iB]*charge[iB]== Kpl 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=12; counter[12]++;
            }
            else if(PDG_0[iB]*charge[iB]== -Dst0    && PDG_1[iB]*charge[iB]== Dst0
                    && PDG_2[iB]*charge[iB]== Kpl 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=13; counter[13]++;
            }////////////////////
            else if(PDG_0[iB]*charge[iB]==  D_spl   && PDG_1[iB]*charge[iB]== pipl
                    && (isnan(PDG_2[iB]) || PDG_2[iB]==gamma)){
                    B_Chain[iB]=14; counter[14]++;
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== -Dst0    && PDG_1[iB]*charge[iB]== pipl){
                if(  isnan(PDG_2[iB]) || PDG_2[iB]==gamma){
                    B_Chain[iB]=15; counter[15]++;
                }else if(  PDG_2[iB]*charge[iB]== pi0 
                    && (isnan(PDG_3[iB]) || PDG_3[iB]==gamma)){
                    B_Chain[iB]=16; counter[16]++;
                }else if(  PDG_2[iB]*charge[iB]== pi0 
                        && PDG_3[iB]*charge[iB]== pi0 
                        && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=17; counter[17]++;
                }else if(  PDG_2[iB]*charge[iB]== pipl 
                        && PDG_3[iB]*charge[iB]==-pipl 
                        && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=18; counter[18]++;
                }else if(  PDG_2[iB]*charge[iB]== pipl 
                        && PDG_3[iB]*charge[iB]==-pipl
                        && PDG_4[iB]*charge[iB]== pi0 
                    && (isnan(PDG_5[iB]) || PDG_5[iB]==gamma)){
                    B_Chain[iB]=19; counter[19]++;
                }
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== D_sstpl  && PDG_1[iB]*charge[iB]== -D0
                    && (isnan(PDG_2[iB]) || PDG_2[iB]==gamma)){
                    B_Chain[iB]=20; counter[20]++;
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== D_spl    && PDG_1[iB]*charge[iB]==- Dst0
                    && (isnan(PDG_2[iB]) || PDG_2[iB]==gamma)){
                    B_Chain[iB]=21; counter[21]++;
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== -D0      && PDG_1[iB]*charge[iB]== Kpl
                    && (isnan(PDG_2[iB]) || PDG_2[iB]==gamma)){
                    B_Chain[iB]=22; counter[22]++;
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== -Dpl     && PDG_1[iB]*charge[iB]== pipl
                    && PDG_2[iB]*charge[iB]== pipl){ 
                if(  isnan(PDG_3[iB]) || PDG_3[iB]==gamma){
                    B_Chain[iB]=23; counter[23]++;
                }else if(  PDG_3[iB]*charge[iB]== pi0 
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=24; counter[24]++;
                }
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== Jpsi){
                if( PDG_1[iB]*charge[iB]== Kpl 
                    && (isnan(PDG_2[iB]) || PDG_2[iB]==gamma)){
                    B_Chain[iB]=25; counter[25]++;
                }else if(PDG_1[iB]*charge[iB]== Kpl && PDG_2[iB]*charge[iB]== pipl
                    &&   PDG_3[iB]*charge[iB]== -pipl
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=26; counter[26]++;
                }else if(PDG_1[iB]*charge[iB]== Kpl && PDG_2[iB]*charge[iB]== pi0
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=27; counter[27]++;
                }else if(PDG_1[iB]*charge[iB]== K_S0&& PDG_2[iB]*charge[iB]== pipl
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=28; counter[28]++;
                }
            }////////////////////
            else if(PDG_0[iB]*charge[iB]== -Lambda  && PDG_1[iB]*charge[iB]== proton 
                    && PDG_2[iB]*charge[iB]== pipl){ 
                if(   isnan(PDG_3[iB]) || PDG_3[iB]==gamma){
                    B_Chain[iB]=35; counter[35]++;
                }else if( PDG_3[iB]*charge[iB]== pi0 
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=29; counter[29]++;
                }else if(PDG_3[iB]*charge[iB]== pipl
                    &&   PDG_4[iB]*charge[iB]== -pipl    
                    && (isnan(PDG_5[iB]) || PDG_5[iB]==gamma)){
                    B_Chain[iB]=30; counter[30]++;
                }
            }////////////////////
            else if(PDG_1[iB]*charge[iB]== proton  && PDG_2[iB]*charge[iB]== -proton 
                    && PDG_3[iB]*charge[iB]== pipl){ 
                if( PDG_0[iB]*charge[iB]== -D0 
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=31; counter[31]++;
                }else if( PDG_0[iB]*charge[iB]== -Dst0 
                    && (isnan(PDG_4[iB]) || PDG_4[iB]==gamma)){
                    B_Chain[iB]=32; counter[32]++;
                }else if(PDG_0[iB]*charge[iB]== Dpl
                    &&   PDG_4[iB]*charge[iB]== -pipl    
                    && (isnan(PDG_5[iB]) || PDG_5[iB]==gamma)){
                    B_Chain[iB]=33; counter[33]++;
                }else if(PDG_0[iB]*charge[iB]== Dstpl
                    &&   PDG_4[iB]*charge[iB]== -pipl    
                    && (isnan(PDG_5[iB]) || PDG_5[iB]==gamma)){
                    B_Chain[iB]=34; counter[34]++;
                }
            }////////////////////
        }//B+B- pairs

        Bflag -> Fill();

    }//events
    //cout<<inclCnt/2.e6<<endl;
    for(int i=0; i<nChnl; i++){
        cout<<counter[i]<<endl;
    }
    fOut.cd();
    Bflag -> Write();
//    Bflag -> Scan();
    fOut.Close();

   //////////////////////////////////////////////////

    return;
}
