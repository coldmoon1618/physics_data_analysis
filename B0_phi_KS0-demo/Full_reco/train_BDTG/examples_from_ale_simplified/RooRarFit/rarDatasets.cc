/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooRarFit
 *    File: $Id: rarDatasets.cc,v 1.20 2007/06/29 08:37:36 zhanglei Exp $
 * Authors: Lei Zhang
 * History:
 * 
 * Copyright (C) 2005 University of California, Riverside
 *****************************************************************************/

// -- CLASS DESCRIPTION [RooRarFit] --
// This class provides dataset class for RooRarFit

#include "RooRarFit/rarVersion.hh"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "TMD5.h"

#include "Roo1DTable.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooStringVar.h"

#include "RooRarFit/rarDatasets.hh"

/// \brief Trivial ctor
///
/// Usually the objects should be created using other ctors.
rarDatasets::rarDatasets()
  : rarConfig(),
    _dsd(0), _fullFObs(0)
{
  init();
}

/// \brief Default ctor
///
/// \param configFile The config file
/// \param configSec The config section
/// \param actionSec The action section
///
/// The default ctor initializes common data members,
/// and because there is only one instantiation of this class,
/// #_configStr, name and title are set to constant values.
/// It then calls #init.
rarDatasets::rarDatasets(const char *configFile, const char *configSec,
			 const char *actionSec)
  : rarConfig(configFile, configSec, "null", "Datasets", "Datasets"),
    _actionSec(actionSec), _dsd(0), _fullFObs(0)
{
  init();
}

rarDatasets::~rarDatasets()
{
  // _dataSets.Delete();
}

/// \brief Initial function called by ctor
///
/// \p init is called by the ctor.
/// It first instantiates a #rarDatasetDef class for dataset definition.
/// It then reads in the info of datasets,
/// and prints out the datasets configured int the config section.
/// It creates those datasets by calling #createDataSet
/// and adds them to the dataset list, #_dataSets.
/// It also checks if the datasets need to be tabulated for output.
void rarDatasets::init()
{
  cout<<"init of rarDatasets:"<<endl;
  // first set #_createFundamental and #_fullNameSchema
  _createFundamental=kTRUE;
  _fullNameSchema="self";

  // first create dsd object
  TString dataDefSec=readConfStr("dsdSec", "Dataset Definition");
  _dsd=new rarDatasetDef(_configFile, dataDefSec);
  _fullObs=_dsd->getFullObs();
  // create full fundamental obs
  _fullFObs=new RooArgSet("theFullFObs");
  RooArgList fullObs(*_fullObs);
  for (Int_t i=0; i<fullObs.getSize(); i++) {
    RooAbsArg *theVar=fullObs.at(i);
    _fullFObs->add(*((RooAbsArg*)_rarVars.FindObject(theVar->GetName())));
  }
  
  // get number of datasets, first check action section
  TString datasetsStr=readConfStr("Datasets", "notSet", _actionSec);
  // if not found, go to dsi section
  if ("notSet"==datasetsStr) datasetsStr=readConfStr("Datasets", "");
  rarStrParser datasetsStrParser=datasetsStr;
  Int_t nDataset=datasetsStrParser.nArgs();
  if (nDataset<=0) {
    cout<<"no datasets defined in RooStringVar \"Datasets\" in section \""
	<<_configSec<<"\""<<endl;
    exit(-1);
  }
  // read in datasets info
  RooArgSet datasetList("Dataset List");
  for (Int_t i=0; i<nDataset; i++) {
    RooStringVar *dataset=new RooStringVar
      (datasetsStrParser[i], datasetsStrParser[i], "notSet", 8192);
    datasetList.addOwned(*dataset);
  }
  datasetList.readFromFile(_configFile, 0, _configSec);
  //datasetList.Print("v");
  // list configed datasets
  cout<<endl
      <<"The datasets defined in config file:"<<endl;
  for (Int_t i=0; i<nDataset; i++) {
    TString datasetStr=(RooStringVar&)datasetList[datasetsStrParser[i]];
    cout<<Form(" dataset%02d ",i)<<datasetsStrParser[i]<<" "<<datasetStr<<endl;
  }
  cout<<endl;
  // now read in the datasets
  for (Int_t i=0; i<nDataset; i++) {
    Bool_t isUB=kFALSE;
    TString datasetStr=(RooStringVar&)datasetList[datasetsStrParser[i]];
    RooDataSet *data=createDataSet(datasetsStrParser[i]+" "+datasetStr, isUB);
    data->SetName(getDSName(datasetsStrParser[i]));
    _dataSets.Add(data);
    if (isUB) ubStr(getDSName(datasetsStrParser[i]), (Char_t*)"Unblinded");
    //data->Print("v");
    //data->get()->Print("v");
  }
  cout<<endl<<"Datasets read in:"<<endl;
  _dataSets.Print();
  cout<<endl;
  
  // do we need to have weight for datasets?
  setWeightVar();
  // do we need to tabulate it?
  tabulateDatasets();
}

