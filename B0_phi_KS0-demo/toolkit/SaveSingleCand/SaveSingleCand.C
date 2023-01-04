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
void SaveSingleCand(bool isMC, bool onlySigEvt, TString fB0_Rec, TString fMCDeT, TString fOutName, float deE_width) {

    if(isMC)cout<<"MC events"<<endl;
    else    cout<<"Real data"<<endl;
    if(onlySigEvt)cout<<"keeping only signal events"<<endl;
    cout<<endl;
    cout<<"Input: "<<fB0_Rec<<endl;
    cout<<"Outout: "<<fOutName<<endl;
    cout<<"deltaE: "<<deE_width<<endl;

    TFile fOut(fOutName,"recreate");

    TChain      *B0_Rec  = new TChain("B0_Rec");
    B0_Rec  -> AddFile(fB0_Rec);
    MyB0Rec     *recB    = new MyB0Rec();
    recB    -> setBranchAddresses(B0_Rec);
    MyPhiRec    *recBPhi = new MyPhiRec();
    recBPhi -> setBranchAddresses(B0_Rec, "phi_");
    MyKsRec     *recBKs0 = new MyKsRec();
    recBKs0 -> setBranchAddresses(B0_Rec, "Ks_");

    TChain      *mcTree  = new TChain("mcTree");
    float mcDeltaTau;
    mcTree  -> AddFile(fMCDeT);
    mcTree  -> SetBranchAddress("mcDeltaTau",&mcDeltaTau);
    
    TTree       *B0_cand = new TTree("B0_cand","");
    MyB0Cand     *candB  = new MyB0Cand();
    candB   -> branches(B0_cand);
    MyPhiCand    *cBPhi  = new MyPhiCand();
    cBPhi   -> branches(B0_cand, "phi_");
    MyKsCand     *cBKs0  = new MyKsCand();
    cBKs0   -> branches(B0_cand, "Ks_");
    
    int Entries = B0_Rec -> GetEntries();
    cout<<Entries<<"  events"<<endl;

    TChain      *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fB0_Rec);
    MyB0MC      *mc      = new MyB0MC();
    mc      -> setBranchAddresses(B0_MC);
    MyPhiKsMC   *mcPhi   = new MyPhiKsMC();
    mcPhi   -> setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC   *mcKs    = new MyPhiKsMC();
    mcKs    -> setBranchAddresses(B0_MC, "Ks_");

    TChain   *Y4S_chain  = new TChain("Y4S_chain");
    Y4S_chain->AddFile(fB0_Rec);
    MyDecChain  *dec     = new MyDecChain();
    dec     -> setBranchAddresses(Y4S_chain);

    for(int i=0; i < Entries; i++)
    {
        B0_Rec      -> GetEntry(i);
        B0_MC       -> GetEntry(i);
        Y4S_chain   -> GetEntry(i);
        mcTree      -> GetEntry(i);

        double hasSigChain = 0.;
        for(int iEnt = 0; iEnt< mc->nCand; iEnt++){
            if( mcPhi->nDaus[iEnt]>=2 &&  mcKs->nDaus[iEnt]>=2
                && abs(mcPhi->dau_0_PDG[iEnt])==321 && abs(mcPhi->dau_1_PDG[iEnt])==321
                && abs( mcKs->dau_0_PDG[iEnt])==211 && abs( mcKs->dau_1_PDG[iEnt])==211){
                hasSigChain = 1.;
            }
        }//B0_MC ?hasSigChain
        //if(!hasSigChain)continue;

        double evt_ch2 = 0;
        double evt_ind = 0;
        bool has_B_ana = false;

    //////////////////////////////////////////////////
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
            if(isnan(B_Mbc))continue;
            if(isnan(B_deE))continue;
            //cout<<"Mbc="<<B_Mbc<<" deltaE="<<B_deE<<" phi_ana="<<phi_ana<<" Ks0_ana="<<Ks0_ana<<endl;
            if(B_Mbc<5.2 ||B_Mbc>5.3)continue;
            if(fabs(B_deE)>deE_width)continue;

            if(!phi_ana||!Ks0_ana)continue;
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

            if(B_ch2>evt_ch2){
                has_B_ana = true;
                evt_ind = iEnt;
            }//save index of wanted candidate
        }//B0_Rec candidates

        if(!has_B_ana)continue;
        for(int iEnt = evt_ind; iEnt<evt_ind+1; iEnt++){
            candB->qrMC                     = recB->qrMC[iEnt];
            candB->MCflvrOtherB             = recB->MCflvrOtherB[iEnt];
            candB->flvrTag_FBDT             = recB->flvrTag_FBDT[iEnt];

            candB->Ecms                     = recB->Ecms[iEnt];
            candB->beamE                    = recB->beamE[iEnt];
            candB->beamPx                   = recB->beamPx[iEnt];
            candB->beamPy                   = recB->beamPy[iEnt];
            candB->beamPz                   = recB->beamPz[iEnt];

            candB->DeltaBoost               = recB->DeltaBoost[iEnt];
            candB->DeltaBoostErr            = recB->DeltaBoostErr[iEnt];
            candB->DeltaT                   = recB->DeltaT[iEnt];
            candB->DeltaTErr                = recB->DeltaTErr[iEnt];
            candB->DeltaTRes                = recB->DeltaTRes[iEnt];
            candB->DeltaZ                   = recB->DeltaZ[iEnt];
            candB->DeltaZErr                = recB->DeltaZErr[iEnt];

            candB->mcDeltaBoost             = recB->mcDeltaBoost[iEnt];
            candB->mcDeltaT                 = recB->mcDeltaT[iEnt];
            candB->mcDeltaTau               = mcDeltaTau;

            candB->isSignal                 = recB->isSignal[iEnt];
            candB->M                        = recB->M[iEnt];
            candB->p                        = recB->p[iEnt];
            candB->pstar                    = recB->pstar[iEnt];
            candB->Mbc                      = recB->Mbc[iEnt];
            candB->deltaE                   = recB->deltaE[iEnt];

            //['R2','cosTBTO','cosTBz','thrustOm','thrustBm']
            candB->R2                       = recB->R2[iEnt];
            candB->cosTBTO                  = recB->cosTBTO[iEnt];
            candB->cosTBz                   = recB->cosTBz[iEnt];
            candB->thrustOm                 = recB->thrustOm[iEnt];
            candB->thrustBm                 = recB->thrustBm[iEnt];

            candB->foxWolframR1             = recB->foxWolframR1[iEnt];
            candB->foxWolframR2             = recB->foxWolframR2[iEnt];
            candB->foxWolframR3             = recB->foxWolframR3[iEnt];
            candB->foxWolframR4             = recB->foxWolframR4[iEnt];

            candB->harmonicMomentThrust0    = recB->harmonicMomentThrust0[iEnt];
            candB->harmonicMomentThrust1    = recB->harmonicMomentThrust1[iEnt];
            candB->harmonicMomentThrust2    = recB->harmonicMomentThrust2[iEnt];
            candB->harmonicMomentThrust3    = recB->harmonicMomentThrust3[iEnt];
            candB->harmonicMomentThrust4    = recB->harmonicMomentThrust4[iEnt];

            candB->cleoConeThrust0          = recB->cleoConeThrust0[iEnt];
            candB->cleoConeThrust1          = recB->cleoConeThrust1[iEnt];
            candB->cleoConeThrust2          = recB->cleoConeThrust2[iEnt];
            candB->cleoConeThrust3          = recB->cleoConeThrust3[iEnt];
            candB->cleoConeThrust4          = recB->cleoConeThrust4[iEnt];
            candB->cleoConeThrust5          = recB->cleoConeThrust5[iEnt];
            candB->cleoConeThrust6          = recB->cleoConeThrust6[iEnt];
            candB->cleoConeThrust7          = recB->cleoConeThrust7[iEnt];
            candB->cleoConeThrust8          = recB->cleoConeThrust8[iEnt];

            candB->sphericity               = recB->sphericity[iEnt];
            candB->aplanarity               = recB->aplanarity[iEnt];
            candB->thrust                   = recB->thrust[iEnt];
            candB->thrustAxisCosTheta       = recB->thrustAxisCosTheta[iEnt];

            candB->CS_hso00                 = recB->CS_hso00[iEnt];
            candB->CS_hso02                 = recB->CS_hso02[iEnt];
            candB->CS_hso04                 = recB->CS_hso04[iEnt];
            candB->CS_hso10                 = recB->CS_hso10[iEnt];
            candB->CS_hso12                 = recB->CS_hso12[iEnt];
            candB->CS_hso14                 = recB->CS_hso14[iEnt];
            candB->CS_hso20                 = recB->CS_hso20[iEnt];
            candB->CS_hso22                 = recB->CS_hso22[iEnt];
            candB->CS_hso24                 = recB->CS_hso24[iEnt];
            candB->CS_hoo1                  = recB->CS_hoo1 [iEnt];
            candB->CS_hoo2                  = recB->CS_hoo2 [iEnt];
            candB->CS_hoo3                  = recB->CS_hoo3 [iEnt];
            candB->CS_hoo4                  = recB->CS_hoo4 [iEnt];
            candB->CS_et                    = recB->CS_et   [iEnt];
            candB->CS_mm2                   = recB->CS_mm2  [iEnt];

            double PDG_1;
            double PDG_2;
            double isMixed;
            double isBBbar;
            double isBBmix;
            PDG_1 = dec->PDG_1[0];
            PDG_2 = dec->PDG_2[0];
            if(dec->nCand==1){
                isBBbar=1.;
            }else{ 
                isBBbar=0.;
            }
            if(dec->nCand==1&&abs(PDG_1)==511&&abs(PDG_2)==511){
                isMixed = 1.;
                if(PDG_1+PDG_2==0){
                    isBBmix = -1;
                }else{
                    isBBmix = 1;
                }

            }else{
                isMixed = 0.;
            }

            candB->isBBbar                  = isBBbar;
            candB->isMixed                  = isMixed;
            candB->isBphiK                  = hasSigChain;
            candB->isBBmix                  = isBBmix;

            candB->MCflvrOtherB             = recB->MCflvrOtherB[iEnt];


            cBPhi->isSignal                 = recBPhi->isSignal[iEnt];
            cBPhi->M                        = recBPhi->M[iEnt];
            cBPhi->p                        = recBPhi->p[iEnt];
            cBPhi->pstar                    = recBPhi->pstar[iEnt];
            cBPhi->cosHel                   = recBPhi->cosHel[iEnt];

            cBKs0->isSignal                 = recBKs0->isSignal[iEnt];
            cBKs0->M                        = recBKs0->M[iEnt];
            cBKs0->p                        = recBKs0->p[iEnt];
            cBKs0->pstar                    = recBKs0->pstar[iEnt];

            candB->weight                   = recB->weight;

            B0_cand -> Fill();
        }
    }//events
    //////////////////////////////////////////////////

    fOut.cd();
    B0_cand -> Write();
    B0_cand -> Print();
    fOut.Close();

    return;
}
