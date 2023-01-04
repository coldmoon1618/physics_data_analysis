import basf2 as b2
import modularAnalysis as ma
import vertex as vt

#Larger selection, total number of events 596,276
nEvents = [66735, 71154, 200000, 161492, 49691, 47204]

#Regular 200k events
#nEvents = [22384,23866,67083,54167,16667,15833]

for MCtype in ['mixed','charged','uubar','ccbar','ddbar','ssbar']:
    if(MCtype=='mixed'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009545_task10020000001.root'
        output_file = 'MC13a_mixed_phi_loose.root'
        max_events  =  nEvents[0]
    if(MCtype=='charged'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009435_task10020000001.root'
        output_file = 'MC13a_charged_phi_loose.root'
        max_events  =  nEvents[1]
    if(MCtype=='uubar'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009436_task10020000001.root'
        output_file = 'MC13a_uubar_phi_loose.root'
        max_events  =  nEvents[2] 
    if(MCtype=='ccbar'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009437_task10020000001.root'
        output_file = 'MC13a_ccbar_phi_loose.root'
        max_events  =  nEvents[3] 
    if(MCtype=='ddbar'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009438_task10020000001.root'
        output_file = 'MC13a_ddbar_phi_loose.root'
        max_events  =  nEvents[4] 
    if(MCtype=='ssbar'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009439_task10020000001.root'
        output_file = 'MC13a_ssbar_phi_loose.root'
        max_events  =  nEvents[5] 

    path = b2.create_path()
    ma.inputMdstList("default", [input_file], path=path)

    from stdCharged import stdK

    stdK("all", path=path)
    ma.reconstructDecay('phi:all -> K+:all K-:all', '0.97 < M < 1.1', path=path)
    vt.treeFit('phi:all',     conf_level=0, updateAllDaughters = True, path=path)
    ma.matchMCTruth("phi:all", path=path)

    ma.variablesToNtuple('phi:all', 
            ['M','p','E','phi','theta',
            'isSignal','chiProb','pt',
            'useCMSFrame(p)',

            'daughter(0,M)','daughter(0,E)','daughter(0,p)','daughter(0,phi)','daughter(0,theta)',
            'daughter(0,useCMSFrame(p))','daughter(0,pt)',
            'daughter(0,electronID)','daughter(0,muonID)','daughter(0,pionID)',
            'daughter(0,kaonID)','daughter(0,protonID)','daughter(0,deuteronID)',
            'daughter(0,thetaInCDCAcceptance)','daughter(0,nCDCHits)','daughter(0,lastCDCLayer)',
            'daughter(0,d0)','daughter(0,z0)',
            'daughter(0,dr)','daughter(0,dz)',

            'daughter(1,M)','daughter(1,E)','daughter(1,p)','daughter(1,phi)','daughter(1,theta)',
            'daughter(1,useCMSFrame(p))','daughter(1,pt)',
            'daughter(1,electronID)','daughter(1,muonID)','daughter(1,pionID)',
            'daughter(1,kaonID)','daughter(1,protonID)','daughter(1,deuteronID)',
            'daughter(1,thetaInCDCAcceptance)','daughter(1,nCDCHits)','daughter(1,lastCDCLayer)',
            'daughter(1,d0)','daughter(1,z0)',
            'daughter(1,dr)','daughter(1,dz)',
    ], filename=output_file, treename="tree", path=path)

    b2.process(path, max_event=max_events)

import os
os.system('rm A_MC13a_combo_phi_loose.root')
os.system('hadd sum.root *loose.root')
os.system('rm *loose.root')
os.system('mv sum.root A_MC13a_combo_phi_loose.root')
