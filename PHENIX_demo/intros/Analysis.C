/**
    Purpose: Output a .root file to include histogram for the following quantities

|-----|-----------------|-----------------------|----------|------------|
|     | pseudo Rapidity	| transverse Momentum	| Rapidity | Event_t	|
|-----|-----------------|-----------------------|----------|------------|
| vm  | this		| this			| This	   |		|
|-----|-----------------|-----------------------|----------|		|
| vm1 | this		| this			|	   |	This	|
|-----|-----------------|-----------------------|----------|		|
| vm2 | this		| this			|	   |		|
|-----|-----------------|-----------------------|----------|------------|

with the cut of condition 

pseudoRapidity = (-1,1)&(1.4,4.0)

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
//#include "TPaveLabel.h"

#include <string.h>
#include "TChain.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "TError.h"

void Analysis(TString fname = "sartre_pAu_Au_emits.root", TString fname2 = "sartre_pAu_p_emits.root")
{
  
//============================== PART 1 ================================== 
//Deal with errors and exceptions

 // Suppress error messages from bad files
  gErrorIgnoreLevel = kFatal; 

  TFile *f = new TFile(fname.Data()); 
  TFile *f2 = new TFile(fname2.Data());
  if(!f) {cout << " input file missing!" << endl; exit; }
  if(f->IsZombie()) {f->Close(); cout << " input file corrupt!" << endl; exit;}
  if(!f2) {cout << " input file 2 missing!" << endl; exit; }
  if(f2->IsZombie()) {f2->Close(); cout << " input file 2 corrupt!" << endl; exit;}
 

//============================== PART 2 ================================== 
//Declare variables and objects



/*
A data structure is a group of data elements grouped together under one name. These data elements, known as members, can have different types and different lengths. Data structures can be declared in C++ using the following syntax:

struct type_name {
member_type1 member_name1;
member_type2 member_name2;
member_type3 member_name3;
.
.
} object_names;

OR type_name object_name1, object_name2;
http://www.cplusplus.com/doc/tutorial/structures/

*/
  // Open the tree, set pointers
  struct rootSartreEvent {	
    double t;
    double Q2;
    double x;
    double s;
    double y;
    double W;
    double xpom;
    int    iEvent;
    int    pol;      // 0=transverse or 1=longitudinal
    int    dmode;    // 0=coherent, 1=Incoherent
  };

  //declare rootSartreEvent object using the above constructor
  rootSartreEvent AuEvent, pEvent;

  //declare root objects, basically 4-vecs
  TLorentzVector *eIn = new TLorentzVector;
  TLorentzVector *pIn = new TLorentzVector;
  TLorentzVector *vm = new TLorentzVector;
  TLorentzVector *eOut = new TLorentzVector;
  TLorentzVector *pOut = new TLorentzVector;
  TLorentzVector *gamma = new TLorentzVector;
  TLorentzVector *vmDaughter1 = new TLorentzVector;
  TLorentzVector *vmDaughter2 = new TLorentzVector;
 
  //start with an empty tree       
  TTree *AuTree = NULL;
  TTree *pTree = NULL;
  f->GetObject("tree",AuTree); 
  f2->GetObject("tree",pTree);
  if(!AuTree) {f->Close(); cout << " tree missing!" << endl; exit;}
  if(!pTree) {f2->Close(); cout << " tree 2 missing!" << endl; exit;}


//============================== PART 3 ================================== 
// Assign values and contents

  AuTree->SetBranchAddress("event", &AuEvent);
  AuTree->SetBranchAddress("eIn", &eIn);
  AuTree->SetBranchAddress("pIn", &pIn);
  AuTree->SetBranchAddress("vm",  &vm);// VectorMeson
  AuTree->SetBranchAddress("eOut", &eOut);
  AuTree->SetBranchAddress("pOut", &pOut);
  AuTree->SetBranchAddress("gamma", &gamma);
  AuTree->SetBranchAddress("vmDaughter1", &vmDaughter1);// vm
  AuTree->SetBranchAddress("vmDaughter2", &vmDaughter2);// vm

  pTree->SetBranchAddress("event", &pEvent);
  pTree->SetBranchAddress("eIn", &eIn);
  pTree->SetBranchAddress("pIn", &pIn);
  pTree->SetBranchAddress("vm",  &vm);// VectorMeson
  pTree->SetBranchAddress("eOut", &eOut);
  pTree->SetBranchAddress("pOut", &pOut);
  pTree->SetBranchAddress("gamma", &gamma);
  pTree->SetBranchAddress("vmDaughter1", &vmDaughter1);// vm
  pTree->SetBranchAddress("vmDaughter2", &vmDaughter2);// vm

