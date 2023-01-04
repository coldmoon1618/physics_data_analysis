//Script to convert charge fraction NLL curve into charge asymmetry
//Scale factor widens curve (central value and width) to account for 
//dilution effect due to the fit bias

void combine_rescale(Char_t* filename, Float_t scale)
{
  TFile* file = new TFile(filename,"update");
  file->cd();
  if (file->Get("NLL_rescaled")!=0) { cout << "Curve already rescaled." << endl; continue;}
  RooPlot* plot = file->Get("NLLScanPlot_Frac_nSigP_chgCat_Plus");
  RooCurve* curve = (RooCurve*)plot->getObject(0);
  RooCurve* newCurve = new RooCurve();
  RooPlot* newPlot = new RooPlot(-1,1,0,plot->GetMaximumStored());

Int_t nPoints=curve->GetN();
  for (Int_t j=0; j<nPoints; j++) {
  Double_t x, y;
  curve->GetPoint(j,x,y);
  newCurve->SetPoint(j,(scale)*(1-2*x),y);
  }

  newCurve->SetName("NLL_curve");
  newCurve->SetTitle("NLL_curve");
  newPlot->addObject(newCurve);

  newPlot->SetName("NLL_rescaled");
  newPlot->SetTitle("NLL_rescaled");

  file->Add(newPlot);
  file->Write();
  file->Close();
}