/// \brief Set weight var for datasets
///
/// It sets weight var according to configs
void rarDatasets::setWeightVar()
{
  TString swvStr=readConfStr("setWeightVar", "no");
  if (swvStr.BeginsWith("no")) return;
  
  // do we have default weight var?
  rarStrParser swvStrParser=swvStr;
  TString swvName=swvStrParser[0];
  if (!_fullFObs->find(swvName)) swvName="";
  else swvStrParser.Remove();
  if (""!=swvName) {
    cout<<endl;
    for (Int_t dIdx=0; dIdx<_dataSets.GetSize(); dIdx++) {
      // orig:
      //RooDataSet *data=(RooDataSet*) _dataSets.At(dIdx);
      //cout<<" Using "<<swvName<<" as weight var for "<<data->GetName()<<endl;
      //data->setWeightVar(swvName);

      // modified (setWeightVar has been deprecated):
      RooDataSet *Tdata=(RooDataSet*) _dataSets.At(dIdx);
      const RooArgSet *cVars = Tdata->get();
      RooDataSet *data= new RooDataSet( Tdata->GetName(), Tdata->GetTitle(), *cVars, swvName);
      cout <<" Using "<< swvName <<" as weight var for "<< data->GetName() << endl;
    }
  }
  // do we have more specifications?
  TList dList;
  while (swvStrParser.nArgs()>0) {
    swvName=swvStrParser[0];
    swvStrParser.Remove();
    if (_fullFObs->find(swvName)) { // find weightVar
      cout<<endl;
      for (Int_t dIdx=0; dIdx<dList.GetSize(); dIdx++) {
	RooDataSet *Tdata=(RooDataSet*) _dataSets.At(dIdx);
	const RooArgSet *cVars = Tdata->get();
	RooDataSet *data= new RooDataSet( Tdata->GetName(), Tdata->GetTitle(), *cVars, swvName);
	cout <<" Using "<< swvName <<" as weight var for "<< data->GetName() << endl;
      }
      dList.Clear();
      continue;
    }
    // add the name as dataset
    TObject *data=_dataSets.FindObject(getDSName(swvName));
    if (data) dList.Add(data);
    else cout<<" Can not find dataset "<<swvName<<endl;
  }
}

/// \brief Tabulate datasets wrt cats
/// \param dsName The dataset name if provided
///
/// It tabulates datasets wrt categories defined according to configs
void rarDatasets::tabulateDatasets(const Char_t *dsName)
{
  TString tdsStr=readConfStr("tabulateDatasets", "no");
  if (tdsStr.BeginsWith("no")) return;
  
  RooArgList catList(*getCats());
  for (Int_t dIdx=0; dIdx<_dataSets.GetSize(); dIdx++) {
    RooDataSet *data=(RooDataSet*) _dataSets.At(dIdx);
    if (dsName&&TString(dsName)!=data->GetName()) continue;
    for (Int_t i=0; i<catList.getSize(); i++) {
      RooAbsCategory *theCat=(RooAbsCategory *)&(catList[i]);
      Roo1DTable *theTable(0);
      if (theCat&&(theTable=data->table(*theCat))) {
	theTable->Print();
	// get frac table
	TIterator* catTypeIter = theCat->typeIterator();
	RooCatType *theType(0);
	while(theType=(RooCatType*)catTypeIter->Next()) {
	  TString typeName=theType->GetName();
	  cout<<"    "<<typeName<<"\t"<<theTable->getFrac(typeName)<<endl;
	}
	cout<<endl;
	delete catTypeIter;
      }
    }
  }
}

/// \brief Create dataset name based on input string
/// \param name The input name string
/// \return The created dataset name
TString rarDatasets::getDSName(TString name)
{
  // first remove "
  name.ReplaceAll("\"", "");
  name.ReplaceAll("  ", " ");
  name.ReplaceAll(">=", ".GE.");
  name.ReplaceAll(">", ".GT.");
  name.ReplaceAll("<=", ".LE.");
  name.ReplaceAll("<", ".LT.");
  name.ReplaceAll("==", ".EQ.");
  name.ReplaceAll("!=", ".NEQ.");
  name.ReplaceAll("=", ".EQ.");
  name.ReplaceAll("&&", ".AND.");
  name.ReplaceAll("||", ".OR.");
  for (Int_t i=0; i<name.Length(); i++)
    if (!isalnum(name[i])) name[i]='_';
  
  return name;
}

