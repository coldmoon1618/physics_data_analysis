#!/bin/csh

# nevents$1, fIndex_ini$2 fIndex_fin$3

# CHANGE THE OUTPUT DIRECTORY AS NECESSARY!
# This is where the output files will end up
set outputdir = "/phenix/mpcex/lajoie/ISU/SimOut"

# CHANGE THE LIBRARY DIRECTORY AS NECESSARY!
# This needs to point to your compiled library area
# At least: 
# offline/AnalysisTrain/ISUJetTrkClus,
# offline/analysis/ISUJets/PythiaParticles
# offline/analysis/ISUJets/anaISUJetTrkClus/makeJets 
# are required!
# NOTE: point to "install"; we'll take it from there... 
set libdir = "/phenix/mpcex/lajoie/ISU/ISUJetTrkClus/install" 

# CHANGE THE LIBRARY DIRECTORY AS NECESSARY!
# This directory contains the simulations source 
# files that will be linked -
# offline/analysis/ISUJets/Simulations
set srcdir = "/phenix/mpcex/lajoie/ISU/offline/analysis/ISUJets/Simulations"

# CHANGE THE ROOTS AS NECESSARY
# valid values are 200 and 510 
set roots = 510

# CHANGE THE CKIN VALUE AS NECESSARY
# make sure the corresponding pythia configuration file exists!
set ckin = 50

# CHANGE THE RUN NUMBER AS NECESSARY
# valid values are 12, 13 or 15
set run = 13

# CHANGE THE SPECIES AS NECESSARY
# valid values are "PP", "PAU" and "CUAU"
set species = "PP"

# CHANGE THE RUN NUMBER AS NECESSARY
# this is a valid, good run number for the run year of interest
set runno = 393483

mkdir -p $outputdir/Run{$run}_{$species}/$roots/ckin{$ckin}

set i = $2
while ($i < $3)
  
  rm -rf $outputdir/Run{$run}_{$species}/$roots/ckin{$ckin}/$i/
  mkdir $outputdir/Run{$run}_{$species}/$roots/ckin{$ckin}/$i/    

  ./submit.sh $roots $ckin $run $1 $i $outputdir $libdir $srcdir $species $runno
  #sleep 2s
  @ i++

end
