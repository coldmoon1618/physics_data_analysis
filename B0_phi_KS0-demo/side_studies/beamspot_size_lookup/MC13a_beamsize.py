import basf2 as b2
import modularAnalysis as ma

nEvents = [187579, 200000]

for MCtype in ['mixed','charged']:
    if(MCtype=='mixed'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009545_task10020000001.root'
        output_file = 'MC13a_mixed_beamspot.root'
        max_events  =  nEvents[0]
    if(MCtype=='charged'):
        input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00009435_task10020000001.root'
        output_file = 'MC13a_charged_beamspot.root'
        max_events  =  nEvents[1]

    path = b2.create_path()
    ma.inputMdstList("default", [input_file], path=path)

    ma.fillParticleListFromMC('Upsilon(4S):generator','',writeOut = True, path=path)

    ma.variablesToNtuple('Upsilon(4S):generator', ['mcX','mcY','mcZ'], filename=output_file, treename='tree',path=path)

    b2.process(path,max_event=max_events)

import os
os.system('rm A_MC13a_combo_beamspot.root')
os.system('hadd sum.root *beamspot.root')
os.system('rm *beamspot.root')
os.system('mv sum.root A_MC13a_combo_beamspot.root')
