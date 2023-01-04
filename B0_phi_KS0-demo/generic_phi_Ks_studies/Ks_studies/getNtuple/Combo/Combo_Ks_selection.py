import basf2 as b2
import modularAnalysis as ma
import vertex as vt

from stdCharged import stdPi
from stdV0s import stdKshorts
#from variables import variables as vm

#import variables.collections as vc
#import variables.utils as vu

ratio_list = [255,270,803,201,192,665]
nEvents = [ele*249 for ele in ratio_list]
#nEvents = [2,2,2,2,2,2]

input_pre = '/home/belle2/zhai/B0_phi_KS0/Combo_MC13a_files/mdst_000001_prod000097'
input_idx = ['06','16','26','36','46','56'] 
input_suf = '_task10020000001.root'
input_typ = ['mixed','charged','uubar','ccbar','ddbar','ssbar']

from buildVars import *
Ks_Rec_vars = setKsRecVars()
Ks_MC_vars  = setKsMCVars()

for i in range(6): 
##########################################################################
# Set up input and output
    input_file  = input_pre + input_idx[i] + input_suf
    output_file = input_typ[i] + '_output.root'
    max_events  = nEvents[i]

    path = b2.create_path()
    ma.inputMdstList("default", [input_file], path=path)

##########################################################################
# Naive 2-trk reconstruction
    stdPi("all", path=path)
    ma.reconstructDecay('K_S0:2trk -> pi+:all pi-:all', '0.45 < M < 0.55 ', path=path)
    vt.treeFit('K_S0:2trk',         conf_level=0, updateAllDaughters = True, path=path)
    ma.matchMCTruth('K_S0:2trk',    path=path)
   
    path.add_module('VariablesToEventBasedTree', 
        particleList    ='K_S0:2trk',
        variables       = Ks_Rec_vars,
        treeName        ='Ks_2trk',
        fileName        = output_file) 

##########################################################################
# Use merged K short for comparison
    stdKshorts(path=path)
    vt.treeFit('K_S0:merged',       conf_level=0, updateAllDaughters = True, path=path)
    ma.matchMCTruth('K_S0:merged',  path=path)

    path.add_module('VariablesToEventBasedTree', 
        particleList    ='K_S0:merged',
        variables       = Ks_Rec_vars,
        treeName        ='Ks_Rec',
        fileName        = output_file) 

##########################################################################
# Load MC K short
    ma.fillParticleListFromMC('K_S0:MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path)

    path.add_module('VariablesToEventBasedTree', 
        particleList    ='K_S0:MC',
        variables       = Ks_MC_vars,
        treeName        ='Ks_MC',
        fileName        = output_file) 

##########################################################################
    b2.process(path, max_event=max_events)
##########################################################################

import os
os.system('rm Combo_Ks_selection.root')
os.system('hadd sum.root *output.root')
os.system('rm *output.root')
os.system('mv sum.root Combo_Ks_selection.root')
print('\n output file: Combo_Ks_selection.root')
