##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

from buildVars import *
B_Tag_vars = setB_TagVars()
B_MC_vars = setB_MCVars()

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
if len(sys.argv) != 4:
    print('Usage: basf2 DX_reco.py mcType BGx index')
    print('')
    sys.exit(1)

mcType  = sys.argv[1]
BGx     = sys.argv[2]
ele     = sys.argv[3]

input_list = []
input_list.append('/group/belle2/users/tenchini/prerelease-05-00-00a/'+str(mcType)+'/'+str(mcType)+'_eph3_BGx'+str(BGx)+'_'+str(ele)+'.root')

#output_file = 'output/'+str(mcType)+'_Rec_BGx'+str(BGx)+'_'+str(ele)+'.root'
output_file = 'decChain.root'
max_event = 0

path = b2.create_path()
ma.inputMdstList('default', input_list, path=path)

##########################################################################
# Save B+ truth
BplusCut  =''
ma.fillParticleListFromMC('B+',BplusCut, addDaughters = True, skipNonPrimaryDaughters = True, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList='B+',
    variables= B_MC_vars, 
    treeName = 'B_MC',
    fileName = output_file)
 
##########################################################################
b2.process(path,max_event)
##########################################################################
