import basf2 as b2
import modularAnalysis as ma
import vertex as vt
import flavorTagger as ft
import variables.collections as vc
import variables.utils as vu
from variables import variables as vm
from stdV0s import stdKshorts

input_file_list = []
input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/0/signalevents_2000.mdst.root')

output_file = 'framework_friendly_decay_chain.root'
max_event = 5

path1 = b2.create_path()
ma.inputMdstList("default", input_file_list, path=path1)

# Save entire decay chain
ma.fillParticleListFromMC('Upsilon(4S):MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

#n=200
n=20
list_of_indices = [str(i) for i in range(n)]

myCollection = []
wrapper     = 'genParticle({variable}, varForMCGen(mcMother(mdstIndex)))'
prefix      = 'MothIndex'
myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

wrapper     = 'genParticle({variable}, useCMSFrame(p))'
prefix      = 'pstar'
myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

wrapper     = 'genParticle({variable}, mcDaughter(0,mdstIndex))'
prefix      = 'firstDau'
myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

decayChainVarList = ['PDG','E','p','px','py','pz','pt','mcFSR','mcISR','thetaInCDCAcceptance']
decayChainVarList += ['x','y','z','dx','dy','dz','dr','dcosTheta','dphi','distance']

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
    treeName    = 'sigEvtChain',
    fileName    = output_file)

b2.process(path1,max_event)
