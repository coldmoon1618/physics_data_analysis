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

void FitMbcBDTcut(  const char* fInType = "FullMC", //"SigMC"
        float deEcut = .2, float BDTcut = -9, 
        bool inclSig = 0, bool inclB0s = 0, bool inclBBs = 0, bool inclQQs = 1,
        bool useArgus= 1, float fixCoef = -999, float fixThres = -999, 
        bool useGaus = 0, float fixMean = -999, float fixSigma = -999,
        int nBins = 100, float lowBound = 5.2){

    TString fInName(Form("%s_resp.root",fInType));

    // Variables
    RooRealVar Mbc("Mbc","",lowBound,5.3);
    RooRealVar deltaE("deltaE","",-.2,.2);
    RooRealVar response("response","",-1,1);
    RooDataSet Mbc_data("Mbc_data","",RooArgSet(Mbc, deltaE, response)); 

    if(inclSig){
        cout<<"Including signal channel..."<<endl;
        RooDataSet SigMC_data("SigMC_data","",RooArgSet(Mbc,deltaE,response), ImportFromFile(fInName, "SigMC"),Cut(Form("abs(deltaE)<%f&&response>%f",deEcut,BDTcut)));
        Mbc_data.append(SigMC_data);
    }
    if(inclB0s){
        cout<<"Including B0B0bar bkg..."<<endl;
        RooDataSet B0bkg_data("B0bkg_data","",RooArgSet(Mbc,deltaE,response), ImportFromFile(fInName, "B0bkg"),Cut(Form("abs(deltaE)<%f&&response>%f",deEcut,BDTcut)));
        Mbc_data.append(B0bkg_data);
    }
    if(inclBBs){
        cout<<"Including   B+B-  bkg..."<<endl;
        RooDataSet BBbkg_data("BBbkg_data","",RooArgSet(Mbc,deltaE,response), ImportFromFile(fInName, "BBbkg"),Cut(Form("abs(deltaE)<%f&&response>%f",deEcut,BDTcut)));
        Mbc_data.append(BBbkg_data);
    }
    if(inclQQs){
        cout<<"Including  continuum ..."<<endl;
        RooDataSet QQbkg_data("QQbkg_data","",RooArgSet(Mbc,deltaE,response), ImportFromFile(fInName, "QQbkg"),Cut(Form("abs(deltaE)<%f&&response>%f",deEcut,BDTcut)));
        Mbc_data.append(QQbkg_data);
    }
    cout<<endl;

    Mbc.setBins(nBins);
    RooPlot *frame = Mbc.frame(Title(Form("M_{bc}[%s,|deE|<%.3f,bdt>%.1f,signal(%i),B0s(%i),BBs(%i),QQs(%i)]", fInType,deEcut,BDTcut,inclSig,inclB0s,inclBBs,inclQQs)));
    Mbc_data.plotOn(frame);

    // D e f i n e   s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar *mean;
    if(fixMean  ==-999 && useGaus){
        mean = new RooRealVar("mean", "", 5.28,5.277,5.283);
    }else{
        mean = new RooRealVar("mean", "", fixMean);
    }
    RooRealVar *sigma;
    if(fixSigma ==-999 && useGaus){
        sigma = new RooRealVar("sigma", "", 0.0023,0.,0.2);
    }else{
        sigma = new RooRealVar("sigma", "", fixSigma);
    }
    RooGaussian sig("sig", "", Mbc, *mean, *sigma);

    RooRealVar *thres;
    if(fixThres ==-999){
        thres = new RooRealVar("thres","",5.29,5.28,5.3);
    }else{
        thres = new RooRealVar("thres","",fixThres);
    }
    RooRealVar *coef;
    if(fixCoef  ==-999){
        coef = new RooRealVar("coef","",-40,-1e3,1e3);
    }else{
        coef = new RooRealVar("coef","",fixCoef);
    }
    RooArgusBG bkg("bkg","bkg",Mbc,*thres,*coef);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------
    RooRealVar nsig("nsig", "", 3e1, 0, 3e5);
    RooRealVar nbkg("nbkg", "", 1e2, 0, 5e4);

    RooAddPdf *model;
    if(useGaus){
        cout<<endl<<"Model = Gauss + Argus"<<endl;
        model = new RooAddPdf("model","sig+bkg",RooArgList(sig,bkg),RooArgList(nsig,nbkg));
    }

    Mbc.setRange("signal", 5.275, 5.285);
    Mbc.setRange("sband1",lowBound,5.275);
    Mbc.setRange("sband2",5.287,5.3);
    Mbc.setRange("plotRange",lowBound,5.3);

    // Fiting and config plots
    if          ( useGaus  &&  useArgus ){
        bkg.fitTo    (Mbc_data, Range("sband1","sband2"));
        sig.fitTo    (Mbc_data, Range("signal"));
        model->fitTo (Mbc_data, Range("plotRange"));
        model->plotOn(frame,    Range("plotRange"), Components(bkg), LineStyle(kDashed), LineColor(kRed), LineWidth(1));
        model->plotOn(frame,    Range("plotRange"));
        model->plotOn(frame,    Range("plotRange"), Components(sig), DrawOption("F"), FillColor(kOrange), MoveToBack());

        //parameters in plot
        sig.paramOn  (frame,    Layout(0.1,0.4,0.9));
        bkg.paramOn  (frame,    Layout(0.1,0.4,0.78));
    }else if    (!useGaus  &&  useArgus ){
        //bkg.fitTo    (Mbc_data, Range("sband1","sband2"));
        bkg.fitTo    (Mbc_data);
        bkg.plotOn   (frame,    Range("plotRange"));
        bkg.paramOn  (frame,    Layout(0.1,0.4,0.78));
    }else if    ( useGaus  && !useArgus){
        sig.fitTo    (Mbc_data, Range("signal"));
        sig.plotOn   (frame,    Range("plotRange"));
        sig.paramOn  (frame,    Layout(0.1,0.4,0.9));
    }

    cout<<endl<<endl;
    cout<<"=====----- CURRENT SETUP -----====="<<endl;
    cout<<"Input type:  "<< fInType <<endl;
    cout<<endl;
    cout<<"deltaE width="<< deEcut  <<endl;
    cout<<"BDT_response>"<< BDTcut  <<endl;
    cout<<endl;
    cout<<"Included components:"<<endl;
    if(inclSig)cout<<"    Signal  "<<endl;
    if(inclB0s)cout<<" B0B0bar bkg"<<endl;
    if(inclBBs)cout<<"  B+B-   bkg"<<endl;
    if(inclQQs)cout<<"Continuum bkg"<<endl;
    cout<<endl;
    if(useArgus){
        cout<<"Using Argus"<<endl;
        if(fixCoef !=-999)cout<<"Fixing Argus coef  to "<<fixCoef<<endl;
        if(fixThres!=-999)cout<<"Fixing Argus thres to "<<fixThres<<endl;
    }
    if(useGaus){
        cout<<"Using Gauss"<<endl;
        if(fixMean !=-999)cout<<"Fixing Gauss mean  to "<<fixMean<<endl;
        if(fixSigma!=-999)cout<<"Fixing Gauss sigma to "<<fixSigma<<endl;
    }
    cout<<endl;
    cout<<"Binning:"<<endl;
    cout<<nBins<<" bins in ("<<lowBound<<",5.3)"<<endl;
    cout<<endl;

    int nParams;
    if          ( useGaus  &&  useArgus ){
        nParams = 2;//nsig, nbkg
        if(fixMean  ==-999)nParams++;//mean
        if(fixSigma ==-999)nParams++;//sigma
        if(fixCoef  ==-999)nParams++;//coef
        if(fixThres ==-999)nParams++;//thres
    }else if    (!useGaus  &&  useArgus ){
        nParams = 0;
        if(fixCoef  ==-999)nParams++;//coef
        if(fixThres ==-999)nParams++;//thres
    }else if    ( useGaus  && !useArgus){
        nParams = 0;
        if(fixMean  ==-999)nParams++;//mean
        if(fixSigma ==-999)nParams++;//sigma
    }
    cout<<nParams<<" fit parameters"<<endl<<endl;

    cout<<"=====----- RESULTS  -----====="<<endl;
    // Print parameters in terminal
    if          ( useGaus  &&  useArgus ){
        nsig.Print();
        cout<<"mean  = "<<  mean->getValV()<<endl;
        cout<<"sigma = "<< sigma->getValV()<<endl;
        cout<<"coef  = "<<  coef->getValV()<<endl;
        cout<<"thres = "<< thres->getValV()<<endl;
    }else if    (!useGaus  &&  useArgus ){
        cout<<"coef  = "<<  coef->getValV()<<endl;
        cout<<"thres = "<< thres->getValV()<<endl;
    }else if    ( useGaus  && !useArgus){
        cout<<"mean  = "<<  mean->getValV()<<endl;
        cout<<"sigma = "<< sigma->getValV()<<endl;
    }
    float chi2 =frame->chiSquare();
    float ratio, nPeff;
    if(useArgus){
        ratio = (lowBound-thres->getValV())/(lowBound-5.3);
        nPeff = floor(ratio*nBins);
        cout<<"chiProb="<< TMath::Prob(chi2*nBins,nPeff-nParams);
    }else{
        //Mbc.setRange("signal", 5.275, 5.285);
        ratio = (5.275-5.285)/(lowBound-5.3);
        nPeff = floor(ratio*nBins);
        cout<<nPeff<<"effective nBins"<<endl;
        cout<<"chiProb="<< TMath::Prob(chi2*nPeff,nPeff-nParams);
    }
    cout<<endl;

    cout<<"=====----- FORMAT  -----====="<<endl;
    cout<<".x FitMbcBDTcut.C(fInType, deEcut,BDTcut, inclSig,inclB0s,inclBBs,inclQQs, useArgus,fixCoef,fixThres, useGaus,fixMean,fixWidth, nBins,lowBound)"<<endl;
    cout<<endl;

    // Show plot
    TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
    frame -> Draw();
    
    /*
    ///////////////////////// Read-In from ntuple ///////////////
    // get number of events                                 ////
    cout<<"=====----- INPUT INFO  -----====="<<endl;
    TChain  *SigMC = new TChain("SigMC");
    SigMC   -> AddFile(fInName);
    int     nSigMC = SigMC->GetEntries();
    float SigMC_Mbc;
    float SigMC_deE;
    float SigMC_res;
    SigMC   -> SetBranchAddress("Mbc",      &SigMC_Mbc);
    SigMC   -> SetBranchAddress("deltaE",   &SigMC_deE);
    SigMC   -> SetBranchAddress("response", &SigMC_res);
    int     N_SigMC = 0;//num passed cuts

    for(int i=0; i<nSigMC; i++){
        SigMC -> GetEntry(i);

        if(SigMC_Mbc<=lowBound||SigMC_Mbc>=5.3)continue;
        if(abs(SigMC_deE)>=deEcut)continue;
        if(SigMC_res<=BDTcut)continue;

        N_SigMC++;
    }
    if(inclSig){
        cout<<"Out of "<<nSigMC<<" entries, ";
        cout<<N_SigMC<<" SigMC passed cuts"<<endl;
    }

    TChain  *B0bkg = new TChain("B0bkg");
    B0bkg   -> AddFile(fInName);
    int     nB0bkg = B0bkg->GetEntries();
    float B0bkg_Mbc;
    float B0bkg_deE;
    float B0bkg_res;
    B0bkg   -> SetBranchAddress("Mbc",      &B0bkg_Mbc);
    B0bkg   -> SetBranchAddress("deltaE",   &B0bkg_deE);
    B0bkg   -> SetBranchAddress("response", &B0bkg_res);
    int     N_B0bkg = 0;//num passed cuts

    for(int i=0; i<nB0bkg; i++){
        B0bkg -> GetEntry(i);

        if(B0bkg_Mbc<=lowBound||B0bkg_Mbc>=5.3)continue;
        if(abs(B0bkg_deE)>=deEcut)continue;
        if(B0bkg_res<=BDTcut)continue;

        N_B0bkg++;
    }
    if(inclB0s){
        cout<<"Out of "<<nB0bkg<<" entries, ";
        cout<<N_B0bkg<<" B0bkg passed cuts"<<endl;
    }

    TChain  *BBbkg = new TChain("BBbkg");
    BBbkg   -> AddFile(fInName);
    int     nBBbkg = BBbkg->GetEntries();
    float BBbkg_Mbc;
    float BBbkg_deE;
    float BBbkg_res;
    BBbkg   -> SetBranchAddress("Mbc",      &BBbkg_Mbc);
    BBbkg   -> SetBranchAddress("deltaE",   &BBbkg_deE);
    BBbkg   -> SetBranchAddress("response", &BBbkg_res);
    int     N_BBbkg = 0;//num passed cuts

    for(int i=0; i<nBBbkg; i++){
        BBbkg -> GetEntry(i);

        if(BBbkg_Mbc<=lowBound||BBbkg_Mbc>=5.3)continue;
        if(abs(BBbkg_deE)>=deEcut)continue;
        if(BBbkg_res<=BDTcut)continue;

        N_BBbkg++;
    }
    if(inclBBs){
        cout<<"Out of "<<nBBbkg<<" entries, ";
        cout<<N_BBbkg<<" BBbkg passed cuts"<<endl;
    }

    TChain  *QQbkg = new TChain("QQbkg");
    QQbkg   -> AddFile(fInName);
    int     nQQbkg = QQbkg->GetEntries();
    float QQbkg_Mbc;
    float QQbkg_deE;
    float QQbkg_res;
    QQbkg   -> SetBranchAddress("Mbc",      &QQbkg_Mbc);
    QQbkg   -> SetBranchAddress("deltaE",   &QQbkg_deE);
    QQbkg   -> SetBranchAddress("response", &QQbkg_res);
    int     N_QQbkg = 0;//num passed cuts

    for(int i=0; i<nQQbkg; i++){
        QQbkg -> GetEntry(i);

        if(QQbkg_Mbc<=lowBound||QQbkg_Mbc>=5.3)continue;
        if(abs(QQbkg_deE)>=deEcut)continue;
        if(QQbkg_res<=BDTcut)continue;

        N_QQbkg++;
    }
    if(inclQQs){
        cout<<"Out of "<<nQQbkg<<" entries, ";
        cout<<N_QQbkg<<" QQbkg passed cuts"<<endl;
    }
    cout<<endl;
    //                                                      ////
    ////////////////////////////////////////////////////////////

    // Show chi2 from generated data
    RooDataSet *toyData;
    int N_toyData = 0;
    if(inclSig)N_toyData+=N_SigMC;
    if(inclB0s)N_toyData+=N_B0bkg;
    if(inclBBs)N_toyData+=N_BBbkg;
    if(inclQQs)N_toyData+=N_QQbkg;
    cout<<"=====----- TOY DATA  -----====="<<endl;
    cout<<endl<<N_toyData<<" data points in toyData "<<endl<<endl;
    RooPlot *frame2 = Mbc.frame(Title(Form("ToyData M_{bc}[%s,|deE|<%.2f,bdt>%.1f,signal(%i),B0s(%i),BBs(%i),QQs(%i)]", fInType,deEcut,BDTcut,inclSig,inclB0s,inclBBs,inclQQs)));

    if          ( useGaus  &&  useArgus ){
        toyData = model->generate(RooArgSet(Mbc),N_toyData);
        toyData -> plotOn(frame2);
        model   -> plotOn(frame2, Range("plotRange"));
    }else if    (!useGaus  &&  useArgus ){
        toyData = bkg  . generate(RooArgSet(Mbc),N_toyData);
        toyData -> plotOn(frame2);
        bkg     .  plotOn(frame2, Range("plotRange"));
    }else if    ( useGaus  && !useArgus){
        toyData = sig  . generate(RooArgSet(Mbc),N_toyData);
        toyData -> plotOn(frame2);
        sig     .  plotOn(frame2, Range("plotRange"));
    }
    cout<<endl;
    cout<<"toyData chi2  = "<< frame2->chiSquare(nParams)<<endl;
    cout<<endl<<endl;
    TCanvas *c2 = new TCanvas("c2", "c2", 600, 400);
    frame2 -> Draw();
    */

    return;

}//End File
