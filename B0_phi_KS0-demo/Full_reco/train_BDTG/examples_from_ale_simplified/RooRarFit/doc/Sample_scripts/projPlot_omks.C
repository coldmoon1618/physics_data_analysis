void projPlot_omks()
{ 
  gROOT->Reset();
  gROOT->SetStyle("BABAR");
  //  gStyle->SetOptFit(0011);
  //  gStyle->SetOptStat(kFALSE);

  // Load omegaks projections
  TFile proj("../results/omks.mlFitter_Config.projPlot.ProjAct.root");
  RooPlot* Mb = proj.Get("proj_mes");
  RooPlot* De = proj.Get("proj_de");
  RooPlot* Fish = proj.Get("proj_fisher");
  RooPlot* Heli = proj.Get("proj_heli");
  RooPlot* MO = proj.Get("proj_mOmega");

  Double_t leftMargin = 0.15;
  Double_t rightMargin = 0.06;
  Double_t topMargin = 0.04;
  Double_t bottomMargin = 0.09;
  gStyle->SetNdivisions(405,"xy");

  TCanvas *c = new TCanvas("c", "all", 600, 0, 600, 900);

  TPad* c1 = NewPad(c,"c1", 0, .660, .5, .995);
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
  
  TPad* c2 = NewPad(c,"c2", .5, .660, 1, .995);
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

  TPad* c3 = NewPad(c,"c3", 0, .330, .5, .655);
  c3->SetLeftMargin(leftMargin); c3->SetRightMargin(rightMargin);
  c3->SetTopMargin(topMargin);         c3->SetBottomMargin(bottomMargin);
  Fish->SetYTitle("Events / 0.25 ");
  Fish->SetMinimum(-1e-6);
  Fish->GetYaxis()->SetLabelSize(0.04);
  Fish->GetYaxis()->SetTitleOffset(0.8);
  Fish->GetYaxis()->SetTitleSize(0.06);
  Fish->SetXTitle("Fisher");
  Fish->GetXaxis()->SetLabelSize(0.04);
  Fish->GetXaxis()->SetTitleOffset(0.8);
  Fish->GetXaxis()->SetTitleSize(0.05);
  Fish->GetXaxis()->SetNdivisions(5);
  Fish->Draw();
  
  TPad* c4 = NewPad(c,"c4", .5, .330, 1, .655);
  c4->SetLeftMargin(leftMargin); c4->SetRightMargin(rightMargin);
  c4->SetTopMargin(topMargin);         c4->SetBottomMargin(bottomMargin);
  Heli->SetYTitle("Events / 0.05 ");
  Heli->SetMinimum(-1e-6);
  Heli->GetYaxis()->SetLabelSize(0.04);
  Heli->GetYaxis()->SetTitleOffset(0.8);
  Heli->GetYaxis()->SetTitleSize(0.06);
  Heli->SetXTitle("cos (#omega Helicity)");
  Heli->GetXaxis()->SetLabelSize(0.04);
  Heli->GetXaxis()->SetTitleOffset(0.8);
  Heli->GetXaxis()->SetTitleSize(0.05);
  Heli->GetXaxis()->SetNdivisions(9);
  Heli->Draw();

  TPad* c5 = NewPad(c,"c5", 0, 0.0, .5, .325);
  c5->SetLeftMargin(leftMargin); c5->SetRightMargin(rightMargin);
  c5->SetTopMargin(topMargin);         c5->SetBottomMargin(bottomMargin);
  MO->SetYTitle("Events / 50 MeV ");
  MO->SetMinimum(-1e-6);
  MO->GetYaxis()->SetLabelSize(0.04);
  MO->GetYaxis()->SetTitleOffset(0.8);
  MO->GetYaxis()->SetTitleSize(0.06);
  MO->SetXTitle("#omega mass (GeV) ");
  MO->GetXaxis()->SetLabelSize(0.04);
  MO->GetXaxis()->SetTitleOffset(0.8);
  MO->GetXaxis()->SetTitleSize(0.05);
  MO->GetXaxis()->SetNdivisions(5);
  MO->Draw();
  
  //  TPad* c6 = NewPad(c,"c6", .5, 0.0, 1, .325);
  //  c6->SetLeftMargin(leftMargin); c6->SetRightMargin(rightMargin);
  //  c6->SetTopMargin(topMargin);         c6->SetBottomMargin(bottomMargin);
  //  De->SetYTitle("Events / 20 MeV ");
  //  De->SetMinimum(-1e-6);
  //  De->GetYaxis()->SetLabelSize(0.04);
  //  De->GetYaxis()->SetTitleOffset(0.8);
  //  De->GetYaxis()->SetTitleSize(0.06);
  //  De->SetXTitle("#Delta E (GeV) ");
  //  De->GetXaxis()->SetLabelSize(0.04);
  //  De->GetXaxis()->SetTitleOffset(0.8);
  //  De->GetXaxis()->SetTitleSize(0.05);
  //  De->GetXaxis()->SetNdivisions(9);
  //  De->Draw();  

  c->cd();
  TLatex* taga = new TLatex(.10, .940, "(a)");
  taga->SetNDC(kTRUE);  taga->SetTextSize(0.05);  taga->Draw();
  TLatex* tagb = new TLatex(.60, .940, "(b)");
  tagb->SetNDC(kTRUE);  tagb->SetTextSize(0.05);  tagb->Draw();
  TLatex* tagc = new TLatex(.10, .600, "(c)");
  tagc->SetNDC(kTRUE);  tagc->SetTextSize(0.05);  tagc->Draw();
  TLatex* tagd = new TLatex(.60, .600, "(d)");
  tagd->SetNDC(kTRUE);  tagd->SetTextSize(0.05);  tagd->Draw();
  TLatex* tage = new TLatex(.10, .270, "(e)");
  tage->SetNDC(kTRUE);  tage->SetTextSize(0.05);  tage->Draw();
  //  TLatex* tagf = new TLatex(.60, .270, "(f)");
  //  tagf->SetNDC(kTRUE);  tagf->SetTextSize(0.05);  tagf->Draw();
  
  //  BABARSmartLabel(.28, .93, .4, "preliminary", .4);
  
  c->SaveAs("projPlot_omks.eps");
  
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
