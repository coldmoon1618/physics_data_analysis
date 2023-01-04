// Make PDF plots for omh
//

void splot_omh()
{
  gROOT->Reset();
  gROOT->SetStyle("BABAR");
  
  Bool_t doKPi=kTRUE, twopage=kTRUE, doFeed = kTRUE, doMass = kTRUE;
  Char_t* modename = "omh";
  Int_t  xsiz=700, ysiz=900, xpos=600, ypos=0, iP=0, row1=0; 
  Int_t row=6;
  
  TFile* f = new TFile(TString("results/omh.mlFitter_Config.sPlot.SPlotAct.root"));
  f->ls();
  
  TCanvas *PDFcanv = new TCanvas("PDFcanv", "", xpos, ypos, xsiz, ysiz);
  row1 = row;
  if(twopage) {row1=6;}
  if (doFeed) {
    PDFcanv->Divide(3,row1);
    pWid = 0.35;
  } else {
    PDFcanv->Divide(2,row1);
    pWid = 0.27;
  }
  
  ///////////////// DE ///////////////////
    
    RooPlot* deltae_sigK = f->Get("sPlot_de_nSigK"); 
    plotItNoBox(deltae_sigK, ++iP, 6, .22, .70, .57);
    
    RooPlot* deltae_bkgK = f->Get("sPlot_de_nBkgK");
    plotItNoBox(deltae_bkgK, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* deltae_feedK = f->Get("sPlot_de_nChlsK");
      plotItNoBox(deltae_feedK, ++iP, 6, .22, .70, .57);
    }

    RooPlot* deltae_sigP = f->Get("sPlot_de_nSigP"); 
    plotItNoBox(deltae_sigP, ++iP, 6, .22, .70, .57);
    
    RooPlot* deltae_bkgP = f->Get("sPlot_de_nBkgP");
    plotItNoBox(deltae_bkgP, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* deltae_feedP = f->Get("sPlot_de_nChlsP");
      plotItNoBox(deltae_feedP, ++iP, 6, .22, .70, .57);
    }

    ////////////////////// mES ////////////////////////

    RooPlot* mes_sigK = f->Get("sPlot_mes_nSigK"); 
    plotItNoBox(mes_sigK, ++iP, 6, .22, .70, .57);
    
    RooPlot* mes_bkgK = f->Get("sPlot_mes_nBkgK");
    plotItNoBox(mes_bkgK, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* mes_feedK = f->Get("sPlot_mes_nChlsK");
      plotItNoBox(mes_feedK, ++iP, 6, .22, .70, .57);
    }

    RooPlot* mes_sigP = f->Get("sPlot_mes_nSigP"); 
    plotItNoBox(mes_sigP, ++iP, 6, .22, .70, .57);
    
    RooPlot* mes_bkgP = f->Get("sPlot_mes_nBkgP");
    plotItNoBox(mes_bkgP, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* mes_feedP = f->Get("sPlot_mes_nChlsP");
      plotItNoBox(mes_feedP, ++iP, 6, .22, .70, .57);
    }

    ////////////////////// pull ////////////////////////

    RooPlot* pull_sigK = f->Get("sPlot_pullKaon_nSigK"); 
    plotItNoBox(pull_sigK, ++iP, 6, .22, .70, .57);
    
    //RooPlot* pull_bkgK = f->Get("sPlot_pullPion_nBkgK");
    //plotItNoBox(pull_bkgK, ++iP, 3, .18, .15, .20);
    iP++; iP++;
    //RooPlot* pull_feedK = f->Get("sPlot_pullPion_nChlsK");
    // plotItNoBox(pull_feedK, ++iP, 6, .22, .70, .57);
    
    RooPlot* pull_sigP = f->Get("sPlot_pullPion_nSigP"); 
    plotItNoBox(pull_sigP, ++iP, 6, .22, .70, .57);
    
    //RooPlot* pull_bkgP = f->Get("sPlot_pullPion_nBkgP");
    //plotItNoBox(pull_bkgP, ++iP, 3, .18, .15, .20);
    
    //RooPlot* pull_feedP = f->Get("sPlot_pullPion_nChlsP");
    //plotItNoBox(pull_feedP, ++iP, 6, .22, .70, .57);

    PDFcanv->SaveAs(modename+TString("_sPlot1.eps"));
  TCanvas *PDFcanv = new TCanvas("PDFcanv2", "", xpos, ypos, xsiz, ysiz);
    PDFcanv2->Divide(3,6);
    iP=0;

    
    ////////////////////// xF //////////////////////////////////

    RooPlot* fisher_sigK = f->Get("sPlot_fisher_nSigK"); 
    plotIt2NoBox(fisher_sigK, ++iP, 6, .22, .70, .57);
    
    RooPlot* fisher_bkgK = f->Get("sPlot_fisher_nBkgK");
    plotIt2NoBox(fisher_bkgK, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* fisher_feedK = f->Get("sPlot_fisher_nChlsK");
      plotIt2NoBox(fisher_feedK, ++iP, 6, .22, .70, .57);
    }

    RooPlot* fisher_sigP = f->Get("sPlot_fisher_nSigP"); 
    plotIt2NoBox(fisher_sigP, ++iP, 6, .22, .70, .57);
    
    RooPlot* fisher_bkgP = f->Get("sPlot_fisher_nBkgP");
    plotIt2NoBox(fisher_bkgP, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* fisher_feedP = f->Get("sPlot_fisher_nChlsP");
      plotIt2NoBox(fisher_feedP, ++iP, 6, .22, .70, .57);
    }

    ////////////////////// heli //////////////////////////////////
 
       RooPlot* heli_sigK = f->Get("sPlot_heli_nSigK"); 
    plotIt2NoBox(heli_sigK, ++iP, 6, .22, .70, .57);
    
    RooPlot* heli_bkgK = f->Get("sPlot_heli_nBkgK");
    plotIt2NoBox(heli_bkgK, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* heli_feedK = f->Get("sPlot_heli_nChlsK");
      plotIt2NoBox(heli_feedK, ++iP, 6, .22, .70, .57);
    }

    RooPlot* heli_sigP = f->Get("sPlot_heli_nSigP"); 
    plotIt2NoBox(heli_sigP, ++iP, 6, .22, .70, .57);
    
    RooPlot* heli_bkgP = f->Get("sPlot_heli_nBkgP");
    plotIt2NoBox(heli_bkgP, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* heli_feedP = f->Get("sPlot_heli_nChlsP");
      plotIt2NoBox(heli_feedP, ++iP, 6, .22, .70, .57);
    }    
    ////////////////////// mOmega //////////////////////////////////
 
       RooPlot* mOmega_sigK = f->Get("sPlot_mOmega_nSigK"); 
    plotIt2NoBox(mOmega_sigK, ++iP, 6, .22, .70, .57);
    
    RooPlot* mOmega_bkgK = f->Get("sPlot_mOmega_nBkgK");
    plotIt2NoBox(mOmega_bkgK, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* mOmega_feedK = f->Get("sPlot_mOmega_nChlsK");
      plotIt2NoBox(mOmega_feedK, ++iP, 6, .22, .70, .57);
    }

    RooPlot* mOmega_sigP = f->Get("sPlot_mOmega_nSigP"); 
    plotIt2NoBox(mOmega_sigP, ++iP, 6, .22, .70, .57);
    
    RooPlot* mOmega_bkgP = f->Get("sPlot_mOmega_nBkgP");
    plotIt2NoBox(mOmega_bkgP, ++iP, 3, .18, .15, .20);
    
    if (doFeed) {
      RooPlot* mOmega_feedP = f->Get("sPlot_mOmega_nChlsP");
      plotIt2NoBox(mOmega_feedP, ++iP, 6, .22, .70, .57);
    }    


    PDFcanv2->SaveAs(modename+TString("_sPlot2.eps"));
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
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.6);
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
void plotItNoBox(RooPlot* plot, Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot) {
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(1.72);
  plot->GetXaxis()->SetNdivisions(405);
  PDFcanv->cd(iP);
  plot->Draw();
}
void plotIt2NoBox(RooPlot* plot, Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot) {
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(1.72);
  plot->GetXaxis()->SetNdivisions(405);
  PDFcanv2->cd(iP);
  plot->Draw();
}
