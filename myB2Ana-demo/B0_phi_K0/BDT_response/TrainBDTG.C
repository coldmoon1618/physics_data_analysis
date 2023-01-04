#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int TrainBDTG( TString myMethodList = "" )
{
   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();
   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;
   // Boosted Decision Trees
   Use["BDTG"]            = 1; // uses Gradient Boost
   // ---------------------------------------------------------------
   std::cout << std::endl;
   std::cout << "==> Start TrainBDTG" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return 1;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------
   // Read training and test data
   TString sigFile = "signal55.root";
   TString bkgFile = "qqbar.root";
   TFile sigInput(sigFile);
   TFile bkgInput(bkgFile);
   std::cout << "--- TrainBDTG       : Using input files: " << sigFile << " and " << bkgFile << std::endl;

   // Register the training and test trees
   TTree *signalTree     = (TTree*)sigInput.Get("B0_cand");
   TTree *background     = (TTree*)bkgInput.Get("B0_cand");

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "BDTG_result.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TrainBDTG", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

   // Define the input variables that shall be used for the MVA training
   dataloader->AddVariable( "foxWolframR1", "R1", "", 'F' );
   //dataloader->AddVariable( "foxWolframR2", "R2", "", 'F' );
   //dataloader->AddVariable( "foxWolframR3", "R3", "", 'F' );
   //dataloader->AddVariable( "foxWolframR4", "R4", "", 'F' );
   dataloader->AddVariable( "harmonicMomentThrust1", "h1", "", 'F' );
   //dataloader->AddVariable( "harmonicMomentThrust2", "h2", "", 'F' );
   dataloader->AddVariable( "harmonicMomentThrust3", "h3", "", 'F' );
   //dataloader->AddVariable( "harmonicMomentThrust4", "h4", "", 'F' );
   //dataloader->AddVariable( "cleoConeThrust0", "cc0", "", 'F' );
   //dataloader->AddVariable( "cleoConeThrust1", "cc1", "", 'F' );
   //dataloader->AddVariable( "cleoConeThrust2", "cc2", "", 'F' );
   dataloader->AddVariable( "cleoConeThrust3", "cc3", "", 'F' );
   dataloader->AddVariable( "cleoConeThrust4", "cc4", "", 'F' );
   dataloader->AddVariable( "cleoConeThrust5", "cc5", "", 'F' );
   dataloader->AddVariable( "cleoConeThrust6", "cc6", "", 'F' );
   dataloader->AddVariable( "cleoConeThrust7", "cc7", "", 'F' );
   dataloader->AddVariable( "cleoConeThrust8", "cc8", "", 'F' );
   dataloader->AddVariable( "thrustAxisCosTheta","thAxCosTh","",'F');
   dataloader->AddVariable( "CS_hso00","hso00","",'F');
   //dataloader->AddVariable( "CS_hso02","hso02","",'F');
   dataloader->AddVariable( "CS_hso04","hso04","",'F');
   dataloader->AddVariable( "CS_hso10","hso10","",'F');
   //dataloader->AddVariable( "CS_hso12","hso12","",'F');
   dataloader->AddVariable( "CS_hso14","hso14","",'F');
   dataloader->AddVariable( "CS_hso20","hso20","",'F');
   dataloader->AddVariable( "CS_hso22","hso22","",'F');
   dataloader->AddVariable( "CS_hso24","hso24","",'F');
   dataloader->AddVariable( "CS_hoo1","hoo1","",'F');
   dataloader->AddVariable( "CS_hoo2","hoo2","",'F');
   dataloader->AddVariable( "CS_hoo3","hoo3","",'F');
   dataloader->AddVariable( "CS_hoo4","hoo4","",'F');
   //dataloader->AddVariable( "CS_et","et","",'F');
   //dataloader->AddVariable( "CS_mm2","mm2","",'F');
   dataloader->AddVariable( "phi_M","mKK","",'F');
   dataloader->AddVariable( "phi_cosHel","cosHel","",'F');
   /*
   dataloader->AddVariable( "harmonicMomentThrust0", "h0", "", 'F' );
   dataloader->AddVariable( "sphericity","sph","",'F');
   dataloader->AddVariable( "aplanarity","apl","",'F');
   dataloader->AddVariable( "thrust","th","",'F');
   dataloader->AddVariable( "R2","R2","",'F');
   dataloader->AddVariable( "cosTBTO","TBTO","",'F');
   dataloader->AddVariable( "cosTBz","TBz","",'F');
   dataloader->AddVariable( "thrustBm","thB","",'F');
   dataloader->AddVariable( "thrustOm","thO","",'F');
   */

   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   // You can add an arbitrary number of signal or background trees
   dataloader->AddSignalTree    ( signalTree,     signalWeight );
   dataloader->AddBackgroundTree( background, backgroundWeight );

   // Set individual event weights (the variables must exist in the original TTree)
   // Make sure the value is always non-zero or omit the line of code, otherwise get warning:
   // "Variable XX has zero, negative, or NaN RMS^2: nan ==> set to zero. Please check the variable content"
   //dataloader->SetBackgroundWeightExpression( "__weight__" );

   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = "isBBbar==1";
   TCut mycutb = "isBBbar==0";

   // Tell the dataloader how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used
   // for training, and the other half for testing:
   //
   dataloader->PrepareTrainingAndTestTree( mycuts, mycutb, "SplitMode=random:!V" );
   //
   // To also specify the number of testing events, use:
   //
   //    dataloader->PrepareTrainingAndTestTree( mycut,
   //         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   //
   //dataloader->PrepareTrainingAndTestTree( mycuts, mycutb, "nTrain_Signal=22719:nTrain_Background=22719:SplitMode=Random:NormMode=NumEvents:!V" );

   // ### Book MVA methods
   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

   // Now you can tell the factory to train, test, and evaluate the MVAs
   factory->TrainAllMethods();
   factory->TestAllMethods();
   factory->EvaluateAllMethods();

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TrainBDTG is done!" << std::endl;

   delete factory;
   delete dataloader;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

   return 0;
}

int main( int argc, char** argv )
{
   // Select methods (don't look at this code - not of interest)
   TString methodList;
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(",");
      methodList += regMethod;
   }
   return TrainBDTG(methodList);
}
