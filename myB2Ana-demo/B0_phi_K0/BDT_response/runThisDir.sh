#! /bin/bash

#root -l -b -q TrainBDTG.C
#root -l -b -q 'GetResponse.C("FullMC")'
root -l -b -q 'GetResponse.C("SigMC")'
#root -l -b -q 'GetResponse.C("Data")'
#root -l -b -q CompareResponse.C
