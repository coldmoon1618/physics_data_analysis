/*
 * Author: Yunxiao "Michelle" Zhai
 * Email: zhai@iastate.edu
 * Map out a 'perfect' PC into actual PC y and z
 * focusing on PC3 right now
 * June 10, 2019
 * */
#include <iostream>
#include <fstream>

#include <TChain.h>
#include <TString.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>
#include <TLine.h>
void mapPlot(){
//TFile *fOut = new TFile("PCMaps.root","RECREATE");
  TH2F *hPC3E = new TH2F("hPC3E", "PC3E y Vs z", 400, -200, 200, 800, -300, 500);
  hPC3E->GetXaxis()->SetTitle("Z");
  hPC3E->GetYaxis()->SetTitle("Y");

  //TH2F *hPC3W = new TH2F("hPC3W", "PC3W y Vs z", 400, -200, 200, 800, -300, 500);
  TH2F *hPC3W = new TH2F("hPC3W", "PC3W y Vs z", 30, -200, 200, 50, -300, 500);
  hPC3W->GetXaxis()->SetTitle("Z");
  hPC3W->GetYaxis()->SetTitle("Y");

//TH2F *hPC1E = new TH2F("hPC1E", "PC1E y Vs z", 200, -100, 100, 400, -150, 250);
//hPC1E->GetXaxis()->SetTitle("Z");
//hPC1E->GetYaxis()->SetTitle("Y");

//TH2F *hPC1W = new TH2F("hPC1W", "PC1W y Vs z", 200, -100, 100, 400, -150, 250);
//hPC1W->GetXaxis()->SetTitle("Z");
//hPC1W->GetYaxis()->SetTitle("Y");
  
  //x-offset
  double pdxoff[2] = {-24.31, -95.7};
  //z-offset		
  double pdzoff[2] = {-89.5575, -178.69};
  //pixel length		
  double pxlen[2]  = {0.82, 1.622};
  //side pixel width		
  double wside[2]  = {0.17, 0.55};
  //center pixel width		
  double wcent[2]  = {0.15, 0.31};
  //pixel space line width		
  double pxcep[2]  = {0.025, 0.025};
  //cell space line width		
  double clcep[2]  = {0.1, 0.2};
  //z-gap between N & E	
  double zgap[2]   = {0.0, 8.106};
  //anode to anode spacing
  double aasep[2]  = {0.84,1.54};
  /*
  phi @ bot of E arm		213.75
  phi @ top of E arm		123.75
  phi @ bot of W arm		-33.75
  phi @ top of W arm		 56.25
  Using the above info we know that 
  PCE={213.75, 191.25, 168.75, 146.25, 123.72}
  PCW={-33.75, -11.25,  11.25,  33.75,  56.25}
  |---------------top of PC----------|
  |  (I)  adjustment factor = 0.707  | 
  |----------------------------------|
  |  (II) adjustment factor = 1      |
  |----------------------------------|
  |  (III)adjustment factor = 0.924  |
  |----------------------------------|
  |  (IV) adjustment factor - 0.707  |
  |---------------bot of PC----------|
   * */

  TCanvas *c_PC3 = new TCanvas("c_PC3","PC3 regions",500,700);
  gStyle -> SetOptStat(0);
  c_PC3 -> Divide(1,2,0.01,0.01,0);
  c_PC3 -> cd(1);
  hPC3W -> DrawCopy("colz");
  c_PC3 -> cd(2);
  hPC3E -> DrawCopy("colz");
  for(int packid=4065; packid<=4096; packid++){
    //focusing on PC3 for now
    for(int grow=0; grow<5;grow++){
      for(int gcol=0;gcol<9;gcol++){
	double pxwid = 0;//pixel width
	double pkwid = 0;//packet width
	double pklen = 0;//packet length

	//S N adjustments
	int SNadj = ((packid-4065)/8)%2;

	//y adjustments
	int yPckAdj = (packid-4065)%8;
	if(yPckAdj==6||yPckAdj==7)
	{//(I)
	  pxwid = 0.707*wcent[1];
	  pkwid = ((packid-4071)%2)*5*20*3*(pxwid+pxcep[1]);
	  pkwid += 2*5*20*3*(0.5*wside[1]+pxcep[1]);
	  pkwid += 2*5*20*3*(0.5*0.924*wside[1]+pxcep[1]);
	  pkwid += 20;
	}
	else if(yPckAdj==4||yPckAdj==5)
	{//(II)
	  pxwid = 0.5*0.924*wside[1];
	  pkwid = ((packid-4069)%2)*5*20*3*(pxwid+pxcep[1]);
	  pkwid += 2*5*20*3*(0.5*wside[1]+pxcep[1]);
	  pkwid += 20;
	}
	else if(yPckAdj==2||yPckAdj==3)
	{//(III)
	  pxwid = 0.5*wside[1];
	  pkwid = ((packid-4067)%2)*5*20*3*(pxwid+pxcep[1]);
	}
	else if(yPckAdj==0||yPckAdj==1)
	{//(IV)
	  pxwid = 0.707*wcent[1];
	  pkwid = ((packid-4065)%2-2)*5*20*3*(pxwid+pxcep[1]);
	  pkwid -= 20;
	}

	double pcz_min = gcol*4*3*(pxlen[1]+pxcep[1])+pklen;
	double pcz_max = pcz_min+4*3*(pxlen[1]+pxcep[1]);

	double pcy_min = grow*20*3*(pxwid+pxcep[1])+pdxoff[1]+pkwid;
	double pcy_max = pcy_min+20*3*(pxwid+pxcep[1]);

	//S N adjustments
	if(SNadj==0){//S
	  pcz_min=-0.5*zgap[1]-pcz_min;
	  pcz_max=-0.5*zgap[1]-pcz_max;
	}
	else{//N
	  pcz_min+= 0.5*zgap[1];
	  pcz_max+= 0.5*zgap[1];
	}

	TLine *l1 = new TLine(pcz_min,pcy_min,pcz_min,pcy_max);
	TLine *l2 = new TLine(pcz_min,pcy_min,pcz_max,pcy_min);
	TLine *l3 = new TLine(pcz_min,pcy_max,pcz_max,pcy_max);
	TLine *l4 = new TLine(pcz_max,pcy_min,pcz_max,pcy_max);
	TLine *l5 = new TLine(pcz_min,pcy_min,pcz_max,pcy_max);
	TLine *l6 = new TLine(pcz_min,pcy_max,pcz_max,pcy_min);
	
	if(packid>=4065&&packid<=4080){
	  c_PC3 -> cd(1);
	  l1->Draw();
	  l2->Draw();
	  l3->Draw();
	  l4->Draw();
	  l5->Draw();
	  l6->Draw();
	}else if(packid>=4081&&packid<=4096){
	  c_PC3 -> cd(2);
	  l1->Draw();
	  l2->Draw();
	  l3->Draw();
	  l4->Draw();
	  l5->Draw();
	  l6->Draw();
	}

      }//gcol
    }//grow
  }//packid






  
//fOut -> Write();
//fOut -> Close();
  
  return;

}//end makeMap
