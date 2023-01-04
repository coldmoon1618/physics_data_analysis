#! /bin/bash

root -l -b -q PlotMbc.C
root -l -b -q TrainBDTG.C
root -l -b -q 'GetResponse.C("Ana")'
root -l -b -q 'GetResponse.C("Data")'
root -l -b -q CompareResponse.C
