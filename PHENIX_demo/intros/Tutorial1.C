/*
Prompt:
1) Generate 1D Gaussian distributions
    Mean = -0.5 Sigma = 1.5
    Mean = 1    Sigma = 2.5
    Mean = 1.5  Sigma = 3.5

    Plot them on one Canvas using different colors

2) Fill 1D histograms over the range [-10,10] with one million samples

3) Add three of the histograms into one, 
    and then multiply it by the histo centered at 1.

4) Scale the histogram so that the integral is equal to 10.

5) Save the histograms to a .root file, and save each Canvas as .png

Note:
One-dimensional functions are TF1's, 1D histos = TH1F
*/
void Tutorial1(){

	//Display no stat info
  	gStyle -> SetOptStat(0);	

	//declare Guassians
	TF1 *g1 = new TF1("g1","gaus(0)",-10,10);
	g1->SetParameters(1,-0.5,1.5);
	g1->SetLineColor(41);

	
	TF1 *g2 = new TF1("g2","gaus(0)",-10,10);
	g2->SetParameters(1,1,2.5);
	g2->SetLineColor(3);

	TF1 *g3 = new TF1("g3","gaus(0)",-10,10);
	g3->SetParameters(1,1.5,3.5);
	g3->SetLineColor(5);

	//draw Gaussians on canvas1
	TCanvas *c1 = new TCanvas("c1","",500,500);
	g1->Draw();
	g2->Draw("SAME");
	g3->Draw("SAME");

	//set up histograms
	TH1F *h1 = new TH1F("h1","The Great Emperor Milo",100,-10,10);
	TH1F *h2 = new TH1F("h2","",100,-10,10);
	TH1F *h3 = new TH1F("h3","",100,-10,10);

	for(int i=0;i<1000000;i++) {
		h1->Fill(g1->GetRandom());
		h2->Fill(g2->GetRandom());
		h3->Fill(g3->GetRandom());
	}

	//draw histograms on canvas2
	TCanvas *c2 = new TCanvas("c2","",500,500);
	h1->SetLineColor(41);
	h1->Draw();
	h2->SetLineColor(3);
	h2->Draw("SAME");
	h3->SetLineColor(5);
	h3->Draw("SAME");

	//histogram 4 for adding all of them up
	TH1F *h4 = new TH1F("h4","Our Brethren",100,-10,10);
	
	h4->Add(h1,1);//the second one is weight factor
	h4->Add(h2,1);
	h4->Add(h3,1);

	h4->Multiply(h2);

	//draw histograms on canvas3
	TCanvas *c3 = new TCanvas("c3","",500,500);
	h4->SetLineColor(6);
	h4->SetLineWidth(5);
	h4->DrawCopy();

	//find integral of h4
	float intl= h4->Integral(1,100);

	h4->Scale(10/intl);
	//draw histograms on canvas3
	TCanvas *c4 = new TCanvas("c4","",500,500);
	h4->SetLineColor(7);
	h4->SetLineWidth(3);
	h4->DrawCopy();

	//creat fiile object
	TFile *outfile = new TFile("outTutorial1.root","RECREATE");
	h1 -> Write();//will automatically write on the most recent file
	h2 -> Write();
	h3 -> Write();
	h4 -> Write();	
	
	// save canvases
	c1->SaveAs("histogram.png");
}//end of Tutorial1

