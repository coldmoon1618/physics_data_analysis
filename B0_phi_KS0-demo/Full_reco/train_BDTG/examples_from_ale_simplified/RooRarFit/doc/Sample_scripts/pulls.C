//
//  For selected variables in toy fit results tree, print mean and sigma of
//   pull (from a Gauss fit), and mean error.
//
void pulls(char *dirName, char *fileName="all.root", char *oPrefix="pulls")
{
  // Call with the dirname for toys, e.g. etoy-omks
  TFile *f=new TFile(Form("%s/%s", dirName, fileName));
  TTree* fitParData(0);
  fitParData = (TTree*)f->Get("toyResults");
  if (!fitParData) fitParData = (TTree*)f->Get("fitResults");
  if (!fitParData) {
    cout<<"Can not find toy results in file "
	<<Form("%s/%s", dirName, fileName)<<endl;
    return;
  }
  
  ofstream poutFile(Form("%s.p.out", oPrefix));
  ofstream voutFile(Form("%s.v.out", oPrefix));
  poutFile <<"       Pulls      mean              sigma           mean err" 
	   <<endl;
  voutFile <<"      Params      mean              sigma"<< endl;
  
  TObjArray *leafList=fitParData->GetListOfLeaves();
  Int_t nLeaves=leafList->GetSize();
  for (Int_t i=0; i<nLeaves; i++) {
    TString lName=leafList->At(i)->GetName();
    pullPrint(lName, fitParData, poutFile, voutFile);
  }
  
  // close output
  poutFile.close();
  voutFile.close();
}

void pullPrint(char* var, TTree* tree, ostream& poutFile, ostream& voutFile)
{
  TString pull(var); pull.Append("pull");
  TString err(var); err.Append("err");
  char lineBuffer[81], label[20];
  strncpy(label, var, 20);  label[20-1] = '\0';
  if (!tree->GetLeaf(var)) return;
  if (!tree->GetLeaf(err)) return;
  if (!tree->GetLeaf(pull)) return;
  { // for pull
    tree->Draw(err);
    Double_t meanErr=htemp->GetMean();
    if (tree->GetLeaf(pull+"_new")) pull+="_new";
    if (tree->GetLeaf(pull+"_embd")) pull+="_embd";
    tree->Draw(pull, "abs("+pull+")<10");
    // check if mean is nan
    if(htemp->GetRMS()==0) return;
    htemp->Fit("gaus", "L");
    TF1 *fitResult=htemp->GetFunction("gaus");
    Double_t mean = fitResult->GetParameter(1);
    Double_t Dmean = fitResult->GetParError(1);
    Double_t sigma = fitResult->GetParameter(2);
    Double_t Dsigma = fitResult->GetParError(2);
    sprintf(lineBuffer,
	    "%12s   %7.2f +/-%5.2f  %7.2f +/-%5.2f   %6.4f",
	    label, mean, Dmean, sigma, Dsigma, meanErr);
    poutFile << lineBuffer << endl;
    gPad->Update();
    cout<<"Press Enter to continue"<<endl;
    getchar();
  }
  { // for value itself
    tree->Draw(var);
    htemp->Fit("gaus", "L");
    TF1 *fitResult=htemp->GetFunction("gaus");
    Double_t mean = fitResult->GetParameter(1);
    Double_t Dmean = fitResult->GetParError(1);
    Double_t sigma = fitResult->GetParameter(2);
    Double_t Dsigma = fitResult->GetParError(2);
    sprintf(lineBuffer,
	    "%12s   %7.2f +/-%5.2f  %7.2f +/-%5.2f",
	    label, mean, Dmean, sigma, Dsigma);
    voutFile << lineBuffer << endl;
    gPad->Update();
    cout<<"Press Enter to continue"<<endl;
    getchar();
  }
}
