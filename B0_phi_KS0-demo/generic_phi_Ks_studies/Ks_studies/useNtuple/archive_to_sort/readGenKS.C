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

void readGenKS() {

    TH1F *h1_pstar_KS0   = new TH1F("h1_pstar_KS0", "p*(KS0)",80,0.,4);
    TH1F *h1_p_KS0       = new TH1F("h1_p_KS0",     "p(KS0)",80,0.,4);
    TH1F *h1_pstar_pi    = new TH1F("h1_pstar_pi",  "p*(pi)",60,0.,4);
    TH1F *h1_p_pi        = new TH1F("h1_p_pi",      "p(pi)",60,0.,4);

    TH1F *h2_pstar_KS0   = new TH1F("h2_pstar_KS0", "",80,0.,4);
    TH1F *h2_p_KS0       = new TH1F("h2_p_KS0",     "",80,0.,4);
    TH1F *h2_pstar_pi    = new TH1F("h2_pstar_pi",  "",60,0.,4);
    TH1F *h2_p_pi        = new TH1F("h2_p_pi",      "",60,0.,4);

    //TH1F *h1_pstar_KS0   = new TH1F("h1_pstar_KS0", "p*(KS0)",40,2.2,2.8);
    //TH1F *h1_p_KS0       = new TH1F("h1_p_KS0",     "p(KS0)", 40,1.6,3.8);

    //TH1F *h2_pstar_KS0   = new TH1F("h2_pstar_KS0", "",40,2.2,2.8);
    //TH1F *h2_p_KS0       = new TH1F("h2_p_KS0",     "",40,1.6,3.8);

    int numJ1 = 0;
    int numJ2 = 0;
    for(int j=1;j<=2;j++)
    {
        TChain *KS0s = new TChain("tree");

        if(j==1) KS0s -> AddFile("MCBphiK_MC.root");
        else     KS0s -> AddFile("MC13a_KSpipi_MCTrue.root");   

        int KEntries;
        KEntries = KS0s -> GetEntries();
        cout << "j=" << j <<" entries="<<KEntries<<endl;

        double  mass;
        KS0s -> SetBranchAddress("M", &mass);
        double  p;
        KS0s -> SetBranchAddress("p", &p);
        double  pstar_KS0;
        KS0s -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_KS0);
        
        double  p0;
        KS0s -> SetBranchAddress("mcDaughter__bo0__cmp__bc", &p0);
        double  p1;
        KS0s -> SetBranchAddress("mcDaughter__bo1__cmp__bc", &p1);
        double  pstar0;
        KS0s -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bop__bc__bc", &pstar0);
        double  pstar1;
        KS0s -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bop__bc__bc", &pstar1);

        double  dau0;
        KS0s -> SetBranchAddress("mcDaughter__bo0__cmPDG__bc", &dau0);
        double  dau1;
        KS0s -> SetBranchAddress("mcDaughter__bo1__cmPDG__bc", &dau1);
        double  dau2;
        KS0s -> SetBranchAddress("mcDaughter__bo2__cmPDG__bc", &dau2);
        double  Lxy;
        KS0s -> SetBranchAddress("mcFlightDistance", &Lxy);
        double  L3D;
        KS0s -> SetBranchAddress("mcDistance", &L3D);

        for(int i=0; i<KEntries; i++){
            KS0s -> GetEntry(i);
            if(dau2!=-999)continue;//has more than two daughters
            if((dau0==211&&dau1==-211)||(dau0==-211&&dau1==211)){
                if(j==1)
                {
                    h1_p_KS0 ->Fill(p);
                    h1_p_pi ->Fill(p0);
                    h1_p_pi ->Fill(p1);
                    h1_pstar_KS0 ->Fill(pstar_KS0);
                    h1_pstar_pi ->Fill(pstar0);
                    h1_pstar_pi ->Fill(pstar1);
                    numJ1++;
                }
                else
                {
                    h2_p_KS0 ->Fill(p);
                    h2_p_pi ->Fill(p0);
                    h2_p_pi ->Fill(p1);
                    h2_pstar_KS0 ->Fill(pstar_KS0);
                    h2_pstar_pi ->Fill(pstar0);
                    h2_pstar_pi ->Fill(pstar1);
                    numJ2++;
                }
            }//KS0->pi+pi-
        }//KEntries
    }
    cout<<numJ1<<" and "<<numJ2<<endl;

    h1_pstar_KS0    ->Add(h2_pstar_KS0,-1);
    h1_p_KS0        ->Add(h2_p_KS0,-1);

    h1_pstar_pi     ->Add(h2_pstar_pi,-1);
    h1_p_pi         ->Add(h2_p_pi,-1);

    //gStyle->SetOptStat("nermou");
    gStyle->SetOptStat(0);
    TCanvas *c2 = new TCanvas("c2","c2",600,800);
    c2 -> Divide(1,2,0.01,0.01,0);
    c2 -> cd(1);
    h1_pstar_KS0 -> SetLineColor(kRed);
    h1_pstar_KS0 -> Draw();
    gPad -> SetLogy();
    c2 -> cd(2);
    h1_p_KS0 -> SetLineColor(kRed);
    h1_p_KS0 -> Draw();
    gPad -> SetLogy();

    /*
    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",1200,800);
    c1 -> Divide(2,2,0.01,0.01,0);
    c1 -> cd(1);
    h1_pstar_KS0 -> SetLineColor(kRed);
    h1_pstar_KS0 -> Draw();
    gPad -> SetLogy();
    c1 -> cd(2);
    h1_p_KS0 -> SetLineColor(kRed);
    h1_p_KS0 -> Draw();
    gPad -> SetLogy();
    c1 -> cd(3);
    h1_pstar_pi -> SetLineColor(kRed);
    h1_pstar_pi -> Draw();
    c1 -> cd(4);
    h1_p_pi -> SetLineColor(kRed);
    h1_p_pi -> Draw();
*/
    return ;
}
