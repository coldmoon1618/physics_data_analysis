import basf2 as b2
import modularAnalysis as ma

# Last update: Feb 18, 2021
input_file = '/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/0/signalevents_2000.mdst.root'
output_file = 'Bug_report.root'

# main path
path = b2.create_path()
ma.inputMdstList("default", [input_file], path=path)

from buildVars import *
B0_MC_vars   = setB0MCVars()

# Directly loading B0 -> phi Ks
B2phiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310 '
B2phiKsCut += 'and daughter(0,nDaughters)>=2 and daughter(1,nDaughters)>=2 '
B2phiKsCut += 'and abs(daughter(0,daughter(0,mcPDG)))==321 and abs(daughter(0,daughter(1,mcPDG)))==321 '
B2phiKsCut += 'and abs(daughter(1,daughter(0,mcPDG)))==211 and abs(daughter(1,daughter(1,mcPDG)))==211'

ma.fillParticleListFromMC('B0:MC', B2phiKsCut, addDaughters = True, skipNonPrimaryDaughters = False, path = path)
 
skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B0:MC'])
path.add_module(skimfilter1)
emptypath = b2.create_path()
skimfilter1.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath,b2.AfterConditionPath.END)

path.add_module('VariablesToEventBasedTree',
    particleList='B0:MC',
    variables= B0_MC_vars, 
    treeName = 'B0_MC',
    fileName = output_file)

# Use reconstructMCDecay
ma.fillParticleListFromMC('K+:MC', '', path = path)
ma.fillParticleListFromMC('pi+:MC', '', path = path)

ma.reconstructMCDecay('phi:recMC  -> K+:MC K-:MC','',path=path)
ma.reconstructMCDecay('K_S0:recMC -> pi+:MC pi-:MC','',path=path)
ma.reconstructMCDecay('B0:recMC   ->  phi:recMC K_S0:recMC','',path=path)

path.add_module('VariablesToEventBasedTree',
    particleList='B0:recMC',
    variables= B0_MC_vars, 
    treeName = 'B0_recMC',
    fileName = output_file)

b2.process(path)
