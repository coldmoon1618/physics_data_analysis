//Signal MC sig vs bkg distributions of variables
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

void setStyle(TH1F *h, int color, const char *xtitle, const char *ytitle, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(2);
    h -> GetXaxis() -> SetTitle(xtitle);
    h -> GetYaxis() -> SetTitle(ytitle);
    h -> SetTitle(title);
}
void setStyle(TH1F *h, int color, const char *xtitle, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(2);
    h -> GetXaxis() -> SetTitle(xtitle);
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
    double scale = 1.0/(h->GetEntries());
    h->Scale(scale);
}
void SigMC_sig_vs_bkg_vars_distr() {

    TString fileName("A_BphiK_phi_loose.root");
    
    //////////////////////////////////////////////////
    TChain *PhiAll = new TChain("tree");            //
                                                    //
    PhiAll -> AddFile(fileName);                    //
                                                    //
    int allEntries = PhiAll -> GetEntries();        //
    //////////////////////////////////////////////////

    ////////////////////////////////////////// entries ////////////////////////////////////////////////
    double  all_isSignal;
    PhiAll -> SetBranchAddress("isSignal", &all_isSignal);
    double  all_M_phi;
    PhiAll -> SetBranchAddress("M", &all_M_phi);
    double  all_p_phi;
    PhiAll -> SetBranchAddress("p", &all_p_phi);
    double  all_pstar;
    PhiAll -> SetBranchAddress("useCMSFrame__bop__bc", &all_pstar);
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
    double  all_nCDChits0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmnCDCHits__bc", &all_nCDChits0);
    double  all_nCDChits1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmnCDCHits__bc", &all_nCDChits1);
    double  all_nSVDhits0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmnSVDHits__bc", &all_nSVDhits0);
    double  all_nSVDhits1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmnSVDHits__bc", &all_nSVDhits1);

    double  all_KID0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmkaonID__bc", &all_KID0);
    double  all_KID1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmkaonID__bc", &all_KID1);

    double  all_d0_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmd0__bc", &all_d0_0);
    double  all_d0_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmd0__bc", &all_d0_1);
    double  all_z0_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmz0__bc", &all_z0_0);
    double  all_z0_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmz0__bc", &all_z0_1);
    double  all_dr_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmdr__bc", &all_dr_0);
    double  all_dr_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmdr__bc", &all_dr_1);
    double  all_dz_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmdz__bc", &all_dz_0);
    double  all_dz_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmdz__bc", &all_dz_1);

    TH1F *h_sig_PID = new TH1F("h_sig_PID","",50,0,1);
    TH1F *h_bkg_PID = new TH1F("h_bkg_PID","",50,0,1);

    TH1F *h_sig_nSVDhits = new TH1F("h_sig_nSVDhits","",24,0,24);
    TH1F *h_bkg_nSVDhits = new TH1F("h_bkg_nSVDhits","",24,0,24);
    TH1F *h_sig_nCDChits = new TH1F("h_sig_nCDChits","",120,0,120);
    TH1F *h_bkg_nCDChits = new TH1F("h_bkg_nCDChits","",120,0,120);

    TH1F *h_sig_d0  = new TH1F("h_sig_d0","",50,-5,5); 
    TH1F *h_bkg_d0  = new TH1F("h_bkg_d0","",50,-5,5);
    TH1F *h_sig_z0  = new TH1F("h_sig_z0","",50,-5,5);
    TH1F *h_bkg_z0  = new TH1F("h_bkg_z0","",50,-5,5);

    TH1F *h_sig_dr  = new TH1F("h_sig_dr","",50,-5,5);
    TH1F *h_bkg_dr  = new TH1F("h_bkg_dr","",50,-5,5);
    TH1F *h_sig_dz  = new TH1F("h_sig_dz","",50,-5,5);
    TH1F *h_bkg_dz  = new TH1F("h_bkg_dz","",50,-5,5);

    TH1F *h_M_all = new TH1F("h_M_all","",80,0.98,1.06);
    TH1F *h_M_sig = new TH1F("h_M_sig","",80,0.98,1.06);
    TH1F *h_M_bkg = new TH1F("h_M_bkg","",80,0.98,1.06);
    
    gStyle->SetOptStat(0);
    int nSig = 0;
    int nBkg = 0;
    const int totSig =155880;
    const int totBkg =  5331;
    const double KIDcut = 0.05;
    int totTrkPass = 0;
    int totTrkFail = 0;

    for(int i=0; i < allEntries; i++){
        PhiAll -> GetEntry(i);
        if(all_pstar<2.2)continue;
        if(all_isSignal==1){
            h_sig_PID -> Fill(all_KID0);
            h_sig_PID -> Fill(all_KID1);
            h_sig_nCDChits -> Fill(all_nCDChits0);
            h_sig_nCDChits -> Fill(all_nCDChits1);
            h_sig_nSVDhits -> Fill(all_nSVDhits0);
            h_sig_nSVDhits -> Fill(all_nSVDhits1);

            h_sig_d0 -> Fill(all_d0_0);
            h_sig_d0 -> Fill(all_d0_1);
            h_sig_z0 -> Fill(all_z0_0);
            h_sig_z0 -> Fill(all_z0_1);

            h_sig_dr -> Fill(all_dr_0);
            h_sig_dr -> Fill(all_dr_1);
            h_sig_dz -> Fill(all_dz_0);
            h_sig_dz -> Fill(all_dz_1);
        }else{
            h_bkg_PID -> Fill(all_KID0);
            h_bkg_PID -> Fill(all_KID1);
            h_bkg_nCDChits -> Fill(all_nCDChits0);
            h_bkg_nCDChits -> Fill(all_nCDChits1);
            h_bkg_nSVDhits -> Fill(all_nSVDhits0);
            h_bkg_nSVDhits -> Fill(all_nSVDhits1);

            h_bkg_d0 -> Fill(all_d0_0);
            h_bkg_d0 -> Fill(all_d0_1);
            h_bkg_z0 -> Fill(all_z0_0);
            h_bkg_z0 -> Fill(all_z0_1);

            h_bkg_dr -> Fill(all_dr_0);
            h_bkg_dr -> Fill(all_dr_1);
            h_bkg_dz -> Fill(all_dz_0);
            h_bkg_dz -> Fill(all_dz_1);
        }
        //continue;
        if(all_M_phi>0.98&&all_M_phi<1.06
          ){
            if(all_M_phi<1.037&&all_M_phi>1.001){
                h_M_all -> Fill(all_M_phi);
                if(all_isSignal==1){
                    if(all_d0_0<0.5&&abs(all_z0_0)<2)totTrkPass++;
                    if(all_d0_1<0.5&&abs(all_z0_1)<2)totTrkPass++;
                    h_M_sig -> Fill(all_M_phi);
                    nSig++;
                }
                else{
                    if(all_d0_0>=0.5||abs(all_z0_0)>=2)totTrkFail++;
                    if(all_d0_1>=0.5||abs(all_z0_1)>=2)totTrkFail++;
                    h_M_bkg -> Fill(all_M_phi);
                    nBkg++;
                }
            }//3-sigma mass range
        }
    }//phi:all

    
    float sig = nSig*1./sqrt(nBkg);
    TCanvas *c_M = new TCanvas("c_M","c_M",500,400);
    setStyle(h_M_sig,kGreen+2);
    setStyle(h_M_bkg,kBlue+0);
    cout<<"S="<<nSig<<", B="<<nBkg<<endl;
    cout<<"IP cut TRK EFF="<<totTrkPass*0.5/nSig<<endl;
    cout<<"IP cut TRK REJ="<<totTrkFail*0.5/nBkg<<endl;

    h_M_all->Draw();
    h_M_sig->Draw("same");
    h_M_bkg->Draw("same");
    h_M_all->Draw("same");
    //KID graphs
    TCanvas *c1 = new TCanvas("c1","c1",900,400);
    c1 -> Divide(2,1,.01,.01,0);
    c1 -> cd(1);
    setStyle(h_sig_PID,kBlue+0,"KID","trks KID(BphiK, p*(#phi)>2.2GeV, SIG)");
    gPad -> SetLogy();
    h_sig_PID -> Draw();
    c1 -> cd(2);
    setStyle(h_bkg_PID,kBlue+0,"KID","trks KID(BphiK, p*(#phi)>2.2GeV, BKG)");
    gPad -> SetLogy();
    h_bkg_PID -> Draw();
    c1 -> SaveAs("SigMC_sig_vs_bkg_KID.png");

    //detector hits
    TCanvas *c2 = new TCanvas("c2","c2",900,800);
    c2 -> Divide(2,2,.01,.01,0);
    c2 -> cd(1);
    setStyle(h_sig_nCDChits,kBlue+0,"nCDChits","trks CDChits(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_nCDChits -> Draw();
    c2 -> cd(2);
    setStyle(h_bkg_nCDChits,kBlue+0,"nCDChits","trks CDChits(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_nCDChits -> Draw();
    c2 -> cd(3);
    setStyle(h_sig_nSVDhits,kBlue+0,"nSVDhits","trks SVDhits(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_nSVDhits -> Draw();
    c2 -> cd(4);
    setStyle(h_bkg_nSVDhits,kBlue+0,"nSVDhits","trks SVDhits(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_nSVDhits -> Draw();
    c2 -> SaveAs("SigMC_sig_vs_bkg_hits.png");


    //d0 z0
    TCanvas *c3 = new TCanvas("c3","c3",900,800);
    c3 -> Divide(2,2,.01,.01,0);
    c3 -> cd(1);
    setStyle(h_sig_d0,kBlue+0,"d0","trks d0(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_d0 -> Draw();
    c3 -> cd(2);
    setStyle(h_bkg_d0,kBlue+0,"d0","trks d0(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_d0 -> Draw();
    c3 -> cd(3);
    setStyle(h_sig_z0,kBlue+0,"z0","trks z0(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_z0 -> Draw();
    c3 -> cd(4);
    setStyle(h_bkg_z0,kBlue+0,"z0","trks z0(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_z0 -> Draw();
    c3 -> SaveAs("SigMC_sig_vs_bkg_d0z0.png");

    //dr dz
    TCanvas *c4 = new TCanvas("c4","c4",900,800);
    c4 -> Divide(2,2,.01,.01,0);
    c4 -> cd(1);
    setStyle(h_sig_dr,kBlue+0,"dr","trks dr(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_dr -> Draw();
    c4 -> cd(2);
    setStyle(h_bkg_dr,kBlue+0,"dr","trks dr(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_dr -> Draw();
    c4 -> cd(3);
    setStyle(h_sig_dz,kBlue+0,"dz","trks dz(BphiK, p*(#phi)>2.2GeV, SIG)");
    h_sig_dz -> Draw();
    c4 -> cd(4);
    setStyle(h_bkg_dz,kBlue+0,"dz","trks dz(BphiK, p*(#phi)>2.2GeV, BKG)");
    h_bkg_dz -> Draw();
    c4 -> SaveAs("SigMC_sig_vs_bkg_drdz.png");

    return ;
}
