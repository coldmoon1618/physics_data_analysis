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
output_file = outName+'_'+jobNum+'.root'

#Using setting in the note
useHad = True
useSL  = False
useBay = True

max_event = 0
path = b2.create_path()
ma.inputMdstList("default", [], path=path)

##########################################################################
# Tag side: F E I
b2.conditions.prepend_globaltag(ma.getAnalysisGlobaltag())

# Get FEI default channels.
# Utilise the arguments to toggle on and off certain channels
particles = fei.get_default_channels(
    B_extra_cut  = 'Mbc>5.1 and abs(deltaE)<0.6',
    chargedB     = True,
    neutralB     = False,
    hadronic     = useHad,
    semileptonic = useSL,
    baryonic     = useBay,
)

# Set up FEI configuration specifying the FEI prefix, monitor is for internal characteristics
configuration = fei.FeiConfiguration(prefix='FEIv4_2021_MC14_release_05_01_12', monitor=False)

# Add FEI path to the path to be processed
feistate = fei.get_path(particles, configuration)
path.add_path(feistate.path)

Btag_cuts = 'Mbc>5.2 and abs(deltaE)<0.5 and extraInfo(SignalProbability)>0.001'
ma.cutAndCopyList('B+:tag','B+:generic', Btag_cuts, path=path)

##########################################################################
# Save B+ reco candidate outputs
path.add_module('VariablesToEventBasedTree',
    particleList='B+:tag',
    variables = B_Tag_vars,
    treeName = 'B_Tag',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
b2.process(path,max_event)
##########################################################################
