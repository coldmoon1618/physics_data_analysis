//Gonna use TChain to chain 100 files (each from 500 folders) together and plot them
//Finetune the drawing aspect
#include <iostream>
#include <fstream>

#include <TChain.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>
#include <TList.h>
//void splitTruthJets(TString jetRoot, unsigned long ini, unsigned long fin){
//  TString ind; ind.Form("%d",(ini/1000));
void MakePlots(TString jetRoot, int ini, int fin, float y_max){
  TH2F *h_ZGs     = new TH2F("h_ZGs",     "ZG between matched rJets and tJets",32,0,.64,32,0,.64);
  h_ZGs->GetXaxis()->SetTitle("Reco Jet ZG");
  h_ZGs->GetYaxis()->SetTitle("True Jet ZG");
  
  TH1F *h_mr_ZG_3  = new TH1F("h_mr_ZG_3",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *h_mr_ZG_4  = new TH1F("h_mr_ZG_4",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *h_mr_ZG_5  = new TH1F("h_mr_ZG_5",  "z_g of matched reco Jets",32,0,0.64);
  TH1F *h_mr_ZG_6  = new TH1F("h_mr_ZG_6",  "z_g of matched reco Jets",32,0,0.64);

  TH1F *h_mt_ZG_3  = new TH1F("h_mt_ZG_3",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *h_mt_ZG_4  = new TH1F("h_mt_ZG_4",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *h_mt_ZG_5  = new TH1F("h_mt_ZG_5",  "z_g of matched truth Jets",32,0,0.64);
  TH1F *h_mt_ZG_6  = new TH1F("h_mt_ZG_6",  "z_g of matched truth Jets",32,0,0.64);

  TH1F *h_mt_nc_r3  = new TH1F("h_mt_nc_r3", "nc for mtJets when mr_nc=3",15,0,15);
  TH1F *h_mt_nc_r4  = new TH1F("h_mt_nc_r4", "nc for mtJets when mr_nc=4",15,0,15);
  TH1F *h_mt_nc_r5  = new TH1F("h_mt_nc_r5", "nc for mtJets when mr_nc=5",15,0,15);
  TH1F *h_mt_nc_r6  = new TH1F("h_mt_nc_r6", "nc for mtJets when mr_nc=6",15,0,15);

  TH1F *h_tra_phi = new TH1F("h_tra_phi","phi distr for all charged particles used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_clu_phi = new TH1F("h_clu_phi","phi distr for all neutral particles used in reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_cha_phi = new TH1F("h_cha_phi","phi distr for all charged particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_neu_phi = new TH1F("h_neu_phi","phi distr for all neutral particles used in truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());

  TH1F *h_tra_eta = new TH1F("h_tra_eta","eta distr for all tracks used in reco jets",100,-5,5);
  TH1F *h_clu_eta = new TH1F("h_clu_eta","eta distr for all clusters used in reco jets",100,-5,5);  
  TH1F *h_neu_eta = new TH1F("h_neu_eta","eta distr for all neutral particles used in truth jets",100,-5,5);  
  TH1F *h_cha_eta = new TH1F("h_cha_eta","eta distr for all charged particles used in truth jets",100,-5,5);  

  TH1F *h_mrJet_phi = new TH1F("h_mrJet_phi","phi distribution for matched reco jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  TH1F *h_mtJet_phi = new TH1F("h_mtJet_phi","phi distribution for matched truth jets",20,-TMath::PiOver2(),3.0*TMath::PiOver2());
  
  TH1F *h_mrJet_eta = new TH1F("h_mrJet_eta","eta distribution for matched reco jets",100,-5,5);
  TH1F *h_mtJet_eta = new TH1F("h_mtJet_eta","eta distribution for matched truth jets",100,-5,5);
//~~~~~~~~~~~~~~~~ Input filename structure, individual style 
  //TString myDir  = "/sphenix/user/yzhai/Run12pp_zg/combine/"; 
  for(int fInd = ini; fInd <= fin; fInd++){
    TString plot_fname = "nc3456"+jetRoot+"_"+Form("%d",fInd)+".root";
    cout<<plot_fname<<endl;
  
    TFile fIn(plot_fname);
    if(fIn.IsZombie()) {fIn.Close();cout<<"Zombine file skipped"<<endl; continue;}
    
    h_ZGs     -> Add((TH2F*)fIn.Get("H_ZGs"));
    
    h_mr_ZG_3  -> Add((TH1F*)fIn.Get("H_MR_ZG_3"));
    h_mr_ZG_4  -> Add((TH1F*)fIn.Get("H_MR_ZG_4"));
    h_mr_ZG_5  -> Add((TH1F*)fIn.Get("H_MR_ZG_5"));
    h_mr_ZG_6  -> Add((TH1F*)fIn.Get("H_MR_ZG_6"));
    
    h_mt_ZG_3  -> Add((TH1F*)fIn.Get("H_MT_ZG_3"));
    h_mt_ZG_4  -> Add((TH1F*)fIn.Get("H_MT_ZG_4"));
    h_mt_ZG_5  -> Add((TH1F*)fIn.Get("H_MT_ZG_5"));
    h_mt_ZG_6  -> Add((TH1F*)fIn.Get("H_MT_ZG_6"));
    
    h_mt_nc_r3 -> Add((TH1F*)fIn.Get("H_mt_nc_r3"));
    h_mt_nc_r4 -> Add((TH1F*)fIn.Get("H_mt_nc_r4"));
    h_mt_nc_r5 -> Add((TH1F*)fIn.Get("H_mt_nc_r5"));
    h_mt_nc_r6 -> Add((TH1F*)fIn.Get("H_mt_nc_r6"));
    
    h_tra_phi   -> Add((TH1F*)fIn.Get("H_tra_phi"));
    h_clu_phi   -> Add((TH1F*)fIn.Get("H_clu_phi"));
    h_cha_phi   -> Add((TH1F*)fIn.Get("H_cha_phi"));
    h_neu_phi   -> Add((TH1F*)fIn.Get("H_neu_phi"));
    
    h_tra_eta   -> Add((TH1F*)fIn.Get("H_tra_eta"));
    h_clu_eta   -> Add((TH1F*)fIn.Get("H_clu_eta"));
    h_cha_eta   -> Add((TH1F*)fIn.Get("H_cha_eta"));
    h_neu_eta   -> Add((TH1F*)fIn.Get("H_neu_eta"));
    
    h_mrJet_phi -> Add((TH1F*)fIn.Get("H_mrJet_phi"));
    h_mtJet_phi -> Add((TH1F*)fIn.Get("H_mtJet_phi"));
    h_mrJet_eta -> Add((TH1F*)fIn.Get("H_mrJet_eta"));
    h_mtJet_eta -> Add((TH1F*)fIn.Get("H_mtJet_eta"));

    fIn.Close();
   }//end index
  
  TCanvas *c1 = new TCanvas("c1","mr mt nc corrln",1000,1000);
  gStyle -> SetOptStat(0);
  c1 -> Divide(2,2,0.01,0.01,0);
  c1 -> cd(1);
  h_mt_nc_r3 -> DrawCopy();
  c1 -> cd(2);
  h_mt_nc_r4 -> DrawCopy();
  c1 -> cd(3);
  h_mt_nc_r5 -> DrawCopy();
  c1 -> cd(4);
  h_mt_nc_r6 -> DrawCopy();
  c1 -> SaveAs("mr_mt_ncs_3456"+jetRoot+".png");

  TCanvas *c2 = new TCanvas("c2","truth zg distr",1500,500);
  gStyle -> SetOptStat(0);
  c2 -> Divide(3,1,0.01,0.01,0);
  c2 -> cd(1);
  h_mr_ZG_3 -> SetLineColor(1);
  h_mr_ZG_3 -> SetAxisRange(0.,y_max,"Y");
  h_mr_ZG_3 -> DrawCopy();
  h_mr_ZG_4 -> SetLineColor(2);
  h_mr_ZG_4 -> SetAxisRange(0.,y_max,"Y");
  h_mr_ZG_4 -> DrawCopy("same");
  h_mr_ZG_5 -> SetLineColor(3);
  h_mr_ZG_5 -> SetAxisRange(0.,y_max,"Y");
  h_mr_ZG_5 -> DrawCopy("same");
  h_mr_ZG_6 -> SetLineColor(4);
  h_mr_ZG_6 -> SetAxisRange(0.,y_max,"Y");
  h_mr_ZG_6 -> DrawCopy("same");

  TLegend *mr_legend = new TLegend(0.75,0.7,0.9,0.9);
  mr_legend -> AddEntry("h_mr_ZG_3","r_nc=3","l");
  mr_legend -> AddEntry("h_mr_ZG_4","r_nc=4","l");
  mr_legend -> AddEntry("h_mr_ZG_5","r_nc=5","l");
  mr_legend -> AddEntry("h_mr_ZG_6","r_nc=6","l");
  mr_legend -> Draw("same");

  c2 -> cd(2);
  h_mt_ZG_3 -> SetLineColor(1);
  h_mt_ZG_3 -> SetAxisRange(0.,y_max,"Y");
  h_mt_ZG_3 -> DrawCopy();
  h_mt_ZG_4 -> SetLineColor(2);
  h_mt_ZG_4 -> SetAxisRange(0.,y_max,"Y");
  h_mt_ZG_4 -> DrawCopy("same");
  h_mt_ZG_5 -> SetLineColor(3);
  h_mt_ZG_5 -> SetAxisRange(0.,y_max,"Y");
  h_mt_ZG_5 -> DrawCopy("same");
  h_mt_ZG_6 -> SetLineColor(4);
  h_mt_ZG_6 -> SetAxisRange(0.,y_max,"Y");
  h_mt_ZG_6 -> DrawCopy("same");
  
  TLegend *mt_legend = new TLegend(0.75,0.7,0.9,0.9);
  mt_legend -> AddEntry("h_mt_ZG_3","r_nc=3","l");
  mt_legend -> AddEntry("h_mt_ZG_4","r_nc=4","l");
  mt_legend -> AddEntry("h_mt_ZG_5","r_nc=5","l");
  mt_legend -> AddEntry("h_mt_ZG_6","r_nc=6","l");
  mt_legend -> Draw("same");

  c2 -> cd(3);
  h_ZGs     -> DrawCopy("colz");
  c2 -> SaveAs("matched_nc3456"+jetRoot+".png");

  TCanvas *c3 = new TCanvas("c3","Reco phi distr",1000,500);
  c3 -> Divide(2,1,0.01,0.01,0);
  c3 -> cd(1);
  h_tra_phi -> Draw();
  c3 -> cd(2);
  h_clu_phi -> Draw();
  c3 -> SaveAs("rPhi"+jetRoot+".png");

  TCanvas *c4 = new TCanvas("c4","Truth phi distr",1000,500);
  c4 -> Divide(2,1,0.01,0.01,0);
  c4 -> cd(1);
  h_cha_phi -> Draw();
  c4 -> cd(2);
  h_neu_phi -> Draw();
  c4 -> SaveAs("tPhi"+jetRoot+".png");
  
  TCanvas *c5 = new TCanvas("c5","Reco eta distr",1000,500);
  c5 -> Divide(2,1,0.01,0.01,0);
  c5 -> cd(1);
  h_tra_eta -> Draw();
  c5 -> cd(2);
  h_clu_eta -> Draw();
  c5 -> SaveAs("rEta"+jetRoot+".png");

  TCanvas *c6 = new TCanvas("c6","Truth eta distr",1000,500);
  c6 -> Divide(2,1,0.01,0.01,0);
  c6 -> cd(1);
  h_cha_eta -> Draw();
  c6 -> cd(2);
  h_neu_eta -> Draw();
  c6 -> SaveAs("tEta"+jetRoot+".png");

  TCanvas *c7 = new TCanvas("c7","jet phi comparison",1000,500);
  c7 -> Divide(2,1,0.01,0.01,0);
  c7 -> cd(1);
  h_mrJet_phi -> Draw();
  c7 -> cd(2);
  h_mtJet_phi -> Draw();
  c7 -> SaveAs("phi_mat_pair"+jetRoot+".png");

  TCanvas *c8 = new TCanvas("c8","jet eta comparison",1000,500);
  c8 -> Divide(2,1,0.01,0.01,0);
  c8 -> cd(1);
  h_mrJet_eta -> Draw();
  c8 -> cd(2);
  h_mtJet_eta -> Draw();
  c8 -> SaveAs("etai_mat_pair"+jetRoot+".png");

 return;

}//end MakePlots
