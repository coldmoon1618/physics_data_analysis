void omrho_roostyle()
{  TStyle *myStyle= new TStyle("MY","My approved plots style");

  // use plain black on white colors
  myStyle->SetFrameBorderMode(0);
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetPadColor(0);
  myStyle->SetCanvasColor(0);
  //myStyle->SetTitleColor(0);
  myStyle->SetStatColor(0);
  myStyle->SetTitleFillColor(0);

  // set the paper & margin sizes
  myStyle->SetPaperSize(20,26);
  myStyle->SetPadTopMargin(0.02);
  myStyle->SetPadRightMargin(0.05);
  myStyle->SetPadBottomMargin(0.27);
  myStyle->SetPadLeftMargin(0.12);

  // use large Times-Roman fonts
  myStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
  myStyle->SetTitleFont(132," ");    // set the pad title font
  myStyle->SetTitleSize(0.12,"xyz"); // set the 3 axes title size
  myStyle->SetTitleSize(0.06," ");   // set the pad title size
  myStyle->SetLabelFont(132,"xyz");
  myStyle->SetLabelSize(0.1,"xz");
  myStyle->SetLabelSize(0.07,"y");
  myStyle->SetTextFont(132);
  myStyle->SetTextSize(0.08);
  myStyle->SetStatFont(132);
  

  // use bold lines and markers
  myStyle->SetMarkerStyle(8);
  myStyle->SetMarkerSize(1);
  myStyle->SetHistLineWidth(1.85);
  myStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  //..Get rid of X error bars
  myStyle->SetErrorX(0.001);

  // do not display any of the standard histogram decorations
  myStyle->SetOptTitle(0);
  myStyle->SetOptStat(0);
  myStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);

  myStyle->SetOptStat(0000000);

  // restore the plain style. Add tick marks and extra stats
  //gROOT->SetStyle("Plain");
  //gStyle->SetPadTickX(1);
  //gStyle->SetPadTickY(1);
  //gStyle->SetOptStat(1111111);
}
