//The plots show the dependency of nCDCHits on theta and pt
//Counters show that nCDCHits increase with llayer reached
//Cut on nCDCHits essentially cutting on llayer, and in turn the pt (or theta)
#include <iostream>
#include <fstream>
#include <cmath>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <TMath.h>
#include <TChain.h>

void setStyle(TH1F *h, int color, const char *unit, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(2);
    h -> GetXaxis() -> SetTitle(unit);
    h -> SetTitle(title);
}
void setStyle(TH1F *h, int color){
    h -> SetLineColor(color);
    h -> SetLineWidth(2);
}
void setStyleTH2F(TH2F *H, const char *xtitle, const char *ytitle, const char *title){
    H -> GetXaxis() -> SetTitle(xtitle);
    H -> GetYaxis() -> SetTitle(ytitle);
    H -> SetTitle(title);
}
void normalize(TH1F *h){
    double scale = 1.0/(h->Integral());
    h->Scale(scale);
}
void all_cand_nCDCHits_study() {

    bool SigMC = 0;
    bool logZ = 1;

    TString fileName("");
    if(SigMC) fileName+="A_BphiK_phi_loose.root";
    else      fileName+="A_MC13a_combo_phi_loose.root";
    //////////////////////////////////////////////////
    TChain *PhiAll = new TChain("tree");            //
                                                    //
    PhiAll -> AddFile(fileName);                    //
                                                    //
    int allEntries = PhiAll -> GetEntries();        //
    //////////////////////////////////////////////////
    TH2F *H_pt_llayer     = new TH2F("H_pt_llayer","",  120,0,3,    60,0,60);
    TH2F *H_theta_llayer  = new TH2F("H_theta_llayer","", 128,0,3.2,  60,0,60);
    TH2F *H_pt_nHit       = new TH2F("H_pt_nHit","",    120,0,3,    200,0,200);
    TH2F *H_theta_nHit    = new TH2F("H_theta_nHit","",   128,0,3.2,  200,0,200);

    TH2F *H1_pt_nHit      = new TH2F("H1_pt_nHit","",   120,0,3,    250,0,250);
    TH2F *H2_pt_nHit      = new TH2F("H2_pt_nHit","",   120,0,3,    250,0,250);

    TH2F *H1_pt_llayer    = new TH2F("H1_pt_llayer","", 120,0,3,    60,0,60);
    TH2F *H2_pt_llayer    = new TH2F("H2_pt_llayer","", 120,0,3,    60,0,60);

    TH2F *H1_theta_nHit   = new TH2F("H1_theta_nHit","",  128,0,3.2,  250,0,250);
    TH2F *H2_theta_nHit   = new TH2F("H2_theta_nHit","",  128,0,3.2,  250,0,250);

    TH2F *H1_theta_llayer = new TH2F("H1_theta_llayer","",128,0,3.2,  60,0,60);
    TH2F *H2_theta_llayer = new TH2F("H2_theta_llayer","",128,0,3.2,  60,0,60);

    TH2F *H1_llayer_nHit  = new TH2F("H1_llayer_nHit","", 60,0,60,    250,0,250);
    TH2F *H2_llayer_nHit  = new TH2F("H2_llayer_nHit","", 60,0,60,    250,0,250);

    TH1F *h1_nhit         = new TH1F("h1_nhit","",        50,0,250);
    TH1F *h2_nhit         = new TH1F("h2_nhit","",        50,0,250);

    TH1F *h1_llayer       = new TH1F("h1_llayer","",      60,0,60);

    TH1F *h1_d0           = new TH1F("h1_d0","",          50,0,5);
    TH1F *h2_d0           = new TH1F("h2_d0","",          50,0,5);

    TH1F *h1_z0           = new TH1F("h1_z0","",          50,0,5);
    TH1F *h2_z0           = new TH1F("h2_z0","",          50,0,5);

    TH1F *hA_M            = new TH1F("hA_M","",           80,0.98,1.06);
    TH1F *hB_M            = new TH1F("hB_M","",           80,0.98,1.06);
    TH1F *hC_M            = new TH1F("hC_M","",           80,0.98,1.06);

    TH1F *hA_M_mat        = new TH1F("hA_M_mat","",       80,0.98,1.06);
    TH1F *hB_M_mat        = new TH1F("hB_M_mat","",       80,0.98,1.06);
    TH1F *hC_M_mat        = new TH1F("hC_M_mat","",       80,0.98,1.06);

    ////////////////////////////////////////// entries ////////////////////////////////////////////////
    double  all_M_phi;
    PhiAll -> SetBranchAddress("M", &all_M_phi);
    double  all_p_phi;
    PhiAll -> SetBranchAddress("p", &all_p_phi);
    double  all_pstar;
    if(SigMC)PhiAll -> SetBranchAddress("useCMSFrame__bop__bc",&all_pstar);
    double  all_p0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmp__bc", &all_p0);
    double  all_p1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmp__bc", &all_p1);
    double  all_pt0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmpt__bc", &all_pt0);
    double  all_pt1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmpt__bc", &all_pt1);
    double  all_theta0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmtheta__bc", &all_theta0);
    double  all_theta1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmtheta__bc", &all_theta1);
    double  all_KID0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmkaonID__bc", &all_KID0);
    double  all_KID1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmkaonID__bc", &all_KID1);

    double  all_inCDCacc0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmthetaInCDCAcceptance__bc", &all_inCDCacc0);
    double  all_inCDCacc1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmthetaInCDCAcceptance__bc", &all_inCDCacc1);
    double  all_nCDChits0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmnCDCHits__bc", &all_nCDChits0);
    double  all_nCDChits1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmnCDCHits__bc", &all_nCDChits1);
    double  all_lastLayer0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmlastCDCLayer__bc", &all_lastLayer0);
    double  all_lastLayer1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmlastCDCLayer__bc", &all_lastLayer1);

    double  all_d0_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmd0__bc", &all_d0_0);
    double  all_d0_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmd0__bc", &all_d0_1);
    double  all_z0_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmz0__bc", &all_z0_0);
    double  all_z0_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmz0__bc", &all_z0_1);
    double  all_isSignal;
    PhiAll -> SetBranchAddress("isSignal", &all_isSignal);

    int n01=0;
    int n02=0;
    int n03=0;

    int nTot_candi = 0;
    int nTrk_passHits = 0;
    int nPhi_passHits = 0;
    int nTrk_passLlayer = 0;
    int nPhi_passLlayer = 0;
    
    const int llayer = 55;
    
    int layer20 = 0;
    int layer21 = 0;
    int layer22 = 0;
    int layer23 = 0;
    int layer24 = 0;

    gStyle->SetOptStat(0);
    for(int i=0; i < allEntries; i++){
        PhiAll -> GetEntry(i);
        if(SigMC&&all_pstar<2.2)continue;
        //(all_KID0>0.1         && all_KID1>0.1)
        //&& (all_theta0>0.3 && all_theta0<2.6)
        //&& (all_theta1>0.3 && all_theta1<2.6)
        //&& (all_inCDCacc0        && all_inCDCacc1)
        //&& (all_nCDChits0>20     && all_nCDChits1>20)
        //&& (all_d0_0<0.5         && all_d0_1<0.5)
        //&& (abs(all_z0_0)<2      && abs(all_z0_1)<2)
        if(
            //all_isSignal==1&&
            all_M_phi>0.98&&all_M_phi<1.06
        ){
            nTot_candi++;
            h1_nhit -> Fill(all_nCDChits0);
            h1_nhit -> Fill(all_nCDChits1);
            if(all_nCDChits0>20)nTrk_passHits++;
            if(all_nCDChits1>20)nTrk_passHits++;
            if(all_nCDChits0>20&&all_nCDChits1>20)nPhi_passHits++;
            h1_llayer->Fill(all_lastLayer0);
            h1_llayer->Fill(all_lastLayer1);
            if(all_lastLayer0>20)nTrk_passLlayer++;
            if(all_lastLayer1>20)nTrk_passLlayer++;
            if(all_lastLayer0>20&&all_lastLayer1>20)nPhi_passLlayer++;

            H_pt_llayer  -> Fill(all_pt0,      all_lastLayer0);
            H_pt_llayer  -> Fill(all_pt1,      all_lastLayer1);
            
            H_theta_llayer -> Fill(all_theta0,     all_lastLayer0);
            H_theta_llayer -> Fill(all_theta1,     all_lastLayer1);

            H_pt_nHit  -> Fill(all_pt0,      all_nCDChits0);
            H_pt_nHit  -> Fill(all_pt1,      all_nCDChits1);
            
            H_theta_nHit -> Fill(all_theta0,     all_nCDChits0);
            H_theta_nHit -> Fill(all_theta1,     all_nCDChits1);

        }//isSignal&&0.98<M<1.06
        if(all_M_phi>0.98&&all_M_phi<1.06){
            if(all_lastLayer0==20)layer20++;
            if(all_lastLayer1==20)layer20++;
            if(all_lastLayer0==21)layer21++;
            if(all_lastLayer1==21)layer21++;
            if(all_lastLayer0==22)layer22++;
            if(all_lastLayer1==22)layer22++;
            if(all_lastLayer0==23)layer23++;
            if(all_lastLayer1==23)layer23++;
            if(all_lastLayer0==24)layer24++;
            if(all_lastLayer1==24)layer24++;
        }
        if(true)continue;
        if(all_M_phi>0.98&&all_M_phi<1.06){
            if(all_isSignal==1){

                H1_pt_nHit    -> Fill(all_pt0,      all_nCDChits0);
                H1_pt_nHit    -> Fill(all_pt1,      all_nCDChits1);

                H1_pt_llayer  -> Fill(all_pt0,      all_lastLayer0);
                H1_pt_llayer  -> Fill(all_pt1,      all_lastLayer1);

                H1_theta_nHit   -> Fill(all_theta0,     all_nCDChits0);
                H1_theta_nHit   -> Fill(all_theta1,     all_nCDChits1);

                H1_theta_llayer -> Fill(all_theta0,     all_lastLayer0);
                H1_theta_llayer -> Fill(all_theta1,     all_lastLayer1);

                H1_llayer_nHit  -> Fill(all_lastLayer0, all_nCDChits0);
                H1_llayer_nHit  -> Fill(all_lastLayer1, all_nCDChits1);

                h1_d0 -> Fill(all_d0_0);
                h1_d0 -> Fill(all_d0_1);

                h1_z0 -> Fill(abs(all_z0_0));
                h1_z0 -> Fill(abs(all_z0_1));
            }else{

                H2_pt_nHit    -> Fill(all_pt0,      all_nCDChits0);
                H2_pt_nHit    -> Fill(all_pt1,      all_nCDChits1);

                H2_pt_llayer  -> Fill(all_pt0,      all_lastLayer0);
                H2_pt_llayer  -> Fill(all_pt1,      all_lastLayer1);

                H2_theta_nHit   -> Fill(all_theta0,     all_nCDChits0);
                H2_theta_nHit   -> Fill(all_theta1,     all_nCDChits1);

                H2_llayer_nHit  -> Fill(all_lastLayer0, all_nCDChits0);
                H2_llayer_nHit  -> Fill(all_lastLayer1, all_nCDChits1);

                H2_theta_llayer -> Fill(all_theta0,     all_lastLayer0);
                H2_theta_llayer -> Fill(all_theta1,     all_lastLayer1);

                h2_nhit -> Fill(all_nCDChits0);
                h2_nhit -> Fill(all_nCDChits1);

                h2_d0 -> Fill(all_d0_0);
                h2_d0 -> Fill(all_d0_1);

                h2_z0 -> Fill(abs(all_z0_0));
                h2_z0 -> Fill(abs(all_z0_1));
            }
        }//In the plot mass range
        //if(!all_isSignal)continue;
        hA_M_mat -> Fill(all_M_phi);
        n01++;
        if(all_nCDChits0> 0&&all_nCDChits1> 0){
            hB_M_mat->Fill(all_M_phi);
            n02++;
            if(all_nCDChits0>20&&all_nCDChits1>20){
                hC_M_mat->Fill(all_M_phi);
                n03++;
            }//nCDCHits>20
        }//nCDCHits>0
        if(all_KID0<=0.1||all_KID1<=0.1)continue;
        hA_M -> Fill(all_M_phi);
        if(all_nCDChits0> 0&&all_nCDChits1> 0){
            hB_M->Fill(all_M_phi);
            if(all_nCDChits0>20&&all_nCDChits1>20)hC_M->Fill(all_M_phi);
        }
    }//phi:all
    TCanvas *c3 = new TCanvas("c3","c3",1000,900);
    c3 -> Divide(2,2,0.01,0.01,0);
    c3 -> cd(1);
    setStyleTH2F(H_pt_llayer,"pt(GeV)","last_CDC_layer","all cand MC combo 0.98<M_#phi<1.06");
    H_pt_llayer -> Draw("colz");
    if(logZ)gPad -> SetLogz();
    c3 -> cd(2);
    setStyleTH2F(H_theta_llayer,"#theta(rad)","last_CDC_layer","all cand MC combo 0.98<M_#phi<1.06");
    H_theta_llayer -> Draw("colz");
    if(logZ)gPad -> SetLogz();
    c3 -> cd(3);
    setStyleTH2F(H_pt_nHit,"pt(GeV)","nCDCHits","all cand MC combo 0.98<M_#phi<1.06");
    H_pt_nHit -> Draw("colz");
    if(logZ)gPad -> SetLogz();
    c3 -> cd(4);
    setStyleTH2F(H_theta_nHit,"#theta(rad)","nCDCHits","all cand MC combo 0.98<M_#phi<1.06");
    H_theta_nHit -> Draw("colz");
    if(logZ)gPad -> SetLogz();
    c3->SaveAs("all_candi_pt_theta.png");

    TCanvas *c_h_hits = new TCanvas("c_h_hits","c_h_hits",450,450);
    setStyle(h1_nhit, kGreen+2, "nCDCHits","all cand MC combo 0.98<M_#phi<1.06");
    h1_nhit -> Draw();
    c_h_hits -> SaveAs("all_cand_hits_info.png");

    TCanvas *c_h_llayer = new TCanvas("c_h_llayer","c_h_llayer",450,450);
    setStyle(h1_llayer, kGreen+2, "last_CDC_layer","all cand MC combo 0.98<M_#phi<1.06");
    h1_llayer -> Draw();
    c_h_llayer -> SaveAs("all_cand_llayer_info.png");

    cout<<endl;
    cout<<"Require both dau_trks last CDC layer="<<llayer<<endl;
    printf("nTot_candi = %i, \nnPhi_passHits = %i, \nnTrk_passHits = %i",nTot_candi,nPhi_passHits,nTrk_passHits);
    cout<<endl;
    cout<<"Percentage phi cand pass hits>20 cut: "<<nPhi_passHits*1./nTot_candi<<endl;
    cout<<"Percentage dau trks pass hits>20 cut: "<<nTrk_passHits*.5/nTot_candi<<endl;
    cout<<endl;
    cout<<"Percentage phi cand pass llayer>20 cut: "<<nPhi_passLlayer*1./nTot_candi<<endl;
    cout<<"Percentage dau trks pass llayer>20 cut: "<<nTrk_passLlayer*.5/nTot_candi<<endl;

    cout<<endl;
    cout<<"Hits for layerxx: "<<endl;
    cout<<"20\t21\t22\t23\t24"<<endl;
    cout<<layer20<<"\t"<<layer21<<"\t"<<layer22<<"\t"<<layer23<<"\t"<<layer24<<endl;
    return ;

}
/*
Require both dau_trks last CDC layer=55
nTot_candi = 1932649,
nPhi_passHits = 1055784,
nTrk_passHits = 2805091
Percentage phi cand pass hits>20 cut: 0.546289
Percentage dau trks pass hits>20 cut: 0.725711

Percentage phi cand pass llayer>20 cut: 0.454032
Percentage dau trks pass llayer>20 cut: 0.652296

Hits for layerxx:
20	21	22	23	24
31158	34759	42469	32566	33163
*/
