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

void CheckBins()
{
    const int nBins = 7;
    array<float ,nBins> w_values;
    array<float ,nBins> dw_values;
    array<float ,nBins> eff_ratio;

    array<float ,nBins> absLamb;
    array<float ,nBins> argLamb;
    array<float ,nBins> absLambErr;
    array<float ,nBins> argLambErr;

    //Counting
    /* with response cut
    w_values  ={0.4815,0.4238,0.3363,0.2561,0.1988,0.1458,0.0530};
    dw_values ={-0.0218,-0.0300,-0.0148,0.0045,-0.0077,-0.0242,-0.0031};
    */
    // without response cut
    w_values  ={0.4794,0.4235,0.3362,0.2552,0.1972,0.1452,0.0531};
    dw_values ={-0.0225,-0.0288,-0.0202,0.0023,-0.0059,-0.0182,-0.0027};
    
    //Fitting
    //w_values  ={0.4819,0.4206,0.3296,0.2562,0.1774,0.1381,0.0458};
    //dw_values ={-0.0210,-0.0291,-0.0150,0.0130,-0.0085,-0.0459,-0.0081};

    eff_ratio ={0.9593,0.9834,1.0000,0.9613,1.0161,1.0707,1.0185};
    //eff_ratio ={-0.0208,-0.0084,0.0000,-0.0197,0.0080,0.0342,0.0092};//mu
    for(int iBin=0; iBin<7; iBin++){
        int myBin = iBin+1;
        // Observable
        RooRealVar trueDeTau("trueDeTau", "trueDeTau", -10, 10);
        trueDeTau.setBins(50);
        // Parameters
        RooRealVar dm   ("dm",  "delta m(B0)", 0.507);// MC13a value
        RooRealVar tau  ("tau", "tau (B0)", 1.525 );// MC13a value
        RooRealVar w    ("w",   "", w_values[iBin]);
        RooRealVar dw   ("dw",  "", dw_values[iBin]);
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
        RooRealVar absLambda("absLambda", "|lambda|", 1,0,2);
        RooRealVar argLambda("argLambda", "|lambda|", 0.703,-1,1);
        RooRealVar effR("effR", "B0/B0bar reco efficiency ratio", eff_ratio[iBin]);

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
        c1 -> SaveAs(Form("bin%i.pdf",myBin));

        absLambda.Print();
        argLambda.Print();
        cout<<"bin"<<myBin<<endl;
        absLamb[iBin]=absLambda.getVal();
        argLamb[iBin]=argLambda.getVal();
        absLambErr[iBin]=absLambda.getError();
        argLambErr[iBin]=argLambda.getError();

    }

    cout<<"    //TruthDeTau_RecoFlavor by rBins, use counting"<<endl;
    cout<<Form("    Double_t abs_val[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
            absLamb[0], absLamb[1], absLamb[2], absLamb[3], absLamb[4], absLamb[5], absLamb[6])<<endl;
    cout<<Form("    Double_t abs_err[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};" ,
            absLambErr[0], absLambErr[1], absLambErr[2], absLambErr[3], absLambErr[4], absLambErr[5], absLambErr[6])<<endl;
    cout<<Form("    Double_t arg_val[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
            argLamb[0],argLamb[1],argLamb[2],argLamb[3],argLamb[4],argLamb[5],argLamb[6])<<endl;
    cout<<Form("    Double_t arg_err[n] ={%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f};", 
            argLambErr[0],argLambErr[1],argLambErr[2],argLambErr[3],argLambErr[4],argLambErr[5],argLambErr[6])<<endl;

}
