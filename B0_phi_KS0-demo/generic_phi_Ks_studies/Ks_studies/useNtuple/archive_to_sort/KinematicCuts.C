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

void KinematicCuts() {
    bool bkgSub     = 1;
    bool sigLcut    = 0;
    bool thetaCut   = 1;
    bool pCut       = 1;
    const float minTheta = 0.3;
    const float maxTheta = 2.6;
    const float cosThetaCut = 0.92;
    const float minPt    = 0.06;
    const float pi  = TMath::Pi();
    int  numGen = 0;
    int  numSband = 0;
    int  num3Sig = 0;
    int  numMat = 0;
    const float maxM = 2;//2. for matchEff or 3. for other uses

    int  fitted = 72579;//mixed only 
    double  frac = 1.0;
    //int  fitted = 39210;//combo 
    //double  frac = 1.5055;
    //int  fitted = 195064;//signalMC 
    //double  frac = 1.24986;
//////////////////////////// RECO ///////////////////////////////////////////////
//fiting & bkgSub
    TH1F *h_p_sband         = new TH1F("h_p_sband","",              60,0.,maxM);
    TH1F *h_p_bgsub         = new TH1F("h_p_bgsub","",              60,0.,maxM);
    TH1F *h_pi_p_sband      = new TH1F("h_pi_p_sband","",           92,0.,maxM);
    TH1F *h_pi_p_bgsub      = new TH1F("h_pi_p_bgsub","",           92,0.,maxM);
    TH1F *h_pi_phi_sband    = new TH1F("h_pi_phi_sband","",         40,-pi,pi);
    TH1F *h_pi_phi_bgsub    = new TH1F("h_pi_phi_bgsub","",         40,-pi,pi);
    TH1F *h_pi_theta_sband  = new TH1F("h_pi_theta_sband","",       40,0.,pi);
    TH1F *h_pi_theta_bgsub  = new TH1F("h_pi_theta_bgsub","",       40,0.,pi);

    TH1F *h_pstar_sband         = new TH1F("h_pstar_sband","",          60,0.,maxM);
    TH1F *h_pstar_bgsub         = new TH1F("h_pstar_bgsub","",          60,0.,maxM);
    TH1F *h_pi_pstar_sband      = new TH1F("h_pi_pstar_sband","",       92,0.,maxM);
    TH1F *h_pi_pstar_bgsub      = new TH1F("h_pi_pstar_bgsub","",       92,0.,maxM);
    TH1F *h_pi_phistar_sband    = new TH1F("h_pi_phistar_sband","",     40,-pi,pi);
    TH1F *h_pi_phistar_bgsub    = new TH1F("h_pi_phistar_bgsub","",     40,-pi,pi);
    TH1F *h_pi_thetastar_sband  = new TH1F("h_pi_thetastar_sband","",   40,-1,1);
    TH1F *h_pi_thetastar_bgsub  = new TH1F("h_pi_thetastar_bgsub","",   40,-1,1);
//////////////////////// MC true ////////////////////////////////////////////////
    TH1F *h_p_gen           = new TH1F("h_p_gen","p(KS0)",              60,0.,maxM);
    TH1F *h_pi_p_gen        = new TH1F("h_pi_p_gen","p(pi)",            92,0,maxM);
    TH1F *h_pi_phi_gen      = new TH1F("h_pi_phi_gen","#phi(pi)",       40,-pi,pi);
    TH1F *h_pi_theta_gen    = new TH1F("h_pi_theta_gen","#theta(pi)",   40,0,pi);

    TH1F *h_pstar_gen       = new TH1F("h_pstar_gen","p*(K_S0)",        60,0.,maxM);
    TH1F *h_pi_pstar_gen    = new TH1F("h_pi_pstar_gen","p*(pi)",        92,0,maxM);
    TH1F *h_pi_phistar_gen      = new TH1F("h_pi_phistar_gen","#phi*(pi)",       40,-pi,pi);
    TH1F *h_pi_thetastar_gen    = new TH1F("h_pi_thetastar_gen","cos(#theta*(pi))",   40,-1,1);
//////////////////////////////////////////////////////////
    TChain *KS0s = new TChain("tree");                  //
    TChain *KSgen = new TChain("tree");                 //
                                                        //
    KS0s -> AddFile("MC13a_KS_compareDistance.root");   //
    KSgen -> AddFile("MC13a_KSpipi_MCTrue.root");       //
    //KS0s -> AddFile("MC13a_reco_200k.root");            //
    //KSgen -> AddFile("MC13a_MC_200k.root");             //
    //KS0s -> AddFile("MCBphiK_reco.root");               //
    //KSgen -> AddFile("MCBphiK_MC.root");                //
                                                        //
    int KEntries;                                       //
    KEntries = KS0s -> GetEntries();                    //
    int KgenEntries;                                    //
    KgenEntries = KSgen -> GetEntries();                //
//////////////////////////////////////////////////////////


//////////////////////////// RECO ///////////////////////////////////////////////
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
    double isMCSig;
    KS0s -> SetBranchAddress("isSignal", &isMCSig);
    double decayLength;
    KS0s -> SetBranchAddress("flightDistance", &decayLength);
    double decayLengthErr;
    KS0s -> SetBranchAddress("flightDistanceErr", &decayLengthErr);
    double sig_2D = -9999;
    double sig_3D;
    KS0s -> SetBranchAddress("significanceOfDistance", &sig_3D);
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
     
//////////////////////// MC true ////////////////////////////////////////////////
    double  p_KS0_gen;
    KSgen -> SetBranchAddress("p", &p_KS0_gen);
    double  dau0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmPDG__bc", &dau0);
    double  dau1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmPDG__bc", &dau1);
    double  dau2;
    KSgen -> SetBranchAddress("mcDaughter__bo2__cmPDG__bc", &dau2);
    double  gen_p0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmp__bc", &gen_p0);
    double  gen_p1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmp__bc", &gen_p1);
    double  gen_theta0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmtheta__bc", &gen_theta0);
    double  gen_theta1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmtheta__bc", &gen_theta1);
    double  gen_phi0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmphi__bc", &gen_phi0);
    double  gen_phi1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmphi__bc", &gen_phi1);
    
    double  pstar_KS0_gen;
    KSgen -> SetBranchAddress("useCMSFrame__bop__bc", &pstar_KS0_gen);
    double  gen_pstar0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bop__bc__bc", &gen_pstar0);
    double  gen_pstar1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bop__bc__bc", &gen_pstar1);
    double  gen_phistar0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__bophi__bc__bc", &gen_phistar0);
    double  gen_phistar1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__bophi__bc__bc", &gen_phistar1);
    double  gen_thetastar0;
    KSgen -> SetBranchAddress("mcDaughter__bo0__cmuseCMSFrame__botheta__bc__bc", &gen_thetastar0);
    double  gen_thetastar1;
    KSgen -> SetBranchAddress("mcDaughter__bo1__cmuseCMSFrame__botheta__bc__bc", &gen_thetastar1);
//////////////////////////// RECO ///////////////////////////////////////////////
    for(int i=0; i<KEntries; i++)
    {
        KS0s -> GetEntry(i);
        //to match the range in the fitting
        if(mass<=0.46||mass>=0.54)continue;
        if(sigLcut){
            sig_2D = decayLength/decayLengthErr;
            if(sig_2D<3)continue;//yes, it's verified this continue works properly
        }//to apply cut
        if(pCut&&(rec_p0<=minPt||rec_p1<=minPt))continue;
        //if(thetaCut&&(rec_theta0<=minTheta||rec_theta1<=minTheta
        //            ||rec_theta0>=maxTheta||rec_theta1>=maxTheta))continue;
        if(thetaCut&&(abs(cos(thetastar0))>cosThetaCut||abs(cos(thetastar1))>cosThetaCut))continue;
        if(bkgSub){
            if((mass<0.4748&&mass>0.47)||(mass<0.52&&mass>0.5152))
            {//side bands
                h_p_sband        -> Fill(p);
                h_pi_p_sband     -> Fill(rec_p0);
                h_pi_p_sband     -> Fill(rec_p1);
                h_pi_phi_sband   -> Fill(rec_phi0);
                h_pi_phi_sband   -> Fill(rec_phi1);
                h_pi_theta_sband -> Fill(rec_theta0);
                h_pi_theta_sband -> Fill(rec_theta1);

                h_pstar_sband        -> Fill(pstar_KS0);
                h_pi_pstar_sband     -> Fill(pstar0);
                h_pi_pstar_sband     -> Fill(pstar1);
                h_pi_phistar_sband   -> Fill(phistar0);
                h_pi_phistar_sband   -> Fill(phistar1);
                h_pi_thetastar_sband -> Fill(cos(thetastar0));
                h_pi_thetastar_sband -> Fill(cos(thetastar1));
                
                numSband++;
            }
            else if(mass>0.4929&&mass<0.5025)
            {//3 sigmas
                h_p_bgsub        -> Fill(p);
                h_pi_p_bgsub     -> Fill(rec_p0);
                h_pi_p_bgsub     -> Fill(rec_p1);
                h_pi_phi_bgsub   -> Fill(rec_phi0);
                h_pi_phi_bgsub   -> Fill(rec_phi1);
                h_pi_theta_bgsub -> Fill(rec_theta0);
                h_pi_theta_bgsub -> Fill(rec_theta1);

                h_pstar_bgsub        -> Fill(pstar_KS0);
                h_pi_pstar_bgsub     -> Fill(pstar0);
                h_pi_pstar_bgsub     -> Fill(pstar1);
                h_pi_phistar_bgsub   -> Fill(phistar0);
                h_pi_phistar_bgsub   -> Fill(phistar1);
                h_pi_thetastar_bgsub -> Fill(cos(thetastar0));
                h_pi_thetastar_bgsub -> Fill(cos(thetastar1));
                
                num3Sig++;
            }//signal region
        }//for bkgSub
    }//reco

    //////////////////////// MC true ////////////////////////////////////////////////
    for(int i=0; i<KgenEntries; i++){
        KSgen -> GetEntry(i);
        if(dau2!=-999)continue;//has more than two daughters
        if(pCut&&(gen_p0<=minPt||gen_p1<=minPt))continue;
        //if(thetaCut&&(gen_theta0<=minTheta||gen_theta1<=minTheta
        //            ||gen_theta0>=maxTheta||gen_theta1>=maxTheta))continue;
        if(thetaCut&&(abs(cos(gen_thetastar0))>cosThetaCut||abs(cos(gen_thetastar1))>cosThetaCut))continue;
        if((dau0==211&&dau1==-211)||(dau0==-211&&dau1==211)){
            h_p_gen         ->Fill(p_KS0_gen);
            h_pi_p_gen      ->Fill(gen_p0);
            h_pi_p_gen      ->Fill(gen_p1);
            h_pi_phi_gen    ->Fill(gen_phi0);
            h_pi_phi_gen    ->Fill(gen_phi1);
            h_pi_theta_gen  ->Fill(gen_theta0);
            h_pi_theta_gen  ->Fill(gen_theta1);
            
            h_pstar_gen         ->Fill(pstar_KS0_gen);
            h_pi_pstar_gen      ->Fill(gen_pstar0);
            h_pi_pstar_gen      ->Fill(gen_pstar1);
            h_pi_phistar_gen    ->Fill(gen_phistar0);
            h_pi_phistar_gen    ->Fill(gen_phistar1);
            h_pi_thetastar_gen  ->Fill(cos(gen_thetastar0));
            h_pi_thetastar_gen  ->Fill(cos(gen_thetastar1));

            numGen++;
        }
    }//gen

/////////////////////// PLOTS //////////////////////////////////////////////////
    if(bkgSub){
        int numRec = floor(num3Sig-frac*numSband);
        h_p_bgsub        -> Add(h_p_sband,-frac); 
        h_pi_p_bgsub     -> Add(h_pi_p_sband,-frac); 
        h_pi_phi_bgsub   -> Add(h_pi_phi_sband,-frac); 
        h_pi_theta_bgsub -> Add(h_pi_theta_sband,-frac); 

        h_pstar_bgsub        -> Add(h_pstar_sband,-frac); 
        h_pi_pstar_bgsub     -> Add(h_pi_pstar_sband,-frac); 
        h_pi_phistar_bgsub   -> Add(h_pi_phistar_sband,-frac); 
        h_pi_thetastar_bgsub -> Add(h_pi_thetastar_sband,-frac); 

        const auto toReco = fitted/(numRec*1.0);
        TH1F *h_p_scaled        = (TH1F*)h_p_bgsub->Clone();
        h_p_scaled              -> SetName("h_p_scaled");
        h_p_scaled              -> Scale(toReco);
        TH1F *h_pi_p_scaled     = (TH1F*)h_pi_p_bgsub->Clone();
        h_pi_p_scaled           -> SetName("h_pi_p_scaled");
        h_pi_p_scaled           -> Scale(toReco);
        TH1F *h_pi_phi_scaled   = (TH1F*)h_pi_phi_bgsub->Clone();
        h_pi_phi_scaled         -> SetName("h_pi_phi_scaled");
        h_pi_phi_scaled         -> Scale(toReco);
        TH1F *h_pi_theta_scaled = (TH1F*)h_pi_theta_bgsub->Clone();
        h_pi_theta_scaled       -> SetName("h_pi_theta_scaled");
        h_pi_theta_scaled       -> Scale(toReco);

        TH1F *h_pstar_scaled        = (TH1F*)h_pstar_bgsub->Clone();
        h_pstar_scaled              -> SetName("h_pstar_scaled");
        h_pstar_scaled              -> Scale(toReco);
        TH1F *h_pi_pstar_scaled     = (TH1F*)h_pi_pstar_bgsub->Clone();
        h_pi_pstar_scaled           -> SetName("h_pi_pstar_scaled");
        h_pi_pstar_scaled           -> Scale(toReco);
        TH1F *h_pi_phistar_scaled   = (TH1F*)h_pi_phistar_bgsub->Clone();
        h_pi_phistar_scaled         -> SetName("h_pi_phistar_scaled");
        h_pi_phistar_scaled         -> Scale(toReco);
        TH1F *h_pi_thetastar_scaled = (TH1F*)h_pi_thetastar_bgsub->Clone();
        h_pi_thetastar_scaled       -> SetName("h_pi_thetastar_scaled");
        h_pi_thetastar_scaled       -> Scale(toReco);

        //Lab frame plots
        gStyle->SetOptStat(0);
        TCanvas *c1 = new TCanvas("c1","c1",1200,800);
        c1 -> Divide(2,2,0.01,0.01,0);
        c1 -> cd(1);
        h_p_gen             -> SetLineColor(kRed);
        h_p_gen             -> Draw();
        gPad -> SetLogy();
        h_p_bgsub           -> SetLineColor(kBlue);
        h_p_bgsub           -> Draw("same");
        gPad -> SetLogy();
        h_p_scaled          -> SetLineColor(kGreen+2);
        h_p_scaled          -> Draw("same");
        gPad -> SetLogy();
        auto leg1 = new TLegend(0.7,0.8,0.9,0.9);
        leg1->AddEntry(h_p_gen,Form("gen: %i",numGen));
        leg1->AddEntry(h_p_scaled,Form("scaled rec: %i", fitted));
        leg1->AddEntry(h_p_bgsub,Form("rec in 3sig: %i",numRec));
        leg1->Draw();
        c1 -> cd(2);
        h_pi_p_gen          -> SetLineColor(kRed);
        h_pi_p_gen          -> Draw();
        gPad -> SetLogy();
        h_pi_p_bgsub        -> SetLineColor(kBlue);
        h_pi_p_bgsub        -> Draw("same");
        gPad -> SetLogy();
        h_pi_p_scaled       -> SetLineColor(kGreen+2);
        h_pi_p_scaled       -> Draw("same");
        gPad -> SetLogy();
        c1 -> cd(3);
        h_pi_phi_gen        -> SetLineColor(kRed);
        h_pi_phi_gen        -> SetMinimum(0.);
        h_pi_phi_gen        -> Draw();
        h_pi_phi_bgsub      -> SetLineColor(kBlue);
        h_pi_phi_bgsub      -> Draw("same");
        h_pi_phi_scaled     -> SetLineColor(kGreen+2);
        h_pi_phi_scaled     -> Draw("same");
        c1 -> cd(4);
        h_pi_theta_gen      -> SetLineColor(kRed);
        h_pi_theta_gen      -> SetMinimum(0.);
        h_pi_theta_gen      -> Draw();
        h_pi_theta_bgsub    -> SetLineColor(kBlue);
        h_pi_theta_bgsub    -> Draw("same");
        h_pi_theta_scaled   -> SetLineColor(kGreen+2);
        h_pi_theta_scaled   -> Draw("same");

        //CoM frame plots
        gStyle->SetOptStat(0);
        TCanvas *c2 = new TCanvas("c2","c2",1200,800);
        c2 -> Divide(2,2,0.01,0.01,0);
        c2 -> cd(1);
        h_pstar_gen             -> SetLineColor(kRed);
        h_pstar_gen             -> Draw();
        gPad -> SetLogy();
        h_pstar_bgsub           -> SetLineColor(kBlue);
        h_pstar_bgsub           -> Draw("same");
        gPad -> SetLogy();
        h_pstar_scaled          -> SetLineColor(kGreen+2);
        h_pstar_scaled          -> Draw("same");
        gPad -> SetLogy();
        auto leg2 = new TLegend(0.7,0.8,0.9,0.9);
        leg2->AddEntry(h_pstar_gen,Form("gen: %i",numGen));
        leg2->AddEntry(h_pstar_scaled,Form("scaled rec: %i", fitted));
        leg2->AddEntry(h_pstar_bgsub,Form("rec in 3sig: %i",numRec));
        leg2->Draw();
        c2 -> cd(2);
        h_pi_pstar_gen          -> SetLineColor(kRed);
        h_pi_pstar_gen          -> Draw();
        gPad -> SetLogy();
        h_pi_pstar_bgsub        -> SetLineColor(kBlue);
        h_pi_pstar_bgsub        -> Draw("same");
        gPad -> SetLogy();
        h_pi_pstar_scaled       -> SetLineColor(kGreen+2);
        h_pi_pstar_scaled       -> Draw("same");
        gPad -> SetLogy();

        c2 -> cd(3);
        h_pi_phistar_gen        -> SetLineColor(kRed);
        h_pi_phistar_gen        -> SetMinimum(0.);
        h_pi_phistar_gen        -> Draw();
        h_pi_phistar_bgsub      -> SetLineColor(kBlue);
        h_pi_phistar_bgsub      -> Draw("same");
        h_pi_phistar_scaled     -> SetLineColor(kGreen+2);
        h_pi_phistar_scaled     -> Draw("same");
        c2 -> cd(4);
        h_pi_thetastar_gen      -> SetLineColor(kRed);
        h_pi_thetastar_gen      -> SetMinimum(0.);
        h_pi_thetastar_gen      -> Draw();
        h_pi_thetastar_bgsub    -> SetLineColor(kBlue);
        h_pi_thetastar_bgsub    -> Draw("same");
        h_pi_thetastar_scaled   -> SetLineColor(kGreen+2);
        h_pi_thetastar_scaled   -> Draw("same");

        TH1F *h_pi_p_sqrt = (TH1F*)h_pi_p_scaled ->Clone();
        h_pi_p_sqrt -> SetName("h_p_sqrt");
        h_pi_p_sqrt -> Divide(h_pi_p_gen);
        int nCells = h_pi_p_sqrt -> GetNcells();
        cout<<nCells<<" cells"<<endl;
        for(int bin=0; bin<=nCells; bin++)
        {
            auto newContent = h_pi_p_sqrt->GetBinContent(bin);
            if(newContent<0)newContent=0;
            h_pi_p_sqrt -> SetBinContent(bin,sqrt(newContent));
        }
        TCanvas *c5 = new TCanvas("c5","c5",500,300);
        h_pi_p_sqrt -> SetLineColor(kGreen+2);
        h_pi_p_sqrt -> GetXaxis() -> SetTitle("p(#pi#pm) GeV");
        h_pi_p_sqrt -> GetYaxis() -> SetTitle("#sqrt{reco/gen ratio}");
        h_pi_p_sqrt -> SetTitle("Estimated tracking efficiency");
        h_pi_p_sqrt -> Draw();
        c5->SaveAs("EstTrkEff.png");

        TCanvas *c6 = new TCanvas("c6","c6",600,400);
        auto rp3 = new TRatioPlot(h_pi_p_scaled,h_pi_p_gen);
        h_p_scaled -> GetXaxis() -> SetTitle("p(K_S0)");
        gPad -> SetLogy();
        rp3 -> Draw();
        rp3 -> GetLowYaxis() -> SetNdivisions(6);
        
    }//bkgSub

    return ;
}
