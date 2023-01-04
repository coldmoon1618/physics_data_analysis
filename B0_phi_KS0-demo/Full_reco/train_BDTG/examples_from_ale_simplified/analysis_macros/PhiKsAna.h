//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 11 09:00:50 2020 by ROOT version 6.18/04
// from TTree B0_ch1/
// found on file: data/PhiKs_analysis/PhiKs_signal_BGx0.root
//////////////////////////////////////////////////////////

#ifndef PhiKsAna_h
#define PhiKsAna_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class PhiKsAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMax__experiment_ = 1;
   static constexpr Int_t kMax__run_ = 1;
   static constexpr Int_t kMax__event_ = 1;
   static constexpr Int_t kMax__candidate_ = 1;
   static constexpr Int_t kMax__ncandidates_ = 1;
   static constexpr Int_t kMax__weight_ = 1;

   // Declaration of leaf types
   Int_t           __experiment__;
   Int_t           __run__;
   Int_t           __event__;
   Int_t           __candidate__;
   Int_t           __ncandidates__;
   Double_t        __weight__;
   Double_t        M;
   Double_t        ErrM;
   Double_t        SigM;
   Double_t        InvM;
   Double_t        distance;
   Double_t        significanceOfDistance;
   Double_t        dx;
   Double_t        dy;
   Double_t        dz;
   Double_t        x;
   Double_t        y;
   Double_t        z;
   Double_t        x_uncertainty;
   Double_t        y_uncertainty;
   Double_t        z_uncertainty;
   Double_t        dr;
   Double_t        dphi;
   Double_t        dcosTheta;
   Double_t        prodVertexX;
   Double_t        prodVertexY;
   Double_t        prodVertexZ;
   Double_t        prodVertexXErr;
   Double_t        prodVertexYErr;
   Double_t        prodVertexZErr;
   Double_t        chiProb;
   Double_t        mcX;
   Double_t        mcY;
   Double_t        mcZ;
   Double_t        mcDistance;
   Double_t        mcRho;
   Double_t        mcProdVertexX;
   Double_t        mcProdVertexY;
   Double_t        mcProdVertexZ;
   Double_t        px;
   Double_t        py;
   Double_t        pz;
   Double_t        pt;
   Double_t        p;
   Double_t        E;
   Double_t        isSignal;
   Double_t        mcErrors;
   Double_t        mcPDG;
   Double_t        mothID;
   Double_t        gdMothID;
   Double_t        gdgdMothID;
   Double_t        mothPDG;
   Double_t        gdMothPDG;
   Double_t        gdgdMothPDG;
   Double_t        Mbc;
   Double_t        deltaE;
   Double_t        foxWolframR1;
   Double_t        foxWolframR2;
   Double_t        foxWolframR3;
   Double_t        foxWolframR4;
   Double_t        harmonicMomentThrust0;
   Double_t        harmonicMomentThrust1;
   Double_t        harmonicMomentThrust2;
   Double_t        harmonicMomentThrust3;
   Double_t        harmonicMomentThrust4;
   Double_t        cleoConeThrust0;
   Double_t        cleoConeThrust1;
   Double_t        cleoConeThrust2;
   Double_t        cleoConeThrust3;
   Double_t        cleoConeThrust4;
   Double_t        cleoConeThrust5;
   Double_t        cleoConeThrust6;
   Double_t        cleoConeThrust7;
   Double_t        cleoConeThrust8;
   Double_t        sphericity;
   Double_t        aplanarity;
   Double_t        thrust;
   Double_t        thrustAxisCosTheta;
   Double_t        cosTBTO;
   Double_t        cosTBz;
   Double_t        thrustOm;
   Double_t        thrustBm;
   Double_t        KSFWVariables__boet__bc;
   Double_t        KSFWVariables__bomm2__bc;
   Double_t        KSFWVariables__bohso00__bc;
   Double_t        KSFWVariables__bohso04__bc;
   Double_t        KSFWVariables__bohso10__bc;
   Double_t        KSFWVariables__bohso14__bc;
   Double_t        KSFWVariables__bohso20__bc;
   Double_t        KSFWVariables__bohso22__bc;
   Double_t        KSFWVariables__bohso24__bc;
   Double_t        KSFWVariables__bohoo0__bc;
   Double_t        KSFWVariables__bohoo1__bc;
   Double_t        KSFWVariables__bohoo2__bc;
   Double_t        KSFWVariables__bohoo3__bc;
   Double_t        KSFWVariables__bohoo4__bc;
   Double_t        DeltaT;
   Double_t        DeltaTErr;
   Double_t        DeltaZ;
   Double_t        DeltaZErr;
   Double_t        DeltaBoost;
   Double_t        DeltaBoostErr;
   Double_t        TagVLBoost;
   Double_t        TagVLBoostErr;
   Double_t        TagVOBoost;
   Double_t        TagVOBoostErr;
   Double_t        TagVpVal;
   Double_t        TagVNDF;
   Double_t        TagVChi2;
   Double_t        TagVChi2IP;
   Double_t        TagVx;
   Double_t        TagVxErr;
   Double_t        TagVy;
   Double_t        TagVyErr;
   Double_t        TagVz;
   Double_t        TagVzErr;
   Double_t        MCDeltaT;
   Double_t        TagVmcLBoost;
   Double_t        TagVmcOBoost;
   Double_t        mcLBoost;
   Double_t        mcOBoost;
   Double_t        mcTagVx;
   Double_t        mcTagVy;
   Double_t        mcTagVz;
   Double_t        isNotContinuumEvent;
   Double_t        phiCosHel;
   Double_t        out_FastBDT;
   Double_t        csv_FastBDT;
   Double_t        out_FastBDT1;
   Double_t        csv_FastBDT1;
   Double_t        out_FastBDT2;
   Double_t        csv_FastBDT2;
   Double_t        out_FastBDT3;
   Double_t        csv_FastBDT3;
   Double_t        FBDT_flTag_qr;
   Double_t        FANN_flTag_qr;
   Double_t        B0_phi_M;
   Double_t        B0_phi_ErrM;
   Double_t        B0_phi_SigM;
   Double_t        B0_phi_InvM;
   Double_t        B0_phi_distance;
   Double_t        B0_phi_significanceOfDistance;
   Double_t        B0_phi_dx;
   Double_t        B0_phi_dy;
   Double_t        B0_phi_dz;
   Double_t        B0_phi_x;
   Double_t        B0_phi_y;
   Double_t        B0_phi_z;
   Double_t        B0_phi_x_uncertainty;
   Double_t        B0_phi_y_uncertainty;
   Double_t        B0_phi_z_uncertainty;
   Double_t        B0_phi_dr;
   Double_t        B0_phi_dphi;
   Double_t        B0_phi_dcosTheta;
   Double_t        B0_phi_prodVertexX;
   Double_t        B0_phi_prodVertexY;
   Double_t        B0_phi_prodVertexZ;
   Double_t        B0_phi_prodVertexXErr;
   Double_t        B0_phi_prodVertexYErr;
   Double_t        B0_phi_prodVertexZErr;
   Double_t        B0_phi_chiProb;
   Double_t        B0_phi_mcX;
   Double_t        B0_phi_mcY;
   Double_t        B0_phi_mcZ;
   Double_t        B0_phi_mcDistance;
   Double_t        B0_phi_mcRho;
   Double_t        B0_phi_mcProdVertexX;
   Double_t        B0_phi_mcProdVertexY;
   Double_t        B0_phi_mcProdVertexZ;
   Double_t        B0_phi_px;
   Double_t        B0_phi_py;
   Double_t        B0_phi_pz;
   Double_t        B0_phi_pt;
   Double_t        B0_phi_p;
   Double_t        B0_phi_E;
   Double_t        B0_phi_isSignal;
   Double_t        B0_phi_mcErrors;
   Double_t        B0_phi_mcPDG;
   Double_t        B0_phi_mothID;
   Double_t        B0_phi_gdMothID;
   Double_t        B0_phi_gdgdMothID;
   Double_t        B0_phi_mothPDG;
   Double_t        B0_phi_gdMothPDG;
   Double_t        B0_phi_gdgdMothPDG;
   Double_t        B0_Ks_M;
   Double_t        B0_Ks_ErrM;
   Double_t        B0_Ks_SigM;
   Double_t        B0_Ks_InvM;
   Double_t        B0_Ks_distance;
   Double_t        B0_Ks_significanceOfDistance;
   Double_t        B0_Ks_dx;
   Double_t        B0_Ks_dy;
   Double_t        B0_Ks_dz;
   Double_t        B0_Ks_x;
   Double_t        B0_Ks_y;
   Double_t        B0_Ks_z;
   Double_t        B0_Ks_x_uncertainty;
   Double_t        B0_Ks_y_uncertainty;
   Double_t        B0_Ks_z_uncertainty;
   Double_t        B0_Ks_dr;
   Double_t        B0_Ks_dphi;
   Double_t        B0_Ks_dcosTheta;
   Double_t        B0_Ks_prodVertexX;
   Double_t        B0_Ks_prodVertexY;
   Double_t        B0_Ks_prodVertexZ;
   Double_t        B0_Ks_prodVertexXErr;
   Double_t        B0_Ks_prodVertexYErr;
   Double_t        B0_Ks_prodVertexZErr;
   Double_t        B0_Ks_chiProb;
   Double_t        B0_Ks_mcX;
   Double_t        B0_Ks_mcY;
   Double_t        B0_Ks_mcZ;
   Double_t        B0_Ks_mcDistance;
   Double_t        B0_Ks_mcRho;
   Double_t        B0_Ks_mcProdVertexX;
   Double_t        B0_Ks_mcProdVertexY;
   Double_t        B0_Ks_mcProdVertexZ;
   Double_t        B0_Ks_px;
   Double_t        B0_Ks_py;
   Double_t        B0_Ks_pz;
   Double_t        B0_Ks_pt;
   Double_t        B0_Ks_p;
   Double_t        B0_Ks_E;
   Double_t        B0_Ks_isSignal;
   Double_t        B0_Ks_mcErrors;
   Double_t        B0_Ks_mcPDG;
   Double_t        B0_Ks_mothID;
   Double_t        B0_Ks_gdMothID;
   Double_t        B0_Ks_gdgdMothID;
   Double_t        B0_Ks_mothPDG;
   Double_t        B0_Ks_gdMothPDG;
   Double_t        B0_Ks_gdgdMothPDG;
   Double_t        B0_phi_K1_charge;
   Double_t        B0_phi_K1_px;
   Double_t        B0_phi_K1_py;
   Double_t        B0_phi_K1_pz;
   Double_t        B0_phi_K1_pt;
   Double_t        B0_phi_K1_p;
   Double_t        B0_phi_K1_E;
   Double_t        B0_phi_K1_isSignal;
   Double_t        B0_phi_K1_mcErrors;
   Double_t        B0_phi_K1_mcPDG;
   Double_t        B0_phi_K1_kaonID;
   Double_t        B0_phi_K1_kaonBID;
   Double_t        B0_phi_K1_pionID;
   Double_t        B0_phi_K1_protonID;
   Double_t        B0_phi_K1_muonID;
   Double_t        B0_phi_K1_electronID;
   Double_t        B0_phi_K1_deuteronID;
   Double_t        B0_phi_K1_mothID;
   Double_t        B0_phi_K1_gdMothID;
   Double_t        B0_phi_K1_gdgdMothID;
   Double_t        B0_phi_K1_mothPDG;
   Double_t        B0_phi_K1_gdMothPDG;
   Double_t        B0_phi_K1_gdgdMothPDG;
   Double_t        B0_phi_K1_dr;
   Double_t        B0_phi_K1_dx;
   Double_t        B0_phi_K1_dy;
   Double_t        B0_phi_K1_dz;
   Double_t        B0_phi_K1_d0;
   Double_t        B0_phi_K1_z0;
   Double_t        B0_phi_K1_pValue;
   Double_t        B0_phi_K1_nCDCHits;
   Double_t        B0_phi_K1_nPXDHits;
   Double_t        B0_phi_K1_nSVDHits;
   Double_t        B0_phi_K1_nVXDHits;
   Double_t        B0_phi_K2_charge;
   Double_t        B0_phi_K2_px;
   Double_t        B0_phi_K2_py;
   Double_t        B0_phi_K2_pz;
   Double_t        B0_phi_K2_pt;
   Double_t        B0_phi_K2_p;
   Double_t        B0_phi_K2_E;
   Double_t        B0_phi_K2_isSignal;
   Double_t        B0_phi_K2_mcErrors;
   Double_t        B0_phi_K2_mcPDG;
   Double_t        B0_phi_K2_kaonID;
   Double_t        B0_phi_K2_kaonBID;
   Double_t        B0_phi_K2_pionID;
   Double_t        B0_phi_K2_protonID;
   Double_t        B0_phi_K2_muonID;
   Double_t        B0_phi_K2_electronID;
   Double_t        B0_phi_K2_deuteronID;
   Double_t        B0_phi_K2_mothID;
   Double_t        B0_phi_K2_gdMothID;
   Double_t        B0_phi_K2_gdgdMothID;
   Double_t        B0_phi_K2_mothPDG;
   Double_t        B0_phi_K2_gdMothPDG;
   Double_t        B0_phi_K2_gdgdMothPDG;
   Double_t        B0_phi_K2_dr;
   Double_t        B0_phi_K2_dx;
   Double_t        B0_phi_K2_dy;
   Double_t        B0_phi_K2_dz;
   Double_t        B0_phi_K2_d0;
   Double_t        B0_phi_K2_z0;
   Double_t        B0_phi_K2_pValue;
   Double_t        B0_phi_K2_nCDCHits;
   Double_t        B0_phi_K2_nPXDHits;
   Double_t        B0_phi_K2_nSVDHits;
   Double_t        B0_phi_K2_nVXDHits;
   Double_t        B0_Ks_pi1_charge;
   Double_t        B0_Ks_pi1_px;
   Double_t        B0_Ks_pi1_py;
   Double_t        B0_Ks_pi1_pz;
   Double_t        B0_Ks_pi1_pt;
   Double_t        B0_Ks_pi1_p;
   Double_t        B0_Ks_pi1_E;
   Double_t        B0_Ks_pi1_isSignal;
   Double_t        B0_Ks_pi1_mcErrors;
   Double_t        B0_Ks_pi1_mcPDG;
   Double_t        B0_Ks_pi1_kaonID;
   Double_t        B0_Ks_pi1_kaonBID;
   Double_t        B0_Ks_pi1_pionID;
   Double_t        B0_Ks_pi1_protonID;
   Double_t        B0_Ks_pi1_muonID;
   Double_t        B0_Ks_pi1_electronID;
   Double_t        B0_Ks_pi1_deuteronID;
   Double_t        B0_Ks_pi1_mothID;
   Double_t        B0_Ks_pi1_gdMothID;
   Double_t        B0_Ks_pi1_gdgdMothID;
   Double_t        B0_Ks_pi1_mothPDG;
   Double_t        B0_Ks_pi1_gdMothPDG;
   Double_t        B0_Ks_pi1_gdgdMothPDG;
   Double_t        B0_Ks_pi1_dr;
   Double_t        B0_Ks_pi1_dx;
   Double_t        B0_Ks_pi1_dy;
   Double_t        B0_Ks_pi1_dz;
   Double_t        B0_Ks_pi1_d0;
   Double_t        B0_Ks_pi1_z0;
   Double_t        B0_Ks_pi1_pValue;
   Double_t        B0_Ks_pi1_nCDCHits;
   Double_t        B0_Ks_pi1_nPXDHits;
   Double_t        B0_Ks_pi1_nSVDHits;
   Double_t        B0_Ks_pi1_nVXDHits;
   Double_t        B0_Ks_pi2_charge;
   Double_t        B0_Ks_pi2_px;
   Double_t        B0_Ks_pi2_py;
   Double_t        B0_Ks_pi2_pz;
   Double_t        B0_Ks_pi2_pt;
   Double_t        B0_Ks_pi2_p;
   Double_t        B0_Ks_pi2_E;
   Double_t        B0_Ks_pi2_isSignal;
   Double_t        B0_Ks_pi2_mcErrors;
   Double_t        B0_Ks_pi2_mcPDG;
   Double_t        B0_Ks_pi2_kaonID;
   Double_t        B0_Ks_pi2_kaonBID;
   Double_t        B0_Ks_pi2_pionID;
   Double_t        B0_Ks_pi2_protonID;
   Double_t        B0_Ks_pi2_muonID;
   Double_t        B0_Ks_pi2_electronID;
   Double_t        B0_Ks_pi2_deuteronID;
   Double_t        B0_Ks_pi2_mothID;
   Double_t        B0_Ks_pi2_gdMothID;
   Double_t        B0_Ks_pi2_gdgdMothID;
   Double_t        B0_Ks_pi2_mothPDG;
   Double_t        B0_Ks_pi2_gdMothPDG;
   Double_t        B0_Ks_pi2_gdgdMothPDG;
   Double_t        B0_Ks_pi2_dr;
   Double_t        B0_Ks_pi2_dx;
   Double_t        B0_Ks_pi2_dy;
   Double_t        B0_Ks_pi2_dz;
   Double_t        B0_Ks_pi2_d0;
   Double_t        B0_Ks_pi2_z0;
   Double_t        B0_Ks_pi2_pValue;
   Double_t        B0_Ks_pi2_nCDCHits;
   Double_t        B0_Ks_pi2_nPXDHits;
   Double_t        B0_Ks_pi2_nSVDHits;
   Double_t        B0_Ks_pi2_nVXDHits;
   Int_t           isNonRes;
   Int_t           customIsSig;


   // List of branches
   TBranch        *b___experiment__;   //!
   TBranch        *b___run__;   //!
   TBranch        *b___event__;   //!
   TBranch        *b___candidate__;   //!
   TBranch        *b___ncandidates__;   //!
   TBranch        *b___weight__;   //!
   TBranch        *b_M;   //!
   TBranch        *b_ErrM;   //!
   TBranch        *b_SigM;   //!
   TBranch        *b_InvM;   //!
   TBranch        *b_distance;   //!
   TBranch        *b_significanceOfDistance;   //!
   TBranch        *b_dx;   //!
   TBranch        *b_dy;   //!
   TBranch        *b_dz;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_z;   //!
   TBranch        *b_x_uncertainty;   //!
   TBranch        *b_y_uncertainty;   //!
   TBranch        *b_z_uncertainty;   //!
   TBranch        *b_dr;   //!
   TBranch        *b_dphi;   //!
   TBranch        *b_dcosTheta;   //!
   TBranch        *b_prodVertexX;   //!
   TBranch        *b_prodVertexY;   //!
   TBranch        *b_prodVertexZ;   //!
   TBranch        *b_prodVertexXErr;   //!
   TBranch        *b_prodVertexYErr;   //!
   TBranch        *b_prodVertexZErr;   //!
   TBranch        *b_chiProb;   //!
   TBranch        *b_mcX;   //!
   TBranch        *b_mcY;   //!
   TBranch        *b_mcZ;   //!
   TBranch        *b_mcDistance;   //!
   TBranch        *b_mcRho;   //!
   TBranch        *b_mcProdVertexX;   //!
   TBranch        *b_mcProdVertexY;   //!
   TBranch        *b_mcProdVertexZ;   //!
   TBranch        *b_px;   //!
   TBranch        *b_py;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_p;   //!
   TBranch        *b_E;   //!
   TBranch        *b_isSignal;   //!
   TBranch        *b_mcErrors;   //!
   TBranch        *b_mcPDG;   //!
   TBranch        *b_mothID;   //!
   TBranch        *b_gdMothID;   //!
   TBranch        *b_gdgdMothID;   //!
   TBranch        *b_mothPDG;   //!
   TBranch        *b_gdMothPDG;   //!
   TBranch        *b_gdgdMothPDG;   //!
   TBranch        *b_Mbc;   //!
   TBranch        *b_deltaE;   //!
   TBranch        *b_foxWolframR1;   //!
   TBranch        *b_foxWolframR2;   //!
   TBranch        *b_foxWolframR3;   //!
   TBranch        *b_foxWolframR4;   //!
   TBranch        *b_harmonicMomentThrust0;   //!
   TBranch        *b_harmonicMomentThrust1;   //!
   TBranch        *b_harmonicMomentThrust2;   //!
   TBranch        *b_harmonicMomentThrust3;   //!
   TBranch        *b_harmonicMomentThrust4;   //!
   TBranch        *b_cleoConeThrust0;   //!
   TBranch        *b_cleoConeThrust1;   //!
   TBranch        *b_cleoConeThrust2;   //!
   TBranch        *b_cleoConeThrust3;   //!
   TBranch        *b_cleoConeThrust4;   //!
   TBranch        *b_cleoConeThrust5;   //!
   TBranch        *b_cleoConeThrust6;   //!
   TBranch        *b_cleoConeThrust7;   //!
   TBranch        *b_cleoConeThrust8;   //!
   TBranch        *b_sphericity;   //!
   TBranch        *b_aplanarity;   //!
   TBranch        *b_thrust;   //!
   TBranch        *b_thrustAxisCosTheta;   //!
   TBranch        *b_cosTBTO;   //!
   TBranch        *b_cosTBz;   //!
   TBranch        *b_thrustOm;   //!
   TBranch        *b_thrustBm;   //!
   TBranch        *b_KSFWVariables__boet__bc;   //!
   TBranch        *b_KSFWVariables__bomm2__bc;   //!
   TBranch        *b_KSFWVariables__bohso00__bc;   //!
   TBranch        *b_KSFWVariables__bohso04__bc;   //!
   TBranch        *b_KSFWVariables__bohso10__bc;   //!
   TBranch        *b_KSFWVariables__bohso14__bc;   //!
   TBranch        *b_KSFWVariables__bohso20__bc;   //!
   TBranch        *b_KSFWVariables__bohso22__bc;   //!
   TBranch        *b_KSFWVariables__bohso24__bc;   //!
   TBranch        *b_KSFWVariables__bohoo0__bc;   //!
   TBranch        *b_KSFWVariables__bohoo1__bc;   //!
   TBranch        *b_KSFWVariables__bohoo2__bc;   //!
   TBranch        *b_KSFWVariables__bohoo3__bc;   //!
   TBranch        *b_KSFWVariables__bohoo4__bc;   //!
   TBranch        *b_DeltaT;   //!
   TBranch        *b_DeltaTErr;   //!
   TBranch        *b_DeltaZ;   //!
   TBranch        *b_DeltaZErr;   //!
   TBranch        *b_DeltaBoost;   //!
   TBranch        *b_DeltaBoostErr;   //!
   TBranch        *b_TagVLBoost;   //!
   TBranch        *b_TagVLBoostErr;   //!
   TBranch        *b_TagVOBoost;   //!
   TBranch        *b_TagVOBoostErr;   //!
   TBranch        *b_TagVpVal;   //!
   TBranch        *b_TagVNDF;   //!
   TBranch        *b_TagVChi2;   //!
   TBranch        *b_TagVChi2IP;   //!
   TBranch        *b_TagVx;   //!
   TBranch        *b_TagVxErr;   //!
   TBranch        *b_TagVy;   //!
   TBranch        *b_TagVyErr;   //!
   TBranch        *b_TagVz;   //!
   TBranch        *b_TagVzErr;   //!
   TBranch        *b_MCDeltaT;   //!
   TBranch        *b_TagVmcLBoost;   //!
   TBranch        *b_TagVmcOBoost;   //!
   TBranch        *b_mcLBoost;   //!
   TBranch        *b_mcOBoost;   //!
   TBranch        *b_mcTagVx;   //!
   TBranch        *b_mcTagVy;   //!
   TBranch        *b_mcTagVz;   //!
   TBranch        *b_isNotContinuumEvent;   //!
   TBranch        *b_phiCosHel;   //!
   TBranch        *b_out_FastBDT;   //!
   TBranch        *b_csv_FastBDT;   //!
   TBranch        *b_out_FastBDT1;   //!
   TBranch        *b_csv_FastBDT1;   //!
   TBranch        *b_out_FastBDT2;   //!
   TBranch        *b_csv_FastBDT2;   //!
   TBranch        *b_out_FastBDT3;   //!
   TBranch        *b_csv_FastBDT3;   //!
   TBranch        *b_FBDT_flTag_qr;   //!
   TBranch        *b_FANN_flTag_qr;   //!
   TBranch        *b_B0_phi_M;   //!
   TBranch        *b_B0_phi_ErrM;   //!
   TBranch        *b_B0_phi_SigM;   //!
   TBranch        *b_B0_phi_InvM;   //!
   TBranch        *b_B0_phi_distance;   //!
   TBranch        *b_B0_phi_significanceOfDistance;   //!
   TBranch        *b_B0_phi_dx;   //!
   TBranch        *b_B0_phi_dy;   //!
   TBranch        *b_B0_phi_dz;   //!
   TBranch        *b_B0_phi_x;   //!
   TBranch        *b_B0_phi_y;   //!
   TBranch        *b_B0_phi_z;   //!
   TBranch        *b_B0_phi_x_uncertainty;   //!
   TBranch        *b_B0_phi_y_uncertainty;   //!
   TBranch        *b_B0_phi_z_uncertainty;   //!
   TBranch        *b_B0_phi_dr;   //!
   TBranch        *b_B0_phi_dphi;   //!
   TBranch        *b_B0_phi_dcosTheta;   //!
   TBranch        *b_B0_phi_prodVertexX;   //!
   TBranch        *b_B0_phi_prodVertexY;   //!
   TBranch        *b_B0_phi_prodVertexZ;   //!
   TBranch        *b_B0_phi_prodVertexXErr;   //!
   TBranch        *b_B0_phi_prodVertexYErr;   //!
   TBranch        *b_B0_phi_prodVertexZErr;   //!
   TBranch        *b_B0_phi_chiProb;   //!
   TBranch        *b_B0_phi_mcX;   //!
   TBranch        *b_B0_phi_mcY;   //!
   TBranch        *b_B0_phi_mcZ;   //!
   TBranch        *b_B0_phi_mcDistance;   //!
   TBranch        *b_B0_phi_mcRho;   //!
   TBranch        *b_B0_phi_mcProdVertexX;   //!
   TBranch        *b_B0_phi_mcProdVertexY;   //!
   TBranch        *b_B0_phi_mcProdVertexZ;   //!
   TBranch        *b_B0_phi_px;   //!
   TBranch        *b_B0_phi_py;   //!
   TBranch        *b_B0_phi_pz;   //!
   TBranch        *b_B0_phi_pt;   //!
   TBranch        *b_B0_phi_p;   //!
   TBranch        *b_B0_phi_E;   //!
   TBranch        *b_B0_phi_isSignal;   //!
   TBranch        *b_B0_phi_mcErrors;   //!
   TBranch        *b_B0_phi_mcPDG;   //!
   TBranch        *b_B0_phi_mothID;   //!
   TBranch        *b_B0_phi_gdMothID;   //!
   TBranch        *b_B0_phi_gdgdMothID;   //!
   TBranch        *b_B0_phi_mothPDG;   //!
   TBranch        *b_B0_phi_gdMothPDG;   //!
   TBranch        *b_B0_phi_gdgdMothPDG;   //!
   TBranch        *b_B0_Ks_M;   //!
   TBranch        *b_B0_Ks_ErrM;   //!
   TBranch        *b_B0_Ks_SigM;   //!
   TBranch        *b_B0_Ks_InvM;   //!
   TBranch        *b_B0_Ks_distance;   //!
   TBranch        *b_B0_Ks_significanceOfDistance;   //!
   TBranch        *b_B0_Ks_dx;   //!
   TBranch        *b_B0_Ks_dy;   //!
   TBranch        *b_B0_Ks_dz;   //!
   TBranch        *b_B0_Ks_x;   //!
   TBranch        *b_B0_Ks_y;   //!
   TBranch        *b_B0_Ks_z;   //!
   TBranch        *b_B0_Ks_x_uncertainty;   //!
   TBranch        *b_B0_Ks_y_uncertainty;   //!
   TBranch        *b_B0_Ks_z_uncertainty;   //!
   TBranch        *b_B0_Ks_dr;   //!
   TBranch        *b_B0_Ks_dphi;   //!
   TBranch        *b_B0_Ks_dcosTheta;   //!
   TBranch        *b_B0_Ks_prodVertexX;   //!
   TBranch        *b_B0_Ks_prodVertexY;   //!
   TBranch        *b_B0_Ks_prodVertexZ;   //!
   TBranch        *b_B0_Ks_prodVertexXErr;   //!
   TBranch        *b_B0_Ks_prodVertexYErr;   //!
   TBranch        *b_B0_Ks_prodVertexZErr;   //!
   TBranch        *b_B0_Ks_chiProb;   //!
   TBranch        *b_B0_Ks_mcX;   //!
   TBranch        *b_B0_Ks_mcY;   //!
   TBranch        *b_B0_Ks_mcZ;   //!
   TBranch        *b_B0_Ks_mcDistance;   //!
   TBranch        *b_B0_Ks_mcRho;   //!
   TBranch        *b_B0_Ks_mcProdVertexX;   //!
   TBranch        *b_B0_Ks_mcProdVertexY;   //!
   TBranch        *b_B0_Ks_mcProdVertexZ;   //!
   TBranch        *b_B0_Ks_px;   //!
   TBranch        *b_B0_Ks_py;   //!
   TBranch        *b_B0_Ks_pz;   //!
   TBranch        *b_B0_Ks_pt;   //!
   TBranch        *b_B0_Ks_p;   //!
   TBranch        *b_B0_Ks_E;   //!
   TBranch        *b_B0_Ks_isSignal;   //!
   TBranch        *b_B0_Ks_mcErrors;   //!
   TBranch        *b_B0_Ks_mcPDG;   //!
   TBranch        *b_B0_Ks_mothID;   //!
   TBranch        *b_B0_Ks_gdMothID;   //!
   TBranch        *b_B0_Ks_gdgdMothID;   //!
   TBranch        *b_B0_Ks_mothPDG;   //!
   TBranch        *b_B0_Ks_gdMothPDG;   //!
   TBranch        *b_B0_Ks_gdgdMothPDG;   //!
   TBranch        *b_B0_phi_K1_charge;   //!
   TBranch        *b_B0_phi_K1_px;   //!
   TBranch        *b_B0_phi_K1_py;   //!
   TBranch        *b_B0_phi_K1_pz;   //!
   TBranch        *b_B0_phi_K1_pt;   //!
   TBranch        *b_B0_phi_K1_p;   //!
   TBranch        *b_B0_phi_K1_E;   //!
   TBranch        *b_B0_phi_K1_isSignal;   //!
   TBranch        *b_B0_phi_K1_mcErrors;   //!
   TBranch        *b_B0_phi_K1_mcPDG;   //!
   TBranch        *b_B0_phi_K1_kaonID;   //!
   TBranch        *b_B0_phi_K1_kaonBID;   //!
   TBranch        *b_B0_phi_K1_pionID;   //!
   TBranch        *b_B0_phi_K1_protonID;   //!
   TBranch        *b_B0_phi_K1_muonID;   //!
   TBranch        *b_B0_phi_K1_electronID;   //!
   TBranch        *b_B0_phi_K1_deuteronID;   //!
   TBranch        *b_B0_phi_K1_mothID;   //!
   TBranch        *b_B0_phi_K1_gdMothID;   //!
   TBranch        *b_B0_phi_K1_gdgdMothID;   //!
   TBranch        *b_B0_phi_K1_mothPDG;   //!
   TBranch        *b_B0_phi_K1_gdMothPDG;   //!
   TBranch        *b_B0_phi_K1_gdgdMothPDG;   //!
   TBranch        *b_B0_phi_K1_dr;   //!
   TBranch        *b_B0_phi_K1_dx;   //!
   TBranch        *b_B0_phi_K1_dy;   //!
   TBranch        *b_B0_phi_K1_dz;   //!
   TBranch        *b_B0_phi_K1_d0;   //!
   TBranch        *b_B0_phi_K1_z0;   //!
   TBranch        *b_B0_phi_K1_pValue;   //!
   TBranch        *b_B0_phi_K1_nCDCHits;   //!
   TBranch        *b_B0_phi_K1_nPXDHits;   //!
   TBranch        *b_B0_phi_K1_nSVDHits;   //!
   TBranch        *b_B0_phi_K1_nVXDHits;   //!
   TBranch        *b_B0_phi_K2_charge;   //!
   TBranch        *b_B0_phi_K2_px;   //!
   TBranch        *b_B0_phi_K2_py;   //!
   TBranch        *b_B0_phi_K2_pz;   //!
   TBranch        *b_B0_phi_K2_pt;   //!
   TBranch        *b_B0_phi_K2_p;   //!
   TBranch        *b_B0_phi_K2_E;   //!
   TBranch        *b_B0_phi_K2_isSignal;   //!
   TBranch        *b_B0_phi_K2_mcErrors;   //!
   TBranch        *b_B0_phi_K2_mcPDG;   //!
   TBranch        *b_B0_phi_K2_kaonID;   //!
   TBranch        *b_B0_phi_K2_kaonBID;   //!
   TBranch        *b_B0_phi_K2_pionID;   //!
   TBranch        *b_B0_phi_K2_protonID;   //!
   TBranch        *b_B0_phi_K2_muonID;   //!
   TBranch        *b_B0_phi_K2_electronID;   //!
   TBranch        *b_B0_phi_K2_deuteronID;   //!
   TBranch        *b_B0_phi_K2_mothID;   //!
   TBranch        *b_B0_phi_K2_gdMothID;   //!
   TBranch        *b_B0_phi_K2_gdgdMothID;   //!
   TBranch        *b_B0_phi_K2_mothPDG;   //!
   TBranch        *b_B0_phi_K2_gdMothPDG;   //!
   TBranch        *b_B0_phi_K2_gdgdMothPDG;   //!
   TBranch        *b_B0_phi_K2_dr;   //!
   TBranch        *b_B0_phi_K2_dx;   //!
   TBranch        *b_B0_phi_K2_dy;   //!
   TBranch        *b_B0_phi_K2_dz;   //!
   TBranch        *b_B0_phi_K2_d0;   //!
   TBranch        *b_B0_phi_K2_z0;   //!
   TBranch        *b_B0_phi_K2_pValue;   //!
   TBranch        *b_B0_phi_K2_nCDCHits;   //!
   TBranch        *b_B0_phi_K2_nPXDHits;   //!
   TBranch        *b_B0_phi_K2_nSVDHits;   //!
   TBranch        *b_B0_phi_K2_nVXDHits;   //!
   TBranch        *b_B0_Ks_pi1_charge;   //!
   TBranch        *b_B0_Ks_pi1_px;   //!
   TBranch        *b_B0_Ks_pi1_py;   //!
   TBranch        *b_B0_Ks_pi1_pz;   //!
   TBranch        *b_B0_Ks_pi1_pt;   //!
   TBranch        *b_B0_Ks_pi1_p;   //!
   TBranch        *b_B0_Ks_pi1_E;   //!
   TBranch        *b_B0_Ks_pi1_isSignal;   //!
   TBranch        *b_B0_Ks_pi1_mcErrors;   //!
   TBranch        *b_B0_Ks_pi1_mcPDG;   //!
   TBranch        *b_B0_Ks_pi1_kaonID;   //!
   TBranch        *b_B0_Ks_pi1_kaonBID;   //!
   TBranch        *b_B0_Ks_pi1_pionID;   //!
   TBranch        *b_B0_Ks_pi1_protonID;   //!
   TBranch        *b_B0_Ks_pi1_muonID;   //!
   TBranch        *b_B0_Ks_pi1_electronID;   //!
   TBranch        *b_B0_Ks_pi1_deuteronID;   //!
   TBranch        *b_B0_Ks_pi1_mothID;   //!
   TBranch        *b_B0_Ks_pi1_gdMothID;   //!
   TBranch        *b_B0_Ks_pi1_gdgdMothID;   //!
   TBranch        *b_B0_Ks_pi1_mothPDG;   //!
   TBranch        *b_B0_Ks_pi1_gdMothPDG;   //!
   TBranch        *b_B0_Ks_pi1_gdgdMothPDG;   //!
   TBranch        *b_B0_Ks_pi1_dr;   //!
   TBranch        *b_B0_Ks_pi1_dx;   //!
   TBranch        *b_B0_Ks_pi1_dy;   //!
   TBranch        *b_B0_Ks_pi1_dz;   //!
   TBranch        *b_B0_Ks_pi1_d0;   //!
   TBranch        *b_B0_Ks_pi1_z0;   //!
   TBranch        *b_B0_Ks_pi1_pValue;   //!
   TBranch        *b_B0_Ks_pi1_nCDCHits;   //!
   TBranch        *b_B0_Ks_pi1_nPXDHits;   //!
   TBranch        *b_B0_Ks_pi1_nSVDHits;   //!
   TBranch        *b_B0_Ks_pi1_nVXDHits;   //!
   TBranch        *b_B0_Ks_pi2_charge;   //!
   TBranch        *b_B0_Ks_pi2_px;   //!
   TBranch        *b_B0_Ks_pi2_py;   //!
   TBranch        *b_B0_Ks_pi2_pz;   //!
   TBranch        *b_B0_Ks_pi2_pt;   //!
   TBranch        *b_B0_Ks_pi2_p;   //!
   TBranch        *b_B0_Ks_pi2_E;   //!
   TBranch        *b_B0_Ks_pi2_isSignal;   //!
   TBranch        *b_B0_Ks_pi2_mcErrors;   //!
   TBranch        *b_B0_Ks_pi2_mcPDG;   //!
   TBranch        *b_B0_Ks_pi2_kaonID;   //!
   TBranch        *b_B0_Ks_pi2_kaonBID;   //!
   TBranch        *b_B0_Ks_pi2_pionID;   //!
   TBranch        *b_B0_Ks_pi2_protonID;   //!
   TBranch        *b_B0_Ks_pi2_muonID;   //!
   TBranch        *b_B0_Ks_pi2_electronID;   //!
   TBranch        *b_B0_Ks_pi2_deuteronID;   //!
   TBranch        *b_B0_Ks_pi2_mothID;   //!
   TBranch        *b_B0_Ks_pi2_gdMothID;   //!
   TBranch        *b_B0_Ks_pi2_gdgdMothID;   //!
   TBranch        *b_B0_Ks_pi2_mothPDG;   //!
   TBranch        *b_B0_Ks_pi2_gdMothPDG;   //!
   TBranch        *b_B0_Ks_pi2_gdgdMothPDG;   //!
   TBranch        *b_B0_Ks_pi2_dr;   //!
   TBranch        *b_B0_Ks_pi2_dx;   //!
   TBranch        *b_B0_Ks_pi2_dy;   //!
   TBranch        *b_B0_Ks_pi2_dz;   //!
   TBranch        *b_B0_Ks_pi2_d0;   //!
   TBranch        *b_B0_Ks_pi2_z0;   //!
   TBranch        *b_B0_Ks_pi2_pValue;   //!
   TBranch        *b_B0_Ks_pi2_nCDCHits;   //!
   TBranch        *b_B0_Ks_pi2_nPXDHits;   //!
   TBranch        *b_B0_Ks_pi2_nSVDHits;   //!
   TBranch        *b_B0_Ks_pi2_nVXDHits;   //!
   TBranch        *b_isNonRes;   //!
   TBranch        *b_customIsSig;   //!

   
   //-----------------------------------------------------------------------------

   int nBinsMbc = 40;
   float lowMbc = 5.20;
   float highMbc = 5.30;

   int nBinsDeltaE = 40;
   float lowDeltaE = -0.2;
   float highDeltaE = -0.2;

   int nBinsMPhi = 40;
   float lowMPhi = 1.00;
   float highMPhi = 1.04;

   int nBinsMKst = 60;
   float lowMKst = 0.74;
   float highMKst = 1.04;

   int nBinsPhiHel = 40;
   float lowPhiHel = -1.;
   float highPhiHel = 1.;

   int nBinsKstHel = 40;
   float lowKstHel = -1.;
   float highKstHel = 1.;

   int nBinsFBDT = 40;
   float lowFBDT = -10.;
   float highFBDT = 10.;

   
   TH1F *hNCand;
   TH1F *hMbc;
   TH1F *hDeltaE;
   TH1F *hMKst;
   TH1F *hMPhi;
   TH1F *hPhiHel;
   TH1F *hKstHel;
   TH1F *hFBDT;

   
   //-----------------------------------------------------------------------------

   
   PhiKsAna(TTree *tree=0);
   virtual ~PhiKsAna();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     InitializeHistos(TString hLabel);
   virtual void     Loop(bool isDebug, bool isBlind, bool onlyOneCand, TString outRootFileName, bool applyDataPID);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PhiKsAna_cxx
