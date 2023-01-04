#include "MakeJets.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  
  if( (argc<3) || (argc>9) ){
    cout << " usage: makejets outputfile trkclusfile [jet min pT] [reco jet R] [truth jet R] [run] [simfile] [partons/hadrons]" << endl;
    cout << " passing [simfile] requires [partons/hadrons] = p,h" << endl; 
    return -1; 
  }

  float reco_jet_R = 0.3; 
  float truth_jet_R = reco_jet_R; 
  float jet_min_pT = 5.0; 
  int ml_run = 13; 
  std::string simfile = ""; 
  char type = 'r'; // real by default (not valid)

  if(argc>=4) jet_min_pT = atof(argv[3]); 
  
  if(argc>=5){
    reco_jet_R = atof(argv[4]); 
  }

  if(argc>=6){
    truth_jet_R = atof(argv[5]); 
  }
  else{
    truth_jet_R = reco_jet_R; 
  }

  if(argc>=7){
    ml_run = atoi(argv[6]); 
  }
  
  if(argc==8){
    cout << " passing [simfile] requires [partons/hadrons] = p,h" << endl; 
    return -1; 
  }

  if(argc==9){
    simfile = argv[7]; 
    type = *argv[8]; 
  }
  
  std::cout << endl << " jet_min_pT = " << jet_min_pT << " GeV" << endl; 
  std::cout << " reco_jet_R = " << reco_jet_R << endl; 
  std::cout << " truth_jet_R = " << truth_jet_R << endl; 
  std::cout << " MLP run configuration = " << ml_run << endl; 
  std::cout << " simfile = " << simfile << endl; 
  std::cout << " type = " << type << endl << endl; 

  MakeJets makeJets;
  int retval = makeJets.InitNow(argv[1], argv[2], reco_jet_R, truth_jet_R, jet_min_pT, ml_run, simfile, type); 

  std::cout << "MakeJets init return value:  " << retval << std::endl;

  // only run if init had no error 
  if(retval>=0) {
    int retvalMain = makeJets.Run();
    cout << "Run return value:  " << retvalMain << endl;
  }

  return 0;

}//end main

