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
for ele in range(0,200):
    input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/' + str(ele) + '/signalevents_2000.mdst.root')

output_file = 'Trk_eff_SigMC.root'
max_event = 0

path = b2.create_path()
ma.inputMdstList("default", input_file_list, path=path)

from buildVars import *
B0_MC_vars  = setB0MCVars()
pi_Rec_vars = setRecTrkVars()
pi_MC_vars  = setMCDauVars()
K_Rec_vars  = setRecTrkVars()
K_MC_vars   = setMCDauVars()
##########################################################################
# Extract signal events
# skip the rest of path if no signal decay chain
B2phiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310'
ma.fillParticleListFromMC('B0:MC', B2phiKsCut, addDaughters = True, skipNonPrimaryDaughters = False, path = path)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B0:MC'])
path.add_module(skimfilter1)
emptypath1 = b2.create_path()
skimfilter1.if_true(emptypath1,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath1,b2.AfterConditionPath.END)

path.add_module('VariablesToEventBasedTree',
    particleList='B0:MC',
    variables= B0_MC_vars, 
    treeName = 'B0_MC',
    fileName = output_file)
##########################################################################
# Save tracks matched to K+/- daughters of phi(KK)
phiKcuts = 'mcPrimary>0 and thetaInCDCAcceptance>0 and mcMother(PDG)==333 and abs(mcDaughter(0,mcMother(PDG)))==321 and abs(mcDaughter(1,mcMother(PDG)))==321'
ma.fillParticleList('K+:all', '', path=path)
ma.matchMCTruth('K+:all', path=path)
ma.cutAndCopyList('K+:reco','K+:all','isSignal==1 and '+phiKcuts, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList='K+:reco',
    variables= K_Rec_vars,
    treeName = 'K_Rec',
    fileName = output_file)

##########################################################################
# Save MC K+/- from phi(KK)
ma.fillParticleListFromMC('K+:MC',phiKcuts,addDaughters=True, skipNonPrimaryDaughters = False, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList='K+:MC',
    variables= K_MC_vars,
    treeName = 'K_MC',
    fileName = output_file)

##########################################################################
# Save tracks matched to pi+/- daughters of Ks(pipi)
KsPiCuts = 'mcPrimary>0 and thetaInCDCAcceptance>0 and mcMother(PDG)==310 and abs(mcDaughter(0,mcMother(PDG)))==211 and abs(mcDaughter(1,mcMother(PDG)))==211'
ma.fillParticleList('pi+:all', '', path=path)
ma.matchMCTruth('pi+:all', path=path)
ma.cutAndCopyList('pi+:reco','pi+:all','isSignal==1 and '+KsPiCuts, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList='pi+:reco',
    variables= pi_Rec_vars,
    treeName = 'pi_Rec',
    fileName = output_file)

##########################################################################
# Save MC pi+/- from Ks(pipi)
ma.fillParticleListFromMC('pi+:MC',KsPiCuts,addDaughters=True, skipNonPrimaryDaughters = False, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList='pi+:MC',
    variables= pi_MC_vars,
    treeName = 'pi_MC',
    fileName = output_file)

##########################################################################
b2.process(path,max_event)
##########################################################################
