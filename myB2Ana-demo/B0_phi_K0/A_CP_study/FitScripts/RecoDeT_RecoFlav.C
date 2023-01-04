#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooCategory.h"
#include "RooBMixDecay.h"
#include "RooBCPEffDecay.h"
#include "RooBDecay.h"
#include "RooFormulaVar.h"
#include "RooTruthModel.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit;

void RecoDeT_RecoFlav(bool fixDw = 0)
{
    const int nBins = 7;
    // Observable
    RooRealVar recoDeT("recoDeT", "recoDeT", -10, 10);
    recoDeT.setBins(50);

    // Parameters
    RooRealVar dm("dm", "delta m(B0)", 0.507, .4,.6);
    //RooRealVar tau("tau", "tau (B0)", 1.525 );// MC13a value
    //RooRealVar tau("tau", "tau (B0)", 1.519 );// MC14 value
    RooRealVar tau("tau", "tau (B0)", 1.5, 1.4, 1.6);// float
    RooRealVar *w[nBins];
    RooRealVar *dw[nBins];
    // Data values
    //float w_values[nBins]  = { .469 , .411 , .299 , .233 , .129 , .098 , .027 };
    float w_values[nBins]  = { .4781, .4231, .3530, .2739, .1916, .1081 , .00198};
    float dw_values[nBins] = { .009 , .0074, .0019, .0062, .0008, .0079, .0005};

    // flavor
    RooCategory recoFlav("recoFlav", "Flavour of the tagged B0");
    recoFlav.defineType("B0", 1);
    recoFlav.defineType("B0bar", -1);

    // rBins defined accordding to flavorTag paper in SaveAcpParams.C
    RooCategory rBins("rBins", "rBins");
    rBins.defineType("bin1",1);
    rBins.defineType("bin2",2);
    rBins.defineType("bin3",3);
    rBins.defineType("bin4",4);
    rBins.defineType("bin5",5);
    rBins.defineType("bin6",6);
    rBins.defineType("bin7",7);

    // Use delta function resolution model
    RooTruthModel truthModel("tm", "truth model", recoDeT);
    // core
    RooRealVar sig_biasc("sig_biasc","biasc",0.0,-1.0,1.0) ;
    RooRealVar sig_sigmac("sig_sigmac","sigmac",1.4,0.1,2.0) ;
    RooGaussModel sig_core("sig_core","core deltaT resolution",recoDeT,sig_biasc,sig_sigmac) ;
    // tail
    RooRealVar sig_biast("sig_biast","biast",0, -1.0,1.0) ;
    RooRealVar sig_sigmat("sig_sigmat","sigmat",6.0,3.0,15.0) ;
    RooGaussModel sig_tail("sig_tail","Tail deltaT resolution",recoDeT,sig_biast,sig_sigmat) ;

 // Build a composite resolution model f*core+(1-f)*tail

    RooRealVar sig_corefrac("sig_corefrac","fraction of core",0.7,0.05,0.90) ;
    RooAddModel sig_resol("sig_resol","sum of core and gm2",RooArgList(sig_core,sig_tail),sig_corefrac) ;

    //Import data
    RooDataSet data("data","data",RooArgSet(recoDeT,recoFlav,rBins),ImportFromFile("AcpFitParams.root","tree"));
    RooPlot *frame1 = recoDeT.frame(Title("A_{CP}"));
    RooPlot *frame2 = recoDeT.frame(Title("B0(dark) anti_B0(light)"));
    //RooSuperCategory fitCat("fitCat","fitCat", RooArgSet(recoFlav,rBins));
    //data.table(fitCat)->Print("v");

    // Additional parameters needed for B decay with CPV
    RooRealVar CPeigen("CPeigen", "CP eigen value", -1);
    RooRealVar absLambda("absLambda", "|lambda|", 1, 0, 2);
    RooRealVar argLambda("argLambda", "|lambda|", 0.7, -1, 1);
    RooRealVar effR("effR", "B0/B0bar reco efficiency ratio", 1);

    // Construct Bdecay with CP violation
    RooBCPEffDecay *bmix[nBins];
    RooSimultaneous simPdf("simPdf","simPdf",rBins);
    for(int iBin=0; iBin<nBins; iBin++){
        w   [iBin]  = new RooRealVar(Form("w%i",iBin+1),  "flavour mistag rate", w_values[iBin]);
        if(fixDw){
            dw[iBin]= new RooRealVar(Form("dw%i",iBin+1), "delta mistag rate for B0/B0bar", dw_values[iBin]);
        }else{
            dw[iBin]= new RooRealVar(Form("dw%i",iBin+1), "delta mistag rate for B0/B0bar", .1,0.,1);
        }
        bmix[iBin]  = new RooBCPEffDecay(Form("bmix_%i",iBin+1), "bmix", recoDeT, recoFlav, tau, dm, 
        *w[iBin], CPeigen, absLambda, argLambda, effR, *dw[iBin], sig_resol, RooBCPEffDecay::DoubleSided);
        simPdf.addPdf(*bmix[iBin],Form("bin%i",iBin+1));
    }

    data.plotOn(frame1, Asymmetry(recoFlav));
    simPdf.fitTo(data);
    simPdf.plotOn(frame1, Asymmetry(recoFlav), ProjWData(data));

    data.plotOn(frame2, Cut("recoFlav==recoFlav::B0"));
    data.plotOn(frame2, Cut("recoFlav==recoFlav::B0bar"), MarkerColor(kCyan));
    simPdf.plotOn(frame2, Slice(recoFlav, "B0"), ProjWData(data));
    simPdf.plotOn(frame2, Slice(recoFlav, "B0bar"), LineColor(kCyan), ProjWData(data));

    TCanvas *c1 = new TCanvas("c1", "c1", 500, 800);
    c1->Divide(1,2,.01,.01);
    c1->cd(1);
    frame1->GetYaxis()->SetTitleOffset(1.6);
    frame1 -> Draw();
    c1->cd(2);
    frame2->GetYaxis()->SetTitleOffset(1.6);
    frame2 -> Draw();

    absLambda.Print();
    argLambda.Print();
    dm.Print();
    tau.Print();
    sig_biasc.Print();
    sig_sigmac.Print();
    sig_biast.Print();
    sig_sigmat.Print();
    sig_corefrac.Print();

}
