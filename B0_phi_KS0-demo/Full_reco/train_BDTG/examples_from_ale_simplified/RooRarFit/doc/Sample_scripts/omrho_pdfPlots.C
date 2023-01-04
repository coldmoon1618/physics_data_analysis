// Make PDF plots to canvases and save in files
//
//void pdfPlots(const char* rootFile, Bool_t doFeed = kFALSE)
void omrho_pdfPlots( Bool_t doFeed = kTRUE)
{
  gROOT->Reset();
  //  gROOT->LoadMacro("roostyle.C");
  gROOT->SetStyle("MY"); //defined in omrho_roostyle.C
  gROOT->ForceStyle();

  //Bool_t doKPi = kFALSE;  

    TFile* f = new TFile("results/Rhop_Run1-4_newcuts_oldpdf_PdfAct.root");
  //TFile* f = new TFile(rootFile);
  f->ls();


  Int_t  xsiz=700, ysiz=900, xpos=600, ypos=0, iPad = 0;
  Double_t xMin = 0.16, xMax = 0.90, yMin = 0.2, yMax = 0.91, pWid;

  TCanvas *PDFcanv = new TCanvas("PDFcanv ", "", xpos, ypos, xsiz, ysiz);

  if (doFeed) {
  PDFcanv->Divide(3,3);
  pWid = 0.35;
  } else {
  PDFcanv->Divide(2,3);
  pWid = 0.27;
  }

 
  ///////////////// DE ///////////////////
    RooPlot* de_the_deSig = f->Get("de_the_deSig"); 
    PDFcanv->cd(++iPad);
    //replaceParName(de_the_deSig); SetMarkerSize(de_the_deSig);
    de_the_deSig->GetXaxis()->SetTitle("#Delta E (GeV)  ");
    de_the_deSig->GetXaxis()->SetNdivisions(405);
    //moveParBox(de_the_deSig, 6, pWid+0.03, -0.99, -yMax); 
    de_the_deSig->Draw();

    RooPlot* de_the_deBkg = f->Get("de_the_deBkg");
    PDFcanv->cd(++iPad); 
    //replaceParName(de_the_deBkg); SetMarkerSize(de_the_deBkg); 
    de_the_deBkg->GetXaxis()->SetTitle("#Delta E (GeV)  ");
    de_the_deBkg->GetXaxis()->SetNdivisions(405);
    //moveParBox(de_the_deBkg, 3, pWid, xMin, yMin+0.07); 
    de_the_deBkg->Draw();
    
    if (doFeed) {
      RooPlot* de_the_deChls = f->Get("de_the_deChls");
      PDFcanv->cd(++iPad); 
      //replaceParName(de_the_deChls); SetMarkerSize(de_the_deChls);
      de_the_deChls->GetXaxis()->SetTitle("#Delta E (GeV)  ");
      de_the_deChls->GetXaxis()->SetNdivisions(405);
      //moveParBox(de_the_deChls, 4, 0.35, xMin, yMin+0.05); 
      de_the_deChls->Draw();
    }


    ////////////////////// mES ////////////////////////
    RooPlot* mes_the_mesSig = f->Get("mes_the_mesSig");
    RooPlot* mes_the_mesBkg = f->Get("mes_the_mesBkg");
    
    PDFcanv->cd(++iPad); 
    //replaceParName(mes_the_mesSig); SetMarkerSize(mes_the_mesSig);
    mes_the_mesSig->GetXaxis()->SetTitle("M_{ES} (GeV)  ");
    mes_the_mesSig->GetXaxis()->SetNdivisions(405);
    //moveParBox(mes_the_mesSig, 6, 1.17*pWid, xMin, -yMax); 
    mes_the_mesSig->Draw();
    
    PDFcanv->cd(++iPad); 
    //replaceParName(mes_the_mesBkg); SetMarkerSize(mes_the_mesBkg);
    mes_the_mesBkg->GetXaxis()->SetTitle("M_{ES} (GeV)  ");
    mes_the_mesBkg->GetXaxis()->SetNdivisions(405);
    //moveParBox(mes_the_mesBkg, 2, pWid, xMin, yMin+0.07); 
    mes_the_mesBkg->Draw();
    
    if (doFeed) {
      PDFcanv->cd(++iPad); 
      //replaceParName(mes_the_mesChls); SetMarkerSize(mes_the_mesChls);
      mes_the_mesChls->GetXaxis()->SetTitle("M_{ES} (GeV)  ");
      mes_the_mesChls->GetXaxis()->SetNdivisions(405);
      //moveParBox(mes_the_mesChls, 5, 0.35, xMin, -yMax);
      mes_the_mesChls->Draw();
    }

    ////////////////////// xF //////////////////////////////////
    RooPlot* fisher_the_fisSig = f->Get("fisher_the_fisSig");
    RooPlot* fisher_the_fisBkg = f->Get("fisher_the_fisBkg");
    
    PDFcanv->cd(++iPad); 
    //replaceParName(fisher_the_fisSig); SetMarkerSize(fisher_the_fisSig);
    //moveParBox(fisher_the_fisSig, 4, pWid, -xMax, -yMax); 
    fisher_the_fisSig->Draw();
    
    PDFcanv->cd(++iPad); 
    //replaceParName(fisher_the_fisBkg); SetMarkerSize(fisher_the_fisBkg);
    //  PDFcanv_11->SetLogy(1); fisher_the_fisBkg->SetMinimum(.1); 
    gPad->SetLogy(1); fisher_the_fisBkg->SetMinimum(.1); 
    //moveParBox(fisher_the_fisBkg, 7, pWid, 0.37, yMin); 
    fisher_the_fisBkg->Draw();
    
  if (doFeed) {
    PDFcanv->cd(++iPad); 
    //replaceParName(fisher_the_fisChls); SetMarkerSize(fisher_the_fisChls);
    // moveParBox(fisher_the_fisChls, 7, pWid, -xMax, -yMax); 
    fisher_the_fisChls->Draw();
  }


  TCanvas *PDFcanv1 = new TCanvas("PDFcanv1 ", "", xpos, ypos, xsiz, ysiz);
  if (doFeed) {
  PDFcanv1->Divide(3,4);
  pWid = 0.35;
  } else {
  PDFcanv1->Divide(2,4);
  pWid = 0.27;
  }
  iPad=0;

  ///////////////////// m(rho) ///////////////////////
    //RooPlot* mRho_the_mRSig = f->Get("mass_kstar_Sig");
    //RooPlot* mRho_the_mRBkg = f->Get("mass_kstar_Bkg");
    //RooPlot* mRho_the_mRChls = f->Get("mass_kstar_Chls");

  PDFcanv1->cd(++iPad); 
  //replaceParName(mRho_the_mRSig); SetMarkerSize(mRho_the_mRSig);
  mRho_the_mRSig->GetXaxis()->SetTitle("M_{#rho^{+}} (GeV)  ");
  //moveParBox(mRho_the_mRSig, .67, .6 , .99, .99); 
  mRho_the_mRSig->GetXaxis()->SetNdivisions(405);
  mRho_the_mRSig->Draw();

  PDFcanv1->cd(++iPad); 
  //replaceParName(mRho_the_mRBkg); SetMarkerSize(mRho_the_mRBkg);
  mRho_the_mRBkg->GetXaxis()->SetTitle("M_{#rho^{+}} (GeV)  ");
  //moveParBox(mRho_the_mRBkg, .69, .68 , .99, .99); 
  mRho_the_mRBkg->GetXaxis()->SetNdivisions(405);
  mRho_the_mRBkg->Draw();

  if (doFeed) {
    PDFcanv1->cd(++iPad); 
    //replaceParName(mRho_the_mRChls); SetMarkerSize(mRho_the_mRChls);
    mRho_the_mRChls->GetXaxis()->SetTitle("M_{#rho^{+}} (GeV)  ");
    //moveParBox(mRho_the_mRChls, .69, .68 , .99, .99);
    mRho_the_mRChls->GetXaxis()->SetNdivisions(405);
    mRho_the_mRChls->Draw();
  }

  ///////////////////// m(omega) ///////////////////////
  RooPlot* mOmega_the_mOSig = f->Get("mOmega_the_mOSig");
  RooPlot* mOmega_the_mOBkg = f->Get("mOmega_the_mOBkg");

  PDFcanv1->cd(++iPad); 
  //replaceParName(mOmega_the_mOSig); SetMarkerSize(mOmega_the_mOSig);
  mOmega_the_mOSig->GetXaxis()->SetTitle("M_{#omega} (GeV)  ");
  mOmega_the_mOSig->GetXaxis()->SetNdivisions(405);
  //moveParBox(mOmega_the_mOSig, 6, 1.17*pWid, xMin, -yMax); 
  mOmega_the_mOSig->Draw();

  PDFcanv1->cd(++iPad); 
  //replaceParName(mOmega_the_mOBkg); SetMarkerSize(mOmega_the_mOBkg);
  mOmega_the_mOBkg->GetXaxis()->SetTitle("M_{#omega} (GeV)  ");
  mOmega_the_mOBkg->GetXaxis()->SetNdivisions(405);
  //moveParBox(mOmega_the_mOBkg, 5, pWid, xMin, -yMax); 
  mOmega_the_mOBkg->Draw();
  //  moveParBox(mOmega_the_mOBkg, 3, pWid, xMin, yMin+0.1); mOmega_the_mOBkg->Draw();

  if (doFeed) {
    PDFcanv1->cd(++iPad); 
    //replaceParName(mOmega_the_mOChls); SetMarkerSize(mOmega_the_mOChls);
    mOmega_the_mOChls->GetXaxis()->SetTitle("M_{#omega} (GeV)  ");
    mOmega_the_mOChls->GetXaxis()->SetNdivisions(405);
    //moveParBox(mOmega_the_mOChls, 5, pWid, xMin, yMin);
    mOmega_the_mOChls->Draw();
  }

  ///////////////////// h(rho) ///////////////////////
    //RooPlot* hRho_the_rHSig = f->Get("mass_kstar_Sig");
    //RooPlot* hRho_the_rHBkg = f->Get("mass_kstar_Bkg");
    //RooPlot* hRho_the_rHChls = f->Get("mass_kstar_Chls");

  PDFcanv1->cd(++iPad); 
  //replaceParName(hRho_the_heliSig); SetMarkerSize(hRho_the_heliSig);
  hRho_the_heliSig->GetXaxis()->SetTitle("cos(#theta_{h}^{#rho})   ");
  //moveParBox(hRho_the_heliSig, .67, .6 , .99, .99); 
  hRho_the_heliSig->GetXaxis()->SetNdivisions(405);
  hRho_the_heliSig->Draw();

  PDFcanv1->cd(++iPad); 
  //replaceParName(hRho_the_ rHMBkg); SetMarkerSize(hRho_the_ rHMBkg);
  hRho_the_ rHMBkg->GetXaxis()->SetTitle("cos(#theta_{h}^{#rho})   ");
  //moveParBox(hRho_the_ rHMBkg, .69, .68 , .99, .99); 
  hRho_the_ rHMBkg->GetXaxis()->SetNdivisions(405);
  hRho_the_ rHMBkg->Draw();

  if (doFeed) {
    PDFcanv1->cd(++iPad); 
    //replaceParName(hRho_the_ rHChls); SetMarkerSize(hRho_the_ rHChls);
    hRho_the_rHChls->GetXaxis()->SetTitle("cos(#theta_{h}^{#rho})  ");
    //moveParBox(hRho_the_ rHChls, .69, .68 , .99, .99);
    hRho_the_rHChls->GetXaxis()->SetNdivisions(405);
    hRho_the_rHChls->Draw();
  }

  ///////////////////// h(omega) ///////////////////////

  PDFcanv1->cd(++iPad); 
  //replaceParName(hOmega_the_heliSig); SetMarkerSize(hOmega_the_heliSig);
  hOmega_the_heliSig->GetXaxis()->SetTitle("|cos(#theta_{h}^{#omega})|  ");
  hOmega_the_heliSig->GetXaxis()->SetNdivisions(405);
  //moveParBox(hOmega_the_heliSig, 6, 1.17*pWid, xMin, -yMax); 
  hOmega_the_heliSig->Draw();

  PDFcanv1->cd(++iPad); 
  //replaceParName(hOmega_the_oHMBkg); SetMarkerSize(hOmega_the_oHMBkg);
  hOmega_the_oHMBkg->GetXaxis()->SetTitle("|cos(#theta_{h}^{#omega})|  ");
  hOmega_the_oHMBkg->GetXaxis()->SetNdivisions(405);
  //moveParBox(hOmega_the_oHMBkg, 5, pWid, xMin, -yMax); 
  hOmega_the_oHMBkg->Draw();
  //  moveParBox(hOmega_the_oHMBkg, 3, pWid, xMin, yMin+0.1); hOmega_the_oHMBkg->Draw();

  if (doFeed) {
    PDFcanv1->cd(++iPad); 
    //replaceParName(hOmega_the_oHChls); SetMarkerSize(hOmega_the_oHChls);
    hOmega_the_oHChls->GetXaxis()->SetTitle("|cos(#theta_{h}^{#omega})|  ");
    hOmega_the_oHChls->GetXaxis()->SetNdivisions(405);
    // moveParBox(hOmega_the_oHChls, 5, pWid, xMin, yMin);
 TPaveStats* pbox = (TPaveStats*) hOmega_the_oHChls->findObject("TPave");
 pbox->TPaveStats::SetOptStat(000000);

    hOmega_the_oHChls->Draw();
  }

}
 


