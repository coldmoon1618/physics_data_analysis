#ifndef MY_SET_STYLE
#define MY_SET_STYLE

struct MySetStyle{
};

inline void setStyle(TH1F *h, int color, const char *xtitle, const char *ytitle, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(1);
    h -> GetXaxis() -> SetTitle(xtitle);
    h -> GetYaxis() -> SetTitle(ytitle);
    h -> SetTitle(title);
}

inline void setStyle(TH1F *h, int color, const char *xtitle){
    h -> SetLineColor(color);
    h -> SetLineWidth(1);
    h -> GetXaxis() -> SetTitle(xtitle);
}

inline void setStyle(TH1F *h, int color, const char *xtitle, const char *title){
    h -> SetLineColor(color);
    h -> SetLineWidth(1);
    h -> GetXaxis() -> SetTitle(xtitle);
    h -> SetTitle(title);
}

inline void setStyle(TH1F *h, int color){
    h -> SetLineColor(color);
    h -> SetLineWidth(1);
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
#endif