//============================== PART 4 ================================== 
//Creating histograms

  //Output the results into a new file
  //TFile *g = new TFile(Form("%s_plots.root",fname.Data()),"RECREATE");
  TFile *g = new TFile("sartre_pAu_plots.root","RECREATE");
  
  // Declare Histograms
  //TH1D("name of the histogram", "title of the histogram", "number of bins", "lower edge of the first bin", "upper edge of the last, not included, bin" )
  //int bin_size = 150;

  TH1D *Au_event_t = new TH1D("Au_event_t","",100,0.0,0.1); 
  TH1D *p_event_t = new TH1D("p_event_t","",100,0.0,0.1); 

  TH1D *Au_Rapidity = new TH1D("Au_Rapidity","",120,-2.0,4.0); 
  TH1D *p_Rapidity = new TH1D("p_Rapidity","",120,-2.0,4.0); 

  TH1D *Au_psR = new TH1D("Au_psR","",160,-8.0,8.0);
  TH1D *p_psR = new TH1D("p_psR","",160,-8.0,8.0);
  TH1D *Au_vm1_psR = new TH1D("Au_vm1_psR","",160,-8.0,8.0);
  TH1D *p_vm1_psR = new TH1D("p_vm1_psR","",160,-8.0,8.0);
  TH1D *Au_vm2_psR = new TH1D("Au_vm2_psR","",160,-8.0,8.0);
  TH1D *p_vm2_psR = new TH1D("p_vm2_psR","",160,-8.0,8.0);

  TH1D *Au_pT = new TH1D("Au_pT","",120,-0.5,2.5);
  TH1D *p_pT = new TH1D("p_pT","",120,-0.5,2.5);
  TH1D *Au_vm1_pT = new TH1D("Au_vm1_pT","",120,-0.5,2.5);
  TH1D *p_vm1_pT = new TH1D("p_vm1_pT","",120,-0.5,2.5);
  TH1D *Au_vm2_pT = new TH1D("Au_vm2_pT","",120,-0.5,2.5);
  TH1D *p_vm2_pT = new TH1D("p_vm2_pT","",120,-0.5,2.5);


/*
1) Both decay electrons are in the forward acceptance  1.4 < eta < 4.0
2) Both decay electrons are in the central barrel acceptance -1.0 < eta < 1.0
3) One electron in the central barrel, one in the forward acceptance. 
*/

  TH1D *Au_event_t_1 = new TH1D("Au_event_t_1","",100,0.0,0.1); 
  TH1D *p_event_t_1 = new TH1D("p_event_t_1","",100,0.0,0.1); 
  TH1D *Au_event_t_2 = new TH1D("Au_event_t_2","",100,0.0,0.1); 
  TH1D *p_event_t_2 = new TH1D("p_event_t_2","",100,0.0,0.1); 
  TH1D *Au_event_t_3 = new TH1D("Au_event_t_3","",100,0.0,0.1); 
  TH1D *p_event_t_3 = new TH1D("p_event_t_3","",100,0.0,0.1); 

  // Loop over tree entries, this is already dealing with the tree, not the file
  unsigned long long  nsim = AuTree->GetEntries(); //get num of entries
  unsigned long long  nsim2 = pTree->GetEntries(); 
  //long long just means it can be really big number

	//pseudoRapidity
	double psR_vm1=0.0;
	double psR_vm2=0.0;
	//double psR=0.0;
	bool vm1_cut, vm2_cut;
	
	//boolean for two cuts on two variables
	bool bool_1a, bool_2a, bool_1b, bool_2b;
	double event_t_abs;
	
   
//============================== PART 5 ================================== 
//Assigning values into histograms and writing them to files

//--------This is for t-events only

  for(unsigned long long isim=0; isim< nsim; isim++){
	AuTree->GetEntry(isim);//isim is local iterator

	psR_vm1 = vmDaughter1 -> PseudoRapidity();
	psR_vm2 = vmDaughter2 -> PseudoRapidity();

	event_t_abs = fabs(AuEvent.t);

	bool_1a = (psR_vm1 > 1.4 && psR_vm1 < 4);
	bool_2a = (psR_vm2 > 1.4 && psR_vm2 < 4);
	bool_1b = (psR_vm1 > -1 && psR_vm1 < 1);
	bool_2b = (psR_vm2 > -1 && psR_vm2 < 1);

	if( bool_1a ){
		if( bool_2a )
		Au_event_t_1->Fill(event_t_abs); 
		else if (bool_2b)
		Au_event_t_3->Fill(event_t_abs);
	}

	else if( bool_1b ){
		if( bool_2b )
		Au_event_t_2->Fill(event_t_abs); 
		else if (bool_1a)
		Au_event_t_3->Fill(event_t_abs);
	}
  } //end of loop

	Au_event_t_1 -> Write();
	Au_event_t_2 -> Write();
	Au_event_t_3 -> Write();

  for(unsigned long long isim=0; isim< nsim2; isim++){
	pTree->GetEntry(isim);//isim is local iterator

	psR_vm1 = vmDaughter1 -> PseudoRapidity();
	psR_vm2 = vmDaughter2 -> PseudoRapidity();

	event_t_abs = fabs(pEvent.t);

	bool_1a = (psR_vm1 > 1.4 && psR_vm1 < 4);
	bool_2a = (psR_vm2 > 1.4 && psR_vm2 < 4);
	bool_1b = (psR_vm1 > -1 && psR_vm1 < 1);
	bool_2b = (psR_vm2 > -1 && psR_vm2 < 1);

	if( bool_1a ){
		if( bool_2a )
		p_event_t_1->Fill(event_t_abs); 
		else if (bool_2b)
		p_event_t_3->Fill(event_t_abs);
	}

	else if( bool_1b ){
		if( bool_2b )
		p_event_t_2->Fill(event_t_abs); 
		else if (bool_1a)
		p_event_t_3->Fill(event_t_abs);
	}
  } //end of loop

	p_event_t_1 -> Write();
	p_event_t_2 -> Write();
	p_event_t_3 -> Write();

