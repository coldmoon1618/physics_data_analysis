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

//p and cos(theta* cuts applied)
void SigMCKinematics() {
    bool bkgSub     = 1;
    bool thetaCut   = 1;
    const float minTheta = 0.3;
    const float maxTheta = 2.6;
    const float cosThetaCut = 0.92;
    const float minP     = 0.06;
    const float pi  = TMath::Pi();
    int numPhi = 0;
    int numB0Phi = 0;
    const float maxM = 4;

////////////////////////////////////////// MC phi ////////////////////////////////////////////////
    TH1F *h_p_phi           = new TH1F("h_p_phi","p(#phi)",                     60,0.,maxM);
    TH1F *h_p_phi_K         = new TH1F("h_p_phi_K","p(K)",                      92,0,maxM);
    TH1F *h_phi_phi_K       = new TH1F("h_phi_phi_K","#phi(K)",                 40,-pi,pi);
    TH1F *h_theta_phi_K     = new TH1F("h_theta_phi_K","#theta(K)",             40,0,pi);

    TH1F *h_pstar_phi       = new TH1F("h_pstar_phi","p*(#phi)",                60,0.,maxM);
    TH1F *h_pstar_phi_K     = new TH1F("h_pstar_phi_K","p*(K)",                 92,0,maxM);
    TH1F *h_phistar_phi_K   = new TH1F("h_phistar_phi_K","#phi*(K)",            40,-pi,pi);
    TH1F *h_thetastar_phi_K = new TH1F("h_thetastar_phi_K","cos(#theta*(K))",   40,-1,1);
////////////////////////  MC B0  ////////////////////////////////////////////////
    TH1F *h_p_B0_phi        = new TH1F("h_p_B0_phi","",               60,0.,maxM);
    TH1F *h_pstar_B0_phi    = new TH1F("h_pstar_B0_phi","",           60,0.,maxM);

//////////////////////////////////////////////////////////
    TChain *PhiGen = new TChain("tree");                //
    TChain *B0gen = new TChain("tree");                 //
                                                        //
    PhiGen -> AddFile("MCBphiK_phi_MC.root");           //
    B0gen -> AddFile("MCBphiK_B0_MC.root");             //
                                                        //
    int KgenEntries;                                    //
    KgenEntries = PhiGen -> GetEntries();               //
    int B0Entries;                                      //
    B0Entries = B0gen -> GetEntries();                  //
//////////////////////////////////////////////////////////

////////////////////////////////////////// MC phi ////////////////////////////////////////////////
    double  p_phi;
    PhiGen -> SetBranchAddress("p", &p_phi);
    double  dau0_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmPDG__bc", &dau0_phi);
    double  dau1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmPDG__bc", &dau1_phi);
    double  dau2_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo2__cmPDG__bc", &dau2_phi);
    double  p0_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmp__bc", &p0_phi);
    double  p1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmp__bc", &p1_phi);
    double  theta0_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmtheta__bc", &theta0_phi);
    double  theta1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmtheta__bc", &theta1_phi);
    double  phi0_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmphi__bc", &phi0_phi);
    double  phi1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmphi__bc", &phi1_phi);

    double  motherPDG;
    PhiGen -> SetBranchAddress("mcMother__boPDG__bc", &motherPDG);
    
    double  pstar_phi;
    PhiGen -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_phi);
    double  pstar0_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bop__bc__bc", &pstar0_phi);
    double  pstar1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bop__bc__bc", &pstar1_phi);
    double  gen_phistar0;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bophi__bc__bc", &gen_phistar0);
    double  phistar1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bophi__bc__bc", &phistar1_phi);
    double  thetastar0_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__botheta__bc__bc", &thetastar0_phi);
    double  thetastar1_phi;
    PhiGen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__botheta__bc__bc", &thetastar1_phi);
