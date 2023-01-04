#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

#include "MyPhiRec.h"
#include "MySetStyle.h"
void OriginalCutsEff(){
    TChain *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile("/home/belle2/zhai/B0_phi_KS0/Phi_cut_selection_Combo/Combo_phi_selection.root");
    
    int Entries = phi_Rec -> GetEntries();

    MyPhiRec rec;
    rec.setBranchAddresses(phi_Rec);

    TH1F *h_M_all = new TH1F("h_M_all","",80,0.98,1.06);
    TH1F *h_M_sig = new TH1F("h_M_sig","",80,0.98,1.06);
    TH1F *h_M_bkg = new TH1F("h_M_bkg","",80,0.98,1.06);

    //in 3-sigma region
    int totSig =0;
    int totBkg =0;
    const double kaonIDcut = 0.1;
    int passSigTrk_kid = 0;
    int failBkgTrk_kid = 0;
    int passSigTrk_cdc = 0;
    int failBkgTrk_cdc = 0;
    int passSigTrk_IPs = 0;
    int failBkgTrk_IPs = 0;
    int passSigTrk_std = 0;
    int failBkgTrk_std = 0;

    for(int i=0; i < Entries; i++){
        phi_Rec -> GetEntry(i);
        for(int iEnt = 0; iEnt<rec.nCand; iEnt++){
            if(rec.M[iEnt]<=0.98||rec.M[iEnt]>=1.06)continue;
            h_M_all->Fill(rec.M[iEnt]);
            if(rec.isSignal[iEnt]==1)   h_M_sig->Fill(rec.M[iEnt]);
            else                        h_M_bkg->Fill(rec.M[iEnt]);
            
            if(rec.M[iEnt]<1.037&&rec.M[iEnt]>1.002){
                bool dau0_pass_kid = rec.dau_0_kaonID[iEnt]>kaonIDcut;
                bool dau1_pass_kid = rec.dau_1_kaonID[iEnt]>kaonIDcut;
                bool dau0_pass_cdc = rec.dau_0_thetaInCDCAcceptance[iEnt]>0 && rec.dau_0_nCDCHits[iEnt]>20;
                bool dau1_pass_cdc = rec.dau_1_thetaInCDCAcceptance[iEnt]>0 && rec.dau_1_nCDCHits[iEnt]>20;
                bool dau0_pass_IPs = rec.dau_0_d0[iEnt]<0.5 && abs(rec.dau_0_z0[iEnt])<2;
                bool dau1_pass_IPs = rec.dau_1_d0[iEnt]<0.5 && abs(rec.dau_1_z0[iEnt])<2;
                bool dau0_pass_all = dau0_pass_kid && dau0_pass_cdc && dau0_pass_IPs;
                bool dau1_pass_all = dau1_pass_kid && dau1_pass_cdc && dau1_pass_IPs;

                if(rec.isSignal[iEnt]==1){
                    totSig++;
                    if(dau0_pass_kid) passSigTrk_kid++;
                    if(dau1_pass_kid) passSigTrk_kid++;
                    if(dau0_pass_cdc) passSigTrk_cdc++;
                    if(dau1_pass_cdc) passSigTrk_cdc++;
                    if(dau0_pass_IPs) passSigTrk_IPs++;
                    if(dau1_pass_IPs) passSigTrk_IPs++;
                    if(dau0_pass_all) passSigTrk_std++;
                    if(dau1_pass_all) passSigTrk_std++;
                } else{
                    totBkg++;
                    if(!dau0_pass_kid) failBkgTrk_kid++;
                    if(!dau1_pass_cdc) failBkgTrk_kid++;
                    if(!dau0_pass_IPs) failBkgTrk_cdc++;
                    if(!dau1_pass_IPs) failBkgTrk_cdc++;
                    if(!dau0_pass_all) failBkgTrk_IPs++;
                    if(!dau1_pass_all) failBkgTrk_IPs++;
                    if(!dau0_pass_all)failBkgTrk_std++;
                    if(!dau1_pass_all)failBkgTrk_std++;
                }
            }//3-sigma mass range
            
        }//candidates
    }//phi:all

    cout<<"cut(s)\t\t\tsig eff\t\tbkg rej\t\t\tsignifiance"<<endl;
    cout<<"totPHi\t\t\t"<<totSig<<"\t\t\t"                  <<totBkg                                                                           <<endl;
    cout<<"kaonID\t\t\t"<<passSigTrk_kid*0.5/totSig<<"\t"   <<failBkgTrk_kid*0.5/totBkg<<"\t"    <<passSigTrk_kid/sqrt(totBkg*2-failBkgTrk_kid)<<endl;
    cout<<"CDCcut\t\t\t"<<passSigTrk_cdc*0.5/totSig<<"\t"   <<failBkgTrk_cdc*0.5/totBkg<<"\t"    <<passSigTrk_cdc/sqrt(totBkg*2-failBkgTrk_cdc)<<endl;
    cout<<"ipCut \t\t\t"<<passSigTrk_IPs*0.5/totSig<<"\t"   <<failBkgTrk_IPs*0.5/totBkg<<"\t"    <<passSigTrk_IPs/sqrt(totBkg*2-failBkgTrk_IPs)<<endl;
    cout<<endl;
    cout<<"stdCut\t\t\t"<<passSigTrk_std*0.5/totSig<<"\t"   <<failBkgTrk_std*0.5/totBkg<<"\t"    <<passSigTrk_std/sqrt(totBkg*2-failBkgTrk_std)<<endl;

    gStyle->SetOptStat(0);

    TCanvas *c_M = new TCanvas("c_M","c_M",500,400);
    setStyle(h_M_sig,kGreen+2);
    setStyle(h_M_bkg,kBlue+0);
    setStyle(h_M_all,kBlack+0,"M(K+K-)[GeV]","MC combo phi:all->K+:all K-:all");
    h_M_all->Draw();
    h_M_sig->Draw("same");
    h_M_bkg->Draw("same");
    h_M_all->Draw("same");
    c_M -> SaveAs("plots/MCcombo_all_phi_candidates.pdf"); 

    return;
}
