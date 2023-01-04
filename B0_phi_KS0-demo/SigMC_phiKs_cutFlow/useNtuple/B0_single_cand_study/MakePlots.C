#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0Rec.h"
#include "MyPhiRec.h"
#include "MyKsRec.h"
#include "MyB0MC.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
#include "MyCutFlow.h"

//Studies the differences of candidates from different cuts
void MakePlots(){
    
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");

    TChain      *B0_Rec  = new TChain("B0_Rec");
    B0_Rec  -> AddFile(fileName);
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs  = new MyKsRec();
    recBKs  -> setBranchAddresses(B0_Rec, "Ks_");
    
    int Entries = B0_Rec -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    TChain      *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC      *mc      = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi   = new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs    = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    const double dE_wid_ = 0.10;
    const double min_Mbc = 5.26;
    const double max_Mbc = 9999;
    const double dE_WID_ = 0.06;
    const double MIN_Mbc = 5.27;
    const double MAX_Mbc = 5.29;
    const double min_mKK = 1.004;
    const double max_mKK = 1.034;
    const double min_mKs = 0.483;
    const double max_mKs = 0.513;

    TH1F *h_mul_1 = new TH1F("h_mul_1","phi and Ks ana cuts",       5,0,5);
    TH1F *h_mul_2 = new TH1F("h_mul_2","anaCuts +3sigma cut",       5,0,5);
    TH1F *h_mul_3 = new TH1F("h_mul_3","ana+3sigma + 5.27<Mbc<5.29",5,0,5);
    TH1F *h_mul_4 = new TH1F("h_mul_4","ana+3sigma + abs(deE)<.06", 5,0,5);
    TH1F *h_mul_5 = new TH1F("h_mul_5","ana+3sigma + abs(deE)<.06 + 5.27<Mbc<5.29",5,0,5);
    //phi and Ks ana cuts
    TH1F *h_Mbc_1 = new TH1F("h_Mbc_1","phi and Ks ana cuts",60,5.26,5.29);
    TH1F *h_deE_1 = new TH1F("h_deE_1","phi and Ks ana cuts",40, -.2,  .2);
    //phi and Ks 3sigma cuts
    TH1F *h_Mbc_2 = new TH1F("h_Mbc_2","anaCuts +3sigma cut",60,5.26,5.29);
    TH1F *h_deE_2 = new TH1F("h_deE_2","anaCuts +3sigma cut",40, -.2,  .2);
    //Tighter deE cut
    TH1F *h_Mbc_3 = new TH1F("h_Mbc_3","ana+3sigma + 5.27<Mbc<5.29",60,5.26,5.29);
    TH1F *h_deE_3 = new TH1F("h_deE_3","ana+3sigma + 5.27<Mbc<5.29",40, -.2,  .2);
    //Tighter Mbc cut
    TH1F *h_Mbc_4 = new TH1F("h_Mbc_4","ana+3sigma + abs(deE)<.06",60,5.26,5.29);
    TH1F *h_deE_4 = new TH1F("h_deE_4","ana+3sigma + abs(deE)<.06",40, -.2,  .2);
    //Tighter everything cut
    TH1F *h_Mbc_5 = new TH1F("h_Mbc_5","ana+3sigma + abs(deE)<.06 + 5.27<Mbc<5.29",60,5.26,5.29);
    TH1F *h_deE_5 = new TH1F("h_deE_5","ana+3sigma + abs(deE)<.06 + 5.27<Mbc<5.29",40, -.2,  .2);

    //phi and Ks 3sigma cuts
    TH1F *h_Mbc_6 = new TH1F("h_Mbc_6","ana+only Ks 3sigma cut",60,5.26,5.29);
    TH1F *h_deE_6 = new TH1F("h_deE_6","ana+only Ks 3sigma cut",40, -.2,  .2);
    //phi and Ks 3sigma cuts
    TH1F *h_Mbc_7 = new TH1F("h_Mbc_7","ana+only phi 3sigma cut",60,5.26,5.29);
    TH1F *h_deE_7 = new TH1F("h_deE_7","ana+only phi 3sigma cut",40, -.2,  .2);
    //Only Ale's choice on phi mass window
    TH1F *h_Mbc_8 = new TH1F("h_Mbc_8","ana+only 1<m(#phi)<1.04 cut",60,5.26,5.29);
    TH1F *h_deE_8 = new TH1F("h_deE_8","ana+only 1<m(#phi)<1.04 cut",40, -.2,  .2);

    for(int i=0; i < Entries; i++){
        B0_Rec -> GetEntry(i);
        B0_MC  -> GetEntry(i);

        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;
    //////////////////////////////////////////////////
        bool has_B0_ana = false;
        bool has_B0_mss = false;
        bool has_B0_mMd = false;
        bool has_B0_mmD = false;
        bool has_B0_mMD = false;

        int  nFinCand_1 = 0;
        int  nFinCand_2 = 0;
        int  nFinCand_3 = 0;
        int  nFinCand_4 = 0;
        int  nFinCand_5 = 0;
    //////////////////////////////////////////////////
        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            bool phi_vtx = recBPhi->chiProb[iEnt]>0;
            bool phi_cdc = recBPhi->dau_0_thetaInCDCAcceptance[iEnt]==1 
                        && recBPhi->dau_1_thetaInCDCAcceptance[iEnt]==1;
            bool phi_hit = recBPhi->dau_0_nCDCHits[iEnt]>0
                        && recBPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recBPhi->dau_0_kaonID[iEnt]>0.05
                        && recBPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_mKK = recBPhi->M[iEnt]>min_mKK
                        && recBPhi->M[iEnt]<max_mKK;
            bool phi_Ale = recBPhi->M[iEnt]>1
                        && recBPhi->M[iEnt]<1.04;
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid;

            bool Ks0_vtx = recBKs->chiProb[iEnt]>0;
            bool Ks0_gbk = recBKs->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recBKs->M[iEnt]>min_mKs 
                        && recBKs->M[iEnt]<max_mKs;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk;

            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];
            bool B0__vtx = recB->chiProb[iEnt]>0;
            bool B0__mbc = B_Mbc > min_Mbc
                    && B_Mbc < max_Mbc;
            bool B0__deE = abs(B_deE)<dE_wid_;
            bool B0__Mbc = B_Mbc > MIN_Mbc
                    && B_Mbc < MAX_Mbc;
            bool B0__DeE = abs(B_deE)<dE_WID_;

            bool baseCut = phi_ana && Ks0_ana && B0__vtx && B0__mbc && B0__deE;
            bool addMass = baseCut && phi_mKK && Ks0_mKs;

            if(baseCut){
                has_B0_ana = true;
                nFinCand_1++;
                h_Mbc_1 -> Fill(B_Mbc);
                h_deE_1 -> Fill(B_deE);
            }
            if(addMass){
                has_B0_mss = true;
                nFinCand_2++;
                h_Mbc_2 -> Fill(B_Mbc);
                h_deE_2 -> Fill(B_deE);
            }
            if(addMass && B0__Mbc){
                has_B0_mMd = true;
                nFinCand_3++;
                h_Mbc_3 -> Fill(B_Mbc);
                h_deE_3 -> Fill(B_deE);
            }
            if(addMass && B0__DeE){
                has_B0_mmD = true;
                nFinCand_4++;
                h_Mbc_4 -> Fill(B_Mbc);
                h_deE_4 -> Fill(B_deE);
            }
            if(addMass && B0__Mbc && B0__DeE){
                has_B0_mMD = true;
                nFinCand_5++;
                h_Mbc_5 -> Fill(B_Mbc);
                h_deE_5 -> Fill(B_deE);
            }
            if(baseCut && Ks0_mKs){
                h_Mbc_6 -> Fill(B_Mbc);
                h_deE_6 -> Fill(B_deE);
            }
            if(baseCut && phi_mKK){
                h_Mbc_7 -> Fill(B_Mbc);
                h_deE_7 -> Fill(B_deE);
            }
            if(baseCut && phi_Ale){
                h_Mbc_8 -> Fill(B_Mbc);
                h_deE_8 -> Fill(B_deE);
            }
        }//B0_Rec candidates
    //////////////////////////////////////////////////
        if(has_B0_ana)
            h_mul_1 -> Fill(nFinCand_1);
        if(has_B0_mss)
            h_mul_2 -> Fill(nFinCand_2);
        if(has_B0_mMd)
            h_mul_3 -> Fill(nFinCand_3);
        if(has_B0_mmD)
            h_mul_4 -> Fill(nFinCand_4);
        if(has_B0_mMD)
            h_mul_5 -> Fill(nFinCand_5);
    }//nTotalEvents

    //////////////////////////////////////////////////

    gStyle -> SetOptStat("nermous");

    setStyle(h_mul_1,kBlue+0,"nCandidates/evt");
    setStyle(h_mul_2,kBlue+0,"nCandidates/evt");
    setStyle(h_mul_3,kBlue+0,"nCandidates/evt");
    setStyle(h_mul_4,kBlue+0,"nCandidates/evt");
    setStyle(h_mul_5,kBlue+0,"nCandidates/evt");
    TCanvas *c1 = new TCanvas("c1","c1",200,500);
    c1 -> Divide(1,5,.01,.01);
    c1 -> cd(1);
    h_mul_1 -> Draw();
    gPad -> SetLogy();
    c1 -> cd(2);
    h_mul_2 -> Draw();
    gPad -> SetLogy();
    c1 -> cd(3);
    h_mul_3 -> Draw();
    gPad -> SetLogy();
    c1 -> cd(4);
    h_mul_4 -> Draw();
    gPad -> SetLogy();
    c1 -> cd(5);
    h_mul_5 -> Draw();
    gPad -> SetLogy();
    c1 -> SaveAs("multiplicity.pdf");

    setStyle(h_Mbc_1,kBlue+0,"Mbc");
    setStyle(h_Mbc_2,kBlue+0,"Mbc");
    setStyle(h_Mbc_3,kBlue+0,"Mbc");
    setStyle(h_Mbc_4,kBlue+0,"Mbc");
    setStyle(h_Mbc_5,kBlue+0,"Mbc");

    setStyle(h_deE_1,kBlue+0,"deltaE");
    setStyle(h_deE_2,kBlue+0,"deltaE");
    setStyle(h_deE_3,kBlue+0,"deltaE");
    setStyle(h_deE_4,kBlue+0,"deltaE");
    setStyle(h_deE_5,kBlue+0,"deltaE");

    h_Mbc_5 -> Add(h_Mbc_2,-1);
    h_Mbc_4 -> Add(h_Mbc_2,-1);
    h_Mbc_3 -> Add(h_Mbc_2,-1);
    h_Mbc_2 -> Add(h_Mbc_1,-1);
    h_deE_5 -> Add(h_deE_2,-1);
    h_deE_4 -> Add(h_deE_2,-1);
    h_deE_3 -> Add(h_deE_2,-1);
    h_deE_2 -> Add(h_deE_1,-1);

    TCanvas *c2 = new TCanvas("c2","c2",500,1000);
    c2 -> Divide(2,5,.01,.01);
    c2 -> cd(1);
    h_Mbc_1 -> Draw();
    c2 -> cd(3);
    h_Mbc_2 -> Draw();
    c2 -> cd(5);
    h_Mbc_3 -> Draw();
    c2 -> cd(7);
    h_Mbc_4 -> Draw();
    c2 -> cd(9);
    h_Mbc_5 -> Draw();
    c2 -> cd(2);
    h_deE_1 -> Draw();
    c2 -> cd(4);
    h_deE_2 -> Draw();
    c2 -> cd(6);
    h_deE_3 -> Draw();
    c2 -> cd(8);
    h_deE_4 -> Draw();
    c2 -> cd(10);
    h_deE_5 -> Draw();
    c2 -> SaveAs("Mbc_deltaE_diff.pdf");

    setStyle(h_Mbc_6,kBlue+0,"Mbc");
    setStyle(h_Mbc_7,kBlue+0,"Mbc");
    setStyle(h_Mbc_8,kBlue+0,"Mbc");
    setStyle(h_deE_6,kBlue+0,"deltaE");
    setStyle(h_deE_7,kBlue+0,"deltaE");
    setStyle(h_deE_8,kBlue+0,"deltaE");

    h_Mbc_8 -> Add(h_Mbc_1,-1);
    h_Mbc_7 -> Add(h_Mbc_1,-1);
    h_Mbc_6 -> Add(h_Mbc_1,-1);
    h_deE_8 -> Add(h_deE_1,-1);
    h_deE_7 -> Add(h_deE_1,-1);
    h_deE_6 -> Add(h_deE_1,-1);

    TCanvas *c3 = new TCanvas("c3","c3",500,600);
    c3 -> Divide(2,3,.01,.01);
    c3 -> cd(1);
    h_Mbc_6 -> Draw();
    c3 -> cd(3);
    h_Mbc_7 -> Draw();
    c3 -> cd(5);
    h_Mbc_8 -> Draw();
    c3 -> cd(2);
    h_deE_6 -> Draw();
    c3 -> cd(4);
    h_deE_7 -> Draw();
    c3 -> cd(6);
    h_deE_8 -> Draw();
    c3 -> SaveAs("phiKs_3sigma_cuts.pdf");

    return;
}
