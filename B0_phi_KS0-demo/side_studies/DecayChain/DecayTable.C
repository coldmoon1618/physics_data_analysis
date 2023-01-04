#include <iostream>
#include <fstream>

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

#include <TDatabasePDG.h>

void DecayTable() {
    auto *pdg_obj = TDatabasePDG::Instance();
    /*
    EvtGenDatabasePDG::Instance()->GetParticle(…)
    In this case EvtGenDatabasePDG::Instance() which you can call without an object.
    Or: EvtGenDatabasePDG *pdg = EvtGenDatabasePDG::Instance();
    As design pattern that is called a singleton, it's an object that is shared throughout the program, 
    and that you yourself (typically) don't have to worry about setting up.  
    Conceptually it is similar to global variables, but can be somewhat smarter about initialization, cleanup, etc.
    */

    TChain *MCs = new TChain("mcTree");
    MCs -> AddFile("SigMC_decayChain.root");
    //MCs -> AddFile("MC13a_decayChain.root");

    unsigned long long KEntries;
    KEntries = MCs -> GetEntries();

    int nParticles;
    MCs -> SetBranchAddress("nParticles", &nParticles);
    const int kMaxParticles = 200;

    int     dec_step  [kMaxParticles];
    int     pdg       [kMaxParticles];
    float   energy    [kMaxParticles];
    float   px        [kMaxParticles];
    float   py        [kMaxParticles];
    float   pz        [kMaxParticles];
    float   pt        [kMaxParticles];
    float   lifetime  [kMaxParticles];
    float   t_prod    [kMaxParticles];
    float   v_prod_x  [kMaxParticles];
    float   v_prod_y  [kMaxParticles];
    float   v_prod_z  [kMaxParticles];
    float   t_dec     [kMaxParticles];
    float   v_dec_x   [kMaxParticles];
    float   v_dec_y   [kMaxParticles];
    float   v_dec_z   [kMaxParticles];
    int     mother    [kMaxParticles];
    int     first_dau [kMaxParticles];
    int     last_dau  [kMaxParticles];
    int     secPhysProc [kMaxParticles];
    MCs -> SetBranchAddress("dec_step", dec_step);
    MCs -> SetBranchAddress("pdg", pdg);
    MCs -> SetBranchAddress("energy", energy);
    MCs -> SetBranchAddress("px", px);
    MCs -> SetBranchAddress("py", py);
    MCs -> SetBranchAddress("pz", pz);
    MCs -> SetBranchAddress("pt", pt);
    MCs -> SetBranchAddress("lifetime", lifetime);
    MCs -> SetBranchAddress("t_prod", t_prod);
    MCs -> SetBranchAddress("v_prod_x", v_prod_x);
    MCs -> SetBranchAddress("v_prod_y", v_prod_y);
    MCs -> SetBranchAddress("v_prod_z", v_prod_z);
    MCs -> SetBranchAddress("t_dec", t_dec);
    MCs -> SetBranchAddress("v_dec_x", v_dec_x);
    MCs -> SetBranchAddress("v_dec_y", v_dec_y);
    MCs -> SetBranchAddress("v_dec_z", v_dec_z);
    MCs -> SetBranchAddress("mother", mother);
    MCs -> SetBranchAddress("first_dau", first_dau);
    MCs -> SetBranchAddress("last_dau", last_dau);
    MCs -> SetBranchAddress("secPhysProc", secPhysProc);

    string str;
    int i = 66;
    cout<<"event"<<i+1<<endl;
    {
        //printf("Event number: %7i\n", evtNum);
        MCs -> GetEntry(i);
        printf("%4s%8s%12s", "mdstIndex", "pdg", "name"); 
        printf("%8s%8s%10s", "mother", "1st_dau", "last_dau");
        printf("%18s%8s\n", "energy","  secPhysProc");
        for(int iEnt = 0; iEnt < nParticles; iEnt++){
            if(pdg[iEnt]==300553){
                str = "Upsilon(4S)";//Somehow this is not included, so have to manually include
            }else{
                str = pdg_obj -> GetParticle(pdg[iEnt])-> GetName();
            }
            printf("%4i%8i%12s", dec_step[iEnt], pdg[iEnt], str.c_str()); 
            printf("%8i%8i%8i",  mother[iEnt], first_dau[iEnt], last_dau[iEnt]);
            printf("\t%4f\t\t%i\n", energy[iEnt],secPhysProc[iEnt]);
        }//event level
    }
    return ;
}
