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

void ByBinsTruth()
{
    const int nBins = 7;
    array<float ,nBins> w_values;
    array<float ,nBins> dw_values;
    array<float ,nBins> eff_ratio;

    array<float ,nBins> absLamb;
    array<float ,nBins> argLamb;
    array<float ,nBins> absLambErr;
    array<float ,nBins> argLambErr;

    for(int iBin=0; iBin<7; iBin++){
        int myBin = iBin+1;
        // Observable
        RooRealVar trueDeTau("trueDeTau", "trueDeTau", -10, 10);
        trueDeTau.setBins(50);
        // Parameters
        RooRealVar dm   ("dm",  "delta m(B0)", 0.507);// MC13a value
        RooRealVar tau  ("tau", "tau (B0)", 1.525 );// MC13a value
        RooRealVar w    ("w",   "", 0.);
        RooRealVar dw   ("dw",  "", 0.);
        RooRealVar rBins("rBins","",1,7);

        // flavor
        RooCategory trueFlav("trueFlav", "Flavour of the tagged B0");
        trueFlav.defineType("B0", 1);
        trueFlav.defineType("B0bar", -1);

        // Use delta function resolution model
        RooTruthModel truthModel("tm", "truth model", trueDeTau);

        //Import data
        RooDataSet data("data","data",RooArgSet(trueDeTau,trueFlav,rBins),ImportFromFile("AcpFitParams.root","tree"),Cut(Form("rBins==%i",myBin)));
        RooPlot *frame1 = trueDeTau.frame(Title("A_{CP}"));
        RooPlot *frame2 = trueDeTau.frame(Title("B0(dark) anti_B0(light)"));

        // Additional parameters needed for B decay with CPV
        RooRealVar CPeigen("CPeigen", "CP eigen value", -1);
        RooRealVar absLambda("absLambda", "|lambda|", 1,0,2);
        RooRealVar argLambda("argLambda", "|lambda|", 0.703,-1,1);
        RooRealVar effR("effR", "B0/B0bar reco efficiency ratio", 1.);

        // Construct Bdecay with CP violation
        RooBCPEffDecay bmix("bmix", "bmix", trueDeTau, trueFlav, tau, dm, w, CPeigen, absLambda, argLambda, effR, dw, truthModel, RooBCPEffDecay::DoubleSided);

        data.plotOn(frame1, Asymmetry(trueFlav));
        RooFitResult* result = bmix.fitTo(data, Save());
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
        c1 -> SaveAs(Form("true_bin%i.pdf",myBin));

        absLambda.Print();
        argLambda.Print();
        cout<<"bin"<<myBin<<endl;
        absLamb[iBin]=absLambda.getVal();
        argLamb[iBin]=argLambda.getVal();
        absLambErr[iBin]=absLambda.getError();
        argLambErr[iBin]=argLambda.getError();

    }

    cout<<"    //TruthDeTau_TruthFlavor by rBins"<<endl;
    cout<<Form("    Double_t abs_val[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
            absLamb[0], absLamb[1], absLamb[2], absLamb[3], absLamb[4], absLamb[5], absLamb[6])<<endl;
    cout<<Form("    Double_t abs_err[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
            absLambErr[0], absLambErr[1], absLambErr[2], absLambErr[3], absLambErr[4], absLambErr[5], absLambErr[6])<<endl;
    cout<<Form("    Double_t arg_val[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
            argLamb[0],argLamb[1],argLamb[2],argLamb[3],argLamb[4],argLamb[5],argLamb[6])<<endl;
    cout<<Form("    Double_t arg_err[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
            argLambErr[0],argLambErr[1],argLambErr[2],argLambErr[3],argLambErr[4],argLambErr[5],argLambErr[6])<<endl;

}
