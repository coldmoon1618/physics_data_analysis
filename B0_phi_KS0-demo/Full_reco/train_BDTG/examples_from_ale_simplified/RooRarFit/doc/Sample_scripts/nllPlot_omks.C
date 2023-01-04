void nllPlot_omks()
{
  gROOT->Reset();
  gROOT->SetStyle("BABAR");

  TFile proj("../results/omks.mlFitter_Config.projPlot.ProjAct.root");
  TCanvas *c = new TCanvas("c", "all", 800, 600);

  TH1F *bkg = new TH1F("","",100,0,1);
  TH1F *bkgandsig = new TH1F("","",100,0,1);

  bkg->Add(LLR_dtBkgPdf,LLR_dtChlsPdf);
  bkgandsig->Add(bkg,LLR_dtSigPdf);
  bkg->SetLineWidth(2);
  bkg->SetFillColor(kRed);
  bkg->SetFillStyle(1001);
  bkg->Scale(.1);
  bkgandsig->SetLineWidth(2);
  bkgandsig->SetFillColor(kGreen);
  bkgandsig->Scale(.1);
  c->SetLogy();
  
  Int_t Nbins = LLR_onData->GetNbinsX();
  Double_t xmin = LLR_dtBkgPdf->GetXaxis()->GetXmin();
  Double_t xmax = LLR_dtBkgPdf->GetXaxis()->GetXmax();
  RooRealVar xvar("xvar", "the plot variable", xmin, xmax);
  RooPlot* frame = xvar.frame(Nbins);

  LLR_onData->Sumw2();
  LLR_onData->SetMarkerStyle(8);
  LLR_onData->Rebin(2);
  bkg->Rebin(2);
  bkgandsig->Rebin(2);

  frame->addTH1(bkgandsig, "hist2");
  frame->addTH1(bkg, "hist3");
  frame->addTH1(LLR_onData);

  frame->SetMinimum(1);  frame->SetMaximum(10000);
  frame->SetXTitle("L(S)/[L(S)+L(B)]");
  frame->SetYTitle("Events/bin");
  frame->SetTitle();
  frame->Draw();
  c->SaveAs("omks_likelihood.eps");
  frame->Delete();
}
