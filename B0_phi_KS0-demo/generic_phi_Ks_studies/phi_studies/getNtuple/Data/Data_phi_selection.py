import basf2 as b2
import modularAnalysis as ma
from ROOT import Belle2
import glob
import sys
import vertex as vt

from stdCharged import stdK

b2.use_central_database("data_reprocessing_proc11")

if len(sys.argv) != 3:
    print('Usage: basf2 -i inputFile '+ sys.argv[0] + ' jobID  outName ')
    print('')
    print('jobID: an int indentifying the job number. It will be included in the output file name')
    print('outName: output file name, without .root. The final file name will look like: [outName]_num_[jobID].root')
    sys.exit(1)
    
jobNum = sys.argv[1]
outName = sys.argv[2] 
output_file = outName+'_'+jobNum+'.root'

path = b2.create_path()
ma.inputMdstList("default", [], path=path)

from buildVars import *
phi_Rec_vars = setPhiRecVars()

##########################################################################
# Naive 2-trk reconstruction
stdK("all", path=path)
ma.reconstructDecay('phi:reco -> K+:all K-:all', '0.97 < M < 1.1 ', path=path)
vt.treeFit('phi:reco',         conf_level=0, updateAllDaughters = True, path=path)
ma.matchMCTruth('phi:reco',    path=path)

path.add_module('VariablesToEventBasedTree', 
    particleList    ='phi:reco',
    variables       = phi_Rec_vars,
    treeName        ='phi_Rec',
    fileName        = output_file) 

##########################################################################
b2.process(path, max_event=0)
##########################################################################
