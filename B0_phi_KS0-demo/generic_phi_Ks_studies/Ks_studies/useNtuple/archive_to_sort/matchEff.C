#include <iostream>
#include <fstream>

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

void matchEff() {

    TH1F *h_pstar_gen   = new TH1F("h_pstar_gen","CMSFrame p(KS0:gen)",40,0.,2);
    TH1F *h_pstar_mat   = new TH1F("h_pstar_mat","CMSFrame p(KS0:mat)",40,0.,2);

    TChain *KSgen = new TChain("tree");
    TChain *KSmat = new TChain("tree");

    KSgen -> AddFile("MC13a_KSpipi_MCTrue.root");
    KSmat -> AddFile("MC13a_KS_compareDistance.root");

    int KgenEntries;
    KgenEntries = KSgen -> GetEntries();
    int KmatEntries;
    KmatEntries = KSmat -> GetEntries();

    double  mass;
    KSgen -> SetBranchAddress("M", &mass);
    double  p;
    KSgen -> SetBranchAddress("p", &p);
    double  pstar_KS0;
    KSgen -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_KS0);
    double  phi;
    KSgen -> SetBranchAddress("useCMSFrame__bophi__bc", &phi);
    double  theta;
    KSgen -> SetBranchAddress("useCMSFrame__botheta__bc", &theta);
    double  dau0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmPDG__bc", &dau0);
    double  dau1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmPDG__bc", &dau1);
    double  dau2;
    KSgen -> SetBranchAddress("mcDaughter__bo2__cmPDG__bc", &dau2);
    double  Lxy;
    KSgen -> SetBranchAddress("mcFlightDistance", &Lxy);
    double  L3D;
    KSgen -> SetBranchAddress("mcDistance", &L3D);

    double  pstar_KS0_matched;
    KSmat -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_KS0_matched);
    double  isMatched;
    KSmat -> SetBranchAddress("isSignal",&isMatched);

    const int nBins = 8;
    int     count_gen[nBins] = {0,0,0,0,0,0,0,0};
    int     count_mat[nBins] = {0,0,0,0,0,0,0,0};
    double  lo_cut[nBins] = { 0.0,0.06,0.2,0.5,1.0,1.5,2.0,2.5};
    double  hi_cut[nBins] = {0.06, 0.2,0.5,1.0,1.5,2.0,2.5,9.0};
    int     tot_gen = 0;
    int     tot_mat = 0;

    for(int i=0; i<KgenEntries; i++){
        KSgen -> GetEntry(i);
        if(dau2!=-999)continue;//has more than two daughters
        if((dau0==211&&dau1==-211)||(dau0==-211&&dau1==211)){
            h_pstar_gen ->Fill(pstar_KS0);
            if(     pstar_KS0>=lo_cut[0] && pstar_KS0<hi_cut[0])  count_gen[0]++;
            else if(pstar_KS0>=lo_cut[1] && pstar_KS0<hi_cut[1])  count_gen[1]++;
            else if(pstar_KS0>=lo_cut[2] && pstar_KS0<hi_cut[2])  count_gen[2]++;
            else if(pstar_KS0>=lo_cut[3] && pstar_KS0<hi_cut[3])  count_gen[3]++;
            else if(pstar_KS0>=lo_cut[4] && pstar_KS0<hi_cut[4])  count_gen[4]++;
            else if(pstar_KS0>=lo_cut[5] && pstar_KS0<hi_cut[5])  count_gen[5]++;
            else if(pstar_KS0>=lo_cut[6] && pstar_KS0<hi_cut[6])  count_gen[6]++;
            tot_gen++;
        }
    }
    for(int i=0; i<KmatEntries; i++){
        KSmat -> GetEntry(i);
        if(isMatched==1){
            h_pstar_mat ->Fill(pstar_KS0_matched);
            if(     pstar_KS0_matched>=lo_cut[0] && pstar_KS0_matched<hi_cut[0])  count_mat[0]++;
            else if(pstar_KS0_matched>=lo_cut[1] && pstar_KS0_matched<hi_cut[1])  count_mat[1]++;
            else if(pstar_KS0_matched>=lo_cut[2] && pstar_KS0_matched<hi_cut[2])  count_mat[2]++;
            else if(pstar_KS0_matched>=lo_cut[3] && pstar_KS0_matched<hi_cut[3])  count_mat[3]++;
            else if(pstar_KS0_matched>=lo_cut[4] && pstar_KS0_matched<hi_cut[4])  count_mat[4]++;
            else if(pstar_KS0_matched>=lo_cut[5] && pstar_KS0_matched<hi_cut[5])  count_mat[5]++;
            else if(pstar_KS0_matched>=lo_cut[6] && pstar_KS0_matched<hi_cut[6])  count_mat[6]++;
            tot_mat++;
        }
    }

    gStyle->SetOptStat(0);

    auto c1 = new TCanvas("c1","a ratio example");
    h_pstar_mat -> GetXaxis() -> SetTitle("GeV");
    h_pstar_mat -> GetYaxis() -> SetTitle("Entries");
    auto rp = new TRatioPlot(h_pstar_mat,h_pstar_gen);
    gPad -> SetLogy();
    c1->SetTicks(0,1);
    rp->Draw();
    rp->GetLowYaxis()->SetNdivisions(5);
    c1->Update();
    c1 -> SaveAs("recoEff.png");

    TCanvas *c2 = new TCanvas("c2","c2",800,600);
    h_pstar_mat->SetLineColor(kBlack);
    h_pstar_gen -> Draw();
    h_pstar_mat -> Draw("same");
    h_pstar_gen -> GetXaxis() -> SetTitle("GeV");
    h_pstar_gen -> GetYaxis() -> SetTitle("Entries");
    c2 -> SaveAs("Ks_pstar_MC_vs_mat.png");

    for(int i = 0; i<nBins; i++){
        int mat = count_mat[i];
        int gen = count_gen[i];
        cout<<"{["<<lo_cut[i]<<","<<hi_cut[i]<<"]:"<<mat<<"/"<<gen;
        if(gen!=0)cout<<"="<<mat/(gen*1.0);
        cout<<"},"<<endl;
    }
    cout<<"Overall "<<tot_mat<<"/"<<tot_gen<<"="<<tot_mat/(tot_gen*1.0)<<" compared to generator"<<endl;
    //higher than ChooseCut because this one didn't include 0.46,0.54 cut

    return ;
}
