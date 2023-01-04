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

void FitW()
{
    const int nBins = 7;
    array<float ,nBins> w_val;
    array<float ,nBins> w_err;
    array<float ,nBins> dw_values;
    for(int iBin=0; iBin<7; iBin++){
        int myBin = iBin+1;
        // Observable
        RooRealVar trueDeTau("trueDeTau", "trueDeTau", -10, 10);
        trueDeTau.setBins(50);

        // Parameters
        RooRealVar dm   ("dm",  "delta m(B0)", 0.507);// MC13a value
        RooRealVar tau  ("tau", "tau (B0)", 1.525 );// MC13a value
        RooRealVar w    ("w",   "", 0.3,.0,1.);
        RooRealVar dw   ("dw",  "", 0.0, -.5,.5);
        RooRealVar rBins("rBins","",1,7);

        // flavor
        RooCategory recoFlav("recoFlav", "Flavour of the tagged B0");
        recoFlav.defineType("B0", 1);
        recoFlav.defineType("B0bar", -1);

        // Use delta function resolution model
        RooTruthModel truthModel("tm", "truth model", trueDeTau);

        //Import data
        RooDataSet data("data","data",RooArgSet(trueDeTau,recoFlav,rBins),ImportFromFile("AcpFitParams.root","tree"),Cut(Form("rBins==%i",myBin)));
        RooPlot *frame1 = trueDeTau.frame(Title("A_{CP}"));
        RooPlot *frame2 = trueDeTau.frame(Title("B0(dark) anti_B0(light)"));

        // Additional parameters needed for B decay with CPV
        RooRealVar CPeigen("CPeigen", "CP eigen value", -1);
        RooRealVar absLambda("absLambda", "|lambda|", 1);
        RooRealVar argLambda("argLambda", "|lambda|", 0.703);
        RooRealVar effR("effR", "B0/B0bar reco efficiency ratio", 1);

        // Construct Bdecay with CP violation
        RooBCPEffDecay bmix("bmix", "bmix", trueDeTau, recoFlav, tau, dm, w, CPeigen, absLambda, argLambda, effR, dw, truthModel, RooBCPEffDecay::DoubleSided);

        data.plotOn(frame1, Asymmetry(recoFlav));
        RooFitResult* result = bmix.fitTo(data, Save());
        bmix.plotOn(frame1, Asymmetry(recoFlav));

        data.plotOn(frame2, Cut("recoFlav==recoFlav::B0"));
        data.plotOn(frame2, Cut("recoFlav==recoFlav::B0bar"), MarkerColor(kCyan));
        bmix.plotOn(frame2, Slice(recoFlav, "B0"));
        bmix.plotOn(frame2, Slice(recoFlav, "B0bar"), LineColor(kCyan));

        TCanvas *c1 = new TCanvas("c1", "c1", 500, 800);
        c1->Divide(1,2,.01,.01);
        c1->cd(1);
        frame1->GetYaxis()->SetTitleOffset(1.6);
        frame1 -> Draw();
        c1->cd(2);
        frame2->GetYaxis()->SetTitleOffset(1.6);
        frame2 -> Draw();
        c1->SaveAs(Form("bin%i.pdf",myBin));

        cout<<"bin"<<myBin<<endl;
        w.Print();

        w_val[iBin]=w.getVal();
        w_err[iBin]=w.getError();

        dw_values[iBin]=dw.getVal();
    }
    cout<<Form("w_val  ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
            w_val[0], w_val[1], w_val[2], w_val[3], w_val[4], w_val[5], w_val[6])<<endl;
    cout<<Form("w_err ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
            w_err[0],w_err[1],w_err[2],w_err[3],w_err[4],w_err[5],w_err[6])<<endl;

    cout<<Form("w_values  ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
            w_val[0], w_val[1], w_val[2], w_val[3], w_val[4], w_val[5], w_val[6])<<endl;
    cout<<Form("dw_values ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
            dw_values[0],dw_values[1],dw_values[2],dw_values[3],dw_values[4],dw_values[5],dw_values[6])<<endl;

}
