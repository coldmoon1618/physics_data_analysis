#!/bin/csh

root -l -b -q "SaveSingleCand.C(0,0)"
root -l -b -q "PlotSingleCandidate.C(0,0)"
root -l -b -q "SaveSingleCand.C(0,1)"
root -l -b -q "PlotSingleCandidate.C(0,1)"
root -l -b -q "SaveSingleCand.C(1,0)"
root -l -b -q "PlotSingleCandidate.C(1,0)"
root -l -b -q "SaveSingleCand.C(1,1)"
root -l -b -q "PlotSingleCandidate.C(1,1)"
