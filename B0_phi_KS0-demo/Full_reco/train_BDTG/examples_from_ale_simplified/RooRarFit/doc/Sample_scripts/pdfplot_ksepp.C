// Make PDF plots for ksepp (4 rows, no BBbar)
//

void pdfplot_ksepp()
{
  gROOT->Reset();
  gROOT->SetStyle("BABAR");

  Bool_t doKPi=kFALSE, twopage=kFALSE, doFeed = kFALSE, doMass = kFALSE;
  Char_t* modename = "ksepp";
  Int_t  xsiz=700, ysiz=900, xpos=600, ypos=0, iP=0, row1=0; 
  Int_t row=4;

  TFile* f = new TFile(TString("results/pdfPlot_")+modename+TString(".root"));
  f->ls();



  TCanvas *PDFcanv = new TCanvas("PDFcanv", "", xpos, ypos, xsiz, ysiz);
  row1 = row;
  if(twopage) {row1=4;}
  if (doFeed) {
    PDFcanv->Divide(3,row1);
    pWid = 0.35;
  } else {
    PDFcanv->Divide(2,row1);
    pWid = 0.27;
  }

  ///////////////// DE ///////////////////
  if(doKPi) {
  RooPlot* deltae_sigpion = f->Get("de_the_deSig"); 
  plotIt(deltae_sigpion, ++iP, 6, .27, .65, .57);
  } else{
  RooPlot* deltae_sig = f->Get("de_the_deSig"); 
  plotIt(deltae_sig, ++iP, 6, .27, .65, .57);
  }

  RooPlot* deltae_bkg = f->Get("de_the_deBkg");
  plotIt(deltae_bkg, ++iP, 3, .18, .15, .20);

  if (doFeed) {
    RooPlot* deltae_feed = f->Get("de_the_deChls");
    plotIt(deltae_feed, ++iP, 6, .32, .15, .20);
  }


  ////////////////////// mES ////////////////////////
  RooPlot* mB_sig = f->Get("mes_the_mesSig");
  plotIt(mB_sig, ++iP, 6, .30, .15, .55);
  
  RooPlot* mB_bkg = f->Get("mes_the_mesBkg");
  plotIt(mB_bkg, ++iP, 2, .15, .15, .20);

  if (doFeed) {
    RooPlot* mB_feed = f->Get("mes_the_mesChls");
    plotIt(mB_feed, ++iP, 6, .32, .15, .63);
  }

  ////////////////////// xF //////////////////////////////////
  RooPlot* fisher_sig = f->Get("fisher_the_fisSig");
  plotIt(fisher_sig, ++iP, 4, .25, .65, .65); fisher_sig->Draw();

  RooPlot* fisher_bkg = f->Get("fisher_the_fisBkg");
  PDFcanv->cd(++iP);  gPad->SetLogy(1); fisher_bkg->SetMinimum(.1);
  plotIt(fisher_bkg, iP, 7, .20, .42, .18);

  if (doFeed) {
    RooPlot* fisher_feed = f->Get("fisher_the_fisChls");
    plotIt(fisher_feed, ++iP, 4, .35, .55, .70 );
  }

  if (twopage) { PDFcanv->SaveAs(modename+TString("_PDF.eps")); }

  if (doMass) {
    //TCanvas *PDFcanv = new TCanvas("PDFcanv2", "", xpos, ypos, xsiz, ysiz);
    //if (doFeed) { PDFcanv2->Divide(3,3);}
    //else { PDFcanv2->Divide(2,3); }

    RooPlot* mOmega_sig = f->Get("mOmega_the_mOmegaSig");
    plotIt(mOmega_sig, ++iP, 6, .22, .15, .55);

    RooPlot* mOmega_bkg = f->Get("mOmega_the_mOmegaBkg");
    plotIt(mOmega_bkg, ++iP, 4, .20, .15, .65);

    if (doFeed) {
      RooPlot* mOmega_feed = f->Get("mOmega_the_mOmegaChls");
      plotIt(mOmega_feed, ++iP, 6, .35, .15, .61);
    }

  ////////////////////// heli //////////////////////////////////
    RooPlot* helomega_sig = f->Get("heli_the_heliSig");
    plotIt(helomega_sig, ++iP, 3, .15, .15, .70);

    RooPlot* helomega_bkgsg = f->Get("heli_the_helimassBkg");
    plotIt(helomega_bkgsg, ++iP, 5, .17, .15, .28);
  }

  ////////////////////// dt //////////////////////////////////
  PDFcanv->cd(++iP); // increment to skip dtSig

  RooPlot* deltaT_bkg = f->Get("deltaT_the_dtBkg");
  deltaT_bkg->GetXaxis()->SetRangeUser(-4., 4.);
  if (twopage) {
    plotIt2(deltaT_bkg, ++iP, 7, .20, .70, .50);
    if (doFeed) {
      RooPlot* deltaT_feed = f->Get("deltaT_the_dtChls");
      PDFcanv2->cd(++iP);  deltaT_feed->GetXaxis()->SetRangeUser(-8., 8.);
      plotIt2(deltaT_feed, iP, 7, .33, .59, .57);
    } 
  } else {
    plotIt(deltaT_bkg, ++iP,  7, .20, .70, .50);
    if (doFeed) {
      RooPlot* deltaT_feed = f->Get("deltaT_the_dtChls");
      PDFcanv->cd(++iP);  deltaT_feed->GetXaxis()->SetRangeUser(-8., 8.);
      plotIt(deltaT_feed, iP,  7, .33, .59, .57);
    }
  }

  if (twopage) {
    PDFcanv2->SaveAs(modename+TString("_PDF2.eps"));
  } else {
    PDFcanv->SaveAs(modename+TString("_PDF.eps"));
  }
}
void moveParBox(RooPlot* plot, Int_t nLines, Double_t width,
                Double_t xCorner, Double_t yCorner) { 
  Double_t x1, y1, x2, y2;
  Double_t lineHeight = 0.06;
  x1 = xCorner > 0 ? xCorner : -xCorner - width;
  x2 = xCorner > 0 ? xCorner + width : -xCorner;
  y1 = yCorner > 0 ? yCorner : -yCorner - nLines*lineHeight;
  y2 = yCorner > 0 ? yCorner + nLines*lineHeight : -yCorner;
  TPave* pbox = (TPave*) plot->findObject("TPave");
  pbox->SetX1(x1);
  pbox->SetY1(y1);
  pbox->SetX2(x2);
  pbox->SetY2(y2);
//  cout<<"x1,x2 " << x1 << "  " << x2 <<endl;
//  cout<<"y1,y2 " << y1 << "  " << y2 <<endl;
}
void plotIt(RooPlot* plot, Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot) {
  if (!plot) return;
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  if (hist) hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(1.72);
  plot->GetXaxis()->SetNdivisions(405);
  PDFcanv->cd(iP);
  moveParBox(plot,nlines,width,xLeft,yBot);
  plot->Draw();
}
void plotIt2(RooPlot* plot, Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot) {
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(1.72);
  plot->GetXaxis()->SetNdivisions(405);
  PDFcanv2->cd(iP);
  moveParBox(plot,nlines,width,xLeft,yBot); 
  plot->Draw();
}
