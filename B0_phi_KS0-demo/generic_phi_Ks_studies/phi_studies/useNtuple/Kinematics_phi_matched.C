#include <iostream>
#include <fstream>
#include <cmath>

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

void setStyle1(TH1F *h, const char *unit, const char *title){
    h -> SetLineColor(kBlack);
    h -> SetLineWidth(2);
    h -> GetXaxis() -> SetTitle(unit);
    h -> SetTitle(title);
}
void setStyle2(TH1F *h){
    h -> SetLineColor(kRed);
    h -> SetLineWidth(2);
}
void setStyle3(TH1F *h){
    h -> SetLineColor(kOrange-3);
    h -> SetLineWidth(2);
}
void setStyle4(TH1F *h){
    h -> SetLineColor(kGreen+2);
    h -> SetLineWidth(2);
}
void setStyle5(TH1F *h){
    h -> SetLineColor(kAzure+10);
    h -> SetLineWidth(2);
}
void setStyle6(TH1F *h){
    h -> SetLineColor(kBlue);
    h -> SetLineWidth(2);
}
void Kinematics_phi_matched() {
    const float minM  = 0.98;
    const float maxM  = 1.06;
    const float SigMC = 0;

    TString fileName("");
    if(SigMC) fileName+="A_BphiK_phi_loose.root";
    else      fileName+="A_MC13a_combo_phi_loose.root";
    //K_loose cuts:
    //trackQuality = 'thetaInCDCAcceptance and nCDCHits > 20'
    //ipCut = 'd0 < 0.5 and abs(z0) < 2'
    //https://stash.desy.de/projects/B2/repos/software/browse/analysis/release-validation/stdCharged/truth_charged.py#37,39,42,45,48,51
    ////////////////////////////////////////// histos ////////////////////////////////////////////////
    //No cut on dau trks
    TH1F *h1_M_phi           = new TH1F("h1_M_phi","",          100,minM,maxM);
    TH1F *h1_pt_phi          = new TH1F("h1_pt_phi","",         100,0.,4.);
    TH1F *h1_pt_phi_K        = new TH1F("h1_pt_phi_K","",       100,0.,4.);
    TH1F *h1_theta_phi_K     = new TH1F("h1_theta_phi_K","",    100,0.,3.2);

    //Only CDC acc cut
    TH1F *h2_M_phi           = new TH1F("h2_M_phi","",          100,minM,maxM);
    TH1F *h2_pt_phi          = new TH1F("h2_pt_phi","",         100,0.,4.);
    TH1F *h2_pt_phi_K        = new TH1F("h2_pt_phi_K","",       100,0.,4.);
    TH1F *h2_theta_phi_K     = new TH1F("h2_theta_phi_K","",    100,0.,3.2);

    //Only CDC hit cut
    TH1F *h3_M_phi           = new TH1F("h3_M_phi","",          100,minM,maxM);
    TH1F *h3_pt_phi          = new TH1F("h3_pt_phi","",         100,0.,4.);
    TH1F *h3_pt_phi_K        = new TH1F("h3_pt_phi_K","",       100,0.,4.);
    TH1F *h3_theta_phi_K     = new TH1F("h3_theta_phi_K","",    100,0.,3.2);

    //Only IP cut
    TH1F *h4_M_phi           = new TH1F("h4_M_phi","",          100,minM,maxM);
    TH1F *h4_pt_phi          = new TH1F("h4_pt_phi","",         100,0.,4.);
    TH1F *h4_pt_phi_K        = new TH1F("h4_pt_phi_K","",       100,0.,4.);
    TH1F *h4_theta_phi_K     = new TH1F("h4_theta_phi_K","",    100,0.,3.2);

    //Only KID cut 
    TH1F *h5_M_phi           = new TH1F("h5_M_phi","",          100,minM,maxM);
    TH1F *h5_pt_phi          = new TH1F("h5_pt_phi","",         100,0.,4.);
    TH1F *h5_pt_phi_K        = new TH1F("h5_pt_phi_K","",       100,0.,4.);
    TH1F *h5_theta_phi_K     = new TH1F("h5_theta_phi_K","",    100,0.,3.2);

    //Mimic K+/-:loose 
    TH1F *h6_M_phi           = new TH1F("h6_M_phi","",          100,minM,maxM);
    TH1F *h6_pt_phi          = new TH1F("h6_pt_phi","",         100,0.,4.);
    TH1F *h6_pt_phi_K        = new TH1F("h6_pt_phi_K","",       100,0.,4.);
    TH1F *h6_theta_phi_K     = new TH1F("h6_theta_phi_K","",    100,0.,3.2);

    //matched mass histos
    TH1F *h1_M_matched       = new TH1F("h1_M_matched","",      100,minM,maxM);
    TH1F *h2_M_matched       = new TH1F("h2_M_matched","",      100,minM,maxM);
    TH1F *h3_M_matched       = new TH1F("h3_M_matched","",      100,minM,maxM);
    TH1F *h4_M_matched       = new TH1F("h4_M_matched","",      100,minM,maxM);
    TH1F *h5_M_matched       = new TH1F("h5_M_matched","",      100,minM,maxM);
    TH1F *h6_M_matched       = new TH1F("h6_M_matched","",      100,minM,maxM);

    unsigned int n1=0;
    unsigned int n2=0;
    unsigned int n3=0;
    unsigned int n4=0;
    unsigned int n5=0;
    unsigned int n6=0;
    //////////////////////////////////////////////////
    TChain *PhiAll = new TChain("tree");            //
                                                    //
    PhiAll -> AddFile(fileName);                    //
                                                    //
    int allEntries = PhiAll -> GetEntries();        //
    //////////////////////////////////////////////////

    ////////////////////////////////////////// entries ////////////////////////////////////////////////
    double  all_M_phi;
    PhiAll -> SetBranchAddress("M", &all_M_phi);
    double  all_p_phi;
    PhiAll -> SetBranchAddress("p", &all_p_phi);
    double  all_pt_phi;
    PhiAll -> SetBranchAddress("pt", &all_pt_phi);
    double  all_pstar;
    if(SigMC)PhiAll -> SetBranchAddress("useCMSFrame__bop__bc",&all_pstar);
    double  all_p0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmp__bc", &all_p0);
    double  all_p1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmp__bc", &all_p1);
    double  all_pt0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmpt__bc", &all_pt0);
    double  all_pt1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmpt__bc", &all_pt1);
    double  all_theta0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmtheta__bc", &all_theta0);
    double  all_theta1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmtheta__bc", &all_theta1);
    double  all_KID0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmkaonID__bc", &all_KID0);
    double  all_KID1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmkaonID__bc", &all_KID1);
    double  all_inCDCacc0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmthetaInCDCAcceptance__bc", &all_inCDCacc0);
    double  all_inCDCacc1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmthetaInCDCAcceptance__bc", &all_inCDCacc1);
    double  all_nCDChits0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmnCDCHits__bc", &all_nCDChits0);
    double  all_nCDChits1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmnCDCHits__bc", &all_nCDChits1);
    double  all_d0_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmd0__bc", &all_d0_0);
    double  all_d0_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmd0__bc", &all_d0_1);
    double  all_z0_0;
    PhiAll -> SetBranchAddress("daughter__bo0__cmz0__bc", &all_z0_0);
    double  all_z0_1;
    PhiAll -> SetBranchAddress("daughter__bo1__cmz0__bc", &all_z0_1);
    double  all_isSignal;
    PhiAll -> SetBranchAddress("isSignal", &all_isSignal);

    for(int i=0; i < allEntries; i++){
        PhiAll -> GetEntry(i);
        //if(SigMC&&all_pstar<2.2)continue;
        //No cut on dau trks
        if(all_isSignal==1&&all_M_phi>minM&&all_M_phi<maxM){
            if(true){
                h1_pt_phi       -> Fill(all_pt_phi);

                h1_pt_phi_K     -> Fill(all_pt0);
                h1_pt_phi_K     -> Fill(all_pt1);
                h1_theta_phi_K  -> Fill(all_theta0);
                h1_theta_phi_K  -> Fill(all_theta1);

                h1_M_phi        -> Fill(all_M_phi);
                n1++;
            }
            //Only CDC acc cut
            if(all_inCDCacc0    && all_inCDCacc1){
                h2_pt_phi       -> Fill(all_pt_phi);

                h2_pt_phi_K     -> Fill(all_pt0);
                h2_pt_phi_K     -> Fill(all_pt1);
                h2_theta_phi_K  -> Fill(all_theta0);
                h2_theta_phi_K  -> Fill(all_theta1);

                h2_M_phi        -> Fill(all_M_phi);
                n2++;
            }
            //only CDC hit cut
            if(all_nCDChits0>20 && all_nCDChits1>20){
                h3_pt_phi       -> Fill(all_pt_phi);

                h3_pt_phi_K     -> Fill(all_pt0);
                h3_pt_phi_K     -> Fill(all_pt1);
                h3_theta_phi_K  -> Fill(all_theta0);
                h3_theta_phi_K  -> Fill(all_theta1);

                h3_M_phi        -> Fill(all_M_phi);
                n3++;
            }
            //Only IP cut
            if( (all_d0_0<0.5   && all_d0_1<0.5)
            &&  (abs(all_z0_0)<2&& abs(all_z0_1)<2)
                ){
                h4_pt_phi       -> Fill(all_pt_phi);

                h4_pt_phi_K     -> Fill(all_pt0);
                h4_pt_phi_K     -> Fill(all_pt1);
                h4_theta_phi_K  -> Fill(all_theta0);
                h4_theta_phi_K  -> Fill(all_theta1);

                h4_M_phi        -> Fill(all_M_phi);
                n4++;
            }
            //Only KID cut
            if( all_KID0>0.1    && all_KID1>0.1){
                h5_pt_phi       -> Fill(all_pt_phi);

                h5_pt_phi_K     -> Fill(all_pt0);
                h5_pt_phi_K     -> Fill(all_pt1);
                h5_theta_phi_K  -> Fill(all_theta0);
                h5_theta_phi_K  -> Fill(all_theta1);

                h5_M_phi        -> Fill(all_M_phi);
                n5++;
            }
            //Mimic K+/-:loose
            if( (all_KID0>0.1   && all_KID1>0.1)
            && (all_theta0>0.3  && all_theta0<2.6)
            && (all_theta1>0.3  && all_theta1<2.6)
            && (all_inCDCacc0   && all_inCDCacc1)
            && (all_nCDChits0>20&& all_nCDChits1>20)
            && (all_d0_0<0.5    && all_d0_1<0.5)
            && (abs(all_z0_0)<2 && abs(all_z0_1)<2)
              ){
                h6_pt_phi       -> Fill(all_pt_phi);

                h6_pt_phi_K     -> Fill(all_pt0);
                h6_pt_phi_K     -> Fill(all_pt1);
                h6_theta_phi_K  -> Fill(all_theta0);
                h6_theta_phi_K  -> Fill(all_theta1);

                h6_M_phi        -> Fill(all_M_phi);
                n6++;
            }
        }//isSignal
    }//phi:all

//////////////////////////////////////////  plots  ////////////////////////////////////////////////
gStyle->SetOptStat(0);
TCanvas *c1 = new TCanvas("c1","c1",1200,800);
c1 -> Divide(2,2,0.01,0.01,0);
//for(int i=0; i<2; i++)
int i = 0;
{
    c1 -> cd(1);
    if(SigMC)setStyle1(h1_pt_phi_K,"GeV","pt(K#pm) [matched #phi BphiK MC, p*>2.2]");
    else     setStyle1(h1_pt_phi_K,"GeV","pt(K#pm) [matched #phi combo MC]");
    setStyle2(h2_pt_phi_K);
    setStyle3(h3_pt_phi_K);
    setStyle4(h4_pt_phi_K);
    setStyle5(h5_pt_phi_K);
    setStyle6(h6_pt_phi_K);
    h1_pt_phi_K      -> Draw();
    h2_pt_phi_K      -> Draw("same");
    h3_pt_phi_K      -> Draw("same");
    h4_pt_phi_K      -> Draw("same");
    h5_pt_phi_K      -> Draw("same");
    h6_pt_phi_K      -> Draw("same");
    auto leg1 = new TLegend(0.55,0.7,0.9,0.9);
    leg1->AddEntry(h1_pt_phi_K, Form("No cut on dau trks\t%i  \tref",n1));
    leg1->AddEntry(h2_pt_phi_K, Form("Only inCDC acc cut\t%i  \t%0.2f",n2,n2*1./n1));
    leg1->AddEntry(h3_pt_phi_K, Form("Only nCDC hits cut\t%i  \t%0.2f",n3,n3*1./n1));
    leg1->AddEntry(h4_pt_phi_K, Form("Only IP(d0&z0) cut\t%i  \t%0.2f",n4,n4*1./n1));
    leg1->AddEntry(h5_pt_phi_K, Form("Only KID > 0.1 cut\t%i  \t%0.2f",n5,n5*1./n1));
    leg1->AddEntry(h6_pt_phi_K, Form("Mimic loose:phi   \t%i  \t%0.2f",n6,n6*1./n1));
    leg1->SetTextSize(0.035);
    if(i)gPad -> SetLogy();

    c1 -> cd(2);
    setStyle1(h1_pt_phi,"GeV","p(#phi)");
    if(SigMC)setStyle1(h1_pt_phi,"GeV","pt(#phi) [matched #phi BphiK MC, p*>2.2]");
    else     setStyle1(h1_pt_phi,"GeV","pt(#phi) [matched #phi combo MC]");
    setStyle2(h2_pt_phi);
    setStyle3(h3_pt_phi);
    setStyle4(h4_pt_phi);
    setStyle5(h5_pt_phi);
    setStyle6(h6_pt_phi);
    h1_pt_phi        -> Draw();
    h2_pt_phi        -> Draw("same");
    h3_pt_phi        -> Draw("same");
    h4_pt_phi        -> Draw("same");
    h5_pt_phi        -> Draw("same");
    h6_pt_phi        -> Draw("same");
    if(i)gPad -> SetLogy();
    
    c1 -> cd(3);
    if(SigMC)setStyle1(h1_theta_phi_K,"GeV","#theta(K#pm) [matched #phi BphiK MC, p*>2.2]");
    else     setStyle1(h1_theta_phi_K,"GeV","#theta(K#pm) [matched #phi combo MC]");
    setStyle2(h2_theta_phi_K);
    setStyle3(h3_theta_phi_K);
    setStyle4(h4_theta_phi_K);
    setStyle5(h5_theta_phi_K);
    setStyle6(h6_theta_phi_K);
    h1_theta_phi_K  -> Draw();
    h2_theta_phi_K  -> Draw("same");
    h3_theta_phi_K  -> Draw("same");
    h4_theta_phi_K  -> Draw("same");
    h5_theta_phi_K  -> Draw("same");
    h6_theta_phi_K  -> Draw("same");
    if(i)gPad -> SetLogy();
    c1 -> cd(4);
    if(SigMC)setStyle1(h1_M_phi,"GeV","m(#phi) [matched #phi BphiK MC, p*>2.2]");
    else     setStyle1(h1_M_phi,"GeV","m(#phi) [matched #phi combo MC]");
    setStyle2(h2_M_phi);
    setStyle3(h3_M_phi);
    setStyle4(h4_M_phi);
    setStyle5(h5_M_phi);
    setStyle6(h6_M_phi);
    h1_M_phi        -> Draw();
    h2_M_phi        -> Draw("same");
    h3_M_phi        -> Draw("same");
    h4_M_phi        -> Draw("same");
    h5_M_phi        -> Draw("same");
    h6_M_phi        -> Draw("same");
    leg1->Draw();
    if(i)gPad -> SetLogy();
    /*
    if(SigMC){
        if(i)c1 -> SaveAs("SigMC_matched_logKinematics.png");
        else c1 -> SaveAs("SigMC_matched_Kinematics.png");
    }
    else{
        if(i)c1 -> SaveAs("combo_matched_logKinematics.png");
        //else c1 -> SaveAs("combo_matched_Kinematics.png");
        else c1 -> SaveAs("combo_matched_Kinematics_M98106.png");
    }
    */
}

return ;

}
