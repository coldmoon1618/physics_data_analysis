##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

from buildPoseidonVars import *
B0_Rec_vars  = setB0RecVars()
B0_MC_vars   = setB0MCVars()
Y4Schain_vars= setY4Schain()
##########################################################################
# Analysis modules
import basf2 as b2
import modularAnalysis as ma
import vertex as vt
import flavorTagger as ft
from stdV0s import stdKshorts

##########################################################################
# Basic setups
import sys
if len(sys.argv) != 2:
    print('Usage: basf2 SigMC_B0_noUpdate.py index')
    print('')
    sys.exit(1)

ele = sys.argv[1]

input_file_list = []
input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/' + str(ele) + '/signalevents_2000.mdst.root')

output_file = 'output/SigMC_B0_'+str(ele)+'.root'
max_event = 0

path = b2.create_path()
ma.inputMdstList("default", input_file_list, path=path)

##########################################################################
# Extract signal events
# skip the rest of path if no signal decay chain
# this eliminates the non-exact events due to early mat. int.

B2phiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310 '
B2phiKsCut += 'and daughter(0,nDaughters)>=2 and daughter(1,nDaughters)>=2 '
B2phiKsCut += 'and abs(daughter(0,daughter(0,mcPDG)))==321 and abs(daughter(0,daughter(1,mcPDG)))==321 '
B2phiKsCut += 'and abs(daughter(1,daughter(0,mcPDG)))==211 and abs(daughter(1,daughter(1,mcPDG)))==211'
ma.fillParticleListFromMC('B0', B2phiKsCut, addDaughters = True, skipNonPrimaryDaughters = False, path = path)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B0'])
path.add_module(skimfilter1)
emptypath = b2.create_path()
skimfilter1.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath,b2.AfterConditionPath.END)

##########################################################################
# Saving in the order Y4S_chain, B0_MC, B0_rec
##########################################################################
# Save entire decay chain
ma.fillParticleListFromMC('Upsilon(4S):MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList= 'Upsilon(4S):MC',
    variables   = Y4Schain_vars,
    treeName    = 'Y4S_chain',
    fileName    = output_file)

##########################################################################
# Save MC B0 info

path.add_module('VariablesToEventBasedTree',
    particleList='B0',
    variables= B0_MC_vars, 
    treeName = 'B0_MC',
    fileName = output_file)

##########################################################################
# Full reco to assess cut efficiency
# phi reco
phiCuts  = '0.97<M<1.1 '
phiCuts += 'and daughter(0,thetaInCDCAcceptance)==1 '
phiCuts += 'and daughter(1,thetaInCDCAcceptance)==1 '
phiCuts += 'and daughter(0,nCDCHits)>0 '
phiCuts += 'and daughter(1,nCDCHits)>0 '
phiCuts += 'and daughter(0,kaonID)>.05 '
phiCuts += 'and daughter(1,kaonID)>.05 '
ma.fillParticleList('K-', '', path=path)
ma.reconstructDecay('phi:reco -> K+ K-', phiCuts,  path=path)
vt.treeFit('phi:reco',     conf_level=-1, updateAllDaughters = True,  path=path)
ma.matchMCTruth('phi:reco',      path=path)

# K_S0 reco
Ks0Cuts  = 'goodBelleKshort == 1 and chiProb>0'
stdKshorts(path=path)
vt.treeFit('K_S0:merged', conf_level=-1, updateAllDaughters = True,  path=path)
ma.matchMCTruth('K_S0:merged',      path=path)
ma.cutAndCopyList('K_S0:reco','K_S0:merged',Ks0Cuts, path=path)

# B0 -> phi K_S0 reco
ma.reconstructDecay('B0:reco -> phi:reco K_S0:reco', '', path=path)
vt.treeFit('B0:reco',     conf_level=-1, updateAllDaughters = False,  path=path)
ma.matchMCTruth('B0:reco',      path=path)

#default is pi+ and gamma
ma.buildEventShape(foxWolfram=True, path=path)
ma.buildRestOfEvent(target_list_name='B0:reco', path=path)
ma.appendROEMask('B0:reco', 'IPtracks', 'nCDCHits > 0 and abs(d0) < 0.05 and abs(z0) < 0.1', '', path=path)
ma.buildContinuumSuppression('B0:reco', 'IPtracks', path=path)
ft.flavorTagger('B0:reco', weightFiles='B2nunubarBGx1', path=path)
vt.TagV(list_name='B0:reco',MCassociation='breco',constraintType='noConstraint',path=path)

##########################################################################
# Save B0 reco candidate outputs
path.add_module('VariablesToEventBasedTree',
    particleList='B0:reco',
    variables = B0_Rec_vars,
    treeName = 'B0_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
b2.process(path,max_event)
##########################################################################
