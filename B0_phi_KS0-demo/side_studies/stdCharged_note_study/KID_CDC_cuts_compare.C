#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

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

#include "MyPhiRec.h"
#include "MySetStyle.h"

void KID_CDC_cuts_compare() {
    TChain *phi_Rec = new TChain("phi_Rec");
    phi_Rec -> AddFile("/home/belle2/zhai/B0_phi_KS0/Phi_cut_selection_Combo/Combo_phi_selection.root");
    
    int Entries = phi_Rec -> GetEntries();

    MyPhiRec rec;
    rec.setBranchAddresses(phi_Rec);

    TH1F *h005_M_all = new TH1F("h005_M_all","",80,0.98,1.06);
    TH1F *h005_M_sig = new TH1F("h005_M_sig","",80,0.98,1.06);
    TH1F *h005_M_bkg = new TH1F("h005_M_bkg","",80,0.98,1.06);

    TH1F *h010_M_all = new TH1F("h010_M_all","",80,0.98,1.06);
    TH1F *h010_M_sig = new TH1F("h010_M_sig","",80,0.98,1.06);
    TH1F *h010_M_bkg = new TH1F("h010_M_bkg","",80,0.98,1.06);

    TH1F *h015_M_all = new TH1F("h015_M_all","",80,0.98,1.06);
    TH1F *h015_M_sig = new TH1F("h015_M_sig","",80,0.98,1.06);
    TH1F *h015_M_bkg = new TH1F("h015_M_bkg","",80,0.98,1.06);

    int totSig = 0;
    int totBkg = 0;
    int passSigTrk_005 = 0;
    int failBkgTrk_005 = 0;
    int passSigTrk_010 = 0;
    int failBkgTrk_010 = 0;
    int passSigTrk_015 = 0;
    int failBkgTrk_015 = 0;
    int passSigTrk_k00 = 0;
    int failBkgTrk_k00 = 0;
    int passSigTrk_k20 = 0;
    int failBkgTrk_k20 = 0;
    int passSigTrk_4_2 = 0;
    int failBkgTrk_4_2 = 0;
    int passSigTrk_3_1 = 0;
    int failBkgTrk_3_1 = 0;
    int passSigTrk_2_5 = 0;
    int failBkgTrk_2_5 = 0;
    
    for(int i=0; i < Entries; i++){
        phi_Rec -> GetEntry(i);
        for(int iEnt=0; iEnt<rec.nCand; iEnt++){
            if(rec.M[iEnt]<=0.98||rec.M[iEnt]>=1.06)continue;
            bool dau0_pass_005 = rec.dau_0_kaonID[iEnt]>0.05;
            bool dau1_pass_005 = rec.dau_1_kaonID[iEnt]>0.05;
            bool dau0_pass_010 = rec.dau_0_kaonID[iEnt]>0.10;
            bool dau1_pass_010 = rec.dau_1_kaonID[iEnt]>0.10;
            bool dau0_pass_015 = rec.dau_0_kaonID[iEnt]>0.15;
            bool dau1_pass_015 = rec.dau_1_kaonID[iEnt]>0.15;
            
            bool dau0_pass_k00 = dau0_pass_005 && rec.dau_0_nCDCHits[iEnt]>0;
            bool dau1_pass_k00 = dau1_pass_005 && rec.dau_1_nCDCHits[iEnt]>0;
            bool dau0_pass_k20 = dau0_pass_005 && rec.dau_0_nCDCHits[iEnt]>20;
            bool dau1_pass_k20 = dau1_pass_005 && rec.dau_1_nCDCHits[iEnt]>20;

            bool dau0_pass_4_2 = dau0_pass_k00 && abs(rec.dau_0_dz[iEnt])<4 && rec.dau_0_dr[iEnt]<2 ;
            bool dau1_pass_4_2 = dau1_pass_k00 && abs(rec.dau_1_dz[iEnt])<4 && rec.dau_1_dr[iEnt]<2 ;
            bool dau0_pass_3_1 = dau0_pass_k00 && abs(rec.dau_0_dz[iEnt])<3 && rec.dau_0_dr[iEnt]<1 ;
            bool dau1_pass_3_1 = dau1_pass_k00 && abs(rec.dau_1_dz[iEnt])<3 && rec.dau_1_dr[iEnt]<1 ;
            bool dau0_pass_2_5 = dau0_pass_k00 && abs(rec.dau_0_dz[iEnt])<2 && rec.dau_0_dr[iEnt]<.5;
            bool dau1_pass_2_5 = dau1_pass_k00 && abs(rec.dau_1_dz[iEnt])<2 && rec.dau_1_dr[iEnt]<.5;
            
            double mass = rec.M[iEnt];
            double isSignal = rec.isSignal[iEnt]==1;

            //kID>0.05
            if(dau0_pass_005&&dau1_pass_005){
                h005_M_all->Fill(mass);
                if(isSignal)h005_M_sig->Fill(mass);
                else        h005_M_bkg->Fill(mass);
                
                //kID>0.10
                if(dau0_pass_010&&dau1_pass_010){
                    h010_M_all->Fill(mass);
                    if(isSignal)h010_M_sig->Fill(mass);
                    else        h010_M_bkg->Fill(mass);

                    //kID>0.05
                    if(dau0_pass_015&&dau1_pass_015){
                        h015_M_all->Fill(mass);
                        if(isSignal)h015_M_sig->Fill(mass);
                        else        h015_M_bkg->Fill(mass);
                    }//kID>0.15

                }//kID>0.10
            
            }//kID>0.05

            //3-sigma mass range
            if(mass<1.037&&mass>1.002){
                if(isSignal){
                    totSig++;
                    if(dau0_pass_005)   passSigTrk_005++;
                    if(dau1_pass_005)   passSigTrk_005++;
                    if(dau0_pass_010)   passSigTrk_010++;
                    if(dau1_pass_010)   passSigTrk_010++;
                    if(dau0_pass_015)   passSigTrk_015++;
                    if(dau1_pass_015)   passSigTrk_015++;

                    if(dau0_pass_k00)   passSigTrk_k00++;
                    if(dau1_pass_k00)   passSigTrk_k00++;
                    if(dau0_pass_k20)   passSigTrk_k20++;
                    if(dau1_pass_k20)   passSigTrk_k20++;

                    if(dau0_pass_4_2)   passSigTrk_4_2++;
                    if(dau1_pass_4_2)   passSigTrk_4_2++;
                    if(dau0_pass_3_1)   passSigTrk_3_1++;
                    if(dau1_pass_3_1)   passSigTrk_3_1++;
                    if(dau0_pass_2_5)   passSigTrk_2_5++;
                    if(dau1_pass_2_5)   passSigTrk_2_5++;
                
                }else{
                    totBkg++;
                    if(!dau0_pass_005)  failBkgTrk_005++;
                    if(!dau1_pass_005)  failBkgTrk_005++; 
                    if(!dau0_pass_010)  failBkgTrk_010++;
                    if(!dau1_pass_010)  failBkgTrk_010++; 
                    if(!dau0_pass_015)  failBkgTrk_015++;
                    if(!dau1_pass_015)  failBkgTrk_015++; 

                    if(!dau0_pass_k00)  failBkgTrk_k00++;
                    if(!dau1_pass_k00)  failBkgTrk_k00++;
                    if(!dau0_pass_k20)  failBkgTrk_k20++;
                    if(!dau1_pass_k20)  failBkgTrk_k20++;

                    if(!dau0_pass_4_2)  failBkgTrk_4_2++;
                    if(!dau1_pass_4_2)  failBkgTrk_4_2++;
                    if(!dau0_pass_3_1)  failBkgTrk_3_1++;
                    if(!dau1_pass_3_1)  failBkgTrk_3_1++;
                    if(!dau0_pass_2_5)  failBkgTrk_2_5++;
                    if(!dau1_pass_2_5)  failBkgTrk_2_5++;
                
                }
            }//3_sigma mass range

        }//candidates
    }//phi

    cout<<"cut(s)\t\t\tsig eff\t\tbkg rej\t\t\tsignifiance"<<endl;
    cout<<"kaonID>0.05\t\t\t"<<passSigTrk_005*.5/totSig<<"\t"   <<failBkgTrk_005*.5/totBkg<<"\t"    <<passSigTrk_005/sqrt(totBkg*2-failBkgTrk_005)<<endl;
    cout<<"kaonID>0.10\t\t\t"<<passSigTrk_010*.5/totSig<<"\t"   <<failBkgTrk_010*.5/totBkg<<"\t"    <<passSigTrk_010/sqrt(totBkg*2-failBkgTrk_010)<<endl;
    cout<<"kaonID>0.15\t\t\t"<<passSigTrk_015*.5/totSig<<"\t"   <<failBkgTrk_015*.5/totBkg<<"\t"    <<passSigTrk_015/sqrt(totBkg*2-failBkgTrk_015)<<endl;

    cout<<"On top of kaonID>0.05, relative"<<endl;
    cout<<"nCDCHits>0 \t\t\t"<<passSigTrk_k00/(passSigTrk_005*1.)<<"\t"   <<1.-(totBkg-failBkgTrk_k00*.5)/(totBkg-failBkgTrk_005*.5)<<"\t"    <<passSigTrk_k00/sqrt(totBkg*2-failBkgTrk_k00)<<endl;
    cout<<"nCDCHits>20\t\t\t"<<passSigTrk_k20/(passSigTrk_005*1.)<<"\t"   <<1.-(totBkg-failBkgTrk_k20*.5)/(totBkg-failBkgTrk_005*.5)<<"\t"    <<passSigTrk_k20/sqrt(totBkg*2-failBkgTrk_k20)<<endl;
    
    cout<<"On top of kaonID and nCDCHits>0"<<endl;
    cout<<"dr<4 and abs(dz)<2\t\t" <<passSigTrk_4_2/(passSigTrk_k00*1.)<<"\t"   <<1.-(totBkg-failBkgTrk_4_2*.5)/(totBkg-failBkgTrk_k00*.5)<<"\t" <<passSigTrk_4_2/sqrt(totBkg*2-failBkgTrk_4_2)<<endl;
    cout<<"dr<3 and abs(dz)<1\t\t" <<passSigTrk_3_1/(passSigTrk_k00*1.)<<"\t"   <<1.-(totBkg-failBkgTrk_3_1*.5)/(totBkg-failBkgTrk_k00*.5)<<"\t"    <<passSigTrk_3_1/sqrt(totBkg*2-failBkgTrk_3_1)<<endl;
    cout<<"dr<2 and abs(dz)<.5\t\t"<<passSigTrk_2_5/(passSigTrk_k00*1.)<<"\t"   <<1.-(totBkg-failBkgTrk_2_5*.5)/(totBkg-failBkgTrk_k00*.5)<<"\t"    <<passSigTrk_2_5/sqrt(totBkg*2-failBkgTrk_2_5)<<endl;

    gStyle->SetOptStat(0);

    setStyle(h005_M_all,kBlack+0,"M(K+K-)[GeV]","MC combo with kaonID>0.05 on tracks");
    setStyle(h010_M_all,kBlack+0,"M(K+K-)[GeV]","MC combo with kaonID>0.1 on tracks");
    setStyle(h015_M_all,kBlack+0,"M(K+K-)[GeV]","MC combo with kaonID>0.15 on tracks");

    TCanvas *c_005 = new TCanvas("c_005","c_005",500,400);
    setStyle(h005_M_sig,kGreen+2);
    setStyle(h005_M_bkg,kBlue+0);
    h005_M_all->Draw();
    h005_M_sig->Draw("same");
    h005_M_bkg->Draw("same");
    h005_M_all->Draw("same");
    c_005 -> SaveAs("plots/MCcombo_kaonID005.pdf");  

    TCanvas *c_010 = new TCanvas("c_010","c_010",500,400);
    setStyle(h010_M_sig,kGreen+2);
    setStyle(h010_M_bkg,kBlue+0);
    h010_M_all->Draw();
    h010_M_sig->Draw("same");
    h010_M_bkg->Draw("same");
    h010_M_all->Draw("same");
    c_010 -> SaveAs("plots/MCcombo_kaonID010.pdf");  

    TCanvas *c_015 = new TCanvas("c_015","c_015",500,400);
    setStyle(h015_M_sig,kGreen+2);
    setStyle(h015_M_bkg,kBlue+0);
    h015_M_all->Draw();
    h015_M_sig->Draw("same");
    h015_M_bkg->Draw("same");
    h015_M_all->Draw("same");
    c_015 -> SaveAs("plots/MCcombo_kaonID015.pdf");  

    return ;
}
