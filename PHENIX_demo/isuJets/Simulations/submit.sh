#!/bin/csh

# roots$1, ckin$2, run$3, nevents$4, index$5, outdir$6, libdir$7, srcdir$8, species$9

condor_submit ISUJetSim.job -append "Arguments=$1 $2 $3 $4 $5 $6 $7 $8 $9 $10" -append "Log=$6/Run$3_$9/$1/ckin$2/$5/condor.log" -append "Output=$6/Run$3_$9/$1/ckin$2/$5/condor.out" -append "Error=$6/Run$3_$9/$1/ckin$2/$5/condor.err" 
