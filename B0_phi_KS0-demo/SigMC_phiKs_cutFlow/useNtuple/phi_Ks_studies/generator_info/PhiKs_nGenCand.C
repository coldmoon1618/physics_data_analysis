#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyB0MC.h"
#include "MyKsRec.h"
#include "MyPhiRec.h"
#include "MyPhiKsMC.h"

#include "MySetStyle.h"
void PhiKs_nGenCand(){
    TString fileName("/home/belle2/zhai/B0_phi_KS0/SigMC_cutflow/getNtuple/SigMC_B0_Rec_20k.root");
   
    TChain *Ks_MC   = new TChain("Ks_MC");
    Ks_MC   -> AddFile(fileName);
    MyPhiKsMC genKs;
    genKs.setBranchAddresses(Ks_MC);
    int Entries = Ks_MC -> GetEntries();

    TChain *phi_MC  = new TChain("phi_MC");
    phi_MC -> AddFile(fileName);
    MyPhiKsMC genPhi;
    genPhi.setBranchAddresses(phi_MC);

    TChain *B0_MC   = new TChain("B0_MC");
    B0_MC   -> AddFile(fileName);
    MyB0MC mc;
    mc.setBranchAddresses(B0_MC);
    MyPhiKsMC mcPhi;
    mcPhi.setBranchAddresses(B0_MC, "phi_");
    MyPhiKsMC mcKs;
    mcKs.setBranchAddresses(B0_MC, "Ks_");

    TH1F *h_Ks0_allCand   = new TH1F("h_Ks0_allCand","h_Ks0_allCand",30,0,6);
    TH1F *h_phi_allCand   = new TH1F("h_phi_allCand","h_phi_allCand",30,0,6);

    TH1F *h_Ks0_sigCand   = new TH1F("h_Ks0_sigCand","h_Ks0_sigCand",30,0,6);
    TH1F *h_phi_sigCand   = new TH1F("h_phi_sigCand","h_phi_sigCand",30,0,6);

    TH1F *hGenKs0_p   = new TH1F("hGenKs0_p","hGenKs0_p",50,0,5);
    TH1F *hGenPhi_p   = new TH1F("hGenPhi_p","hGenPhi_p",50,0,5);

    TH1F *hGenKs0_p2  = new TH1F("hGenKs0_p2","hGenKs0_p2",50,0,5);
    TH1F *hGenPhi_p2  = new TH1F("hGenPhi_p2","hGenPhi_p2",50,0,5);

    for(int i=0; i < Entries; i++){
        Ks_MC   -> GetEntry(i);
        phi_MC  -> GetEntry(i);
        B0_MC   -> GetEntry(i);
    //////////////////////////////////////////////////
        bool hasSigChain = false;
        for(int iEnt = 0; iEnt< mc.nCand; iEnt++){
            if(mcPhi.nDaus[iEnt]>=2 && mcKs.nDaus[iEnt]>=2
                    && abs(mcPhi.dau_0_PDG[iEnt])==321 && abs(mcPhi.dau_1_PDG[iEnt])==321
                    && abs( mcKs.dau_0_PDG[iEnt])==211 && abs( mcKs.dau_1_PDG[iEnt])==211){
                hasSigChain = true;
            }
        }//B0_MC ?hasSigChain

    //////////////////////////////////////////////////
        int Ks_allCand = 0;
        int Ks_sigCand = 0;
        for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
            if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                Ks_allCand++;
                if(hasSigChain) Ks_sigCand++;
            }
        }//genKs
        h_Ks0_allCand -> Fill(Ks_allCand);
        int phi_allCand = 0;
        int phi_sigCand = 0;
        for(int iEnt = 0; iEnt<genPhi.nCand; iEnt++){
            if(abs(genPhi.dau_0_PDG[iEnt])==321 && abs(genPhi.dau_1_PDG[iEnt])==321){
                phi_allCand++;
                if(hasSigChain)phi_sigCand++;
            }
        }//genPhi
        h_phi_allCand -> Fill(phi_allCand);

    //////////////////////////////////////////////////
        if(!hasSigChain)continue;
        h_Ks0_sigCand -> Fill(Ks_sigCand);
        h_phi_sigCand -> Fill(phi_sigCand);

    //////////////////////////////////////////////////
        if(Ks_sigCand==1){
            for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
                if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                    hGenKs0_p -> Fill(genKs.p[iEnt]); 
                }
            }
        }//Single exact_Ks events
        else{
            for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
                if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                    hGenKs0_p2 -> Fill(genKs.p[iEnt]); 
                }
            }
        }

    //////////////////////////////////////////////////
        if(phi_sigCand==1){
            for(int iEnt = 0; iEnt<genPhi.nCand; iEnt++){
                if(abs(genPhi.dau_0_PDG[iEnt])==321 && abs(genPhi.dau_1_PDG[iEnt])==321){
                    hGenPhi_p -> Fill(genPhi.p[iEnt]);
                }
            }
        }//Single exact_phi events
        else{
            for(int iEnt = 0; iEnt<genPhi.nCand; iEnt++){
                if(abs(genPhi.dau_0_PDG[iEnt])==321 && abs(genPhi.dau_1_PDG[iEnt])==321){
                    hGenPhi_p2 -> Fill(genPhi.p[iEnt]);
                }
            }
        }

    }//nTotalEvents

    gStyle -> SetOptStat("nermous");

    TCanvas *c_allCand= new TCanvas("c_allCand","c_allCand",1000,400);
    c_allCand -> Divide(2,1,.01,.01);
    setStyle(h_Ks0_allCand,kBlue+0,"nCand","generated Ks(pipi) in all events");
    setStyle(h_phi_allCand,kBlue+0,"nCand","generated phi(KK)  in all events");
    c_allCand -> cd(1);
    h_Ks0_allCand -> Draw();
    gPad -> SetLogy();
    c_allCand -> cd(2);
    h_phi_allCand -> Draw();
    gPad -> SetLogy();
    c_allCand -> SaveAs("phi_Ks_nCand_allEvt.pdf");

    TCanvas *c_sigCand= new TCanvas("c_sigCand","c_sigCand",1000,400);
    c_sigCand -> Divide(2,1,.01,.01);
    setStyle(h_Ks0_sigCand,kBlue+0,"nCand","generated Ks(pipi) in sig evts");
    setStyle(h_phi_sigCand,kBlue+0,"nCand","generated phi(KK)  in sig evts");
    c_sigCand -> cd(1);
    h_Ks0_sigCand -> Draw();
    gPad -> SetLogy();
    c_sigCand -> cd(2);
    h_phi_sigCand -> Draw();
    gPad -> SetLogy();
    c_sigCand -> SaveAs("phi_Ks_nCand_sigEvt.pdf");

    gStyle -> SetOptStat("nems");
    TCanvas *c_genP = new TCanvas("c_genP","c_genP",1000,400);
    c_genP -> Divide(2,1,.01,.01);
    setStyle(hGenKs0_p, kBlue+0, "p","Single genKs(pipi) p");
    setStyle(hGenPhi_p, kBlue+0, "p","Single genPhi(KK)  p");
    c_genP -> cd(1);
    hGenKs0_p -> Draw();
    c_genP -> cd(2);
    hGenPhi_p -> Draw();
    c_genP -> SaveAs("phi_Ks_singleCand_p_distr.pdf");

    TCanvas *c2_genP = new TCanvas("c2_genP","c2_genP",1000,400);
    c2_genP -> Divide(2,1,.01,.01);
    setStyle(hGenKs0_p2, kBlue+0, "p","multi genKs(pipi) p");
    setStyle(hGenPhi_p2, kBlue+0, "p","multi genPhi(KK)  p");
    c2_genP -> cd(1);
    hGenKs0_p2 -> Draw();
    c2_genP -> cd(2);
    hGenPhi_p2 -> Draw();
    c2_genP -> SaveAs("phi_Ks_multiCand_p_distr.pdf");

    return;
}
