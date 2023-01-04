#!/bin/csh

root -l -b -q Save2tMatched.C
root -l -b -q FitPhiSignal.C
root -l -q -b OriginalCutsEff.C
root -l -b -q CDCHitsCutEff.C
root -l -b -q KID_CDC_cuts_compare.C
