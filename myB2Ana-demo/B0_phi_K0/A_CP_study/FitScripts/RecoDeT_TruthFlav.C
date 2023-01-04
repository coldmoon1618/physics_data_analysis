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

void RecoDeT_TruthFlav()
{
    // Observable
    RooRealVar recoDeT("recoDeT", "recoDeT", -10, 10);
    recoDeT.setBins(50);

    // Parameters
    RooRealVar dm("dm", "delta m(B0)", 0.507, .4,.6);
    //RooRealVar tau("tau", "tau (B0)", 1.525 );// MC13a value
    //RooRealVar tau("tau", "tau (B0)", 1.519 );// MC14 value
    RooRealVar tau("tau", "tau (B0)", 1.5, 1.4, 1.6);// float

    RooCategory mixState("mixState", "B0/B0bar mixing state");
    mixState.defineType("mixed", -1);
    mixState.defineType("unmixed", 1);

    RooCategory trueFlav("trueFlav", "Flavour of the tagged B0");
    trueFlav.defineType("B0", 1);
    trueFlav.defineType("B0bar", -1);

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
    RooDataSet data("data","data",RooArgSet(recoDeT,trueFlav,mixState),ImportFromFile("AcpFitParams.root","tree"));
    RooPlot *frame1 = recoDeT.frame(Title("A_{CP}"));
    RooPlot *frame2 = recoDeT.frame(Title("B0(dark) anti_B0(light)"));

    // Additional parameters needed for B decay with CPV
    RooRealVar CPeigen("CPeigen", "CP eigen value", -1);
    RooRealVar absLambda("absLambda", "|lambda|", 1, 0, 2);
    RooRealVar argLambda("argLambda", "|lambda|", 0.7, -1, 1);
    RooRealVar effR("effR", "B0/B0bar reco efficiency ratio", 1);

    // Construct Bdecay with CP violation
    //RooBCPEffDecay bmix("bmix", "bmix", recoDeT, trueFlav, tau, dm, RooConst(0), CPeigen, absLambda, argLambda, effR, RooConst(0), truthModel, RooBCPEffDecay::DoubleSided);
    RooBCPEffDecay bmix("bmix", "bmix", recoDeT, trueFlav, tau, dm, RooConst(0), CPeigen, absLambda, argLambda, effR, RooConst(0), sig_resol, RooBCPEffDecay::DoubleSided);

    data.plotOn(frame1, Asymmetry(trueFlav));
    bmix.fitTo(data);
    bmix.plotOn(frame1, Asymmetry(trueFlav));

    data.plotOn(frame2, Cut("trueFlav==trueFlav::B0"));
    data.plotOn(frame2, Cut("trueFlav==trueFlav::B0bar"), MarkerColor(kCyan));
    bmix.plotOn(frame2, Slice(trueFlav, "B0"));
    bmix.plotOn(frame2, Slice(trueFlav, "B0bar"), LineColor(kCyan));

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
