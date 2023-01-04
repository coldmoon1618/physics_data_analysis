/**
	Purpose: Make plots out of the histograms from sartre_pAu_plots.root
	
	@author Dr. John Lajoie, Yunxiao Zhai
	@version 1.1 June 2017 
*/

#include <iostream>
#include <sstream>
#include <string> 
#include "TROOT.h"
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TRandom3.h"
#include "TVector.h"
#include "TRotation.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TVector3.h"
#include "TLorentzVector.h"

//-----
#include "TStyle.h"
#include "TPad.h"
#include "TPaveLabel.h"

#include <string.h>
#include "TChain.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "TError.h"

void Plots(){
//============================== PART 1 ================================== 
//Deal with errors and exceptions

	// Suppress error messages from bad files
	gErrorIgnoreLevel = kFatal; 

	TFile *f = new TFile("sartre_pAu_plots.root"); 
	if(!f) {cout << " input file missing!" << endl; exit; }
	if(f->IsZombie()) {f->Close(); cout << " input file corrupt!" << endl; exit;}


//============================== PART 2 ================================== 
// Assign values and contents

	//histograms
/*
 	TH1D *Au_psR = (TH1D*)f -> Get("Au_psR");
 	TH1D *p_psR = (TH1D*)f -> Get("p_psR");
 	TH1D *Au_vm1_psR = (TH1D*)f -> Get("Au_vm1_psR");
 	TH1D *p_vm1_psR = (TH1D*)f -> Get("p_vm1_psR");
 	TH1D *Au_vm2_psR = (TH1D*)f -> Get("Au_vm2_psR");
 	TH1D *p_vm2_psR = (TH1D*)f -> Get("p_vm2_psR");
 	TH1D *Au_pT = (TH1D*)f -> Get("Au_pT");
 	TH1D *p_pT = (TH1D*)f -> Get("p_pT");
 	TH1D *Au_vm1_pT = (TH1D*)f -> Get("Au_vm1_pT");
 	TH1D *p_vm1_pT = (TH1D*)f -> Get("p_vm1_pT");
 	TH1D *Au_vm2_pT = (TH1D*)f -> Get("Au_vm2_pT");
 	TH1D *p_vm2_pT = (TH1D*)f -> Get("p_vm2_pT");

 	TH1D *Au_event_t = (TH1D*)f -> Get("Au_event_t");
 	TH1D *p_event_t = (TH1D*)f -> Get("p_event_t");

 	TH1D *Au_Rapidity = (TH1D*)f -> Get("Au_Rapidity");
 	TH1D *p_Rapidity = (TH1D*)f -> Get("p_Rapidity");
*/

 	TH1D *Au_event_t_1 = (TH1D*)f -> Get("Au_event_t_1");
 	TH1D *p_event_t_1 = (TH1D*)f -> Get("p_event_t_1");
 	TH1D *Au_event_t_2 = (TH1D*)f -> Get("Au_event_t_2");
 	TH1D *p_event_t_2 = (TH1D*)f -> Get("p_event_t_2");
 	TH1D *Au_event_t_3 = (TH1D*)f -> Get("Au_event_t_3");
 	TH1D *p_event_t_3 = (TH1D*)f -> Get("p_event_t_3");

//============================== PART 3 ================================== 
//Plot all graphs

	
  	gStyle -> SetOptStat(0);

/*
	TCanvas *c1 = 
		new TCanvas("c1","Pseudo Rapidity of J/Psi Particle",400,500);
	p_psR -> SetTitle("Pseudo Rapidity of J/Psi particle");
	p_psR -> SetLineColor(12);
	p_psR -> DrawCopy();
	Au_psR -> SetLineColor(42);
	Au_psR -> DrawCopy("SAME");
	//c1 -> BuildLegend();//200.0,300.0,200.0,300.0,"");
	c1->SaveAs("pAu_psR_JPsi.png","RECREATE");

	TCanvas *c2 = 
		new TCanvas("c2","Pseudo Rapidity of vm1",400,500);
	Au_vm1_psR -> SetTitle("Pseudo Rapidity of vm1");
	Au_vm1_psR -> SetLineColor(42);
	Au_vm1_psR -> DrawCopy();
	p_vm1_psR -> SetLineColor(12);
	p_vm1_psR -> DrawCopy("SAME");
	c2->SaveAs("pAu_psR_vm1.png","RECREATE");

	TCanvas *c3 = 
		new TCanvas("c3","Pseudo Rapidity of vm2",400,500);
	Au_vm2_psR -> SetTitle("Pseudo Rapidity of vm2");
	Au_vm2_psR -> SetLineColor(42);
	Au_vm2_psR -> DrawCopy();
	p_vm2_psR -> SetLineColor(12);
	p_vm2_psR -> DrawCopy("SAME");
	c3->SaveAs("pAu_psR_vm2.png","RECREATE");

	TCanvas *c4 = 
		new TCanvas("c4","Transverse Momentum of J/Psi Particle",400,500);
	p_pT -> SetTitle("Transverse Momentum of J/Psi particle");
	p_pT -> SetLineColor(12);
	p_pT -> DrawCopy();
	Au_pT -> SetLineColor(42);
	Au_pT -> DrawCopy("SAME");
	c4->SaveAs("pAu_pT_JPsi.png","RECREATE");

	TCanvas *c5 = 
		new TCanvas("c5","Transverse Momentum of vm1",400,500);
	p_vm1_pT -> SetTitle("Transverse Momentum of vm1");
	p_vm1_pT -> SetLineColor(12);
	p_vm1_pT -> DrawCopy();
	Au_vm1_pT -> SetLineColor(42);
	Au_vm1_pT -> DrawCopy("SAME");
	c5->SaveAs("pAu_pT_vm1.png","RECREATE");

	TCanvas *c6 = 
		new TCanvas("c6","Transverse Momentum of vm2",400,500);
	p_vm2_pT -> SetTitle("Transverse Momentum of vm2");
	p_vm2_pT -> SetLineColor(12);
	p_vm2_pT -> DrawCopy();
	Au_vm2_pT -> SetLineColor(42);
	Au_vm2_pT -> DrawCopy("SAME");
	c6->SaveAs("pAu_pT_vm2.png","RECREATE");

	TCanvas *c7 = 
		new TCanvas("c7","Event t",400,500);
	p_event_t -> SetTitle("Event t");
	p_event_t -> SetLineColor(12);
	p_event_t -> DrawCopy();
	Au_event_t -> SetLineColor(42);
	Au_event_t -> DrawCopy("SAME");
	c7->SaveAs("pAu_Event_t.png","RECREATE");

	TCanvas *c8 = 
		new TCanvas("c8","Rapidity of J/Psi particle",400,500);
	Au_Rapidity -> SetTitle("Rapidity of J/Psi particle");
	Au_Rapidity -> SetLineColor(42);
	Au_Rapidity -> DrawCopy();
	p_Rapidity -> SetLineColor(12);
	p_Rapidity -> DrawCopy("SAME");
	c8->SaveAs("pAu_Rapidity_JPsi.png","RECREATE");
*/

	TCanvas *c_t1 = 
		new TCanvas("c_t1","Event t case 1",400,500);
	p_event_t_1 -> SetTitle("Event t case 1");
	p_event_t_1 -> SetLineColor(12);
	p_event_t_1 -> DrawCopy();
	Au_event_t_1 -> SetLineColor(42);
	Au_event_t_1 -> DrawCopy("SAME");
	c_t1->SaveAs("pAu_Event_t_case1.png","RECREATE");

	TCanvas *c_t2 = 
		new TCanvas("c_t2","Event t case 2",400,500);
	p_event_t_2 -> SetTitle("Event t case 2");
	p_event_t_2 -> SetLineColor(12);
	p_event_t_2 -> DrawCopy();
	Au_event_t_2 -> SetLineColor(42);
	Au_event_t_2 -> DrawCopy("SAME");
	c_t2->SaveAs("pAu_Event_t_case1.png","RECREATE");

	TCanvas *c_t3 = 
		new TCanvas("c_t3","Event t case 3",400,500);
	p_event_t_3 -> SetTitle("Event t case 3");
	p_event_t_3 -> SetLineColor(12);
	p_event_t_3 -> DrawCopy();
	Au_event_t_3 -> SetLineColor(42);
	Au_event_t_3 -> DrawCopy("SAME");
	c_t3->SaveAs("pAu_Event_t_case1.png","RECREATE");


	f -> Close();

}//end of Plots.C
