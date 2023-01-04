/*
Prompt:
1) Extract histos from the saved .root fil and plot them in specific formats
    Draw on a single Canvas divided into two panels, three on left, combined on right

2) Fit the histograms with Gaussians to extract: mean and sigma, along with uncertainties 

3) Combine the information extracted from the fits into a histogram for both the mean
    and sigma, where the values in the bins correspond to the value extracted from the fit, 
    and the error is set as the error(uncertaintiy) of the fit parameters

4) Display the erros with TGraphErrors
*/
void Tutorial2(){
	//Display no stat info
  	gStyle -> SetOptStat(3);

	//Read in .root file to create a new file
	TFile *f1 = new TFile("outTutorial1.root"); 
	//This is a Linux command to list the histograms in the terminal
  	//f1 -> ls(); 
	//It's the general structure to match the function typees TH1F*
 	TH1F *h1 = (TH1F*)f1 -> Get("h1");
 	TH1F *h2 = (TH1F*)f1 -> Get("h2");
 	TH1F *h3 = (TH1F*)f1 -> Get("h3");
 	TH1F *h4 = (TH1F*)f1 -> Get("h4");
		
	//This creates a new canvas
	TCanvas *c1 = 
		new TCanvas("c1","Reminiscence of Milonian Empire",900,700);
	//This gives the type of stat info
  	gStyle -> SetOptStat(0);
	//Divide the canvas into 2x1 pads
	c1 -> Divide(2,1);
   	
	//Draw on respective pads in the same canvas
  	c1 -> cd(1);
	h1->SetLineWidth(3);
  	h1 -> DrawCopy();
	h2->SetLineWidth(3);
  	h2 -> DrawCopy("SAME");
	h3->SetLineWidth(3);
  	h3 -> DrawCopy("SAME");

  	c1 -> cd(2);
	h4->SetLineWidth(3);
 	h4 -> DrawCopy(); 	

	//Fit Gaussian functions to obtain parameters
	TF1 *fn1 = new TF1("fn1","gaus(0)",-10,10);
	fn1 -> SetParameters(1,h1->GetMean(),h1->GetRMS()/3);
	fn1 -> SetParNames("Constant","Mean_value","Sigma");
	h1 -> Fit("fn1","0");//“0” Do not plot the result of the fit. By default the fitted function is drawn unless the option “N” above is specified.

 	TF1 *fn2 = new TF1("fn2","gaus(0)",-10,10);
	fn2 -> SetParameters(1,h2->GetMean(),h2->GetRMS()/3);
	fn2 -> SetParNames("Constant","Mean_value","Sigma");
	h2 -> Fit("fn2","0");//,"","",-1,1);

	
	TF1 *fn3 = new TF1("fn3","gaus(0)",-10,10);
	fn3 -> SetParameters(1,h3->GetMean(),h3->GetRMS()/3);
	fn3 -> SetParNames("Constant","Mean_value","Sigma");
	h3 -> Fit("fn3","0");

	TF1 *fn4 = new TF1("fn4","gaus(0)",-10,10);
	fn4 -> SetParameters(1,h4 -> GetMean(),h4 -> GetRMS()/3);
	fn4 -> SetParNames("Constant","Mean_value","Sigma");
	h4 -> Fit("fn4","0");
	
	c1 -> cd(1);
	fn1->SetLineWidth(1);
	fn1 -> SetLineColor(1);
  	fn1 -> DrawCopy("SAME");
	fn2->SetLineWidth(1);
	fn2 -> SetLineColor(2);
  	fn2 -> DrawCopy("SAME");
	fn3->SetLineWidth(1);
	fn3 -> SetLineColor(1);
  	fn3 -> DrawCopy("SAME");

	c1 -> cd(2);
	fn4 -> SetLineColor(1);
	fn4 -> SetLineWidth(1);
  	fn4 -> DrawCopy("SAME");

	//Create a small histogram of size four
	TH1F *h5 = new TH1F("h5","",4,0,4);
	//The index of a histogram goes 1,2,3,4
	/*for(float i=1; i<5; i++) {
		h5 -> SetBinContent(i,i+3);
		h5 -> SetBinError(i,i/2);
	}*/

	Double_t mean1 = fn1 -> GetParameter("Mean_value");
	Double_t mean2 = fn2 -> GetParameter("Mean_value");
	Double_t mean3 = fn3 -> GetParameter("Mean_value");
	Double_t mean4 = fn4 -> GetParameter("Mean_value");

	//Still have to use int index, can't call by assigned name
	Double_t err1 = fn1 -> GetParError(1);
	Double_t err2 = fn2 -> GetParError(1);
	Double_t err3 = fn3 -> GetParError(1);
	Double_t err4 = fn4 -> GetParError(1);

	h5 -> SetBinContent(1,mean1);
	h5 -> SetBinError(1,err1);
	h5 -> SetBinContent(2,mean2);
	h5 -> SetBinError(2,err2);
	h5 -> SetBinContent(3,mean3);
	h5 -> SetBinError(3,err3);
	h5 -> SetBinContent(4,mean4);
	h5 -> SetBinError(4,err4);

	//Create a new small canvas to draw this small histogram
	TCanvas *c2 = new TCanvas("c2","My Tiny Histogram",900,700);
	h5 -> SetLineWidth(3);
	h5->SetLineColor(7);
	h5->Draw();

	//Now use "TGraphErrors" to do the same thing
	TCanvas *c3 = new TCanvas("c3","TGraphErros stuff",100,150,700,500);
	/*
	canvas name
	canvas title
	wtopx,wtopy	are the pixel coordinates of the top left corner of the canvas (if wtopx < 0) the menubar is not shown)
	ww	is the canvas size in pixels along X
	wh	is the canvas size in pixels along Y	
	*/
	//Draw something I'm giving up on you
	

	//Example link https://root.cern.ch/doc/v608/classTGraphErrors.html
	//c3->SetFillColor(42);
	c3->SetGrid();
	//c1->GetFrame()->SetFillColor(21);
	c1->GetFrame()->SetBorderSize(12);
	const Int_t n = 4;
	Double_t x[n] = {1, 2, 3, 4};
	Double_t y[n] = {mean1, mean2, mean3, mean4};
	Double_t ex[n] = {0,0,0,0};
	Double_t ey[n] = {err1, err2, err3, err4};

	/*TGraphErrors::TGraphErrors 	( 	Int_t  	n,
		const Float_t *  	x,
		const Float_t *  	y,
		const Float_t *  	ex = 0,
		const Float_t *  	ey = 0 
	) */	
	TGraphErrors *gr = new TGraphErrors(n,x,y,ex,ey);
	gr -> SetTitle("TGraphErrors~~");
	gr -> SetMarkerColor(4);
	gr -> SetMarkerStyle(21);
	gr -> Draw("AP");
}//end of Tutorial2