PhiKsAna::PhiKsAna(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data/PhiKs_analysis/PhiKs_signal_BGx0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data/PhiKs_analysis/PhiKs_signal_BGx0.root");
      }
      f->GetObject("B0_ch1",tree);

   }
   Init(tree);
}

PhiKsAna::~PhiKsAna()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PhiKsAna::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PhiKsAna::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void PhiKsAna::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("__experiment__", &__experiment__, &b___experiment__);
   fChain->SetBranchAddress("__run__", &__run__, &b___run__);
   fChain->SetBranchAddress("__event__", &__event__, &b___event__);
   fChain->SetBranchAddress("__candidate__", &__candidate__, &b___candidate__);
   fChain->SetBranchAddress("__ncandidates__", &__ncandidates__, &b___ncandidates__);
   fChain->SetBranchAddress("__weight__", &__weight__, &b___weight__);
   fChain->SetBranchAddress("M", &M, &b_M);
   fChain->SetBranchAddress("ErrM", &ErrM, &b_ErrM);
   fChain->SetBranchAddress("SigM", &SigM, &b_SigM);
   fChain->SetBranchAddress("InvM", &InvM, &b_InvM);
   fChain->SetBranchAddress("distance", &distance, &b_distance);
   fChain->SetBranchAddress("significanceOfDistance", &significanceOfDistance, &b_significanceOfDistance);
   fChain->SetBranchAddress("dx", &dx, &b_dx);
   fChain->SetBranchAddress("dy", &dy, &b_dy);
   fChain->SetBranchAddress("dz", &dz, &b_dz);
   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("z", &z, &b_z);
   fChain->SetBranchAddress("x_uncertainty", &x_uncertainty, &b_x_uncertainty);
   fChain->SetBranchAddress("y_uncertainty", &y_uncertainty, &b_y_uncertainty);
   fChain->SetBranchAddress("z_uncertainty", &z_uncertainty, &b_z_uncertainty);
   fChain->SetBranchAddress("dr", &dr, &b_dr);
   fChain->SetBranchAddress("dphi", &dphi, &b_dphi);
   fChain->SetBranchAddress("dcosTheta", &dcosTheta, &b_dcosTheta);
   fChain->SetBranchAddress("prodVertexX", &prodVertexX, &b_prodVertexX);
   fChain->SetBranchAddress("prodVertexY", &prodVertexY, &b_prodVertexY);
   fChain->SetBranchAddress("prodVertexZ", &prodVertexZ, &b_prodVertexZ);
   fChain->SetBranchAddress("prodVertexXErr", &prodVertexXErr, &b_prodVertexXErr);
   fChain->SetBranchAddress("prodVertexYErr", &prodVertexYErr, &b_prodVertexYErr);
   fChain->SetBranchAddress("prodVertexZErr", &prodVertexZErr, &b_prodVertexZErr);
   fChain->SetBranchAddress("chiProb", &chiProb, &b_chiProb);
   fChain->SetBranchAddress("mcX", &mcX, &b_mcX);
   fChain->SetBranchAddress("mcY", &mcY, &b_mcY);
   fChain->SetBranchAddress("mcZ", &mcZ, &b_mcZ);
   fChain->SetBranchAddress("mcDistance", &mcDistance, &b_mcDistance);
   fChain->SetBranchAddress("mcRho", &mcRho, &b_mcRho);
   fChain->SetBranchAddress("mcProdVertexX", &mcProdVertexX, &b_mcProdVertexX);
   fChain->SetBranchAddress("mcProdVertexY", &mcProdVertexY, &b_mcProdVertexY);
   fChain->SetBranchAddress("mcProdVertexZ", &mcProdVertexZ, &b_mcProdVertexZ);
   fChain->SetBranchAddress("px", &px, &b_px);
   fChain->SetBranchAddress("py", &py, &b_py);
   fChain->SetBranchAddress("pz", &pz, &b_pz);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("p", &p, &b_p);
   fChain->SetBranchAddress("E", &E, &b_E);
   fChain->SetBranchAddress("isSignal", &isSignal, &b_isSignal);
   fChain->SetBranchAddress("mcErrors", &mcErrors, &b_mcErrors);
   fChain->SetBranchAddress("mcPDG", &mcPDG, &b_mcPDG);
   fChain->SetBranchAddress("mothID", &mothID, &b_mothID);
   fChain->SetBranchAddress("gdMothID", &gdMothID, &b_gdMothID);
   fChain->SetBranchAddress("gdgdMothID", &gdgdMothID, &b_gdgdMothID);
   fChain->SetBranchAddress("mothPDG", &mothPDG, &b_mothPDG);
   fChain->SetBranchAddress("gdMothPDG", &gdMothPDG, &b_gdMothPDG);
   fChain->SetBranchAddress("gdgdMothPDG", &gdgdMothPDG, &b_gdgdMothPDG);
   fChain->SetBranchAddress("Mbc", &Mbc, &b_Mbc);
   fChain->SetBranchAddress("deltaE", &deltaE, &b_deltaE);
   fChain->SetBranchAddress("foxWolframR1", &foxWolframR1, &b_foxWolframR1);
   fChain->SetBranchAddress("foxWolframR2", &foxWolframR2, &b_foxWolframR2);
   fChain->SetBranchAddress("foxWolframR3", &foxWolframR3, &b_foxWolframR3);
   fChain->SetBranchAddress("foxWolframR4", &foxWolframR4, &b_foxWolframR4);
   fChain->SetBranchAddress("harmonicMomentThrust0", &harmonicMomentThrust0, &b_harmonicMomentThrust0);
   fChain->SetBranchAddress("harmonicMomentThrust1", &harmonicMomentThrust1, &b_harmonicMomentThrust1);
   fChain->SetBranchAddress("harmonicMomentThrust2", &harmonicMomentThrust2, &b_harmonicMomentThrust2);
   fChain->SetBranchAddress("harmonicMomentThrust3", &harmonicMomentThrust3, &b_harmonicMomentThrust3);
   fChain->SetBranchAddress("harmonicMomentThrust4", &harmonicMomentThrust4, &b_harmonicMomentThrust4);
   fChain->SetBranchAddress("cleoConeThrust0", &cleoConeThrust0, &b_cleoConeThrust0);
   fChain->SetBranchAddress("cleoConeThrust1", &cleoConeThrust1, &b_cleoConeThrust1);
   fChain->SetBranchAddress("cleoConeThrust2", &cleoConeThrust2, &b_cleoConeThrust2);
   fChain->SetBranchAddress("cleoConeThrust3", &cleoConeThrust3, &b_cleoConeThrust3);
   fChain->SetBranchAddress("cleoConeThrust4", &cleoConeThrust4, &b_cleoConeThrust4);
   fChain->SetBranchAddress("cleoConeThrust5", &cleoConeThrust5, &b_cleoConeThrust5);
   fChain->SetBranchAddress("cleoConeThrust6", &cleoConeThrust6, &b_cleoConeThrust6);
   fChain->SetBranchAddress("cleoConeThrust7", &cleoConeThrust7, &b_cleoConeThrust7);
   fChain->SetBranchAddress("cleoConeThrust8", &cleoConeThrust8, &b_cleoConeThrust8);
   fChain->SetBranchAddress("sphericity", &sphericity, &b_sphericity);
   fChain->SetBranchAddress("aplanarity", &aplanarity, &b_aplanarity);
   fChain->SetBranchAddress("thrust", &thrust, &b_thrust);
   fChain->SetBranchAddress("thrustAxisCosTheta", &thrustAxisCosTheta, &b_thrustAxisCosTheta);
   fChain->SetBranchAddress("cosTBTO", &cosTBTO, &b_cosTBTO);
   fChain->SetBranchAddress("cosTBz", &cosTBz, &b_cosTBz);
   fChain->SetBranchAddress("thrustOm", &thrustOm, &b_thrustOm);
   fChain->SetBranchAddress("thrustBm", &thrustBm, &b_thrustBm);
   fChain->SetBranchAddress("KSFWVariables__boet__bc", &KSFWVariables__boet__bc, &b_KSFWVariables__boet__bc);
   fChain->SetBranchAddress("KSFWVariables__bomm2__bc", &KSFWVariables__bomm2__bc, &b_KSFWVariables__bomm2__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso00__bc", &KSFWVariables__bohso00__bc, &b_KSFWVariables__bohso00__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso04__bc", &KSFWVariables__bohso04__bc, &b_KSFWVariables__bohso04__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso10__bc", &KSFWVariables__bohso10__bc, &b_KSFWVariables__bohso10__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso14__bc", &KSFWVariables__bohso14__bc, &b_KSFWVariables__bohso14__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso20__bc", &KSFWVariables__bohso20__bc, &b_KSFWVariables__bohso20__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso22__bc", &KSFWVariables__bohso22__bc, &b_KSFWVariables__bohso22__bc);
   fChain->SetBranchAddress("KSFWVariables__bohso24__bc", &KSFWVariables__bohso24__bc, &b_KSFWVariables__bohso24__bc);
   fChain->SetBranchAddress("KSFWVariables__bohoo0__bc", &KSFWVariables__bohoo0__bc, &b_KSFWVariables__bohoo0__bc);
   fChain->SetBranchAddress("KSFWVariables__bohoo1__bc", &KSFWVariables__bohoo1__bc, &b_KSFWVariables__bohoo1__bc);
   fChain->SetBranchAddress("KSFWVariables__bohoo2__bc", &KSFWVariables__bohoo2__bc, &b_KSFWVariables__bohoo2__bc);
   fChain->SetBranchAddress("KSFWVariables__bohoo3__bc", &KSFWVariables__bohoo3__bc, &b_KSFWVariables__bohoo3__bc);
   fChain->SetBranchAddress("KSFWVariables__bohoo4__bc", &KSFWVariables__bohoo4__bc, &b_KSFWVariables__bohoo4__bc);
   fChain->SetBranchAddress("DeltaT", &DeltaT, &b_DeltaT);
   fChain->SetBranchAddress("DeltaTErr", &DeltaTErr, &b_DeltaTErr);
   fChain->SetBranchAddress("DeltaZ", &DeltaZ, &b_DeltaZ);
   fChain->SetBranchAddress("DeltaZErr", &DeltaZErr, &b_DeltaZErr);
   fChain->SetBranchAddress("DeltaBoost", &DeltaBoost, &b_DeltaBoost);
   fChain->SetBranchAddress("DeltaBoostErr", &DeltaBoostErr, &b_DeltaBoostErr);
   fChain->SetBranchAddress("TagVLBoost", &TagVLBoost, &b_TagVLBoost);
   fChain->SetBranchAddress("TagVLBoostErr", &TagVLBoostErr, &b_TagVLBoostErr);
   fChain->SetBranchAddress("TagVOBoost", &TagVOBoost, &b_TagVOBoost);
   fChain->SetBranchAddress("TagVOBoostErr", &TagVOBoostErr, &b_TagVOBoostErr);
   fChain->SetBranchAddress("TagVpVal", &TagVpVal, &b_TagVpVal);
   fChain->SetBranchAddress("TagVNDF", &TagVNDF, &b_TagVNDF);
   fChain->SetBranchAddress("TagVChi2", &TagVChi2, &b_TagVChi2);
   fChain->SetBranchAddress("TagVChi2IP", &TagVChi2IP, &b_TagVChi2IP);
   fChain->SetBranchAddress("TagVx", &TagVx, &b_TagVx);
   fChain->SetBranchAddress("TagVxErr", &TagVxErr, &b_TagVxErr);
   fChain->SetBranchAddress("TagVy", &TagVy, &b_TagVy);
   fChain->SetBranchAddress("TagVyErr", &TagVyErr, &b_TagVyErr);
   fChain->SetBranchAddress("TagVz", &TagVz, &b_TagVz);
   fChain->SetBranchAddress("TagVzErr", &TagVzErr, &b_TagVzErr);
   fChain->SetBranchAddress("MCDeltaT", &MCDeltaT, &b_MCDeltaT);
   fChain->SetBranchAddress("TagVmcLBoost", &TagVmcLBoost, &b_TagVmcLBoost);
   fChain->SetBranchAddress("TagVmcOBoost", &TagVmcOBoost, &b_TagVmcOBoost);
   fChain->SetBranchAddress("mcLBoost", &mcLBoost, &b_mcLBoost);
   fChain->SetBranchAddress("mcOBoost", &mcOBoost, &b_mcOBoost);
   fChain->SetBranchAddress("mcTagVx", &mcTagVx, &b_mcTagVx);
   fChain->SetBranchAddress("mcTagVy", &mcTagVy, &b_mcTagVy);
   fChain->SetBranchAddress("mcTagVz", &mcTagVz, &b_mcTagVz);
   fChain->SetBranchAddress("isNotContinuumEvent", &isNotContinuumEvent, &b_isNotContinuumEvent);
   fChain->SetBranchAddress("phiCosHel", &phiCosHel, &b_phiCosHel);
   fChain->SetBranchAddress("out_FastBDT", &out_FastBDT, &b_out_FastBDT);
   fChain->SetBranchAddress("csv_FastBDT", &csv_FastBDT, &b_csv_FastBDT);
   fChain->SetBranchAddress("out_FastBDT1", &out_FastBDT1, &b_out_FastBDT1);
   fChain->SetBranchAddress("csv_FastBDT1", &csv_FastBDT1, &b_csv_FastBDT1);
   fChain->SetBranchAddress("out_FastBDT2", &out_FastBDT2, &b_out_FastBDT2);
   fChain->SetBranchAddress("csv_FastBDT2", &csv_FastBDT2, &b_csv_FastBDT2);
   fChain->SetBranchAddress("out_FastBDT3", &out_FastBDT3, &b_out_FastBDT3);
   fChain->SetBranchAddress("csv_FastBDT3", &csv_FastBDT3, &b_csv_FastBDT3);
   fChain->SetBranchAddress("FBDT_flTag_qr", &FBDT_flTag_qr, &b_FBDT_flTag_qr);
   fChain->SetBranchAddress("FANN_flTag_qr", &FANN_flTag_qr, &b_FANN_flTag_qr);
   fChain->SetBranchAddress("B0_phi_M", &B0_phi_M, &b_B0_phi_M);
   fChain->SetBranchAddress("B0_phi_ErrM", &B0_phi_ErrM, &b_B0_phi_ErrM);
   fChain->SetBranchAddress("B0_phi_SigM", &B0_phi_SigM, &b_B0_phi_SigM);
   fChain->SetBranchAddress("B0_phi_InvM", &B0_phi_InvM, &b_B0_phi_InvM);
   fChain->SetBranchAddress("B0_phi_distance", &B0_phi_distance, &b_B0_phi_distance);
   fChain->SetBranchAddress("B0_phi_significanceOfDistance", &B0_phi_significanceOfDistance, &b_B0_phi_significanceOfDistance);
   fChain->SetBranchAddress("B0_phi_dx", &B0_phi_dx, &b_B0_phi_dx);
   fChain->SetBranchAddress("B0_phi_dy", &B0_phi_dy, &b_B0_phi_dy);
   fChain->SetBranchAddress("B0_phi_dz", &B0_phi_dz, &b_B0_phi_dz);
   fChain->SetBranchAddress("B0_phi_x", &B0_phi_x, &b_B0_phi_x);
   fChain->SetBranchAddress("B0_phi_y", &B0_phi_y, &b_B0_phi_y);
   fChain->SetBranchAddress("B0_phi_z", &B0_phi_z, &b_B0_phi_z);
   fChain->SetBranchAddress("B0_phi_x_uncertainty", &B0_phi_x_uncertainty, &b_B0_phi_x_uncertainty);
   fChain->SetBranchAddress("B0_phi_y_uncertainty", &B0_phi_y_uncertainty, &b_B0_phi_y_uncertainty);
   fChain->SetBranchAddress("B0_phi_z_uncertainty", &B0_phi_z_uncertainty, &b_B0_phi_z_uncertainty);
   fChain->SetBranchAddress("B0_phi_dr", &B0_phi_dr, &b_B0_phi_dr);
   fChain->SetBranchAddress("B0_phi_dphi", &B0_phi_dphi, &b_B0_phi_dphi);
   fChain->SetBranchAddress("B0_phi_dcosTheta", &B0_phi_dcosTheta, &b_B0_phi_dcosTheta);
   fChain->SetBranchAddress("B0_phi_prodVertexX", &B0_phi_prodVertexX, &b_B0_phi_prodVertexX);
   fChain->SetBranchAddress("B0_phi_prodVertexY", &B0_phi_prodVertexY, &b_B0_phi_prodVertexY);
   fChain->SetBranchAddress("B0_phi_prodVertexZ", &B0_phi_prodVertexZ, &b_B0_phi_prodVertexZ);
   fChain->SetBranchAddress("B0_phi_prodVertexXErr", &B0_phi_prodVertexXErr, &b_B0_phi_prodVertexXErr);
   fChain->SetBranchAddress("B0_phi_prodVertexYErr", &B0_phi_prodVertexYErr, &b_B0_phi_prodVertexYErr);
   fChain->SetBranchAddress("B0_phi_prodVertexZErr", &B0_phi_prodVertexZErr, &b_B0_phi_prodVertexZErr);
   fChain->SetBranchAddress("B0_phi_chiProb", &B0_phi_chiProb, &b_B0_phi_chiProb);
   fChain->SetBranchAddress("B0_phi_mcX", &B0_phi_mcX, &b_B0_phi_mcX);
   fChain->SetBranchAddress("B0_phi_mcY", &B0_phi_mcY, &b_B0_phi_mcY);
   fChain->SetBranchAddress("B0_phi_mcZ", &B0_phi_mcZ, &b_B0_phi_mcZ);
   fChain->SetBranchAddress("B0_phi_mcDistance", &B0_phi_mcDistance, &b_B0_phi_mcDistance);
   fChain->SetBranchAddress("B0_phi_mcRho", &B0_phi_mcRho, &b_B0_phi_mcRho);
   fChain->SetBranchAddress("B0_phi_mcProdVertexX", &B0_phi_mcProdVertexX, &b_B0_phi_mcProdVertexX);
   fChain->SetBranchAddress("B0_phi_mcProdVertexY", &B0_phi_mcProdVertexY, &b_B0_phi_mcProdVertexY);
   fChain->SetBranchAddress("B0_phi_mcProdVertexZ", &B0_phi_mcProdVertexZ, &b_B0_phi_mcProdVertexZ);
   fChain->SetBranchAddress("B0_phi_px", &B0_phi_px, &b_B0_phi_px);
   fChain->SetBranchAddress("B0_phi_py", &B0_phi_py, &b_B0_phi_py);
   fChain->SetBranchAddress("B0_phi_pz", &B0_phi_pz, &b_B0_phi_pz);
   fChain->SetBranchAddress("B0_phi_pt", &B0_phi_pt, &b_B0_phi_pt);
   fChain->SetBranchAddress("B0_phi_p", &B0_phi_p, &b_B0_phi_p);
   fChain->SetBranchAddress("B0_phi_E", &B0_phi_E, &b_B0_phi_E);
   fChain->SetBranchAddress("B0_phi_isSignal", &B0_phi_isSignal, &b_B0_phi_isSignal);
   fChain->SetBranchAddress("B0_phi_mcErrors", &B0_phi_mcErrors, &b_B0_phi_mcErrors);
   fChain->SetBranchAddress("B0_phi_mcPDG", &B0_phi_mcPDG, &b_B0_phi_mcPDG);
   fChain->SetBranchAddress("B0_phi_mothID", &B0_phi_mothID, &b_B0_phi_mothID);
   fChain->SetBranchAddress("B0_phi_gdMothID", &B0_phi_gdMothID, &b_B0_phi_gdMothID);
   fChain->SetBranchAddress("B0_phi_gdgdMothID", &B0_phi_gdgdMothID, &b_B0_phi_gdgdMothID);
   fChain->SetBranchAddress("B0_phi_mothPDG", &B0_phi_mothPDG, &b_B0_phi_mothPDG);
   fChain->SetBranchAddress("B0_phi_gdMothPDG", &B0_phi_gdMothPDG, &b_B0_phi_gdMothPDG);
   fChain->SetBranchAddress("B0_phi_gdgdMothPDG", &B0_phi_gdgdMothPDG, &b_B0_phi_gdgdMothPDG);
   fChain->SetBranchAddress("B0_Ks_M", &B0_Ks_M, &b_B0_Ks_M);
   fChain->SetBranchAddress("B0_Ks_ErrM", &B0_Ks_ErrM, &b_B0_Ks_ErrM);
   fChain->SetBranchAddress("B0_Ks_SigM", &B0_Ks_SigM, &b_B0_Ks_SigM);
   fChain->SetBranchAddress("B0_Ks_InvM", &B0_Ks_InvM, &b_B0_Ks_InvM);
   fChain->SetBranchAddress("B0_Ks_distance", &B0_Ks_distance, &b_B0_Ks_distance);
   fChain->SetBranchAddress("B0_Ks_significanceOfDistance", &B0_Ks_significanceOfDistance, &b_B0_Ks_significanceOfDistance);
   fChain->SetBranchAddress("B0_Ks_dx", &B0_Ks_dx, &b_B0_Ks_dx);
   fChain->SetBranchAddress("B0_Ks_dy", &B0_Ks_dy, &b_B0_Ks_dy);
   fChain->SetBranchAddress("B0_Ks_dz", &B0_Ks_dz, &b_B0_Ks_dz);
   fChain->SetBranchAddress("B0_Ks_x", &B0_Ks_x, &b_B0_Ks_x);
   fChain->SetBranchAddress("B0_Ks_y", &B0_Ks_y, &b_B0_Ks_y);
   fChain->SetBranchAddress("B0_Ks_z", &B0_Ks_z, &b_B0_Ks_z);
   fChain->SetBranchAddress("B0_Ks_x_uncertainty", &B0_Ks_x_uncertainty, &b_B0_Ks_x_uncertainty);
   fChain->SetBranchAddress("B0_Ks_y_uncertainty", &B0_Ks_y_uncertainty, &b_B0_Ks_y_uncertainty);
   fChain->SetBranchAddress("B0_Ks_z_uncertainty", &B0_Ks_z_uncertainty, &b_B0_Ks_z_uncertainty);
   fChain->SetBranchAddress("B0_Ks_dr", &B0_Ks_dr, &b_B0_Ks_dr);
   fChain->SetBranchAddress("B0_Ks_dphi", &B0_Ks_dphi, &b_B0_Ks_dphi);
   fChain->SetBranchAddress("B0_Ks_dcosTheta", &B0_Ks_dcosTheta, &b_B0_Ks_dcosTheta);
   fChain->SetBranchAddress("B0_Ks_prodVertexX", &B0_Ks_prodVertexX, &b_B0_Ks_prodVertexX);
   fChain->SetBranchAddress("B0_Ks_prodVertexY", &B0_Ks_prodVertexY, &b_B0_Ks_prodVertexY);
   fChain->SetBranchAddress("B0_Ks_prodVertexZ", &B0_Ks_prodVertexZ, &b_B0_Ks_prodVertexZ);
   fChain->SetBranchAddress("B0_Ks_prodVertexXErr", &B0_Ks_prodVertexXErr, &b_B0_Ks_prodVertexXErr);
   fChain->SetBranchAddress("B0_Ks_prodVertexYErr", &B0_Ks_prodVertexYErr, &b_B0_Ks_prodVertexYErr);
   fChain->SetBranchAddress("B0_Ks_prodVertexZErr", &B0_Ks_prodVertexZErr, &b_B0_Ks_prodVertexZErr);
   fChain->SetBranchAddress("B0_Ks_chiProb", &B0_Ks_chiProb, &b_B0_Ks_chiProb);
   fChain->SetBranchAddress("B0_Ks_mcX", &B0_Ks_mcX, &b_B0_Ks_mcX);
   fChain->SetBranchAddress("B0_Ks_mcY", &B0_Ks_mcY, &b_B0_Ks_mcY);
   fChain->SetBranchAddress("B0_Ks_mcZ", &B0_Ks_mcZ, &b_B0_Ks_mcZ);
   fChain->SetBranchAddress("B0_Ks_mcDistance", &B0_Ks_mcDistance, &b_B0_Ks_mcDistance);
   fChain->SetBranchAddress("B0_Ks_mcRho", &B0_Ks_mcRho, &b_B0_Ks_mcRho);
   fChain->SetBranchAddress("B0_Ks_mcProdVertexX", &B0_Ks_mcProdVertexX, &b_B0_Ks_mcProdVertexX);
   fChain->SetBranchAddress("B0_Ks_mcProdVertexY", &B0_Ks_mcProdVertexY, &b_B0_Ks_mcProdVertexY);
   fChain->SetBranchAddress("B0_Ks_mcProdVertexZ", &B0_Ks_mcProdVertexZ, &b_B0_Ks_mcProdVertexZ);
   fChain->SetBranchAddress("B0_Ks_px", &B0_Ks_px, &b_B0_Ks_px);
   fChain->SetBranchAddress("B0_Ks_py", &B0_Ks_py, &b_B0_Ks_py);
   fChain->SetBranchAddress("B0_Ks_pz", &B0_Ks_pz, &b_B0_Ks_pz);
   fChain->SetBranchAddress("B0_Ks_pt", &B0_Ks_pt, &b_B0_Ks_pt);
   fChain->SetBranchAddress("B0_Ks_p", &B0_Ks_p, &b_B0_Ks_p);
   fChain->SetBranchAddress("B0_Ks_E", &B0_Ks_E, &b_B0_Ks_E);
   fChain->SetBranchAddress("B0_Ks_isSignal", &B0_Ks_isSignal, &b_B0_Ks_isSignal);
   fChain->SetBranchAddress("B0_Ks_mcErrors", &B0_Ks_mcErrors, &b_B0_Ks_mcErrors);
   fChain->SetBranchAddress("B0_Ks_mcPDG", &B0_Ks_mcPDG, &b_B0_Ks_mcPDG);
   fChain->SetBranchAddress("B0_Ks_mothID", &B0_Ks_mothID, &b_B0_Ks_mothID);
   fChain->SetBranchAddress("B0_Ks_gdMothID", &B0_Ks_gdMothID, &b_B0_Ks_gdMothID);
   fChain->SetBranchAddress("B0_Ks_gdgdMothID", &B0_Ks_gdgdMothID, &b_B0_Ks_gdgdMothID);
   fChain->SetBranchAddress("B0_Ks_mothPDG", &B0_Ks_mothPDG, &b_B0_Ks_mothPDG);
   fChain->SetBranchAddress("B0_Ks_gdMothPDG", &B0_Ks_gdMothPDG, &b_B0_Ks_gdMothPDG);
   fChain->SetBranchAddress("B0_Ks_gdgdMothPDG", &B0_Ks_gdgdMothPDG, &b_B0_Ks_gdgdMothPDG);
   fChain->SetBranchAddress("B0_phi_K1_charge", &B0_phi_K1_charge, &b_B0_phi_K1_charge);
   fChain->SetBranchAddress("B0_phi_K1_px", &B0_phi_K1_px, &b_B0_phi_K1_px);
   fChain->SetBranchAddress("B0_phi_K1_py", &B0_phi_K1_py, &b_B0_phi_K1_py);
   fChain->SetBranchAddress("B0_phi_K1_pz", &B0_phi_K1_pz, &b_B0_phi_K1_pz);
   fChain->SetBranchAddress("B0_phi_K1_pt", &B0_phi_K1_pt, &b_B0_phi_K1_pt);
   fChain->SetBranchAddress("B0_phi_K1_p", &B0_phi_K1_p, &b_B0_phi_K1_p);
   fChain->SetBranchAddress("B0_phi_K1_E", &B0_phi_K1_E, &b_B0_phi_K1_E);
   fChain->SetBranchAddress("B0_phi_K1_isSignal", &B0_phi_K1_isSignal, &b_B0_phi_K1_isSignal);
   fChain->SetBranchAddress("B0_phi_K1_mcErrors", &B0_phi_K1_mcErrors, &b_B0_phi_K1_mcErrors);
   fChain->SetBranchAddress("B0_phi_K1_mcPDG", &B0_phi_K1_mcPDG, &b_B0_phi_K1_mcPDG);
   fChain->SetBranchAddress("B0_phi_K1_kaonID", &B0_phi_K1_kaonID, &b_B0_phi_K1_kaonID);
   fChain->SetBranchAddress("B0_phi_K1_kaonBID", &B0_phi_K1_kaonBID, &b_B0_phi_K1_kaonBID);
   fChain->SetBranchAddress("B0_phi_K1_pionID", &B0_phi_K1_pionID, &b_B0_phi_K1_pionID);
   fChain->SetBranchAddress("B0_phi_K1_protonID", &B0_phi_K1_protonID, &b_B0_phi_K1_protonID);
   fChain->SetBranchAddress("B0_phi_K1_muonID", &B0_phi_K1_muonID, &b_B0_phi_K1_muonID);
   fChain->SetBranchAddress("B0_phi_K1_electronID", &B0_phi_K1_electronID, &b_B0_phi_K1_electronID);
   fChain->SetBranchAddress("B0_phi_K1_deuteronID", &B0_phi_K1_deuteronID, &b_B0_phi_K1_deuteronID);
   fChain->SetBranchAddress("B0_phi_K1_mothID", &B0_phi_K1_mothID, &b_B0_phi_K1_mothID);
   fChain->SetBranchAddress("B0_phi_K1_gdMothID", &B0_phi_K1_gdMothID, &b_B0_phi_K1_gdMothID);
   fChain->SetBranchAddress("B0_phi_K1_gdgdMothID", &B0_phi_K1_gdgdMothID, &b_B0_phi_K1_gdgdMothID);
   fChain->SetBranchAddress("B0_phi_K1_mothPDG", &B0_phi_K1_mothPDG, &b_B0_phi_K1_mothPDG);
   fChain->SetBranchAddress("B0_phi_K1_gdMothPDG", &B0_phi_K1_gdMothPDG, &b_B0_phi_K1_gdMothPDG);
   fChain->SetBranchAddress("B0_phi_K1_gdgdMothPDG", &B0_phi_K1_gdgdMothPDG, &b_B0_phi_K1_gdgdMothPDG);
   fChain->SetBranchAddress("B0_phi_K1_dr", &B0_phi_K1_dr, &b_B0_phi_K1_dr);
   fChain->SetBranchAddress("B0_phi_K1_dx", &B0_phi_K1_dx, &b_B0_phi_K1_dx);
   fChain->SetBranchAddress("B0_phi_K1_dy", &B0_phi_K1_dy, &b_B0_phi_K1_dy);
   fChain->SetBranchAddress("B0_phi_K1_dz", &B0_phi_K1_dz, &b_B0_phi_K1_dz);
   fChain->SetBranchAddress("B0_phi_K1_d0", &B0_phi_K1_d0, &b_B0_phi_K1_d0);
   fChain->SetBranchAddress("B0_phi_K1_z0", &B0_phi_K1_z0, &b_B0_phi_K1_z0);
   fChain->SetBranchAddress("B0_phi_K1_pValue", &B0_phi_K1_pValue, &b_B0_phi_K1_pValue);
   fChain->SetBranchAddress("B0_phi_K1_nCDCHits", &B0_phi_K1_nCDCHits, &b_B0_phi_K1_nCDCHits);
   fChain->SetBranchAddress("B0_phi_K1_nPXDHits", &B0_phi_K1_nPXDHits, &b_B0_phi_K1_nPXDHits);
   fChain->SetBranchAddress("B0_phi_K1_nSVDHits", &B0_phi_K1_nSVDHits, &b_B0_phi_K1_nSVDHits);
   fChain->SetBranchAddress("B0_phi_K1_nVXDHits", &B0_phi_K1_nVXDHits, &b_B0_phi_K1_nVXDHits);
   fChain->SetBranchAddress("B0_phi_K2_charge", &B0_phi_K2_charge, &b_B0_phi_K2_charge);
   fChain->SetBranchAddress("B0_phi_K2_px", &B0_phi_K2_px, &b_B0_phi_K2_px);
   fChain->SetBranchAddress("B0_phi_K2_py", &B0_phi_K2_py, &b_B0_phi_K2_py);
   fChain->SetBranchAddress("B0_phi_K2_pz", &B0_phi_K2_pz, &b_B0_phi_K2_pz);
   fChain->SetBranchAddress("B0_phi_K2_pt", &B0_phi_K2_pt, &b_B0_phi_K2_pt);
   fChain->SetBranchAddress("B0_phi_K2_p", &B0_phi_K2_p, &b_B0_phi_K2_p);
   fChain->SetBranchAddress("B0_phi_K2_E", &B0_phi_K2_E, &b_B0_phi_K2_E);
   fChain->SetBranchAddress("B0_phi_K2_isSignal", &B0_phi_K2_isSignal, &b_B0_phi_K2_isSignal);
   fChain->SetBranchAddress("B0_phi_K2_mcErrors", &B0_phi_K2_mcErrors, &b_B0_phi_K2_mcErrors);
   fChain->SetBranchAddress("B0_phi_K2_mcPDG", &B0_phi_K2_mcPDG, &b_B0_phi_K2_mcPDG);
   fChain->SetBranchAddress("B0_phi_K2_kaonID", &B0_phi_K2_kaonID, &b_B0_phi_K2_kaonID);
   fChain->SetBranchAddress("B0_phi_K2_kaonBID", &B0_phi_K2_kaonBID, &b_B0_phi_K2_kaonBID);
   fChain->SetBranchAddress("B0_phi_K2_pionID", &B0_phi_K2_pionID, &b_B0_phi_K2_pionID);
   fChain->SetBranchAddress("B0_phi_K2_protonID", &B0_phi_K2_protonID, &b_B0_phi_K2_protonID);
   fChain->SetBranchAddress("B0_phi_K2_muonID", &B0_phi_K2_muonID, &b_B0_phi_K2_muonID);
   fChain->SetBranchAddress("B0_phi_K2_electronID", &B0_phi_K2_electronID, &b_B0_phi_K2_electronID);
   fChain->SetBranchAddress("B0_phi_K2_deuteronID", &B0_phi_K2_deuteronID, &b_B0_phi_K2_deuteronID);
   fChain->SetBranchAddress("B0_phi_K2_mothID", &B0_phi_K2_mothID, &b_B0_phi_K2_mothID);
   fChain->SetBranchAddress("B0_phi_K2_gdMothID", &B0_phi_K2_gdMothID, &b_B0_phi_K2_gdMothID);
   fChain->SetBranchAddress("B0_phi_K2_gdgdMothID", &B0_phi_K2_gdgdMothID, &b_B0_phi_K2_gdgdMothID);
   fChain->SetBranchAddress("B0_phi_K2_mothPDG", &B0_phi_K2_mothPDG, &b_B0_phi_K2_mothPDG);
   fChain->SetBranchAddress("B0_phi_K2_gdMothPDG", &B0_phi_K2_gdMothPDG, &b_B0_phi_K2_gdMothPDG);
   fChain->SetBranchAddress("B0_phi_K2_gdgdMothPDG", &B0_phi_K2_gdgdMothPDG, &b_B0_phi_K2_gdgdMothPDG);
   fChain->SetBranchAddress("B0_phi_K2_dr", &B0_phi_K2_dr, &b_B0_phi_K2_dr);
   fChain->SetBranchAddress("B0_phi_K2_dx", &B0_phi_K2_dx, &b_B0_phi_K2_dx);
   fChain->SetBranchAddress("B0_phi_K2_dy", &B0_phi_K2_dy, &b_B0_phi_K2_dy);
   fChain->SetBranchAddress("B0_phi_K2_dz", &B0_phi_K2_dz, &b_B0_phi_K2_dz);
   fChain->SetBranchAddress("B0_phi_K2_d0", &B0_phi_K2_d0, &b_B0_phi_K2_d0);
   fChain->SetBranchAddress("B0_phi_K2_z0", &B0_phi_K2_z0, &b_B0_phi_K2_z0);
   fChain->SetBranchAddress("B0_phi_K2_pValue", &B0_phi_K2_pValue, &b_B0_phi_K2_pValue);
   fChain->SetBranchAddress("B0_phi_K2_nCDCHits", &B0_phi_K2_nCDCHits, &b_B0_phi_K2_nCDCHits);
   fChain->SetBranchAddress("B0_phi_K2_nPXDHits", &B0_phi_K2_nPXDHits, &b_B0_phi_K2_nPXDHits);
   fChain->SetBranchAddress("B0_phi_K2_nSVDHits", &B0_phi_K2_nSVDHits, &b_B0_phi_K2_nSVDHits);
   fChain->SetBranchAddress("B0_phi_K2_nVXDHits", &B0_phi_K2_nVXDHits, &b_B0_phi_K2_nVXDHits);
   fChain->SetBranchAddress("B0_Ks_pi1_charge", &B0_Ks_pi1_charge, &b_B0_Ks_pi1_charge);
   fChain->SetBranchAddress("B0_Ks_pi1_px", &B0_Ks_pi1_px, &b_B0_Ks_pi1_px);
   fChain->SetBranchAddress("B0_Ks_pi1_py", &B0_Ks_pi1_py, &b_B0_Ks_pi1_py);
   fChain->SetBranchAddress("B0_Ks_pi1_pz", &B0_Ks_pi1_pz, &b_B0_Ks_pi1_pz);
   fChain->SetBranchAddress("B0_Ks_pi1_pt", &B0_Ks_pi1_pt, &b_B0_Ks_pi1_pt);
   fChain->SetBranchAddress("B0_Ks_pi1_p", &B0_Ks_pi1_p, &b_B0_Ks_pi1_p);
   fChain->SetBranchAddress("B0_Ks_pi1_E", &B0_Ks_pi1_E, &b_B0_Ks_pi1_E);
   fChain->SetBranchAddress("B0_Ks_pi1_isSignal", &B0_Ks_pi1_isSignal, &b_B0_Ks_pi1_isSignal);
   fChain->SetBranchAddress("B0_Ks_pi1_mcErrors", &B0_Ks_pi1_mcErrors, &b_B0_Ks_pi1_mcErrors);
   fChain->SetBranchAddress("B0_Ks_pi1_mcPDG", &B0_Ks_pi1_mcPDG, &b_B0_Ks_pi1_mcPDG);
   fChain->SetBranchAddress("B0_Ks_pi1_kaonID", &B0_Ks_pi1_kaonID, &b_B0_Ks_pi1_kaonID);
   fChain->SetBranchAddress("B0_Ks_pi1_kaonBID", &B0_Ks_pi1_kaonBID, &b_B0_Ks_pi1_kaonBID);
   fChain->SetBranchAddress("B0_Ks_pi1_pionID", &B0_Ks_pi1_pionID, &b_B0_Ks_pi1_pionID);
   fChain->SetBranchAddress("B0_Ks_pi1_protonID", &B0_Ks_pi1_protonID, &b_B0_Ks_pi1_protonID);
   fChain->SetBranchAddress("B0_Ks_pi1_muonID", &B0_Ks_pi1_muonID, &b_B0_Ks_pi1_muonID);
   fChain->SetBranchAddress("B0_Ks_pi1_electronID", &B0_Ks_pi1_electronID, &b_B0_Ks_pi1_electronID);
   fChain->SetBranchAddress("B0_Ks_pi1_deuteronID", &B0_Ks_pi1_deuteronID, &b_B0_Ks_pi1_deuteronID);
   fChain->SetBranchAddress("B0_Ks_pi1_mothID", &B0_Ks_pi1_mothID, &b_B0_Ks_pi1_mothID);
   fChain->SetBranchAddress("B0_Ks_pi1_gdMothID", &B0_Ks_pi1_gdMothID, &b_B0_Ks_pi1_gdMothID);
   fChain->SetBranchAddress("B0_Ks_pi1_gdgdMothID", &B0_Ks_pi1_gdgdMothID, &b_B0_Ks_pi1_gdgdMothID);
   fChain->SetBranchAddress("B0_Ks_pi1_mothPDG", &B0_Ks_pi1_mothPDG, &b_B0_Ks_pi1_mothPDG);
   fChain->SetBranchAddress("B0_Ks_pi1_gdMothPDG", &B0_Ks_pi1_gdMothPDG, &b_B0_Ks_pi1_gdMothPDG);
   fChain->SetBranchAddress("B0_Ks_pi1_gdgdMothPDG", &B0_Ks_pi1_gdgdMothPDG, &b_B0_Ks_pi1_gdgdMothPDG);
   fChain->SetBranchAddress("B0_Ks_pi1_dr", &B0_Ks_pi1_dr, &b_B0_Ks_pi1_dr);
   fChain->SetBranchAddress("B0_Ks_pi1_dx", &B0_Ks_pi1_dx, &b_B0_Ks_pi1_dx);
   fChain->SetBranchAddress("B0_Ks_pi1_dy", &B0_Ks_pi1_dy, &b_B0_Ks_pi1_dy);
   fChain->SetBranchAddress("B0_Ks_pi1_dz", &B0_Ks_pi1_dz, &b_B0_Ks_pi1_dz);
   fChain->SetBranchAddress("B0_Ks_pi1_d0", &B0_Ks_pi1_d0, &b_B0_Ks_pi1_d0);
   fChain->SetBranchAddress("B0_Ks_pi1_z0", &B0_Ks_pi1_z0, &b_B0_Ks_pi1_z0);
   fChain->SetBranchAddress("B0_Ks_pi1_pValue", &B0_Ks_pi1_pValue, &b_B0_Ks_pi1_pValue);
   fChain->SetBranchAddress("B0_Ks_pi1_nCDCHits", &B0_Ks_pi1_nCDCHits, &b_B0_Ks_pi1_nCDCHits);
   fChain->SetBranchAddress("B0_Ks_pi1_nPXDHits", &B0_Ks_pi1_nPXDHits, &b_B0_Ks_pi1_nPXDHits);
   fChain->SetBranchAddress("B0_Ks_pi1_nSVDHits", &B0_Ks_pi1_nSVDHits, &b_B0_Ks_pi1_nSVDHits);
   fChain->SetBranchAddress("B0_Ks_pi1_nVXDHits", &B0_Ks_pi1_nVXDHits, &b_B0_Ks_pi1_nVXDHits);
   fChain->SetBranchAddress("B0_Ks_pi2_charge", &B0_Ks_pi2_charge, &b_B0_Ks_pi2_charge);
   fChain->SetBranchAddress("B0_Ks_pi2_px", &B0_Ks_pi2_px, &b_B0_Ks_pi2_px);
   fChain->SetBranchAddress("B0_Ks_pi2_py", &B0_Ks_pi2_py, &b_B0_Ks_pi2_py);
   fChain->SetBranchAddress("B0_Ks_pi2_pz", &B0_Ks_pi2_pz, &b_B0_Ks_pi2_pz);
   fChain->SetBranchAddress("B0_Ks_pi2_pt", &B0_Ks_pi2_pt, &b_B0_Ks_pi2_pt);
   fChain->SetBranchAddress("B0_Ks_pi2_p", &B0_Ks_pi2_p, &b_B0_Ks_pi2_p);
   fChain->SetBranchAddress("B0_Ks_pi2_E", &B0_Ks_pi2_E, &b_B0_Ks_pi2_E);
   fChain->SetBranchAddress("B0_Ks_pi2_isSignal", &B0_Ks_pi2_isSignal, &b_B0_Ks_pi2_isSignal);
   fChain->SetBranchAddress("B0_Ks_pi2_mcErrors", &B0_Ks_pi2_mcErrors, &b_B0_Ks_pi2_mcErrors);
   fChain->SetBranchAddress("B0_Ks_pi2_mcPDG", &B0_Ks_pi2_mcPDG, &b_B0_Ks_pi2_mcPDG);
   fChain->SetBranchAddress("B0_Ks_pi2_kaonID", &B0_Ks_pi2_kaonID, &b_B0_Ks_pi2_kaonID);
   fChain->SetBranchAddress("B0_Ks_pi2_kaonBID", &B0_Ks_pi2_kaonBID, &b_B0_Ks_pi2_kaonBID);
   fChain->SetBranchAddress("B0_Ks_pi2_pionID", &B0_Ks_pi2_pionID, &b_B0_Ks_pi2_pionID);
   fChain->SetBranchAddress("B0_Ks_pi2_protonID", &B0_Ks_pi2_protonID, &b_B0_Ks_pi2_protonID);
   fChain->SetBranchAddress("B0_Ks_pi2_muonID", &B0_Ks_pi2_muonID, &b_B0_Ks_pi2_muonID);
   fChain->SetBranchAddress("B0_Ks_pi2_electronID", &B0_Ks_pi2_electronID, &b_B0_Ks_pi2_electronID);
   fChain->SetBranchAddress("B0_Ks_pi2_deuteronID", &B0_Ks_pi2_deuteronID, &b_B0_Ks_pi2_deuteronID);
   fChain->SetBranchAddress("B0_Ks_pi2_mothID", &B0_Ks_pi2_mothID, &b_B0_Ks_pi2_mothID);
   fChain->SetBranchAddress("B0_Ks_pi2_gdMothID", &B0_Ks_pi2_gdMothID, &b_B0_Ks_pi2_gdMothID);
   fChain->SetBranchAddress("B0_Ks_pi2_gdgdMothID", &B0_Ks_pi2_gdgdMothID, &b_B0_Ks_pi2_gdgdMothID);
   fChain->SetBranchAddress("B0_Ks_pi2_mothPDG", &B0_Ks_pi2_mothPDG, &b_B0_Ks_pi2_mothPDG);
   fChain->SetBranchAddress("B0_Ks_pi2_gdMothPDG", &B0_Ks_pi2_gdMothPDG, &b_B0_Ks_pi2_gdMothPDG);
   fChain->SetBranchAddress("B0_Ks_pi2_gdgdMothPDG", &B0_Ks_pi2_gdgdMothPDG, &b_B0_Ks_pi2_gdgdMothPDG);
   fChain->SetBranchAddress("B0_Ks_pi2_dr", &B0_Ks_pi2_dr, &b_B0_Ks_pi2_dr);
   fChain->SetBranchAddress("B0_Ks_pi2_dx", &B0_Ks_pi2_dx, &b_B0_Ks_pi2_dx);
   fChain->SetBranchAddress("B0_Ks_pi2_dy", &B0_Ks_pi2_dy, &b_B0_Ks_pi2_dy);
   fChain->SetBranchAddress("B0_Ks_pi2_dz", &B0_Ks_pi2_dz, &b_B0_Ks_pi2_dz);
   fChain->SetBranchAddress("B0_Ks_pi2_d0", &B0_Ks_pi2_d0, &b_B0_Ks_pi2_d0);
   fChain->SetBranchAddress("B0_Ks_pi2_z0", &B0_Ks_pi2_z0, &b_B0_Ks_pi2_z0);
   fChain->SetBranchAddress("B0_Ks_pi2_pValue", &B0_Ks_pi2_pValue, &b_B0_Ks_pi2_pValue);
   fChain->SetBranchAddress("B0_Ks_pi2_nCDCHits", &B0_Ks_pi2_nCDCHits, &b_B0_Ks_pi2_nCDCHits);
   fChain->SetBranchAddress("B0_Ks_pi2_nPXDHits", &B0_Ks_pi2_nPXDHits, &b_B0_Ks_pi2_nPXDHits);
   fChain->SetBranchAddress("B0_Ks_pi2_nSVDHits", &B0_Ks_pi2_nSVDHits, &b_B0_Ks_pi2_nSVDHits);
   fChain->SetBranchAddress("B0_Ks_pi2_nVXDHits", &B0_Ks_pi2_nVXDHits, &b_B0_Ks_pi2_nVXDHits);
   fChain->SetBranchAddress("isNonRes", &isNonRes, &b_isNonRes);
   fChain->SetBranchAddress("customIsSig", &customIsSig, &b_customIsSig);
   Notify();
}

