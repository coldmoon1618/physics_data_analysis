#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#include "MyB0Cand.h"
#include "MyPhiCand.h"
#include "MyKsCand.h"

using namespace TMVA;

void GetResponse(TString type, TString myMethodList = "" )
{

    //---------------------------------------------------------------
    // This loads the library
    TMVA::Tools::Instance();

    // Default MVA methods to be trained + tested
    std::map<std::string,int> Use;

    Use["BDTG"]            = 1; // uses Gradient Boost

    std::cout << std::endl;
    std::cout << "==> Start GetResponse" << std::endl;

    // Select methods (don't look at this code - not of interest)
    if (myMethodList != "") {
        for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

        std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
        for (UInt_t i=0; i<mlist.size(); i++) {
            std::string regMethod(mlist[i]);

            if (Use.find(regMethod) == Use.end()) {
                std::cout << "Method \"" << regMethod
                    << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
                for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
                    std::cout << it->first << " ";
                }
                std::cout << std::endl;
                return;
            }
            Use[regMethod] = 1;
        }
    }

    // --------------------------------------------------------------------------------------------------

    // Create the Reader object

    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

    // Create a set of variables and declare them to the reader
    // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
    Float_t foxWolframR1, foxWolframR2, foxWolframR3, foxWolframR4;
    Float_t harmonicMomentThrust0, harmonicMomentThrust1, harmonicMomentThrust2, harmonicMomentThrust3, harmonicMomentThrust4;
    Float_t cleoConeThrust0, cleoConeThrust1, cleoConeThrust2, cleoConeThrust3, cleoConeThrust4, cleoConeThrust5, cleoConeThrust6, cleoConeThrust7, cleoConeThrust8;
    Float_t sphericity, aplanarity, thrust, thrustAxisCosTheta;
    Float_t CS_hso00, CS_hso02, CS_hso04, CS_hso10, CS_hso12, CS_hso14, CS_hso20, CS_hso22, CS_hso24;
    Float_t CS_hoo1, CS_hoo2, CS_hoo3, CS_hoo4, CS_et, CS_mm2;
    Float_t phi_M, phi_cosHel;

    reader->AddVariable("foxWolframR1",          &foxWolframR1           );
    reader->AddVariable("foxWolframR2",          &foxWolframR2           );
    reader->AddVariable("foxWolframR3",          &foxWolframR3           );
    reader->AddVariable("foxWolframR4",          &foxWolframR4           );
    reader->AddVariable("harmonicMomentThrust1", &harmonicMomentThrust1  );
    reader->AddVariable("harmonicMomentThrust2", &harmonicMomentThrust2  );
    reader->AddVariable("harmonicMomentThrust3", &harmonicMomentThrust3  );
    reader->AddVariable("harmonicMomentThrust4", &harmonicMomentThrust4  );
    reader->AddVariable("cleoConeThrust0",       &cleoConeThrust0        );
    reader->AddVariable("cleoConeThrust1",       &cleoConeThrust1        );
    reader->AddVariable("cleoConeThrust2",       &cleoConeThrust2        );
    reader->AddVariable("cleoConeThrust3",       &cleoConeThrust3        );
    reader->AddVariable("cleoConeThrust4",       &cleoConeThrust4        );
    reader->AddVariable("cleoConeThrust5",       &cleoConeThrust5        );
    reader->AddVariable("cleoConeThrust6",       &cleoConeThrust6        );
    reader->AddVariable("cleoConeThrust7",       &cleoConeThrust7        );
    reader->AddVariable("cleoConeThrust8",       &cleoConeThrust8        );
    reader->AddVariable("thrustAxisCosTheta",    &thrustAxisCosTheta     );
    reader->AddVariable("CS_hso00",              &CS_hso00               );
    reader->AddVariable("CS_hso02",              &CS_hso02               );
    reader->AddVariable("CS_hso04",              &CS_hso04               );
    reader->AddVariable("CS_hso10",              &CS_hso10               );
    reader->AddVariable("CS_hso12",              &CS_hso12               );
    reader->AddVariable("CS_hso14",              &CS_hso14               );
    reader->AddVariable("CS_hso20",              &CS_hso20               );
    reader->AddVariable("CS_hso22",              &CS_hso22               );
    reader->AddVariable("CS_hso24",              &CS_hso24               );
    reader->AddVariable("CS_hoo1",               &CS_hoo1                );
    reader->AddVariable("CS_hoo2",               &CS_hoo2                );
    reader->AddVariable("CS_hoo3",               &CS_hoo3                );
    reader->AddVariable("CS_hoo4",               &CS_hoo4                );
    reader->AddVariable("CS_et",                 &CS_et                  );
    reader->AddVariable("CS_mm2",                &CS_mm2                 );
    reader->AddVariable("phi_M",                 &phi_M                  );
    reader->AddVariable("phi_cosHel",            &phi_cosHel             );

    // Book the MVA methods

    TString dir    = "dataset/weights/";
    TString prefix = "TrainBDTG";

    // Book method(s)
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
        if (it->second) {
            TString methodName = TString(it->first) + TString(" method");
            TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
            reader->BookMVA( methodName, weightfile );
        }
    }

    // Book output histograms
    UInt_t nbin = 100;
    TH1F *histBdtG(0);

    histBdtG    = new TH1F( "MVA_BDTG",          "MVA_BDTG",          nbin, -1.0, 1.0 );


    Float_t response, Mbc;
    TTree *res_BDT = new TTree("res_BDT","BDTG response");
    res_BDT -> Branch("response", &response);
    res_BDT -> Branch("Mbc", &Mbc);

    // Prepare input tree (this must be replaced by your data source)
    // in this example, there is a toy tree with signal and one with background events
    // we'll later on use only the "signal" events for the test in this example.
    //
    //TString fname = "Data_cand.root";
    TString fname = type+"_cand.root";
    TFile input(fname);
    std::cout << "--- TMVAClassificationApp    : Using input file: " << fname << std::endl;

    // Event loop

    // Prepare the event tree
    // - Here the variable names have to corresponds to your tree
    // - You can use the same variables as above which is slightly faster,
    //   but of course you can use different ones and copy the values inside the event loop
    //
    std::cout << "--- Select signal sample" << std::endl;
    TTree* theTree = (TTree*)input.Get("B0_cand");

    theTree->SetBranchAddress("foxWolframR1",          &foxWolframR1           );
    theTree->SetBranchAddress("foxWolframR2",          &foxWolframR2           );
    theTree->SetBranchAddress("foxWolframR3",          &foxWolframR3           );
    theTree->SetBranchAddress("foxWolframR4",          &foxWolframR4           );
    theTree->SetBranchAddress("harmonicMomentThrust1", &harmonicMomentThrust1  );
    theTree->SetBranchAddress("harmonicMomentThrust2", &harmonicMomentThrust2  );
    theTree->SetBranchAddress("harmonicMomentThrust3", &harmonicMomentThrust3  );
    theTree->SetBranchAddress("harmonicMomentThrust4", &harmonicMomentThrust4  );
    theTree->SetBranchAddress("cleoConeThrust0",       &cleoConeThrust0        );
    theTree->SetBranchAddress("cleoConeThrust1",       &cleoConeThrust1        );
    theTree->SetBranchAddress("cleoConeThrust2",       &cleoConeThrust2        );
    theTree->SetBranchAddress("cleoConeThrust3",       &cleoConeThrust3        );
    theTree->SetBranchAddress("cleoConeThrust4",       &cleoConeThrust4        );
    theTree->SetBranchAddress("cleoConeThrust5",       &cleoConeThrust5        );
    theTree->SetBranchAddress("cleoConeThrust6",       &cleoConeThrust6        );
    theTree->SetBranchAddress("cleoConeThrust7",       &cleoConeThrust7        );
    theTree->SetBranchAddress("cleoConeThrust8",       &cleoConeThrust8        );
    theTree->SetBranchAddress("thrustAxisCosTheta",    &thrustAxisCosTheta     );
    theTree->SetBranchAddress("CS_hso00",              &CS_hso00               );
    theTree->SetBranchAddress("CS_hso02",              &CS_hso02               );
    theTree->SetBranchAddress("CS_hso04",              &CS_hso04               );
    theTree->SetBranchAddress("CS_hso10",              &CS_hso10               );
    theTree->SetBranchAddress("CS_hso12",              &CS_hso12               );
    theTree->SetBranchAddress("CS_hso14",              &CS_hso14               );
    theTree->SetBranchAddress("CS_hso20",              &CS_hso20               );
    theTree->SetBranchAddress("CS_hso22",              &CS_hso22               );
    theTree->SetBranchAddress("CS_hso24",              &CS_hso24               );
    theTree->SetBranchAddress("CS_hoo1",               &CS_hoo1                );
    theTree->SetBranchAddress("CS_hoo2",               &CS_hoo2                );
    theTree->SetBranchAddress("CS_hoo3",               &CS_hoo3                );
    theTree->SetBranchAddress("CS_hoo4",               &CS_hoo4                );
    theTree->SetBranchAddress("CS_et",                 &CS_et                  );
    theTree->SetBranchAddress("CS_mm2",                &CS_mm2                 );
    theTree->SetBranchAddress("phi_M",                 &phi_M                  );
    theTree->SetBranchAddress("phi_cosHel",            &phi_cosHel             );

