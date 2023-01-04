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

void TruthDeTau_RecoFlav(TString fitValue = "SampleCount", bool fixDw = 1)
{
    const int nBins = 7;
    // Observable
    RooRealVar trueDeTau("trueDeTau", "trueDeTau", -10, 10);
    trueDeTau.setBins(50);

    // Parameters
    RooRealVar dm("dm", "delta m(B0)", 0.507);// MC13a value
    //RooRealVar dm("dm", "delta m(B0)", 0.507, .4,.6);
    //RooRealVar tau("tau", "tau (B0)", 1.525, 1.4,1.6);
    RooRealVar tau("tau", "tau (B0)", 1.525 );// MC13a value
    //RooRealVar tau("tau", "tau (B0)", 1.519 );// MC14 value
    RooRealVar *w[nBins];
    RooRealVar *dw[nBins];
    // Data values
    array<float,nBins>  w_values;
    array<float,nBins> dw_values;

    cout<<endl;
    if      (fitValue=="NoteCount"){
        cout<<"Belle II binning, note counting"<<endl<<endl;
        w_values  = { .4705, .3960, .2944, .2052, .1495, .0826, .0179};
        dw_values = { .0632, .0219, .0035, .0228, .0055, .0038, .0016};
    }else if(fitValue=="NoteFit"){
        cout<<"Belle II binning, note fitting"<<endl<<endl;
        w_values  = { .4657, .3995, .2994, .1970, .1577, .0816, .0168};
        dw_values = { .0536, .0272, .0001, .0176,-.0037, .0053, .0038};
    }else if(fitValue=="SampleCount"){
        cout<<"Counting from 200 file sample"<<endl;

        w_values  ={0.4815,0.4238,0.3363,0.2561,0.1988,0.1458,0.0530};
        dw_values ={-0.0218,-0.0300,-0.0148,0.0045,-0.0077,-0.0242,-0.0031};
    }else if(fitValue=="SampleFit"){
        cout<<"Fitting from 200 file sample"<<endl;

        w_values  ={0.4819,0.4206,0.3296,0.2562,0.1774,0.1381,0.0458};
        dw_values ={-0.0210,-0.0291,-0.0150,0.0130,-0.0085,-0.0459,-0.0081};
    }
    if(fixDw){
        cout<<"Fixing dw_values"<<endl<<endl;
    }else{
        cout<<"Floating dw_values"<<endl<<endl;
    }
    array<float,nBins> eff_ratio = {0.9592,0.9871,0.9961,0.9608,1.0183,1.0718,1.0186};

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
    RooTruthModel truthModel("tm", "truth model", trueDeTau);

    //Import data
    RooDataSet data("data","data",RooArgSet(trueDeTau,recoFlav,rBins),ImportFromFile("AcpFitParams.root","tree"));
    RooPlot *frame1 = trueDeTau.frame(Title("A_{CP}"));
    RooPlot *frame2 = trueDeTau.frame(Title("B0(dark) anti_B0(light)"));
    //RooSuperCategory fitCat("fitCat","fitCat", RooArgSet(recoFlav,rBins));
    //data.table(fitCat)->Print("v");

    // Additional parameters needed for B decay with CPV
    RooRealVar CPeigen("CPeigen", "CP eigen value", -1);
    RooRealVar absLambda("absLambda", "|lambda|", 1, 0, 2);
    RooRealVar argLambda("argLambda", "|lambda|", 0.7, -1, 1);

    // Construct Bdecay with CP violation
    RooBCPEffDecay *bmix[nBins];
    RooRealVar *effR[nBins];
    RooSimultaneous simPdf("simPdf","simPdf",rBins);
    for(int iBin=0; iBin<nBins; iBin++){
        w   [iBin]  = new RooRealVar(Form("w%i",iBin+1),  "flavour mistag rate", w_values[iBin]);
        //effR[iBin]  = new RooRealVar(Form("effR%i",iBin+1),  "B0/B0bar reco eff ratio", eff_ratio[iBin]);
        effR[iBin]  = new RooRealVar(Form("effR%i",iBin+1),  "B0/B0bar reco eff ratio", 1);
        if(fixDw){
            dw[iBin]= new RooRealVar(Form("dw%i",iBin+1), "delta mistag rate for B0/B0bar", dw_values[iBin]);
        }else{
            dw[iBin]= new RooRealVar(Form("dw%i",iBin+1), "delta mistag rate for B0/B0bar", .05,0,.1);
        }
        bmix[iBin]  = new RooBCPEffDecay(Form("bmix_%i",iBin+1), "bmix", trueDeTau, recoFlav, tau, dm, 
        *w[iBin], CPeigen, absLambda, argLambda, *effR[iBin], *dw[iBin], truthModel, RooBCPEffDecay::DoubleSided);
        simPdf.addPdf(*bmix[iBin],Form("bin%i",iBin+1));
    }

    data.plotOn(frame1, Asymmetry(recoFlav));
    RooFitResult* result = simPdf.fitTo(data, Save());
    result->Print("v");
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

}