void PhiKsAna::InitializeHistos(TString hLabel="")
{

  TString hNam = "hNCand";
  hNCand = new TH1F(hNam, "", 10, 0.5, 10.5);
  
  
  hNam = "hMBc";
  hNam += hLabel;
  hMbc = new TH1F(hNam, "", nBinsMbc, lowMbc, highMbc);

  hNam = "hDeltaE";
  hNam += hLabel;
  hDeltaE = new TH1F(hNam, "", nBinsDeltaE, lowDeltaE, highDeltaE);

  hNam = "hMPhi";
  hNam += hLabel;
  hMPhi = new TH1F(hNam, "", nBinsMPhi, lowMPhi, highMPhi);

  hNam = "hMKst";
  hNam += hLabel;
  hMKst = new TH1F(hNam, "", nBinsMKst, lowMKst, highMKst);

  hNam = "hPhiHel";
  hNam += hLabel;
  hPhiHel = new TH1F(hNam, "", nBinsPhiHel, lowPhiHel, highPhiHel);

  hNam = "hKstHel";
  hNam += hLabel;
  hKstHel = new TH1F(hNam, "", nBinsKstHel, lowKstHel, highKstHel);

  hNam = "hFBDT";
  hNam += hLabel;
  hFBDT = new TH1F(hNam, "", nBinsFBDT, lowFBDT, highFBDT);

}


Bool_t PhiKsAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PhiKsAna::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PhiKsAna::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef PhiKsAna_cxx
