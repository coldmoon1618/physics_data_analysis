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
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit;

void Ks_fit(){
    const int nCuts = 10;

    ofstream myfile;
    myfile.open("fitResults.txt");
    double cuts[nCuts] = {-999 , .5, 1.5, 2.5, 3., 3.5, 4., 4.5, 5.,6.};
    TFile fOut("sigL_Rej_Eff.root","recreate");
    TTree *tree_2D = new TTree("tree_2D","sigL2DEffTree");
    TTree *tree_3D = new TTree("tree_3D","sigL3DEffTree");
    double L2DCut = -999;
    double L3DCut = -999;
    float nsig0_2D;
    float dnsig_2D;
    float nbkg0_2D;
    float dnbkg_2D;
    float nsig0_3D;
    float dnsig_3D;
    float nbkg0_3D;
    float dnbkg_3D;
    tree_2D -> Branch("L2DCut",&L2DCut,"L2DCut/D");
    tree_2D -> Branch("nsig0_2D",&nsig0_2D,"nsig0_2D/F");
    tree_2D -> Branch("dnsig_2D",&dnsig_2D,"dnsig_2D/F");
    tree_2D -> Branch("nbkg0_2D",&nbkg0_2D,"nbkg0_2D/F");
    tree_2D -> Branch("dnbkg_2D",&dnbkg_2D,"dnbkg_2D/F");
    tree_3D -> Branch("L3DCut",&L3DCut,"L3DCut/D");
    tree_3D -> Branch("nsig0_3D",&nsig0_3D,"nsig0_3D/F");
    tree_3D -> Branch("dnsig_3D",&dnsig_3D,"dnsig_3D/F");
    tree_3D -> Branch("nbkg0_3D",&nbkg0_3D,"nbkg0_3D/F");
    tree_3D -> Branch("dnbkg_3D",&dnbkg_3D,"dnbkg_3D/F");

    TString fInName("Combo_sigL_cut.root");

    for(int iL = 2; iL<=3; iL++){
        myfile<<iL<<"D cuts"<<endl;
        TCanvas *c_M = new TCanvas("c_M","c_M",1000,2000);
        c_M -> Divide(2,5);

        for(int iCut = 0; iCut< nCuts; iCut++) {
            RooRealVar M("M","M",.46,.54);

            RooDataSet data("data","MC data",M, ImportFromFile(fInName,Form("tree_%iD_%i",iL,iCut)));

            // Plot 
            M.setBins(200);
            RooPlot *frame = M.frame(Title(Form("Lxy_cut_%i",iCut)));
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
            RooRealVar nsig("nsig","nsig",2e6,0,1e7);
            RooRealVar nbkg("nbkg","nbkg",2e6,0,1e7);
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

            myfile<<iCut<<endl;
            myfile<<"\tnsig=\t"<<nsig.getVal()<<"\t+/-\t"<<nsig.getError()<<endl;
            myfile<<"\tnbkg=\t"<<nbkg.getVal()<<"\t+/-\t"<<nbkg.getError()<<endl;
            myfile<<endl;

            c_M -> cd(iCut+1);
            frame -> Draw();

            if(iL==2){
                L2DCut = cuts[iCut];
                nsig0_2D = nsig.getVal();
                dnsig_2D = nsig.getError();
                nbkg0_2D = nbkg.getVal();
                dnbkg_2D = nbkg.getError();
                tree_2D -> Fill();
            }else{
                L3DCut = cuts[iCut];
                nsig0_3D = nsig.getVal();
                dnsig_3D = nsig.getError();
                nbkg0_3D = nbkg.getVal();
                dnbkg_3D = nbkg.getError();
                tree_3D -> Fill();
            }
        }//iCuts
        if(iL==2)c_M -> SaveAs("Ksp_fit_2D.pdf");
        if(iL==3)c_M -> SaveAs("Ksp_fit_3D.pdf");
    }//2D vs 3D

    fOut.cd();
    tree_2D -> Print();
    tree_3D -> Print();
    tree_2D -> Write();
    tree_3D -> Write();
    fOut.Close();


    return ;
}
