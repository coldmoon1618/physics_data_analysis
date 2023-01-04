/*
 * Author: Yunxiao "Michelle" Zhai
 * Email: zhai@iastate.edu
 * Simplified version of makeMap.C that make the same 
 * qualitative map.
 * June 8, 2019
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
void makeMap_simp(){
  TFile *fOut = new TFile("simplified_PCmap.root","RECREATE");
  //^^To have the same maps I had in the presentation from above file
  //gStyle -> SetOptStat(0)
  //gStyle -> SetPalette(23)
  ////////
  //For dead channels
  ////////
  string line;
  int packetid, grprow, grpcolumn, rocType;
    ifstream deadrocfile("pad_deadroc_393483.dat");
  
  TH2I *hDeadROC_PC3W = new TH2I("hDeadROC_PC3W","Dead ROC map in PC3W",18,-9,9,40,0,40);
  TH2I *hDeadROC_PC3E = new TH2I("hDeadROC_PC3E","Dead ROC map in PC3E",18,-9,9,40,0,40);
  
  TH2I *hDeadROC_PC1W = new TH2I("hDeadROC_PC1W","Dead ROC map in PC1W",18,-9,9,40,0,40);
  TH2I *hDeadROC_PC1E = new TH2I("hDeadROC_PC1E","Dead ROC map in PC1E",18,-9,9,40,0,40);
  
  if(deadrocfile.is_open()){
  getline(deadrocfile,line);
  cout<<line<<" dead ROCs"<<endl;
  //Total 10 rows, but in each chamber half 5 rows of ROCS
  //With 9 ROCS per each row
    while(deadrocfile>>packetid>>grprow>>grpcolumn>>rocType){
      int mapy = 0;
      int mapx = 0;
      
          if(packetid>=4001&&packetid<=4008){
            mapy = (packetid%4001)*5+grprow+0.5;
            mapx = -grpcolumn-1.5;
            hDeadROC_PC1W -> Fill(mapx,mapy);
          }
          else if(packetid>=4009&&packetid<=4016){
            mapy = (packetid%4009)*5+5-grprow-0.5;
            mapx = grpcolumn+0.5;
            hDeadROC_PC1W -> Fill(mapx,mapy);
          }
          else if(packetid>=4017&&packetid<=4024){
            mapy = (packetid%4017)*5+grprow+0.5;
            mapx = -grpcolumn-1.5;
            hDeadROC_PC1E -> Fill(mapx,mapy);
          }
          else if(packetid>=4025&&packetid<=4032){
            mapy = (packetid%4025)*5+5-grprow-0.5;
            mapx = grpcolumn+0.5;
            hDeadROC_PC1E -> Fill(mapx,mapy);
          }
          if(packetid>=4065&&packetid<=4072){
            mapy = (packetid%4065)*5+grprow+0.5;
            mapx = -grpcolumn-1.5;
            hDeadROC_PC3W -> Fill(mapx,mapy);
          }
          else if(packetid>=4073&&packetid<=4080){
            mapy = (packetid%4073)*5+5-grprow-0.5;
            mapx = grpcolumn+0.5;
            hDeadROC_PC3W -> Fill(mapx,mapy);
          }
          else if(packetid>=4081&&packetid<=4088){
            mapy = (packetid%4081)*5+grprow+0.5;
            mapx = -grpcolumn-1.5;
            hDeadROC_PC3E -> Fill(mapx,mapy);
          }
          else if(packetid>=4089&&packetid<=4096){
            mapy = (packetid%4089)*5+5-grprow-0.5;
            mapx = grpcolumn+0.5;
            hDeadROC_PC3E -> Fill(mapx,mapy);
          }

    }//end while
  }//end deadrocfile
  else{
    cout<<"Invalid filename for deadrocfile."<<endl;
    }
  deadrocfile.close();
  
  fOut -> Write();
  fOut -> Close();
  
  return;

}//end makeMap_simp.C
