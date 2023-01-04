void runISUJetSim_Perfect(const char *species, int run, int runno, int nevents = 0, const char *simDstFile = "SimDST_Perfect.root")
{
    gSystem->Load("libfun4all.so");	// framework + reco modules
    gSystem->Load("libPHPythiaEventGen.so");
    gSystem->Load("libPHPythia.so");
    gSystem->Load("librecal.so");
    gSystem->Load("libsimreco.so");	// framework + reco modules

    //For fastjet package
    gSystem->Load("libCGAL.so");
    gSystem->Load("libfastjet.so");
    gSystem->Load("libfastjettools.so");
    gSystem->Load("libsiscone.so");
    gSystem->Load("libsiscone_spherical.so");

    //Set LD_LIBRARY_PATH to pick this up!
    gSystem->Load("libISUJetTrkClus.so");

    //for spin stuff
    gSystem->Load("libuspin.so");

    recoConsts *rc = recoConsts::instance();
    rc->set_IntFlag("RUNNUMBER", runno);

    Fun4AllServer *se = Fun4AllServer::instance();

    ISUJetTrkClus *myTrkClus = new ISUJetTrkClus("RunSimTrkClus_Perfect.root",run,species);
    myTrkClus->setMB(false);
    myTrkClus->setData(false);
    myTrkClus->setSkipDeadWarnMap(true);
    myTrkClus->setSkipDCBrokenWires(true);
    se->registerSubsystem(myTrkClus);
 
    Fun4AllInputManager *in2 = new Fun4AllNoSyncDstInputManager("DSTin2", "DST", "TOP");
    se->registerInputManager(in2);
    in2->AddFile(simDstFile);//needed by TrkClus 

        
    if(nevents > 0)
        {
            cout << "Running over " << nevents << " Events" << endl;
        }
    else
        {
            cout << "Running over all Events" << endl;
        }

    se->run(nevents);
    cout << "Calling Fun4AllServer::End()" << endl;
    se->End();

    cout << "Done." << endl;
}




