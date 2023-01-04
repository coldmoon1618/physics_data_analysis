Offline analysis directory for B -> DX

Utility:

    include_Cpp/
        [Sort of hisotircal now as subdirs diverge, might entralize again later]

        My*.h
        Header files of custom defined structs
        rootlogon.C
        Usage: ln -s ~/thisDir/rootlogon anaDir/. for ROOT to pick the path

        SaveSingleCand.C
        Select single best candidate per event after cuts

Analysis:

    Tag_side_reco/

        Workflow:
            Reco_xx.py          : event-based ntuple using basf2
            SaveModeFiles.C     : save single best candidate into mode-specific files 
            Fit and Plot offline

        Data_73ifb/
            See dir ReadMe.txt

        MC14ri_c_GRID/
            TO-DO: unify to the same modes as the ones selected from data reco

        mc_1ifb_data_73ifb_rel5217/
            Outdated but content for documention for now.
    
Last updated(CDT/CST):
Nov 16, 2021 
