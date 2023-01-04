#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

#include <TChain.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>

//RooFit
#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit;

void PhiRecDistr(){
    const int nTrees = 12;
    const char *tLoP[nTrees] = {"tPre_LoP_ind",
                                "tVtx_LoP_ind",
                                "tCdc_LoP_ind",
                                "tHit_LoP_ind",
                                "tKID_LoP_ind",
                                "tMKK_LoP_ind",
                                "tPre_LoP_acc",
                                "tVtx_LoP_acc",
                                "tCdc_LoP_acc",
                                "tHit_LoP_acc",
                                "tKID_LoP_acc",
                                "tMKK_LoP_acc"};
    const char *tHiP[nTrees] = {"tPre_HiP_ind",
                                "tVtx_HiP_ind",
                                "tCdc_HiP_ind",
                                "tHit_HiP_ind",
                                "tKID_HiP_ind",
                                "tMKK_HiP_ind",
                                "tPre_HiP_acc",
                                "tVtx_HiP_acc",
                                "tCdc_HiP_acc",
                                "tHit_HiP_acc",
                                "tKID_HiP_acc",
                                "tMKK_HiP_acc"};

    for(int iF = 0; iF < 2; iF++){
        for(int iP = 0; iP < 2; iP++){

            TCanvas *c_M = new TCanvas("c_M","c_M",1500,1600);
            c_M -> Divide(3,4,.01,.01);

            for(int iTree = 0; iTree< nTrees; iTree++)
            {
                RooRealVar M("M","M",.97,1.06);
                TString treeName("");
                TString fileName("");
                if(iP==0) treeName = tLoP[iTree];
                if(iP==1) treeName = tHiP[iTree];
                if(iF==0) fileName = "PhiRec.root";
                if(iF==1) fileName = "PhiMat.root";
                RooDataSet data("data","MC data",M, ImportFromFile(fileName,treeName));

                // Plot 
                if(iP==0)M.setBins(50);
                if(iP==1)M.setBins(150);

                RooPlot *frame = M.frame(Title(tLoP[iTree]));
                data.plotOn(frame);

                c_M -> cd(iTree+1);
                frame -> Draw();
            }
            if(iF==0){
                if(iP==0)c_M -> SaveAs("SigMCPhiRecLoP.pdf");
                if(iP==1)c_M -> SaveAs("SigMCPhiRecHiP.pdf");
            }else{
                if(iP==0)c_M -> SaveAs("SigMCPhiMatLoP.pdf");
                if(iP==1)c_M -> SaveAs("SigMCPhiMatHiP.pdf");
            }
        }//momentum
    }//files
    
    return ;
}
