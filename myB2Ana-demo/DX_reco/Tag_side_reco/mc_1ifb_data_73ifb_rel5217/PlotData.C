#include <iostream>
#include <fstream>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TString.h>
#include <TMath.h>
#include <TChain.h>

void PlotData(int myMode=0, double minProb=0.001){

    TChain *B_cand = new TChain("B_cand");
    B_cand -> AddFile("Data_cand.root");
    double Mbc;
    B_cand -> SetBranchAddress("Mbc", &Mbc);
    double decMode;
    B_cand -> SetBranchAddress("decMode", &decMode);
    double SigProb;
    B_cand -> SetBranchAddress("SigProb", &SigProb);

    int Entries = B_cand -> GetEntries();

    TH1F *h_Mbc = new TH1F("Mbc","",30,5.24,5.3);

    const int nModes = 36;
    const char *fState[nModes] = {
/*  0 */"#bar{D}^{0}#pi^{+}",
/*  1 */"#bar{D}^{0}#pi^{+}#pi^{0}",
/*  2 */"#bar{D}^{0}#pi^{+}#pi^{0}#pi^{0}",
/*  3 */"#bar{D}^{0}#pi^{+}#pi^{+}#pi^{-}",
/*  4 */"#bar{D}^{0}#pi^{+}#pi^{+}#pi^{-}#pi^{0}",

/*  5 */"#bar{D}^{0}D^{+}",
/*  6 */"#bar{D}^{0}D^{+}K_{S}^{0}",
/*  7 */"#bar{D}^{0*}D^{+}K_{S}^{0}",
/*  8 */"#bar{D}^{0}D^{+*}K_{S}^{0}",
/*  9 */"#bar{D}^{0*}D^{+*}K_{S}^{0}",

/* 10 */"#bar{D}^{0}D^{0}K^{+}",
/* 11 */"#bar{D}^{0*}D^{0}K^{+}",
/* 12 */"#bar{D}^{0}D^{0*}K^{+}",
/* 13 */"#bar{D}^{0*}D^{0*}K^{+}",

/* 14 */"D^{+}_{s}#bar{D^{0}}",

/* 15 */"#bar{D}^{0*}#pi^{+}",
/* 16 */"#bar{D}^{0*}#pi^{+}#pi^{0}",
/* 17 */"#bar{D}^{0*}#pi^{+}#pi^{0}#pi^{0}",
/* 18 */"#bar{D}^{0*}#pi^{+}#pi^{+}#pi^{-}",
/* 19 */"#bar{D}^{0*}#pi^{+}#pi^{+}#pi^{-}#pi^{0}",

/* 20 */"D^{+*}_{s}#bar{D}^{0}",
/* 21 */"D^{+}_{s}#bar{D}^{0*}",

/* 22 */"#bar{D}^{0}K^{+}",

/* 23 */"D^{-}#pi^{+}#pi^{+}",
/* 24 */"D^{-}#pi^{+}#pi^{+}#pi^{0}",

/* 25 */"J/#psi K^{+}",
/* 26 */"J/#psi K^{+}#pi^{+}#pi^{-}",
/* 27 */"J/#psi K^{+}#pi^{0}",
/* 28 */"J/#psi K^{0}_{S}#pi^{+}",

/* 29 */"#Lambda_{c}^{-}p#pi^{+}#pi^{0}",
/* 30 */"#Lambda_{c}^{-}p#pi^{+}#pi^{-}#pi^{+}",

/* 31 */"#bar{D}^{0}p#bar{p}#pi^{+}",
/* 32 */"#bar{D}^{0*}p#bar{p}#pi^{+}",
/* 33 */"D^{+}p#bar{p}#pi^{+}#pi^{-}",
/* 34 */"D^{+*}p#bar{p}#pi^{+}#pi^{-}",

/* 35 */"#Lambda_{c}^{-}p#pi^{+}",
        }; 

    int counter = 0;
    for(int i=0; i<Entries; i++){
        B_cand -> GetEntry(i);

        if(decMode!=myMode*1.0)continue;
        if(isnan(Mbc))continue;
        if(Mbc<=5.24)continue;
        if(SigProb<=minProb)continue;
        
        h_Mbc->Fill(Mbc);
        counter++;
    }

    cout<<counter<<endl;
    if(counter>0){
        int intMode = floor(myMode);
        h_Mbc -> SetLineWidth(2);
        h_Mbc -> SetLineColor(kViolet+2);
        h_Mbc -> SetTitle(Form("[%02d]   %s   SigProb>%.3f",myMode,fState[myMode],minProb));

        TCanvas *c1 = new TCanvas("c1","c1",400,300);
        h_Mbc->Draw();
        c1 -> SaveAs(Form("plots/mode%02d_SigProb_%.3f.pdf", intMode, minProb));
    }

    return;
}
