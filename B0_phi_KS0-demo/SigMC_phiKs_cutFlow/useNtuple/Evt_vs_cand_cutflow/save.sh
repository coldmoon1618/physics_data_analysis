#!/bin/csh

bsub -o log root -l -b -q "SaveCand.C(0,1,-1,1)"  
