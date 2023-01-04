#ifndef MY_SET_STYLE
#define MY_SET_STYLE

struct MySetStyle{
};

inline void setStyle(TH1F *h, int color, int width, const char *xtitle, const char *ytitle, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(width);
    h -> GetXaxis() -> SetTitle(xtitle);
    h -> GetYaxis() -> SetTitle(ytitle);
    h -> SetTitle(title);
}

inline void setStyle(TH1F *h, int color, int width, const char *xtitle){
    h -> SetLineColor(color);
    h -> SetLineWidth(width);
    h -> GetXaxis() -> SetTitle(xtitle);
}

inline void setStyle(TH1F *h, int color, int width, const char *xtitle, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(width);
    h -> GetXaxis() -> SetTitle(xtitle);
    h -> SetTitle(title);
}

inline void setStyle(TH1F *h, int color, int width){
    h -> SetLineColor(color);
    h -> SetLineWidth(width);
}

inline void setStyleTH2F(TH2F *H, const char *xtitle, const char *ytitle, const char *title){
    H -> GetXaxis() -> SetTitle(xtitle);
    H -> GetYaxis() -> SetTitle(ytitle);
    H -> SetTitle(title);
}

inline void normalize(TH1F *h){
    double scale = 1.0/(h->GetEntries());
    h->Scale(scale);
}

inline void drawRatioPlot(TRatioPlot *rp, double maxY, double minY, int yDivisions, double line, TCanvas *c){
    rp -> Draw();
    rp -> GetLowerRefGraph() -> SetMaximum(maxY);
    rp -> GetLowerRefGraph() -> SetMinimum(minY);
    rp -> GetLowYaxis() -> SetNdivisions(yDivisions);
    rp -> SetGridlines({line});
    c  -> Update();
}

inline void DrawStacked(TH1F *histo1, TH1F *histo2, TH1F *histo3, TString varName, 
    TString plotTitle, 
    TString pdfName){
    TH1F *h1 = (TH1F*)histo1->Clone();
    TH1F *h2 = (TH1F*)histo2->Clone();
    TH1F *h3 = (TH1F*)histo3->Clone();
        
    //h3->SetFillColor(kOrange+7);
    h3->SetFillColor(kRed+2);
    h2->SetFillColor(kOrange-3);
    h1->SetFillColor(kGreen+2);

    THStack *hs = new THStack("hs1_Mbc","");
    hs->Add(h3);
    hs->Add(h2);
    hs->Add(h1);

    TCanvas *c1 = new TCanvas(pdfName,"",800,600);

    TLatex T;
    T.SetTextFont(42); 
    T.SetTextAlign(21);
    T.SetNDC();
    hs -> Draw();
    gPad->BuildLegend(0.7,0.7,0.9,0.9,"");
    T.DrawLatex(.5,.93,plotTitle);
    T.DrawTextNDC(.9,.005,varName);
    
    c1 -> SaveAs(pdfName);
}

inline void DrawStacked(TH1F *histo1, TH1F *histo2, TH1F *histo3, TH1F *histo4, TString varName, 
    TString plotTitle, 
    TString pdfName){
    TH1F *h1 = (TH1F*)histo1->Clone();
    TH1F *h2 = (TH1F*)histo2->Clone();
    TH1F *h3 = (TH1F*)histo3->Clone();
    TH1F *h4 = (TH1F*)histo4->Clone();
        
    h4->SetFillColor(kRed+2);
    h3->SetFillColor(kOrange+7);
    h2->SetFillColor(kOrange-3);
    h1->SetFillColor(kGreen+2);

    THStack *hs = new THStack("hs1_Mbc","");
    hs->Add(h4);
    hs->Add(h3);
    hs->Add(h2);
    hs->Add(h1);

    TCanvas *c1 = new TCanvas(pdfName,"",800,600);

    TLatex T;
    T.SetTextFont(42); 
    T.SetTextAlign(21);
    T.SetNDC();
    hs -> Draw();
    gPad->BuildLegend(0.7,0.7,0.9,0.9,"");
    T.DrawLatex(.5,.93,plotTitle);
    T.DrawTextNDC(.9,.005,varName);
    
    c1 -> SaveAs(pdfName);
}
#endif
