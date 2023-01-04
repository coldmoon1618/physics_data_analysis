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

#include <TDatabasePDG.h>
void PhiKs_genInfo(){
    auto *pdg_obj = TDatabasePDG::Instance();
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

    int noExactPhi_1 = 0;
    int noExactKs_1 = 0;
    int noExactPhi = 0;
    int noExactKs = 0;
    int neitherDau = 0;
    int noSigDirect = 0;

    int phi_trk_inCDC = 0;
    int phi_bot_inCDC = 0;
    int Ks0_trk_inCDC = 0;
    int Ks0_bot_inCDC = 0;
    int phi_trk_seen = 0;
    int phi_bot_seen = 0;
    int Ks0_trk_seen = 0;
    int Ks0_bot_seen = 0;
    int phi_trk_both = 0;
    int phi_bot_both = 0;
    int Ks0_trk_both = 0;
    int Ks0_bot_both = 0;

    bool printSecPhys = 0;
    bool usePDGnames = 0;

    TH1F *h1_Lxy = new TH1F("h1_Lxy","mcLxy of Ks not exact",100,0,100);
    TH1F *h2_Lxy = new TH1F("h2_Lxy","mcLxy of Ks -> pi+pi-",100,0,100);
    TH1F *h3_Lxy = new TH1F("h3_Lxy","mcLxy of phi->K+K-",100,0,5);

    TH1F *hp_Ks0 = new TH1F("hp_Ks0","hp_Ks0",100,0,5);
    TH1F *hp_phi = new TH1F("hp_phi","hp_phi",100,0,5);
    TH1F *hpstar_Ks0 = new TH1F("hpstar_Ks0","hpstar_Ks0",100,0,5);
    TH1F *hpstar_phi = new TH1F("hpstar_phi","hpstar_phi",100,0,5);

    for(int i=0; i < Entries; i++){
        Ks_MC   -> GetEntry(i);
        phi_MC  -> GetEntry(i);
        B0_MC   -> GetEntry(i);
    //////////////////////////////////////////////////
        bool exactPhi = false;
        bool exactKs = false;
        //Always only one B-> phi Ks
        if(mcPhi.nDaus[0]>=2){
            if(abs(mcPhi.dau_0_PDG[0])==321 && abs(mcPhi.dau_1_PDG[0])==321)exactPhi=true;
        }
        if(mcKs.nDaus[0]>=2){
            if(abs( mcKs.dau_0_PDG[0])==211 && abs( mcKs.dau_1_PDG[0])==211)exactKs=true;
        }
        bool hasSigChain=exactPhi && exactKs;
        if(hasSigChain){
            bool phi_0_in = mcPhi.dau_0_thetaInCDCAcceptance[0]==1;
            bool phi_1_in = mcPhi.dau_1_thetaInCDCAcceptance[0]==1;
            bool Ks0_0_in = mcKs.dau_0_thetaInCDCAcceptance[0]==1; 
            bool Ks0_1_in = mcKs.dau_1_thetaInCDCAcceptance[0]==1;
            bool phi_0_seen = mcPhi.dau_0_seenInCDC[0]==1;
            bool phi_1_seen = mcPhi.dau_1_seenInCDC[0]==1;
            bool Ks0_0_seen = mcKs.dau_0_seenInCDC[0]==1; 
            bool Ks0_1_seen = mcKs.dau_1_seenInCDC[0]==1;
            if(phi_0_in)phi_trk_inCDC++;
            if(phi_1_in)phi_trk_inCDC++;
            if(phi_0_in&&phi_1_in)phi_bot_inCDC++;
            if(Ks0_0_in)Ks0_trk_inCDC++;
            if(Ks0_1_in)Ks0_trk_inCDC++;
            if(Ks0_0_in&&Ks0_1_in)Ks0_bot_inCDC++;

            if(phi_0_seen)phi_trk_seen++;
            if(phi_1_seen)phi_trk_seen++;
            if(phi_0_seen&&phi_1_seen)phi_bot_seen++;
            if(Ks0_0_seen)Ks0_trk_seen++;
            if(Ks0_1_seen)Ks0_trk_seen++;
            if(Ks0_0_seen&&Ks0_1_seen)Ks0_bot_seen++;

            if(phi_0_seen&&phi_0_in)phi_trk_both++;
            if(phi_1_seen&&phi_1_in)phi_trk_both++;
            if(phi_0_seen&&phi_0_in&&phi_1_seen&&phi_1_in)phi_bot_both++;
            if(Ks0_0_seen&&Ks0_0_in)Ks0_trk_both++;
            if(Ks0_1_seen&&Ks0_1_in)Ks0_trk_both++;
            if(Ks0_0_seen&&Ks0_0_in&&Ks0_1_seen&&Ks0_1_in)Ks0_bot_both++;
        }
        
        if(!exactKs)h1_Lxy->Fill(mcKs.mcFlightDistance[0]);
        else        h2_Lxy->Fill(mcKs.mcFlightDistance[0]);
                    h3_Lxy->Fill(mcPhi.mcFlightDistance[0]);

    //////////////////////////////////////////////////
        int Ks_genCand = 0;
        for(int iEnt = 0; iEnt<genKs.nCand; iEnt++){
            hp_Ks0      -> Fill(genKs.p[iEnt]);
            hpstar_Ks0  -> Fill(genKs.pstar[iEnt]);
            if(abs(genKs.dau_0_PDG[iEnt])==211 && abs(genKs.dau_1_PDG[iEnt])==211){
                Ks_genCand++;
            }
        }//generated exact Kshort 

    //////////////////////////////////////////////////
        int phi_genCand = 0;
        for(int iEnt = 0; iEnt<genPhi.nCand; iEnt++){
            hp_phi      -> Fill(genPhi.p[iEnt]);
            hpstar_phi  -> Fill(genPhi.pstar[iEnt]);
            if(abs(genPhi.dau_0_PDG[iEnt])==321 && abs(genPhi.dau_1_PDG[iEnt])==321){
                phi_genCand++;
            }
        }//generated exact Phi
        if(!exactKs&&!exactPhi) neitherDau++;
        if(!hasSigChain)noSigDirect++;
        if(!exactKs ) noExactKs++; 
        if(!exactPhi)noExactPhi++;
        if(!exactKs &&Ks_genCand==1) noExactKs_1++; 
        if(!exactPhi&&phi_genCand==1)noExactPhi_1++;

        if(exactKs)continue;
        if(!printSecPhys)continue;
        cout<<"event"<<i<<endl<<" Ks : ";
        cout<<mcKs.dau_0_PDG[0]<<" ("<<mcKs.dau_0_mcSecPhysProc[0]<<")";
        if(!isnan(mcKs.dau_1_PDG[0]))cout<<" + "<<mcKs.dau_1_PDG[0]<<" ("<<mcKs.dau_1_mcSecPhysProc[0]<<")";
        if(!isnan(mcKs.dau_2_PDG[0]))cout<<" + "<<mcKs.dau_2_PDG[0]<<" ("<<mcKs.dau_2_mcSecPhysProc[0]<<")";
        if(!isnan(mcKs.dau_3_PDG[0]))cout<<" + "<<mcKs.dau_3_PDG[0]<<" ("<<mcKs.dau_3_mcSecPhysProc[0]<<")";
        cout<<endl;;

        if(!usePDGnames)continue;
        if(i>=10824)continue;
        
        cout<<" Ks : ";
        cout<<pdg_obj -> GetParticle(mcKs.dau_0_PDG[0])-> GetName()<<" ("<<mcKs.dau_0_mcSecPhysProc[0]<<")";
        if(!isnan(mcKs.dau_1_PDG[0]))cout<<" + "<<pdg_obj -> GetParticle(mcKs.dau_1_PDG[0])-> GetName()<<" ("<<mcKs.dau_1_mcSecPhysProc[0]<<")";
        if(!isnan(mcKs.dau_2_PDG[0]))cout<<" + "<<pdg_obj -> GetParticle(mcKs.dau_2_PDG[0])-> GetName()<<" ("<<mcKs.dau_2_mcSecPhysProc[0]<<")";
        if(!isnan(mcKs.dau_3_PDG[0]))cout<<" + "<<pdg_obj -> GetParticle(mcKs.dau_3_PDG[0])-> GetName()<<" ("<<mcKs.dau_3_mcSecPhysProc[0]<<")";
        cout<<endl;;
        //10824th event has weird number for dau2, string version otherwise works

    }//nTotalEvents

    cout<<"noExactKs    : "<<noExactKs<<endl;
    cout<<"noExactPhi   : "<<noExactPhi<<endl;
    cout<<"noExactKs_1  : "<<noExactKs_1<<endl;
    cout<<"noExactPhi_1 : "<<noExactPhi_1<<endl;
    cout<<"neitherDau   : "<<neitherDau<<endl;
    cout<<"noSigDirect  : "<<noSigDirect<<endl;

    cout<<endl;
    cout<<"phi trk "<<phi_trk_inCDC*.5/(20000-noSigDirect)<<endl;
    cout<<"Ks0 trk "<<Ks0_trk_inCDC*.5/(20000-noSigDirect)<<endl;
    cout<<"phi bot "<<phi_bot_inCDC*.5/(10000-noSigDirect*.5)<<endl;
    cout<<"Ks0 bot "<<Ks0_bot_inCDC*.5/(10000-noSigDirect*.5)<<endl;
    cout<<endl;
    cout<<"phi trk "<<phi_trk_seen*.5/(20000-noSigDirect)<<endl;
    cout<<"Ks0 trk "<<Ks0_trk_seen*.5/(20000-noSigDirect)<<endl;
    cout<<"phi bot "<<phi_bot_seen*.5/(10000-noSigDirect*.5)<<endl;
    cout<<"Ks0 bot "<<Ks0_bot_seen*.5/(10000-noSigDirect*.5)<<endl;
    cout<<endl;
    cout<<"phi trk "<<phi_trk_both*.5/(20000-noSigDirect)<<endl;
    cout<<"Ks0 trk "<<Ks0_trk_both*.5/(20000-noSigDirect)<<endl;
    cout<<"phi bot "<<phi_bot_both*.5/(10000-noSigDirect*.5)<<endl;
    cout<<"Ks0 bot "<<Ks0_bot_both*.5/(10000-noSigDirect*.5)<<endl;


    gStyle -> SetOptStat(0);
    setStyle(h1_Lxy, kRed+1, "mcLxy", "green exact red not");
    setStyle(h2_Lxy, kGreen+2, "mcLxy", "green exact red not");
    TCanvas *c1 = new TCanvas("c1","c1",500,400);
    h2_Lxy -> Draw();
    h1_Lxy -> Draw("same");
    gPad -> SetLogy();
    c1 -> SaveAs("Ks_Lxy.pdf");

    setStyle(h3_Lxy, kGreen+2, "mcLxy", "phi -> K+K-");
    TCanvas *c2 = new TCanvas("c2","c2",500,400);
    h3_Lxy -> Draw();
    gPad -> SetLogy();
    c2 -> SaveAs("phi_Lxy.pdf");

    setStyle(hp_phi, kBlue+0,"GeV","generated phi p");
    setStyle(hp_Ks0, kBlue+0,"GeV","generated Ks p");
    setStyle(hpstar_phi, kBlue+0,"GeV","generated phi p*");
    setStyle(hpstar_Ks0, kBlue+0,"GeV","generated Ks p*");
    TCanvas *c3 = new TCanvas("c3","c3",1000,800);
    c3 -> Divide(2,2,.01,.01);
    c3 -> cd(1);
    hp_phi -> Draw();
    c3 -> cd(2);
    hp_Ks0 -> Draw();
    c3 -> cd(3);
    hpstar_phi -> Draw();
    c3 -> cd(4);
    hpstar_Ks0 -> Draw();
    c3 -> SaveAs("phiKs_sigEvt_all_genP.pdf");

    return;
}
