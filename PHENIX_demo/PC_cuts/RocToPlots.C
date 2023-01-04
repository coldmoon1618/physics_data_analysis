/*
 * Author: Yunxiao "Michelle" Zhai
 * Email: zhai@iastate.edu
 * Convert PC dead rocs to real coor plots
 * At the moment, mainly focus on PC3
 * June 10, 2019
 * */
#include <iostream>
#include <fstream>
#include <string>
void RocToPlots(bool isData){
  float max_pc3 = 0.0; 
  float max_pc1 = 0.0; 

  TString fname  = "";
  if(isData)	
  {
    fname  = "/sphenix/user/yzhai/PC_cuts/run12_dat.root";
    //max_pc3=1500.0;max_pc1=20000.0;
    max_pc3=3200.0;max_pc1=28000.0;
  }
  else		
  {
    fname  = "/sphenix/user/yzhai/PC_cuts/run12_sim.root";
    max_pc3=25.0;max_pc1=150.0;
  }
  cout<<fname<<endl;
  TFile fIn(fname);
  if(fIn.IsZombie()){ 
    fIn.Close();cout<<"Zombine file skipped"<<endl; 
  }
  else{
    TH2F *hPC3W = (TH2F*)fIn.Get("H_PC3W_yVSz")->Clone();
    TH2F *hPC3E = (TH2F*)fIn.Get("H_PC3E_yVSz")->Clone();

    TH2F *hPC1W = (TH2F*)fIn.Get("H_PC1W_yVSz")->Clone();
    TH2F *hPC1E = (TH2F*)fIn.Get("H_PC1E_yVSz")->Clone();
  }
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
  PCE={213.75, 191.25, 168.75, 146.25, 123.75}
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


  string nRoc;
  int packid, grow, gcol, rocType;

  ifstream deadrocfile("run12_pad_deadroc.dat");
  ofstream deadpc3file("run12_deadPC3.txt");
  ofstream deadpc1file("run12_deadPC1.txt");

  if(deadrocfile.is_open()){
  getline(deadrocfile,nRoc);
  cout<<nRoc<<" dead ROCs"<<endl;
  int totRegion = atoi(nRoc.c_str());
  cout<<totRegion<<" dead regions"<<endl;
  deadpc3file<<"isCut = "<<endl;
  deadpc1file<<"isCut = "<<endl;
  int nRegion = 0; 
  int nPC1dead = 0; 
  int nPC3dead = 0; 
  TCanvas *c_PC3 = new TCanvas("c_PC3","PC3 regions",500,700);
  gStyle -> SetOptStat(0);
  gStyle -> SetPalette(1);
  c_PC3 -> Divide(1,2,0.01,0.01,0);
  c_PC3 -> cd(1);
  hPC3W -> SetAxisRange(0.,max_pc3,"Z");
  hPC3W -> DrawCopy("colz");
  c_PC3 -> cd(2);
  hPC3E -> SetAxisRange(0.,max_pc3,"Z");
  hPC3E -> DrawCopy("colz");

  TCanvas *c_PC1 = new TCanvas("c_PC1","PC1 regions",500,700);
  gStyle -> SetOptStat(0);
  gStyle -> SetPalette(1);
  c_PC1 -> Divide(1,2,0.01,0.01,0);
  c_PC1 -> cd(1);
  hPC1W -> SetAxisRange(0.,max_pc1,"Z");
  hPC1W -> DrawCopy("colz");
  c_PC1 -> cd(2);
  hPC1E -> SetAxisRange(0.,max_pc1,"Z");
  hPC1E -> DrawCopy("colz");
    while(deadrocfile>>packid>>grow>>gcol>>rocType){


	double pxwid = 0;//pixel width
	double pkwid = 0;//packet width
	double pklen = 0;//packet length

	if(packid>=4065&&packid<=4096){//PC3
	  nPC3dead++;
	//This unflip what flipROC has done
	//back to data base format
	if(packid>=4065&&packid<=4080){
	  grow = 4 - grow;
	  if(packid%2==1){
	    packid++;
	  }else{
	    packid--;
	  } 
	}
	  //S N adjustments
	  int SNadj = ((packid-4065)/8)%2;
	  if(SNadj==1){//S
	    grow = 4-grow;
	  }
	  //y adjustments
	  int yPckAdj = (packid-4001)%8;
	  if(yPckAdj==6||yPckAdj==7)
	  {//(I)
	    pxwid = 0.707*wcent[1];
	    pkwid = ((packid-4007)%2)*5*20*3*(pxwid+pxcep[1]);
	    pkwid += 2*5*20*3*(0.5*wside[1]+pxcep[1]);
	    pkwid += 2*5*20*3*(0.5*0.924*wside[1]+pxcep[1]);
	    pkwid += 22;
	  }
	  else if(yPckAdj==4||yPckAdj==5)
	  {//(II)
	    pxwid = 0.5*0.924*wside[1];
	    pkwid = ((packid-4005)%2)*5*20*3*(pxwid+pxcep[1]);
	    pkwid += 2*5*20*3*(0.5*wside[1]+pxcep[1]);
	    pkwid += 22;
	  }
	  else if(yPckAdj==2||yPckAdj==3)
	  {//(III)
	    pxwid = 0.5*wside[1];
	    pkwid = ((packid-4003)%2)*5*20*3*(pxwid+pxcep[1]);
	    pkwid += 9;
	  }
	  else if(yPckAdj==0||yPckAdj==1)
	  {//(IV)
	    pxwid = 0.807*wcent[1];
	    pkwid = ((packid-4001)%2-2)*5*20*3*(pxwid+pxcep[1]);
	  }

	  double pc3z_min = gcol*4*3*(pxlen[1]+pxcep[1]);
	  double pc3z_max = pc3z_min+4*3*(pxlen[1]+pxcep[1]);

	  double pc3y_min = grow*20*3*(pxwid+pxcep[1])+pdxoff[1]+pkwid;
	  double pc3y_max = pc3y_min+20*3*(pxwid+pxcep[1]);

	  //S N adjustments
	  if(SNadj==0){//S
	    pc3z_min=-0.5*zgap[1]-pc3z_min;
	    pc3z_max=-0.5*zgap[1]-pc3z_max;
	  }
	  else{//N
	    pc3z_min+= 0.5*zgap[1];
	    pc3z_max+= 0.5*zgap[1];
	  }
	  //final fine-tuning
	  if(yPckAdj==0){
	    pc3y_min -= 15;
	    pc3y_max -= 15;
	  }else if(yPckAdj==1){
	    pc3y_min -= 5;
	    pc3y_max -= 5;
	  }

	  TLine *l1 = new TLine(pc3z_min,pc3y_min,pc3z_min,pc3y_max);l1->SetLineColor(kRed);
	  TLine *l2 = new TLine(pc3z_min,pc3y_min,pc3z_max,pc3y_min);l2->SetLineColor(kRed);
	  TLine *l3 = new TLine(pc3z_min,pc3y_max,pc3z_max,pc3y_max);l3->SetLineColor(kRed);
	  TLine *l4 = new TLine(pc3z_max,pc3y_min,pc3z_max,pc3y_max);l4->SetLineColor(kRed);
	  TLine *l5 = new TLine(pc3z_min,pc3y_min,pc3z_max,pc3y_max);l5->SetLineColor(kRed);
	  TLine *l6 = new TLine(pc3z_min,pc3y_max,pc3z_max,pc3y_min);l6->SetLineColor(kRed);
	  
	  if(packid>=4065&&packid<=4080){//PC3W
	    c_PC3 -> cd(1);
	    l1->Draw();
	    l2->Draw();
	    l3->Draw();
	    l4->Draw();
	    l5->Draw();
	    l6->Draw();
	  }else if(packid>=4081&&packid<=4096){//PC3E
	    c_PC3 -> cd(2);
	    l1->Draw();
	    l2->Draw();
	    l3->Draw();
	    l4->Draw();
	    l5->Draw();
	    l6->Draw();
	  }//W vs E
	}//PC3
	else if(packid>=4001&&packid<=4032){//PC1
	  nPC1dead++;

	  //S N adjustments
	  int SNadj = ((packid-4001)/8)%2;
	  if(packid>4016&&SNadj==0 || packid<=4016&&SNadj==1)grow = 4- grow;
	  //y adjustments
	  int yPckAdj = (packid-4001)%8;
	  if(yPckAdj==6||yPckAdj==7)
	  {//(I)
	    pxwid = 0.5*wcent[0];
	    pkwid = 2*((packid-4071)%2)*5*20*3*(pxwid+pxcep[0]);
	    pkwid += 4*5*20*3*(0.5*wside[0]+pxcep[0]);
	    pkwid += 4*5*20*3*(0.5*0.924*wside[0]+pxcep[0]);
	    double pc1z_min = gcol*4*3*(pxlen[0]+pxcep[0]);
	    double pc1z_max = pc1z_min+4*3*(pxlen[0]+pxcep[0]);

	    double pc1y_min = -grow*20*3*(pxwid+pxcep[0])+pdxoff[0]-pkwid;
	    if(yPckAdj==7)pc1y_min+=422;else pc1y_min+=448;
	    double pc1y_max = pc1y_min+20*3*(pxwid+pxcep[0]);
	  }
	  else if(yPckAdj==4||yPckAdj==5)
	  {//(II)
	    pxwid = 0.75*wside[0];
	    pkwid = ((packid-4069)%2)*5*20*3*(pxwid+pxcep[0]);
	    pkwid += 4*5*20*3*(0.5*wside[0]+pxcep[0]);
	    double pc1z_min = gcol*4*3*(pxlen[0]+pxcep[0]);
	    double pc1z_max = pc1z_min+4*3*(pxlen[0]+pxcep[0]);

	    double pc1y_min = -grow*20*3*(pxwid+pxcep[0])+pdxoff[0]-pkwid;
	    pc1y_min+=242;
	    double pc1y_max = pc1y_min+20*3*(pxwid+pxcep[0]);
	  }
	  else if(yPckAdj==2||yPckAdj==3)
	  {//(III)
	    pxwid = 0.75*wside[0];
	    pkwid = ((packid-4067)%2)*5*20*3*(pxwid+pxcep[0]);
  	    pkwid -= 15;
	    double pc1z_min = gcol*4*3*(pxlen[0]+pxcep[0]);
	    double pc1z_max = pc1z_min+4*3*(pxlen[0]+pxcep[0]);

	    double pc1y_min = -grow*20*3*(pxwid+pxcep[0])+pdxoff[0]-pkwid;
	    double pc1y_max = pc1y_min+20*3*(pxwid+pxcep[0]);
	  }
	  else if(yPckAdj==0||yPckAdj==1)
	  {//(IV)
	    pxwid = 0.707*wcent[0];
	    pkwid = 2*((packid-4065)%2-2)*5*20*3*(pxwid+pxcep[0]);
	    double pc1z_min = gcol*4*3*(pxlen[0]+pxcep[0]);
	    double pc1z_max = pc1z_min+4*3*(pxlen[0]+pxcep[0]);

	    double pc1y_min = -grow*20*3*(pxwid+pxcep[0])-pkwid;
	    if(yPckAdj==0)pc1y_min-=262;else pc1y_min-=295;
	    double pc1y_max = pc1y_min+20*3*(pxwid+pxcep[0]);
	  }

	  //S N adjustments
	  if(SNadj==0){//s
	    pc1z_min=-0.5*zgap[0]-pc1z_min;
	    pc1z_max=-0.5*zgap[0]-pc1z_max;
	  }
	  else{//n
	    pc1z_min+= 0.5*zgap[0];
	    pc1z_max+= 0.5*zgap[0];
	  }

	  TLine *l1 = new TLine(pc1z_min,pc1y_min,pc1z_min,pc1y_max);l1->SetLineColor(kRed);
	  TLine *l2 = new TLine(pc1z_min,pc1y_min,pc1z_max,pc1y_min);l2->SetLineColor(kRed);
	  TLine *l3 = new TLine(pc1z_min,pc1y_max,pc1z_max,pc1y_max);l3->SetLineColor(kRed);
	  TLine *l4 = new TLine(pc1z_max,pc1y_min,pc1z_max,pc1y_max);l4->SetLineColor(kRed);
	  TLine *l5 = new TLine(pc1z_min,pc1y_min,pc1z_max,pc1y_max);l5->SetLineColor(kRed);
	  TLine *l6 = new TLine(pc1z_min,pc1y_max,pc1z_max,pc1y_min);l6->SetLineColor(kRed);
	  
	  if(packid>=4001&&packid<=4016){//PC1W
	    c_PC1 -> cd(1);
	    l1->Draw();
	    l2->Draw();
	    l3->Draw();
	    l4->Draw();
	    l5->Draw();
	    l6->Draw();
	  }else if(packid>=4017&&packid<=4032){//PC1E
	    c_PC1 -> cd(2);
	    l1->Draw();
	    l2->Draw();
	    l3->Draw();
	    l4->Draw();
	    l5->Draw();
	    l6->Draw();
	  }//W vs E
	}//PC1

      nRegion++;
      if(packid>=4065&&packid<=4096){//PC3
	if(packid<=4080) //PC3W
	  if(pc3z_min<pc3z_max)//normal
	    deadpc3file << "( arm==1 && (pc3z >= " <<pc3z_min<< " && pc3z <= " <<pc3z_max<<")"<<endl;
	  else //sign flipping
	    deadpc3file << "( arm==1 && (pc3z <= " <<pc3z_min<< " && pc3z >= " <<pc3z_max<<")"<<endl;
	else//PC3E
	  if(pc3z_min<pc3z_max)//normal
	    deadpc3file << "( arm==0 && (pc3z >= " <<pc3z_min<< " && pc3z <= " <<pc3z_max<<")"<<endl;
	  else //sign flipping
	    deadpc3file << "( arm==0 && (pc3z <= " <<pc3z_min<< " && pc3z >= " <<pc3z_max<<")"<<endl;
	if(pc3y_min<pc3y_max)
	  deadpc3file << " && (pc3y >= "<<pc3y_min<< " && pc3y <= " <<pc3y_max<<") )";
	else
	  deadpc3file << " && (pc3y <= "<<pc3y_min<< " && pc3y >= " <<pc3y_max<<") )";
	if(nRegion<totRegion)deadpc3file<<"|| "<<endl;    
      }else if(packid>=4001&&packid<=4032){
	if(packid<=4016) 
	  if(pc1z_min<pc1z_max)//normal
	    deadpc1file << "( arm==1 && (pc1z >= " <<pc1z_min<< " && pc1z <= " <<pc1z_max<<")"<<endl;
	  else //sign flipping
	    deadpc1file << "( arm==1 && (pc1z <= " <<pc1z_min<< " && pc1z >= " <<pc1z_max<<")"<<endl;
	else
	  if(pc1z_min<pc1z_max)//normal
	    deadpc1file << "( arm==0 && (pc1z >= " <<pc1z_min<< " && pc1z <= " <<pc1z_max<<")"<<endl;
	  else //sign flipping
	    deadpc1file << "( arm==0 && (pc1z <= " <<pc1z_min<< " && pc1z >= " <<pc1z_max<<")"<<endl;
	if(pc1y_min<pc1y_max)
	  deadpc1file << " && (pc1y >= "<<pc1y_min<< " && pc1y <= " <<pc1y_max<<") )";
	else
	  deadpc1file << " && (pc1y <= "<<pc1y_min<< " && pc1y >= " <<pc1y_max<<") )";
	deadpc1file<<"|| "<<endl;//will give an extra ||, but easy to delete by hand    
      }

    }//end while
  deadpc3file<<";"<<endl<<"//Total of "<<nPC3dead<<" dead PC3 ROCs/regions"<<endl;
  deadpc1file<<";"<<endl<<"//Total of "<<nPC1dead<<" dead PC1 ROCs/regions"<<endl;
  cout<<nPC3dead<<" dead PC3 ROCs/regions"<<endl;
  cout<<nPC1dead<<" dead PC1 ROCs/regions"<<endl;
  }//end deadrocfile
  else{
    cout<<"Invalid filename for deadrocfile."<<endl;
    }
  deadrocfile.close();
  deadpc3file.close();
  deadpc1file.close();
    
  return;

}//end flipROC
