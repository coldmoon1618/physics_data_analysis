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

void KsRecPdep_fit(){
    const int nTrees = 8;
    const char *treeName[nTrees] = {"tPre_LoP_ind",
                                    "tVtx_LoP_ind",
                                    "tGbk_LoP_ind",
                                    "tGbk_LoP_acc",
                                    "tPre_HiP_ind",
                                    "tVtx_HiP_ind",
                                    "tGbk_HiP_ind",
                                    "tGbk_HiP_acc"};

    TCanvas *c_M = new TCanvas("c_M","c_M",4000,1600);
    c_M -> Divide(4,2);

    for(int isMat = 0; isMat<2; isMat++){

        ofstream myfile;
        if(isMat)   myfile.open("KsMatPdep_fitResults.txt");
        else        myfile.open("KsRecPdep_fitResults.txt");

        TString fInName("");
        if(isMat)   fInName+="KsMatPdep_toFit.root";
        else        fInName+="KsRecPdep_toFit.root";

        for(int iTree = 0; iTree< nTrees; iTree++)
        {
            RooRealVar M("M","M",.46,.54);

            RooDataSet data("data","MC data",M, ImportFromFile(fInName,treeName[iTree]));

            // Plot 
            M.setBins(200);
            RooPlot *frame = M.frame(Title(treeName[iTree]));
            data.plotOn(frame, MarkerSize(1));

            // Signal double Gaussian
            RooRealVar mean("mean","mean of signal", .49,.47,.50);
            RooRealVar sigma1("sigma1","sigma1",.0012,.0001,.0018);
            RooRealVar sigma2("sigma2","sigma2",.0045,.003,.0049);
            RooGaussian sig1("sig1","sig1",M,mean,sigma1);
            RooGaussian sig2("sig2","sig2",M,mean,sigma2);
            RooRealVar frac1("frac1","frac1",.5,0,1);
            RooRealVar frac2("frac2","frac2",.5,0,1);
            RooAddPdf signal("signal","sig1+sig2",RooArgList(sig1,sig2),RooArgList(frac1,frac2));

            // Background
            RooRealVar c0("c0","c0",1.0,-1.,1.);
            //RooRealVar c1("c1","c1",0.1,-1.,1.);
            //RooRealVar c2("c2","c2",-.1,-1.,1.);
            RooChebychev background("background","background",M,RooArgList(c0));

            // Add signal and bakcground
            RooRealVar nsig("nsig","nsig", 4000,0,1e8);
            RooRealVar nbkg("nbkg","nbkg",40000,0,1e8);
            RooAddPdf model("model","sig+bkg",RooArgList(signal,background),RooArgList(nsig,nbkg));

            // Fitting
            M.setRange("bkg1",.460,.485);
            M.setRange("bkg2",.515,.535);
            M.setRange("peak",.485,.515);

            background.fitTo(data,Range("bkg1","bkg2"));
            signal.fitTo(data, Range("peak"));
            model.fitTo(data, Range("Full"));
            model.plotOn(frame, Range("Full"), Components(background), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
            model.plotOn(frame, Range("Full"),LineWidth(1));
            model.plotOn(frame, Range("Full"), Components(signal), DrawOption("F"), FillColor(kOrange), MoveToBack());

            sig1.paramOn(frame, Layout(.1,.3,.9));
            frame->getAttText()->SetTextSize(.02);
            sig2.paramOn(frame, Layout(.1,.3,.78));
            frame->getAttText()->SetTextSize(.02);

            myfile<<treeName[iTree]<<endl;
            myfile<<"\tnsig=\t"<<nsig.getVal()<<"\t+/-\t"<<nsig.getError()<<endl;
            myfile<<"\tnbkg=\t"<<nbkg.getVal()<<"\t+/-\t"<<nbkg.getError()<<endl;
            myfile<<endl;

            c_M -> cd(iTree+1);
            frame -> Draw();
            //gPad -> SetLogy();
        }
        if(isMat)   c_M -> SaveAs("KsMatPdep_fit.pdf");
        else        c_M -> SaveAs("KsRecPdep_fit.pdf");
    }
    
    return ;
}