/// \brief Return the named dataset.
/// \param name The name of the dataset to return
/// \return The returned dataset
///
/// It first checks if dataset named \p name exsits in #_dataSets,
/// if yes, it returns the dataset,
/// if no, it parses \p name and if the number of tokens
/// is greater than 0, it will check if there is dataset named
/// as the first token, if yes, it will reduce that dataset
/// according to the second token and return the reduced dataset.
/// Otherwise, it returns null.
RooDataSet *rarDatasets::getData(const char *name)
{
  RooDataSet *theData=(RooDataSet*)_dataSets.FindObject(getDSName(name));
  if (theData) return theData;
  // can not find the data set parser the name
  rarStrParser nameParser=name;
  if (nameParser.nArgs()<=0) return theData;
  theData=(RooDataSet*)_dataSets.FindObject(getDSName(nameParser[0]));
  if (!theData) return theData; //can not find any
  // reduce the dataset
  theData=(RooDataSet*)theData->reduce(nameParser[1]);
  theData->SetName(getDSName(name));
  _dataSets.Add(theData);
  // check if need to set ub bit
  if (!isBlind(getDSName(nameParser[0]))) {
    ubStr(getDSName(name), (Char_t*)"Unblinded");
  }
  cout<<" New dataset created"<<endl;
  if (theData) {
    //theData->Print("v");
    theData->Print();
    tabulateDatasets(theData->GetName());
  }
  return theData;
}

/// \brief Return/set the unblind string for the dataset
/// \param dsName The name of the dataset
/// \param ubStrVal The ubStr value (to set)
/// \return The unblind string for the dataset
TString rarDatasets::ubStr(TString dsName, Char_t *ubStrVal)
{
  TString ubStrName="ub_"+dsName;
  // have we calculated it?
  RooStringVar *theStr=(RooStringVar*)_UBs.find(ubStrName);
  if (theStr&&("notSet"!=TString(theStr->getVal()))) {
    if (ubStrVal) theStr->setVal(ubStrVal);
    return theStr->getVal();
  }
  if (!theStr) {
    theStr=new RooStringVar(ubStrName, ubStrName, "notSet", 8192);
    _UBs.add(*theStr);
  }
  if (ubStrVal) {
    theStr->setVal(ubStrVal);
    return theStr->getVal();
  }
  // compute chksum
  TMD5 chksum;
  stringstream o;
  // read in all the data entries
  RooDataSet *theData=getData(dsName);
  if (!theData) {
    cout<<" W A R N I N G !"<<endl
        <<" Can not find dataset named "<<dsName<<" for ub calculation!"<<endl;
    return theStr->getVal();
  }
  Int_t nEvt=theData->numEntries();
  Int_t nStep=nEvt/10000;
  nStep++;
  Int_t i=0;
  while (i<nEvt) {
    RooArgList evt(*theData->get(i));
    evt.writeToStream((ostream&)o, kTRUE);
    i+=nStep;
  }
  string chkStr=o.str();
  chksum.Update((UChar_t*)chkStr.c_str(), chkStr.length());
  chksum.Final();
  theStr->setVal(chksum.AsString());
  
  return theStr->getVal();
}

/// \brief Check if the named dataset is blind or not
/// \param dsName The name of the dataset
/// \return Boolean for blind (true) or unblind (false)
Bool_t rarDatasets::isBlind(TString dsName)
{
  Bool_t retVal(kTRUE);
  TString ubStrVal=ubStr(dsName);
  if ("notSet"==ubStrVal) {
    cout<<" W A R N I N G !"<<endl
        <<" Somehow the ub status for "<<dsName<<" is "<<ubStrVal<<endl
        <<" So "<<dsName<<" remains blind. Please double check"<<endl;
    return retVal;
  }
  if ("Unblinded"==ubStrVal) return kFALSE;
  rarStrParser ubIDParser=ubStrVal;
  rarStrParser dsiUBStrParser=readConfStr("ub_"+dsName, "");
  while(dsiUBStrParser.nArgs()>0) {
    TString dsiUBStr=dsiUBStrParser[0];
    dsiUBStrParser.Remove();
    if (ubIDParser.Have(dsiUBStr)) return kFALSE;
  }
  
  return retVal;
}
