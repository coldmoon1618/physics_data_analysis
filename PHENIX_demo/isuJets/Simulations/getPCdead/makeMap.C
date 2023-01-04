/*
 * Author: Yunxiao "Michelle" Zhai
 * Email: zhai@iastate.edu
 * Maps PC dead areas used in presentation slides
 * The complexity of the code mainly came from
 * the fact I started debugging with the deadch.
 * Yes, this code can be simplifed if we start
 * with on the arrangement of ROCs in mind.
 * I might do that if simplification is needed.
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
void makeMap(){
  TFile *fOut = new TFile("PadChamberMaps.root","RECREATE");
  //^^To have the same maps I had in the presentation from above file
  //gStyle -> SetOptStat(0)
  //gStyle -> SetPalette(23)
  ////////
  //For dead channels
  ////////
  string line;
  int packetid, channelIndex, padType; 
  /*
    ifstream deadchfile("/sphenix/user/yzhai/ISUJetAna/getDead/pad_deadch_393483.dat");
  TH2I *hDeadCh_PC3W = new TH2I("hDeadCh_PC3W","Dead Ch map in PC3W",216,-108,108,160,0,160);
  TH2I *hDeadCh_PC3E = new TH2I("hDeadCh_PC3E","Dead Ch map in PC3E",216,-108,108,160,0,160);
  
  TH2I *hDeadCh_PC1W = new TH2I("hDeadCh_PC1W","Dead Ch map in PC1W",216,-108,108,160,0,160);
  TH2I *hDeadCh_PC1E = new TH2I("hDeadCh_PC1E","Dead Ch map in PC1E",216,-108,108,160,0,160);
  
  if(deadchfile.is_open()){
  getline(deadchfile,line);
  cout<<line<<" dead channels"<<endl;
    while(deadchfile>>packetid>>channelIndex>>padType){
        int mapy = 0;
        int mapx = 0;
        if(packetid>=4001&&packetid<=4008){
          mapy = (packetid%4001)*20+channelIndex%20;
          mapx = channelIndex/20;
          mapx = -mapx-1;
          hDeadCh_PC1W -> Fill(mapx,mapy);
        }
        else if(packetid>=4009&&packetid<=4016){
          mapy = (packetid%4009)*20+19-channelIndex%20;
          mapx = channelIndex/20;
          hDeadCh_PC1W -> Fill(mapx,mapy);
        }
        else if(packetid>=4017&&packetid<=4024){
          mapy = (packetid%4017)*20+channelIndex%20;
          mapx = channelIndex/20;
          mapx = -mapx-1;
          hDeadCh_PC1E -> Fill(mapx,mapy);
        }
        else if(packetid>=4025&&packetid<=4032){
          mapy = (packetid%4025)*20+19-channelIndex%20;
          mapx = channelIndex/20;
          hDeadCh_PC1E -> Fill(mapx,mapy);
        }
        if(packetid>=4065&&packetid<=4072){
          mapy = (packetid%4065)*20+channelIndex%20;
          mapx = channelIndex/20;
          mapx = -mapx-1;
          hDeadCh_PC3W -> Fill(mapx,mapy);
        }
        else if(packetid>=4073&&packetid<=4080){
          mapy = (packetid%4073)*20+19-channelIndex%20;
          mapx = channelIndex/20;
          hDeadCh_PC3W -> Fill(mapx,mapy);
        }
        else if(packetid>=4081&&packetid<=4088){
          mapy = (packetid%4081)*20+channelIndex%20;
          mapx = channelIndex/20;
          mapx = -mapx-1;
          hDeadCh_PC3E -> Fill(mapx,mapy);
        }
        else if(packetid>=4089&&packetid<=4096){
          mapy = (packetid%4089)*20+19-channelIndex%20;
          mapx = channelIndex/20;
          hDeadCh_PC3E -> Fill(mapx,mapy);
        }
    }//end while
  }//end deadchfile
  else{
    cout<<"Invalid filename for deadchfile."<<endl;
    }
  deadchfile.close();
  */
  /////////
  //For dead roc
  ////////
  int grprow, grpcolumn, rocType;
    ifstream deadrocfile("pad_deadroc_393483.dat");
  
  TH2I *hDeadROC_PC3W = new TH2I("hDeadROC_PC3W","Dead ROC map in PC3W",216,-108,108,160,0,160);
  TH2I *hDeadROC_PC3E = new TH2I("hDeadROC_PC3E","Dead ROC map in PC3E",216,-108,108,160,0,160);
  
  TH2I *hDeadROC_PC1W = new TH2I("hDeadROC_PC1W","Dead ROC map in PC1W",216,-108,108,160,0,160);
  TH2I *hDeadROC_PC1E = new TH2I("hDeadROC_PC1E","Dead ROC map in PC1E",216,-108,108,160,0,160);
  
  if(deadrocfile.is_open()){
  getline(deadrocfile,line);
  cout<<line<<" dead ROCs"<<endl;
  //Total 10 rows, but in each chamber half 5 rows of ROCS
  //With 9 ROCS per each row
  //and each ROC reads in 3*16 channel
    while(deadrocfile>>packetid>>grprow>>grpcolumn>>rocType){
      int mapy = 0;
      int mapx = 0;
      
      for(int tgl = 1; tgl <= 3; tgl++){
        int startchid = grpcolumn*240+((tgl-1)*4)*20+grprow*4;
        for(int channel = 1; channel <= 16; channel++){
          int chid = startchid + ((channel-1)/4)*20+(channel-1)%4;
          if(packetid>=4001&&packetid<=4008){
            mapy = (packetid%4001)*20+chid%20;
            mapx = chid/20;
            mapx = -mapx-1;
            hDeadROC_PC1W -> Fill(mapx,mapy);
          }
          else if(packetid>=4009&&packetid<=4016){
            mapy = (packetid%4009)*20+19-chid%20;
            mapx = chid/20;
            hDeadROC_PC1W -> Fill(mapx,mapy);
          }
          else if(packetid>=4017&&packetid<=4024){
            mapy = (packetid%4017)*20+chid%20;
            mapx = chid/20;
            mapx = -mapx-1;
            hDeadROC_PC1E -> Fill(mapx,mapy);
          }
          else if(packetid>=4025&&packetid<=4032){
            mapy = (packetid%4025)*20+19-chid%20;
            mapx = chid/20;
            hDeadROC_PC1E -> Fill(mapx,mapy);
          }
          if(packetid>=4065&&packetid<=4072){
            mapy = (packetid%4065)*20+chid%20;
            mapx = chid/20;
            mapx = -mapx-1;
            hDeadROC_PC3W -> Fill(mapx,mapy);
          }
          else if(packetid>=4073&&packetid<=4080){
            mapy = (packetid%4073)*20+19-chid%20;
            mapx = chid/20;
            hDeadROC_PC3W -> Fill(mapx,mapy);
          }
          else if(packetid>=4081&&packetid<=4088){
            mapy = (packetid%4081)*20+chid%20;
            mapx = chid/20;
            mapx = -mapx-1;
            hDeadROC_PC3E -> Fill(mapx,mapy);
          }
          else if(packetid>=4089&&packetid<=4096){
            mapy = (packetid%4089)*20+19-chid%20;
            mapx = chid/20;
            hDeadROC_PC3E -> Fill(mapx,mapy);
          }
        }//end channel
      }//end tgl

    }//end while
  }//end deadrocfile
  else{
    cout<<"Invalid filename for deadrocfile."<<endl;
    }
  deadrocfile.close();
  
  fOut -> Write();
  fOut -> Close();
  
  return;

}//end makeMap
