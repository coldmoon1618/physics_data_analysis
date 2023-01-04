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

void TruthDeTau_TruthFlav()
{
    // Observable
    RooRealVar trueDeTau("trueDeTau", "trueDeTau", -10, 10);
    trueDeTau.setBins(40);

    // Parameters
    RooRealVar dm("dm", "delta m(B0)", 0.507);
    //RooRealVar dm("dm", "delta m(B0)", 0.507, .4,.6);
    RooRealVar tau("tau", "tau (B0)", 1.525 );// MC13a value
    //RooRealVar tau("tau", "tau (B0)", 1.519 );// MC14 value
    //RooRealVar tau("tau", "tau (B0)", 1.5, 1.4, 1.6);// float

    RooCategory trueFlav("trueFlav", "Flavour of the tagged B0");
    trueFlav.defineType("B0", 1);
    trueFlav.defineType("B0bar", -1);

    // Use delta function resolution model
    RooTruthModel truthModel("tm", "truth model", trueDeTau);

    //Import data
    RooDataSet data("data","data",RooArgSet(trueDeTau,trueFlav),ImportFromFile("AcpFitParams.root","tree"));
    RooPlot *frame1 = trueDeTau.frame(Title("A_{CP}"));
    RooPlot *frame2 = trueDeTau.frame(Title("B0(dark) anti_B0(light)"));

    // Additional parameters needed for B decay with CPV
    RooRealVar CPeigen("CPeigen", "CP eigen value", -1);
    RooRealVar absLambda("absLambda", "|lambda|", 1, 0, 2);
    RooRealVar argLambda("argLambda", "|lambda|", 0.7, -1, 1);
    RooRealVar effR("effR", "B0/B0bar reco efficiency ratio", 1);

    // Construct Bdecay with CP violation
    RooBCPEffDecay bmix("bmix", "bmix", trueDeTau, trueFlav, tau, dm, RooConst(0), CPeigen, absLambda, argLambda, effR, RooConst(0), truthModel, RooBCPEffDecay::DoubleSided);

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

}
