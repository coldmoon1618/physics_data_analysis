#!/bin/csh

#root -l -b -q "PrintPhiInfo.C(0)"
#root -l -b -q PrintPhiInfo.C
#root -l -b -q "PrintKsInfo.C(0)"
#root -l -b -q PrintKsInfo.C
root -l -b -q CompareKsVars.C
root -l -b -q "CompareKsVars.C(1)"
