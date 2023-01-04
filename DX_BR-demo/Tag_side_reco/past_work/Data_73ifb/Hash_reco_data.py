# Saving all the candidates pass cut
# All channels allowed
##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

from buildVars import *
B_Tag_vars  = setB_TagVars(False)
##########################################################################
# Analysis modules
import basf2 as b2
import fei
import modularAnalysis as ma
import vertex as vt
from variables import variables as vm

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
file_prefix = outName+'_'+jobNum+'.root'

max_event = 0
path = b2.create_path()
ma.inputMdstList("default", [], path=path)

##########################################################################
# Tag side: F E I
b2.conditions.prepend_globaltag(ma.getAnalysisGlobaltag())

# Get FEI default channels.
# Utilise the arguments to toggle on and off certain channels
B_preselection = 'Mbc>5.1 and abs(deltaE)<0.6'
B_preselection+= ' and decMode!=5'
B_preselection+= ' and decMode!=6'
B_preselection+= ' and decMode!=7'
B_preselection+= ' and decMode!=8'
B_preselection+= ' and decMode!=9'
B_preselection+= ' and decMode!=10'
B_preselection+= ' and decMode!=11'
B_preselection+= ' and decMode!=12'
B_preselection+= ' and decMode!=13'
B_preselection+= ' and decMode!=33'
B_preselection+= ' and decMode!=34'

particles = fei.get_default_channels(
    B_extra_cut  = B_preselection,
    chargedB     = True,
    neutralB     = False,
    hadronic     = True,
    semileptonic = False,
    baryonic     = True,
)

# Set up FEI configuration specifying the FEI prefix, monitor is for internal characteristics
configuration = fei.FeiConfiguration(prefix='FEIv4_2021_MC14_release_05_01_12', monitor=False)

# Add FEI path to the path to be processed
feistate = fei.get_path(particles, configuration)
path.add_path(feistate.path)

Btag_cuts = 'Mbc>5.2 and abs(deltaE)<0.5 and extraInfo(SignalProbability)>0.01'
ma.cutAndCopyList('B+:myList','B+:generic', Btag_cuts, path=path)

# Ranking after the cuts
ma.rankByHighest("B+:myList", "SigProb", path=path)
vm.addAlias("Rank",'extraInfo(SigProb_rank)')
ma.cutAndCopyList('B+:tag','B+:myList', 'Rank==1', path=path)

'''
skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B+:tag'])
path.add_module(skimfilter1)
emptypath = b2.create_path()
skimfilter1.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath,b2.AfterConditionPath.END)
'''

##########################################################################
# Save B+ reco candidate outputs
# usual candidate reconstruction info
ma.variablesToNtuple('B+:tag',B_Tag_vars,treename='B_Tag', filename=file_prefix+'_mainOut.root',path=path)
# What FEI thinks the reco decay chain is
path.add_module('ParticleMCDecayString', listName='B+:tag',fileName=file_prefix+'_feiHash.root')

##########################################################################
b2.process(path,max_event)
##########################################################################
