import basf2 as b2
import modularAnalysis as ma
import vertex as vt
import flavorTagger as ft
import variables.collections as vc
import variables.utils as vu
from variables import variables as vm
from stdV0s import stdKshorts

##########################################################################
# Basic setups
input_file_list = []
for ele in range(0,10):
    input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/' + str(ele) + '/signalevents_2000.mdst.root')

output_file = 'SigMC_B0_Rec_20k_noPhiPreVtxCut.root'
max_event = 0

path1 = b2.create_path()
ma.inputMdstList("default", input_file_list, path=path1)

from buildVars import *
phi_Rec_vars = setPhiRecVars()
phi_MC_vars  = setPhiMCVars()
Ks_Rec_vars  = setKsRecVars()
Ks_MC_vars   = setKsMCVars()
B0_Rec_vars  = setB0RecVars()
B0_MC_vars   = setB0MCVars()
Y4Schain_vars= setY4Schain()

##########################################################################
# Extract signal events
# skip the rest of path1 if no signal decay chain
B2phiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310'
ma.fillParticleListFromMC('B0:MC', B2phiKsCut, addDaughters = True, skipNonPrimaryDaughters = False, path = path1)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B0:MC'])
path1.add_module(skimfilter1)
emptypath1 = b2.create_path()
skimfilter1.if_true(emptypath1,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath1,b2.AfterConditionPath.END)

##########################################################################
# Saving in the order Y4S_chain, B0_MC, B0_rec, phi_rec, Ks_rec
##########################################################################
# Save entire decay chain
ma.fillParticleListFromMC('Upsilon(4S):MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

path1.add_module('VariablesToEventBasedTree',
    particleList= 'Upsilon(4S):MC',
    variables   = Y4Schain_vars,
    treeName    = 'Y4S_chain',
    fileName    = output_file)

##########################################################################
# Save MC B0 info

path1.add_module('VariablesToEventBasedTree',
    particleList='B0:MC',
    variables= B0_MC_vars, 
    treeName = 'B0_MC',
    fileName = output_file)

##########################################################################
# Full reco to assess cut efficiency
# phi reco
ma.fillParticleList('K-', '', path=path1)
ma.reconstructDecay('phi:reco -> K+ K-', '',  path=path1)
vt.treeFit('phi:reco',     conf_level=-1, updateAllDaughters = True,  path=path1)
ma.matchMCTruth('phi:reco',      path=path1)

# K_S0 reco
stdKshorts(path=path1)
vt.treeFit('K_S0:merged', conf_level=-1, updateAllDaughters = True,  path=path1)
ma.matchMCTruth('K_S0:merged',      path=path1)

# B0 -> phi K_S0 reco
ma.reconstructDecay('B0:reco -> phi:reco K_S0:merged', '', path=path1)
vt.treeFit('B0:reco',     conf_level=-1, updateAllDaughters = True,  path=path1)
ma.matchMCTruth('B0:reco',      path=path1)

##########################################################################
# Save B0 reco candidate outputs
path1.add_module('VariablesToEventBasedTree',
    particleList='B0:reco',
    variables = B0_Rec_vars,
    treeName = 'B0_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save phi reco candidate outputs
path1.add_module('VariablesToEventBasedTree',
    particleList='phi:reco',
    variables= phi_Rec_vars, 
    treeName = 'phi_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save phi MC candidate outputs
ma.fillParticleListFromMC('phi:MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

path1.add_module('VariablesToEventBasedTree',
    particleList='phi:MC',
    variables= phi_MC_vars,
    treeName = 'phi_MC',
    fileName = output_file)

##########################################################################
# Save Ks  reco candidate outputs

path1.add_module('VariablesToEventBasedTree',
    particleList='K_S0:merged',
    variables= Ks_Rec_vars,
    treeName = 'Ks_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save Ks  MC candidate outputs
ma.fillParticleListFromMC('K_S0:MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

path1.add_module('VariablesToEventBasedTree',
    particleList='K_S0:MC',
    variables= Ks_MC_vars,
    treeName = 'Ks_MC',
    fileName = output_file)

##########################################################################
b2.process(path1,max_event)
##########################################################################
