import basf2 as b2
import modularAnalysis as ma
from ROOT import Belle2
import glob
import sys
import vertex as vt

from stdCharged import stdPi
from stdV0s import stdKshorts

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
Ks_Rec_vars = setKsRecVars()

##########################################################################
# Naive 2-trk reconstruction
stdPi("all", path=path)
ma.reconstructDecay('K_S0:2trk -> pi+:all pi-:all', '0.45 < M < 0.55 ', path=path)
vt.treeFit('K_S0:2trk',         conf_level=0, updateAllDaughters = True, path=path)
ma.matchMCTruth('K_S0:2trk',    path=path)

path.add_module('VariablesToEventBasedTree', 
    particleList    ='K_S0:2trk',
    variables       = Ks_Rec_vars,
    treeName        ='Ks_2trk',
    fileName        = output_file) 

##########################################################################
# Use merged K short for comparison
stdKshorts(path=path)
vt.treeFit('K_S0:merged',       conf_level=0, updateAllDaughters = True, path=path)
ma.matchMCTruth('K_S0:merged',  path=path)

path.add_module('VariablesToEventBasedTree', 
    particleList    ='K_S0:merged',
    variables       = Ks_Rec_vars,
    treeName        ='Ks_Rec',
    fileName        = output_file) 

##########################################################################
b2.process(path, max_event=0)
##########################################################################
