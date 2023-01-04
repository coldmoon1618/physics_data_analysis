void LLR(Char_t *f, Char_t *n)
{
  LLR(TString(f), TString(n));
}

void LLR(TString f, TString ds)
{
  Double_t hMin=.01; //.2; //.11, //.05,
  Double_t scale=.01;
  TH1F *hDS=mergeHist(f, ds, kTRUE);
  TH1F *LLR_mlFitter=mergeHist(f, "LLR_mlFitter");
  TH1F *LLR_Bkg1=mergeHist(f, "LLR_Bkg1");
  TH1F *LLR_Bkg2=0;
  TH1F *LLR_BBPdf=0;
  TH1F *LLR_KKKst=mergeHist(f, "LLR_KKKst");
  Double_t scale_KKKst=scale/10;
  TH1F *LLR_KKPiZ=mergeHist(f, "LLR_KKPiZ");
  Double_t scale_KKPiZ=scale/100;
  TH1F *LLR_Sig1=mergeHist(f, "LLR_Sig1");
  Double_t scale_Sig1=scale/10;
  LLR(hDS, hMin, LLR_mlFitter, scale,
      LLR_Bkg1, scale, LLR_Bkg2, scale,
      LLR_BBPdf, scale,
      LLR_KKKst, scale_KKKst, LLR_KKPiZ, scale_KKPiZ,
      LLR_Sig1, scale_Sig1);
}

void LLR(TFile &f, TString ds)
{
  Double_t hMin=.2;
  Double_t scale=.1;
  TH1F *hDS=(TH1F*)f.Get(ds);
  TH1F *LLR_mlFitter=(TH1F*)f.Get("LLR_mlFitter");
  TH1F *LLR_Bkg1=(TH1F*)f.Get("LLR_Bkg1");
  TH1F *LLR_Bkg2=0;
  TH1F *LLR_BBPdf=0;
  TH1F *LLR_KKKst=(TH1F*)f.Get("LLR_KKKst");
  TH1F *LLR_KKPiZ=(TH1F*)f.Get("LLR_KKPiZ");
  TH1F *LLR_Sig1=(TH1F*)f.Get("LLR_Sig1");
  LLR(hDS, hMin, LLR_mlFitter, scale,
      LLR_Bkg1, scale, LLR_Bkg2, scale,
      LLR_BBPdf, scale,
      LLR_KKKst, scale, LLR_KKPiZ, scale,
      LLR_Sig1, scale);
}

void LLR(TH1F *hDS, Double_t min,
	 TH1F *LLR_mlFitter, Double_t scale_mlFitter,
	 TH1F *LLR_Bkg1, Double_t scale_Bkg1,
	 TH1F *LLR_Bkg2, Double_t scale_Bkg2,
	 TH1F *LLR_BBPdf, Double_t scale_BBPdf,
	 TH1F *LLR_KKKst, Double_t scale_KKKst,
	 TH1F *LLR_KKPiZ, Double_t scale_KKPiZ,
	 TH1F *LLR_Sig1, Double_t scale_Sig1
         )
{
  if (!hDS) {
    cout<<" Can not find data points"<<endl;
    return;
  }
  hDS->SetTitle();
  hDS->SetStats(kFALSE);
  hDS->SetMarkerStyle(8);
  hDS->SetMarkerSize(.4);
  hDS->SetMinimum(min);
  hDS->Draw("err");
  
  if (LLR_mlFitter) LLR(LLR_mlFitter, scale_mlFitter, 2, kGreen, 3004);
  if (LLR_Bkg1) LLR(LLR_Bkg1, scale_Bkg1, 2, kRed, 3005);
  if (LLR_Bkg2) LLR(LLR_Bkg2, scale_Bkg2, 2, kRed, 3005);
  if (LLR_BBPdf) LLR(LLR_BBPdf, scale_BBPdf, 2, 6, 3004);
  if (LLR_KKKst) LLR(LLR_KKKst, scale_KKKst, 2, 4, 3005);
  if (LLR_KKPiZ) LLR(LLR_KKPiZ, scale_KKPiZ, 2, 28, 3001);
  if (LLR_Sig1) LLR(LLR_Sig1, scale_Sig1, 2, 13, 3002);
}

void LLR(TH1F *h, Double_t scale, Int_t w, Int_t c, Int_t s)
{
  h->Scale(scale);
  h->SetLineWidth(w);
  h->SetLineColor(c);
  h->SetFillColor(c);
  h->SetFillStyle(s);
  h->Draw("same");
}

TH1F *mergeHist(TString f, TString n, Bool_t firstOnly=kFALSE)
{
  TH1F *h(0);
  TChain *t=new TChain("");
  t->Add(f);
  TObjArray *fs=t->GetListOfFiles();
  TFile *fn=new TFile(fs->At(0)->GetTitle());
  h=(TH1F*)fn->Get(n);
  h=(TH1F*)h->Clone();
  if (firstOnly) return h;
  TList *list=new TList;
  for (Int_t i=0; i<fs->GetSize(); i++) {
    if (!fs->At(i)) continue;
    TFile *ff=new TFile(fs->At(i)->GetTitle());
    TH1F *myH=(TH1F*)ff->Get(n);
    myH->SetName(Form("%s @ %s", myH->GetName(), fs->At(i)->GetTitle()));
    list->Add(myH);
  }
  h->Reset();
  list->Print();
  h->Merge(list);
  return h;
}