void moveParBox(RooPlot* plot, Double_t x1, Double_t y1, Double_t x2, Double_t y2) {
  TPave* pbox = (TPave*) plot->findObject("TPave");
  pbox->SetX1(x1);
  pbox->SetY1(y1);
  pbox->SetX2(x2);
  pbox->SetY2(y2);
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
}

void replaceParName(RooPlot* plot) {
  //first find the text pave
  TPaveText* pbox = (TPaveText*) plot->findObject("TPave");
  replaceParName(pbox);
}

void replaceParName(TPaveText* pbox) {
  TList* parList=pbox->GetListOfLines();
  TIterator* parIter=parList->MakeIterator();
  TText *nextParText(0);
  while(0!=(nextParText=(TText *)parIter->Next())) {
    replaceParName(nextParText);
  }
}

void replaceParName(TText* parText) {
  TString parStr=parText->GetTitle();
  parStr.ReplaceAll(" ", ""); //we do not need space here
  TString parNameStr=parStr(0, parStr.Index("="));
  
  //general name should be put as late as possible
  if (parNameStr.Contains("ThisIsADummy")) parNameStr="ThisIsADummy";
  else if(parNameStr.Contains("SigScf")) parNameStr="f_{scale}";
  else if(parNameStr.Contains("SigMoff")) parNameStr="#mu_{offset}";
  else if(parNameStr.Contains("SigmaT")) parNameStr="#sigma_{tail}";
  else if(parNameStr.Contains("SigmaC")) parNameStr="#sigma_{core}";
  else if(parNameStr.Contains("MeanT")) parNameStr="#mu_{tail}";
  else if(parNameStr.Contains("MeanC")) parNameStr="#mu_{core}";
  else if(parNameStr.Contains("SigrC")) parNameStr="#sigma_{rCore}";
  else if(parNameStr.Contains("SiglC")) parNameStr="#sigma_{lCore}";
  else if(parNameStr.Contains("Sigr")) parNameStr="#sigma_{r}";
  else if(parNameStr.Contains("Sigl")) parNameStr="#sigma_{l}";
  else if(parNameStr.Contains("FracT")) parNameStr="f_{tail}";
  else if(parNameStr.Contains("FracC")) parNameStr="f_{core}";
  else if(parNameStr.Contains("FracS")) parNameStr="f_{sig}";
  else if(parNameStr.Contains("Frac")) parNameStr="f_{peak}";
  else if(parNameStr.Contains("Slope")) parNameStr="Slope";
  else if(parNameStr.Contains("Coef1")) parNameStr="p_{1}";
  else if(parNameStr.Contains("Coef2")) parNameStr="p_{2}";
  else if(parNameStr.Contains("Thres")) parNameStr="Thres";
  else if(parNameStr.Contains("mbBkgCa")) parNameStr="AGSPar";
  else if(parNameStr.Contains("mbFeedCa")) parNameStr="AGSPar";
  else if(parNameStr.Contains("Mean")) parNameStr="#mu";
  else if(parNameStr.Contains("Tail")) parNameStr="Tail";
  else if(parNameStr.Contains("Cub")) parNameStr="Cub";
  else if(parNameStr.Contains("Quad")) parNameStr="Quad";
  else if(parNameStr.Contains("Sigma")) parNameStr="#sigma";

  parStr=parNameStr+parStr(parStr.Index("="), parStr.Length());
  parText->SetTitle(parStr);

}

void SetMarkerSize(RooPlot* plot){
  //RooHist* hist= (RooHist*) plot->findObject("TPave");
  RooHist* hist = (RooHist*) plot->findObject(0,RooHist::Class()) ;
  hist->SetMarkerSize(.5);
}
