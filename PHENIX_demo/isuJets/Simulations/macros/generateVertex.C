#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TRandom3.h>

using namespace std;

void generateVertex(int nevents = 10, int run = 13, std::string species = "PP")
{
    ofstream vertexFile;
    vertexFile.open("vertex.txt");

    // set vertex cut based on run and species

    float vtxCut = 30.0; 

    std::string PP = "PP";
    std::string pAu = "PAU";
    std::string pAl = "PAL";
    std::string CuAu = "CUAU";

    if(run==13){

      vtxCut = 30.0; 

    }else if(run==12) {
      
      if(species.compare(PP)==0){
	vtxCut = 10.0;  // narrow
      }
      if(species.compare(CuAu)==0){
	vtxCut = 30.0; // normal
      }

    }
    else{
      vtxCut = 30.0; 
    }

    int events = 0;
    while(events < nevents)
        {
            TRandom3 *myRandom3 = new TRandom3();
            myRandom3->SetSeed(0);
            float zvertex = myRandom3->Gaus(0.0, 17);

            if(fabs(zvertex) <= vtxCut)
                {
                    vertexFile << events << " " << zvertex << " " << 0.0 << "\n";
                    events++;
                }
        }
    vertexFile.close();
}


