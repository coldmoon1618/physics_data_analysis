// Make PDF plots for omks (6 rows, no BBbar)
//

void splot_omks()
{
  gROOT->Reset();
  gROOT->SetStyle("BABAR");

  Bool_t doKPi=kFALSE, twopage=kFALSE, doFeed = kFALSE, doMass = kTRUE;
  Char_t* modename = "omks";
  Int_t  xsiz=700, ysiz=900, xpos=600, ypos=0, iP=0, row1=0; 
  Int_t row=5;

  TFile* f = new TFile("../results/omks.mlFitter_Config.sPlot.SPlotAct.root");
  f->ls();

  TCanvas *PDFcanv = new TCanvas("PDFcanv", "", xpos, ypos, xsiz, ysiz);
  row1 = row;
  if(twopage) {row1=4;}
  if (doFeed) {
    PDFcanv->Divide(3,row1);
  } else {
    PDFcanv->Divide(2,row1);
  }

  ////////////////////// mES ////////////////////////
  RooPlot* mB_sig = f->Get("sPlot_mes_nSig");
  mB_sig->SetXTitle("mES (GeV) ");
  plotIt(mB_sig, ++iP, 6, .25, .15, .55);
  
  RooPlot* mB_bkg = f->Get("sPlot_mes_nBkg");
  mB_bkg->SetXTitle("mES (GeV) ");
  plotIt(mB_bkg, ++iP, 2, .15, .15, .20);

  ///////////////// DE ///////////////////
  RooPlot* deltae_sig = f->Get("sPlot_de_nSig"); 
  deltae_sig->SetXTitle("#Delta E (GeV) ");
  plotIt(deltae_sig, ++iP, 6, .22, .70, .57);

  RooPlot* deltae_bkg = f->Get("sPlot_de_nBkg");
  deltae_bkg->SetXTitle("#Delta E (GeV) ");
  plotIt(deltae_bkg, ++iP, 3, .18, .15, .20);

  ////////////////////// xF //////////////////////////////////
  RooPlot* fisher_sig = f->Get("sPlot_fisher_nSig");
  fisher_sig->SetXTitle("Fisher");
  plotIt(fisher_sig, ++iP, 4, .17, .70, .65); fisher_sig->Draw();

  RooPlot* fisher_bkg = f->Get("sPlot_fisher_nBkg");
  fisher_bkg->SetXTitle("Fisher");
  PDFcanv->cd(++iP);  gPad->SetLogy(1); fisher_bkg->SetMinimum(.1);
  plotIt(fisher_bkg, iP, 7, .18, .45, .18);

  ////////////////////// heli //////////////////////////////////
  RooPlot* helomega_sig = f->Get("sPlot_heli_nSig");
  helomega_sig->SetXTitle("cos (#omega Helicity)");
  plotIt(helomega_sig, ++iP, 3, .15, .15, .70);
  
  RooPlot* helomega_bkg = f->Get("sPlot_heli_nBkg");
  helomega_bkg->SetXTitle("cos (#omega Helicity)");
  plotIt(helomega_bkg, ++iP, 5, .17, .15, .28);
  
  RooPlot* mOmega_sig = f->Get("sPlot_mOmega_nSig");
  mOmega_sig->SetXTitle("#omega mass (GeV) ");
  plotIt(mOmega_sig, ++iP, 6, .22, .15, .55);
  
  RooPlot* mOmega_bkg = f->Get("sPlot_mOmega_nBkg");
  mOmega_bkg->SetXTitle("#omega mass (GeV) ");
  plotIt(mOmega_bkg, ++iP, 4, .20, .15, .65);
  
  PDFcanv->SaveAs(modename+TString("_sPlot.eps"));
  
}

void moveParBox(RooPlot* plot, Int_t nLines, Double_t width,
                Double_t xCorner, Double_t yCorner) { 
  Double_t x1, y1, x2, y2;
  Double_t lineHeight = 0.06;
  x1 = xCorner > 0 ? xCorner : -xCorner - width;
  x2 = xCorner > 0 ? xCorner + width : -xCorner;
  y1 = yCorner > 0 ? yCorner : -yCorner - nLines*lineHeight;
  y2 = yCorner > 0 ? yCorner + nLines*lineHeight : -yCorner;
  //  TPave* pbox = (TPave*) plot->findObject("TPave");
  //  pbox->SetX1(x1);
  //  pbox->SetY1(y1);
  //  pbox->SetX2(x2);
  //  pbox->SetY2(y2);
  //  cout<<"x1,x2 " << x1 << "  " << x2 <<endl;
  //  cout<<"y1,y2 " << y1 << "  " << y2 <<endl;
}
void plotIt(RooPlot* plot, Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot) {
  if (!plot) return;
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  if (hist) hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(0.72);
  plot->GetYaxis()->SetTitleSize(0.08);
  plot->GetXaxis()->SetNdivisions(405);
  plot->GetXaxis()->SetTitleSize(0.08);
  PDFcanv->cd(iP);
  moveParBox(plot,nlines,width,xLeft,yBot);
  plot->Draw();
}
void plotIt2(RooPlot* plot, Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot) {
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(0.72);
  plot->GetYaxis()->SetTitleSize(0.08);
  plot->GetXaxis()->SetNdivisions(405);
  plot->GetXaxis()->SetTitleSize(0.08);
  PDFcanv2->cd(iP);
  moveParBox(plot,nlines,width,xLeft,yBot); 
  plot->Draw();
}
