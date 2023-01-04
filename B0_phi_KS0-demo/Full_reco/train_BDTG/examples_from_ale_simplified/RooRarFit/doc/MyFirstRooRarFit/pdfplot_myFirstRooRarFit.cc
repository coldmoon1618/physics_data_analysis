// Make PDF plots for rho gamma (6 rows, no BBbar)
//

void pdfplot_myFirstRooRarFit(TString &filename)
{
  gROOT->Reset();
  gROOT->SetStyle("BABAR");

  Bool_t doMass = kTRUE;
  Char_t* modename = "myFirstRooRarFit";
  Int_t  xsiz=700, ysiz=900, xpos=600, ypos=0, iP=0, iP2=0, row1=0; 
  Int_t row=2;
  TString deltaE_Sig;
  TString deltaE_Cont;
  TString mES_Sig;
  TString mES_Cont;
  TString plotMode;
  Bool_t isProjPlot=kFALSE;
  Bool_t hasTPave=kFALSE;


  // Figure out which file is loaded
  if( filename.Contains("pdfPlot") ) {
    cout << "pdfPlot" << endl;
    deltaE_Sig  = "deltaE_the_deSignal";
    deltaE_Cont = "deltaE_the_deContinuum";
    mES_Sig  = "mES_the_mesSignal";
    mES_Cont = "mES_the_mesContinuum";
    hasTPave=kTRUE;
    plotMode = "pdfPlot";
  }
  if( filename.Contains("sPlot") ) {
    cout << "sPlot" << endl;
    deltaE_Sig  = "sPlot_deltaE_nSignal";
    deltaE_Cont = "sPlot_deltaE_nContinuum";
    mES_Sig  = "sPlot_mES_nSignal";
    mES_Cont = "sPlot_mES_nContinuum";
    plotMode = "sPlot";
  }
  if( filename.Contains("projPlot") ) {
    cout << "projPlot" << endl;
    deltaE_Sig  = "proj_deltaE";
    mES_Sig  = "proj_mES";
    isProjPlot=kTRUE;
    plotMode = "projPlot";
    row = 2;
  }

  TFile* f = new TFile(filename);
  //TFile* f = new TFile(TString("FitResults/pdfPlot_")+modename+TString(".root"));
  f->ls();

  TCanvas *PDFcanv = new TCanvas("PDFcanv", "", xpos, ypos, xsiz, ysiz);
  row1 = row;
  if (!(plotMode == "projPlot") ) {
    PDFcanv->Divide(2,row1);
  } else if (plotMode == "projPlot" ) {
    PDFcanv->Divide(2,row1);
  } 
  else {
    PDFcanv->Divide(2,row1);
  }


  ///////////////// DE ///////////////////
  RooPlot* deltae_sig = f->Get(deltaE_Sig); 
  plotIt(deltae_sig, ++iP, 6, .55, .15, .55, hasTPave);

  if ( !isProjPlot ) {
    RooPlot* deltae_bkg = f->Get(deltaE_Cont);
    plotIt(deltae_bkg, ++iP, 4, .45, .15, .20, hasTPave);
  }


  ////////////////////// mES ////////////////////////
  RooPlot* mB_sig = f->Get(mES_Sig);
  plotIt(mB_sig, ++iP, 6, .55, .15, .55, hasTPave);
  
  if ( !isProjPlot ) {
    RooPlot* mB_bkg = f->Get(mES_Cont);
    plotIt(mB_bkg, ++iP, 4, .50, .15, .20, hasTPave);
  }



  PDFcanv->SaveAs(modename+TString("_")+plotMode+TString("_PDF.eps"));
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


void plotIt(RooPlot* plot, 
	    Int_t iP, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot, 
	    Bool_t &hasParBox) {
  if (!plot) return;
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  if (hist) hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(1.72);
  plot->GetXaxis()->SetNdivisions(405);
  PDFcanv->cd(iP);
  if( hasParBox ) moveParBox(plot,nlines,width,xLeft,yBot);
  plot->Draw();
}


void plotIt2(RooPlot* plot, 
	     Int_t iP2, Int_t nlines, Double_t width, Double_t xLeft, Double_t yBot,
	     Bool_t &hasParBox) {
  if (!plot) cout << "Can't find plot " << iP2 << " on Canvas 2" << endl;
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.6);
  plot->GetYaxis()->SetTitleOffset(1.72);
  plot->GetXaxis()->SetNdivisions(405);
  PDFcanv2->cd(iP2);
  if( hasParBox ) moveParBox(plot,nlines,width,xLeft,yBot); 
  plot->Draw();
}















