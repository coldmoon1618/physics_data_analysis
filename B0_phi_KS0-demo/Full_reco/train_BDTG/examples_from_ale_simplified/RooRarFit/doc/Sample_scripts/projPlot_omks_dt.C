void projPlot_omks_dt()
{  
  gROOT->Reset();
  gROOT->SetStyle("BABAR");
  //  gStyle->SetOptFit(0011);
  //  gStyle->SetOptStat(kFALSE);

  // Load omegaks projections
  TFile dt("../results/omks.mlFitter_Config.projPlot.ProjAct.root");
  RooPlot* Sin = dt.Get("proj_deltaT_Asym");
  RooPlot* B = dt.Get("proj_deltaT_Plus");
  RooPlot* Bbar = dt.Get("proj_deltaT_Minus");
  
  Double_t leftMargin = 0.15;
  Double_t rightMargin = 0.06;
  Double_t topMargin = 0.04;
  Double_t bottomMargin = 0.06;
  Double_t ymax = 15.;
  
  TCanvas *c = new TCanvas("c", "all", 600, 0, 600, 900);
  
  TPad* c1 = NewPad(c,"c1", 0, 0.67, 1, 0.98);
  c1->SetBottomMargin(bottomMargin);
  B->SetMinimum(-1e-6);
  B->SetMaximum(ymax);
  B->GetXaxis()->SetTitle(0);
  B->SetXTitle(0);
  B->GetXaxis()->SetLabelSize(0.0003);
  B->SetYTitle("Events / ps ");
  B->GetYaxis()->SetLabelSize(0.04);
  B->GetYaxis()->SetTitleOffset(0.5);
  B->GetYaxis()->SetTitleSize(0.09);
  B->GetYaxis()->SetLabelSize(0.05);
  B->Draw();
    
  TPad* c2 = NewPad(c,"c2", 0, .38, 1, .689);
  c2->SetBottomMargin(bottomMargin);
  Bbar->SetMinimum(-1e-6);
  Bbar->SetMaximum(ymax);
  Bbar->GetXaxis()->SetTitle(0);
  Bbar->SetXTitle(0);
  Bbar->GetXaxis()->SetLabelSize(0.0003);
  Bbar->SetYTitle("");
  Bbar->GetYaxis()->SetLabelSize(0.05);
  Bbar->Draw();
  
  TPad* c3 = NewPad(c,"c3", 0, 0.0, 1, .399);
  c3->SetBottomMargin(0.16);
  Sin->SetMinimum(-1.15);
  Sin->SetMaximum(1.15);
  Sin->GetXaxis()->SetTicks();
  Sin->GetXaxis()->SetTickLength(0.03);
  Sin->SetXTitle("#Deltat, t_{CP} - t_{tag} (ps) ");
  Sin->GetXaxis()->SetLabelSize(0.04);
  Sin->GetXaxis()->SetTitleOffset(0.8);
  Sin->GetXaxis()->SetTitleSize(0.07);
  Sin->SetYTitle("Asymmetry ");
  Sin->GetYaxis()->SetLabelSize(0.04);
  Sin->GetYaxis()->SetTitleOffset(0.7);
  Sin->GetYaxis()->SetTitleSize(0.07);
  Sin->Draw();

  c->cd();
  TLatex* taga = new TLatex(.22, .930, "(a)");
  taga->SetNDC(kTRUE);  taga->SetTextSize(0.045);  taga->Draw();
  TLatex* tagb = new TLatex(.22, .635, "(b)");
  tagb->SetNDC(kTRUE);  tagb->SetTextSize(0.045);  tagb->Draw();
  TLatex* tagc = new TLatex(.22, .348, "(c)");
  tagc->SetNDC(kTRUE);  tagc->SetTextSize(0.045);  tagc->Draw();

  //    BABARSmartLabel(.88, .95, .5, "~1");

    c->SaveAs("projPlot_omks_dt.eps");

}


TPad* NewPad(TCanvas *c, char *name, Double_t x1, Double_t y1,
	Double_t x2, Double_t y2)
{
  c->cd();
  TPad *pad = new TPad(name, "", x1, y1, x2, y2);
  pad->SetFrameFillStyle(0);
  pad->SetTicks();
  pad->SetTopMargin(0);
  pad->SetFillStyle(0);
  pad->Draw();
  pad->cd();

  return pad;
  
}






