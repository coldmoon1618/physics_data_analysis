#include <iostream>
#include <fstream>
#include <iomanip>

#include <TChain.h>
#include <TMath.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1.h>

//Functions need to be declare before called
float dphiReduce(float dphi){
  if (dphi < -TMath::Pi()) 
    dphi += TMath::TwoPi();
  if (dphi >=  TMath::Pi()) 
    dphi -= TMath::TwoPi();
  return dphi; 
}//end dphireduce()

float sub_dR(float eta1, float eta2, float phi1, float phi2){
  float sub_deta = eta1 - eta2;
  float sub_dphi = dphiReduce(phi1 - phi2);

  return sqrt(sub_dphi * sub_dphi + sub_deta * sub_deta);
}//

void normalize(TH1F *tHisto){
  double integral = tHisto -> Integral();
  tHisto	-> Scale(1.0/integral);
}
void normalize(TH2F *tHisto){
}

void readNewTree()
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ckin info
  int runno = 13;
  cout<<"Run13pp510"<<endl;

  const int NUMCKIN = 15; 
  int ckins[NUMCKIN]	  = {     5,	     10,	 20,	     30,         50, 
				 80,	    100,	120,	    135,        150, 
				170,  	    190,	210,	    225,	240};
  float crossSec[NUMCKIN] = 
			{9.7042E-01, 4.4605E-02,  1.2588e-3, 1.4216E-04, 5.2881E-06, 
			 1.1033E-07,  1.1303E-8, 1.0511E-09, 1.8485E-10, 2.9553E-11,
			 1.7788E-12, 5.9371E-14, 6.4681E-16, 5.3895E-18, 2.1154E-21}; 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~pT bins
  const int numType = 8;
  TString types[numType]= {"nc","oang","cf","z_g","nc1","nc2","cf1","cf2"};
  int nBins[numType]    = {  25,    40,  50,   32,   25,   25,   50,   50};
  float minX[numType]   = {   0,     0,   0,    0,    0,    0,    0,    0};
  float maxX[numType]   = {  25,    .4,   1,  .64,   25,   25,    1,    1};
  float maxY[numType]   = {  .2,  .095, .22,  .16,  .25,   .5,   .1,   .1};
  //float maxY[numType]   = {  .2,  .095, .13,  .16,  .25,   .5,   .1,   .1};
  TString desciption[numType] ={"Number of Constituents","Opening angle","Charged fraction","z_g",
  "nc of higher pT subjet","nc of lower pT subjet","cf of higher pT subjet","nc of lower pT subjet"};
  const int   numPt  = 1;//4;
  int minPt[numPt] = {10};//{20, 30, 40, 50};
  int maxPt[numPt] = {25};//{30, 40, 50, 60};
  int SimCounter[numPt] = {0};//{0,0,0,0};
  int DatCounter[numPt] = {0};//{0,0,0,0};
  TCanvas *c_ZGs[numPt];
  for(int iPt = 0; iPt < numPt; iPt++){
    c_ZGs[iPt] = new TCanvas(Form("c_ZGs_%i_%i",minPt[iPt],maxPt[iPt]),"ZGs", 2500, 1500);
    c_ZGs[iPt] -> Divide(5,3,0.01,0.01,0);
  }
  //sim
  TH1F *h_r_[numType][NUMCKIN][numPt]; 
  TH1F *H_r_[numType][numPt]; 
  TH2F *h_ZGs[NUMCKIN][numPt]; 
  TH2F *H_ZGs[numPt]; 
  TH2F *h_ZGs_low[NUMCKIN][numPt]; 
  TH2F *H_ZGs_low[numPt]; 
  //dat
  TH1F *H_re_[numType][numPt]; 
  //fill in each small h_ and then add onto H, draw in each tPad according to pT bin
  TCanvas *C_ZGs= new TCanvas("C_ZGs","ZGs", 1000, 1000);
  C_ZGs -> Divide(2,2,0.01,0.01,0);
  TCanvas *C_ZGs_low= new TCanvas("C_ZGs_low","ZGs_low", 1000, 1000);
  C_ZGs_low -> Divide(2,2,0.01,0.01,0);
  TCanvas *C_[numType];
  for(int iType = 0; iType < numType; iType++){
    C_[iType] = new TCanvas(Form("C_%s",types[iType].Data()),Form("%s",types[iType].Data()), 500, 500);
    C_[iType] -> Divide(1,1,0.01,0.01,0);
    //C_[iType] = new TCanvas(Form("C_%s",types[iType].Data()),Form("%s",types[iType].Data()), 1000, 1000);
    //C_[iType] -> Divide(2,2,0.01,0.01,0);
  }

  //declare the histos before loops
  for(int iPt = 0; iPt < numPt; iPt++)
  {
    H_ZGs[iPt]		= new TH2F(Form("H_ZGs_%i_%i",minPt[iPt],maxPt[iPt]), Form("ZGs %i=<jetPt<%i",minPt[iPt],maxPt[iPt]),32,0,0.64,32,0,0.64);
    H_ZGs[iPt]		->GetXaxis()->SetTitle("Reco Jet ZG");
    H_ZGs[iPt]		->GetYaxis()->SetTitle("True Jet ZG");
    H_ZGs_low[iPt]	= new TH2F(Form("H_ZGs_low_%i_%i",minPt[iPt],maxPt[iPt]), Form("ZGs_low %i=<jetPt<%i",minPt[iPt],maxPt[iPt]),16,0,0.32,16,0,0.32);
    H_ZGs_low[iPt]	->GetXaxis()->SetTitle("Reco Jet ZG");
    H_ZGs_low[iPt]	->GetYaxis()->SetTitle("True Jet ZG");
    for(int iType = 0; iType < numType; iType++)
    {
      //H_r_xx histos
      H_r_[iType][iPt] = new TH1F(Form("H_r_%s_%i_%i",types[iType].Data(),minPt[iPt],maxPt[iPt]),
      Form("%s with %i<=jetPt<%i",desciption[iType].Data(),minPt[iPt],maxPt[iPt]),nBins[iType],minX[iType],maxX[iType]);
      H_r_[iType][iPt]	->GetXaxis()->SetTitle(Form("%s",types[iType].Data()));
      //H_re_xx histos
      H_re_[iType][iPt] = new TH1F(Form("H_re_%s_%i_%i",types[iType].Data(),minPt[iPt],maxPt[iPt]),
      Form("%s with %i<=jetPt<%i",desciption[iType].Data(),minPt[iPt],maxPt[iPt]),nBins[iType],minX[iType],maxX[iType]);
      H_re_[iType][iPt]	->GetXaxis()->SetTitle(Form("%s",types[iType].Data()));
    }
    for(int ickin = 0; ickin < NUMCKIN; ickin++)
    {
      //h_r_xx histos
      h_ZGs[ickin][iPt]		= new TH2F(Form("h_ZGs_%i_%i_ckin%i",minPt[iPt],maxPt[iPt],ckins[ickin]), Form("ZGs %i=<jetPt<%i",minPt[iPt],maxPt[iPt]),32,0,0.64,32,0,0.64);
      for(int iType = 0; iType < numType; iType++)
      {
	//H_r_xx histos
	h_r_[iType][ickin][iPt] = new TH1F(Form("h_r_%s_%i_%i_ckin%i",types[iType].Data(),minPt[iPt],maxPt[iPt],ckins[ickin]),
	Form("%s with %i<=jetPt<%i",desciption[iType].Data(),minPt[iPt],maxPt[iPt]),nBins[iType],minX[iType],maxX[iType]);
      }
    }//NUMCKIN
  }//iPt
