##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

from buildPoseidonVars import *
B0_Rec_vars  = setB0RecVars()
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
if len(sys.argv) != 3:
    print('Usage: basf2 -i inputFile '+ sys.argv[0] + ' jobID  outName ')
    print('')
    print('jobID: an int indentifying the job number. It will be included in the output file name')
    print('outName: output file name, without .root. The final file name will look like: [outName]_num_[jobID].root')
    sys.exit(1)

jobNum = sys.argv[1]
outName = sys.argv[2] 
output_file = outName+'_'+jobNum+'.root'

max_event = 0
path = b2.create_path()
ma.inputMdstList("default", [], path=path)

##########################################################################
# phi reco
phiCuts  = '0.97<M<1.1 '
phiCuts += 'and daughter(0,thetaInCDCAcceptance)==1 '
phiCuts += 'and daughter(1,thetaInCDCAcceptance)==1 '
phiCuts += 'and daughter(0,nCDCHits)>0 '
phiCuts += 'and daughter(1,nCDCHits)>0 '
phiCuts += 'and daughter(0,kaonID)>.05 '
phiCuts += 'and daughter(1,kaonID)>.05 '
ma.fillParticleList('K-', '', path=path)
ma.reconstructDecay('phi:cuts -> K+ K-', phiCuts,  path=path)
vt.treeFit('phi:cuts',     conf_level=0, updateAllDaughters = True,  path=path)
ma.cutAndCopyList('phi:reco','phi:cuts','chiProb>0', path=path)

# K_S0 reco
Ks0Cuts  = 'goodBelleKshort == 1 and chiProb>0'
stdKshorts(path=path)
vt.treeFit('K_S0:merged', conf_level=0, updateAllDaughters = True,  path=path)
ma.cutAndCopyList('K_S0:reco','K_S0:merged',Ks0Cuts, path=path)

# B0 -> phi K_S0 reco
ma.reconstructDecay('B0:rec -> phi:reco K_S0:reco', '', path=path)
vt.treeFit('B0:rec',     conf_level=-1, updateAllDaughters = False,  path=path)
ma.cutAndCopyList('B0:reco','B0:rec','chiProb>0 and 5.2<Mbc<5.3 and abs(deltaE)<0.2',path=path)

# Only carry on if non-zero candidates in B0:reco
skimfilter = b2.register_module('SkimFilter')
skimfilter.param('particleLists', ['B0:reco'])
path.add_module(skimfilter)
emptypath = b2.create_path()
skimfilter.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter.if_false(emptypath,b2.AfterConditionPath.END)

#default is pi+ and gamma
ma.buildEventShape(foxWolfram=True, path=path)
ma.buildRestOfEvent(target_list_name='B0:reco', path=path)
ma.appendROEMask('B0:reco', 'IPtracks', 'nCDCHits > 0 and abs(d0) < 0.05 and abs(z0) < 0.1', '', path=path)
ma.buildContinuumSuppression('B0:reco', 'IPtracks', path=path)
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
