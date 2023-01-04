void projPlot_omks_mesde()
{ 
  gROOT->Reset();
  gROOT->SetStyle("BABAR");
  //  gStyle->SetOptFit(0011);
  //  gStyle->SetOptStat(kFALSE);

  // Load omegaks projections
  TFile proj("../results/omks.mlFitter_Config.projPlot.ProjAct.root");
  RooPlot* Mb = proj.Get("proj_mes");
  RooPlot* De = proj.Get("proj_de");
  
  Double_t leftMargin = 0.15;
  Double_t rightMargin = 0.06;
  Double_t topMargin = 0.04;
  Double_t bottomMargin = 0.09;
  gStyle->SetNdivisions(405,"xy");

  TCanvas *c = new TCanvas("c", "all", 600, 0, 600, 300);

  TPad* c1 = NewPad(c,"c1", 0, 0.0, .5, .995);
  c1->SetLeftMargin(leftMargin); c1->SetRightMargin(rightMargin);
  c1->SetTopMargin(topMargin);         c1->SetBottomMargin(bottomMargin);
  Mb->SetYTitle("Events / 2 MeV ");
  Mb->SetMinimum(-1e-6);
  Mb->GetYaxis()->SetLabelSize(0.04);
  Mb->GetYaxis()->SetTitleOffset(0.8);
  Mb->GetYaxis()->SetTitleSize(0.06);
  Mb->SetXTitle("mES (GeV) ");
  Mb->GetXaxis()->SetLabelSize(0.04);
  Mb->GetXaxis()->SetTitleOffset(0.8);
  Mb->GetXaxis()->SetTitleSize(0.05);
  Mb->GetXaxis()->SetNdivisions(5);
  Mb->Draw();
  
  TPad* c2 = NewPad(c,"c2", .5, 0.0, 1, .995);
  c2->SetLeftMargin(leftMargin); c2->SetRightMargin(rightMargin);
  c2->SetTopMargin(topMargin);         c2->SetBottomMargin(bottomMargin);
  De->SetYTitle("Events / 20 MeV ");
  De->SetMinimum(-1e-6);
  De->GetYaxis()->SetLabelSize(0.04);
  De->GetYaxis()->SetTitleOffset(0.8);
  De->GetYaxis()->SetTitleSize(0.06);
  De->SetXTitle("#Delta E (GeV) ");
  De->GetXaxis()->SetLabelSize(0.04);
  De->GetXaxis()->SetTitleOffset(0.8);
  De->GetXaxis()->SetTitleSize(0.05);
  De->GetXaxis()->SetNdivisions(9);
  De->Draw();
  
  c->cd();
  TLatex* taga = new TLatex(.40, .860, "(a)");
  taga->SetNDC(kTRUE);  taga->SetTextSize(0.08);  taga->Draw();
  TLatex* tagb = new TLatex(.90, .860, "(b)");
  tagb->SetNDC(kTRUE);  tagb->SetTextSize(0.08);  tagb->Draw();

  //    BABARSmartLabel(.95, .75, .7, "~1");

    c->SaveAs("projPlot_omks_mesde.eps");

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