//note 2017.6.17: Commented out the below but Plots.C will plot the old files.
/*

  //basically to print in terminal to ask you to wait
  cout << "Creating histograms from " << nsim << " entries from <sartre_pAu_Au_emits.root>" << endl; 

  for(unsigned long long isim=0; isim< nsim; isim++){
	AuTree->GetEntry(isim);//isim is local iterator
	//psR =  vm -> PseudoRapidity();

	psR_vm1 = vmDaughter1 -> PseudoRapidity();
	psR_vm2 = vmDaughter2 -> PseudoRapidity();

	vm1_cut = !((psR_vm1 > -1.000 && psR_vm1 < 1.000 )|| (psR_vm1 > 1.400 && psR_vm1 <= 4.0 ));
	vm2_cut = !((psR_vm2 > -1.000 && psR_vm2 < 1.000)|| (psR_vm2 > 1.400 && psR_vm2 <= 4.0 ));

	// if (false) continue to skip the rest of the for-loop to the next isim
	if( vm1_cut || vm2_cut ) continue;
	
		//cout << psR_vm1 << " and " << psR_vm2 << endl; 

	Au_psR -> Fill(vm -> PseudoRapidity());
	Au_event_t->Fill(fabs(AuEvent.t)); 
			//fabs() Function, absolute value, from C++
			//AuEvent is declared in PART 1
			//t is the first variable
	Au_Rapidity -> Fill(vm -> Rapidity());
	Au_vm1_psR -> Fill(psR_vm1);
	Au_vm2_psR -> Fill(psR_vm2);
	Au_pT -> Fill(vm -> Pt());
	Au_vm1_pT -> Fill(vmDaughter1 -> Pt());
	Au_vm2_pT -> Fill(vmDaughter2 -> Pt());
  } 

  cout << "Writing histograms from <sartre_pAu_Au_emits.root>" << endl; 
  
	Au_psR -> Write();
	Au_vm1_psR -> Write();
	Au_vm2_psR -> Write();
	Au_pT -> Write();
	Au_vm1_pT -> Write();
	Au_vm2_pT -> Write();

	Au_event_t -> Write();
	Au_Rapidity -> Write();

  cout << "Creating histograms from " << nsim2 << " entries from <sartre_pAu_p_emits.root>" << endl;


  for(unsigned long long isim=0; isim<nsim2; isim++){
	pTree->GetEntry(isim);

	psR_vm1 = vmDaughter1 -> PseudoRapidity();
	psR_vm2 = vmDaughter2 -> PseudoRapidity();


	vm1_cut = !((psR_vm1 > -1.000 && psR_vm1 < 1.000 )|| (psR_vm1 > 1.400 && psR_vm1 <= 4.0 ));
	vm2_cut = !((psR_vm2 > -1.000 && psR_vm2 < 1.000 )|| (psR_vm2 > 1.400 && psR_vm2 <= 4.0 ));

	//vm1_cut = (psR_vm1 < -1.0 || (psR_vm1 > 1.0 && psR_vm1 < 1.4 )|| psR_vm1 > 4.0);
	//vm2_cut = (psR_vm2 < -1.0 || (psR_vm2 > 1.0 && psR_vm2 < 1.4 )|| psR_vm2 > 4.0);

	if( vm1_cut || vm2_cut ) continue;

	p_psR -> Fill(vm -> PseudoRapidity());
	p_event_t->Fill(fabs(pEvent.t)); 
	p_Rapidity -> Fill(vm -> Rapidity());
	p_psR -> Fill(vm -> PseudoRapidity());
	p_vm1_psR -> Fill(psR_vm1);
	p_vm2_psR -> Fill(psR_vm2);
	p_pT -> Fill(vm -> Pt());
	p_vm1_pT -> Fill(vmDaughter1 -> Pt());
	p_vm2_pT -> Fill(vmDaughter2 -> Pt());
  }

  cout << "Writing histograms from <sartre_pAu_p_emits.root>" << endl; 

	p_psR -> Write();
	p_vm1_psR -> Write();
	p_vm2_psR -> Write();
	p_pT -> Write();
	p_vm1_pT -> Write();
	p_vm2_pT -> Write();

	p_event_t -> Write();
	p_Rapidity -> Write();

*/

//============================== PART 6 ================================== 
//Closing the files written on and read from

  //g->Write();
  //Write the above onto the file g
  //but we don't need this anymore because we wrote the histograms individually 
  g->Close();//Close that file
  f->Close();//Also close the file that just read in
  //g1.cd(); //to get back to writing on g1, if declared earlier
  f2->Close();

}//end of Analysis.C
