#include <iostream>
#include <fstream>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <TMath.h>
#include <TChain.h>

void RejVsEff() {
    double sig_div = 1/168600.;
    double bkg_div = 1/1.18047e6;

    TChain *results_2D = new TChain("tree_2D");
    results_2D -> AddFile("sigL_Rej_Eff.root");
    TChain *results_3D = new TChain("tree_3D");
    results_3D -> AddFile("sigL_Rej_Eff.root");
    int Entries;
    Entries = results_2D -> GetEntries();
    cout << Entries << " entries" << endl;
    double L2DCut;
    results_2D -> SetBranchAddress("L2DCut",   &L2DCut);
    float nsig0_2D;
    results_2D -> SetBranchAddress("nsig0_2D",    &nsig0_2D);
    float dnsig_2D;
    results_2D -> SetBranchAddress("dnsig_2D",    &dnsig_2D);
    float nbkg0_2D;
    results_2D -> SetBranchAddress("nbkg0_2D",    &nbkg0_2D);
    float dnbkg_2D;
    results_2D -> SetBranchAddress("dnbkg_2D",    &dnbkg_2D); 
    double L3DCut;
    results_3D -> SetBranchAddress("L3DCut",   &L3DCut);
    float nsig0_3D;
    results_3D -> SetBranchAddress("nsig0_3D",    &nsig0_3D);
    float dnsig_3D;
    results_3D -> SetBranchAddress("dnsig_3D",    &dnsig_3D);
    float nbkg0_3D;
    results_3D -> SetBranchAddress("nbkg0_3D",    &nbkg0_3D);
    float dnbkg_3D;
    results_3D -> SetBranchAddress("dnbkg_3D",    &dnbkg_3D); 

    auto c1 = new TCanvas("c1","Rejection vs Efficiency of sigL2D cut",500,500);
    c1->SetGrid();
    c1->GetFrame()->SetBorderSize(12);
    const int n = 9;
    double Eff_2D[n], Rej_2D[n], Eff_3D[n], Rej_3D[n];
    for(int i=0; i<n; i++)
    {
        results_2D -> GetEntry(i+1);
        results_3D -> GetEntry(i+1);
        cout << "cuts: " << L2DCut << ", " << L3DCut << endl;
        cout << "2D significance: " << nsig0_2D/sqrt(nbkg0_2D) << endl;
        cout << "3D significance: " << nsig0_3D/sqrt(nbkg0_3D) << endl;
        cout << endl;
        Eff_2D[i] =    nsig0_2D*sig_div;
        Rej_2D[i] = 1 -nbkg0_2D*bkg_div;  
        Eff_3D[i] =    nsig0_3D*sig_div;
        Rej_3D[i] = 1 -nbkg0_3D*bkg_div;  
    }

    //auto gr_2D = new TGraphErrors(n,x,y,ex,ey);
    auto gr_2D = new TGraphErrors(n,Eff_2D,Rej_2D);
    gr_2D->SetMarkerColor(kBlue);
    gr_2D->SetLineColor(kBlue);
    gr_2D->SetMarkerStyle(21);
    //gr_2D->Draw("ALP");
    //gr_2D->Draw("AP");

    auto gr_3D = new TGraphErrors(n,Eff_3D,Rej_3D);
    gr_3D->SetMarkerColor(kRed);
    gr_3D->SetLineColor(kRed);
    gr_3D->SetMarkerStyle(21);
    //gr_3D->Draw("AP");

    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gr_2D,"APL");
    mg->Add(gr_3D,"APL");
    mg->SetTitle("Blue Lxy, Red L3D; #varepsilon_{sig}; 1-#varepsilon_{bkg}");
    mg->Draw("a");

    c1 -> SaveAs("ROC_Ks.pdf");

    return ;
}