////////////////////////  MC B0  ////////////////////////////////////////////////
    double  dau0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmPDG__bc", &dau0_B0);
    double  dau1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmPDG__bc", &dau1_B0);
    double  dau2_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo2__cmPDG__bc", &dau2_B0);
    double  p0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmp__bc", &p0_B0);
    double  p1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmp__bc", &p1_B0);
    double  theta0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmtheta__bc", &theta0_B0);
    double  theta1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmtheta__bc", &theta1_B0);
    double  phi0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmphi__bc", &phi0_B0);
    double  phi1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmphi__bc", &phi1_B0);
    
    double  pstar_B0;
    B0gen -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_B0);
    double  pstar0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bop__bc__bc", &pstar0_B0);
    double  pstar1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bop__bc__bc", &pstar1_B0);
    double  phistar0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bophi__bc__bc", &phistar0_B0);
    double  phistar1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bophi__bc__bc", &phistar1_B0);
    double  thetastar0_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__botheta__bc__bc", &thetastar0_B0);
    double  thetastar1_B0;
    B0gen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__botheta__bc__bc", &thetastar1_B0);

    ////////////////////////////////////////// MC phi ////////////////////////////////////////////////
    for(int i=0; i<KgenEntries; i++){
        PhiGen -> GetEntry(i);
        if(dau2_phi!=-999)continue;//has more than two daughters
        if(p0_phi<=minP||p1_phi<=minP)continue;
        if(thetaCut&&(abs(cos(thetastar0_phi))>cosThetaCut||abs(cos(thetastar1_phi))>cosThetaCut))continue;
        if(motherPDG!=511&&motherPDG!=-511)continue;
        if((dau0_phi==321&&dau1_phi==-321)||(dau0_phi==-321&&dau1_phi==321)){
            h_p_phi         ->Fill(p_phi);
            h_p_phi_K      ->Fill(p0_phi);
            h_p_phi_K      ->Fill(p1_phi);
            h_phi_phi_K    ->Fill(phi0_phi);
            h_phi_phi_K    ->Fill(phi1_phi);
            h_theta_phi_K  ->Fill(theta0_phi);
            h_theta_phi_K  ->Fill(theta1_phi);
            
            h_pstar_phi         ->Fill(pstar_phi);
            h_pstar_phi_K      ->Fill(pstar0_phi);
            h_pstar_phi_K      ->Fill(pstar1_phi);
            h_phistar_phi_K    ->Fill(gen_phistar0);
            h_phistar_phi_K    ->Fill(phistar1_phi);
            h_thetastar_phi_K  ->Fill(cos(thetastar0_phi));
            h_thetastar_phi_K  ->Fill(cos(thetastar1_phi));

            numPhi++;
        }
    }//phi
    ////////////////////////  MC B0  ////////////////////////////////////////////////
    for(int i=0; i<B0Entries; i++){
        B0gen -> GetEntry(i);
        if(dau2_B0!=-999)continue;
        //K_S0 310 phi 333
        if( dau0_B0==333 && dau1_B0==310)
        {
            h_p_B0_phi   ->Fill(p0_B0);
            h_pstar_B0_phi  ->Fill(pstar0_B0);

            numB0Phi++;
        }
        else if( dau0_B0==310 && dau1_B0==333)
        {
            h_p_B0_phi       ->Fill(p1_B0);
            h_pstar_B0_phi   ->Fill(pstar1_B0);

            numB0Phi++;
        }
    }//B0

    ///////////////////////////////////////// PLOTS //////////////////////////////////////////////////
    //Lab frame plots
    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",1200,800);
    c1 -> Divide(2,2,0.01,0.01,0);
    c1 -> cd(1);
    h_p_phi             -> SetLineColor(kRed);
    h_p_phi             -> Draw();
    h_p_B0_phi          -> SetLineColor(kViolet-7);
    h_p_B0_phi          -> Draw("same");
    auto leg1 = new TLegend(0.,0.8,0.5,0.9);
    leg1->AddEntry(h_p_phi,             "All phi ->K+K- from B0 in acceptance");
    leg1->AddEntry(h_p_B0_phi,          "All phi from B0->phi KS (no acc. cut)");
    leg1->Draw();
    gPad -> SetLogy();
    c1 -> cd(2);
    h_p_phi_K          -> SetLineColor(kRed);
    h_p_phi_K          -> Draw();
    gPad -> SetLogy();
    c1 -> cd(3);
    h_phi_phi_K        -> SetLineColor(kRed);
    h_phi_phi_K        -> SetMinimum(0.);
    h_phi_phi_K        -> Draw();
    c1 -> cd(4);
    h_theta_phi_K      -> SetLineColor(kRed);
    h_theta_phi_K      -> SetMinimum(0.);
    h_theta_phi_K      -> Draw();

    //CoM frame plots
    gStyle->SetOptStat(0);
    TCanvas *c2 = new TCanvas("c2","c2",1200,800);
    c2 -> Divide(2,2,0.01,0.01,0);
    c2 -> cd(1);
    h_pstar_phi             -> SetLineColor(kRed);
    h_pstar_phi             -> Draw();
    h_pstar_B0_phi          -> SetLineColor(kViolet-7);
    h_pstar_B0_phi          -> Draw("same");
    auto leg2 = new TLegend(0.,0.8,0.5,0.9);
    leg2->AddEntry(h_pstar_phi,         "All phi ->K+K- from B0 in acceptance");
    leg2->AddEntry(h_pstar_B0_phi,      "All phi from B0->phi KS (no acc. cut)");
    leg2->Draw();
    gPad -> SetLogy();
    c2 -> cd(2);
    h_pstar_phi_K          -> SetLineColor(kRed);
    h_pstar_phi_K          -> Draw();
    gPad -> SetLogy();
    c2 -> cd(3);
    h_phistar_phi_K        -> SetLineColor(kRed);
    h_phistar_phi_K        -> SetMinimum(0.);
    h_phistar_phi_K        -> Draw();
    c2 -> cd(4);
    h_thetastar_phi_K      -> SetLineColor(kRed);
    h_thetastar_phi_K      -> SetMinimum(0.);
    h_thetastar_phi_K      -> Draw();

    return ;
}
