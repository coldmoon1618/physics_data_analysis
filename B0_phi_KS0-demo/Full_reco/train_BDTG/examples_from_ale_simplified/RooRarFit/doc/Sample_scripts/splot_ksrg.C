// Make PDF plots for ksrg (4 rows, no BBbar)
//

void splot_ksrg()
{
  gROOT->Reset();
  gROOT->SetStyle("BABAR");

  Bool_t doKPi=kFALSE, twopage=kFALSE, doFeed = kTRUE, doMass = kFALSE;
  Char_t* modename = "ksrg";
  Int_t  xsiz=700, ysiz=900, xpos=600, ypos=0, iP=0, row1=0; 
  Int_t row=4;

  TFile* f = new TFile("../results/ksrg.mlFitter_Config.sPlot.SPlotAct.root");
  f->ls();



  TCanvas *PDFcanv = new TCanvas("PDFcanv", "", xpos, ypos, xsiz, ysiz);
  row1 = row;
  if(twopage) {row1=4;}
  if (doFeed) {
    PDFcanv->Divide(3,row1);
  } else {
    PDFcanv->Divide(2,row1);
  }

  ///////////////// DE ///////////////////
  RooPlot* deltae_sig = f->Get("sPlot_de_nSig"); 
  plotIt(deltae_sig, ++iP, 6, .22, .70, .57);

  RooPlot* deltae_bkg = f->Get("sPlot_de_nBkg");
  plotIt(deltae_bkg, ++iP, 3, .18, .15, .20);

  if (doFeed) {
    RooPlot* deltae_feed = f->Get("sPlot_de_nChls");
    plotIt(deltae_feed, ++iP, 6, .32, .15, .20);
  }


  ////////////////////// mES ////////////////////////
  RooPlot* mB_sig = f->Get("sPlot_mes_nSig");
  plotIt(mB_sig, ++iP, 6, .25, .15, .55);
  
  RooPlot* mB_bkg = f->Get("sPlot_mes_nBkg");
  plotIt(mB_bkg, ++iP, 2, .15, .15, .20);

  if (doFeed) {
    RooPlot* mB_feed = f->Get("sPlot_mes_nChls");
    plotIt(mB_feed, ++iP, 6, .32, .15, .63);
  }

  ////////////////////// xF //////////////////////////////////
  RooPlot* fisher_sig = f->Get("sPlot_fisher_nSig");
  fisher_sig->GetXaxis()->SetRangeUser(-3., 2.9999);
  plotIt(fisher_sig, ++iP, 4, .17, .70, .65); 

  RooPlot* fisher_bkg = f->Get("sPlot_fisher_nBkg");
  fisher_bkg->GetXaxis()->SetRangeUser(-3., 2.9999);
  plotIt(fisher_bkg, ++iP, 7, .18, .45, .18); 

  if (doFeed) {
    RooPlot* fisher_feed = f->Get("sPlot_fisher_nChls");
  fisher_feed->GetXaxis()->SetRangeUser(-3., 2.9999);
    plotIt(fisher_feed, ++iP, 4, .35, .55, .70 );
  }

  if (twopage) { PDFcanv->SaveAs(modename+TString("_sPlot.eps")); }

  if (doMass) {
    //TCanvas *PDFcanv = new TCanvas("PDFcanv2", "", xpos, ypos, xsiz, ysiz);
    //if (doFeed) { PDFcanv2->Divide(3,3);}
    //else { PDFcanv2->Divide(2,3); }

    RooPlot* mOmega_sig = f->Get("sPlot_mOmega_nSig");
    plotIt(mOmega_sig, ++iP, 6, .22, .15, .55);

    RooPlot* mOmega_bkg = f->Get("sPlot_mOmega_nBkg");
    plotIt(mOmega_bkg, ++iP, 4, .20, .15, .65);

    if (doFeed) {
      RooPlot* mOmega_feed = f->Get("sPlot_mOmega_nChls");
      plotIt(mOmega_feed, ++iP, 6, .35, .15, .61);
    }

  }
  RooPlot* deltaT_sig = f->Get("sPlot_deltaT_nSig"); 
  deltaT_sig->GetXaxis()->SetRangeUser(-6., 5.9999);
  plotIt(deltaT_sig, ++iP, 6, .22, .70, .57); 

  RooPlot* deltaT_bkg = f->Get("sPlot_deltaT_nBkg");
  deltaT_bkg->GetXaxis()->SetRangeUser(-6., 5.9999);
  plotIt(deltaT_bkg, ++iP, 3, .18, .15, .20); 

  if (doFeed) {
    RooPlot* deltaT_feed = f->Get("sPlot_deltaT_nChls");
    deltaT_feed->GetXaxis()->SetRangeUser(-8., 8.);
    plotIt(deltaT_feed, ++iP, 6, .32, .15, .20);
  }

  if (twopage) {
    PDFcanv2->SaveAs(modename+TString("_sPlot2.eps"));
  } else {
    PDFcanv->SaveAs(modename+TString("_sPlot.eps"));
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
