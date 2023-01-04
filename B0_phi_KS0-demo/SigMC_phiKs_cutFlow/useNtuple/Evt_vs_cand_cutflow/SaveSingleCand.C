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

#include "MyB0Cand.h"
#include "MyPhiCand.h"
#include "MyKsCand.h"
#include "MyDecChain.h"
//Get multiplicity and save candidates at different stages
void SaveSingleCand() {
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_phiKs_cutFlow/getNtuple/SigMC_B0_Rec_20k.root");
    //TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_phiKs_cutFlow/getNtuple/SigMC_B0_noUpdate.root");
    TString fOutName("SigMC_cand.root");
    TFile fOut(fOutName,"recreate");

    TChain      *B0_Rec  = new TChain("B0_Rec");
    B0_Rec  -> AddFile(fileName);
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs0  = new MyKsRec();
    recBKs0 -> setBranchAddresses(B0_Rec, "Ks_");

    TChain      *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile(fileName);
    MyPhiRec    *recPhi  = new MyPhiRec();
    recPhi  -> setBranchAddresses(phi_Rec);
    
    TChain      *Ks_Rec  = new TChain("Ks_Rec");
    Ks_Rec  -> AddFile(fileName);
    MyKsRec     *recKs   = new MyKsRec;
    recKs   -> setBranchAddresses(Ks_Rec);
    
    TTree       *B0_phi = new TTree("B0_phi","");
    MyB0Cand    *cand_phi  = new MyB0Cand();
    cand_phi-> branches(B0_phi);

    TTree       *B0_Ks0 = new TTree("B0_Ks0","");
    MyB0Cand    *cand_Ks0  = new MyB0Cand();
    cand_Ks0-> branches(B0_Ks0);

    TTree       *B0_deE = new TTree("B0_deE","");
    MyB0Cand    *cand_deE  = new MyB0Cand();
    cand_deE-> branches(B0_deE);

    TTree       *B0_Mbc = new TTree("B0_Mbc","");
    MyB0Cand    *cand_Mbc  = new MyB0Cand();
    cand_Mbc-> branches(B0_Mbc);

    TTree       *B0_csv = new TTree("B0_csv","");
    MyB0Cand    *cand_csv  = new MyB0Cand();
    cand_csv-> branches(B0_csv);
    
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

    TChain   *Y4S_chain  = new TChain("Y4S_chain");
    Y4S_chain->AddFile(fileName);
    MyDecChain  *dec     = new MyDecChain();
    dec     -> setBranchAddresses(Y4S_chain);

    int nEvt_phi = 0;
    int nEvt_Ks0 = 0;
    int nEvt_deE = 0;
    int nEvt_Mbc = 0;
    int nEvt_csv = 0;

    TH1F *h1 = new TH1F("h1","has good phi",10,0,10);
    TH1F *h2 = new TH1F("h2","has good Ks",10,0,10);
    TH1F *h3 = new TH1F("h3","pass deE cut",10,0,10);
    TH1F *h4 = new TH1F("h4","pass Mbc cut",10,0,10);

    for(int i=0; i < Entries; i++)
    {
        phi_Rec     -> GetEntry(i);
        Ks_Rec      -> GetEntry(i);
        B0_Rec      -> GetEntry(i);
        B0_MC       -> GetEntry(i);
        Y4S_chain   -> GetEntry(i);

        double hasSigChain = 0.;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = 1.;
            }
        }//B0_MC ?hasSigChain
        if(!hasSigChain)continue;

        bool has_B_phi = false;
        double phi_ch2 = 0;
        double phi_ind = 0;

        bool has_B_Ks0 = false;
        double Ks0_ch2 = 0;
        double Ks0_ind = 0;

        bool has_B_deE = false;
        double deE_ch2 = 0;
        double deE_ind = 0;
        
        bool has_B_Mbc = false;
        double Mbc_ch2 = 0;
        double Mbc_ind = 0; 
        
        bool has_B_csv = false;
        double csv_ch2 = 0;
        double csv_ind = 0;

        int nCan_phi = 0;
        int nCan_Ks0 = 0;
        int nCan_deE = 0;
        int nCan_Mbc = 0;
   
        /* 
        for(int iEnt = 0; iEnt<recPhi->nCand; iEnt++){
            bool phi_vtx = recPhi->chiProb[iEnt]>0;
            bool phi_cdc = recPhi->dau_0_thetaInCDCAcceptance[iEnt]==1. 
                        && recPhi->dau_1_thetaInCDCAcceptance[iEnt]==1.;
            bool phi_hit = recPhi->dau_0_nCDCHits[iEnt]>0
                        && recPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recPhi->dau_0_kaonID[iEnt]>0.05
                        && recPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_mKK = recPhi->M[iEnt]>1.004
                        && recPhi->M[iEnt]<1.034;
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid && phi_mKK;
        
            if(phi_ana)has_B_phi=true;
            nCan_phi++;
        }//recPhi
        if(!has_B_phi)continue;
        h1 -> Fill(nCan_phi);

        for(int iEnt = 0; iEnt<recKs->nCand; iEnt++){
            bool Ks0_vtx = recKs->chiProb[iEnt]>0;
            bool Ks0_gbk = recKs->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recKs->M[iEnt]>.483
                        && recKs->M[iEnt]<.513;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk && Ks0_mKs;

            if(Ks0_ana)has_B_Ks0=true;
            nCan_Ks0++;
        }//recKs
        if(!has_B_Ks0)continue;
        h2 -> Fill(nCan_Ks0);
        
        //Reset to B0 cand daughter cuts for cand saving
        has_B_phi = false;
        has_B_Ks0 = false;
        */

        for(int iEnt = 0; iEnt<recB->nCand; iEnt++){
            double B_ch2 = recB->chiProb[iEnt];

            bool phi_vtx = recBPhi->chiProb[iEnt]>0;
            bool phi_cdc = recBPhi->dau_0_thetaInCDCAcceptance[iEnt]==1. 
                        && recBPhi->dau_1_thetaInCDCAcceptance[iEnt]==1.;
            bool phi_hit = recBPhi->dau_0_nCDCHits[iEnt]>0
                        && recBPhi->dau_1_nCDCHits[iEnt]>0;
            bool phi_kid = recBPhi->dau_0_kaonID[iEnt]>0.05
                        && recBPhi->dau_1_kaonID[iEnt]>0.05;
            bool phi_mKK = recBPhi->M[iEnt]>1.004
                        && recBPhi->M[iEnt]<1.034;
            bool phi_ana = phi_vtx && phi_cdc && phi_hit && phi_kid && phi_mKK;

            bool Ks0_vtx = recBKs0->chiProb[iEnt]>0;
            bool Ks0_gbk = recBKs0->goodBelleKshort[iEnt]==1;
            bool Ks0_mKs = recBKs0->M[iEnt]>.483
                        && recBKs0->M[iEnt]<.513;
            bool Ks0_ana = Ks0_vtx && Ks0_gbk && Ks0_mKs;

            double B_Mbc = recB->Mbc[iEnt];
            double B_deE = recB->deltaE[iEnt];

            //cout<<"Mbc="<<B_Mbc<<" deltaE="<<B_deE<<" phi_ana="<<phi_ana<<" Ks0_ana="<<Ks0_ana<<endl;
        
            if(!phi_ana)continue;
            nCan_phi++;
            if(B_ch2>phi_ch2){
                has_B_phi = true;
                phi_ind = iEnt;
            }
            //////// has_a_phi 
        
            if(!Ks0_ana)continue;
            nCan_Ks0++;
            if(B_ch2>Ks0_ch2){
                has_B_Ks0 = true;
                Ks0_ind = iEnt;
            }
            //////// has_a_Ks0
            
            if(isnan(B_deE))continue;
            if(B_deE<-.02||B_deE>.02)continue;
            nCan_deE++;
            if(B_ch2>deE_ch2){
                has_B_deE = true;
                deE_ind = iEnt;
            }
            //////// has_a_deE 
            
            if(isnan(B_Mbc))continue;
            if(B_Mbc<5.2 ||B_Mbc>5.3)continue;
            nCan_Mbc++;
            if(B_ch2>Mbc_ch2){
                has_B_Mbc = true;
                Mbc_ind = iEnt;
            }
            //////// has_a_Mbc 

            if(isnan(recB->foxWolframR1[iEnt]))continue;
            if(isnan(recB->foxWolframR2[iEnt]))continue;
            if(isnan(recB->foxWolframR3[iEnt]))continue;
            if(isnan(recB->foxWolframR4[iEnt]))continue;

            if(isnan(recB->harmonicMomentThrust0[iEnt]))continue;
            if(isnan(recB->harmonicMomentThrust1[iEnt]))continue;
            if(isnan(recB->harmonicMomentThrust2[iEnt]))continue;
            if(isnan(recB->harmonicMomentThrust3[iEnt]))continue;
            if(isnan(recB->harmonicMomentThrust4[iEnt]))continue;
            
            if(isnan(recB->cleoConeThrust0[iEnt]))continue;
            if(isnan(recB->cleoConeThrust1[iEnt]))continue;
            if(isnan(recB->cleoConeThrust2[iEnt]))continue;
            if(isnan(recB->cleoConeThrust3[iEnt]))continue;
            if(isnan(recB->cleoConeThrust4[iEnt]))continue;
            if(isnan(recB->cleoConeThrust5[iEnt]))continue;
            if(isnan(recB->cleoConeThrust6[iEnt]))continue;
            if(isnan(recB->cleoConeThrust7[iEnt]))continue;
            if(isnan(recB->cleoConeThrust8[iEnt]))continue;

            if(isnan(recB->sphericity[iEnt]))continue;
            if(isnan(recB->aplanarity[iEnt]))continue;
            if(isnan(recB->thrust[iEnt]))continue;
            if(isnan(recB->thrustAxisCosTheta[iEnt]))continue;

            if(isnan(recB->CS_hso00[iEnt]))continue;
            if(isnan(recB->CS_hso02[iEnt]))continue;
            if(isnan(recB->CS_hso04[iEnt]))continue;
            if(isnan(recB->CS_hso10[iEnt]))continue;
            if(isnan(recB->CS_hso12[iEnt]))continue;
            if(isnan(recB->CS_hso14[iEnt]))continue;
            if(isnan(recB->CS_hso20[iEnt]))continue;
            if(isnan(recB->CS_hso22[iEnt]))continue;
            if(isnan(recB->CS_hso24[iEnt]))continue;
            if(isnan(recB->CS_hoo1 [iEnt]))continue;
            if(isnan(recB->CS_hoo2 [iEnt]))continue;
            if(isnan(recB->CS_hoo3 [iEnt]))continue;
            if(isnan(recB->CS_hoo4 [iEnt]))continue;
            if(isnan(recB->CS_et   [iEnt]))continue;
            if(isnan(recB->CS_mm2  [iEnt]))continue;

            if(B_ch2>csv_ch2){
                has_B_csv = true;
                csv_ind = iEnt;
            }
            //////// has_a_csv 

        }//B0_Rec candidates

        if(!has_B_phi)continue;
        nEvt_phi++;
        h1 -> Fill(nCan_phi);   
        for(int iEnt = phi_ind; iEnt<phi_ind+1; iEnt++){
            cand_phi->isSignal                 = recB->isSignal[iEnt];
            cand_phi->M                        = recB->M[iEnt];
            cand_phi->p                        = recB->p[iEnt];
            cand_phi->pstar                    = recB->pstar[iEnt];
            cand_phi->Mbc                      = recB->Mbc[iEnt];
            cand_phi->deltaE                   = recB->deltaE[iEnt];

            B0_phi -> Fill();
        }

        if(!has_B_Ks0)continue;
        nEvt_Ks0++;
        h2 -> Fill(nCan_Ks0);
        for(int iEnt = Ks0_ind; iEnt<Ks0_ind+1; iEnt++){
            cand_Ks0->isSignal                 = recB->isSignal[iEnt];
            cand_Ks0->M                        = recB->M[iEnt];
            cand_Ks0->p                        = recB->p[iEnt];
            cand_Ks0->pstar                    = recB->pstar[iEnt];
            cand_Ks0->Mbc                      = recB->Mbc[iEnt];
            cand_Ks0->deltaE                   = recB->deltaE[iEnt];

            B0_Ks0 -> Fill();
        }

        if(!has_B_deE)continue;
        nEvt_deE++;
        h3 -> Fill(nCan_deE);
        for(int iEnt = deE_ind; iEnt<deE_ind+1; iEnt++){
            cand_deE->isSignal                 = recB->isSignal[iEnt];
            cand_deE->M                        = recB->M[iEnt];
            cand_deE->p                        = recB->p[iEnt];
            cand_deE->pstar                    = recB->pstar[iEnt];
            cand_deE->Mbc                      = recB->Mbc[iEnt];
            cand_deE->deltaE                   = recB->deltaE[iEnt];

            B0_deE -> Fill();
        }

        if(!has_B_Mbc)continue;
        nEvt_Mbc++;
        h4 -> Fill(nCan_Mbc);
        for(int iEnt = Mbc_ind; iEnt<Mbc_ind+1; iEnt++){
            cand_Mbc->isSignal                 = recB->isSignal[iEnt];
            cand_Mbc->M                        = recB->M[iEnt];
            cand_Mbc->p                        = recB->p[iEnt];
            cand_Mbc->pstar                    = recB->pstar[iEnt];
            cand_Mbc->Mbc                      = recB->Mbc[iEnt];
            cand_Mbc->deltaE                   = recB->deltaE[iEnt];

            B0_Mbc -> Fill();
        }

        if(!has_B_csv)continue;
        nEvt_csv++;
        for(int iEnt = csv_ind; iEnt<csv_ind+1; iEnt++){
            cand_csv->isSignal                 = recB->isSignal[iEnt];
            cand_csv->M                        = recB->M[iEnt];
            cand_csv->p                        = recB->p[iEnt];
            cand_csv->pstar                    = recB->pstar[iEnt];
            cand_csv->Mbc                      = recB->Mbc[iEnt];
            cand_csv->deltaE                   = recB->deltaE[iEnt];

            B0_csv -> Fill();
        }

    }//events
    //////////////////////////////////////////////////
    
    cout<<"phi: "<<nEvt_phi<<endl;
    cout<<"Ks0: "<<nEvt_Ks0<<endl;
    cout<<"deE: "<<nEvt_deE<<endl;
    cout<<"Mbc: "<<nEvt_Mbc<<endl;
    cout<<"csv: "<<nEvt_csv<<endl;

/*
    gStyle -> SetOptStat("nermous");
    TCanvas *c1 = new TCanvas("c1","",500,400);
    c1 -> Divide(2,2,.01,.01);
    c1 -> cd(1);
    h1 -> Draw();
    c1 -> cd(2);
    h2 -> Draw();
    c1 -> cd(3);
    h3 -> Draw();
    c1 -> cd(4);
    h4 -> Draw();
    c1 -> SaveAs("SigMC_multiplcity.pdf");
*/

    fOut.cd();
    B0_phi -> Write();
    B0_Ks0 -> Write();
    B0_deE -> Write();
    B0_Mbc -> Write();
    B0_csv -> Write();
    fOut.Close();

    return;
}