//Read in other cand info
    TChain     *B0_cand = new TChain("B0_cand");
    B0_cand -> AddFile(fname);
    MyB0Cand    *candB  = new MyB0Cand();
    candB   -> setBranchAddresses(B0_cand);
    MyPhiCand   *cBPhi  = new MyPhiCand();
    cBPhi   -> setBranchAddresses(B0_cand, "phi_");
    MyKsCand    *cBKs0  = new MyKsCand();
    cBKs0   -> setBranchAddresses(B0_cand, "Ks_");
//

    std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
    TStopwatch sw;
    sw.Start();
    for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

        if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

        theTree->GetEntry(ievt);
        B0_cand->GetEntry(ievt);

        // Return the MVA outputs and fill into histograms
        response = reader->EvaluateMVA( "BDTG method"          );
        histBdtG   ->Fill( response );
        Mbc = candB->Mbc;
        res_BDT -> Fill();

    }

    // Get elapsed time
    sw.Stop();
    std::cout << "--- End of event loop: "; sw.Print();

    // Write histograms

    TString resFile = type+"_resp.root"; 
    TFile *target  = new TFile( resFile,"RECREATE" );
    histBdtG   ->Write();
    res_BDT   ->Write();

    std::cout << "--- Created root file: "<< resFile<<" containing the MVA output histograms" << std::endl;

    delete reader;

    std::cout << "==> GetResponse is done!" << std::endl << std::endl;
}

int main( int argc, char** argv )
{
    TString methodList;
    for (int i=1; i<argc; i++) {
        TString regMethod(argv[i]);
        if(regMethod=="-b" || regMethod=="--batch") continue;
        if (!methodList.IsNull()) methodList += TString(",");
        methodList += regMethod;
    }
    GetResponse(methodList);
    return 0;
}