//~~~~~~~~~~~~~~~~~ TChain for trees
  //data
  TChain *rJets = new TChain("TReco");
  unsigned long long rEntries;
  //sim
  TChain * mJets[NUMCKIN];
  for(int i=0; i<NUMCKIN; i++){
    mJets[i] = new TChain("TMatched");
  }
  unsigned long long mEntries[NUMCKIN];
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets{
  //only need to add rJet once
  //Can still draw onto different ckin bins
    rJets  ->AddFile("/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13MB_data_ertFired.root");
    //rJets  ->AddFile("/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13MB_data.root");
    //rJets  ->AddFile("/sphenix/user/yzhai/JetSubStructure/TaxiTree/newTree_Run13_data.root");
    rEntries = rJets->GetEntries();

    cout<<endl; 
    cout << "-------------- data ----------------------" << endl;
    cout<<"rEntries = "<<rEntries<<endl;
    cout<<endl;

    float re_pT; 
    rJets->SetBranchAddress("pT",  &re_pT);
    float re_eta; 
    rJets->SetBranchAddress("eta", &re_eta);
    float re_phi; 
    rJets->SetBranchAddress("phi", &re_phi);
    int   re_nc; 
    rJets->SetBranchAddress("nc",  &re_nc);
    float re_cf; 
    rJets->SetBranchAddress("cf",  &re_cf);

    float re_oang;
    rJets->SetBranchAddress("oang",&re_oang);
    float re_z_g; 
    rJets->SetBranchAddress("z_g",&re_z_g);
    float re_pT1; 
    rJets->SetBranchAddress("pT1",  &re_pT1);
    float re_pT2; 
    rJets->SetBranchAddress("pT2",  &re_pT2);

    int   re_nc1; 
    rJets->SetBranchAddress("nc1",  &re_nc1);
    int   re_nc2; 
    rJets->SetBranchAddress("nc2",  &re_nc2);

    float re_cf1; 
    rJets->SetBranchAddress("cf1",  &re_cf1);
    float re_cf2; 
    rJets->SetBranchAddress("cf2",  &re_cf2);

    float re_eta1; 
    rJets->SetBranchAddress("eta1",  &re_eta1);
    float re_eta2; 
    rJets->SetBranchAddress("eta2",  &re_eta2);
    float re_phi1; 
    rJets->SetBranchAddress("phi1",  &re_phi1);
    float re_phi2; 
    rJets->SetBranchAddress("phi2",  &re_phi2);

    float Evnt_vertex; 
    rJets->SetBranchAddress("vertex", &Evnt_vertex);
	
    int numDatJet = 0;
    for(unsigned long long i=0; i<rEntries; i++){
      rJets->GetEntry(i); 
      //Basic cuts [0.3<cf<0.7 already applied]
      if(Evnt_vertex>30.0)continue;
      //if(re_z_g<0)continue; 
      
      //Check zg 0.1-0.2 what opening angle looks like
      //Opening angle
      float re_dR = sub_dR(re_eta1,re_eta2,re_phi1,re_phi2);

      if(re_nc < 3)continue;
      //if(re_cf1==0||re_cf1>0.95||re_cf2==0||re_cf2>0.95)continue;
      if(re_pT<10.0)continue;
      numDatJet++;

      for(int iPt = 0; iPt < numPt; iPt++)
      {
	if(re_pT>=minPt[iPt]&&re_pT<maxPt[iPt])
	{
	  DatCounter[iPt]++;
	  //fill H_re_xx histos
	  H_re_[0][iPt]		-> Fill(re_nc);
	  H_re_[1][iPt]		-> Fill(re_oang);
	  H_re_[2][iPt]		-> Fill(re_cf);
	  H_re_[3][iPt]		-> Fill(re_z_g);
	  H_re_[4][iPt]		-> Fill(re_nc1);
	  H_re_[5][iPt]		-> Fill(re_nc2);
	  H_re_[6][iPt]		-> Fill(re_cf1);
	  H_re_[7][iPt]		-> Fill(re_cf2);
	}
      }//iPt

    }//for entries
    cout<<numDatJet<<"data jets total"<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~rJets}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets{
  for(int n=0; n<NUMCKIN;n++)
  {  
    TString jet_fname =     
    Form("/sphenix/user/yzhai/JetSubStructure/GenSimTree/outTree/early/newTree_Run%i_ckin%i_hadrons.root", runno, ckins[n]);
    TFile fIn(jet_fname);
    if(fIn.IsZombie()) {fIn.Close();cout<<"Zombine file skipped"<<endl; continue;}
    //if(fIn.GetSize()<121000){fIn.Close(); cout<<"Small file skipped"<<endl;continue;}
    fIn.Close();
      
    mJets[n]  ->AddFile(jet_fname);
    if(n>9){
      mJets[n]->AddFile(Form("/sphenix/user/yzhai/JetSubStructure/GenSimTree/outTree/late/newTree_Run%i_ckin%i_hadrons.root", runno, ckins[n]));
    }
    mEntries[n] = mJets[n]->GetEntries();

    cout << Form("-------------- ckin%i ----------------------",ckins[n]) << endl;
    cout<<"mEntries = "<<mEntries[n]<<endl;
    cout<<endl; 
  
    float M_r_z_g; 
    mJets[n]->SetBranchAddress("r_z_g",&M_r_z_g);
    float M_t_z_g; 
    mJets[n]->SetBranchAddress("t_z_g",&M_t_z_g);
    float M_r_oang; 
    mJets[n]->SetBranchAddress("r_oang",&M_r_oang);
    float M_t_oang; 
    mJets[n]->SetBranchAddress("t_oang",&M_t_oang);

    float M_r_cf; 
    mJets[n]->SetBranchAddress("r_cf",  &M_r_cf);

    int   M_t_nc; 
    mJets[n]->SetBranchAddress("t_nc",  &M_t_nc);
    int   M_t_nc1; 
    mJets[n]->SetBranchAddress("t_nc1",  &M_t_nc1);
    int   M_t_nc2; 
    mJets[n]->SetBranchAddress("t_nc2",  &M_t_nc2);
    int   M_r_nc; 
    mJets[n]->SetBranchAddress("r_nc",  &M_r_nc);
    int   M_r_nc1; 
    mJets[n]->SetBranchAddress("r_nc1",  &M_r_nc1);
    int   M_r_nc2; 
    mJets[n]->SetBranchAddress("r_nc2",  &M_r_nc2);

    float M_r_cf1; 
    mJets[n]->SetBranchAddress("r_cf1",  &M_r_cf1);
    float M_r_cf2; 
    mJets[n]->SetBranchAddress("r_cf2",  &M_r_cf2);

    float M_r_pT; 
    mJets[n]->SetBranchAddress("r_pT",  &M_r_pT);
    float M_r_pT1; 
    mJets[n]->SetBranchAddress("r_pT1",  &M_r_pT1);
    float M_r_pT2; 
    mJets[n]->SetBranchAddress("r_pT2",  &M_r_pT2);
    float M_t_pT; 
    mJets[n]->SetBranchAddress("t_pT",  &M_t_pT);
    float M_t_pT1; 
    mJets[n]->SetBranchAddress("t_pT1",  &M_t_pT1);
    float M_t_pT2; 
    mJets[n]->SetBranchAddress("t_pT2",  &M_t_pT2);

    float M_r_eta; 
    mJets[n]->SetBranchAddress("r_eta", &M_r_eta);
    float M_r_phi; 
    mJets[n]->SetBranchAddress("r_phi", &M_r_phi);
    float M_t_eta; 
    mJets[n]->SetBranchAddress("t_eta", &M_t_eta);
    float M_t_phi; 
    mJets[n]->SetBranchAddress("t_phi", &M_t_phi);

    float M_t_eta1; 
    mJets[n]->SetBranchAddress("t_eta1",  &M_t_eta1);
    float M_t_eta2; 
    mJets[n]->SetBranchAddress("t_eta2",  &M_t_eta2);
    float M_t_phi1; 
    mJets[n]->SetBranchAddress("t_phi1",  &M_t_phi1);
    float M_t_phi2; 
    mJets[n]->SetBranchAddress("t_phi2",  &M_t_phi2);

    float M_r_eta1; 
    mJets[n]->SetBranchAddress("r_eta1",  &M_r_eta1);
    float M_r_eta2; 
    mJets[n]->SetBranchAddress("r_eta2",  &M_r_eta2);
    float M_r_phi1; 
    mJets[n]->SetBranchAddress("r_phi1",  &M_r_phi1);
    float M_r_phi2; 
    mJets[n]->SetBranchAddress("r_phi2",  &M_r_phi2);

    //Matching dR
    float M_dR; 
    mJets[n]->SetBranchAddress("dR", &M_dR);
    float Evt_vertex; 
    mJets[n]->SetBranchAddress("vertex", &Evt_vertex);

    for(unsigned long long i=0; i<mEntries[n]; i++){
      mJets[n]->GetEntry(i); 
      //Basic cuts [0.3<cf<0.7 already applied]
      if(Evt_vertex>30.0)continue;
      //if(M_dR>0.075)continue;
      //if(M_dR>0.075)cout<<"matching dR="<<M_dR;
      //if(M_t_z_g<0 |M_r_z_g<0)continue; 
      //Check zg 0.1-0.2 what opening angle looks like
      //Opening angles
      //Reco
      float r_dR = sub_dR(M_r_eta1,M_r_eta2,M_r_phi1,M_r_phi2);
      //Truth
      float t_dR = sub_dR(M_t_eta1,M_t_eta2,M_t_phi1,M_t_phi2);
      float rt_dR = (r_dR*1.1)/(t_dR*1.1);

      if(M_r_nc < 3)continue;
      //if(M_r_cf1==0||M_r_cf1>0.95||M_r_cf2==0||M_r_cf2>0.95)continue;

      for(int iPt = 0; iPt < numPt; iPt++)
      {
	//To avoid low stat spikes
	if(ckins[n]<=minPt[iPt])continue;
	if(M_r_pT>=minPt[iPt]&&M_r_pT<maxPt[iPt])
	{
	  SimCounter[iPt]++;
	  //fill h_re_xx histos
	  h_r_[0][n][iPt]	-> Fill(M_r_nc);
	  h_r_[1][n][iPt]	-> Fill(M_r_oang);
	  h_r_[2][n][iPt]	-> Fill(M_r_cf);
	  h_r_[3][n][iPt]	-> Fill(M_r_z_g);
	  h_r_[4][n][iPt]	-> Fill(M_r_nc1);
	  h_r_[5][n][iPt]	-> Fill(M_r_nc2);
	  h_r_[6][n][iPt]	-> Fill(M_r_cf1);
	  h_r_[7][n][iPt]	-> Fill(M_r_cf2);
	  if(M_r_z_g>=0.2&&M_t_z_g>=0.2)
	    h_ZGs[n][iPt]		-> Fill(M_r_z_g, M_t_z_g);
	}
      }//iPt

    }//for entries

    for(int iPt = 0; iPt <numPt; iPt++){
      c_ZGs[iPt]	-> cd(n+1);
      gStyle		-> SetOptStat(10);
      //gPad		-> SetLogz();
      //h_ZGs[n][iPt]	-> Draw("colz");
      //h_r_[1][n][iPt]	-> Draw();
      //TLegend *l_ZGs = new TLegend(0.6,0.8,0.9,0.9);
      //l_ZGs	-> AddEntry(h_ZGs[n][iPt],Form("ckin%i",ckins[n]),"l"); 
      //l_ZGs	-> Draw("same");
    }
    //adding histos at end of ckin
    float scale = 1.0*(mEntries[1]*crossSec[n])/(mEntries[n]*crossSec[1]);
    for(int iPt = 0; iPt < numPt; iPt++)
    {
      H_ZGs[iPt]	-> Add(h_ZGs[n][iPt],scale);
      for(int iType = 0; iType < numType; iType++)
      {
	H_r_[iType][iPt]-> Add(h_r_[iType][n][iPt],scale);
      }
    }//iPt
    //cout<<Form("SimCounter= %i, %i, %i",SimCounter[0],SimCounter[1],SimCounter[2])<<endl;
  }//end-for NUMCKIN
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~mJets}
  //for numPt draw in each pT bin
    for(int iPt = 0; iPt < numPt; iPt++)
    {
      for(int iType = 0; iType < numType; iType++)
      {
	C_[iType]		-> cd(iPt+1);
	gStyle			-> SetOptStat(0);
	normalize(H_re_[iType][iPt]);
	H_re_[iType][iPt]	-> SetLineStyle(2);
	H_re_[iType][iPt]	-> SetAxisRange(0.,maxY[iType],"Y");
	H_re_[iType][iPt]	-> DrawCopy();
	normalize(H_r_[iType][iPt]);
	H_r_[iType][iPt]	-> DrawCopy("same");
	TLegend *myLegend = new TLegend(0.6,0.8,0.9,0.9);
	myLegend	-> AddEntry(Form("H_re_%s_%i_%i",types[iType].Data(),minPt[iPt],maxPt[iPt]), Form("%i dat jets",DatCounter[iPt]),"l");
	myLegend	-> AddEntry(Form("H_r_%s_%i_%i",types[iType].Data(),minPt[iPt],maxPt[iPt]), Form("%i sim jets",SimCounter[iPt]),"l");
	myLegend	-> Draw("same");
      }
    }//iPt
  //Save the panels
  for(int iType = 0; iType < numType; iType++)
  {
    C_[iType]		-> SaveAs(Form("r_%s.png",types[iType].Data()));	
  }
  //C_ZGs			-> SaveAs("ZGs.png");
  /*
  for(int iPt=0; iPt<numPt; iPt++)
  {
    c_ZGs[iPt] -> SaveAs(Form("ZGs_%i_%i.png",minPt[iPt],maxPt[iPt]));
  }
  */

  return;

}//end readNewTree
