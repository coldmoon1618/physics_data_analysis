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
for ele in range(0,1):
    input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/' + str(ele) + '/signalevents_2000.mdst.root')

output_file = 'SigMC_B0_Rec_20k.root'
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
##########################################################################
# Extract signal events
# skip the rest of path1 if no signal decay chain
isBphiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310'
ma.fillParticleListFromMC('B0:MC', isBphiKsCut, addDaughters = True, skipNonPrimaryDaughters = True, path = path1)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B0:MC'])
path1.add_module(skimfilter1)
emptypath1 = b2.create_path()
skimfilter1.if_true(emptypath1,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath1,b2.AfterConditionPath.END)

##########################################################################
# Saving in the order Y4S_chain, BphiKs_MC, BphiKs_rec, phi_rec, Ks_rec
##########################################################################
# Save entire decay chain
ma.fillParticleListFromMC('Upsilon(4S):MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

n=200
list_of_indices = [str(i) for i in range(n)]

myCollection = []
wrapper     = 'genParticle({variable}, varForMCGen(mcMother(mdstIndex)))'
prefix      = 'MothIndex'
myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

wrapper     = 'genParticle({variable}, varForMCGen(useCMSFrame(p)))'
prefix      = 'pstar'
myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

wrapper     = 'genParticle({variable}, mcDaughter(0,mdstIndex))'
prefix      = 'firstDau'
myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

decayChainVarList = ['PDG','E','p','px','py','pz','pt','mcFSR','mcISR','thetaInCDCAcceptance']
decayChainVarList += ['dx','dy','dz','dr','dcosTheta','dphi','distance']

for decVars in decayChainVarList:
    wrapper = 'genParticle({variable}, varForMCGen('+decVars+'))'
    prefix  = decVars
    myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

vm.addAlias('nMCGen', 'nMCParticles')
list_of_variables = ['nMCGen']

for i in range(n):
    for ele in myCollection:
        list_of_variables.append(ele[i])

path1.add_module('VariablesToEventBasedTree',
    particleList= 'Upsilon(4S):MC',
    variables   = list_of_variables,
    treeName    = 'Y4S_chain',
    fileName    = output_file)

##########################################################################
b2.process(path1,max_event)
##########################################################################
