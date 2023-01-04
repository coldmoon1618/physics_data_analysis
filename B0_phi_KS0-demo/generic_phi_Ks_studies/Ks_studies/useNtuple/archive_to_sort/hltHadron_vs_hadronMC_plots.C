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

void hltHadron_vs_hadronMC_plots() {
    const float maxM = 4;//2. for matchEff or 3. for other uses
    const float pi  = TMath::Pi();
    const float frac = 1.0;
    int         comboEnts = 0;
    int         dataEnts   = 0;
    //////////////////////////// RECO ///////////////////////////////////////////////
    TH1F *h1_p_sband         = new TH1F("h1_p_sband","",              60,0.,maxM);
    TH1F *h1_p_bgsub         = new TH1F("h1_p_bgsub","",              60,0.,maxM);
    TH1F *h1_pi_p_sband      = new TH1F("h1_pi_p_sband","",           92,0.,maxM);
    TH1F *h1_pi_p_bgsub      = new TH1F("h1_pi_p_bgsub","",           92,0.,maxM);
    TH1F *h1_pi_phi_sband    = new TH1F("h1_pi_phi_sband","",         40,-pi,pi);
    TH1F *h1_pi_phi_bgsub    = new TH1F("h1_pi_phi_bgsub","",         40,-pi,pi);
    TH1F *h1_pi_theta_sband  = new TH1F("h1_pi_theta_sband","",       40,0.,pi);
    TH1F *h1_pi_theta_bgsub  = new TH1F("h1_pi_theta_bgsub","",       40,0.,pi);

    TH1F *h1_pstar_sband         = new TH1F("h1_pstar_sband","",          60,0.,maxM);
    TH1F *h1_pstar_bgsub         = new TH1F("h1_pstar_bgsub","",          60,0.,maxM);
    TH1F *h1_pi_pstar_sband      = new TH1F("h1_pi_pstar_sband","",       92,0.,maxM);
    TH1F *h1_pi_pstar_bgsub      = new TH1F("h1_pi_pstar_bgsub","",       92,0.,maxM);
    TH1F *h1_pi_phistar_sband    = new TH1F("h1_pi_phistar_sband","",     40,-pi,pi);
    TH1F *h1_pi_phistar_bgsub    = new TH1F("h1_pi_phistar_bgsub","",     40,-pi,pi);
    TH1F *h1_pi_thetastar_sband  = new TH1F("h1_pi_thetastar_sband","",   40,-1,1);
    TH1F *h1_pi_thetastar_bgsub  = new TH1F("h1_pi_thetastar_bgsub","",   40,-1,1);

    TH1F *h2_p_sband         = new TH1F("h2_p_sband","",              60,0.,maxM);
    TH1F *h2_p_bgsub         = new TH1F("h2_p_bgsub","",              60,0.,maxM);
    TH1F *h2_pi_p_sband      = new TH1F("h2_pi_p_sband","",           92,0.,maxM);
    TH1F *h2_pi_p_bgsub      = new TH1F("h2_pi_p_bgsub","",           92,0.,maxM);
    TH1F *h2_pi_phi_sband    = new TH1F("h2_pi_phi_sband","",         40,-pi,pi);
    TH1F *h2_pi_phi_bgsub    = new TH1F("h2_pi_phi_bgsub","",         40,-pi,pi);
    TH1F *h2_pi_theta_sband  = new TH1F("h2_pi_theta_sband","",       40,0.,pi);
    TH1F *h2_pi_theta_bgsub  = new TH1F("h2_pi_theta_bgsub","",       40,0.,pi);

    TH1F *h2_pstar_sband         = new TH1F("h2_pstar_sband","",          60,0.,maxM);
    TH1F *h2_pstar_bgsub         = new TH1F("h2_pstar_bgsub","",          60,0.,maxM);
    TH1F *h2_pi_pstar_sband      = new TH1F("h2_pi_pstar_sband","",       92,0.,maxM);
    TH1F *h2_pi_pstar_bgsub      = new TH1F("h2_pi_pstar_bgsub","",       92,0.,maxM);
    TH1F *h2_pi_phistar_sband    = new TH1F("h2_pi_phistar_sband","",     40,-pi,pi);
    TH1F *h2_pi_phistar_bgsub    = new TH1F("h2_pi_phistar_bgsub","",     40,-pi,pi);
    TH1F *h2_pi_thetastar_sband  = new TH1F("h2_pi_thetastar_sband","",   40,-1,1);
    TH1F *h2_pi_thetastar_bgsub  = new TH1F("h2_pi_thetastar_bgsub","",   40,-1,1);

    //////////////////////////// RECO ///////////////////////////////////////////////
    for(int iType=1;iType<=2;iType++){
        TChain *KS0s = new TChain("tree");                 
        if(iType==1)KS0s -> AddFile("MC13a_reco_200k.root");            
        else{
                    KS0s -> AddFile("Reco_KS_proc11_exp8.root");
        }
        int KEntries;                                       
        KEntries = KS0s -> GetEntries();                    

        double mass;
        KS0s -> SetBranchAddress("M", &mass);
        double p;
        KS0s -> SetBranchAddress("p", &p);
        double pstar_KS0;
        KS0s -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_KS0);
        double  pstar0;
        KS0s->SetBranchAddress("daughter__bo0__cmuseCMSFrame__bop__bc__bc",&pstar0);
        double  pstar1;
        KS0s->SetBranchAddress("daughter__bo1__cmuseCMSFrame__bop__bc__bc",&pstar1);
        double  phistar0;
        KS0s->SetBranchAddress("daughter__bo0__cmuseCMSFrame__bophi__bc__bc",&phistar0);
        double  phistar1;
        KS0s->SetBranchAddress("daughter__bo1__cmuseCMSFrame__bophi__bc__bc",&phistar1);
        double  thetastar0;
        KS0s->SetBranchAddress("daughter__bo0__cmuseCMSFrame__botheta__bc__bc",&thetastar0);
        double  thetastar1;
        KS0s->SetBranchAddress("daughter__bo1__cmuseCMSFrame__botheta__bc__bc",&thetastar1);

        double phi;
        KS0s -> SetBranchAddress("useCMSFrame__bophi__bc", &phi);
        double theta;
        KS0s -> SetBranchAddress("useCMSFrame__botheta__bc", &theta);
        double decayLength;
        KS0s -> SetBranchAddress("flightDistance", &decayLength);
        double decayLengthErr;
        KS0s -> SetBranchAddress("flightDistanceErr", &decayLengthErr);
        double sig_2D = -9999;
        double rec_p0;
        KS0s -> SetBranchAddress("daughter__bo0__cmp__bc", &rec_p0);
        double rec_p1;
        KS0s -> SetBranchAddress("daughter__bo1__cmp__bc", &rec_p1);
        double rec_phi0;
        KS0s -> SetBranchAddress("daughter__bo0__cmphi__bc", &rec_phi0);
        double rec_phi1;
        KS0s -> SetBranchAddress("daughter__bo1__cmphi__bc", &rec_phi1);
        double rec_theta0;
        KS0s -> SetBranchAddress("daughter__bo0__cmtheta__bc", &rec_theta0);
        double rec_theta1;
        KS0s -> SetBranchAddress("daughter__bo1__cmtheta__bc", &rec_theta1);

        //////////////////////////// COMBO RECO ///////////////////////////////////////////////
        if(iType==1)for(int i=0; i<KEntries; i++)
        {
            KS0s -> GetEntry(i);
            //to match the range in the fitting
            if(mass<=0.46||mass>=0.54)continue;
            sig_2D = decayLength/decayLengthErr;
            //if(sig_2D<3)continue;//yes, it's verified this continue works properly
            //if(pstar_KS0<2.2)continue;
            if((mass<0.4748&&mass>0.47)||(mass<0.52&&mass>0.5152))
            {//side bands
                h1_p_sband        -> Fill(p);
                h1_pi_p_sband     -> Fill(rec_p0);
                h1_pi_p_sband     -> Fill(rec_p1);
                h1_pi_phi_sband   -> Fill(rec_phi0);
                h1_pi_phi_sband   -> Fill(rec_phi1);
                h1_pi_theta_sband -> Fill(rec_theta0);
                h1_pi_theta_sband -> Fill(rec_theta1);

                h1_pstar_sband        -> Fill(pstar_KS0);
                h1_pi_pstar_sband     -> Fill(pstar0);
                h1_pi_pstar_sband     -> Fill(pstar1);
                h1_pi_phistar_sband   -> Fill(phistar0);
                h1_pi_phistar_sband   -> Fill(phistar1);
                h1_pi_thetastar_sband -> Fill(cos(thetastar0));
                h1_pi_thetastar_sband -> Fill(cos(thetastar1));

                comboEnts--;
            }
            else if(mass>0.4929&&mass<0.5025)
            {//3 sigmas
                h1_p_bgsub        -> Fill(p);
                h1_pi_p_bgsub     -> Fill(rec_p0);
                h1_pi_p_bgsub     -> Fill(rec_p1);
                h1_pi_phi_bgsub   -> Fill(rec_phi0);
                h1_pi_phi_bgsub   -> Fill(rec_phi1);
                h1_pi_theta_bgsub -> Fill(rec_theta0);
                h1_pi_theta_bgsub -> Fill(rec_theta1);

                h1_pstar_bgsub        -> Fill(pstar_KS0);
                h1_pi_pstar_bgsub     -> Fill(pstar0);
                h1_pi_pstar_bgsub     -> Fill(pstar1);
                h1_pi_phistar_bgsub   -> Fill(phistar0);
                h1_pi_phistar_bgsub   -> Fill(phistar1);
                h1_pi_thetastar_bgsub -> Fill(cos(thetastar0));
                h1_pi_thetastar_bgsub -> Fill(cos(thetastar1));

                comboEnts++;
            }//signal region
        }//combo reco
        //////////////////////////// PROC11 RECO ///////////////////////////////////////////////
        // 30975./23706.=1.306631233
        if(iType==2)for(int i=0; i<KEntries/50; i++)
        //if(iType==2)for(int i=0; i<355797; i++)
        {
            KS0s -> GetEntry(i);
            //to match the range in the fitting
            if(mass<=0.46||mass>=0.54)continue;
            sig_2D = decayLength/decayLengthErr;
            //if(sig_2D<3)continue;//yes, it's verified this continue works properly
            //if(pstar_KS0<2.2)continue;
            if((mass<0.4748&&mass>0.47)||(mass<0.52&&mass>0.5152))
            {//side bands
                h2_p_sband        -> Fill(p);
                h2_pi_p_sband     -> Fill(rec_p0);
                h2_pi_p_sband     -> Fill(rec_p1);
                h2_pi_phi_sband   -> Fill(rec_phi0);
                h2_pi_phi_sband   -> Fill(rec_phi1);
                h2_pi_theta_sband -> Fill(rec_theta0);
                h2_pi_theta_sband -> Fill(rec_theta1);

                h2_pstar_sband        -> Fill(pstar_KS0);
                h2_pi_pstar_sband     -> Fill(pstar0);
                h2_pi_pstar_sband     -> Fill(pstar1);
                h2_pi_phistar_sband   -> Fill(phistar0);
                h2_pi_phistar_sband   -> Fill(phistar1);
                h2_pi_thetastar_sband -> Fill(cos(thetastar0));
                h2_pi_thetastar_sband -> Fill(cos(thetastar1));

                dataEnts--;
            }
            else if(mass>0.4929&&mass<0.5025)
            {//3 sigmas
                h2_p_bgsub        -> Fill(p);
                h2_pi_p_bgsub     -> Fill(rec_p0);
                h2_pi_p_bgsub     -> Fill(rec_p1);
                h2_pi_phi_bgsub   -> Fill(rec_phi0);
                h2_pi_phi_bgsub   -> Fill(rec_phi1);
                h2_pi_theta_bgsub -> Fill(rec_theta0);
                h2_pi_theta_bgsub -> Fill(rec_theta1);

                h2_pstar_bgsub        -> Fill(pstar_KS0);
                h2_pi_pstar_bgsub     -> Fill(pstar0);
                h2_pi_pstar_bgsub     -> Fill(pstar1);
                h2_pi_phistar_bgsub   -> Fill(phistar0);
                h2_pi_phistar_bgsub   -> Fill(phistar1);
                h2_pi_thetastar_bgsub -> Fill(cos(thetastar0));
                h2_pi_thetastar_bgsub -> Fill(cos(thetastar1));

                dataEnts++;
            }//signal region
        }//proc reco
    }//iType

    /////////////////////// PLOTS //////////////////////////////////////////////////
    double      toCombo = comboEnts/(dataEnts*1.0);

    h1_p_bgsub        -> Add(h1_p_sband,-frac); 
    h1_pi_p_bgsub     -> Add(h1_pi_p_sband,-frac); 
    h1_pi_phi_bgsub   -> Add(h1_pi_phi_sband,-frac); 
    h1_pi_theta_bgsub -> Add(h1_pi_theta_sband,-frac); 

    h1_pstar_bgsub        -> Add(h1_pstar_sband,-frac); 
    h1_pi_pstar_bgsub     -> Add(h1_pi_pstar_sband,-frac); 
    h1_pi_phistar_bgsub   -> Add(h1_pi_phistar_sband,-frac); 
    h1_pi_thetastar_bgsub -> Add(h1_pi_thetastar_sband,-frac); 

    h2_p_bgsub        -> Add(h2_p_sband,-frac); 
    h2_pi_p_bgsub     -> Add(h2_pi_p_sband,-frac); 
    h2_pi_phi_bgsub   -> Add(h2_pi_phi_sband,-frac); 
    h2_pi_theta_bgsub -> Add(h2_pi_theta_sband,-frac); 

    h2_pstar_bgsub        -> Add(h2_pstar_sband,-frac); 
    h2_pi_pstar_bgsub     -> Add(h2_pi_pstar_sband,-frac); 
    h2_pi_phistar_bgsub   -> Add(h2_pi_phistar_sband,-frac); 
    h2_pi_thetastar_bgsub -> Add(h2_pi_thetastar_sband,-frac); 

    double myScale;
    //Lab frame plots
    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",1200,800);
    c1 -> Divide(2,2,0.01,0.01,0);
    
    c1 -> cd(1);
    h1_p_bgsub           -> SetLineColor(kBlue);
    h1_p_bgsub           -> Draw();
    h1_p_bgsub           -> SetTitle("p(KS0)");
    gPad -> SetLogy();
    h2_p_bgsub           -> Scale(toCombo);
    h2_p_bgsub           -> SetLineColor(kRed);
    h2_p_bgsub           -> Draw("same");
    gPad -> SetLogy();
    auto leg1 = new TLegend(0.6,0.8,0.9,0.9);
    leg1->AddEntry(h1_p_bgsub,Form("combo MC: %i", comboEnts));
    leg1->AddEntry(h2_p_bgsub,Form("scaled data: %i", dataEnts));
    leg1->Draw();
    
    c1 -> cd(2);
    h1_pi_p_bgsub        -> SetLineColor(kBlue);
    h1_pi_p_bgsub        -> Draw();
    h1_pi_p_bgsub        -> SetTitle("p(pi)");
    gPad -> SetLogy();
    h2_pi_p_bgsub        -> Scale(toCombo);
    h2_pi_p_bgsub        -> SetLineColor(kRed);
    h2_pi_p_bgsub        -> Draw("same");
    gPad -> SetLogy();
    
    c1 -> cd(3);
    h1_pi_phi_bgsub      -> SetLineColor(kBlue);
    h1_pi_phi_bgsub      -> SetMinimum(0.);
    h1_pi_phi_bgsub      -> Draw();
    h1_pi_phi_bgsub      -> SetTitle("#phi(pi)");
    h2_pi_phi_bgsub      -> Scale(toCombo);
    h2_pi_phi_bgsub      -> SetLineColor(kRed);
    h2_pi_phi_bgsub      -> Draw("same");

    c1 -> cd(4);
    h1_pi_theta_bgsub    -> SetLineColor(kBlue);
    h1_pi_theta_bgsub    -> SetMinimum(0.);
    h1_pi_theta_bgsub    -> Draw();
    h1_pi_theta_bgsub    -> SetTitle("#theta(pi)");
    h2_pi_theta_bgsub    -> Scale(toCombo);
    h2_pi_theta_bgsub    -> SetLineColor(kRed);
    h2_pi_theta_bgsub    -> Draw("same");

    //CoM frame plots
    gStyle->SetOptStat(0);
    TCanvas *c2 = new TCanvas("c2","c2",1200,800);
    c2 -> Divide(2,2,0.01,0.01,0);

    c2 -> cd(1);
    h1_pstar_bgsub           -> SetLineColor(kBlue);
    h1_pstar_bgsub           -> Draw();
    h1_pstar_bgsub           -> SetTitle("p*(KS0)");
    gPad -> SetLogy();
    h2_pstar_bgsub           -> SetLineColor(kRed);
    h2_pstar_bgsub           -> Scale(toCombo);
    h2_pstar_bgsub           -> Draw("same");
    gPad -> SetLogy();
    auto leg2 = new TLegend(0.6,0.8,0.9,0.9);
    leg2->AddEntry(h1_pstar_bgsub,Form("combo MC: %i", comboEnts));
    leg2->AddEntry(h2_pstar_bgsub,Form("scaled data: %i", dataEnts));
    leg2->Draw();

    c2 -> cd(2);
    h1_pi_pstar_bgsub        -> SetLineColor(kBlue);
    h1_pi_pstar_bgsub        -> Draw();
    h1_pi_pstar_bgsub        -> SetTitle("p*(pi)");
    gPad -> SetLogy();
    h2_pi_pstar_bgsub        -> Scale(toCombo);
    h2_pi_pstar_bgsub        -> SetLineColor(kRed);
    h2_pi_pstar_bgsub        -> Draw("same");
    gPad -> SetLogy();

    c2 -> cd(3);
    h1_pi_phistar_bgsub      -> SetLineColor(kBlue);
    h1_pi_phistar_bgsub      -> SetMinimum(0.);
    h1_pi_phistar_bgsub      -> Draw();
    h1_pi_phistar_bgsub      -> SetTitle("#phi*(pi)");
    h2_pi_phistar_bgsub      -> Scale(toCombo);
    h2_pi_phistar_bgsub      -> SetLineColor(kRed);
    h2_pi_phistar_bgsub      -> Draw("same");

    c2 -> cd(4);
    h1_pi_thetastar_bgsub    -> SetLineColor(kBlue);
    h1_pi_thetastar_bgsub    -> SetMinimum(0.);
    h1_pi_thetastar_bgsub    -> Draw();
    h1_pi_thetastar_bgsub    -> SetTitle("cos#theta*(pi)");
    h2_pi_thetastar_bgsub    -> Scale(toCombo);
    h2_pi_thetastar_bgsub    -> SetLineColor(kRed);
    h2_pi_thetastar_bgsub    -> Draw("same");

    cout<<"total combo: "<<comboEnts<<", total data: "<<dataEnts<<endl;

    return ;
}
