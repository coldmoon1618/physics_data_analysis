#!/bin/csh
# $1 = roots, $2 = ckin, $3 = run, $4 = no. of events, $5 = index, $6 = outdir, $7 = libdir, $8 = srcdir, $9 = species, $10 = runno

  source /etc/csh.login
  foreach i (/etc/profile.d/*.csh)
       source $i
  end
  source /opt/phenix/bin/phenix_setup.csh
  setenv LD_LIBRARY_PATH {{$7}/lib}:${LD_LIBRARY_PATH}
  setenv TSEARCHPATH {$7}:${TSEARCHPATH}

  echo "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}"
  echo "TSEARCHPATH = ${TSEARCHPATH}"

  cd $6/Run$3_$9/$1/ckin$2/$5/

  # generate vertex distribution
  ln -sf $8/macros/$1/pythia_ckin$2/pythia.cfg .
  ln -sf $8/macros/generateVertex.C .
  root -l -q -b generateVertex.C\($4,$3,\"$9\"\) >& vertex.out

  # generate pythia events
  ln -sf $8/macros/generatePythia.C .
  root -l -q -b "generatePythia.C($4)" >& pythia.out

  # run pythia events through PISA, generate SimDST.root
  ln -sf $8/macros/glogon.kumac .
  ln -sf $8/macros/Run$3/pisaLinker/* .
  ln -sf $8/macros/Run$3/pisaToDSTLinker/* .
  ln -sf $8/macros/Run$3/pisaToDSTLinker/$9/* .
  pisa < pisa.input >& pisa.out
  root -l -q -b "pisaToDST.C($4)" >& pisaToDst.out 
  root -l -q -b "pisaToDST_Perfect.C($4)" >& pisaToDst_Perfect.out 

  # only delete phpythia.root if you are sure you won't need it later
  rm phpythia.root

  # create ISU track and cluster tree output
  ln -sf $8/macros/runISUJetSim_Perfect.C .
  ln -sf $8/macros/runISUJetSim.C .
  root -l -q -b runISUJetSim_Perfect.C\(\"$9\",$3,$10,$4\) >& trkclus_Perfect.out
  root -l -q -b runISUJetSim.C\(\"$9\",$3,$10,$4\) >& trkclus.out

  # only delete SimDST.root if you are sure you won't need 
  # to regenerate the tracks and cluster tree output!
  rm SimDST.root
  rm SimDST_Perfect.root

  # run MakeJets (hadrons and partons)
  $7/bin/makejets makejets_hadrons.root RunSimTrkClus.root 5.0 0.3 0.3 $3 RunSimParticles.root h >& makejets_hadrons.out
  $7/bin/makejets makejets_hadrons_perfect.root RunSimTrkClus_Perfect.root 5.0 0.3 0.3 $3 RunSimParticles.root h >& makejets_hadrons_perfect.out
  $7/bin/makejets makejets_partons.root RunSimTrkClus.root 5.0 0.3 0.3 $3 RunSimParticles.root p >& makejets_partons.out
  $7/bin/makejets makejets_partons_perfect.root RunSimTrkClus_Perfect.root 5.0 0.3 0.3 $3 RunSimParticles.root p >& makejets_partons_perfect.out

  # final cleanup - delete unwanted files to save space
  rm DchAlwaysDead.dat
  rm DchCalibration.Mc
  rm DchEfficiency.Real
  rm DchGeometry.frame00NoRetracted
  rm DchGeometry.frame00Retracted
  rm DchGeometry.frameMc
  rm DchGeometry.frameReal
  rm DchGeometry.info
  rm DchGeometry.wireMc
  rm DchGeometry.wireReal
  rm PISAEvent.root
  rm Sim3D++.root
  rm crk_cabling_vrdc.txt
  rm event.par
  rm fieldIntegral.dat
  rm finishPisa
  rm flukaaf.dat
  rm flukaerr.dat
  rm generatePythia.C
  rm generateVertex.C
  rm getDead
  rm gffgo.dat
  rm gintphnx.hbk
  rm glogon.kumac
  rm phnx.par
  rm phpy_xsec.root
  rm pisa.input
  rm pisa.kumac
  rm pisaToDST.C
  rm pisaToDST_IOManager.C
  rm pisafile.dat.cZ
  rm pythia.cfg
  rm pad_zero.dat
  rm pad_deadroc.dat
  rm runISUJetSim.C
  rm svxPISA.par
  rm tempdead
  rm xsneut95.dat
  rm -f afsVersion
  rm -f getDead
  rm CVS



