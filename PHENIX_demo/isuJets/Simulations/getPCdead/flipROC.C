/*
 * Author: Yunxiao "Michelle" Zhai
 * Email: zhai@iastate.edu
 * */
#include <iostream>
#include <fstream>
void flipROC(){
  string line;
  int packetid, grprow, grpcolumn, rocType;
  ifstream deadrocfile("pad_deadroc_393483.dat");
  
  ofstream badrocfile("pad_deadroc_393483_flip.dat");

  if(deadrocfile.is_open()){
  getline(deadrocfile,line);
  cout<<line<<" dead ROCs"<<endl;
  badrocfile<<line<<endl;
    while(deadrocfile>>packetid>>grprow>>grpcolumn>>rocType){
      if(packetid>=4065&&packetid<=4080){
        grprow = 4 - grprow;
        if(packetid%2==1){
          packetid++;
        }else{
          packetid--;
        } 
      }
      //badrocfile<<packetid<<"\t"<<grprow<<"\t"<<grpcolumn<<"\t"<<222<<endl;    
      badrocfile<<packetid<<" "<<grprow<<" "<<grpcolumn<<" "<<222<<endl;    
    }//end while
  }//end deadrocfile
  else{
    cout<<"Invalid filename for deadrocfile."<<endl;
    }
  deadrocfile.close();
  badrocfile.close();
    
  return;

}//end flipROC
