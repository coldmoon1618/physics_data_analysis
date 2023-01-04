import basf2 as b2
import modularAnalysis as ma
import vertex as vt

from stdCharged import stdK

ratio_list = [255,270,803,201,192,665]
nEvents = [ele*249 for ele in ratio_list]
#nEvents = [2,2,2,2,2,2]

input_pre = '/home/belle2/zhai/B0_phi_KS0/Combo_MC13a_files/mdst_000001_prod000097'
input_idx = ['06','16','26','36','46','56'] 
input_suf = '_task10020000001.root'
input_typ = ['mixed','charged','uubar','ccbar','ddbar','ssbar']

from buildVars import *
phi_Rec_vars = setPhiRecVars()
phi_MC_vars  = setPhiMCVars()

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
    stdK("all", path=path)
    ma.reconstructDecay('phi:Rec -> K+:all K-:all', '0.97 < M < 1.10 ', path=path)
    vt.treeFit('phi:Rec',         conf_level=0, updateAllDaughters = True, path=path)
    ma.matchMCTruth('phi:Rec',    path=path)
   
    path.add_module('VariablesToEventBasedTree', 
        particleList    ='phi:Rec',
        variables       = phi_Rec_vars,
        treeName        ='phi_Rec',
        fileName        = output_file) 

##########################################################################
# Load MC phi
    ma.fillParticleListFromMC('phi:MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path)

    path.add_module('VariablesToEventBasedTree', 
        particleList    ='phi:MC',
        variables       = phi_MC_vars,
        treeName        ='phi_MC',
        fileName        = output_file) 

##########################################################################
    b2.process(path, max_event=max_events)
##########################################################################

import os
os.system('rm Combo_phi_selection.root')
os.system('hadd sum.root *output.root')
os.system('rm *output.root')
os.system('mv sum.root Combo_phi_selection.root')
print('\n output file: Combo_phi_selection.root')
