Explains the structure and contents of offline/analysis/ISUJets

//////////////////////////////////////////////////////////////////////////
//                         SIMULATION OVERVIEW                          //                    
//////////////////////////////////////////////////////////////////////////
//  Aside from cvs co /MyisuJetsArea[=srcdir] & /MyisuJetTrkClusArea    //
//  Setup:                                                              //
//  /MyISUJets[libdir]                                                  //
//    /build & /install [for ISUJetTrkClus, PythiaParticles & MakeJets] //
//    /condor_run12 /condor_run13 /condor_15                            //
//      ISUJetSim.job runJetSim.csh submit.sh [unmodified]              //
//      submit_condor.sh                                                //
//      [edit: ckin, run_year, runno, outdir, libdir, srcdir, species]  //
//    /output [=outdir] No need to make sub dirs, ^^takes care of it.   //
//  Run  : /submit_condor.sh nevents$1, fIndex_ini$2 fIndex_fin $3      //
//////////////////////////////////////////////////////////////////////////

==========================================================================
Active directories:
--------------------------------------------------------------------------
/Simulations
ISUJetSim.job runJetSim.csh submit.sh submit_condor.sh
See README in that dir for more detailed instructions

  /macros
  generatePythia.C generateVertex.C glogon.kumac runISUJetSim.C
  files to run simulation at pythia level
    /Run12
    /Run13
    /Run15
    For each /Run_yr:
      /pisaLinker
      Sim3D++.root  afsVersion[empty folder]  event.par  flukaaf.dat  
      gffgo.dat  phnx.par  pisa.input  pisa.kumac  xsneut95.dat
      /pisaToDSTLinker
      Dchgeometry.* [Drift chamber files]
      pad_deadroc.dat pad_zero.dat [Pad Chamber files]
        /getDead [empty folder]

    /200
      /ckin10
    /510
      /ckin10, 20, 30, 50, 80, 100, 150
    For each /E_cm: pythia.cfg files for ckin#

    /getPCdead
    ReadMe.txt getBadRoc.C flipROC.C makeMap.C pisaToDST.C
    Shows example to generate, map, & use chamber dead area files.
--------------------------------------------------------------------------
/PythiaParticles
PythiaParticles/C&h & other framework tools

Input  : phpythia.root 
Output : Run#SimParticles.root
Install: cd /buildDir/; /codeDir/autogen.sh --prefix=/installDir/
Compile: cd /buildDir/; make install
To run : root -l generatePythia.C
--------------------------------------------------------------------------
/anaISUJetTrkClus/makeJets
MakeJets.C&h PHENIXFilter.cpp&h main.C & other framework tools

Input  : outputfile trkclusfile [jet min pT] [reco jet R] [truth jet R] [simfile] [partons/hadrons]   
Output : Run#(Sim)JetAna.root
Install: cd /buildDir/; /codeDir/autogen.sh --prefix=/installDir/
Compile: cd /buildDir/; make install
To run : makejets outputfile trkclusfile [jet min pT] [reco jet R] [truth jet R] [simfile] [partons/hadrons]
Other files in folder
To run : root -l (-b needed to save .png files correctly)
         .x myMacro.C(params)
==========================================================================
Deprecated directories:
/ISUJetTrkClus
Deprecated in favor of offline/AnalysisTrain/ISUJetTrkClus
  /Run15pAuCalibs
  Now in offline/AnalysisTrain/ISUJetTrkClus/Run15pAuCalibs
/ISUJets
Abandoned project, now makeJets already based on framework
==========================================================================

Updated: YZhai May 6, 2019

